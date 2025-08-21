////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Sftp.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpSocket.hpp>

// clang-format off
#include <libssh2_sftp.h>
// clang-format on

#include <algorithm>
#include <array>
#include <chrono>
#include <mutex>
#include <string_view>

#include <cassert>
#include <cstring>


namespace
{
struct LibSsh2Initializer
{
    LibSsh2Initializer()
    {
        [[maybe_unused]] const auto result = libssh2_init(0);
        assert(result == 0);
    }

    ~LibSsh2Initializer()
    {
        libssh2_exit();
    }
};

struct Ssh2SessionDeleter
{
    void operator()(LIBSSH2_SESSION* ptr)
    {
        libssh2_session_free(ptr);
    }
};

struct SftpSessionDeleter
{
    void operator()(LIBSSH2_SFTP* ptr)
    {
        libssh2_sftp_shutdown(ptr);
    }
};

std::shared_ptr<LibSsh2Initializer> makeInitializer()
{
    static std::mutex     mutex;
    const std::lock_guard lock(mutex);

    static std::weak_ptr<LibSsh2Initializer> weakPtr;
    auto                                     sharedPtr = weakPtr.lock();

    if (sharedPtr)
        return sharedPtr;

    sharedPtr = std::make_shared<LibSsh2Initializer>();
    weakPtr   = sharedPtr;

    return sharedPtr;
}

int makePermissions(std::filesystem::perms permissions)
{
    int        result{};
    const auto add = [&](int i, std::filesystem::perms p)
    { result |= (((permissions & p) == std::filesystem::perms::none) ? 0 : i); };
    add(LIBSSH2_SFTP_S_IRUSR, std::filesystem::perms::owner_read);
    add(LIBSSH2_SFTP_S_IWUSR, std::filesystem::perms::owner_write);
    add(LIBSSH2_SFTP_S_IXUSR, std::filesystem::perms::owner_exec);
    add(LIBSSH2_SFTP_S_IRGRP, std::filesystem::perms::group_read);
    add(LIBSSH2_SFTP_S_IWGRP, std::filesystem::perms::group_write);
    add(LIBSSH2_SFTP_S_IXGRP, std::filesystem::perms::group_exec);
    add(LIBSSH2_SFTP_S_IROTH, std::filesystem::perms::others_read);
    add(LIBSSH2_SFTP_S_IWOTH, std::filesystem::perms::others_write);
    add(LIBSSH2_SFTP_S_IXOTH, std::filesystem::perms::others_exec);
    return result;
}

sf::Sftp::Attributes makeAttributes(std::filesystem::path path, const LIBSSH2_SFTP_ATTRIBUTES& input)
{
    sf::Sftp::Attributes attributes{};
    attributes.path = std::move(path);

    if (input.flags & LIBSSH2_SFTP_ATTR_SIZE)
        attributes.size = input.filesize;

    if (input.flags & LIBSSH2_SFTP_ATTR_UIDGID)
    {
        attributes.userId  = input.uid;
        attributes.groupId = input.gid;
    }

    if (input.flags & LIBSSH2_SFTP_ATTR_PERMISSIONS)
    {
        auto& permissions = attributes.permissions.emplace();

        if (input.permissions & LIBSSH2_SFTP_S_IRUSR)
            permissions |= std::filesystem::perms::owner_read;
        if (input.permissions & LIBSSH2_SFTP_S_IWUSR)
            permissions |= std::filesystem::perms::owner_write;
        if (input.permissions & LIBSSH2_SFTP_S_IXUSR)
            permissions |= std::filesystem::perms::owner_exec;
        if (input.permissions & LIBSSH2_SFTP_S_IRGRP)
            permissions |= std::filesystem::perms::group_read;
        if (input.permissions & LIBSSH2_SFTP_S_IWGRP)
            permissions |= std::filesystem::perms::group_write;
        if (input.permissions & LIBSSH2_SFTP_S_IXGRP)
            permissions |= std::filesystem::perms::group_exec;
        if (input.permissions & LIBSSH2_SFTP_S_IROTH)
            permissions |= std::filesystem::perms::others_read;
        if (input.permissions & LIBSSH2_SFTP_S_IWOTH)
            permissions |= std::filesystem::perms::others_write;
        if (input.permissions & LIBSSH2_SFTP_S_IXOTH)
            permissions |= std::filesystem::perms::others_exec;

        auto& type = attributes.type.emplace();

        if ((input.permissions & LIBSSH2_SFTP_S_IFMT) == LIBSSH2_SFTP_S_IFIFO)
            type = std::filesystem::file_type::fifo;
        if ((input.permissions & LIBSSH2_SFTP_S_IFMT) == LIBSSH2_SFTP_S_IFCHR)
            type = std::filesystem::file_type::character;
        if ((input.permissions & LIBSSH2_SFTP_S_IFMT) == LIBSSH2_SFTP_S_IFDIR)
            type = std::filesystem::file_type::directory;
        if ((input.permissions & LIBSSH2_SFTP_S_IFMT) == LIBSSH2_SFTP_S_IFBLK)
            type = std::filesystem::file_type::block;
        if ((input.permissions & LIBSSH2_SFTP_S_IFMT) == LIBSSH2_SFTP_S_IFREG)
            type = std::filesystem::file_type::regular;
        if ((input.permissions & LIBSSH2_SFTP_S_IFMT) == LIBSSH2_SFTP_S_IFLNK)
            type = std::filesystem::file_type::symlink;
        if ((input.permissions & LIBSSH2_SFTP_S_IFMT) == LIBSSH2_SFTP_S_IFSOCK)
            type = std::filesystem::file_type::socket;
    }

    if (input.flags & LIBSSH2_SFTP_ATTR_ACMODTIME)
    {
        attributes.accessTime = std::filesystem::file_time_type(
            std::chrono::system_clock::from_time_t(static_cast<std::time_t>(input.atime)).time_since_epoch());
        attributes.modificationTime = std::filesystem::file_time_type(
            std::chrono::system_clock::from_time_t(static_cast<std::time_t>(input.mtime)).time_since_epoch());
    }

    return attributes;
}

sf::Sftp::Result makeSftpError(LIBSSH2_SFTP* sftp)
{
    using Value = sf::Sftp::Result::Value;

    if (!sftp)
        return sf::Sftp::Result(Value::SftpError, "General error");

    switch (libssh2_sftp_last_error(sftp))
    {
        case LIBSSH2_FX_EOF:
            return sf::Sftp::Result(Value::SftpEndOfFile, "End of file");
        case LIBSSH2_FX_NO_SUCH_FILE:
            return sf::Sftp::Result(Value::SftpNoSuchFile, "No such file");
        case LIBSSH2_FX_PERMISSION_DENIED:
            return sf::Sftp::Result(Value::SftpPermissionDenied, "Permission denied");
        case LIBSSH2_FX_FAILURE:
            return sf::Sftp::Result(Value::SftpFailure, "Failure");
        case LIBSSH2_FX_BAD_MESSAGE:
            return sf::Sftp::Result(Value::SftpBadMessage, "Bad message");
        case LIBSSH2_FX_NO_CONNECTION:
            return sf::Sftp::Result(Value::SftpNoConnection, "No connection");
        case LIBSSH2_FX_CONNECTION_LOST:
            return sf::Sftp::Result(Value::SftpConnectionLost, "Connection lost");
        case LIBSSH2_FX_OP_UNSUPPORTED:
            return sf::Sftp::Result(Value::SftpOperationUnsupported, "Operation unsupported");
        case LIBSSH2_FX_INVALID_HANDLE:
            return sf::Sftp::Result(Value::SftpInvalidHandle, "Invalid handle");
        case LIBSSH2_FX_NO_SUCH_PATH:
            return sf::Sftp::Result(Value::SftpNoSuchPath, "No such path");
        case LIBSSH2_FX_FILE_ALREADY_EXISTS:
            return sf::Sftp::Result(Value::SftpFileAlreadyExists, "File already exists");
        case LIBSSH2_FX_WRITE_PROTECT:
            return sf::Sftp::Result(Value::SftpWriteProtect, "Write protect");
        case LIBSSH2_FX_NO_MEDIA:
            return sf::Sftp::Result(Value::SftpNoMedia, "No media");
        case LIBSSH2_FX_NO_SPACE_ON_FILESYSTEM:
            return sf::Sftp::Result(Value::SftpNoSpaceOnFileSystem, "No space on filesystem");
        case LIBSSH2_FX_QUOTA_EXCEEDED:
            return sf::Sftp::Result(Value::SftpQuotaExceeded, "Quota exceeded");
        case LIBSSH2_FX_UNKNOWN_PRINCIPAL:
            return sf::Sftp::Result(Value::SftpUnknownPrincipal, "Unknown principal");
        case LIBSSH2_FX_LOCK_CONFLICT:
            return sf::Sftp::Result(Value::SftpLockConflict, "Lock conflict");
        case LIBSSH2_FX_DIR_NOT_EMPTY:
            return sf::Sftp::Result(Value::SftpDirectoryNotEmpty, "Directory not empty");
        case LIBSSH2_FX_NOT_A_DIRECTORY:
            return sf::Sftp::Result(Value::SftpNotADirectory, "Not a directory");
        case LIBSSH2_FX_INVALID_FILENAME:
            return sf::Sftp::Result(Value::SftpInvalidFilename, "Invalid filename");
        case LIBSSH2_FX_LINK_LOOP:
            return sf::Sftp::Result(Value::SftpLinkLoop, "Link loop");
        default:
            return sf::Sftp::Result(Value::SftpError, "General error");
    }
}

sf::Sftp::Result makeError(LIBSSH2_SESSION* session, LIBSSH2_SFTP* sftp = nullptr)
{
    using Value = sf::Sftp::Result::Value;

    char*      ptr{};
    auto       length = 0;
    const auto error  = libssh2_session_last_error(session, &ptr, &length, 0);
    const auto size   = static_cast<std::size_t>(length);

    switch (error)
    {
        case LIBSSH2_ERROR_BANNER_RECV:
            return sf::Sftp::Result(Value::SshErrorBannerReceive, {ptr, size});
        case LIBSSH2_ERROR_BANNER_SEND:
            return sf::Sftp::Result(Value::SshErrorBannerSend, {ptr, size});
        case LIBSSH2_ERROR_INVALID_MAC:
            return sf::Sftp::Result(Value::SshErrorInvalidMac, {ptr, size});
        case LIBSSH2_ERROR_KEX_FAILURE:
            return sf::Sftp::Result(Value::SshErrorKeyExchangeFailure, {ptr, size});
        case LIBSSH2_ERROR_ALLOC:
            return sf::Sftp::Result(Value::SshErrorAllocation, {ptr, size});
        case LIBSSH2_ERROR_SOCKET_SEND:
            return sf::Sftp::Result(Value::SshErrorSocketSend, {ptr, size});
        case LIBSSH2_ERROR_KEY_EXCHANGE_FAILURE:
            return sf::Sftp::Result(Value::SshErrorKeyExchangeFailure, {ptr, size});
        case LIBSSH2_ERROR_TIMEOUT:
            return sf::Sftp::Result(Value::Timeout, {ptr, size});
        case LIBSSH2_ERROR_HOSTKEY_INIT:
            return sf::Sftp::Result(Value::SshErrorHostKeyInitialization, {ptr, size});
        case LIBSSH2_ERROR_HOSTKEY_SIGN:
            return sf::Sftp::Result(Value::SshErrorHostKeySign, {ptr, size});
        case LIBSSH2_ERROR_DECRYPT:
            return sf::Sftp::Result(Value::SshErrorDecrypt, {ptr, size});
        case LIBSSH2_ERROR_SOCKET_DISCONNECT:
            return sf::Sftp::Result(Value::Disconnected, {ptr, size});
        case LIBSSH2_ERROR_PROTO:
            return sf::Sftp::Result(Value::SshErrorProtocol, {ptr, size});
        case LIBSSH2_ERROR_PASSWORD_EXPIRED:
            return sf::Sftp::Result(Value::SshErrorPasswordExpired, {ptr, size});
        case LIBSSH2_ERROR_FILE:
            return sf::Sftp::Result(Value::SshErrorFile, {ptr, size});
        case LIBSSH2_ERROR_METHOD_NONE:
            return sf::Sftp::Result(Value::SshErrorMethodNone, {ptr, size});
        case LIBSSH2_ERROR_AUTHENTICATION_FAILED:
            return sf::Sftp::Result(Value::SshErrorAuthenticationFailed, {ptr, size});
        case LIBSSH2_ERROR_PUBLICKEY_UNVERIFIED:
            return sf::Sftp::Result(Value::SshErrorPublicKeyUnverified, {ptr, size});
        case LIBSSH2_ERROR_CHANNEL_OUTOFORDER:
            return sf::Sftp::Result(Value::SshErrorChannelOutOfOrder, {ptr, size});
        case LIBSSH2_ERROR_CHANNEL_FAILURE:
            return sf::Sftp::Result(Value::SshErrorChannelFailure, {ptr, size});
        case LIBSSH2_ERROR_CHANNEL_REQUEST_DENIED:
            return sf::Sftp::Result(Value::SshErrorChannelRequestDenied, {ptr, size});
        case LIBSSH2_ERROR_CHANNEL_UNKNOWN:
            return sf::Sftp::Result(Value::SshErrorChannelUnknown, {ptr, size});
        case LIBSSH2_ERROR_CHANNEL_WINDOW_EXCEEDED:
            return sf::Sftp::Result(Value::SshErrorChannelWindowExceeded, {ptr, size});
        case LIBSSH2_ERROR_CHANNEL_PACKET_EXCEEDED:
            return sf::Sftp::Result(Value::SshErrorChannelPacketExceeded, {ptr, size});
        case LIBSSH2_ERROR_CHANNEL_CLOSED:
            return sf::Sftp::Result(Value::SshErrorChannelClosed, {ptr, size});
        case LIBSSH2_ERROR_CHANNEL_EOF_SENT:
            return sf::Sftp::Result(Value::SshErrorChannelEofSent, {ptr, size});
        case LIBSSH2_ERROR_SCP_PROTOCOL:
            return sf::Sftp::Result(Value::SshErrorScpProtocol, {ptr, size});
        case LIBSSH2_ERROR_ZLIB:
            return sf::Sftp::Result(Value::SshErrorZlib, {ptr, size});
        case LIBSSH2_ERROR_SOCKET_TIMEOUT:
            return sf::Sftp::Result(Value::Timeout, {ptr, size});
        case LIBSSH2_ERROR_SFTP_PROTOCOL:
            return makeSftpError(sftp);
        case LIBSSH2_ERROR_REQUEST_DENIED:
            return sf::Sftp::Result(Value::SshErrorRequestDenied, {ptr, size});
        case LIBSSH2_ERROR_METHOD_NOT_SUPPORTED:
            return sf::Sftp::Result(Value::SshErrorMethodNotSupported, {ptr, size});
        case LIBSSH2_ERROR_INVAL:
            return sf::Sftp::Result(Value::SshErrorInvalid, {ptr, size});
        case LIBSSH2_ERROR_PUBLICKEY_PROTOCOL:
            return sf::Sftp::Result(Value::SshErrorPublicKeyProtocol, {ptr, size});
        case LIBSSH2_ERROR_BUFFER_TOO_SMALL:
            return sf::Sftp::Result(Value::SshErrorBufferTooSmall, {ptr, size});
        case LIBSSH2_ERROR_BAD_USE:
            return sf::Sftp::Result(Value::SshErrorBadUse, {ptr, size});
        case LIBSSH2_ERROR_COMPRESS:
            return sf::Sftp::Result(Value::SshErrorCompress, {ptr, size});
        case LIBSSH2_ERROR_OUT_OF_BOUNDARY:
            return sf::Sftp::Result(Value::SshErrorOutOfBoundary, {ptr, size});
        case LIBSSH2_ERROR_AGENT_PROTOCOL:
            return sf::Sftp::Result(Value::SshErrorAgentProtocol, {ptr, size});
        case LIBSSH2_ERROR_SOCKET_RECV:
            return sf::Sftp::Result(Value::SshErrorSocketRecv, {ptr, size});
        case LIBSSH2_ERROR_ENCRYPT:
            return sf::Sftp::Result(Value::SshErrorEncrypt, {ptr, size});
        case LIBSSH2_ERROR_BAD_SOCKET:
            return sf::Sftp::Result(Value::SshErrorBadSocket, {ptr, size});
        case LIBSSH2_ERROR_KNOWN_HOSTS:
            return sf::Sftp::Result(Value::SshErrorKnownHosts, {ptr, size});
        case LIBSSH2_ERROR_CHANNEL_WINDOW_FULL:
            return sf::Sftp::Result(Value::SshErrorChannelWindowFull, {ptr, size});
        case LIBSSH2_ERROR_KEYFILE_AUTH_FAILED:
            return sf::Sftp::Result(Value::SshErrorKeyFileAuthenticationFailed, {ptr, size});
        case LIBSSH2_ERROR_RANDGEN:
            return sf::Sftp::Result(Value::SshErrorRandomNumberGenerator, {ptr, size});
        case LIBSSH2_ERROR_MISSING_USERAUTH_BANNER:
            return sf::Sftp::Result(Value::SshErrorMissingUserAuthenticationBanner, {ptr, size});
        case LIBSSH2_ERROR_ALGO_UNSUPPORTED:
            return sf::Sftp::Result(Value::SshErrorAlgorithmUnsupported, {ptr, size});
        default:
            return sf::Sftp::Result(Value::Error, {ptr, size});
    }
}
} // namespace


namespace sf
{
struct Sftp::Impl
{
    Impl()
    {
        socket.setBlocking(false);
    }

    template <typename Callable>
    int waitForOperationComplete(const Callable& operation, const sf::Sftp::TimeoutPredicate& timeout)
    {
        while (true)
        {
            if (const auto result = operation(); result != LIBSSH2_ERROR_EAGAIN)
                return result; // Operation completed without having to block

            // Wait for socket to become ready
            const auto directions = libssh2_session_block_directions(ssh2Session.get());
            if (directions == (LIBSSH2_SESSION_BLOCK_INBOUND | LIBSSH2_SESSION_BLOCK_OUTBOUND))
            {
                if (selectorEither.wait(timeout.getPeriod()))
                    continue;
            }
            else if (directions == LIBSSH2_SESSION_BLOCK_OUTBOUND)
            {
                if (selectorSend.wait(timeout.getPeriod()))
                    continue;
            }
            else
            {
                if (selectorRecv.wait(timeout.getPeriod()))
                    continue;
            }

            if (!timeout.getPredicate()())
                return LIBSSH2_ERROR_EAGAIN; // Gave up waiting
        }
    }

    static ssize_t handleSend(libssh2_socket_t, const void* buffer, size_t length, int, void** implPtr)
    {
        auto&       impl = *static_cast<Impl*>(*implPtr);
        std::size_t sent{};
        const auto  result = impl.socket.send(buffer, length, sent);

        // Map SFML-specific return values back to POSIX return values
        if (result == Socket::Status::Done)
            return static_cast<ssize_t>(sent);

        if (result == Socket::Status::Partial)
        {
            if (sent > 0)
                return static_cast<ssize_t>(sent);
            return -EAGAIN;
        }

        if (result == Socket::Status::NotReady)
            return -EAGAIN;

        if (result == Socket::Status::Disconnected)
            return 0;

        // Socket::Status::Error
        return -1;
    }

    static ssize_t handleReceive(libssh2_socket_t, void* buffer, size_t length, int, void** implPtr)
    {
        auto&       impl = *static_cast<Impl*>(*implPtr);
        std::size_t received{};
        const auto  result = impl.socket.receive(buffer, length, received);

        // Map SFML-specific return values back to POSIX return values
        if (result == Socket::Status::Done)
            return static_cast<ssize_t>(received);

        if (result == Socket::Status::Partial)
        {
            if (received > 0)
                return static_cast<ssize_t>(received);
            return -EAGAIN;
        }

        if (result == Socket::Status::NotReady)
            return -EAGAIN;

        if (result == Socket::Status::Disconnected)
            return 0;

        // Socket::Status::Error
        return -1;
    }

    std::shared_ptr<LibSsh2Initializer>                  initializer          = makeInitializer();
    bool                                                 posixRenameSupported = true;
    TcpSocket                                            socket;
    sf::SocketSelector                                   selectorRecv;
    sf::SocketSelector                                   selectorSend;
    sf::SocketSelector                                   selectorEither;
    std::unique_ptr<LIBSSH2_SESSION, Ssh2SessionDeleter> ssh2Session;
    std::unique_ptr<LIBSSH2_SFTP, SftpSessionDeleter>    sftpSession;
};


////////////////////////////////////////////////////////////
Sftp::Result::Result(Value value, std::string message) : m_value(value), m_message(std::move(message))
{
}


////////////////////////////////////////////////////////////
bool Sftp::Result::isOk() const
{
    return m_value == Value::Success;
}


////////////////////////////////////////////////////////////
Sftp::Result::Value Sftp::Result::getValue() const
{
    return m_value;
}


////////////////////////////////////////////////////////////
const std::string& Sftp::Result::getMessage() const
{
    return m_message;
}


////////////////////////////////////////////////////////////
Sftp::PathResult::PathResult(const Result& result, std::filesystem::path path) : Sftp::Result(result)
{
    if (isOk())
        m_path = std::move(path);
}


////////////////////////////////////////////////////////////
const std::filesystem::path& Sftp::PathResult::getPath() const
{
    return m_path;
}


////////////////////////////////////////////////////////////
Sftp::AttributesResult::AttributesResult(const Result& result, Attributes attributes) : Sftp::Result(result)
{
    if (isOk())
        m_attributes = std::move(attributes);
}


////////////////////////////////////////////////////////////
const Sftp::Attributes& Sftp::AttributesResult::getAttributes() const
{
    return m_attributes;
}


////////////////////////////////////////////////////////////
Sftp::ListingResult::ListingResult(const Result& result, std::vector<Attributes> listing) : Sftp::Result(result)
{
    if (isOk())
        m_listing = std::move(listing);
}


////////////////////////////////////////////////////////////
const std::vector<Sftp::Attributes>& Sftp::ListingResult::getListing() const
{
    return m_listing;
}


////////////////////////////////////////////////////////////
Sftp::TimeoutPredicate::TimeoutPredicate(Time timeout) : m_predicate([] { return false; }), m_period(timeout)
{
}


////////////////////////////////////////////////////////////
Sftp::TimeoutPredicate::TimeoutPredicate(std::function<bool()> predicate, Time period) :
    m_predicate(std::move(predicate)),
    m_period(period)
{
    if (!m_predicate)
        m_predicate = [] { return false; };
}


////////////////////////////////////////////////////////////
const std::function<bool()>& Sftp::TimeoutPredicate::getPredicate() const
{
    return m_predicate;
}


////////////////////////////////////////////////////////////
const Time& Sftp::TimeoutPredicate::getPeriod() const
{
    return m_period;
}


////////////////////////////////////////////////////////////
Sftp::Sftp() : m_impl(std::make_unique<Impl>())
{
}


////////////////////////////////////////////////////////////
Sftp::~Sftp()
{
    (void)disconnect();
}


////////////////////////////////////////////////////////////
Sftp::Result Sftp::connect(IpAddress server, unsigned short port, const TimeoutPredicate& timeout)
{
    (void)disconnect(timeout);

    if (const auto status = m_impl->socket.connect(server, port);
        (status == Socket::Status::Error) || (status == Socket::Status::Disconnected))
        return Result(Result::Value::Disconnected);

    m_impl->selectorRecv.clear();
    m_impl->selectorSend.clear();
    m_impl->selectorEither.clear();
    m_impl->selectorRecv.add(m_impl->socket, sf::SocketSelector::ReadinessType::Receive);
    m_impl->selectorSend.add(m_impl->socket, sf::SocketSelector::ReadinessType::Send);
    m_impl->selectorEither.add(m_impl->socket, sf::SocketSelector::ReadinessType::Either);

    // Wait for the socket to become connected
    while (true)
    {
        if (m_impl->selectorSend.wait(timeout.getPeriod()))
            break;

        if (!timeout.getPredicate()())
        {
            m_impl->selectorRecv.clear();
            m_impl->selectorSend.clear();
            m_impl->selectorEither.clear();
            m_impl->socket.disconnect();
            return Result(Result::Value::Timeout);
        }
    }

    // Check if our connection was actually accepted or refused
    if (!m_impl->socket.getRemoteAddress().has_value())
    {
        m_impl->selectorRecv.clear();
        m_impl->selectorSend.clear();
        m_impl->selectorEither.clear();
        m_impl->socket.disconnect();
        return Result(Result::Value::Refused);
    }

    // At this point the TCP connection is established

    // Reset POSIX rename supported flag to assume true for new connections
    m_impl->posixRenameSupported = true;

    // Set up SSH session
    m_impl->ssh2Session.reset(libssh2_session_init_ex(nullptr, nullptr, nullptr, m_impl.get()));
    assert(m_impl->ssh2Session);

    // Tell libssh2 we will take care of waiting on socket readiness ourselves
    libssh2_session_set_blocking(m_impl->ssh2Session.get(), 0);

    // Set send and receive callbacks to make use of TcpSocket as the underlying transport
#if (LIBSSH2_VERSION_NUM >= 0x010b01)
    libssh2_session_callback_set2(m_impl->ssh2Session.get(),
                                  LIBSSH2_CALLBACK_SEND,
                                  reinterpret_cast<libssh2_cb_generic*>(Impl::handleSend));
    libssh2_session_callback_set2(m_impl->ssh2Session.get(),
                                  LIBSSH2_CALLBACK_RECV,
                                  reinterpret_cast<libssh2_cb_generic*>(Impl::handleReceive));
#else
    libssh2_session_callback_set(m_impl->ssh2Session.get(), LIBSSH2_CALLBACK_SEND, reinterpret_cast<void*>(&Impl::handleSend));
    libssh2_session_callback_set(m_impl->ssh2Session.get(),
                                 LIBSSH2_CALLBACK_RECV,
                                 reinterpret_cast<void*>(&Impl::handleReceive));
#endif

    // Block SIGPIPE
    if (const auto result = libssh2_session_flag(m_impl->ssh2Session.get(), LIBSSH2_FLAG_SIGPIPE, 0); result < 0)
        return Result(Result::Value::Error);

    // Enable compression if available
    if (const auto result = libssh2_session_flag(m_impl->ssh2Session.get(), LIBSSH2_FLAG_COMPRESS, 1); result < 0)
        return Result(Result::Value::Error);

    // Perform handshake
    const auto
        result = m_impl->waitForOperationComplete([&] { return libssh2_session_handshake(m_impl->ssh2Session.get(), 0); },
                                                  timeout);

    if (result == LIBSSH2_ERROR_EAGAIN)
        return Result(Result::Value::Timeout);

    if (result < 0)
        return makeError(m_impl->ssh2Session.get());

    return Result(Result::Value::Success);
}


////////////////////////////////////////////////////////////
Sftp::Result Sftp::disconnect(const TimeoutPredicate& timeout)
{
    // Shut down the SFTP session
    if (m_impl->sftpSession)
    {
        auto* sftpSession = m_impl->sftpSession.release();
        const auto result = m_impl->waitForOperationComplete([&] { return libssh2_sftp_shutdown(sftpSession); }, timeout);

        if (result == LIBSSH2_ERROR_EAGAIN)
            return Result(Result::Value::Timeout);
    }

    // Disconnect the SSH session
    if (m_impl->ssh2Session)
    {
        const auto result = m_impl->waitForOperationComplete(
            [&]
            { return libssh2_session_disconnect_ex(m_impl->ssh2Session.get(), SSH_DISCONNECT_BY_APPLICATION, "", ""); },
            timeout);

        if (result == LIBSSH2_ERROR_EAGAIN)
            return Result(Result::Value::Timeout);

        // Continue with destroying the SSH session and disconnecting the socket even if an error occurred
    }

    // Destroy the SSH session
    m_impl->ssh2Session.reset();

    // Disconnect the TCP connection
    m_impl->selectorRecv.clear();
    m_impl->selectorSend.clear();
    m_impl->selectorEither.clear();
    m_impl->socket.disconnect();

    return Result(Result::Value::Success);
}


////////////////////////////////////////////////////////////
std::optional<Sftp::SessionInfo> Sftp::getSessionInfo() const
{
    if (!m_impl->ssh2Session)
        return std::nullopt;

    SessionInfo sessionInfo;

    {
        std::size_t       length{};
        auto              type = 0;
        const auto* const ptr  = libssh2_session_hostkey(m_impl->ssh2Session.get(), &length, &type);

        if (length > 0)
        {
            SessionInfo::HostKey hostKey;
            hostKey.data.resize(length);
            std::memcpy(hostKey.data.data(), ptr, length);

            switch (type)
            {
                case LIBSSH2_HOSTKEY_TYPE_RSA:
                    hostKey.type = SessionInfo::HostKey::Type::Rsa;
                    break;
                case LIBSSH2_HOSTKEY_TYPE_DSS:
                    hostKey.type = SessionInfo::HostKey::Type::Dsa;
                    break;
                case LIBSSH2_HOSTKEY_TYPE_ECDSA_256:
                    hostKey.type = SessionInfo::HostKey::Type::Ecdsa256;
                    break;
                case LIBSSH2_HOSTKEY_TYPE_ECDSA_384:
                    hostKey.type = SessionInfo::HostKey::Type::Ecdsa384;
                    break;
                case LIBSSH2_HOSTKEY_TYPE_ECDSA_521:
                    hostKey.type = SessionInfo::HostKey::Type::Ecdsa521;
                    break;
                case LIBSSH2_HOSTKEY_TYPE_ED25519:
                    hostKey.type = SessionInfo::HostKey::Type::Ed25519;
                    break;
                default:
                    hostKey.type = SessionInfo::HostKey::Type::Unknown;
                    break;
            }

            if (const auto* const hash = libssh2_hostkey_hash(m_impl->ssh2Session.get(), LIBSSH2_HOSTKEY_HASH_SHA1); hash)
                std::memcpy(hostKey.sha1.data(), hash, hostKey.sha1.size());

            if (const auto* const hash = libssh2_hostkey_hash(m_impl->ssh2Session.get(), LIBSSH2_HOSTKEY_HASH_SHA256); hash)
                std::memcpy(hostKey.sha256.data(), hash, hostKey.sha256.size());

            sessionInfo.hostKey = std::move(hostKey);
        }
        else
        {
            return std::nullopt;
        }
    }

    const auto getSessionMethod = [session = m_impl->ssh2Session.get()](int method) -> std::string
    {
        if (!session)
            return "";
        if (const auto* const result = libssh2_session_methods(session, method); result)
            return result;
        return "";
    };

    sessionInfo.keyExchangeAlgorithm               = getSessionMethod(LIBSSH2_METHOD_KEX);
    sessionInfo.hostKeyAlgorithm                   = getSessionMethod(LIBSSH2_METHOD_HOSTKEY);
    sessionInfo.clientToServerEncryptionAlgorithm  = getSessionMethod(LIBSSH2_METHOD_CRYPT_CS);
    sessionInfo.serverToClientEncryptionAlgorithm  = getSessionMethod(LIBSSH2_METHOD_CRYPT_SC);
    sessionInfo.clientToServerMacAlgorithm         = getSessionMethod(LIBSSH2_METHOD_MAC_CS);
    sessionInfo.serverToClientMacAlgorithm         = getSessionMethod(LIBSSH2_METHOD_MAC_SC);
    sessionInfo.clientToServerCompressionAlgorithm = getSessionMethod(LIBSSH2_METHOD_COMP_CS);
    sessionInfo.serverToClientCompressionAlgorithm = getSessionMethod(LIBSSH2_METHOD_COMP_SC);

    return sessionInfo;
}


////////////////////////////////////////////////////////////
Sftp::Result Sftp::login(const std::string& name, const std::string& password, const TimeoutPredicate& timeout)
{
    // Perform login
    {
        const auto result = m_impl->waitForOperationComplete(
            [&]
            {
                return libssh2_userauth_password_ex(m_impl->ssh2Session.get(),
                                                    name.data(),
                                                    static_cast<unsigned int>(name.size()),
                                                    password.data(),
                                                    static_cast<unsigned int>(password.size()),
                                                    nullptr);
            },
            timeout);

        if (result == LIBSSH2_ERROR_EAGAIN)
            return Result(Result::Value::Timeout);

        if (result < 0)
            return makeError(m_impl->ssh2Session.get());
    }

    // Initialize SFTP session
    {
        const auto result = m_impl->waitForOperationComplete(
            [&]
            {
                m_impl->sftpSession.reset(libssh2_sftp_init(m_impl->ssh2Session.get()));

                if (m_impl->sftpSession)
                    return LIBSSH2_ERROR_NONE;

                return libssh2_session_last_errno(m_impl->ssh2Session.get());
            },
            timeout);

        if (result == LIBSSH2_ERROR_EAGAIN)
            return Result(Result::Value::Timeout);

        if (result < 0)
            return makeError(m_impl->ssh2Session.get());
    }

    return Result(Result::Value::Success);
}


////////////////////////////////////////////////////////////
Sftp::Result Sftp::login(const std::string&      name,
                         const char*             publicKeyData,
                         std::size_t             publicKeyLength,
                         const char*             privateKeyData,
                         std::size_t             privateKeyLength,
                         const char*             privateKeyPassphrase,
                         const TimeoutPredicate& timeout)
{
    // Perform login
    {
        const auto result = m_impl->waitForOperationComplete(
            [&]
            {
                return libssh2_userauth_publickey_frommemory(m_impl->ssh2Session.get(),
                                                             name.data(),
                                                             name.size(),
                                                             publicKeyData,
                                                             publicKeyLength,
                                                             privateKeyData,
                                                             privateKeyLength,
                                                             privateKeyPassphrase);
            },
            timeout);

        if (result == LIBSSH2_ERROR_EAGAIN)
            return Result(Result::Value::Timeout);

        if (result < 0)
            return makeError(m_impl->ssh2Session.get());
    }

    // Initialize SFTP session
    {
        const auto result = m_impl->waitForOperationComplete(
            [&]
            {
                m_impl->sftpSession.reset(libssh2_sftp_init(m_impl->ssh2Session.get()));

                if (m_impl->sftpSession)
                    return LIBSSH2_ERROR_NONE;

                return libssh2_session_last_errno(m_impl->ssh2Session.get());
            },
            timeout);

        if (result == LIBSSH2_ERROR_EAGAIN)
            return Result(Result::Value::Timeout);

        if (result < 0)
            return makeError(m_impl->ssh2Session.get(), m_impl->sftpSession.get());
    }

    return Result(Result::Value::Success);
}


////////////////////////////////////////////////////////////
Sftp::PathResult Sftp::resolvePath(const std::filesystem::path& path, const TimeoutPredicate& timeout)
{
    const auto             pathString = path.generic_u8string();
    std::array<char, 4096> buffer{};
    const auto             result = m_impl->waitForOperationComplete(
        [&]
        {
            return libssh2_sftp_symlink_ex(m_impl->sftpSession.get(),
                                           pathString.data(),
                                           static_cast<unsigned int>(pathString.size()),
                                           buffer.data(),
                                           static_cast<unsigned int>(buffer.size()),
                                           LIBSSH2_SFTP_REALPATH);
        },
        timeout);

    if (result == LIBSSH2_ERROR_EAGAIN)
        return {Result(Result::Value::Timeout), {}};

    if (result < 0)
        return {makeError(m_impl->ssh2Session.get(), m_impl->sftpSession.get()), {}};

    return {Result(Result::Value::Success),
            std::filesystem::path(std::string_view(buffer.data(), static_cast<std::size_t>(result))).generic_u8string()};
}


////////////////////////////////////////////////////////////
Sftp::PathResult Sftp::getWorkingDirectory(const TimeoutPredicate& timeout)
{
    return resolvePath(".", timeout);
}


////////////////////////////////////////////////////////////
Sftp::AttributesResult Sftp::getAttributes(const std::filesystem::path& path, bool followLinks, const TimeoutPredicate& timeout)
{
    LIBSSH2_SFTP_ATTRIBUTES attributes{};

    const auto pathString = path.generic_u8string();
    const auto result     = m_impl->waitForOperationComplete(
        [&]
        {
            return libssh2_sftp_stat_ex(m_impl->sftpSession.get(),
                                        pathString.data(),
                                        static_cast<unsigned int>(pathString.size()),
                                        followLinks ? LIBSSH2_SFTP_STAT : LIBSSH2_SFTP_LSTAT,
                                        &attributes);
        },
        timeout);

    if (result == LIBSSH2_ERROR_EAGAIN)
        return {Result(Result::Value::Timeout), {}};

    if (result < 0)
        return {makeError(m_impl->ssh2Session.get(), m_impl->sftpSession.get()), {}};

    return {Result(Result::Value::Success), makeAttributes(pathString, attributes)};
}


////////////////////////////////////////////////////////////
Sftp::ListingResult Sftp::getDirectoryListing(const std::filesystem::path& path, const TimeoutPredicate& timeout)
{
    LIBSSH2_SFTP_HANDLE* handle{};

    {
        const auto pathString = path.generic_u8string();
        const auto result     = m_impl->waitForOperationComplete(
            [&]
            {
                handle = libssh2_sftp_open_ex(m_impl->sftpSession.get(),
                                              pathString.data(),
                                              static_cast<unsigned int>(pathString.size()),
                                              0,
                                              0,
                                              LIBSSH2_SFTP_OPENDIR);

                if (handle)
                    return LIBSSH2_ERROR_NONE;

                return libssh2_session_last_errno(m_impl->ssh2Session.get());
            },
            timeout);

        if (result == LIBSSH2_ERROR_EAGAIN)
            return {Result(Result::Value::Timeout), {}};

        if (!handle)
            return {makeError(m_impl->ssh2Session.get(), m_impl->sftpSession.get()), {}};
    }

    std::vector<Attributes> listing;

    {
        std::array<char, 4096>  buffer{};
        LIBSSH2_SFTP_ATTRIBUTES attributes{};

        while (true)
        {
            const auto result = m_impl->waitForOperationComplete(
                [&] { return libssh2_sftp_readdir_ex(handle, buffer.data(), buffer.size(), nullptr, 0, &attributes); },
                timeout);

            if (result > 0)
            {
                listing.emplace_back(
                    makeAttributes((path / std::string_view(buffer.data(), static_cast<std::size_t>(result))).generic_u8string(),
                                   attributes));

                continue;
            }

            if (result == LIBSSH2_ERROR_EAGAIN)
            {
                libssh2_sftp_close_handle(handle);
                return {Result(Result::Value::Timeout), {}};
            }

            if (result == 0)
                break;
        }
    }

    {
        const auto result = m_impl->waitForOperationComplete([&] { return libssh2_sftp_close_handle(handle); }, timeout);

        if (result == LIBSSH2_ERROR_EAGAIN)
            return {Result(Result::Value::Timeout), {}};
    }

    return {Result(Result::Value::Success), std::move(listing)};
}


////////////////////////////////////////////////////////////
Sftp::Result Sftp::createDirectory(const std::filesystem::path& path,
                                   std::filesystem::perms       permissions,
                                   const TimeoutPredicate&      timeout)
{
    const auto pathString = path.generic_u8string();
    const auto result     = m_impl->waitForOperationComplete(
        [&]
        {
            return libssh2_sftp_mkdir_ex(m_impl->sftpSession.get(),
                                         pathString.data(),
                                         static_cast<unsigned int>(pathString.size()),
                                         makePermissions(permissions));
        },
        timeout);

    if (result == LIBSSH2_ERROR_EAGAIN)
        return Result(Result::Value::Timeout);

    if (result < 0)
        return makeError(m_impl->ssh2Session.get(), m_impl->sftpSession.get());

    return Result(Result::Value::Success);
}


////////////////////////////////////////////////////////////
Sftp::Result Sftp::deleteDirectory(const std::filesystem::path& path, const TimeoutPredicate& timeout)
{
    const auto pathString = path.generic_u8string();
    const auto result     = m_impl->waitForOperationComplete(
        [&]
        {
            return libssh2_sftp_rmdir_ex(m_impl->sftpSession.get(),
                                         pathString.data(),
                                         static_cast<unsigned int>(pathString.size()));
        },
        timeout);

    if (result == LIBSSH2_ERROR_EAGAIN)
        return Result(Result::Value::Timeout);

    if (result < 0)
        return makeError(m_impl->ssh2Session.get(), m_impl->sftpSession.get());

    return Result(Result::Value::Success);
}


////////////////////////////////////////////////////////////
Sftp::Result Sftp::rename(const std::filesystem::path& oldPath,
                          const std::filesystem::path& newPath,
                          bool                         overwrite,
                          const TimeoutPredicate&      timeout)
{
    const auto oldPathString = oldPath.generic_u8string();
    const auto newPathString = newPath.generic_u8string();

    // POSIX rename is only supported starting from libssh2 1.11.1
#if (LIBSSH2_VERSION_NUM >= 0x010b01)
    // POSIX rename doesn't support not overwriting
    auto usePosix = overwrite && m_impl->posixRenameSupported;
#endif

    while (true)
    {
        const auto result = m_impl->waitForOperationComplete(
            [&]
            {
#if (LIBSSH2_VERSION_NUM >= 0x010b01)
                if (usePosix)
                {
                    return libssh2_sftp_posix_rename_ex(m_impl->sftpSession.get(),
                                                        oldPathString.data(),
                                                        static_cast<unsigned int>(oldPathString.size()),
                                                        newPathString.data(),
                                                        static_cast<unsigned int>(newPathString.size()));
                }
#endif

                return libssh2_sftp_rename_ex(m_impl->sftpSession.get(),
                                              oldPathString.data(),
                                              static_cast<unsigned int>(oldPathString.size()),
                                              newPathString.data(),
                                              static_cast<unsigned int>(newPathString.size()),
                                              overwrite ? LIBSSH2_SFTP_RENAME_OVERWRITE : 0);
            },
            timeout);

        if (result == LIBSSH2_ERROR_EAGAIN)
            return Result(Result::Value::Timeout);

#if (LIBSSH2_VERSION_NUM >= 0x010b01)
        // Retry using normal rename if we find out POSIX rename isn't supported
        if (usePosix && (result == LIBSSH2_FX_OP_UNSUPPORTED))
        {
            m_impl->posixRenameSupported = false;
            usePosix                     = false;
            continue;
        }
#endif

        if (result < 0)
            return makeError(m_impl->ssh2Session.get(), m_impl->sftpSession.get());

        return Result(Result::Value::Success);
    }
}


////////////////////////////////////////////////////////////
Sftp::Result Sftp::deleteFile(const std::filesystem::path& path, const TimeoutPredicate& timeout)
{
    const auto pathString = path.generic_u8string();
    const auto result     = m_impl->waitForOperationComplete(
        [&]
        {
            return libssh2_sftp_unlink_ex(m_impl->sftpSession.get(),
                                          pathString.data(),
                                          static_cast<unsigned int>(pathString.size()));
        },
        timeout);

    if (result == LIBSSH2_ERROR_EAGAIN)
        return Result(Result::Value::Timeout);

    if (result < 0)
        return makeError(m_impl->ssh2Session.get(), m_impl->sftpSession.get());

    return Result(Result::Value::Success);
}


////////////////////////////////////////////////////////////
Sftp::Result Sftp::download(const std::filesystem::path&                                   remotePath,
                            const std::function<bool(const void* data, std::size_t size)>& callback,
                            std::uint64_t                                                  offset,
                            const TimeoutPredicate&                                        timeout)
{
    if (!callback)
        return Result(Result::Value::Error);

    LIBSSH2_SFTP_HANDLE*    handle{};
    LIBSSH2_SFTP_ATTRIBUTES attributes{};

    // Open the file for reading
    {
        const auto pathString = remotePath.generic_u8string();
        const auto result     = m_impl->waitForOperationComplete(
            [&]
            {
                if (handle = libssh2_sftp_open_ex_r(m_impl->sftpSession.get(),
                                                    pathString.data(),
                                                    static_cast<unsigned int>(pathString.size()),
                                                    LIBSSH2_FXF_READ,
                                                    0,
                                                    LIBSSH2_SFTP_OPENFILE,
                                                    &attributes);
                    handle)
                    return LIBSSH2_ERROR_NONE;

                return libssh2_session_last_errno(m_impl->ssh2Session.get());
            },
            timeout);

        if (result == LIBSSH2_ERROR_EAGAIN)
            return Result(Result::Value::Timeout);

        if (!handle)
            return makeError(m_impl->ssh2Session.get(), m_impl->sftpSession.get());
    }

    // Seek to the offset if one was specified
    if (offset > 0)
        libssh2_sftp_seek64(handle, offset);

    // Read from the file
    {
        static constexpr auto bufferSizeStart = std::size_t{4 * 1024};
        static constexpr auto bufferSizeMax   = std::size_t{1024 * 1024};
        auto                  bufferSize      = bufferSizeStart;
        auto buffer = std::make_unique<char[]>(bufferSize); // NOLINT(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)

        while (true)
        {
            std::size_t length = bufferSize;
            auto*       ptr    = buffer.get();
            int         read{};
            std::size_t totalRead{};

            while (length > 0)
            {
                read = m_impl->waitForOperationComplete(
                    [&] { return static_cast<int>(libssh2_sftp_read(handle, ptr, length)); },
                    timeout);

                if (read == LIBSSH2_ERROR_EAGAIN)
                {
                    libssh2_sftp_close_handle(handle);
                    return Result(Result::Value::Timeout);
                }

                if (read <= 0)
                    break;

                ptr += read;
                length -= static_cast<std::size_t>(read);
                totalRead += static_cast<std::size_t>(read);
            }

            if (read < 0)
            {
                // Get the error before calling libssh2_sftp_close_handle since it could get overwritten
                const auto result = makeError(m_impl->ssh2Session.get(), m_impl->sftpSession.get());
                libssh2_sftp_close_handle(handle);
                return result;
            }

            const auto stop = !callback(buffer.get(), totalRead);

            // If read is 0 then we have reached EOF
            if (stop || (read == 0))
                break;

            const auto expand = (totalRead == bufferSize);

            if (expand && (bufferSize < bufferSizeMax))
            {
                bufferSize = std::min(bufferSize * 3 / 2, bufferSizeMax); // Use a growth factor of 1.5
                buffer = std::make_unique<char[]>(bufferSize); // NOLINT(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
            }
        }
    }

    // Close the file
    {
        const auto result = m_impl->waitForOperationComplete([&] { return libssh2_sftp_close_handle(handle); }, timeout);

        if (result == LIBSSH2_ERROR_EAGAIN)
            return Result(Result::Value::Timeout);

        if (result < 0)
            return makeError(m_impl->ssh2Session.get(), m_impl->sftpSession.get());
    }

    return Result(Result::Value::Success);
}


////////////////////////////////////////////////////////////
Sftp::Result Sftp::upload(const std::filesystem::path&                              remotePath,
                          const std::function<bool(void* data, std::size_t& size)>& callback,
                          std::filesystem::perms                                    permissions,
                          bool                                                      truncate,
                          bool                                                      append,
                          std::uint64_t                                             offset,
                          const TimeoutPredicate&                                   timeout)
{
    if (!callback)
        return Result(Result::Value::Error);

    LIBSSH2_SFTP_HANDLE*    handle{};
    LIBSSH2_SFTP_ATTRIBUTES attributes{};

    // Open the file for writing
    {
        const auto pathString = remotePath.generic_u8string();
        const auto result     = m_impl->waitForOperationComplete(
            [&]
            {
                if (handle = libssh2_sftp_open_ex_r(m_impl->sftpSession.get(),
                                                    pathString.data(),
                                                    static_cast<unsigned int>(pathString.size()),
                                                    LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT |
                                                        (truncate ? LIBSSH2_FXF_TRUNC : 0) |
                                                        (append ? LIBSSH2_FXF_APPEND : 0),
                                                    makePermissions(permissions),
                                                    LIBSSH2_SFTP_OPENFILE,
                                                    &attributes);
                    handle)
                    return LIBSSH2_ERROR_NONE;

                return libssh2_session_last_errno(m_impl->ssh2Session.get());
            },
            timeout);

        if (!handle && (result == LIBSSH2_ERROR_EAGAIN))
            return Result(Result::Value::Timeout);

        if (!handle)
            return makeError(m_impl->ssh2Session.get(), m_impl->sftpSession.get());
    }

    // Seek to the offset if one was specified
    if (offset > 0)
        libssh2_sftp_seek64(handle, offset);

    // Write to the file
    {
        static constexpr auto bufferSizeStart = std::size_t{4 * 1024};
        static constexpr auto bufferSizeMax   = std::size_t{1024 * 1024};
        auto                  bufferSize      = bufferSizeStart;
        auto buffer = std::make_unique<char[]>(bufferSize); // NOLINT(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)

        while (true)
        {
            std::size_t length = bufferSize;
            const auto  stop   = !callback(buffer.get(), length);
            const auto  expand = (length == bufferSize);
            const auto* ptr    = buffer.get();
            int         written{};

            while (length > 0)
            {
                written = m_impl->waitForOperationComplete(
                    [&] { return static_cast<int>(libssh2_sftp_write(handle, ptr, length)); },
                    timeout);

                if (written == LIBSSH2_ERROR_EAGAIN)
                {
                    libssh2_sftp_close_handle(handle);
                    return Result(Result::Value::Timeout);
                }

                if (written < 0)
                    break;

                ptr += written;
                length -= static_cast<std::size_t>(written);
            }

            if (written < 0)
            {
                // Get the error before calling libssh2_sftp_close_handle since it could get overwritten
                const auto result = makeError(m_impl->ssh2Session.get(), m_impl->sftpSession.get());
                libssh2_sftp_close_handle(handle);
                return result;
            }

            if (stop)
                break;

            if (expand && (bufferSize < bufferSizeMax))
            {
                bufferSize = std::min(bufferSize * 3 / 2, bufferSizeMax); // Use a growth factor of 1.5
                buffer = std::make_unique<char[]>(bufferSize); // NOLINT(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
            }
        }
    }

    // Close the file
    {
        const auto result = m_impl->waitForOperationComplete([&] { return libssh2_sftp_close_handle(handle); }, timeout);

        if (result == LIBSSH2_ERROR_EAGAIN)
            return Result(Result::Value::Timeout);

        if (result < 0)
            return makeError(m_impl->ssh2Session.get(), m_impl->sftpSession.get());
    }

    return Result(Result::Value::Success);
}

} // namespace sf

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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network/Export.hpp>

#include <SFML/System/TimeoutWithPredicate.hpp>

#include <array>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <cstddef>
#include <cstdint>


namespace sf
{
class IpAddress;

////////////////////////////////////////////////////////////
/// \brief An SSH File Transfer Protocol (SFTP) client
///
////////////////////////////////////////////////////////////
class SFML_NETWORK_API Sftp
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief SFTP result
    ///
    ////////////////////////////////////////////////////////////
    class SFML_NETWORK_API Result
    {
    public:
        ////////////////////////////////////////////////////////////
        /// \brief Result values
        ///
        ////////////////////////////////////////////////////////////
        enum class Value
        {
            // General result values
            Success,      //!< Operation completed successfully
            Disconnected, //!< The TCP socket has been disconnected
            Timeout,      //!< Operation timed out
            Refused,      //!< Connection refused
            Error,        //!< Generic error

            // SSH result values
            BannerReceive,               //!< Error during banner receive
            BannerSend,                  //!< Error during banner send
            InvalidMac,                  //!< Invalid message authentication code
            AllocationFailure,           //!< Allocation failure
            SocketSend,                  //!< Error sending on socket
            KeyExchangeFailure,          //!< Key exchange failed
            HostKeyInitialization,       //!< Host key initialization failed
            HostKeySign,                 //!< Host key signing failed
            DecryptError,                //!< Decryption failed
            ProtocolError,               //!< SSH protocol error
            PasswordExpired,             //!< Password expired
            FileError,                   //!< File error
            MethodNone,                  //!< No method found
            AuthenticationFailed,        //!< Authentication failed
            PublicKeyUnverified,         //!< Public key unverified
            ChannelOutOfOrder,           //!< Channel out of order
            ChannelFailure,              //!< Channel failure
            ChannelRequestDenied,        //!< Channel request denied
            ChannelUnknown,              //!< Channel unknown
            ChannelWindowExceeded,       //!< Channel window exceeded
            ChannelPacketExceeded,       //!< Channel packet exceeded
            ChannelClosed,               //!< Channel closed
            ChannelEofSent,              //!< Channel EOF sent
            ScpProtocol,                 //!< SCP protocol error
            ZlibError,                   //!< Zlib error
            RequestDenied,               //!< Request denied
            MethodNotSupported,          //!< Method not supported
            InvalidData,                 //!< Invalid data
            PublicKeyProtocol,           //!< Public key protocol error
            BufferTooSmall,              //!< Buffer too small
            BadUse,                      //!< Bad usage
            CompressError,               //!< Compression error
            OutOfBoundary,               //!< Out of boundary
            AgentProtocol,               //!< Agent protocol error
            SocketRecv,                  //!< Socket receive error
            EncryptError,                //!< Encryption failed
            BadSocket,                   //!< Bad socket
            KnownHosts,                  //!< Known hosts error
            ChannelWindowFull,           //!< Channel window full
            KeyFileAuthenticationFailed, //!< Key file authentication failed

            // SFTP result values
            EndOfFile,            //!< End of file
            NoSuchFile,           //!< No such file
            PermissionDenied,     //!< Permission denied
            Failure,              //!< Failure
            BadMessage,           //!< Bad message
            NoConnection,         //!< No connection
            ConnectionLost,       //!< Connection lost
            OperationUnsupported, //!< Operation unsupported
            InvalidHandle,        //!< Invalid handle
            NoSuchPath,           //!< No such path
            FileAlreadyExists,    //!< File already exists
            WriteProtect,         //!< Write protect
            NoMedia,              //!< No media
            NoSpaceOnFileSystem,  //!< No space on filesystem
            QuotaExceeded,        //!< Quota exceeded
            UnknownPrincipal,     //!< Unknown principal
            LockConflict,         //!< Lock conflict
            DirectoryNotEmpty,    //!< Directory not empty
            NotADirectory,        //!< Not a directory
            InvalidFilename,      //!< Invalid filename
            LinkLoop,             //!< Link loop
            SftpError             //!< Generic SFTP error
        };

        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        /// This constructor is used by the SFTP client to build
        /// the result.
        ///
        /// \param value   Result value
        /// \param message Result message
        ///
        ////////////////////////////////////////////////////////////
        explicit Result(Value value, std::string message = "");

        ////////////////////////////////////////////////////////////
        /// \brief Check if the result is a success
        ///
        /// This function is defined for convenience, it is
        /// equivalent to testing if the result value is `Value::Success`.
        ///
        /// \return `true` if the result is `Value::Success`, `false` if it is not `Value::Success`
        ///
        ////////////////////////////////////////////////////////////
        [[nodiscard]] bool isOk() const;

        ////////////////////////////////////////////////////////////
        /// \brief Get the result value
        ///
        /// \return The result value
        ///
        ////////////////////////////////////////////////////////////
        [[nodiscard]] Value getValue() const;

        ////////////////////////////////////////////////////////////
        /// \brief Get the result message
        ///
        /// \return The result message
        ///
        ////////////////////////////////////////////////////////////
        [[nodiscard]] const std::string& getMessage() const;

    private:
        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        Value       m_value;   //!< The contained value
        std::string m_message; //!< The contained message
    };

    ////////////////////////////////////////////////////////////
    /// \brief Result of an operation returning a path
    ///
    ////////////////////////////////////////////////////////////
    class SFML_NETWORK_API PathResult : public Result
    {
    public:
        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        /// \param result Result
        /// \param path   Path
        ///
        ////////////////////////////////////////////////////////////
        PathResult(const Result& result, std::filesystem::path path);

        ////////////////////////////////////////////////////////////
        /// \brief Get the path
        ///
        /// \return The path
        ///
        ////////////////////////////////////////////////////////////
        [[nodiscard]] const std::filesystem::path& getPath() const;

    private:
        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        std::filesystem::path m_path; //!< The contained path
    };

    ////////////////////////////////////////////////////////////
    /// \brief File or directory attributes
    ///
    ////////////////////////////////////////////////////////////
    struct Attributes
    {
        std::filesystem::path                          path;             //!< Path to the entry
        std::optional<std::filesystem::file_type>      type;             //!< Type of the entry
        std::optional<std::uint64_t>                   size;             //!< Size of the entry
        std::optional<std::filesystem::perms>          permissions;      //!< Permissions
        std::optional<std::uint64_t>                   userId;           //!< Owner user ID
        std::optional<std::uint64_t>                   groupId;          //!< Group ID
        std::optional<std::filesystem::file_time_type> accessTime;       //!< Last access time
        std::optional<std::filesystem::file_time_type> modificationTime; //!< Last modification time
    };

    ////////////////////////////////////////////////////////////
    /// \brief Result of an operation returning attributes
    ///
    ////////////////////////////////////////////////////////////
    class SFML_NETWORK_API AttributesResult : public Result
    {
    public:
        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        /// \param result     Result
        /// \param attributes Attributes
        ///
        ////////////////////////////////////////////////////////////
        AttributesResult(const Result& result, Attributes attributes);

        ////////////////////////////////////////////////////////////
        /// \brief Get the attributes
        ///
        /// \return The attributes
        ///
        ////////////////////////////////////////////////////////////
        [[nodiscard]] const Attributes& getAttributes() const;

    private:
        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        Attributes m_attributes; //!< The contained attributes
    };

    ////////////////////////////////////////////////////////////
    /// \brief Result of an operation returning a directory listing
    ///
    ////////////////////////////////////////////////////////////
    class SFML_NETWORK_API ListingResult : public Result
    {
    public:
        ////////////////////////////////////////////////////////////
        /// \brief Constructor
        ///
        /// \param result  Result
        /// \param listing Directory listing
        ///
        ////////////////////////////////////////////////////////////
        ListingResult(const Result& result, std::vector<Attributes> listing);

        ////////////////////////////////////////////////////////////
        /// \brief Get the directory listing
        ///
        /// \return The directory listing
        ///
        ////////////////////////////////////////////////////////////
        [[nodiscard]] const std::vector<Attributes>& getListing() const;

    private:
        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        std::vector<Attributes> m_listing; //!< The contained directory listing
    };

    ////////////////////////////////////////////////////////////
    /// \brief Structure containing information about an active SFTP session
    ///
    ////////////////////////////////////////////////////////////
    struct SessionInfo
    {
        ////////////////////////////////////////////////////////////
        /// \brief Host key used to identify a host
        ///
        ////////////////////////////////////////////////////////////
        struct HostKey
        {
            enum class Type
            {
                Unknown,  //!< Unknown key type
                Rsa,      //!< RSA
                Dsa,      //!< DSA
                Ecdsa256, //!< NIST P-256 ECDSA
                Ecdsa384, //!< NIST P-384 ECDSA
                Ecdsa521, //!< NIST P-521 ECDSA
                Ed25519   //!< ED25519
            };

            Type                      type = Type::Unknown; //!< Host key type
            std::vector<std::byte>    data;                 //!< Host key data
            std::array<std::byte, 20> sha1{};               //!< Host key SHA1 hash
            std::array<std::byte, 32> sha256{};             //!< Host key SHA256 hash
        };

        HostKey     hostKey;                           //!< Host key
        std::string keyExchangeAlgorithm;              //!< Key exchange algorithm used in the session (RFC 4253)
        std::string hostKeyAlgorithm;                  //!< Host key algorithm used in the session (RFC 4253)
        std::string clientToServerEncryptionAlgorithm; //!< Client to server encryption algorithm used in the session (RFC 4253)
        std::string serverToClientEncryptionAlgorithm; //!< Server to client encryption algorithm used in the session (RFC 4253)
        std::string clientToServerMacAlgorithm; //!< Client to server message authentication code algorithm used in the session (RFC 4253)
        std::string serverToClientMacAlgorithm; //!< Server to client message authentication code algorithm used in the session (RFC 4253)
        std::string clientToServerCompressionAlgorithm; //!< Client to server compression algorithm used in the session (RFC 4253)
        std::string serverToClientCompressionAlgorithm; //!< Server to client compression algorithm used in the session (RFC 4253)
    };

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Sftp();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// Automatically closes the connection with the server if
    /// it is still opened.
    ///
    ////////////////////////////////////////////////////////////
    ~Sftp();

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy constructor
    ///
    ////////////////////////////////////////////////////////////
    Sftp(const Sftp&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy assignment
    ///
    ////////////////////////////////////////////////////////////
    Sftp& operator=(const Sftp&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Connect to the specified SFTP server
    ///
    /// The port has a default value of 22, which is the standard
    /// port used by the SFTP protocol.
    /// This function tries to connect to the server so it may take
    /// a while to complete, especially if the server is not
    /// reachable. To avoid blocking your application for too long,
    /// you can use a timeout. The default value, `Time::Zero`, means that the
    /// system timeout will be used (which is usually pretty long).
    ///
    /// \param server  Name or address of the SFTP server to connect to
    /// \param port    Port used for the connection
    /// \param timeout Maximum time to wait, optionally a predicate can be provided for more fine-grained control
    ///
    /// \return Result of the connection attempt
    ///
    /// \see `disconnect`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Result connect(IpAddress server, unsigned short port = 22, const TimeoutWithPredicate& timeout = Time::Zero);

    ////////////////////////////////////////////////////////////
    /// \brief Disconnect the connection with the server
    ///
    /// \param timeout Maximum time to wait, optionally a predicate can be provided for more fine-grained control
    ///
    /// \return Result of disconnecting the connection with the server
    ///
    /// \see `connect`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Result disconnect(const TimeoutWithPredicate& timeout = Time::Zero);

    ////////////////////////////////////////////////////////////
    /// \brief Get SSH session information
    ///
    /// After connecting to the server and before actually
    /// logging in the SSH session information of the underlying
    /// connection will be available.
    ///
    /// The session information contains among other things the
    /// public key identifying the remote host and the connection
    /// parameters such as encryption and compression used. The
    /// identifiers used follow the RFC 4253 specification.
    ///
    /// If the session information is not available `std::nullopt`
    /// will be returned.
    ///
    /// Because SSH was developed as a parallel standard to
    /// SSL/TLS and automatic host certificate verification wasn't
    /// widespread at the time, relying on the user to check the
    /// authenticity of the host key was the typical method used
    /// to verify that they were connecting to the legitimate host,
    /// assuming the private key of the remote host was not
    /// compromised.
    ///
    /// If connection security is a high priority, examining
    /// the parameters and aborting the connection if any weak
    /// algorithms are used is also possible.
    ///
    /// \return SSH session information or `std::nullopt` if it is not available
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::optional<SessionInfo> getSessionInfo() const;

    ////////////////////////////////////////////////////////////
    /// \brief Log in using a username and a password
    ///
    /// Logging in is mandatory after connecting to the server.
    /// Users that are not logged in cannot perform any operation.
    ///
    /// \param name     User name
    /// \param password Password
    /// \param timeout  Maximum time to wait, optionally a predicate can be provided for more fine-grained control
    ///
    /// \return Result of attempting to log in to the server
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Result login(std::string_view            name,
                               std::string_view            password,
                               const TimeoutWithPredicate& timeout = Time::Zero);

    ////////////////////////////////////////////////////////////
    /// \brief Log in using a public/private key pair
    ///
    /// Logging in is mandatory after connecting to the server.
    /// Users that are not logged in cannot perform any operation.
    ///
    /// This overload allows logging into the SFTP server using
    /// public key authentication.
    ///
    /// The public and private key data should be provided in PEM
    /// format. PEM encoded data can be easily recognized by their
    /// `-----BEGIN ............-----` header and
    /// `-----END ............-----` footer.
    ///
    /// Even though it is technically possible to derive the
    /// public key from the private key, due to backend
    /// limitations, providing a pre-generated public key as well
    /// is necessary for this function to be able to succeed.
    ///
    /// If the private key is protected by a passphrase the
    /// passphrase can be provided as a NULL terminated string.
    /// If the private key is not protected by a passphrase the
    /// passphrase should be set to the empty string.
    ///
    /// \param name                 User name
    /// \param publicKeyData        Public key data
    /// \param publicKeyLength      Public key data length
    /// \param privateKeyData       Private key data
    /// \param privateKeyLength     Private key data length
    /// \param privateKeyPassphrase Private key passphrase, NULL terminated
    /// \param timeout              Maximum time to wait, optionally a predicate can be provided for more fine-grained control
    ///
    /// \return Result of attempting to log in to the server
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Result login(std::string_view            name,
                               const char*                 publicKeyData,
                               std::size_t                 publicKeyLength,
                               const char*                 privateKeyData,
                               std::size_t                 privateKeyLength,
                               const char*                 privateKeyPassphrase = "",
                               const TimeoutWithPredicate& timeout              = Time::Zero);

    ////////////////////////////////////////////////////////////
    /// \brief Log in using a public/private key pair
    ///
    /// Logging in is mandatory after connecting to the server.
    /// Users that are not logged in cannot perform any operation.
    ///
    /// This overload allows logging into the SFTP server using
    /// public key authentication.
    ///
    /// The public and private key data should be provided in PEM
    /// format. PEM encoded data can be easily recognized by their
    /// `-----BEGIN ............-----` header and
    /// `-----END ............-----` footer.
    ///
    /// Even though it is technically possible to derive the
    /// public key from the private key, due to backend
    /// limitations, providing a pre-generated public key as well
    /// is necessary for this function to be able to succeed.
    ///
    /// If the private key is protected by a passphrase the
    /// passphrase can be provided as a NULL terminated string.
    /// If the private key is not protected by a passphrase the
    /// passphrase should be set to the empty string.
    ///
    /// \param name                 User name
    /// \param publicKeyData        Public key data
    /// \param privateKeyData       Private key data
    /// \param privateKeyPassphrase Private key passphrase
    /// \param timeout              Maximum time to wait, optionally a predicate can be provided for more fine-grained control
    ///
    /// \return Result of attempting to log in to the server
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Result login(std::string_view            name,
                               std::string_view            publicKeyData,
                               std::string_view            privateKeyData,
                               std::string_view            privateKeyPassphrase = {},
                               const TimeoutWithPredicate& timeout              = Time::Zero);

    ////////////////////////////////////////////////////////////
    /// \brief Resolve a remote path into an absolute remote path
    ///
    /// Paths can contain links and other reserved path identifiers
    /// such as . and .. referring to the current directory and
    /// parent directory respectively.
    ///
    /// When determining the absolute path, which does not contain
    /// links or . or .. is necessary, this function can be used.
    ///
    /// Resolving "." will return the absolute path to the current
    /// working directory of the user after logging in to the SFTP
    /// server.
    ///
    /// \param path    Path to convert into an absolute path
    /// \param timeout Maximum time to wait, optionally a predicate can be provided for more fine-grained control
    ///
    /// \return Result of converting the path into an absolute path
    ///
    /// \see `getWorkingDirectory`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] PathResult resolvePath(const std::filesystem::path& path, const TimeoutWithPredicate& timeout = Time::Zero);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current working directory on the server
    ///
    /// This is an alias for calling `resolvePath(".")`.
    ///
    /// \param timeout Maximum time to wait, optionally a predicate can be provided for more fine-grained control
    ///
    /// \return Result of getting the current working directory
    ///
    /// \see `resolvePath`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] PathResult getWorkingDirectory(const TimeoutWithPredicate& timeout = Time::Zero);

    ////////////////////////////////////////////////////////////
    /// \brief Get the attributes of a remote file or directory
    ///
    /// Depending on whether `path` refers to a file or directory,
    /// the attributes can contain e.g. the type of file, the file
    /// owner, group, file size, modification and access times.
    ///
    /// If links are not to be followed, `followLinks` can be set
    /// to `false`. In this case the attributes of the link itself
    /// will be returned.
    ///
    /// \param path        Path to the remote file or directory whose attributes to get
    /// \param followLinks `true` to follow links, `false` to return attributes of the link itself
    /// \param timeout     Maximum time to wait, optionally a predicate can be provided for more fine-grained control
    ///
    /// \return Result of getting the attributes
    ///
    /// \see `getDirectoryListing`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] AttributesResult getAttributes(const std::filesystem::path& path,
                                                 bool                         followLinks = true,
                                                 const TimeoutWithPredicate&  timeout     = Time::Zero);

    ////////////////////////////////////////////////////////////
    /// \brief Get the contents of the given directory
    ///
    /// This function retrieves the sub-directories and files
    /// contained in the given directory. It is not recursive.
    ///
    /// \param path    Path of the directory whose contents to list
    /// \param timeout Maximum time to wait, optionally a predicate can be provided for more fine-grained control
    ///
    /// \return Result of getting the contents of the given directory
    ///
    /// \see `getAttributes`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] ListingResult getDirectoryListing(const std::filesystem::path& path,
                                                    const TimeoutWithPredicate&  timeout = Time::Zero);

    ////////////////////////////////////////////////////////////
    /// \brief Create a new directory
    ///
    /// The new directory is created as a child of the current
    /// working directory.
    ///
    /// The default permissions value is equivalent to `rwxr-xr-x`
    /// or 0755 in octal notation.
    ///
    /// \param path        Path of the directory to create
    /// \param permissions Permissions of the directory to create
    /// \param timeout     Maximum time to wait, optionally a predicate can be provided for more fine-grained control
    ///
    /// \return Result of creating the directory
    ///
    /// \see `deleteDirectory`, `rename`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Result createDirectory(
        const std::filesystem::path& path,
        std::filesystem::perms permissions  = (std::filesystem::perms::owner_all | std::filesystem::perms::group_read |
                                              std::filesystem::perms::group_exec | std::filesystem::perms::others_read |
                                              std::filesystem::perms::others_exec),
        const TimeoutWithPredicate& timeout = Time::Zero);

    ////////////////////////////////////////////////////////////
    /// \brief Remove an existing directory
    ///
    /// Use this function with caution, the directory will
    /// be removed permanently!
    ///
    /// \param path    Path of the directory to remove
    /// \param timeout Maximum time to wait, optionally a predicate can be provided for more fine-grained control
    ///
    /// \return Result of removing the directory
    ///
    /// \see `createDirectory`, `rename`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Result deleteDirectory(const std::filesystem::path& path, const TimeoutWithPredicate& timeout = Time::Zero);

    ////////////////////////////////////////////////////////////
    /// \brief Rename an existing file or directory
    ///
    /// In POSIX renaming and moving and synonymous. If you want
    /// to move a file or directory from one place to another
    /// you rename it from an old to a new path.
    ///
    /// If a file exists at the specified new path, depending
    /// on whether `ovewrite` is set to true, the rename operation
    /// will overwrite it or not. If a directory is being moved,
    /// the new path must either not point to non-existant
    /// directory or a directory that is empty. Non-empty
    /// directories cannot be overwritten by this operation.
    ///
    /// \param oldPath   Old path to the file or directory
    /// \param newPath   New path to the file or directory
    /// \param overwrite Set to `true` to allow overwriting a file that exists at `newPath`
    /// \param timeout   Maximum time to wait, optionally a predicate can be provided for more fine-grained control
    ///
    /// \return Result of the operation
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Result rename(const std::filesystem::path& oldPath,
                                const std::filesystem::path& newPath,
                                bool                         overwrite = false,
                                const TimeoutWithPredicate&  timeout   = Time::Zero);

    ////////////////////////////////////////////////////////////
    /// \brief Remove an existing file
    ///
    /// Use this function with caution, the file will be
    /// removed permanently!
    ///
    /// \param path    Path to the file to remove
    /// \param timeout Maximum time to wait, optionally a predicate can be provided for more fine-grained control
    ///
    /// \return Result of removing the file
    ///
    /// \see `rename`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Result deleteFile(const std::filesystem::path& path, const TimeoutWithPredicate& timeout = Time::Zero);

    ////////////////////////////////////////////////////////////
    /// \brief Download a file from the server
    ///
    /// This function retrieves the data in the file at the
    /// remote path.
    ///
    /// The file data is transferred in sequential blocks. For
    /// every block of data transferred, the provided callback
    /// is called. The callback is passed a pointer to a data
    /// block and the size of the data contained in the current
    /// block. This size can change over time so it is important
    /// to always check the size value to know how much data is
    /// actually available. The callback should return `true` to
    /// indicate to the `download` function that it should
    /// continue to transfer data. If the data transfer should
    /// be aborted earlier, `false` can be returned from the
    /// callback.
    ///
    /// The function returns once all the data in the remote file
    /// has been transferred or an error occurs or the function
    /// times out.
    ///
    /// If reading from the remote file should not start at the
    /// beginning of the file, you can specify an offset in
    /// bytes at which reading should start.
    ///
    /// \param remotePath Path of the remote file whose data to download
    /// \param callback   Callback to be called for every available data block
    /// \param offset     Byte offset into the remote file at which reading should start
    /// \param timeout    Maximum time to wait, optionally a predicate can be provided for more fine-grained control
    ///
    /// \return Result of downloading the file
    ///
    /// \see `upload`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Result download(const std::filesystem::path&                                   remotePath,
                                  const std::function<bool(const void* data, std::size_t size)>& callback,
                                  std::uint64_t                                                  offset  = 0,
                                  const TimeoutWithPredicate&                                    timeout = Time::Zero);

    ////////////////////////////////////////////////////////////
    /// \brief Upload a file to the server
    ///
    /// This function writes data into a file at the remote path.
    ///
    /// The file data is transferred in sequential blocks. Every
    /// time the function wants to send a new block of data the
    /// provided callback is called. The callback is passed a
    /// pointer to a data block and a reference to the size of
    /// the data block. Data to be sent should be copied into
    /// the data block using e.g. `std::memcpy` and the size value
    /// set to the actual number of bytes copied into the data
    /// block. The size of the block can change over time so it
    /// is important to check the size value that is passed to
    /// the callback to know how many bytes can actually be
    /// copied into the data block. The callback should return
    /// `true` to indicate to the `upload` function that it
    /// should continue to transfer data. Once the data transfer
    /// should be stopped e.g. because there is no more data left
    /// to send, `false` can be returned from the callback.
    ///
    /// The function returns once all the data has been sent or
    /// an error occurs or the function times out.
    ///
    /// If a file does not exist at the remote path yet, it will
    /// be created with the provided permissions.
    ///
    /// If a file already exists at the remote path, setting
    /// `truncate` to `true` will truncate the existing file
    /// i.e. delete all pre-existing data before starting to
    /// write the new data into the file.
    ///
    /// Setting `append` to `true` will append to a file if
    /// it already exists.
    ///
    /// If writing to the remote file should not start at the
    /// beginning of the file, you can specify an offset in
    /// bytes at which writing should start.
    ///
    /// The default permissions value is equivalent to `rw-r--r--`
    /// or 0644 in octal notation.
    ///
    /// \param remotePath  Path of the remote file in which to upload the data
    /// \param callback    Callback to be called for every available data block
    /// \param permissions Permissions of the remote file if it has to be created
    /// \param truncate    Set to `true` to truncate the remote file if it already exists
    /// \param append      Set to `true` to append to the remote file if it already exists
    /// \param offset      Byte offset into the remote file at which writing should start
    /// \param timeout     Maximum time to wait, optionally a predicate can be provided for more fine-grained control
    ///
    /// \return Result of uploading the file
    ///
    /// \see `download`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Result upload(
        const std::filesystem::path&                              remotePath,
        const std::function<bool(void* data, std::size_t& size)>& callback,
        std::filesystem::perms permissions = (std::filesystem::perms::owner_read | std::filesystem::perms::owner_write |
                                              std::filesystem::perms::group_read | std::filesystem::perms::others_read),
        bool                   truncate    = true,
        bool                   append      = false,
        std::uint64_t          offset      = 0,
        const TimeoutWithPredicate& timeout = Time::Zero);

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    struct Impl;
    std::unique_ptr<Impl> m_impl; //!< Implementation
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::Sftp
/// \ingroup network
///
/// `sf::Sftp` is a very simple SFTP client that allows you
/// to communicate with a SFTP server. The SFTP protocol allows
/// you to manipulate a remote file system (list files,
/// upload, download, create, remove, ...).
///
/// Using the SFTP client consists of 4 parts:
/// \li Connecting to the SFTP server
/// \li Logging in (either using username and password or public key authentication)
/// \li Performing file system operations
/// \li Disconnecting (this part can be done implicitly by the destructor)
///
/// Every operation returns an SFTP result, which contains the
/// result code and optionally a message describing the result. Some
/// commands such as `getAttributes()` and `getDirectoryListing()`
/// return additional data, and use a class derived from
/// `sf::Sftp::Result` to provide this data. The commonly used filesystem
/// operations are directly provided as member functions.
///
/// There are three classes of result values: general result values,
/// SSH specific result values and SFTP specific result values. General
/// result values are returned on success and errors not specific to the
/// SSH or SFTP protocols. If an operation manipulating the remote
/// filesystem fails, an SFTP specific result will be returned describing
/// the reason for the failure. If any error related to the communication
/// channel occurs e.g. key exchange failure or authentication failure an
/// SSH error will be returned.
///
/// Because most operations manipulate some part of the remote filesystem
/// these operations, especially upload and download, may take some
/// time to complete. This is important to know if you don't want
/// to block your application while the server is completing
/// the task.
///
/// Every operation that relies on network communication can be passed
/// a timeout argument. In the simplest case, this timeout argument can
/// just be a fixed time duration after which the operation should time
/// out if no progress has been made. Because some operations simply due
/// to their nature can take longer than others it might not be possible
/// to use the same timeout value for all operations. If it is anticipated
/// that an operation could take a longer amount of time to complete due
/// to e.g. an unreliable or slow connection, the timeout should be increased
/// so the operation still has the possibility to complete successfully.
/// Passing `sf::Time::Zero` as a timeout value will cause the operation
/// to block until completion or failure.
///
/// If you want to pass `sf::Time::Zero` to wait indefinitely for an
/// operation to complete but still have the possibility to interrupt the
/// operation e.g. if the user decides to quit the application or interrupt
/// a longer download or upload, the timeout can be constructed from a
/// predicate and polling period. The predicate is a callable that takes
/// no parameters and returns `true` if the operation should continue or
/// `false` if the operation should time out. The time between consecutive
/// evaluations of the predicate is specified by the `period` parameter.
///
/// Example of a `TimeoutWithPredicate`:
/// \code
/// bool run = true;
/// sf::Sftp::TimeoutWithPredicate timeout([&run]{ return run; }, sf::milliseconds(1));
/// \endcode
/// When passed to an operation, the above `TimeoutWithPredicate` object will
/// keep the operation running as long as `run` is `true`. The condition
/// is checked every 1 millisecond. As soon as `run` is set to `false` the
/// operation will terminate with a `Timeout` result.
///
/// Remember that attempting to modify variables from across threads will
/// require proper synchronization using e.g. mutexes or atomics.
///
/// Because the filesystem operations will be performed on the remote
/// system, the paths passed to the operations should be valid paths on
/// the remote system. To guarantee portability, generic POSIX paths should
/// be used. These use `/` as the path seperator and `.` to refer to the
/// current directory and `..` to refer to the parent directory.
///
/// When logging into the server as a specific user the current directory
/// will be set to the home directory that was specified for that user
/// account. Because it cannot be assumed that the SFTP user is also allowed
/// to log into a shell, changing the current directory from the home
/// directory is not always possible and thus not supported. Thus when
/// specifying paths, it is preferred to always make use of absolute paths
/// when possible. Calling `getWorkingDirectory()` will return the current
/// directory as an absolute path. Listing the contents of a directory whose
/// path is passed as an absolute path will yield directory listing entries
/// whose paths are extentions of the directory's path and thus also absolute
/// paths. The `resolvePath()` function can be used to convert relative paths
/// or paths containing links into absolute paths without links.
///
/// Usage example:
/// \code
/// // Create a new SFTP client
/// sf::Sftp sftp;
///
/// // Get the address of the server
/// const auto addresses = sf::Dns::resolve("sftp.myserver.com");
///
/// // Check if we have a valid address
/// if (!addresses.has_value() || addresses->empty())
/// {
///     // Handle no valid address...
/// }
///
/// // Connect to the server
/// sf::Sftp::Result result = sftp.connect(addresses->front());
/// if (result.isOk())
///     std::cout << "Connected" << std::endl;
///
/// // Check session information and verify server
/// std::optional<sf::Sftp::SessionInfo> info = sftp.getSessionInfo();
/// if (info->hostKey.sha256 != expectedSha256)
/// {
///     std::cout << "Server untrusted" << std::endl;
///     // Handle untrusted server
/// }
///
/// // Log in
/// result = sftp.login("sftpuser", "dF6Zm89D");
/// if (result.isOk())
///     std::cout << "Logged in" << std::endl;
///
/// // Print the working directory
/// sf::Sftp::PathResult directory = sftp.getWorkingDirectory();
/// if (directory.isOk())
///     std::cout << "Working directory: " << directory.getPath().string() << std::endl;
///
/// // Create a new directory
/// result = sftp.createDirectory("files");
/// if (result.isOk())
///     std::cout << "Created new directory" << std::endl;
///
/// // Upload a text file to this new directory
/// result = sftp.upload("files/newfile.txt", [](void* data, std::size_t& size)
/// {
///     // In your own code, ensure size is big enough to store your data
///     // We skip the check here for simplicity
///     std::memcpy(data, "Hello World", 11);
///     size = 11;
///     return false; // End transfer after this block
/// });
/// if (response.isOk())
///     std::cout << "File uploaded" << std::endl;
///
/// // Rename file
/// result = sftp.rename("files/newfile.txt", "files/newfile2.txt");
/// if (result.isOk())
///     std::cout << "Renamed file" << std::endl;
///
/// // Download a text file from this new directory
/// std::string text;
/// result = sftp.download("files/newfile2.txt", [&text](const void* data, std::size_t size)
/// {
///     text.append(static_cast<const char*>(data), size);
///     return true; // Continue until all data received
/// });
/// if (response.isOk())
///     std::cout << "File downloaded: " << text << std::endl;
///
/// // Delete file
/// result = sftp.deleteFile("files/newfile2.txt");
/// if (result.isOk())
///     std::cout << "Deleted file" << std::endl;
///
/// // Delete directory
/// result = sftp.deleteDirectory("files");
/// if (result.isOk())
///     std::cout << "Deleted directory" << std::endl;
///
/// // Disconnect from the server (optional)
/// result = sftp.disconnect();
/// if (result.isOk())
///     std::cout << "Disconnected successfully" << std::endl;
/// \endcode
///
////////////////////////////////////////////////////////////

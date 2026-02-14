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
// MinGW defines _WIN32_WINNT to 0x0501 by default if it is not yet defined
// If we don't set _WIN32_WINNT ourselves early here it will take precedence over
// our own value thus preventing inet_pton and inet_ntop from being made available
#include <SFML/Config.hpp>
#if defined(SFML_SYSTEM_WINDOWS)
#include <SFML/System/Win32/WindowsHeader.hpp>
#endif

#include <SFML/Network/Dns.hpp>
#include <SFML/Network/Http.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/SocketImpl.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Utils.hpp>

#include <algorithm>
#include <istream>
#include <ostream>

#include <cstring>

using namespace std::string_view_literals;


namespace sf
{
////////////////////////////////////////////////////////////
const IpAddress IpAddress::Any(0, 0, 0, 0);
const IpAddress IpAddress::LocalHost(127, 0, 0, 1);
const IpAddress IpAddress::Broadcast(255, 255, 255, 255);
const IpAddress IpAddress::AnyV4(0, 0, 0, 0);
const IpAddress IpAddress::LocalHostV4(127, 0, 0, 1);
const IpAddress IpAddress::BroadcastV4(255, 255, 255, 255);
const IpAddress IpAddress::AnyV6({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
const IpAddress IpAddress::LocalHostV6({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1});


////////////////////////////////////////////////////////////
std::optional<IpAddress> IpAddress::resolve(std::string_view address)
{
    // For backwards compatibility try to convert from string representation first
    if (const auto result = fromString(address); result)
        return result;

    // If not a string representation, resolve address
    if (auto addresses = Dns::resolve(address); !addresses.empty())
    {
        // Remove IPv6 addresses to stay backwards compatible
        addresses
            .erase(std::remove_if(addresses.begin(), addresses.end(), [](const auto& entry) { return !entry.isV4(); }),
                   addresses.end());

        if (!addresses.empty())
            return addresses.front();
    }

    return std::nullopt;
}


////////////////////////////////////////////////////////////
IpAddress::IpAddress(std::uint8_t byte0, std::uint8_t byte1, std::uint8_t byte2, std::uint8_t byte3) :
    m_address(static_cast<std::uint32_t>((byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3))
{
}


////////////////////////////////////////////////////////////
IpAddress::IpAddress(std::uint32_t address) : m_address(address)
{
}


////////////////////////////////////////////////////////////
IpAddress::IpAddress(std::array<std::uint8_t, 16> bytes) : m_address(bytes)
{
}


////////////////////////////////////////////////////////////
std::optional<IpAddress> IpAddress::fromString(std::string_view address)
{
    using namespace std::string_view_literals;

    if (address.empty())
        return std::nullopt;

    // Optimizations for known address strings
    if (address == "0.0.0.0"sv)
        return AnyV4;

    if (address == "127.0.0.1"sv)
        return LocalHostV4;

    if (address == "255.255.255.255"sv)
        return BroadcastV4;

    // Optimizations for known address strings
    if (address == "::"sv)
        return AnyV6;

    if (address == "::1"sv)
        return LocalHostV6;

    // inet_pton expects the string to be null-terminated
    const std::string addressString(address);

    // Try to convert the address from dotted-decimal notation ("xxx.xxx.xxx.xxx")
    if (in_addr addr{}; inet_pton(AF_INET, addressString.c_str(), &addr) == 1)
        return IpAddress(ntohl(addr.s_addr));

    // Try to convert the address from internet standard notation ("xxxx:xxxx::xxxx:xxxx")
    if (in6_addr addr{}; inet_pton(AF_INET6, addressString.c_str(), &addr) == 1)
        return IpAddress(
            {addr.s6_addr[0],
             addr.s6_addr[1],
             addr.s6_addr[2],
             addr.s6_addr[3],
             addr.s6_addr[4],
             addr.s6_addr[5],
             addr.s6_addr[6],
             addr.s6_addr[7],
             addr.s6_addr[8],
             addr.s6_addr[9],
             addr.s6_addr[10],
             addr.s6_addr[11],
             addr.s6_addr[12],
             addr.s6_addr[13],
             addr.s6_addr[14],
             addr.s6_addr[15]});

    return std::nullopt;
}


////////////////////////////////////////////////////////////
std::string IpAddress::toString() const
{
    if (const auto* addressV4 = std::get_if<V4Data>(&m_address); addressV4)
    {
        in_addr address{};
        address.s_addr = htonl(*addressV4);

        if (std::array<char, 32> buffer{}; inet_ntop(AF_INET, &address, buffer.data(), buffer.size()))
            return buffer.data();
    }
    else if (const auto* addressV6 = std::get_if<V6Data>(&m_address); addressV6)
    {
        in6_addr address{};
        static_assert(sizeof(address.s6_addr) == sizeof(*addressV6));
        std::memcpy(address.s6_addr, addressV6->data(), addressV6->size());

        if (std::array<char, 64> buffer{}; inet_ntop(AF_INET6, &address, buffer.data(), buffer.size()))
            return buffer.data();
    }

    return "";
}


////////////////////////////////////////////////////////////
std::uint32_t IpAddress::toInteger() const
{
    assert(std::holds_alternative<V4Data>(m_address));
    return std::get<V4Data>(m_address);
}


////////////////////////////////////////////////////////////
std::array<std::uint8_t, 16> IpAddress::toBytes() const
{
    assert(std::holds_alternative<V6Data>(m_address));
    return std::get<V6Data>(m_address);
}


////////////////////////////////////////////////////////////
IpAddress::Type IpAddress::getType() const
{
    return std::holds_alternative<V4Data>(m_address) ? Type::IpV4 : Type::IpV6;
}


////////////////////////////////////////////////////////////
bool IpAddress::isV4() const
{
    return getType() == Type::IpV4;
}


////////////////////////////////////////////////////////////
bool IpAddress::isV6() const
{
    return getType() == Type::IpV6;
}


////////////////////////////////////////////////////////////
std::optional<IpAddress> IpAddress::getLocalAddress(Type type)
{
    // The method here is to connect a UDP socket to a public ip,
    // and get the local socket address with the getsockname function.
    // UDP connection will not send anything to the network, so this function won't cause any overhead.

    if (type == Type::IpV4)
    {
        // Create the socket
        const SocketHandle sock = socket(PF_INET, SOCK_DGRAM, 0);
        if (sock == priv::SocketImpl::invalidSocket())
        {
            err() << "Failed to retrieve local address (invalid socket)" << std::endl;
            return std::nullopt;
        }

        // Connect the socket to a public ip (here 1.1.1.1) on any
        // port. This will give the local address of the network interface
        // used for default routing which is usually what we want.
        sockaddr_in address = priv::SocketImpl::createAddress(0x01010101, 9);
        if (connect(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
        {
            priv::SocketImpl::close(sock);

            err() << "Failed to retrieve local address (socket connection failure)" << std::endl;
            return std::nullopt;
        }

        // Get the local address of the socket connection
        priv::SocketImpl::AddrLength size = sizeof(address);
        if (getsockname(sock, reinterpret_cast<sockaddr*>(&address), &size) == -1)
        {
            priv::SocketImpl::close(sock);

            err() << "Failed to retrieve local address (socket local address retrieval failure)" << std::endl;
            return std::nullopt;
        }

        // Close the socket
        priv::SocketImpl::close(sock);

        // Finally build the IP address
        return IpAddress(ntohl(address.sin_addr.s_addr));
    }

    // If we get to this point the type specified is either IPv6 or Any

    // Create the socket
    const SocketHandle sock = socket(PF_INET6, SOCK_DGRAM, 0);
    if (sock == priv::SocketImpl::invalidSocket())
    {
        err() << "Failed to retrieve local address (invalid socket)" << std::endl;
        return std::nullopt;
    }

    // Connect the socket to a public ip (here 2a00::1) on any
    // port. This will give the local address of the network interface
    // used for default routing which is usually what we want.
    sockaddr_in6 address = priv::SocketImpl::
        createAddress(std::array<std::uint8_t,
                                 16>{0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
                      9);
    if (connect(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
    {
        priv::SocketImpl::close(sock);

        err() << "Failed to retrieve local address (socket connection failure)" << std::endl;
        return std::nullopt;
    }

    // Get the local address of the socket connection
    priv::SocketImpl::AddrLength size = sizeof(address);
    if (getsockname(sock, reinterpret_cast<sockaddr*>(&address), &size) == -1)
    {
        priv::SocketImpl::close(sock);

        err() << "Failed to retrieve local address (socket local address retrieval failure)" << std::endl;
        return std::nullopt;
    }

    // Close the socket
    priv::SocketImpl::close(sock);

    // Finally build the IP address
    return IpAddress(
        {address.sin6_addr.s6_addr[0],
         address.sin6_addr.s6_addr[1],
         address.sin6_addr.s6_addr[2],
         address.sin6_addr.s6_addr[3],
         address.sin6_addr.s6_addr[4],
         address.sin6_addr.s6_addr[5],
         address.sin6_addr.s6_addr[6],
         address.sin6_addr.s6_addr[7],
         address.sin6_addr.s6_addr[8],
         address.sin6_addr.s6_addr[9],
         address.sin6_addr.s6_addr[10],
         address.sin6_addr.s6_addr[11],
         address.sin6_addr.s6_addr[12],
         address.sin6_addr.s6_addr[13],
         address.sin6_addr.s6_addr[14],
         address.sin6_addr.s6_addr[15]});
}


////////////////////////////////////////////////////////////
std::optional<IpAddress> IpAddress::getPublicAddress(Time timeout, Type type, bool secure)
{
    // The trick here is more complicated, because the only way
    // to get our public IP address is to get it from a distant computer.

    // Our own DNS implementation does not support DNSSEC, DNS-over-TLS or DNS-over-HTTPS
    // Both DoT and DoH require communication over a TLS connection anyway
    // so there is no advantage when using DNS to perform secure queries instead of HTTPS
    if (!secure)
    {
        // Try first via DNS query
        if (const auto addressViaDns = Dns::getPublicAddress(timeout, type); addressViaDns)
            return addressViaDns;
    }

    // DNS query wasn't successful, try via HTTP
    // Here we get the web page from e.g. http://ifconfig.co/ip
    // and parse the result to extract our IP address
    // (not very hard: the web page contains only our IP address).
    for (const auto provider : {"ifconfig.co"sv, "ifconfig.me"sv, "icanhazip.com"sv, "ipinfo.io"sv, "v6.ipinfo.io"sv})
    {
        if (type != Type::Any)
        {
            // Make sure the provider has addresses that match the query type
            if (const auto providerAddresses = Dns::resolve(provider);
                (std::find_if(providerAddresses.begin(),
                              providerAddresses.end(),
                              [&type](const auto& address) { return address.getType() == type; }) ==
                 providerAddresses.end()))
                continue;
        }

        const Http                   server((secure ? "https://" : "http://") + std::string(provider), 0, type);
        const Http::Request          request("/ip", Http::Request::Method::Get);
        const Http::Response         page   = server.sendRequest(request, timeout, secure);
        const Http::Response::Status status = page.getStatus();

        if (status == Http::Response::Status::Ok)
        {
            static constexpr std::array charset = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B',
                                                   'C', 'D', 'E', 'F', 'a', 'b', 'c', 'd', 'e', 'f', '.', ':'};
            auto                        body    = page.getBody();
            body.erase(std::remove_if(body.begin(),
                                      body.end(),
                                      [](char c) {
                                          return std::none_of(charset.begin(),
                                                              charset.end(),
                                                              [&c](char cc) { return cc == c; });
                                      }),
                       body.end());

            if (const auto address = IpAddress::fromString(body); address)
            {
                // If the user explicitly requested a specific type of
                // address and we got the other type we skip it
                if ((type == Type::IpV4) && address->isV6())
                    continue;

                if ((type == Type::IpV6) && address->isV4())
                    continue;

                return address;
            }
        }
    }

    return std::nullopt;
}


////////////////////////////////////////////////////////////
bool operator==(IpAddress left, IpAddress right)
{
    return !(left < right) && !(right < left);
}


////////////////////////////////////////////////////////////
bool operator!=(IpAddress left, IpAddress right)
{
    return !(left == right);
}


////////////////////////////////////////////////////////////
bool operator<(IpAddress left, IpAddress right)
{
    return left.m_address < right.m_address;
}


////////////////////////////////////////////////////////////
bool operator>(IpAddress left, IpAddress right)
{
    return right < left;
}


////////////////////////////////////////////////////////////
bool operator<=(IpAddress left, IpAddress right)
{
    return !(right < left);
}


////////////////////////////////////////////////////////////
bool operator>=(IpAddress left, IpAddress right)
{
    return !(left < right);
}


////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& stream, std::optional<IpAddress>& address)
{
    std::string str;
    stream >> str;

    address = IpAddress::fromString(str);

    return stream;
}


////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& stream, IpAddress address)
{
    return stream << address.toString();
}

} // namespace sf

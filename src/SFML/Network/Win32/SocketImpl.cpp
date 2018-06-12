////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Network/Win32/SocketImpl.hpp>
#include <cstring>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
SocketAddress createAddress(const std::experimental::net::ip::address& address, unsigned short port);
{
    SocketAddress addr;
    std::memset(&addr, 0, sizeof(addr));
    if (address.is_v4())
    {
        addr.size                        = sizeof(sockaddr_in);
        addr.memory.ipv4.sin_addr.s_addr = htonl(address.to_v4().to_uint());
        addr.memory.ipv4.sin_family      = AF_INET;
        addr.memory.ipv4.sin_port        = htons(port);
    }
    else
    {
        std::experimental::net::ip::address_v6::bytes_type bytes = address.to_v6().to_bytes();
        for(int i = 0; i < 16; i++)
            addr.memory.ipv6.sin6_addr.s6_addr[i] = bytes[i];
        addr.size                    = sizeof(sockaddr_in6);
        addr.memory.ipv6.sin6_family = AF_INET6;
        addr.memory.ipv6.sin6_port   = htons(port);
    }

    return addr;
}


////////////////////////////////////////////////////////////
SocketHandle SocketImpl::invalidSocket()
{
    return INVALID_SOCKET;
}


////////////////////////////////////////////////////////////
void SocketImpl::close(SocketHandle sock)
{
    closesocket(sock);
}


////////////////////////////////////////////////////////////
void SocketImpl::setBlocking(SocketHandle sock, bool block)
{
    u_long blocking = block ? 0 : 1;
    ioctlsocket(sock, FIONBIO, &blocking);
}


////////////////////////////////////////////////////////////
Socket::Status SocketImpl::getErrorStatus()
{
    switch (WSAGetLastError())
    {
        case WSAEWOULDBLOCK:  return Socket::NotReady;
        case WSAEALREADY:     return Socket::NotReady;
        case WSAECONNABORTED: return Socket::Disconnected;
        case WSAECONNRESET:   return Socket::Disconnected;
        case WSAETIMEDOUT:    return Socket::Disconnected;
        case WSAENETRESET:    return Socket::Disconnected;
        case WSAENOTCONN:     return Socket::Disconnected;
        case WSAEISCONN:      return Socket::Done; // when connecting a non-blocking socket
        default:              return Socket::Error;
    }
}


////////////////////////////////////////////////////////////
// Windows needs some initialization and cleanup to get
// sockets working properly... so let's create a class that will
// do it automatically
////////////////////////////////////////////////////////////
struct SocketInitializer
{
    SocketInitializer()
    {
        WSADATA init;
        WSAStartup(MAKEWORD(2, 2), &init);
    }

    ~SocketInitializer()
    {
        WSACleanup();
    }
};

SocketInitializer globalInitializer;

} // namespace priv

} // namespace sf

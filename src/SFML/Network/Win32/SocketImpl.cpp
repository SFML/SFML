////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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


namespace sf::priv
{
////////////////////////////////////////////////////////////
sockaddr_in SocketImpl::createAddress(std::uint32_t address, unsigned short port)
{
    auto addr            = sockaddr_in();
    addr.sin_addr.s_addr = htonl(address);
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);

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
    ioctlsocket(sock, static_cast<long>(FIONBIO), &blocking);
}


////////////////////////////////////////////////////////////
Socket::Status SocketImpl::getErrorStatus()
{
    // clang-format off
    switch (WSAGetLastError())
    {
        case WSAEWOULDBLOCK:  return Socket::Status::NotReady;
        case WSAEALREADY:     return Socket::Status::NotReady;
        case WSAECONNABORTED: return Socket::Status::Disconnected;
        case WSAECONNRESET:   return Socket::Status::Disconnected;
        case WSAETIMEDOUT:    return Socket::Status::Disconnected;
        case WSAENETRESET:    return Socket::Status::Disconnected;
        case WSAENOTCONN:     return Socket::Status::Disconnected;
        case WSAEISCONN:      return Socket::Status::Done; // when connecting a non-blocking socket
        default:              return Socket::Status::Error;
    }
    // clang-format on
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

} // namespace sf::priv

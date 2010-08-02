////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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
sockaddr_in SocketImpl::CreateAddress(unsigned long address, unsigned short port)
{
    sockaddr_in addr;
    std::memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
    addr.sin_addr.s_addr = htonl(address);
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);

    return addr;
}


////////////////////////////////////////////////////////////
SocketHandle SocketImpl::InvalidSocket()
{
    return INVALID_SOCKET;
}


////////////////////////////////////////////////////////////
void SocketImpl::Close(SocketHandle sock)
{
    closesocket(sock);
}


////////////////////////////////////////////////////////////
void SocketImpl::SetBlocking(SocketHandle sock, bool block)
{
    unsigned long blocking = block ? 0 : 1;
    ioctlsocket(sock, FIONBIO, &blocking);
}


////////////////////////////////////////////////////////////
Socket::Status SocketImpl::GetErrorStatus()
{
    switch (WSAGetLastError())
    {
        case WSAEWOULDBLOCK :  return Socket::NotReady;
        case WSAECONNABORTED : return Socket::Disconnected;
        case WSAECONNRESET :   return Socket::Disconnected;
        case WSAETIMEDOUT :    return Socket::Disconnected;
        case WSAENETRESET :    return Socket::Disconnected;
        case WSAENOTCONN :     return Socket::Disconnected;
        default :              return Socket::Error;
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

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
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/SocketImpl.hpp>
#include <SFML/System/Err.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
Socket::Socket(Type type) :
myType      (type),
mySocket    (priv::SocketImpl::InvalidSocket()),
myIsBlocking(true)
{

}


////////////////////////////////////////////////////////////
Socket::~Socket()
{
    // Close the socket before it gets destructed
    Close();
}


////////////////////////////////////////////////////////////
void Socket::SetBlocking(bool blocking)
{
    // Apply if the socket is already created
    if (mySocket != priv::SocketImpl::InvalidSocket())
        priv::SocketImpl::SetBlocking(mySocket, blocking);

    myIsBlocking = blocking;
}


////////////////////////////////////////////////////////////
bool Socket::IsBlocking() const
{
    return myIsBlocking;
}


////////////////////////////////////////////////////////////
SocketHandle Socket::GetHandle() const
{
    return mySocket;
}


////////////////////////////////////////////////////////////
void Socket::Create()
{
    // Don't create the socket if it already exists
    if (mySocket == priv::SocketImpl::InvalidSocket())
    {
        SocketHandle handle = socket(PF_INET, myType == Tcp ? SOCK_STREAM : SOCK_DGRAM, 0);
        Create(handle);
    }
}


////////////////////////////////////////////////////////////
void Socket::Create(SocketHandle handle)
{
    // Don't create the socket if it already exists
    if (mySocket == priv::SocketImpl::InvalidSocket())
    {
        // Assign the new handle
        mySocket = handle;

        // Set the current blocking state
        SetBlocking(myIsBlocking);

        if (myType == Tcp)
        {
            // Disable the Nagle algorithm (ie. removes buffering of TCP packets)
            int yes = 1;
            if (setsockopt(mySocket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1)
            {
                Err() << "Failed to set socket option \"TCP_NODELAY\" ; "
                      << "all your TCP packets will be buffered" << std::endl;
            }
        }
        else
        {
            // Enable broadcast by default for UDP sockets
            int yes = 1;
            if (setsockopt(mySocket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1)
            {
                Err() << "Failed to enable broadcast on UDP socket" << std::endl;
            }
        }
    }
}


////////////////////////////////////////////////////////////
void Socket::Close()
{
    // Close the socket
    if (mySocket != priv::SocketImpl::InvalidSocket())
    {
        priv::SocketImpl::Close(mySocket);
        mySocket = priv::SocketImpl::InvalidSocket();
    }

    // Reset the pending packet data
    myPendingPacket = PendingPacket();
}


////////////////////////////////////////////////////////////
Socket::PendingPacket::PendingPacket() :
Size        (0),
SizeReceived(0),
Data        ()
{

}

} // namespace sf

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
#include <SFML/Network/SocketImpl.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <SFML/System/Err.hpp>

#include <ostream>

#include <cstring>


namespace sf
{
////////////////////////////////////////////////////////////
TcpListener::TcpListener() : Socket(Type::Tcp)
{
}


////////////////////////////////////////////////////////////
unsigned short TcpListener::getLocalPort() const
{
    if (getNativeHandle() != priv::SocketImpl::invalidSocket())
    {
        // Retrieve information about the local end of the socket
        sockaddr_in6                 addressV6{};
        priv::SocketImpl::AddrLength size = sizeof(addressV6);
        if (getsockname(getNativeHandle(), reinterpret_cast<sockaddr*>(&addressV6), &size) != -1)
        {
            if (addressV6.sin6_family == PF_INET6)
                return ntohs(addressV6.sin6_port);

            if (addressV6.sin6_family == PF_INET)
            {
                sockaddr_in addressV4{};
                std::memcpy(&addressV4, &addressV6, sizeof(addressV4));
                return ntohs(addressV4.sin_port);
            }
        }
    }

    // We failed to retrieve the port
    return 0;
}


////////////////////////////////////////////////////////////
Socket::Status TcpListener::listen(unsigned short port, IpAddress address)
{
    // Close the socket if it is already bound
    close();

    // Check if the address is valid
    if (address == IpAddress::Broadcast)
        return Status::Error;

    // Bind the socket to the specified port
    sockaddr_in                  addressV4{};
    sockaddr_in6                 addressV6{};
    sockaddr*                    sockaddrPtr{};
    priv::SocketImpl::AddrLength sockaddrSize{};

    if (address.isV4())
    {
        addressV4    = priv::SocketImpl::createAddress(address.toInteger(), port);
        sockaddrPtr  = reinterpret_cast<sockaddr*>(&addressV4);
        sockaddrSize = sizeof(addressV4);

        // Create the internal socket if it doesn't exist
        create(AddressFamily::IpV4);
    }
    else if (address.isV6())
    {
        addressV6    = priv::SocketImpl::createAddress(address.toBytes(), port);
        sockaddrPtr  = reinterpret_cast<sockaddr*>(&addressV6);
        sockaddrSize = sizeof(addressV6);

        // Create the internal socket if it doesn't exist
        create(AddressFamily::IpV6);

        // Disable IPv6 sockets only binding to IPv6 addresses,
        // i.e. allow them to handle both IPv4 and IPv6 simultaneously
        int no = 0;
        if (setsockopt(getNativeHandle(), IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<char*>(&no), sizeof(no)) == -1)
        {
            err() << "Failed to set socket option \"IPV6_V6ONLY\" ; "
                  << "IPv6 sockets will only handle IPv6 packets" << std::endl;
        }
    }

    // Bind the socket
    if (::bind(getNativeHandle(), sockaddrPtr, sockaddrSize) == -1)
    {
        // Not likely to happen, but...
        err() << "Failed to bind listener socket to port " << port << std::endl;
        return Status::Error;
    }

    // Listen to the bound port
    if (::listen(getNativeHandle(), SOMAXCONN) == -1)
    {
        // Oops, socket is deaf
        err() << "Failed to listen to port " << port << std::endl;
        return Status::Error;
    }

    return Status::Done;
}


////////////////////////////////////////////////////////////
void TcpListener::close()
{
    // Simply close the socket
    Socket::close();
}


////////////////////////////////////////////////////////////
Socket::Status TcpListener::accept(TcpSocket& socket)
{
    // Make sure that we're listening
    if (getNativeHandle() == priv::SocketImpl::invalidSocket())
    {
        err() << "Failed to accept a new connection, the socket is not listening" << std::endl;
        return Status::Error;
    }

    // Accept a new connection
    sockaddr_in6                 address{};
    priv::SocketImpl::AddrLength length = sizeof(address);
    const SocketHandle           remote = ::accept(getNativeHandle(), reinterpret_cast<sockaddr*>(&address), &length);

    // Check for errors
    if (remote == priv::SocketImpl::invalidSocket())
        return priv::SocketImpl::getErrorStatus();

    // Initialize the new connected socket
    socket.close();
    socket.create(remote);

    return Status::Done;
}

} // namespace sf

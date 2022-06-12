////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/SocketImpl.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Err.hpp>

#include <cstddef>
#include <ostream>


namespace sf
{
////////////////////////////////////////////////////////////
UdpSocket::UdpSocket() : Socket(Type::Udp), m_buffer(MaxDatagramSize)
{
}


////////////////////////////////////////////////////////////
unsigned short UdpSocket::getLocalPort() const
{
    if (getHandle() != priv::SocketImpl::invalidSocket())
    {
        // Retrieve informations about the local end of the socket
        sockaddr_in                  address;
        priv::SocketImpl::AddrLength size = sizeof(address);
        if (getsockname(getHandle(), reinterpret_cast<sockaddr*>(&address), &size) != -1)
        {
            return ntohs(address.sin_port);
        }
    }

    // We failed to retrieve the port
    return 0;
}


////////////////////////////////////////////////////////////
Socket::Status UdpSocket::bind(unsigned short port, const IpAddress& address)
{
    // Close the socket if it is already bound
    close();

    // Create the internal socket if it doesn't exist
    create();

    // Check if the address is valid
    if (address == IpAddress::Broadcast)
        return Status::Error;

    // Bind the socket
    sockaddr_in addr = priv::SocketImpl::createAddress(address.toInteger(), port);
    if (::bind(getHandle(), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1)
    {
        err() << "Failed to bind socket to port " << port << std::endl;
        return Status::Error;
    }

    return Status::Done;
}


////////////////////////////////////////////////////////////
void UdpSocket::unbind()
{
    // Simply close the socket
    close();
}


////////////////////////////////////////////////////////////
Socket::Status UdpSocket::send(const void* data, std::size_t size, const IpAddress& remoteAddress, unsigned short remotePort)
{
    // Create the internal socket if it doesn't exist
    create();

    // Make sure that all the data will fit in one datagram
    if (size > MaxDatagramSize)
    {
        err() << "Cannot send data over the network "
              << "(the number of bytes to send is greater than sf::UdpSocket::MaxDatagramSize)" << std::endl;
        return Status::Error;
    }

    // Build the target address
    sockaddr_in address = priv::SocketImpl::createAddress(remoteAddress.toInteger(), remotePort);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
    // Send the data (unlike TCP, all the data is always sent in one call)
    int sent = static_cast<int>(
        sendto(getHandle(),
               static_cast<const char*>(data),
               static_cast<priv::SocketImpl::Size>(size),
               0,
               reinterpret_cast<sockaddr*>(&address),
               sizeof(address)));
#pragma GCC diagnostic pop

    // Check for errors
    if (sent < 0)
        return priv::SocketImpl::getErrorStatus();

    return Status::Done;
}


////////////////////////////////////////////////////////////
Socket::Status UdpSocket::receive(void*                     data,
                                  std::size_t               size,
                                  std::size_t&              received,
                                  std::optional<IpAddress>& remoteAddress,
                                  unsigned short&           remotePort)
{
    // First clear the variables to fill
    received      = 0;
    remoteAddress = std::nullopt;
    remotePort    = 0;

    // Check the destination buffer
    if (!data)
    {
        err() << "Cannot receive data from the network (the destination buffer is invalid)" << std::endl;
        return Status::Error;
    }

    // Data that will be filled with the other computer's address
    sockaddr_in address = priv::SocketImpl::createAddress(INADDR_ANY, 0);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
    // Receive a chunk of bytes
    priv::SocketImpl::AddrLength addressSize  = sizeof(address);
    int                          sizeReceived = static_cast<int>(
        recvfrom(getHandle(),
                 static_cast<char*>(data),
                 static_cast<priv::SocketImpl::Size>(size),
                 0,
                 reinterpret_cast<sockaddr*>(&address),
                 &addressSize));
#pragma GCC diagnostic pop

    // Check for errors
    if (sizeReceived < 0)
        return priv::SocketImpl::getErrorStatus();

    // Fill the sender informations
    received      = static_cast<std::size_t>(sizeReceived);
    remoteAddress = IpAddress(ntohl(address.sin_addr.s_addr));
    remotePort    = ntohs(address.sin_port);

    return Status::Done;
}


////////////////////////////////////////////////////////////
Socket::Status UdpSocket::send(Packet& packet, const IpAddress& remoteAddress, unsigned short remotePort)
{
    // UDP is a datagram-oriented protocol (as opposed to TCP which is a stream protocol).
    // Sending one datagram is almost safe: it may be lost but if it's received, then its data
    // is guaranteed to be ok. However, splitting a packet into multiple datagrams would be highly
    // unreliable, since datagrams may be reordered, dropped or mixed between different sources.
    // That's why SFML imposes a limit on packet size so that they can be sent in a single datagram.
    // This also removes the overhead associated to packets -- there's no size to send in addition
    // to the packet's data.

    // Get the data to send from the packet
    std::size_t size = 0;
    const void* data = packet.onSend(size);

    // Send it
    return send(data, size, remoteAddress, remotePort);
}


////////////////////////////////////////////////////////////
Socket::Status UdpSocket::receive(Packet& packet, std::optional<IpAddress>& remoteAddress, unsigned short& remotePort)
{
    // See the detailed comment in send(Packet) above.

    // Receive the datagram
    std::size_t received = 0;
    Status      status   = receive(m_buffer.data(), m_buffer.size(), received, remoteAddress, remotePort);

    // If we received valid data, we can copy it to the user packet
    packet.clear();
    if ((status == Status::Done) && (received > 0))
        packet.onReceive(m_buffer.data(), received);

    return status;
}


} // namespace sf

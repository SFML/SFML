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
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/SocketImpl.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include <SFML/System/Err.hpp>

#include <ostream>

#include <cstddef>
#include <cstring>


namespace sf
{
////////////////////////////////////////////////////////////
UdpSocket::UdpSocket() : Socket(Type::Udp)
{
}


////////////////////////////////////////////////////////////
unsigned short UdpSocket::getLocalPort() const
{
    if (getNativeHandle() != priv::SocketImpl::invalidSocket())
    {
        // Retrieve information about the local end of the socket
        // We try first using sockaddr_in6 since it is the bigger structure
        // If the sin6_family field indicates we have an IPv4 socket retry with sockaddr_in
        {
            sockaddr_in6                 address{};
            priv::SocketImpl::AddrLength size = sizeof(address);
            if (getsockname(getNativeHandle(), reinterpret_cast<sockaddr*>(&address), &size) != -1)
            {
                if (address.sin6_family == PF_INET6)
                    return ntohs(address.sin6_port);

                // Give up if the socket isn't IPv4 either
                if (address.sin6_family != PF_INET)
                    return 0;
            }
        }

        // Retry for IPv4
        {
            sockaddr_in                  address{};
            priv::SocketImpl::AddrLength size = sizeof(address);
            if (getsockname(getNativeHandle(), reinterpret_cast<sockaddr*>(&address), &size) != -1)
                return ntohs(address.sin_port);
        }
    }

    // We failed to retrieve the port
    return 0;
}


////////////////////////////////////////////////////////////
Socket::Status UdpSocket::bind(unsigned short port, IpAddress address)
{
    // Close the socket if it is already bound
    close();

    // Check if the address is valid
    if (address == IpAddress::Broadcast)
        return Status::Error;

    // Create the remote address
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
Socket::Status UdpSocket::send(const void* data, std::size_t size, IpAddress remoteAddress, unsigned short remotePort)
{
    // Make sure that all the data will fit in one datagram
    if (size > MaxDatagramSize)
    {
        err() << "Cannot send data over the network "
              << "(the number of bytes to send is greater than sf::UdpSocket::MaxDatagramSize)" << std::endl;
        return Status::Error;
    }

    // Create the remote address
    sockaddr_in                  addressV4{};
    sockaddr_in6                 addressV6{};
    sockaddr*                    sockaddrPtr{};
    priv::SocketImpl::AddrLength sockaddrSize{};

    if (remoteAddress.isV4())
    {
        addressV4    = priv::SocketImpl::createAddress(remoteAddress.toInteger(), remotePort);
        sockaddrPtr  = reinterpret_cast<sockaddr*>(&addressV4);
        sockaddrSize = sizeof(addressV4);

        // Create the internal socket if it doesn't exist
        create(AddressFamily::IpV4);
    }
    else if (remoteAddress.isV6())
    {
        addressV6    = priv::SocketImpl::createAddress(remoteAddress.toBytes(), remotePort);
        sockaddrPtr  = reinterpret_cast<sockaddr*>(&addressV6);
        sockaddrSize = sizeof(addressV6);

        // Create the internal socket if it doesn't exist
        create(AddressFamily::IpV6);
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
    // Send the data (unlike TCP, all the data is always sent in one call)
    const int sent = static_cast<int>(
        sendto(getNativeHandle(),
               static_cast<const char*>(data),
               static_cast<priv::SocketImpl::Size>(size),
               0,
               sockaddrPtr,
               sockaddrSize));
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
    sockaddr_in6 address{};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
    // Receive a chunk of bytes
    priv::SocketImpl::AddrLength addressSize  = sizeof(address);
    const int                    sizeReceived = static_cast<int>(
        recvfrom(getNativeHandle(),
                 static_cast<char*>(data),
                 static_cast<priv::SocketImpl::Size>(size),
                 0,
                 reinterpret_cast<sockaddr*>(&address),
                 &addressSize));
#pragma GCC diagnostic pop

    // Check for errors
    if (sizeReceived < 0)
        return priv::SocketImpl::getErrorStatus();

    // Fill the sender information
    received = static_cast<std::size_t>(sizeReceived);

    if (address.sin6_family == PF_INET6)
    {
        remoteAddress = IpAddress(
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
        remotePort = ntohs(address.sin6_port);
    }
    else if (address.sin6_family == PF_INET)
    {
        sockaddr_in addressV4{};
        std::memcpy(&addressV4, &address, sizeof(addressV4));
        remoteAddress = IpAddress(ntohl(addressV4.sin_addr.s_addr));
        remotePort    = ntohs(addressV4.sin_port);
    }

    return Status::Done;
}


////////////////////////////////////////////////////////////
Socket::Status UdpSocket::send(Packet& packet, IpAddress remoteAddress, unsigned short remotePort)
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
    std::size_t  received = 0;
    const Status status   = receive(m_buffer.data(), m_buffer.size(), received, remoteAddress, remotePort);

    // If we received valid data, we can copy it to the user packet
    packet.clear();
    if ((status == Status::Done) && (received > 0))
        packet.onReceive(m_buffer.data(), received);

    return status;
}


} // namespace sf

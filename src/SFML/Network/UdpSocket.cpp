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
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/SocketImpl.hpp>
#include <SFML/System/Err.hpp>
#include <algorithm>


namespace sf
{
////////////////////////////////////////////////////////////
UdpSocket::UdpSocket() :
Socket  (Udp),
myBuffer(MaxDatagramSize)
{

}


////////////////////////////////////////////////////////////
unsigned short UdpSocket::GetLocalPort() const
{
    if (GetHandle() != priv::SocketImpl::InvalidSocket())
    {
        // Retrieve informations about the local end of the socket
        sockaddr_in address;
        priv::SocketImpl::AddrLength size = sizeof(address);
        if (getsockname(GetHandle(), reinterpret_cast<sockaddr*>(&address), &size) != -1)
        {
            return ntohs(address.sin_port);
        }
    }

    // We failed to retrieve the port
    return 0;
}


////////////////////////////////////////////////////////////
Socket::Status UdpSocket::Bind(unsigned short port)
{
    // Create the internal socket if it doesn't exist
    Create();

    // Bind the socket
    sockaddr_in address = priv::SocketImpl::CreateAddress(INADDR_ANY, port);
    if (bind(GetHandle(), reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
    {
        Err() << "Failed to bind socket to port " << port << std::endl;
        return Error;
    }

    return Done;
}


////////////////////////////////////////////////////////////
void UdpSocket::Unbind()
{
    // Simply close the socket
    Close();
}


////////////////////////////////////////////////////////////
Socket::Status UdpSocket::Send(const char* data, std::size_t size, const IpAddress& remoteAddress, unsigned short remotePort)
{
    // Create the internal socket if it doesn't exist
    Create();

    // Make sure that all the data will fit in one datagram
    if (size > MaxDatagramSize)
    {
        Err() << "Cannot send data over the network "
              << "(the number of bytes to send is greater than sf::UdpSocket::MaxDatagramSize)" << std::endl;
        return Error;
    }

    // Build the target address
    sockaddr_in address = priv::SocketImpl::CreateAddress(remoteAddress.ToInteger(), remotePort);

    // Send the data (unlike TCP, all the data is always sent in one call)
    int sent = sendto(GetHandle(), data, static_cast<int>(size), 0, reinterpret_cast<sockaddr*>(&address), sizeof(address));

    // Check for errors
    if (sent < 0)
        return priv::SocketImpl::GetErrorStatus();

    return Done;
}


////////////////////////////////////////////////////////////
Socket::Status UdpSocket::Receive(char* data, std::size_t size, std::size_t& received, IpAddress& remoteAddress, unsigned short& remotePort)
{
    // First clear the variables to fill
    received      = 0;
    remoteAddress = IpAddress();
    remotePort    = 0;

    // Check the destination buffer
    if (!data)
    {
        Err() << "Cannot receive data from the network (the destination buffer is invalid)" << std::endl;
        return Error;
    }

    // Data that will be filled with the other computer's address
    sockaddr_in address = priv::SocketImpl::CreateAddress(INADDR_ANY, 0);

    // Receive a chunk of bytes
    priv::SocketImpl::AddrLength addressSize = sizeof(address);
    int sizeReceived = recvfrom(GetHandle(), data, static_cast<int>(size), 0, reinterpret_cast<sockaddr*>(&address), &addressSize);

    // Check for errors
    if (sizeReceived < 0)
        return priv::SocketImpl::GetErrorStatus();

    // Fill the sender informations
    received      = static_cast<std::size_t>(sizeReceived);
    remoteAddress = IpAddress(ntohl(address.sin_addr.s_addr));
    remotePort    = ntohs(address.sin_port);

    return Done;
}


////////////////////////////////////////////////////////////
Socket::Status UdpSocket::Send(Packet& packet, const IpAddress& remoteAddress, unsigned short remotePort)
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
    const char* data = packet.OnSend(size);

    // Send it
    return Send(data, size, remoteAddress, remotePort);
}


////////////////////////////////////////////////////////////
Socket::Status UdpSocket::Receive(Packet& packet, IpAddress& remoteAddress, unsigned short& remotePort)
{
    // See the detailed comment in Send(Packet) above.

    // Receive the datagram
    std::size_t received = 0;
    Status status = Receive(&myBuffer[0], myBuffer.size(), received, remoteAddress, remotePort);

    // If we received valid data, we can copy it to the user packet
    packet.Clear();
    if ((status == Done) && (received > 0))
        packet.OnReceive(&myBuffer[0], received);

    return status;
}


} // namespace sf

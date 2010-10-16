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
Socket(Udp)
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
    // As the UDP protocol preserves datagrams boundaries, we don't have to
    // send the packet size first (it would even be a potential source of bug, if
    // that size arrives corrupted), but we must split the packet into multiple
    // pieces if data size is greater than the maximum datagram size.

    // Get the data to send from the packet
    std::size_t size = 0;
    const char* data = packet.OnSend(size);

    // If size is greater than MaxDatagramSize, the data must be split into multiple datagrams
    while (size >= MaxDatagramSize)
    {
        Status status = Send(data, MaxDatagramSize, remoteAddress, remotePort);
        if (status != Done)
            return status;

        data += MaxDatagramSize;
        size -= MaxDatagramSize;
    }

    // It is important to send a final datagram with a size < MaxDatagramSize,
    // even if it is zero, to mark the end of the packet
    return Send(data, size, remoteAddress, remotePort);
}


////////////////////////////////////////////////////////////
Socket::Status UdpSocket::Receive(Packet& packet, IpAddress& remoteAddress, unsigned short& remotePort)
{
    // First clear the variables to fill
    packet.Clear();
    remoteAddress = IpAddress();
    remotePort    = 0;

    // Receive datagrams
    std::size_t received = 0;
    std::size_t size = myPendingPacket.Data.size();
    do
    {
        // Make room in the data buffer for a new datagram
        myPendingPacket.Data.resize(size + MaxDatagramSize);
        char* data = &myPendingPacket.Data[0] + size;

        // Receive the datagram
        Status status = Receive(data, MaxDatagramSize, received, remoteAddress, remotePort);

        // Check for errors
        if (status != Done)
        {
            myPendingPacket.Data.resize(size + received);
            return status;
        }
    }
    while (received == MaxDatagramSize);

    // We have received all the packet data: we can copy it to the user packet
    std::size_t actualSize = size + received;
    if (actualSize > 0)
        packet.OnReceive(&myPendingPacket.Data[0], actualSize);

    // Clear the pending packet data
    myPendingPacket = PendingPacket();

    return Done;
}


} // namespace sf

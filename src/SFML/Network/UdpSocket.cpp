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
#include <string.h>


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

    // Check the parameters
    if (!data || (size == 0))
    {
        Err() << "Cannot send data over the network (invalid parameters)" << std::endl;
        return Error;
    }

    // Build the target address
    sockaddr_in address = priv::SocketImpl::CreateAddress(remoteAddress.ToInteger(), remotePort);

    // Loop until every byte has been sent
    int sent = 0;
    int sizeToSend = static_cast<int>(size);
    for (int length = 0; length < sizeToSend; length += sent)
    {
        // Send a chunk of data
        sent = sendto(GetHandle(), data + length, sizeToSend - length, 0, reinterpret_cast<sockaddr*>(&address), sizeof(address));

        // Check for errors
        if (sent <= 0)
            return priv::SocketImpl::GetErrorStatus();
    }

    return Done;
}


////////////////////////////////////////////////////////////
Socket::Status UdpSocket::Receive(char* data, std::size_t size, std::size_t& received, IpAddress& remoteAddress, unsigned short& remotePort)
{
    // First clear the variables to fill
    received      = 0;
    remoteAddress = IpAddress();
    remotePort    = 0;

    // Check the parameters
    if (!data || (size == 0))
    {
        Err() << "Cannot receive data from the network (invalid parameters)" << std::endl;
        return Error;
    }

    // Data that will be filled with the other computer's address
    sockaddr_in address = priv::SocketImpl::CreateAddress(INADDR_ANY, 0);

    // Receive a chunk of bytes
    priv::SocketImpl::AddrLength addressSize = sizeof(address);
    int sizeReceived = recvfrom(GetHandle(), data, static_cast<int>(size), 0, reinterpret_cast<sockaddr*>(&address), &addressSize);

    // Check for errors
    if (sizeReceived <= 0)
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
    // Get the data to send from the packet
    std::size_t size = 0;
    const char* data = packet.OnSend(size);

    // First send the packet size
    Uint32 packetSize = htonl(static_cast<unsigned long>(size));
    Status status = Send(reinterpret_cast<const char*>(&packetSize), sizeof(packetSize), remoteAddress, remotePort);

    // Make sure that the size was properly sent
    if (status != Done)
        return status;

    // Finally send the packet data
    if (packetSize > 0)
    {
        return Send(data, size, remoteAddress, remotePort);
    }
    else
    {
        return Done;
    }
}


////////////////////////////////////////////////////////////
Socket::Status UdpSocket::Receive(Packet& packet, IpAddress& remoteAddress, unsigned short& remotePort)
{
    // First clear the variables to fill
    packet.Clear();
    remoteAddress = IpAddress();
    remotePort    = 0;

    // We start by getting the size of the incoming packet
    Uint32 packetSize = 0;
    std::size_t received = 0;
    if (myPendingPacket.SizeReceived < sizeof(myPendingPacket.Size))
    {
        // Loop until we've received the entire size of the packet
        // (even a 4 bytes variable may be received in more than one call)
        while (myPendingPacket.SizeReceived < sizeof(myPendingPacket.Size))
        {
            char* data = reinterpret_cast<char*>(&myPendingPacket.Size) + myPendingPacket.SizeReceived;
            std::size_t size = sizeof(myPendingPacket.Size) - myPendingPacket.SizeReceived;
            Status status = Receive(data, size, received, remoteAddress, remotePort);
            myPendingPacket.SizeReceived += received;

            if (status != Done)
                return status;
        }

        // The packet size has been fully received
        packetSize = ntohl(myPendingPacket.Size);
    }
    else
    {
        // The packet size has already been received in a previous call
        packetSize = ntohl(myPendingPacket.Size);
    }

    // Use another address instance for receiving the packet data,
    // chunks of data coming from a different sender will be discarded (and lost...)
    IpAddress currentSender;
    unsigned short currentPort;

    // Loop until we receive all the packet data
    char buffer[1024];
    while (myPendingPacket.Data.size() < packetSize)
    {
        // Receive a chunk of data
        std::size_t sizeToGet = std::min(static_cast<std::size_t>(packetSize - myPendingPacket.Data.size()), sizeof(buffer));
        Status status = Receive(buffer, sizeToGet, received, currentSender, currentPort);
        if (status != Done)
            return status;

        // Append it into the packet
        if ((currentSender == remoteAddress) && (currentPort == remotePort) && (received > 0))
        {
            myPendingPacket.Data.resize(myPendingPacket.Data.size() + received);
            char* begin = &myPendingPacket.Data[0] + myPendingPacket.Data.size() - received;
            memcpy(begin, buffer, received);
        }
    }

    // We have received all the packet data: we can copy it to the user packet
    if (!myPendingPacket.Data.empty())
        packet.OnReceive(&myPendingPacket.Data[0], myPendingPacket.Data.size());

    // Clear the pending packet data
    myPendingPacket = PendingPacket();

    return Done;
}


} // namespace sf

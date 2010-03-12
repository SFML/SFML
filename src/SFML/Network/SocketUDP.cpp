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
#include <SFML/Network/SocketUDP.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Err.hpp>
#include <algorithm>
#include <string.h>


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
SocketUDP::SocketUDP()
{
    Create();
}


////////////////////////////////////////////////////////////
/// Change the blocking state of the socket
////////////////////////////////////////////////////////////
void SocketUDP::SetBlocking(bool blocking)
{
    // Make sure our socket is valid
    if (!IsValid())
        Create();

    SocketHelper::SetBlocking(mySocket, blocking);
    myIsBlocking = blocking;
}


////////////////////////////////////////////////////////////
/// Bind the socket to a specific port
////////////////////////////////////////////////////////////
bool SocketUDP::Bind(unsigned short port)
{
    // Check if the socket is already bound to the specified port
    if (myPort != port)
    {
        // If the socket was previously bound to another port, we need to unbind it first
        Unbind();

        if (port != 0)
        {
            // Build an address with the specified port
            sockaddr_in sockAddr;
            sockAddr.sin_family      = AF_INET;
            sockAddr.sin_port        = htons(port);
            sockAddr.sin_addr.s_addr = INADDR_ANY;
            memset(sockAddr.sin_zero, 0, sizeof(sockAddr.sin_zero));

            // Bind the socket to the port
            if (bind(mySocket, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr)) == -1)
            {
                Err() << "Failed to bind the socket to port " << port << std::endl;
                myPort = 0;
                return false;
            }
        }

        // Save the new port
        myPort = port;
    }

    return true;
}


////////////////////////////////////////////////////////////
/// Unbind the socket to its previous port
////////////////////////////////////////////////////////////
bool SocketUDP::Unbind()
{
    // To unbind the socket, we just recreate it
    if (myPort != 0)
    {
        Close();
        Create();
        myPort = 0;
    }

    return true;
}


////////////////////////////////////////////////////////////
/// Send an array of bytes
////////////////////////////////////////////////////////////
Socket::Status SocketUDP::Send(const char* data, std::size_t sizeInBytes, const IpAddress& address, unsigned short port)
{
    // Make sure the socket is valid
    if (!IsValid())
        Create();

    // Check parameters
    if (data && sizeInBytes)
    {
        // Build the target address
        sockaddr_in sockAddr;
        sockAddr.sin_family      = AF_INET;
        sockAddr.sin_port        = htons(port);
        sockAddr.sin_addr.s_addr = inet_addr(address.ToString().c_str());
        memset(sockAddr.sin_zero, 0, sizeof(sockAddr.sin_zero));

        // Loop until every byte has been sent
        int sent = 0;
        int sizeToSend = static_cast<int>(sizeInBytes);
        for (int length = 0; length < sizeToSend; length += sent)
        {
            // Send a chunk of data
            sent = sendto(mySocket, data + length, sizeToSend - length, 0, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr));

            // Check errors
            if (sent <= 0)
                return SocketHelper::GetErrorStatus();
        }

        return Socket::Done;
    }
    else
    {
        // Error...
        Err() << "Cannot send data over the network (invalid parameters)" << std::endl;
        return Socket::Error;
    }
}


////////////////////////////////////////////////////////////
/// Receive an array of bytes.
/// This function will block if the socket is blocking
////////////////////////////////////////////////////////////
Socket::Status SocketUDP::Receive(char* data, std::size_t maxSize, std::size_t& sizeReceived, IpAddress& address, unsigned short& port)
{
    // First clear the size received
    sizeReceived = 0;

    // Make sure the socket is bound to a port
    if (myPort == 0)
    {
        Err() << "Failed to receive data ; the UDP socket first needs to be bound to a port" << std::endl;
        return Socket::Error;
    }

    // Make sure the socket is valid
    if (!IsValid())
        Create();

    // Check parameters
    if (data && maxSize)
    {
        // Data that will be filled with the other computer's address
        sockaddr_in sockAddr;
        sockAddr.sin_family      = AF_INET;
        sockAddr.sin_port        = 0;
        sockAddr.sin_addr.s_addr = INADDR_ANY;
        memset(sockAddr.sin_zero, 0, sizeof(sockAddr.sin_zero));
        SocketHelper::LengthType sockAddrSize = sizeof(sockAddr);

        // Receive a chunk of bytes
        int received = recvfrom(mySocket, data, static_cast<int>(maxSize), 0, reinterpret_cast<sockaddr*>(&sockAddr), &sockAddrSize);

        // Check the number of bytes received
        if (received > 0)
        {
            address = IpAddress(inet_ntoa(sockAddr.sin_addr));
            port = ntohs(sockAddr.sin_port);
            sizeReceived = static_cast<std::size_t>(received);
            return Socket::Done;
        }
        else
        {
            address = IpAddress();
            port = 0;
            return received == 0 ? Socket::Disconnected : SocketHelper::GetErrorStatus();
        }
    }
    else
    {
        // Error...
        Err() << "Cannot receive data from the network (invalid parameters)" << std::endl;
        return Socket::Error;
    }
}


////////////////////////////////////////////////////////////
/// Send a packet of data
////////////////////////////////////////////////////////////
Socket::Status SocketUDP::Send(Packet& packet, const IpAddress& address, unsigned short port)
{
    // Get the data to send from the packet
    std::size_t dataSize = 0;
    const char* data = packet.OnSend(dataSize);

    // Send the packet size
    Uint32 packetSize = htonl(static_cast<unsigned long>(dataSize));
    Send(reinterpret_cast<const char*>(&packetSize), sizeof(packetSize), address, port);

    // Send the packet data
    if (packetSize > 0)
    {
        return Send(data, dataSize, address, port);
    }
    else
    {
        return Socket::Done;
    }
}


////////////////////////////////////////////////////////////
/// Receive a packet.
/// This function will block if the socket is blocking
////////////////////////////////////////////////////////////
Socket::Status SocketUDP::Receive(Packet& packet, IpAddress& address, unsigned short& port)
{
    // We start by getting the size of the incoming packet
    Uint32      packetSize = 0;
    std::size_t received   = 0;
    if (myPendingPacketSize < 0)
    {
        // Loop until we've received the entire size of the packet
        // (even a 4 bytes variable may be received in more than one call)
        while (myPendingHeaderSize < sizeof(myPendingHeader))
        {
            char* data = reinterpret_cast<char*>(&myPendingHeader) + myPendingHeaderSize;
            Socket::Status status = Receive(data, sizeof(myPendingHeader) - myPendingHeaderSize, received, address, port);
            myPendingHeaderSize += received;

            if (status != Socket::Done)
                return status;
        }

        packetSize = ntohl(myPendingHeader);
        myPendingHeaderSize = 0;
    }
    else
    {
        // There is a pending packet : we already know its size
        packetSize = myPendingPacketSize;
    }

    // Use another address instance for receiving the packet data ;
    // chunks of data coming from a different sender will be discarded (and lost...)
    IpAddress sender;
    unsigned short senderPort;

    // Then loop until we receive all the packet data
    char buffer[1024];
    while (myPendingPacket.size() < packetSize)
    {
        // Receive a chunk of data
        std::size_t sizeToGet = std::min(static_cast<std::size_t>(packetSize - myPendingPacket.size()), sizeof(buffer));
        Socket::Status status = Receive(buffer, sizeToGet, received, sender, senderPort);
        if (status != Socket::Done)
        {
            // We must save the size of the pending packet until we can receive its content
            if (status == Socket::NotReady)
                myPendingPacketSize = packetSize;
            return status;
        }

        // Append it into the packet
        if ((sender == address) && (senderPort == port) && (received > 0))
        {
            myPendingPacket.resize(myPendingPacket.size() + received);
            char* begin = &myPendingPacket[0] + myPendingPacket.size() - received;
            memcpy(begin, buffer, received);
        }
    }

    // We have received all the datas : we can copy it to the user packet, and clear our internal packet
    packet.Clear();
    if (!myPendingPacket.empty())
        packet.OnReceive(&myPendingPacket[0], myPendingPacket.size());
    myPendingPacket.clear();
    myPendingPacketSize = -1;

    return Socket::Done;
}


////////////////////////////////////////////////////////////
/// Close the socket
////////////////////////////////////////////////////////////
bool SocketUDP::Close()
{
    if (IsValid())
    {
        if (!SocketHelper::Close(mySocket))
        {
            Err() << "Failed to close socket" << std::endl;
            return false;
        }

        mySocket = SocketHelper::InvalidSocket();
    }

    myPort       = 0;
    myIsBlocking = true;

    return true;
}


////////////////////////////////////////////////////////////
/// Check if the socket is in a valid state ; this function
/// can be called any time to check if the socket is OK
////////////////////////////////////////////////////////////
bool SocketUDP::IsValid() const
{
    return mySocket != SocketHelper::InvalidSocket();
}


////////////////////////////////////////////////////////////
/// Get the port the socket is currently bound to
////////////////////////////////////////////////////////////
unsigned short SocketUDP::GetPort() const
{
    return myPort;
}


////////////////////////////////////////////////////////////
/// Comparison operator ==
////////////////////////////////////////////////////////////
bool SocketUDP::operator ==(const SocketUDP& other) const
{
    return mySocket == other.mySocket;
}


////////////////////////////////////////////////////////////
/// Comparison operator !=
////////////////////////////////////////////////////////////
bool SocketUDP::operator !=(const SocketUDP& other) const
{
    return mySocket != other.mySocket;
}


////////////////////////////////////////////////////////////
/// Comparison operator <.
/// Provided for compatibility with standard containers, as
/// comparing two sockets doesn't make much sense...
////////////////////////////////////////////////////////////
bool SocketUDP::operator <(const SocketUDP& other) const
{
    return mySocket < other.mySocket;
}


////////////////////////////////////////////////////////////
/// Construct the socket from a socket descriptor
/// (for internal use only)
////////////////////////////////////////////////////////////
SocketUDP::SocketUDP(SocketHelper::SocketType descriptor)
{
    Create(descriptor);
}


////////////////////////////////////////////////////////////
/// Create the socket
////////////////////////////////////////////////////////////
void SocketUDP::Create(SocketHelper::SocketType descriptor)
{
    // Use the given socket descriptor, or get a new one
    mySocket = descriptor ? descriptor : socket(PF_INET, SOCK_DGRAM, 0);
    myIsBlocking = true;

    // Clear the last port used
    myPort = 0;

    // Reset the pending packet
    myPendingHeaderSize = 0;
    myPendingPacket.clear();
    myPendingPacketSize = -1;

    // Setup default options
    if (IsValid())
    {
        // To avoid the "Address already in use" error message when trying to bind to the same port
        int yes = 1;
        if (setsockopt(mySocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1)
        {
            Err() << "Failed to set socket option \"reuse address\" ; "
                  << "binding to a same port may fail if too fast" << std::endl;
        }

        // Enable broadcast by default
        if (setsockopt(mySocket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1)
        {
            Err() << "Failed to enable broadcast on UDP socket" << std::endl;
        }

        // Set blocking by default (should always be the case anyway)
        SetBlocking(true);
    }
}

} // namespace sf

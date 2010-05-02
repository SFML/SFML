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
#include <SFML/Network/UdpSocket.h>
#include <SFML/Network/UdpSocketStruct.h>
#include <SFML/Network/PacketStruct.h>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Internal.h>
#include <string.h>


////////////////////////////////////////////////////////////
/// Construct a new UDP socket
////////////////////////////////////////////////////////////
sfUdpSocket* sfUdpSocket_Create(void)
{
    return new sfUdpSocket;
}


////////////////////////////////////////////////////////////
/// Destroy an existing UDP socket
////////////////////////////////////////////////////////////
void sfUdpSocket_Destroy(sfUdpSocket* socket)
{
    delete socket;
}


////////////////////////////////////////////////////////////
/// Change the blocking state of a UDP socket.
/// The default behaviour of a socket is blocking
////////////////////////////////////////////////////////////
void sfUdpSocket_SetBlocking(sfUdpSocket* socket, sfBool blocking)
{
    CSFML_CALL(socket, SetBlocking(blocking == sfTrue));
}

////////////////////////////////////////////////////////////
/// Get the blocking state of the socket
////////////////////////////////////////////////////////////
sfBool sfUdpSocket_IsBlocking(const sfUdpSocket* socket)
{
    CSFML_CALL_RETURN(socket, IsBlocking(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the port to which the socket is bound locally
////////////////////////////////////////////////////////////
unsigned short sfUdpSocket_GetLocalPort(const sfUdpSocket* socket)
{
    CSFML_CALL_RETURN(socket, GetLocalPort(), 0);
}


////////////////////////////////////////////////////////////
/// Bind a socket to a specific port
////////////////////////////////////////////////////////////
sfSocketStatus sfUdpSocket_Bind(sfUdpSocket* socket, unsigned short port)
{
    CSFML_CHECK_RETURN(socket, sfSocketError);

    return static_cast<sfSocketStatus>(socket->This.Bind(port));
}


////////////////////////////////////////////////////////////
/// Unbind a socket from its previous port, if any
////////////////////////////////////////////////////////////
void sfUdpSocket_Unbind(sfUdpSocket* socket)
{
    CSFML_CALL(socket, Unbind());
}


////////////////////////////////////////////////////////////
/// Send an array of bytes
////////////////////////////////////////////////////////////
sfSocketStatus sfUdpSocket_Send(sfUdpSocket* socket, const char* data, size_t size, sfIpAddress address, unsigned short port)
{
    CSFML_CHECK_RETURN(socket, sfSocketError)

    // Convert the address
    sf::IpAddress receiver(address.Address);

    return static_cast<sfSocketStatus>(socket->This.Send(data, size, receiver, port));
}


////////////////////////////////////////////////////////////
/// Receive an array of bytes.
/// This function is blocking, ie. it won't return before some
/// bytes have been received
////////////////////////////////////////////////////////////
sfSocketStatus sfUdpSocket_Receive(sfUdpSocket* socket, char* data, size_t maxSize, size_t* sizeReceived, sfIpAddress* address, unsigned short* port)
{
    CSFML_CHECK_RETURN(socket, sfSocketError);

    // Call SFML internal function
    sf::IpAddress sender;
    unsigned short senderPort;
    std::size_t received;

    sf::Socket::Status status = socket->This.Receive(data, maxSize, received, sender, senderPort);
    if (status != sf::Socket::Done)
        return static_cast<sfSocketStatus>(status);

    if (sizeReceived)
        *sizeReceived = received;

    if (address)
        strncpy(address->Address, sender.ToString().c_str(), 16);

    if (port)
        *port = senderPort;

    return sfSocketDone;
}


////////////////////////////////////////////////////////////
/// Send a packet of data
////////////////////////////////////////////////////////////
sfSocketStatus sfUdpSocket_SendPacket(sfUdpSocket* socket, sfPacket* packet, sfIpAddress address, unsigned short port)
{
    CSFML_CHECK_RETURN(socket, sfSocketError);
    CSFML_CHECK_RETURN(packet, sfSocketError);

    // Convert the address
    sf::IpAddress receiver(address.Address);

    return static_cast<sfSocketStatus>(socket->This.Send(packet->This, receiver, port));
}


////////////////////////////////////////////////////////////
/// Receive a packet.
/// This function is blocking, ie. it won't return before a
/// packet is received
////////////////////////////////////////////////////////////
sfSocketStatus sfUdpSocket_ReceivePacket(sfUdpSocket* socket, sfPacket* packet, sfIpAddress* address, unsigned short* port)
{
    CSFML_CHECK_RETURN(socket, sfSocketError);
    CSFML_CHECK_RETURN(packet, sfSocketError);

    sf::IpAddress sender;
    unsigned short senderPort;
    sf::Socket::Status status = socket->This.Receive(packet->This, sender, senderPort);
    if (status != sf::Socket::Done)
        return static_cast<sfSocketStatus>(status);

    if (address)
        strncpy(address->Address, sender.ToString().c_str(), 16);

    if (port)
        *port = senderPort;

    return sfSocketDone;
}

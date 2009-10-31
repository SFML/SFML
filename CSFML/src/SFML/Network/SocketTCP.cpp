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
#include <SFML/Network/SocketTCP.h>
#include <SFML/Network/SocketTCPStruct.h>
#include <SFML/Network/PacketStruct.h>
#include <SFML/Network/IPAddress.hpp>
#include <SFML/Internal.h>
#include <string.h>


////////////////////////////////////////////////////////////
/// Construct a new TCP socket
////////////////////////////////////////////////////////////
sfSocketTCP* sfSocketTCP_Create()
{
    return new sfSocketTCP;
}


////////////////////////////////////////////////////////////
/// Destroy an existing TCP socket
////////////////////////////////////////////////////////////
void sfSocketTCP_Destroy(sfSocketTCP* socket)
{
    if (socket)
    {
        socket->This.Close();
        delete socket;
    }
}


////////////////////////////////////////////////////////////
/// Change the blocking state of a TCP socket.
/// The default behaviour of a socket is blocking
////////////////////////////////////////////////////////////
void sfSocketTCP_SetBlocking(sfSocketTCP* socket, sfBool blocking)
{
    CSFML_CALL(socket, SetBlocking(blocking == sfTrue));
}


////////////////////////////////////////////////////////////
/// Connect a TCP socket to another computer on a specified port
////////////////////////////////////////////////////////////
sfSocketStatus sfSocketTCP_Connect(sfSocketTCP* socket, unsigned short port, sfIPAddress host, float timeout)
{
    sf::IPAddress address(host.Address);

    CSFML_CHECK_RETURN(socket, sfSocketError);

    return static_cast<sfSocketStatus>(socket->This.Connect(port, address, timeout));
}


////////////////////////////////////////////////////////////
/// Listen to a specified port for incoming data or connections
////////////////////////////////////////////////////////////
sfBool sfSocketTCP_Listen(sfSocketTCP* socket, unsigned short port)
{
    CSFML_CALL_RETURN(socket, Listen(port), sfFalse);
}


////////////////////////////////////////////////////////////
/// Wait for a connection (must be listening to a port).
/// This function is blocking, ie. it won't return before
/// a connection has been accepted
////////////////////////////////////////////////////////////
sfSocketStatus sfSocketTCP_Accept(sfSocketTCP* socket, sfSocketTCP** connected, sfIPAddress* address)
{
    CSFML_CHECK_RETURN(socket,    sfSocketError);
    CSFML_CHECK_RETURN(connected, sfSocketError);

    // Call SFML internal function
    sf::IPAddress clientAddress;
    sf::SocketTCP client;
    sf::Socket::Status status = socket->This.Accept(client, &clientAddress);
    if (status != sf::Socket::Done)
        return static_cast<sfSocketStatus>(status);

    // Convert the client socket returned
    *connected = sfSocketTCP_Create();
    (*connected)->This = client;

    // Convert the address if needed
    if (address)
        strncpy(address->Address, clientAddress.ToString().c_str(), 16);

    return sfSocketDone;
}


////////////////////////////////////////////////////////////
/// Send an array of bytes to the host (must be connected first)
////////////////////////////////////////////////////////////
sfSocketStatus sfSocketTCP_Send(sfSocketTCP* socket, const char* data, size_t size)
{
    CSFML_CHECK_RETURN(socket, sfSocketError);

    return static_cast<sfSocketStatus>(socket->This.Send(data, size));
}


////////////////////////////////////////////////////////////
/// Receive an array of bytes from the host (must be connected first)
////////////////////////////////////////////////////////////
sfSocketStatus sfSocketTCP_Receive(sfSocketTCP* socket, char* data, size_t maxSize, size_t* sizeReceived)
{
    CSFML_CHECK_RETURN(socket, sfSocketError);

    if (sizeReceived)
    {
        return static_cast<sfSocketStatus>(socket->This.Receive(data, maxSize, *sizeReceived));
    }
    else
    {
        std::size_t size = 0;
        return static_cast<sfSocketStatus>(socket->This.Receive(data, maxSize, size));
    }
}


////////////////////////////////////////////////////////////
/// Send a packet of data to the host (must be connected first)
////////////////////////////////////////////////////////////
sfSocketStatus sfSocketTCP_SendPacket(sfSocketTCP* socket, sfPacket* packet)
{
    CSFML_CHECK_RETURN(socket, sfSocketError);
    CSFML_CHECK_RETURN(packet, sfSocketError);

    return static_cast<sfSocketStatus>(socket->This.Send(packet->This));
}


////////////////////////////////////////////////////////////
/// Receive a packet from the host (must be connected first)
////////////////////////////////////////////////////////////
sfSocketStatus sfSocketTCP_ReceivePacket(sfSocketTCP* socket, sfPacket* packet)
{
    CSFML_CHECK_RETURN(socket, sfSocketError);
    CSFML_CHECK_RETURN(packet, sfSocketError);

    return static_cast<sfSocketStatus>(socket->This.Receive(packet->This));
}


////////////////////////////////////////////////////////////
/// Close the socket
////////////////////////////////////////////////////////////
sfBool sfSocketTCP_Close(sfSocketTCP* socket)
{
    CSFML_CALL_RETURN(socket, Close(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Check if a socket is in a valid state ; this function
/// can be called any time to check if the socket is OK
////////////////////////////////////////////////////////////
sfBool sfSocketTCP_IsValid(sfSocketTCP* socket)
{
    CSFML_CALL_RETURN(socket, IsValid(), sfFalse);
}

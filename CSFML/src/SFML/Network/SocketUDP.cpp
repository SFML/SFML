////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2008 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Network/SocketUDP.h>
#include <SFML/Network/SocketUDP.hpp>
#include <SFML/Network/IPAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Internal.h>
#include <string.h>


// WARNING : this structure must always be the SAME as in Network/Packet.h
struct sfPacket
{
    sf::Packet This;
};

struct sfSocketUDP
{
    sf::SocketUDP This;
};


////////////////////////////////////////////////////////////
/// Construct a new UDP socket
////////////////////////////////////////////////////////////
sfSocketUDP* sfSocketUDP_Create()
{
    return new sfSocketUDP;
}


////////////////////////////////////////////////////////////
/// Destroy an existing UDP socket
////////////////////////////////////////////////////////////
void sfSocketUDP_Destroy(sfSocketUDP* Socket)
{
    delete Socket;
}


////////////////////////////////////////////////////////////
/// Change the blocking state of a UDP socket.
/// The default behaviour of a socket is blocking
////////////////////////////////////////////////////////////
void sfSocketUDP_SetBlocking(sfSocketUDP* Socket, sfBool Blocking)
{
    CSFML_CALL(Socket, SetBlocking(Blocking == sfTrue));
}


////////////////////////////////////////////////////////////
/// Bind a socket to a specific port
////////////////////////////////////////////////////////////
sfBool sfSocketUDP_Bind(sfSocketUDP* Socket, unsigned short Port)
{
    CSFML_CALL_RETURN(Socket, Bind(Port), sfFalse);
}


////////////////////////////////////////////////////////////
/// Unbind a socket from its previous port, if any
////////////////////////////////////////////////////////////
sfBool sfSocketUDP_Unbind(sfSocketUDP* Socket)
{
    CSFML_CALL_RETURN(Socket, Unbind(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Send an array of bytes
////////////////////////////////////////////////////////////
sfSocketStatus sfSocketUDP_Send(sfSocketUDP* Socket, const char* Data, size_t Size, sfIPAddress Address, unsigned short Port)
{
    CSFML_CHECK_RETURN(Socket, sfSocketError)

    // Convert the address
    sf::IPAddress Receiver(Address.Address);

    return static_cast<sfSocketStatus>(Socket->This.Send(Data, Size, Receiver, Port));
}


////////////////////////////////////////////////////////////
/// Receive an array of bytes.
/// This function is blocking, ie. it won't return before some
/// bytes have been received
////////////////////////////////////////////////////////////
sfSocketStatus sfSocketUDP_Receive(sfSocketUDP* Socket, char* Data, size_t MaxSize, size_t* SizeReceived, sfIPAddress* Address, unsigned short* Port)
{
    CSFML_CHECK_RETURN(Socket, sfSocketError);

    // Call SFML internal function
    sf::IPAddress Sender;
    unsigned short SenderPort;
    std::size_t Received;

    sf::Socket::Status Status = Socket->This.Receive(Data, MaxSize, Received, Sender, SenderPort);
    if (Status != sf::Socket::Done)
        return static_cast<sfSocketStatus>(Status);

    if (SizeReceived)
        *SizeReceived = Received;

    if (Address)
        strncpy(Address->Address, Sender.ToString().c_str(), 16);

    if (Port)
        *Port = SenderPort;

    return sfSocketDone;
}


////////////////////////////////////////////////////////////
/// Send a packet of data
////////////////////////////////////////////////////////////
sfSocketStatus sfSocketUDP_SendPacket(sfSocketUDP* Socket, sfPacket* Packet, sfIPAddress Address, unsigned short Port)
{
    CSFML_CHECK_RETURN(Socket, sfSocketError);
    CSFML_CHECK_RETURN(Packet, sfSocketError);

    // Convert the address
    sf::IPAddress Receiver(Address.Address);

    return static_cast<sfSocketStatus>(Socket->This.Send(Packet->This, Receiver, Port));
}


////////////////////////////////////////////////////////////
/// Receive a packet.
/// This function is blocking, ie. it won't return before a
/// packet is received
////////////////////////////////////////////////////////////
sfSocketStatus sfSocketUDP_ReceivePacket(sfSocketUDP* Socket, sfPacket* Packet, sfIPAddress* Address, unsigned short* Port)
{
    CSFML_CHECK_RETURN(Socket, sfSocketError);
    CSFML_CHECK_RETURN(Packet, sfSocketError);

    sf::IPAddress Sender;
    unsigned short SenderPort;
    sf::Socket::Status Status = Socket->This.Receive(Packet->This, Sender, SenderPort);
    if (Status != sf::Socket::Done)
        return static_cast<sfSocketStatus>(Status);

    if (Address)
        strncpy(Address->Address, Sender.ToString().c_str(), 16);

    if (Port)
        *Port = SenderPort;

    return sfSocketDone;
}


////////////////////////////////////////////////////////////
/// Check if a socket is in a valid state ; this function
/// can be called any time to check if the socket is OK
////////////////////////////////////////////////////////////
sfBool sfSocketUDP_IsValid(sfSocketUDP* Socket)
{
    CSFML_CALL_RETURN(Socket, IsValid(), sfFalse);
}

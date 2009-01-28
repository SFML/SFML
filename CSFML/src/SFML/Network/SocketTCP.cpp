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
#include <SFML/Network/SocketTCP.h>
#include <SFML/Network/SocketTCP.hpp>
#include <SFML/Network/IPAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Internal.h>
#include <string.h>


// WARNING : this structure must always be the SAME as in Network/Packet.h
struct sfPacket
{
    sf::Packet This;
};

struct sfSocketTCP
{
    sf::SocketTCP This;
};


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
void sfSocketTCP_Destroy(sfSocketTCP* Socket)
{
    if (Socket)
    {
        Socket->This.Close();
        delete Socket;
    }
}


////////////////////////////////////////////////////////////
/// Change the blocking state of a TCP socket.
/// The default behaviour of a socket is blocking
////////////////////////////////////////////////////////////
void sfSocketTCP_SetBlocking(sfSocketTCP* Socket, sfBool Blocking)
{
    CSFML_CALL(Socket, SetBlocking(Blocking == sfTrue));
}


////////////////////////////////////////////////////////////
/// Connect a TCP socket to another computer on a specified port
////////////////////////////////////////////////////////////
sfSocketStatus sfSocketTCP_Connect(sfSocketTCP* Socket, unsigned short Port, sfIPAddress HostAddress, float Timeout)
{
    sf::IPAddress Address(HostAddress.Address);

    CSFML_CHECK_RETURN(Socket, sfSocketError);

    return static_cast<sfSocketStatus>(Socket->This.Connect(Port, Address, Timeout));
}


////////////////////////////////////////////////////////////
/// Listen to a specified port for incoming data or connections
////////////////////////////////////////////////////////////
sfBool sfSocketTCP_Listen(sfSocketTCP* Socket, unsigned short Port)
{
    CSFML_CALL_RETURN(Socket, Listen(Port), sfFalse);
}


////////////////////////////////////////////////////////////
/// Wait for a connection (must be listening to a port).
/// This function is blocking, ie. it won't return before
/// a connection has been accepted
////////////////////////////////////////////////////////////
sfSocketStatus sfSocketTCP_Accept(sfSocketTCP* Socket, sfSocketTCP** Connected, sfIPAddress* Address)
{
    CSFML_CHECK_RETURN(Socket,    sfSocketError);
    CSFML_CHECK_RETURN(Connected, sfSocketError);

    // Call SFML internal function
    sf::IPAddress ClientAddress;
    sf::SocketTCP Client;
    sf::Socket::Status Status = Socket->This.Accept(Client, &ClientAddress);
    if (Status != sf::Socket::Done)
        return static_cast<sfSocketStatus>(Status);

    // Convert the client socket returned
    *Connected = sfSocketTCP_Create();
    (*Connected)->This = Client;

    // Convert the address if needed
    if (Address)
        strncpy(Address->Address, ClientAddress.ToString().c_str(), 16);

    return sfSocketDone;
}


////////////////////////////////////////////////////////////
/// Send an array of bytes to the host (must be connected first)
////////////////////////////////////////////////////////////
sfSocketStatus sfSocketTCP_Send(sfSocketTCP* Socket, const char* Data, size_t Size)
{
    CSFML_CHECK_RETURN(Socket, sfSocketError);

    return static_cast<sfSocketStatus>(Socket->This.Send(Data, Size));
}


////////////////////////////////////////////////////////////
/// Receive an array of bytes from the host (must be connected first)
////////////////////////////////////////////////////////////
sfSocketStatus sfSocketTCP_Receive(sfSocketTCP* Socket, char* Data, size_t MaxSize, size_t* SizeReceived)
{
    CSFML_CHECK_RETURN(Socket, sfSocketError);

    if (SizeReceived)
    {
        return static_cast<sfSocketStatus>(Socket->This.Receive(Data, MaxSize, *SizeReceived));
    }
    else
    {
        std::size_t Size = 0;
        return static_cast<sfSocketStatus>(Socket->This.Receive(Data, MaxSize, Size));
    }
}


////////////////////////////////////////////////////////////
/// Send a packet of data to the host (must be connected first)
////////////////////////////////////////////////////////////
sfSocketStatus sfSocketTCP_SendPacket(sfSocketTCP* Socket, sfPacket* Packet)
{
    CSFML_CHECK_RETURN(Socket, sfSocketError);
    CSFML_CHECK_RETURN(Packet, sfSocketError);

    return static_cast<sfSocketStatus>(Socket->This.Send(Packet->This));
}


////////////////////////////////////////////////////////////
/// Receive a packet from the host (must be connected first)
////////////////////////////////////////////////////////////
sfSocketStatus sfSocketTCP_ReceivePacket(sfSocketTCP* Socket, sfPacket* Packet)
{
    CSFML_CHECK_RETURN(Socket, sfSocketError);
    CSFML_CHECK_RETURN(Packet, sfSocketError);

    return static_cast<sfSocketStatus>(Socket->This.Receive(Packet->This));
}


////////////////////////////////////////////////////////////
/// Check if a socket is in a valid state ; this function
/// can be called any time to check if the socket is OK
////////////////////////////////////////////////////////////
sfBool sfSocketTCP_IsValid(sfSocketTCP* Socket)
{
    CSFML_CALL_RETURN(Socket, IsValid(), sfFalse);
}

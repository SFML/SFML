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

#ifndef SFML_SOCKETUDP_H
#define SFML_SOCKETUDP_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Network/IPAddress.h>
#include <SFML/Network/SocketStatus.h>
#include <SFML/Network/Types.h>


////////////////////////////////////////////////////////////
/// Construct a new UDP socket
///
/// \return Pointer to the new socket
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketUDP* sfSocketUDP_Create();

////////////////////////////////////////////////////////////
/// Destroy an existing UDP socket
///
/// \param Socket : Socket to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfSocketUDP_Destroy(sfSocketUDP* Socket);

////////////////////////////////////////////////////////////
/// Change the blocking state of a UDP socket.
/// The default behaviour of a socket is blocking
///
/// \param Socket :   Socket to modify
/// \param Blocking : Pass sfTrue to set the socket as blocking, or false for non-blocking
///
////////////////////////////////////////////////////////////
CSFML_API void sfSocketUDP_SetBlocking(sfSocketUDP* Socket, sfBool Blocking);

////////////////////////////////////////////////////////////
/// Bind a socket to a specific port
///
/// \param Socket : Socket to bind
/// \param Port :   Port to bind the socket to
///
/// \return True if operation has been successful
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSocketUDP_Bind(sfSocketUDP* Socket, unsigned short Port);

////////////////////////////////////////////////////////////
/// Unbind a socket from its previous port, if any
///
/// \param Socket : Socket to unbind
///
/// \return sfTrue if operation has been successful
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSocketUDP_Unbind(sfSocketUDP* Socket);

////////////////////////////////////////////////////////////
/// Send an array of bytes
///
/// \param Socket :  Socket to use for sending
/// \param Data :    Pointer to the bytes to send
/// \param Size :    Number of bytes to send
/// \param Address : Address of the computer to send the packet to
/// \param Port :    Port to use for communication
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfSocketUDP_Send(sfSocketUDP* Socket, const char* Data, size_t Size, sfIPAddress Address, unsigned short Port);

////////////////////////////////////////////////////////////
/// Receive an array of bytes.
/// This function is blocking, ie. it won't return before some
/// bytes have been received
///
/// \param Socket :       Socket to use for receiving
/// \param Data :         Pointer to a byte array to fill (make sure it is big enough)
/// \param MaxSize :      Maximum number of bytes to read
/// \param SizeReceived : Number of bytes received
/// \param Address :      Address of the computer which sent the data
/// \param Port :         Port on which the remote computer sent the data
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfSocketUDP_Receive(sfSocketUDP* Socket, char* Data, size_t MaxSize, size_t* SizeReceived, sfIPAddress* Address, unsigned short* Port);

////////////////////////////////////////////////////////////
/// Send a packet of data
///
/// \param Socket :  Socket to use for sending
/// \param Packet :  Packet to send
/// \param Address : Address of the computer to send the packet to
/// \param Port :    Port to use for communication
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfSocketUDP_SendPacket(sfSocketUDP* Socket, sfPacket* Packet, sfIPAddress Address, unsigned short Port);

////////////////////////////////////////////////////////////
/// Receive a packet.
/// This function is blocking, ie. it won't return before a
/// packet is received
///
/// \param Socket :  Socket to use for receiving
/// \param Packet :  Packet to fill with received data
/// \param Address : Address of the computer which sent the packet
/// \param Port :    Port on which the remote computer sent the data
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfSocketUDP_ReceivePacket(sfSocketUDP* Socket, sfPacket* Packet, sfIPAddress* Address, unsigned short* Port);

////////////////////////////////////////////////////////////
/// Check if a socket is in a valid state ; this function
/// can be called any time to check if the socket is OK
///
/// \param Socket : Socket to check
///
/// \return sfTrue if the socket is valid
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSocketUDP_IsValid(sfSocketUDP* Socket);


#endif // SFML_SOCKETUDP_H

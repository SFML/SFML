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

#ifndef SFML_UDPSOCKET_H
#define SFML_UDPSOCKET_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Network/IpAddress.h>
#include <SFML/Network/SocketStatus.h>
#include <SFML/Network/Types.h>


////////////////////////////////////////////////////////////
/// Construct a new UDP socket
///
/// \return Pointer to the new socket
///
////////////////////////////////////////////////////////////
CSFML_API sfUdpSocket* sfUdpSocket_Create(void);

////////////////////////////////////////////////////////////
/// Destroy an existing UDP socket
///
/// \param socket : Socket to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfUdpSocket_Destroy(sfUdpSocket* socket);

////////////////////////////////////////////////////////////
/// Change the blocking state of a UDP socket.
/// The default behaviour of a socket is blocking
///
/// \param socket :   Socket to modify
/// \param blocking : Pass sfTrue to set the socket as blocking, or false for non-blocking
///
////////////////////////////////////////////////////////////
CSFML_API void sfUdpSocket_SetBlocking(sfUdpSocket* socket, sfBool blocking);

////////////////////////////////////////////////////////////
/// Get the blocking state of the socket
///
/// \param socket : Socket to read
///
/// \Return sfTrue if the socket is blocking, sfFalse otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfUdpSocket_IsBlocking(const sfUdpSocket* socket);

////////////////////////////////////////////////////////////
/// Get the port to which a socket is bound locally
///
/// \param socket : Socket to read
///
/// \return Port to which the socket is bound
///
////////////////////////////////////////////////////////////
CSFML_API unsigned short sfUdpSocket_GetLocalPort(const sfUdpSocket* socket);

////////////////////////////////////////////////////////////
/// Bind a socket to a specific port
///
/// \param socket : Socket to bind
/// \param port :   Port to bind the socket to
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfUdpSocket_Bind(sfUdpSocket* socket, unsigned short port);

////////////////////////////////////////////////////////////
/// Unbind a socket from its previous port, if any
///
/// \param socket : Socket to unbind
///
////////////////////////////////////////////////////////////
CSFML_API void sfUdpSocket_Unbind(sfUdpSocket* socket);

////////////////////////////////////////////////////////////
/// Send an array of bytes
///
/// \param socket :  Socket to use for sending
/// \param data :    Pointer to the bytes to send
/// \param size :    Number of bytes to send
/// \param address : Address of the computer to send the packet to
/// \param port :    Port to use for communication
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfUdpSocket_Send(sfUdpSocket* socket, const char* data, size_t size, sfIpAddress address, unsigned short port);

////////////////////////////////////////////////////////////
/// Receive an array of bytes.
/// This function is blocking, ie. it won't return before some
/// bytes have been received
///
/// \param socket :       Socket to use for receiving
/// \param data :         Pointer to a byte array to fill (make sure it is big enough)
/// \param maxSize :      Maximum number of bytes to read
/// \param sizeReceived : Number of bytes received
/// \param address :      Address of the computer which sent the data
/// \param port :         Port on which the remote computer sent the data
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfUdpSocket_Receive(sfUdpSocket* socket, char* data, size_t maxSize, size_t* sizeReceived, sfIpAddress* address, unsigned short* port);

////////////////////////////////////////////////////////////
/// Send a packet of data
///
/// \param socket :  Socket to use for sending
/// \param packet :  Packet to send
/// \param address : Address of the computer to send the packet to
/// \param port :    Port to use for communication
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfUdpSocket_SendPacket(sfUdpSocket* socket, sfPacket* packet, sfIpAddress address, unsigned short port);

////////////////////////////////////////////////////////////
/// Receive a packet.
/// This function is blocking, ie. it won't return before a
/// packet is received
///
/// \param socket :  Socket to use for receiving
/// \param packet :  Packet to fill with received data
/// \param address : Address of the computer which sent the packet
/// \param port :    Port on which the remote computer sent the data
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfUdpSocket_ReceivePacket(sfUdpSocket* socket, sfPacket* packet, sfIpAddress* address, unsigned short* port);


#endif // SFML_UDPSOCKET_H

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

#ifndef SFML_SOCKETTCP_H
#define SFML_SOCKETTCP_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Network/IPAddress.h>
#include <SFML/Network/SocketStatus.h>
#include <SFML/Network/Types.h>


////////////////////////////////////////////////////////////
/// Construct a new TCP socket
///
/// \return Pointer to the new socket
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketTCP* sfSocketTCP_Create();

////////////////////////////////////////////////////////////
/// Destroy an existing TCP socket
///
/// \param socket : Socket to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfSocketTCP_Destroy(sfSocketTCP* socket);

////////////////////////////////////////////////////////////
/// Change the blocking state of a TCP socket.
/// The default behaviour of a socket is blocking
///
/// \param socket :   Socket to modify
/// \param blocking : Pass sfTrue to set the socket as blocking, or false for non-blocking
///
////////////////////////////////////////////////////////////
CSFML_API void sfSocketTCP_SetBlocking(sfSocketTCP* socket, sfBool blocking);

////////////////////////////////////////////////////////////
/// Connect a TCP socket to another computer on a specified port
///
/// \param socket :  Socket to use for connecting
/// \param port :    Port to use for transfers (warning : ports < 1024 are reserved)
/// \param host :    IP Address of the host to connect to
/// \param timeout : Maximum time to wait (0 to use no timeout)
///
/// \return sfTrue if operation has been successful
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfSocketTCP_Connect(sfSocketTCP* socket, unsigned short port, sfIPAddress host, float timeout);

////////////////////////////////////////////////////////////
/// Listen to a specified port for incoming data or connections
///
/// \param socket : Socket to use for listening
/// \param port :   Port to listen to
///
/// \return sfTrue if operation has been successful
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSocketTCP_Listen(sfSocketTCP* socket, unsigned short port);

////////////////////////////////////////////////////////////
/// Wait for a connection (must be listening to a port).
/// This function is blocking, ie. it won't return before
/// a connection has been accepted
///
/// \param socket :    Socket to use for accepting
/// \param connected : Pointer to a socket pointer that will be filled with the connected client
/// \param address :   Pointer to an address to fill with client infos
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfSocketTCP_Accept(sfSocketTCP* socket, sfSocketTCP** connected, sfIPAddress* address);

////////////////////////////////////////////////////////////
/// Send an array of bytes to the host (must be connected first)
///
/// \param socket : Socket to use for sending
/// \param data :   Pointer to the bytes to send
/// \param size :   Number of bytes to send
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfSocketTCP_Send(sfSocketTCP* socket, const char* data, size_t size);

////////////////////////////////////////////////////////////
/// Receive an array of bytes from the host (must be connected first)
///
/// \param socket :       Socket to use for receiving
/// \param data :         Pointer to a byte array to fill (make sure it is big enough)
/// \param maxSize :      Maximum number of bytes to read
/// \param sizeReceived : Number of bytes received
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfSocketTCP_Receive(sfSocketTCP* socket, char* data, size_t maxSize, size_t* sizeReceived);

////////////////////////////////////////////////////////////
/// Send a packet of data to the host (must be connected first)
///
/// \param socket : Socket to use for sending
/// \param packet : Packet to send
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfSocketTCP_SendPacket(sfSocketTCP* socket, sfPacket* packet);

////////////////////////////////////////////////////////////
/// Receive a packet from the host (must be connected first)
///
/// \param socket : Socket to use for receiving
/// \param packet : Packet to fill with received data
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfSocketTCP_ReceivePacket(sfSocketTCP* socket, sfPacket* packet);

////////////////////////////////////////////////////////////
/// Close the socket
///
/// \param socket : Socket to close
///
/// \return True if the socket was successfully closed
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSocketTCP_Close(sfSocketTCP* socket);

////////////////////////////////////////////////////////////
/// Check if a socket is in a valid state ; this function
/// can be called any time to check if the socket is OK
///
/// \param socket : Socket to check
///
/// \return True if the socket is valid
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSocketTCP_IsValid(sfSocketTCP* socket);


#endif // SFML_SOCKETTCP_H

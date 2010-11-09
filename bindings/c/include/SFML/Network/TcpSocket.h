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

#ifndef SFML_TCPSOCKET_H
#define SFML_TCPSOCKET_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Network/IpAddress.h>
#include <SFML/Network/SocketStatus.h>
#include <SFML/Network/Types.h>


////////////////////////////////////////////////////////////
/// Construct a new TCP socket
///
/// \return Pointer to the new socket
///
////////////////////////////////////////////////////////////
CSFML_API sfTcpSocket* sfTcpSocket_Create(void);

////////////////////////////////////////////////////////////
/// Destroy an existing TCP socket
///
/// \param socket : Socket to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfTcpSocket_Destroy(sfTcpSocket* socket);

////////////////////////////////////////////////////////////
/// Change the blocking state of a TCP socket.
/// The default behaviour of a socket is blocking
///
/// \param socket :   Socket to modify
/// \param blocking : Pass sfTrue to set the socket as blocking, or false for non-blocking
///
////////////////////////////////////////////////////////////
CSFML_API void sfTcpSocket_SetBlocking(sfTcpSocket* socket, sfBool blocking);

////////////////////////////////////////////////////////////
/// Get the blocking state of the socket
///
/// \param socket : Socket to read
///
/// \Return sfTrue if the socket is blocking, sfFalse otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfTcpSocket_IsBlocking(const sfTcpSocket* socket);

////////////////////////////////////////////////////////////
/// Get the port to which a socket is bound locally
///
/// \param socket : Socket to read
///
/// \return Port to which the socket is bound
///
////////////////////////////////////////////////////////////
CSFML_API unsigned short sfTcpSocket_GetLocalPort(const sfTcpSocket* socket);

////////////////////////////////////////////////////////////
/// Get the address of the connected peer of a socket
///
/// \param socket : Socket to read
///
/// \return Address of the remote peer
///
////////////////////////////////////////////////////////////
CSFML_API sfIpAddress sfTcpSocket_GetRemoteAddress(const sfTcpSocket* socket);

////////////////////////////////////////////////////////////
/// Get the port of the connected peer to which a socket is connected
///
/// \param socket : Socket to read
///
/// \return Remote port to which the socket is connected
///
////////////////////////////////////////////////////////////
CSFML_API unsigned short sfTcpSocket_GetRemotePort(const sfTcpSocket* socket);

////////////////////////////////////////////////////////////
/// Connect a TCP socket to another computer on a specified port
///
/// \param socket :  Socket to connect
/// \param host :    IP Address of the host to connect to
/// \param port :    Port to use for transfers (warning : ports < 1024 are reserved)
/// \param timeout : Maximum time to wait (0 to use no timeout)
///
/// \return sfTrue if operation has been successful
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfTcpSocket_Connect(sfTcpSocket* socket, sfIpAddress host, unsigned short port, float timeout);

////////////////////////////////////////////////////////////
/// Disconnect a connect from its remote peer
///
/// \param socket : Socket to disconnect
///
////////////////////////////////////////////////////////////
CSFML_API void sfTcpSocket_Disconnect(sfTcpSocket* socket);

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
CSFML_API sfSocketStatus sfTcpSocket_Send(sfTcpSocket* socket, const char* data, size_t size);

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
CSFML_API sfSocketStatus sfTcpSocket_Receive(sfTcpSocket* socket, char* data, size_t maxSize, size_t* sizeReceived);

////////////////////////////////////////////////////////////
/// Send a packet of data to the host (must be connected first)
///
/// \param socket : Socket to use for sending
/// \param packet : Packet to send
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfTcpSocket_SendPacket(sfTcpSocket* socket, sfPacket* packet);

////////////////////////////////////////////////////////////
/// Receive a packet from the host (must be connected first)
///
/// \param socket : Socket to use for receiving
/// \param packet : Packet to fill with received data
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfTcpSocket_ReceivePacket(sfTcpSocket* socket, sfPacket* packet);


#endif // SFML_TCPSOCKET_H

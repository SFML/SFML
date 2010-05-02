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

#ifndef SFML_TCPLISTENER_H
#define SFML_TCPLISTENER_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Network/SocketStatus.h>
#include <SFML/Network/Types.h>


////////////////////////////////////////////////////////////
/// Construct a new TCP socket
///
/// \return Pointer to the new socket
///
////////////////////////////////////////////////////////////
CSFML_API sfTcpListener* sfTcpListener_Create(void);

////////////////////////////////////////////////////////////
/// Destroy an existing TCP socket
///
/// \param socket : Socket to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfTcpListener_Destroy(sfTcpListener* socket);

////////////////////////////////////////////////////////////
/// Change the blocking state of a TCP socket.
/// The default behaviour of a socket is blocking
///
/// \param socket :   Socket to modify
/// \param blocking : Pass sfTrue to set the socket as blocking, or sfFalse for non-blocking
///
////////////////////////////////////////////////////////////
CSFML_API void sfTcpListener_SetBlocking(sfTcpListener* socket, sfBool blocking);

////////////////////////////////////////////////////////////
/// Get the blocking state of the socket
///
/// \param socket : Socket to read
///
/// \Return sfTrue if the socket is blocking, sfFalse otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfTcpListener_IsBlocking(const sfTcpListener* socket);

////////////////////////////////////////////////////////////
/// Listen to a specified port for incoming data or connections
///
/// \param socket : Socket to use for listening
/// \param port :   Port to listen to
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfTcpListener_Listen(sfTcpListener* socket, unsigned short port);

////////////////////////////////////////////////////////////
/// Wait for a connection (must be listening to a port).
/// This function is blocking, ie. it won't return before
/// a connection has been accepted
///
/// \param socket :    Socket to use for accepting
/// \param connected : Pointer to a socket pointer that will be filled with the connected client
///
/// \return Socket status
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketStatus sfTcpListener_Accept(sfTcpListener* socket, sfTcpSocket** connected);


#endif // SFML_TCPLISTENER_H

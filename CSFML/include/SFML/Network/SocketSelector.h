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

#ifndef SFML_SOCKETSELECTOR_H
#define SFML_SOCKETSELECTOR_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Network/Types.h>


////////////////////////////////////////////////////////////
/// Create a new selector
///
/// \return A new sfSelector object
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketSelector* sfSocketSelector_Create(void);

////////////////////////////////////////////////////////////
/// Copy an existing selector
///
/// \param selector : Selector to copy
///
/// \return Copied object
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketSelector* sfSocketSelector_Copy(sfSocketSelector* selector);

////////////////////////////////////////////////////////////
/// Destroy an existing selector
///
/// \param selector : Selector to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfSocketSelector_Destroy(sfSocketSelector* selector);

////////////////////////////////////////////////////////////
/// Add a socket to watch to a selector
///
/// \param selector : Selector to add the socket to
/// \param socket :   Socket to add
///
////////////////////////////////////////////////////////////
CSFML_API void sfSocketSelector_AddTcpListener(sfSocketSelector* selector, sfTcpListener* socket);
CSFML_API void sfSocketSelector_AddTcpSocket(sfSocketSelector* selector, sfTcpSocket* socket);
CSFML_API void sfSocketSelector_AddUdpSocket(sfSocketSelector* selector, sfUdpSocket* socket);

////////////////////////////////////////////////////////////
/// Remove a socket from a selector
///
/// \param selector : Selector to remove the socket from
/// \param socket :   Socket to remove
///
////////////////////////////////////////////////////////////
CSFML_API void sfSocketSelector_RemoveTcpListener(sfSocketSelector* selector, sfTcpListener* socket);
CSFML_API void sfSocketSelector_RemoveTcpSocket(sfSocketSelector* selector, sfTcpSocket* socket);
CSFML_API void sfSocketSelector_RemoveUdpSocket(sfSocketSelector* selector, sfUdpSocket* socket);

////////////////////////////////////////////////////////////
/// Remove all sockets from a selector
///
/// \param selector : Selector to remove the socket from
///
////////////////////////////////////////////////////////////
CSFML_API void sfSocketSelector_Clear(sfSocketSelector* selector);

////////////////////////////////////////////////////////////
/// Wait and collect sockets which are ready for reading.
/// This functions will return either when at least one socket
/// is ready, or when the given timeout is over
///
/// \param selector : Selector to check
/// \param timeout :  Maximum time to wait, in seconds (0 to disable timeout)
///
/// \return sfTrue if there are sockets ready, sfFalse otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSocketSelector_Wait(sfSocketSelector* selector, float timeout);

////////////////////////////////////////////////////////////
/// Test a socket to know if it is ready to receive data
///
/// \param selector : Selector to check
/// \param socket :   Socket to test
///
/// \return sfTrue if the socket is ready to receive data
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfSocketSelector_IsTcpListenerReady(const sfSocketSelector* selector, sfTcpListener* socket);
CSFML_API sfBool sfSocketSelector_IsTcpSocketReady(const sfSocketSelector* selector, sfTcpSocket* socket);
CSFML_API sfBool sfSocketSelector_IsUdpSocketReady(const sfSocketSelector* selector, sfUdpSocket* socket);


#endif // SFML_SOCKETSELECTOR_H

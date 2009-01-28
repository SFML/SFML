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

#ifndef SFML_SELECTOR_H
#define SFML_SELECTOR_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Network/SocketTCP.h>
#include <SFML/Network/SocketUDP.h>


////////////////////////////////////////////////////////////
/// sfSelectorTCP and sfSelectorUDP allow reading from multiple sockets
/// without blocking. It's a kind of multiplexer
////////////////////////////////////////////////////////////
typedef struct sfSelectorTCP sfSelectorTCP;
typedef struct sfSelectorUDP sfSelectorUDP;


////////////////////////////////////////////////////////////
/// Create a new selector
///
/// \return A new sfSelector object
///
////////////////////////////////////////////////////////////
CSFML_API sfSelectorTCP* sfSelectorTCP_Create();
CSFML_API sfSelectorUDP* sfSelectorUDP_Create();

////////////////////////////////////////////////////////////
/// Destroy an existing selector
///
/// \param Selector : Selector to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfSelectorTCP_Destroy(sfSelectorTCP* Selector);
CSFML_API void sfSelectorUDP_Destroy(sfSelectorUDP* Selector);

////////////////////////////////////////////////////////////
/// Add a socket to watch to a selector
///
/// \param Selector : Selector to add the socket to
/// \param Socket :   Socket to add
///
////////////////////////////////////////////////////////////
CSFML_API void sfSelectorTCP_Add(sfSelectorTCP* Selector, sfSocketTCP* Socket);
CSFML_API void sfSelectorUDP_Add(sfSelectorUDP* Selector, sfSocketUDP* Socket);

////////////////////////////////////////////////////////////
/// Remove a socket from a selector
///
/// \param Selector : Selector to remove the socket from
/// \param Socket :   Socket to remove
///
////////////////////////////////////////////////////////////
CSFML_API void sfSelectorTCP_Remove(sfSelectorTCP* Selector, sfSocketTCP* Socket);
CSFML_API void sfSelectorUDP_Remove(sfSelectorUDP* Selector, sfSocketUDP* Socket);

////////////////////////////////////////////////////////////
/// Remove all sockets from a selector
///
/// \param Selector : Selector to remove the socket from
///
////////////////////////////////////////////////////////////
CSFML_API void sfSelectorTCP_Clear(sfSelectorTCP* Selector);
CSFML_API void sfSelectorUDP_Clear(sfSelectorUDP* Selector);

////////////////////////////////////////////////////////////
/// Wait and collect sockets which are ready for reading.
/// This functions will return either when at least one socket
/// is ready, or when the given time is out
///
/// \param Selector : Selector to check
/// \param Timeout :  Maximum time to wait, in seconds (0 to disable timeout)
///
/// \return Number of sockets ready
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfSelectorTCP_Wait(sfSelectorTCP* Selector, float Timeout);
CSFML_API unsigned int sfSelectorUDP_Wait(sfSelectorUDP* Selector, float Timeout);

////////////////////////////////////////////////////////////
/// After a call to Wait(), get the Index-th socket which is
/// ready for reading. The total number of sockets ready
/// is the integer returned by the previous call to Wait()
///
/// \param Selector : Selector to check
/// \param Index :    Index of the socket to get
///
/// \return The Index-th socket
///
////////////////////////////////////////////////////////////
CSFML_API sfSocketTCP* sfSelectorTCP_GetSocketReady(sfSelectorTCP* Selector, unsigned int Index);
CSFML_API sfSocketUDP* sfSelectorUDP_GetSocketReady(sfSelectorUDP* Selector, unsigned int Index);


#endif // SFML_SELECTOR_H

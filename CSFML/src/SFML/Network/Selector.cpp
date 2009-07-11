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
#include <SFML/Network/Selector.h>
#include <SFML/Network/SelectorStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Create a new selector
////////////////////////////////////////////////////////////
sfSelectorTCP* sfSelectorTCP_Create() {return new sfSelectorTCP;}
sfSelectorUDP* sfSelectorUDP_Create() {return new sfSelectorUDP;}


////////////////////////////////////////////////////////////
/// Destroy an existing selector
////////////////////////////////////////////////////////////
void sfSelectorTCP_Destroy(sfSelectorTCP* selector) {delete selector;}
void sfSelectorUDP_Destroy(sfSelectorUDP* selector) {delete selector;}


////////////////////////////////////////////////////////////
/// Add a socket to watch to a selector
////////////////////////////////////////////////////////////
void sfSelectorTCP_Add(sfSelectorTCP* selector, sfSocketTCP* socket) {CSFML_CALL(selector, Add(socket->This)); selector->Sockets[socket->This] = socket;}
void sfSelectorUDP_Add(sfSelectorUDP* selector, sfSocketUDP* socket) {CSFML_CALL(selector, Add(socket->This)); selector->Sockets[socket->This] = socket;}


////////////////////////////////////////////////////////////
/// Remove a socket from a selector
////////////////////////////////////////////////////////////
void sfSelectorTCP_Remove(sfSelectorTCP* selector, sfSocketTCP* socket) {CSFML_CALL(selector, Remove(socket->This)); selector->Sockets.erase(socket->This);}
void sfSelectorUDP_Remove(sfSelectorUDP* selector, sfSocketUDP* socket) {CSFML_CALL(selector, Remove(socket->This)); selector->Sockets.erase(socket->This);}


////////////////////////////////////////////////////////////
/// Remove all sockets from a selector
////////////////////////////////////////////////////////////
void sfSelectorTCP_Clear(sfSelectorTCP* selector) {CSFML_CALL(selector, Clear()); selector->Sockets.clear();}
void sfSelectorUDP_Clear(sfSelectorUDP* selector) {CSFML_CALL(selector, Clear()); selector->Sockets.clear();}

////////////////////////////////////////////////////////////
/// Wait and collect sockets which are ready for reading.
/// This functions will return either when at least one socket
/// is ready, or when the given time is out
////////////////////////////////////////////////////////////
unsigned int sfSelectorTCP_Wait(sfSelectorTCP* selector, float timeout) {CSFML_CALL_RETURN(selector, Wait(timeout), 0);}
unsigned int sfSelectorUDP_Wait(sfSelectorUDP* selector, float timeout) {CSFML_CALL_RETURN(selector, Wait(timeout), 0);}


////////////////////////////////////////////////////////////
/// After a call to Wait(), get the Index-th socket which is
/// ready for reading. The total number of sockets ready
/// is the integer returned by the previous call to Wait()
////////////////////////////////////////////////////////////
sfSocketTCP* sfSelectorTCP_GetSocketReady(sfSelectorTCP* selector, unsigned int index)
{
    CSFML_CHECK_RETURN(selector, NULL);
    return selector->Sockets[selector->This.GetSocketReady(index)];
}
sfSocketUDP* sfSelectorUDP_GetSocketReady(sfSelectorUDP* selector, unsigned int index)
{
    CSFML_CHECK_RETURN(selector, NULL);
    return selector->Sockets[selector->This.GetSocketReady(index)];
}

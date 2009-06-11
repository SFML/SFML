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
void sfSelectorTCP_Destroy(sfSelectorTCP* Selector) {delete Selector;}
void sfSelectorUDP_Destroy(sfSelectorUDP* Selector) {delete Selector;}


////////////////////////////////////////////////////////////
/// Add a socket to watch to a selector
////////////////////////////////////////////////////////////
void sfSelectorTCP_Add(sfSelectorTCP* Selector, sfSocketTCP* Socket) {CSFML_CALL(Selector, Add(Socket->This)); Selector->Sockets[Socket->This] = Socket;}
void sfSelectorUDP_Add(sfSelectorUDP* Selector, sfSocketUDP* Socket) {CSFML_CALL(Selector, Add(Socket->This)); Selector->Sockets[Socket->This] = Socket;}


////////////////////////////////////////////////////////////
/// Remove a socket from a selector
////////////////////////////////////////////////////////////
void sfSelectorTCP_Remove(sfSelectorTCP* Selector, sfSocketTCP* Socket) {CSFML_CALL(Selector, Remove(Socket->This)); Selector->Sockets.erase(Socket->This);}
void sfSelectorUDP_Remove(sfSelectorUDP* Selector, sfSocketUDP* Socket) {CSFML_CALL(Selector, Remove(Socket->This)); Selector->Sockets.erase(Socket->This);}


////////////////////////////////////////////////////////////
/// Remove all sockets from a selector
////////////////////////////////////////////////////////////
void sfSelectorTCP_Clear(sfSelectorTCP* Selector) {CSFML_CALL(Selector, Clear()); Selector->Sockets.clear();}
void sfSelectorUDP_Clear(sfSelectorUDP* Selector) {CSFML_CALL(Selector, Clear()); Selector->Sockets.clear();}

////////////////////////////////////////////////////////////
/// Wait and collect sockets which are ready for reading.
/// This functions will return either when at least one socket
/// is ready, or when the given time is out
////////////////////////////////////////////////////////////
unsigned int sfSelectorTCP_Wait(sfSelectorTCP* Selector, float Timeout) {CSFML_CALL_RETURN(Selector, Wait(Timeout), 0);}
unsigned int sfSelectorUDP_Wait(sfSelectorUDP* Selector, float Timeout) {CSFML_CALL_RETURN(Selector, Wait(Timeout), 0);}


////////////////////////////////////////////////////////////
/// After a call to Wait(), get the Index-th socket which is
/// ready for reading. The total number of sockets ready
/// is the integer returned by the previous call to Wait()
////////////////////////////////////////////////////////////
sfSocketTCP* sfSelectorTCP_GetSocketReady(sfSelectorTCP* Selector, unsigned int Index)
{
    CSFML_CHECK_RETURN(Selector, NULL);
    return Selector->Sockets[Selector->This.GetSocketReady(Index)];
}
sfSocketUDP* sfSelectorUDP_GetSocketReady(sfSelectorUDP* Selector, unsigned int Index)
{
    CSFML_CHECK_RETURN(Selector, NULL);
    return Selector->Sockets[Selector->This.GetSocketReady(Index)];
}

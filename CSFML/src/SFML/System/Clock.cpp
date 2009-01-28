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
#include <SFML/System/Clock.h>
#include <SFML/System/Clock.hpp>
#include <SFML/Internal.h>


struct sfClock
{
    sf::Clock This;
};


////////////////////////////////////////////////////////////
/// Create a new clock and start it
////////////////////////////////////////////////////////////
sfClock* sfClock_Create()
{
    return new sfClock;
}


////////////////////////////////////////////////////////////
/// Destroy an existing clock
////////////////////////////////////////////////////////////
void sfClock_Destroy(sfClock* Clock)
{
    delete Clock;
}


////////////////////////////////////////////////////////////
/// Get the time elapsed for a clock
////////////////////////////////////////////////////////////
float sfClock_GetTime(sfClock* Clock)
{
    CSFML_CALL_RETURN(Clock, GetElapsedTime(), 0.f)
}


////////////////////////////////////////////////////////////
/// Restart a clock
////////////////////////////////////////////////////////////
void sfClock_Reset(sfClock* Clock)
{
    CSFML_CALL(Clock, Reset())
}

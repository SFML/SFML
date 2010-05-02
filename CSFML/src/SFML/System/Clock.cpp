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
#include <SFML/System/Clock.h>
#include <SFML/System/ClockStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Create a new clock and start it
////////////////////////////////////////////////////////////
sfClock* sfClock_Create(void)
{
    return new sfClock;
}


////////////////////////////////////////////////////////////
/// Copy an existing clock
////////////////////////////////////////////////////////////
sfClock* sfClock_Copy(sfClock* clock)
{
    CSFML_CHECK_RETURN(clock, NULL);

    return new sfClock(*clock);
}


////////////////////////////////////////////////////////////
/// Destroy an existing clock
////////////////////////////////////////////////////////////
void sfClock_Destroy(sfClock* clock)
{
    delete clock;
}


////////////////////////////////////////////////////////////
/// Get the time elapsed for a clock
////////////////////////////////////////////////////////////
float sfClock_GetTime(const sfClock* clock)
{
    CSFML_CALL_RETURN(clock, GetElapsedTime(), 0.f)
}


////////////////////////////////////////////////////////////
/// Restart a clock
////////////////////////////////////////////////////////////
void sfClock_Reset(sfClock* clock)
{
    CSFML_CALL(clock, Reset())
}

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/System/Time.hpp>
#include <SFML/System/Win32/SleepImpl.hpp>
#include <SFML/System/Win32/WindowsHeader.hpp>

#include <mmsystem.h>

namespace sf::priv
{
////////////////////////////////////////////////////////////
void sleepImpl(Time time)
{
    // Get the minimum supported timer resolution on this system
    static const UINT periodMin = []
    {
        TIMECAPS tc;
        timeGetDevCaps(&tc, sizeof(TIMECAPS));
        return tc.wPeriodMin;
    }();

    // Set the timer resolution to the minimum for the Sleep call
    timeBeginPeriod(periodMin);

    // Wait...
    ::Sleep(static_cast<DWORD>(time.asMilliseconds()));

    // Reset the timer resolution back to the system default
    timeEndPeriod(periodMin);
}

} // namespace sf::priv

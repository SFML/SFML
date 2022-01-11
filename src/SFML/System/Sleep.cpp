////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/System/Sleep.hpp>
#include <chrono>
#include <thread>

#if defined(SFML_SYSTEM_WINDOWS)
#include <SFML/System/Win32/WindowsHeader.hpp>
#include <mmsystem.h>
#endif

namespace sf
{
////////////////////////////////////////////////////////////
void sleep(Time duration)
{
    // The effects of invoking 'std::this_thread::sleep_for' with a negative
    // duration are not specified in the C++ standard.
    if (duration < Time::Zero)
        return;

    // On Windows, the timer resolution must be temporarily changed in order for
    // the sleep time to be as accurate as possible.
#if defined(SFML_SYSTEM_WINDOWS)
    // Get the supported timer resolutions on this system
    TIMECAPS tc;
    timeGetDevCaps(&tc, sizeof(TIMECAPS));

    // Set the timer resolution to the minimum for the Sleep call
    timeBeginPeriod(tc.wPeriodMin);
#endif

    const auto time = std::chrono::duration<Int64, std::micro>(duration.asMicroseconds());
    std::this_thread::sleep_for(time);

#if defined(SFML_SYSTEM_WINDOWS)
    // Reset the timer resolution back to the system default
    timeEndPeriod(tc.wPeriodMin);
#endif
}

} // namespace sf

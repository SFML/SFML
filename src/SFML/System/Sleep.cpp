////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/System/Time.hpp>

#if defined(SFML_SYSTEM_WINDOWS)
#include <SFML/System/Win32/SleepImpl.hpp>
#else
#include <SFML/System/Unix/SleepImpl.hpp>
#endif


namespace sf
{
////////////////////////////////////////////////////////////
void sleep(Time duration)
{
    // Note that 'std::this_thread::sleep_for' is intentionally not used here
    // as it results in inconsistent sleeping times under MinGW-w64.

    if (duration >= Time::Zero)
        priv::sleepImpl(duration);
}

} // namespace sf

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
#include <SFML/System/Time.hpp>
#include <SFML/System/Unix/SleepImpl.hpp>

#include <cerrno>
#include <ctime>


namespace sf::priv
{
////////////////////////////////////////////////////////////
void sleepImpl(Time time)
{
    const std::int64_t usecs = time.asMicroseconds();

    // Construct the time to wait
    timespec ti{};
    ti.tv_sec  = static_cast<time_t>(usecs / 1000000);
    ti.tv_nsec = static_cast<long>((usecs % 1000000) * 1000);

    // Wait...
    // If nanosleep returns -1, we check errno. If it is EINTR
    // nanosleep was interrupted and has set ti to the remaining
    // duration. We continue sleeping until the complete duration
    // has passed. We stop sleeping if it was due to an error.
    while ((nanosleep(&ti, &ti) == -1) && (errno == EINTR))
    {
    }
}

} // namespace sf::priv

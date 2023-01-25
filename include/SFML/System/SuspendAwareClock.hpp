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


#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Export.hpp>

#include <chrono>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Android, chrono-compatible, suspend-aware clock
///
/// Linux steady clock is represented by CLOCK_MONOTONIC.
/// However, this implementation does not work properly for
/// long-running clocks that work in the background when the
/// system is suspended.
///
/// SuspendAwareClock uses CLOCK_BOOTTIME which is identical
/// to CLOCK_MONOTONIC, except that it also includes any time
/// that the system is suspended.
///
/// Note: In most cases, CLOCK_MONOTONIC is a better choice.
/// Make sure this implementation is required for your use case.
///
////////////////////////////////////////////////////////////
class SFML_SYSTEM_API SuspendAwareClock
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Type traits and static members
    ///
    /// These type traits and static members meet the requirements
    /// of a Clock concept in the C++ Standard. More specifically,
    /// TrivialClock requirements are met. Thus, naming convention
    /// has been kept consistent to allow for extended use e.g.
    /// https://en.cppreference.com/w/cpp/chrono/is_clock
    ///
    ////////////////////////////////////////////////////////////
    using duration   = std::chrono::nanoseconds;
    using rep        = duration::rep;
    using period     = duration::period;
    using time_point = std::chrono::time_point<SuspendAwareClock, duration>;

    static constexpr bool is_steady = true;

    static time_point now() noexcept;
};

} // namespace sf

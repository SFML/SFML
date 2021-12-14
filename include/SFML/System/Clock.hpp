////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2021 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_CLOCK_HPP
#define SFML_CLOCK_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Export.hpp>
#include <SFML/System/Time.hpp>
#include <chrono>
#include <ratio>
#include <type_traits>

#ifdef SFML_SYSTEM_ANDROID
#include <SFML/System/SuspendAwareClock.hpp>
#endif


namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
/// \brief Chooses a monotonic clock of highest resolution
///
/// The high_resolution_clock is usually an alias for other
/// clocks: steady_clock or system_clock, whichever has a
/// higher precision.
///
/// sf::Clock, however, is aimed towards monotonic time
/// measurements and so system_clock could never be a choice
/// as its subject to discontinuous jumps in the system time
/// (e.g., if the system administrator manually changes
/// the clock), and by the incremental adjustments performed
/// by `adjtime` and Network Time Protocol. On the other
/// hand, monotonic clocks are unaffected by this behavior.
///
/// Note: Linux implementation of a monotonic clock that
/// takes sleep time into account is represented by
/// CLOCK_BOOTTIME. Android devices can define the macro:
/// SFML_ANDROID_USE_SUSPEND_AWARE_CLOCK to use a separate
/// implementation of that clock, instead.
///
/// For more information on Linux clocks visit:
/// https://linux.die.net/man/2/clock_gettime
///
////////////////////////////////////////////////////////////
#if defined(SFML_SYSTEM_ANDROID) && defined(SFML_ANDROID_USE_SUSPEND_AWARE_CLOCK)
    using MostSuitableClock = SuspendAwareClock;
#else
    using MostSuitableClock = std::conditional_t<
        std::chrono::high_resolution_clock::is_steady,
        std::chrono::high_resolution_clock,
        std::chrono::steady_clock>;
#endif

} // namespace priv

////////////////////////////////////////////////////////////
/// \brief Utility class that measures the elapsed time
///
////////////////////////////////////////////////////////////
class SFML_SYSTEM_API Clock
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// The clock starts automatically after being constructed.
    ///
    ////////////////////////////////////////////////////////////
    Clock();

    ////////////////////////////////////////////////////////////
    /// \brief Get the elapsed time
    ///
    /// This function returns the time elapsed since the last call
    /// to restart() (or the construction of the instance if restart()
    /// has not been called).
    ///
    /// \return Time elapsed
    ///
    ////////////////////////////////////////////////////////////
    Time getElapsedTime() const;

    ////////////////////////////////////////////////////////////
    /// \brief Restart the clock
    ///
    /// This function puts the time counter back to zero.
    /// It also returns the time elapsed since the clock was started.
    ///
    /// \return Time elapsed
    ///
    ////////////////////////////////////////////////////////////
    Time restart();

private:
    using ClockImpl = priv::MostSuitableClock;

    static_assert(ClockImpl::is_steady,
        "Provided implementation is not a monotonic clock");
    static_assert(std::ratio_less_equal<ClockImpl::period, std::micro>::value,
        "Clock resolution is too low. Expecting at least a microsecond precision");

    ////////////////////////////////////////////////////////////
    /// \brief Convert clock duration to Time
    ///
    /// This function acts as a utility for converting clock
    /// duration type instance into sf::Time
    ///
    /// \return Time instance
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static Time durationToTime(ClockImpl::duration duration);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    ClockImpl::time_point m_startTime; //!< Time of last reset
};

} // namespace sf


#endif // SFML_CLOCK_HPP


////////////////////////////////////////////////////////////
/// \class sf::Clock
/// \ingroup system
///
/// sf::Clock is a lightweight class for measuring time.
///
/// Its provides the most precise time that the underlying
/// OS can achieve (generally microseconds or nanoseconds).
/// It also ensures monotonicity, which means that the returned
/// time can never go backward, even if the system time is
/// changed.
///
/// Usage example:
/// \code
/// sf::Clock clock;
/// ...
/// Time time1 = clock.getElapsedTime();
/// ...
/// Time time2 = clock.restart();
/// \endcode
///
/// The sf::Time value returned by the clock can then be
/// converted to a number of seconds, milliseconds or even
/// microseconds.
///
/// \see sf::Time
///
////////////////////////////////////////////////////////////

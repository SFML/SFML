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

#ifndef SFML_TIME_HPP
#define SFML_TIME_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <chrono>
#include <type_traits>

namespace sf
{

////////////////////////////////////////////////////////////
/// \brief Time unit aliases templates
///
/// These alias templates allow for easy time unit representations
/// and are basically types from <chrono> library, underneath.
/// When alias has an unspecified template parameter
/// the count type is the most suitable integral for a given
/// time unit e.g. sf::Nanoseconds<>(1).count() will likely
/// be represented with an sf::Int64. If a different type
/// is needed, one can supply it as a template parameter
/// e.g. sf::Milliseconds<float>(5.5f).count()
///
/// sf::duration_cast works just like static_cast but it is
/// intended for time durations. It is only required when
/// the conversion can cause overflow or truncation.
///
/// Casting between integer durations where the source period
/// is exactly divisible by the target period (e.g. hours to minutes)
/// or between floating-point durations can be performed implicitly
/// via constructors, no duration_cast is needed.
///
/// For more information, visit:
/// https://en.cppreference.com/w/cpp/header/chrono
///
////////////////////////////////////////////////////////////

template <typename T = std::chrono::nanoseconds::rep>
using Nanoseconds = std::chrono::duration<T, std::nano>;

template <typename T = std::chrono::microseconds::rep>
using Microseconds = std::chrono::duration<T, std::micro>;

template <typename T = std::chrono::milliseconds::rep>
using Milliseconds = std::chrono::duration<T, std::milli>;

template <typename T = std::chrono::seconds::rep>
using Seconds = std::chrono::duration<T, std::ratio<1>>;

template <typename T = std::chrono::minutes::rep>
using Minutes = std::chrono::duration<T, std::ratio<60>>;

template <typename T = std::chrono::hours::rep>
using Hours = std::chrono::duration<T, std::ratio<3600>>;

using std::chrono::duration_cast;

} // namespace sf

#endif // SFML_TIME_HPP


/// CHANGES TO DOC - TO BE CONTINUED - this is a draft
////////////////////////////////////////////////////////////
/// \class sf::Time
/// \ingroup system
///
/// <delete> sf::Time encapsulates a time value in a flexible way.
/// It allows to define a time value either as a number of
/// seconds, milliseconds or microseconds. It also works the
/// other way round: you can read a time value as either
/// a number of seconds, milliseconds or microseconds.
///
/// By using such a flexible interface, the API doesn't
/// impose any fixed type or resolution for time values,
/// and let the user choose its own favorite representation.
///
/// Time values support the usual mathematical operations:
/// you can add or subtract two times, multiply or divide
/// a time by a number, compare two times, etc.
///
/// Since they represent a time span and not an absolute time
/// value, times can also be negative. </delete>
///
/// Usage example:
/// \code
/// constexpr auto t1 = sf::Seconds<float>(0.1f);
/// static_assert(t1.count() == 0.1f);
///
/// // Overflow possible - need to use duration_cast
/// constexpr auto milli = sf::duration_cast<sf::Milliseconds<>>(t1).count();
/// static_assert(milli == 100);
///
/// constexpr auto t2 = sf::Milliseconds<>(30);
/// static_assert(t2.count() == 30);
///
/// // No overflow possible - constructor available
/// constexpr auto micro = sf::Microseconds<>(t2);
/// static_assert(micro.count() == 30'000);
///
/// constexpr auto t3 = sf::Microseconds<>(-800'000);
/// static_assert(t3.count() == -800'000);
///
/// constexpr float sec = sf::duration_cast<sf::Seconds<float>>(t3).count();
/// static_assert(sec == -0.8f);
///
/// struct Game
/// {
///     constexpr void update(sf::Seconds<float> elapsed)
///     {
///         position += speed * elapsed.count();
///     };
///
///     float position = 0.f;
///     const float speed = 2.f;
/// };
/// static_assert([] {
///     Game game;
///     game.update(sf::Milliseconds<>(100));
///     // game.position == 0.2f
///     game.update(sf::Nanoseconds<>(100'000'000));
///     // game.position == 0.4f
///     return game.position;
/// }() == 0.4f);
/// \endcode
///
/// \see sf::Clock
///
////////////////////////////////////////////////////////////

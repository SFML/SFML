////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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


namespace sf
{
class Time;

////////////////////////////////////////////////////////////
/// \ingroup system
/// \brief Make the current thread sleep for a given duration
///
/// sf::sleep is the best way to block a program or one of its
/// threads, as it doesn't consume any CPU power. Compared to
/// the standard std::this_thread::sleep_for function, this
/// one provides more accurate sleeping time thanks to some
/// platform-specific tweaks.
///
/// sf::sleep only guarantees millisecond precision. Sleeping
/// for a duration less than 1 millisecond is prone to result
/// in the actual sleep duration being less than what is
/// requested.
///
/// \param duration Time to sleep
///
////////////////////////////////////////////////////////////
void SFML_SYSTEM_API sleep(Time duration);

} // namespace sf

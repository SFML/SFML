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

#ifndef SFML_CLOCK_H
#define SFML_CLOCK_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/System/Types.h>


////////////////////////////////////////////////////////////
/// Create a new clock and start it
///
/// \return A new sfClock object
///
////////////////////////////////////////////////////////////
CSFML_API sfClock* sfClock_Create(void);

////////////////////////////////////////////////////////////
/// Copy an existing clock
///
/// \param clock : Clock to copy
///
/// \return Copied object
///
////////////////////////////////////////////////////////////
CSFML_API sfClock* sfClock_Copy(sfClock* clock);

////////////////////////////////////////////////////////////
/// Destroy an existing clock
///
/// \param clock : Clock to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfClock_Destroy(sfClock* clock);

////////////////////////////////////////////////////////////
/// Get the time elapsed for a clock
///
/// \param clock : Clock to get time from
///
////////////////////////////////////////////////////////////
CSFML_API float sfClock_GetTime(const sfClock* clock);

////////////////////////////////////////////////////////////
/// Restart a clock
///
/// \param clock : Clock to restart
///
////////////////////////////////////////////////////////////
CSFML_API void sfClock_Reset(sfClock* clock);


#endif // SFML_CLOCK_H

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

#ifndef SFML_LISTENER_H
#define SFML_LISTENER_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>


////////////////////////////////////////////////////////////
/// Change the global volume of all the sounds
///
/// \param volume : New global volume, in the range [0, 100]
///
////////////////////////////////////////////////////////////
CSFML_API void sfListener_SetGlobalVolume(float volume);

////////////////////////////////////////////////////////////
/// Get the current value of the global volume of all the sounds
///
/// \return Current global volume, in the range [0, 100]
///
////////////////////////////////////////////////////////////
CSFML_API float sfListener_GetGlobalVolume(void);

////////////////////////////////////////////////////////////
/// Change the position of the listener
///
/// \param x : X position of the listener in the world
/// \param y : Y position of the listener in the world
/// \param z : Z position of the listener in the world
///
////////////////////////////////////////////////////////////
CSFML_API void sfListener_SetPosition(float x, float y, float z);

////////////////////////////////////////////////////////////
/// Get the current position of the listener
///
/// \param x : X position of the listener in the world
/// \param y : Y position of the listener in the world
/// \param z : Z position of the listener in the world
///
////////////////////////////////////////////////////////////
CSFML_API void sfListener_GetPosition(float* x, float* y, float* z);

////////////////////////////////////////////////////////////
/// Change the orientation of the listener
///
/// \param x : X component of the listener's direction
/// \param y : Y component of the listener's direction
/// \param z : Z component of the listener's direction
///
////////////////////////////////////////////////////////////
CSFML_API void sfListener_SetDirection(float x, float y, float z);

////////////////////////////////////////////////////////////
/// Get the current orientation of the listener
///
/// \param x : X component of the listener's direction
/// \param y : Y component of the listener's direction
/// \param z : Z component of the listener's direction
///
////////////////////////////////////////////////////////////
CSFML_API void sfListener_GetDirection(float* x, float* y, float* z);


#endif // SFML_LISTENER_H

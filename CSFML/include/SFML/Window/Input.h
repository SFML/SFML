////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)//
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

#ifndef SFML_INPUT_H
#define SFML_INPUT_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Window/Event.h>
#include <SFML/Window/Types.h>


////////////////////////////////////////////////////////////
/// Get the state of a key
///
/// \param input : Input object
/// \param code :  Key to check
///
/// \return sfTrue if key is down, sfFalse if key is up
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfInput_IsKeyDown(const sfInput* input, sfKeyCode code);

////////////////////////////////////////////////////////////
/// Get the state of a mouse button
///
/// \param input :  Input object
/// \param button : Button to check
///
/// \return sfTrue if button is down, sfFalse if button is up
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfInput_IsMouseButtonDown(const sfInput* input, sfMouseButton button);

////////////////////////////////////////////////////////////
/// Get the state of a joystick button
///
/// \param input :  Input object
/// \param joyId :  Identifier of the joystick to check (0 or 1)
/// \param button : Button to check
///
/// \return sfTrue if button is down, sfFalse if button is up
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfInput_IsJoystickButtonDown(const sfInput* input, unsigned int joyId, unsigned int button);

////////////////////////////////////////////////////////////
/// Get the mouse X position
///
/// \param input : Input object
///
/// \return Current mouse left position, relative to owner window
///
////////////////////////////////////////////////////////////
CSFML_API int sfInput_GetMouseX(const sfInput* input);

////////////////////////////////////////////////////////////
/// Get the mouse Y position
///
/// \param input : Input object
///
/// \return Current mouse top position, relative to owner window
///
////////////////////////////////////////////////////////////
CSFML_API int sfInput_GetMouseY(const sfInput* input);

////////////////////////////////////////////////////////////
/// Get the joystick position on a given axis
///
/// \param input : Input object
/// \param joyId : Identifier of the joystick to check (0 or 1)
/// \param axis :  Identifier of the axis to read
///
/// \return Current joystick position, in the range [-100, 100]
///
////////////////////////////////////////////////////////////
CSFML_API float sfInput_GetJoystickAxis(const sfInput* input, unsigned int joyId, sfJoyAxis axis);


#endif // SFML_INPUT_H

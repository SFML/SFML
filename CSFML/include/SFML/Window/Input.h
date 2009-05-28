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
/// \param Input :   Input object
/// \param KeyCode : Key to check
///
/// \return sfTrue if key is down, sfFalse if key is up
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfInput_IsKeyDown(sfInput* Input, sfKeyCode KeyCode);

////////////////////////////////////////////////////////////
/// Get the state of a mouse button
///
/// \param Input :  Input object
/// \param Button : Button to check
///
/// \return sfTrue if button is down, sfFalse if button is up
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfInput_IsMouseButtonDown(sfInput* Input, sfMouseButton Button);

////////////////////////////////////////////////////////////
/// Get the state of a joystick button
///
/// \param Input :  Input object
/// \param JoyId :  Identifier of the joystick to check (0 or 1)
/// \param Button : Button to check
///
/// \return sfTrue if button is down, sfFalse if button is up
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfInput_IsJoystickButtonDown(sfInput* Input, unsigned int JoyId, unsigned int Button);

////////////////////////////////////////////////////////////
/// Get the mouse X position
///
/// \param Input : Input object
///
/// \return Current mouse left position, relative to owner window
///
////////////////////////////////////////////////////////////
CSFML_API int sfInput_GetMouseX(sfInput* Input);

////////////////////////////////////////////////////////////
/// Get the mouse Y position
///
/// \param Input : Input object
///
/// \return Current mouse top position, relative to owner window
///
////////////////////////////////////////////////////////////
CSFML_API int sfInput_GetMouseY(sfInput* Input);

////////////////////////////////////////////////////////////
/// Get the joystick position on a given axis
///
/// \param Input : Input object
/// \param JoyId : Identifier of the joystick to check (0 or 1)
/// \param Axis :  Identifier of the axis to read
///
/// \return Current joystick position, in the range [-100, 100]
///
////////////////////////////////////////////////////////////
CSFML_API float sfInput_GetJoystickAxis(sfInput* Input, unsigned int JoyId, sfJoyAxis Axis);


#endif // SFML_INPUT_H

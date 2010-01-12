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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Input.h>
#include <SFML/Window/InputStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Get the state of a key
////////////////////////////////////////////////////////////
sfBool sfInput_IsKeyDown(const sfInput* input, sfKeyCode code)
{
    CSFML_CALL_PTR_RETURN(input, IsKeyDown((sf::Key::Code)code), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the state of a mouse button
////////////////////////////////////////////////////////////
sfBool sfInput_IsMouseButtonDown(const sfInput* input, sfMouseButton button)
{
    CSFML_CALL_PTR_RETURN(input, IsMouseButtonDown((sf::Mouse::Button)button), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the state of a joystick button
////////////////////////////////////////////////////////////
sfBool sfInput_IsJoystickButtonDown(const sfInput* input, unsigned int joyId, unsigned int button)
{
    CSFML_CALL_PTR_RETURN(input, IsJoystickButtonDown(joyId, button), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the mouse X position
////////////////////////////////////////////////////////////
int sfInput_GetMouseX(const sfInput* input)
{
    CSFML_CALL_PTR_RETURN(input, GetMouseX(), 0);
}


////////////////////////////////////////////////////////////
/// Get the mouse Y position
////////////////////////////////////////////////////////////
int sfInput_GetMouseY(const sfInput* input)
{
    CSFML_CALL_PTR_RETURN(input, GetMouseY(), 0);
}

////////////////////////////////////////////////////////////
/// Get the joystick position on a given axis
////////////////////////////////////////////////////////////
float sfInput_GetJoystickAxis(const sfInput* input, unsigned int joyId, sfJoyAxis axis)
{
    CSFML_CALL_PTR_RETURN(input, GetJoystickAxis(joyId, (sf::Joy::Axis)axis), 0.f);
}

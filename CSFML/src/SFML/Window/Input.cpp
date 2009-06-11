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
sfBool sfInput_IsKeyDown(sfInput* Input, sfKeyCode KeyCode)
{
    CSFML_CALL_PTR_RETURN(Input, IsKeyDown((sf::Key::Code)KeyCode), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the state of a mouse button
////////////////////////////////////////////////////////////
sfBool sfInput_IsMouseButtonDown(sfInput* Input, sfMouseButton Button)
{
    CSFML_CALL_PTR_RETURN(Input, IsMouseButtonDown((sf::Mouse::Button)Button), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the state of a joystick button
////////////////////////////////////////////////////////////
sfBool sfInput_IsJoystickButtonDown(sfInput* Input, unsigned int JoyId, unsigned int Button)
{
    CSFML_CALL_PTR_RETURN(Input, IsJoystickButtonDown(JoyId, Button), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the mouse X position
////////////////////////////////////////////////////////////
int sfInput_GetMouseX(sfInput* Input)
{
    CSFML_CALL_PTR_RETURN(Input, GetMouseX(), 0);
}


////////////////////////////////////////////////////////////
/// Get the mouse Y position
////////////////////////////////////////////////////////////
int sfInput_GetMouseY(sfInput* Input)
{
    CSFML_CALL_PTR_RETURN(Input, GetMouseY(), 0);
}

////////////////////////////////////////////////////////////
/// Get the joystick position on a given axis
////////////////////////////////////////////////////////////
float sfInput_GetJoystickAxis(sfInput* Input, unsigned int JoyId, sfJoyAxis Axis)
{
    CSFML_CALL_PTR_RETURN(Input, GetJoystickAxis(JoyId, (sf::Joy::Axis)Axis), 0.f);
}

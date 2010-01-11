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
#include <SFML/Window/Input.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
Input::Input() :
myMouseX(0),
myMouseY(0)
{
    ResetStates();
}


////////////////////////////////////////////////////////////
bool Input::IsKeyDown(Key::Code key) const
{
    return myKeys[key];
}


////////////////////////////////////////////////////////////
bool Input::IsMouseButtonDown(Mouse::Button button) const
{
    return myMouseButtons[button];
}


////////////////////////////////////////////////////////////
bool Input::IsJoystickButtonDown(unsigned int joystick, unsigned int button) const
{
    if ((joystick < Joy::Count) && (button < Joy::ButtonCount))
        return myJoystickButtons[joystick][button];
    else
        return false;
}


////////////////////////////////////////////////////////////
int Input::GetMouseX() const
{
    return myMouseX;
}


////////////////////////////////////////////////////////////
int Input::GetMouseY() const
{
    return myMouseY;
}


////////////////////////////////////////////////////////////
float Input::GetJoystickAxis(unsigned int joystick, Joy::Axis axis) const
{
    if (joystick < Joy::Count)
        return myJoystickAxis[joystick][axis];
    else
        return 0.f;
}


////////////////////////////////////////////////////////////
void Input::OnEvent(const Event& event)
{
    switch (event.Type)
    {
        // Key events
        case Event::KeyPressed :  myKeys[event.Key.Code] = true;  break;
        case Event::KeyReleased : myKeys[event.Key.Code] = false; break;

        // Mouse event
        case Event::MouseButtonPressed :  myMouseButtons[event.MouseButton.Button] = true;  break;
        case Event::MouseButtonReleased : myMouseButtons[event.MouseButton.Button] = false; break;

        // Mouse move event
        case Event::MouseMoved :
            myMouseX = event.MouseMove.X;
            myMouseY = event.MouseMove.Y;
            break;

        // Joystick button events
        case Event::JoyButtonPressed :  myJoystickButtons[event.JoyButton.JoystickId][event.JoyButton.Button] = true;  break;
        case Event::JoyButtonReleased : myJoystickButtons[event.JoyButton.JoystickId][event.JoyButton.Button] = false; break;

        // Joystick move event
        case Event::JoyMoved :
            myJoystickAxis[event.JoyMove.JoystickId][event.JoyMove.Axis] = event.JoyMove.Position;
            break;

        // Lost focus event : we must reset all persistent states
        case Event::LostFocus :
            ResetStates();
            break;

        default :
            break;
    }
}


////////////////////////////////////////////////////////////
void Input::ResetStates()
{
    for (int i = 0; i < Key::Count; ++i)
        myKeys[i] = false;

    for (int i = 0; i < Mouse::ButtonCount; ++i)
        myMouseButtons[i] = false;

    for (int i = 0; i < Joy::Count; ++i)
    {
        for (int j = 0; j < Joy::ButtonCount; ++j)
            myJoystickButtons[i][j] = false;

        for (int j = 0; j < Joy::AxisCount; ++j)
            myJoystickAxis[i][j] = 0.f;
        myJoystickAxis[i][Joy::AxisPOV] = -1.f;
    }
}

} // namespace sf

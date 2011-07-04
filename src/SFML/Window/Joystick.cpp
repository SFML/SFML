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
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/JoystickManager.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
bool Joystick::IsConnected(unsigned int joystick)
{
    return priv::JoystickManager::GetInstance().GetState(joystick).Connected;
}


////////////////////////////////////////////////////////////
unsigned int Joystick::GetButtonCount(unsigned int joystick)
{
    return priv::JoystickManager::GetInstance().GetCapabilities(joystick).ButtonCount;
}


////////////////////////////////////////////////////////////
bool Joystick::HasAxis(unsigned int joystick, Axis axis)
{
    return priv::JoystickManager::GetInstance().GetCapabilities(joystick).Axes[axis];
}


////////////////////////////////////////////////////////////
bool Joystick::IsButtonPressed(unsigned int joystick, int button)
{
    return priv::JoystickManager::GetInstance().GetState(joystick).Buttons[button];
}


////////////////////////////////////////////////////////////
float Joystick::GetAxisPosition(unsigned int joystick, Axis axis)
{
    return priv::JoystickManager::GetInstance().GetState(joystick).Axes[axis];
}


////////////////////////////////////////////////////////////
void Joystick::Update()
{
    return priv::JoystickManager::GetInstance().Update();
}

} // namespace sf

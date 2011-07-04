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
#include <SFML/Window/JoystickManager.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
JoystickManager& JoystickManager::GetInstance()
{
    static JoystickManager instance;
    return instance;
}


////////////////////////////////////////////////////////////
const JoystickCaps& JoystickManager::GetCapabilities(unsigned int joystick) const
{
    return myJoysticks[joystick].Capabilities;
}


////////////////////////////////////////////////////////////
const JoystickState& JoystickManager::GetState(unsigned int joystick) const
{
    return myJoysticks[joystick].State;
}


////////////////////////////////////////////////////////////
void JoystickManager::Update()
{
    for (int i = 0; i < Joystick::Count; ++i)
    {
        Item& item = myJoysticks[i];

        if (item.State.Connected)
        {
            // Get the current state of the joystick
            item.State = item.Joystick.Update();

            // Check if it's still connected
            if (!item.State.Connected)
            {
                item.Joystick.Close();
                item.Capabilities = JoystickCaps();
                item.State = JoystickState();
            }
        }
        else
        {
            // Check if the joystick was connected since last update
            if (JoystickImpl::IsConnected(i))
            {
                if (item.Joystick.Open(i))
                {
                    item.Capabilities = item.Joystick.GetCapabilities();
                    item.State = item.Joystick.Update();
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////
JoystickManager::JoystickManager()
{
}


////////////////////////////////////////////////////////////
JoystickManager::~JoystickManager()
{
    for (int i = 0; i < Joystick::Count; ++i)
    {
        if (myJoysticks[i].State.Connected)
            myJoysticks[i].Joystick.Close();
    }
}

} // namespace priv

} // namespace sf

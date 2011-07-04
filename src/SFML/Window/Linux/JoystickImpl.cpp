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
#include <SFML/Window/JoystickImpl.hpp>
#include <sys/stat.h>
#include <errno.h>
#include <sstream>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
bool JoystickImpl::IsConnected(unsigned int index)
{
    std::ostringstream oss;
    oss << "/dev/input/js" << index;

    struct stat info; 
    return stat(oss.str().c_str(), &info) == 0; 
}


////////////////////////////////////////////////////////////
bool JoystickImpl::Open(unsigned int index)
{
    std::ostringstream oss;
    oss << "/dev/input/js" << index;

    myFile = open(oss.str().c_str(), O_RDONLY);
    if (myFile > 0)
    {
        // Use non-blocking mode
        fcntl(myFile, F_SETFL, O_NONBLOCK);

        // Retrieve the axes mapping
        ioctl(myFile, JSIOCGAXMAP, myMapping);

        // Reset the joystick state
        myState = JoystickState();

        return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
void JoystickImpl::Close()
{
    close(myFile);
}


////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::GetCapabilities() const
{
    JoystickCaps caps;

    // Get the number of buttons
    char buttonCount;
    ioctl(myFile, JSIOCGBUTTONS, &buttonCount);
    caps.ButtonCount = buttonCount;
    if (caps.ButtonCount > Joystick::ButtonCount)
        caps.ButtonCount = Joystick::ButtonCount;

    // Get the supported axes
    char axesCount;
    ioctl(myFile, JSIOCGAXES, &axesCount);
    for (int i = 0; i < axesCount; ++i)
    {
        switch (myMapping[i])
        {
            case ABS_X :        caps.Axes[Joystick::X]    = true; break;
            case ABS_Y :        caps.Axes[Joystick::Y]    = true; break;
            case ABS_Z :
            case ABS_THROTTLE : caps.Axes[Joystick::Z]    = true; break;
            case ABS_RZ:
            case ABS_RUDDER:    caps.Axes[Joystick::R]    = true; break;
            case ABS_RX :       caps.Axes[Joystick::U]    = true; break;
            case ABS_RY :       caps.Axes[Joystick::V]    = true; break;
            case ABS_HAT0X :    caps.Axes[Joystick::PovX] = true; break;
            case ABS_HAT0Y :    caps.Axes[Joystick::PovY] = true; break;
            default : break;
        }
    }

    return caps;
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::JoystickImpl::Update()
{
    // pop events from the joystick file
    js_event joyState;
    while (read(myFile, &joyState, sizeof(joyState)) > 0)
    {
        switch (joyState.type & ~JS_EVENT_INIT)
        {
            // An axis was moved
            case JS_EVENT_AXIS :
            {
                float value = joyState.value * 100.f / 32767.f;
                switch (myMapping[joyState.number])
                {
                    case ABS_X :        myState.Axes[Joystick::X]    = value; break;
                    case ABS_Y :        myState.Axes[Joystick::Y]    = value; break;
                    case ABS_Z :
                    case ABS_THROTTLE : myState.Axes[Joystick::Z]    = value; break;
                    case ABS_RZ:
                    case ABS_RUDDER:    myState.Axes[Joystick::R]    = value; break;
                    case ABS_RX :       myState.Axes[Joystick::U]    = value; break;
                    case ABS_RY :       myState.Axes[Joystick::V]    = value; break;
                    case ABS_HAT0X :    myState.Axes[Joystick::PovX] = value; break;
                    case ABS_HAT0Y :    myState.Axes[Joystick::PovY] = value; break;
                    default : break;
                }
                break;
            }

            // A button was pressed
            case JS_EVENT_BUTTON :
            {
                if (joyState.number < Joystick::ButtonCount)
                    myState.Buttons[joyState.number] = (joyState.value != 0);
                break;
            }
        }
    }

    // Check the connection state of the joystick (read() fails with an error != EGAIN if it's no longer connected)
    myState.Connected = (errno == EAGAIN);

    return myState;
}

} // namespace priv

} // namespace sf

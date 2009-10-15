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
#include <sstream>

#if defined(SFML_SYSTEM_LINUX)
    #include <linux/joystick.h>
    #include <fcntl.h>
#elif defined(SFML_SYSTEM_FREEBSD)
    // #include <sys/joystick.h> ?
#endif


namespace sf
{
namespace priv
{
#if defined(SFML_SYSTEM_LINUX)

////////////////////////////////////////////////////////////
void Joystick::Initialize(unsigned int index)
{
    // Initial state
    myNbAxes    = 0;
    myNbButtons = 0;
    for (int i = 0; i < Joy::Count; ++i)
        myState.Axis[i] = 0.f;
    for (int i = 0; i < JoystickState::MaxButtons; ++i)
        myState.Buttons[i] = false;

    // Open the joystick handle
    std::ostringstream oss;
    oss << "/dev/input/js" << index;
    myDescriptor = open(oss.str().c_str(), O_RDONLY);
    if (myDescriptor > 0)
    {
        // Use non-blocking mode
        fcntl(myDescriptor, F_SETFL, O_NONBLOCK);
        
        // Get number of axes and buttons
        char nbAxes, nbButtons;
        ioctl(myDescriptor, JSIOCGAXES,    &nbAxes);
        ioctl(myDescriptor, JSIOCGBUTTONS, &nbButtons);
        myNbAxes    = nbAxes;
        myNbButtons = nbButtons;
    }
}


////////////////////////////////////////////////////////////
JoystickState Joystick::UpdateState()
{
    if (myDescriptor > 0)
    {
        js_event joyState;
        while (read(myDescriptor, &joyState, sizeof(joyState)) > 0)
        {
            switch (joyState.type & ~JS_EVENT_INIT)
            {
                // An axis has been moved
                case JS_EVENT_AXIS :
                {
                    if (joyState.number < Joy::Count)
                        myState.Axis[joyState.number] = joyState.value * 100.f / 32767.f;
                    break;
                }
    
                // A button has been pressed
                case JS_EVENT_BUTTON :
                {
                    if (joyState.number < GetButtonsCount())
                        myState.Buttons[joyState.number] = (joyState.value != 0);
                    break;
                }
            }
        }
    }

    return myState;
}


////////////////////////////////////////////////////////////
unsigned int Joystick::GetAxesCount() const
{
    return myNbAxes;
}


////////////////////////////////////////////////////////////
unsigned int Joystick::GetButtonsCount() const
{
    return myNbButtons;
}


#elif defined(SFML_SYSTEM_FREEBSD)


////////////////////////////////////////////////////////////
void Joystick::Initialize(unsigned int index)
{
}


////////////////////////////////////////////////////////////
JoystickState Joystick::UpdateState()
{
    return JoystickState();
}


////////////////////////////////////////////////////////////
unsigned int Joystick::GetAxesCount() const
{
    return 0;
}


////////////////////////////////////////////////////////////
unsigned int Joystick::GetButtonsCount() const
{
    return 0;
}

#endif // defined(SFML_SYSTEM_FREEBSD)

} // namespace priv

} // namespace sf

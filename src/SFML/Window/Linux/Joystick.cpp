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
/// Initialize the instance and bind it to a physical joystick
////////////////////////////////////////////////////////////
void Joystick::Initialize(unsigned int Index)
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
    oss << "/dev/input/js" << Index;
    myDescriptor = open(oss.str().c_str(), O_RDONLY);
    if (myDescriptor > 0)
    {
        // Use non-blocking mode
        fcntl(myDescriptor, F_SETFL, O_NONBLOCK);
        
        // Get number of axes and buttons
        char NbAxes, NbButtons;
        ioctl(myDescriptor, JSIOCGAXES,    &NbAxes);
        ioctl(myDescriptor, JSIOCGBUTTONS, &NbButtons);
        myNbAxes    = NbAxes;
        myNbButtons = NbButtons;
    }
}


////////////////////////////////////////////////////////////
/// Update the current joystick and return its new state
////////////////////////////////////////////////////////////
JoystickState Joystick::UpdateState()
{
    if (myDescriptor > 0)
    {
        js_event JoyState;
        while (read(myDescriptor, &JoyState, sizeof(JoyState)) > 0)
        {
            switch (JoyState.type & ~JS_EVENT_INIT)
            {
                // An axis has been moved
                case JS_EVENT_AXIS :
                {
                    if (JoyState.number < Joy::Count)
                        myState.Axis[JoyState.number] = JoyState.value * 100.f / 32767.f;
                    break;
                }
    
                // A button has been pressed
                case JS_EVENT_BUTTON :
                {
                    if (JoyState.number < GetButtonsCount())
                        myState.Buttons[JoyState.number] = (JoyState.value != 0);
                    break;
                }
            }
        }
    }

    return myState;
}


////////////////////////////////////////////////////////////
/// Get the number of axes supported by the joystick
////////////////////////////////////////////////////////////
unsigned int Joystick::GetAxesCount() const
{
    return myNbAxes;
}


////////////////////////////////////////////////////////////
/// Get the number of buttons supported by the joystick
////////////////////////////////////////////////////////////
unsigned int Joystick::GetButtonsCount() const
{
    return myNbButtons;
}


#elif defined(SFML_SYSTEM_FREEBSD)


////////////////////////////////////////////////////////////
/// Initialize the instance and bind it to a physical joystick
////////////////////////////////////////////////////////////
void Joystick::Initialize(unsigned int Index)
{
}


////////////////////////////////////////////////////////////
/// Update the current joystick and return its new state
////////////////////////////////////////////////////////////
JoystickState Joystick::UpdateState()
{
    return JoystickState();
}


////////////////////////////////////////////////////////////
/// Get the number of axes supported by the joystick
////////////////////////////////////////////////////////////
unsigned int Joystick::GetAxesCount() const
{
    return 0;
}


////////////////////////////////////////////////////////////
/// Get the number of buttons supported by the joystick
////////////////////////////////////////////////////////////
unsigned int Joystick::GetButtonsCount() const
{
    return 0;
}

#endif // defined(SFML_SYSTEM_FREEBSD)

} // namespace priv

} // namespace sf

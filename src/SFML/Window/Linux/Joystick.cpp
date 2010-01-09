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
    myNbButtons = 0;
    myPovX = 0;
    myPovY = 0;
    for (int i = 0; i < Joy::ButtonCount; ++i)
    {
        myState.Buttons[i] = false;
    }
    for (int i = 0; i < Joy::AxisCount; ++i)
    {
        myState.Axis[i] = 0.f;
        myAxes[i] = false;
    }

    // Open the joystick handle
    std::ostringstream oss;
    oss << "/dev/input/js" << Index;
    myDescriptor = open(oss.str().c_str(), O_RDONLY);
    if (myDescriptor > 0)
    {
        // Use non-blocking mode
        fcntl(myDescriptor, F_SETFL, O_NONBLOCK);

        // Get number of buttons
        char NbButtons;
        ioctl(myDescriptor, JSIOCGBUTTONS, &NbButtons);
        myNbButtons = NbButtons;
        if (myNbButtons > Joy::ButtonCount)
            myNbButtons = Joy::ButtonCount;

        // Get the supported axes
        char NbAxes;
        ioctl(myDescriptor, JSIOCGAXES, &NbAxes);
        ioctl(myDescriptor, JSIOCGAXMAP, myAxesMapping);
        for (int i = 0; i < NbAxes; ++i)
        {
            switch (myAxesMapping[i])
            {
                case ABS_X :                      myAxes[Joy::AxisX]   = true; break;
                case ABS_Y :                      myAxes[Joy::AxisY]   = true; break;
                case ABS_Z : case ABS_THROTTLE :  myAxes[Joy::AxisZ]   = true; break;
                case ABS_RZ: case ABS_RUDDER:     myAxes[Joy::AxisR]   = true; break;
                case ABS_RX :                     myAxes[Joy::AxisU]   = true; break;
                case ABS_RY :                     myAxes[Joy::AxisV]   = true; break;
                case ABS_HAT0X : case ABS_HAT0Y : myAxes[Joy::AxisPOV] = true; break;
                default : break;
            }
        }
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
                    switch (myAxesMapping[JoyState.number])
                    {
                        case ABS_X :                      myState.Axis[Joy::AxisX] = JoyState.value * 100.f / 32767.f; break;
                        case ABS_Y :                      myState.Axis[Joy::AxisY] = JoyState.value * 100.f / 32767.f; break;
                        case ABS_Z : case ABS_THROTTLE :  myState.Axis[Joy::AxisZ] = JoyState.value * 100.f / 32767.f; break;
                        case ABS_RZ: case ABS_RUDDER:     myState.Axis[Joy::AxisR] = JoyState.value * 100.f / 32767.f; break;
                        case ABS_RX :                     myState.Axis[Joy::AxisU] = JoyState.value * 100.f / 32767.f; break;
                        case ABS_RY :                     myState.Axis[Joy::AxisV] = JoyState.value * 100.f / 32767.f; break;
                        case ABS_HAT0X :                  myPovX = JoyState.value;                                     break;
                        case ABS_HAT0Y :                  myPovY = JoyState.value;                                     break;
                        default : break;
                    }

                    // Compute the new POV angle
                    if (myPovX > 0)
                    {
                        if      (myPovY > 0) myState.Axis[Joy::AxisPOV] = 135.f;
                        else if (myPovY < 0) myState.Axis[Joy::AxisPOV] = 45.f;
                        else                 myState.Axis[Joy::AxisPOV] = 90.f;
                    }
                    else if (myPovX < 0)
                    {
                        if      (myPovY > 0) myState.Axis[Joy::AxisPOV] = 225.f;
                        else if (myPovY < 0) myState.Axis[Joy::AxisPOV] = 315.f;
                        else                 myState.Axis[Joy::AxisPOV] = 270.f;
                    }
                    else
                    {
                        if      (myPovY > 0) myState.Axis[Joy::AxisPOV] = 180.f;
                        else if (myPovY < 0) myState.Axis[Joy::AxisPOV] = 0.f;
                        else                 myState.Axis[Joy::AxisPOV] = -1.f;
                    }
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
/// Check if the joystick supports the given axis
////////////////////////////////////////////////////////////
bool Joystick::HasAxis(Joy::Axis Axis) const
{
    return myAxes[Axis];
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
/// Check if the joystick supports the given axis
////////////////////////////////////////////////////////////
bool Joystick::HasAxis(Joy::Axis Axis) const
{
    return false;
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

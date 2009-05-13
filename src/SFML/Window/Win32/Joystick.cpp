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
#define _WIN32_WINDOWS 0x0501
#define _WIN32_WINNT   0x0501
#include <SFML/Window/Joystick.hpp>
#include <windows.h>
#include <mmsystem.h>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Initialize the instance and bind it to a physical joystick
////////////////////////////////////////////////////////////
void Joystick::Initialize(unsigned int Index)
{
    // Reset state
    myIndex     = JOYSTICKID1;
    myNbAxes    = 0;
    myNbButtons = 0;

    // Get the Index-th connected joystick
    MMRESULT Error;
    JOYINFOEX JoyInfo;
    JoyInfo.dwSize = sizeof(JoyInfo);
    JoyInfo.dwFlags = JOY_RETURNALL;
    for (unsigned int NbFound = 0; (Error = joyGetPosEx(myIndex, &JoyInfo)) != JOYERR_PARMS; myIndex++)
    {
        // Check if the current joystick is connected
        if (Error == JOYERR_NOERROR)
        {
            // Check if it's the required index
            if (NbFound == Index)
            {
                // Ok : store its parameters and return
                JOYCAPS Caps;
                joyGetDevCaps(myIndex, &Caps, sizeof(Caps));
                myNbAxes    = Caps.wNumAxes;
                myNbButtons = Caps.wNumButtons;
                if (myNbButtons > JoystickState::MaxButtons)
                    myNbButtons = JoystickState::MaxButtons;

                return;
            }

            // Go to the next valid joystick
            ++NbFound;
        }
    }
}


////////////////////////////////////////////////////////////
/// Update the current joystick and return its new state
////////////////////////////////////////////////////////////
JoystickState Joystick::UpdateState()
{
    JoystickState State = {0};

    // Get the joystick caps (for range conversions)
    JOYCAPS Caps;
    if (joyGetDevCaps(myIndex, &Caps, sizeof(Caps)) == JOYERR_NOERROR)
    {
        // Get the current joystick state
        JOYINFOEX Pos;
        Pos.dwFlags = JOY_RETURNALL;
        Pos.dwSize  = sizeof(JOYINFOEX);
        if (joyGetPosEx(myIndex, &Pos) == JOYERR_NOERROR)
        {
            // Axes
            State.Axis[Joy::AxisX] = (Pos.dwXpos - (Caps.wXmax + Caps.wXmin) / 2.f) * 200.f / (Caps.wXmax - Caps.wXmin);
            State.Axis[Joy::AxisY] = (Pos.dwYpos - (Caps.wYmax + Caps.wYmin) / 2.f) * 200.f / (Caps.wYmax - Caps.wYmin);
            State.Axis[Joy::AxisZ] = (Pos.dwZpos - (Caps.wZmax + Caps.wZmin) / 2.f) * 200.f / (Caps.wZmax - Caps.wZmin);
            State.Axis[Joy::AxisR] = (Pos.dwRpos - (Caps.wRmax + Caps.wRmin) / 2.f) * 200.f / (Caps.wRmax - Caps.wRmin);
            State.Axis[Joy::AxisU] = (Pos.dwUpos - (Caps.wUmax + Caps.wUmin) / 2.f) * 200.f / (Caps.wUmax - Caps.wUmin);
            State.Axis[Joy::AxisV] = (Pos.dwVpos - (Caps.wVmax + Caps.wVmin) / 2.f) * 200.f / (Caps.wVmax - Caps.wVmin);

            // POV
            State.Axis[Joy::AxisPOV] = Pos.dwPOV / 100.f;

            // Buttons
            for (unsigned int i = 0; i < GetButtonsCount(); ++i)
                State.Buttons[i] = (Pos.dwButtons & (1 << i)) != 0;
        }
    }

    return State;
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


} // namespace priv

} // namespace sf

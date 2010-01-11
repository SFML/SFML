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
void Joystick::Initialize(unsigned int index)
{
    // Reset state
    myIndex            = JOYSTICKID1;
    myNbButtons        = 0;
    myIsConnected      = false;
    myHasContinuousPOV = false;
    for (int i = 0; i < Joy::AxisCount; ++i)
        myAxes[i] = false;

    // Get the Index-th connected joystick
    MMRESULT error;
    JOYINFOEX joyInfos;
    joyInfos.dwSize = sizeof(joyInfos);
    joyInfos.dwFlags = JOY_RETURNALL;
    for (unsigned int found = 0; (error = joyGetPosEx(myIndex, &joyInfos)) != JOYERR_PARMS; myIndex++)
    {
        // Check if the current joystick is connected
        if (error == JOYERR_NOERROR)
        {
            // Check if it's the required index
            if (found == index)
            {
                // Ok : store its parameters and return
                myIsConnected = true;
                JOYCAPS caps;
                joyGetDevCaps(myIndex, &caps, sizeof(caps));
                myNbButtons = caps.wNumButtons;
                if (myNbButtons > Joy::ButtonCount)
                    myNbButtons = Joy::ButtonCount;

                myAxes[Joy::AxisX]   = true;
                myAxes[Joy::AxisY]   = true;
                myAxes[Joy::AxisZ]   = (caps.wCaps & JOYCAPS_HASZ) != 0;
                myAxes[Joy::AxisR]   = (caps.wCaps & JOYCAPS_HASR) != 0;
                myAxes[Joy::AxisU]   = (caps.wCaps & JOYCAPS_HASU) != 0;
                myAxes[Joy::AxisV]   = (caps.wCaps & JOYCAPS_HASV) != 0;
                myAxes[Joy::AxisPOV] = (caps.wCaps & JOYCAPS_HASPOV) != 0;
                myHasContinuousPOV   = (caps.wCaps & JOYCAPS_POVCTS) != 0;

                return;
            }

            // Go to the next valid joystick
            ++found;
        }
    }
}


////////////////////////////////////////////////////////////
JoystickState Joystick::UpdateState()
{
    JoystickState state;

    if (myIsConnected)
    {
        // Get the joystick caps (for range conversions)
        JOYCAPS caps;
        if (joyGetDevCaps(myIndex, &caps, sizeof(caps)) == JOYERR_NOERROR)
        {
            // Get the current joystick state
            JOYINFOEX pos;
            pos.dwFlags  = JOY_RETURNX | JOY_RETURNY | JOY_RETURNZ | JOY_RETURNR | JOY_RETURNU | JOY_RETURNV | JOY_RETURNBUTTONS;
            pos.dwFlags |= myHasContinuousPOV ? JOY_RETURNPOVCTS : JOY_RETURNPOV;
            pos.dwSize   = sizeof(JOYINFOEX);
            if (joyGetPosEx(myIndex, &pos) == JOYERR_NOERROR)
            {
                // Axes
                state.Axis[Joy::AxisX] = (pos.dwXpos - (caps.wXmax + caps.wXmin) / 2.f) * 200.f / (caps.wXmax - caps.wXmin);
                state.Axis[Joy::AxisY] = (pos.dwYpos - (caps.wYmax + caps.wYmin) / 2.f) * 200.f / (caps.wYmax - caps.wYmin);
                state.Axis[Joy::AxisZ] = (pos.dwZpos - (caps.wZmax + caps.wZmin) / 2.f) * 200.f / (caps.wZmax - caps.wZmin);
                state.Axis[Joy::AxisR] = (pos.dwRpos - (caps.wRmax + caps.wRmin) / 2.f) * 200.f / (caps.wRmax - caps.wRmin);
                state.Axis[Joy::AxisU] = (pos.dwUpos - (caps.wUmax + caps.wUmin) / 2.f) * 200.f / (caps.wUmax - caps.wUmin);
                state.Axis[Joy::AxisV] = (pos.dwVpos - (caps.wVmax + caps.wVmin) / 2.f) * 200.f / (caps.wVmax - caps.wVmin);

                // POV
                if (pos.dwPOV != 0xFFFF)
                    state.Axis[Joy::AxisPOV] = pos.dwPOV / 100.f;
                else
                    state.Axis[Joy::AxisPOV] = -1.f;

                // Buttons
                for (unsigned int i = 0; i < GetButtonsCount(); ++i)
                    state.Buttons[i] = (pos.dwButtons & (1 << i)) != 0;
            }
        }
    }

    return state;
}


////////////////////////////////////////////////////////////
bool Joystick::HasAxis(Joy::Axis Axis) const
{
    return myAxes[Axis];
}


////////////////////////////////////////////////////////////
unsigned int Joystick::GetButtonsCount() const
{
    return myNbButtons;
}


} // namespace priv

} // namespace sf

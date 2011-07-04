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
#include <windows.h>
#include <cmath>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
bool JoystickImpl::IsConnected(unsigned int index)
{
    JOYINFOEX joyInfo;
    joyInfo.dwSize = sizeof(joyInfo);
    joyInfo.dwFlags = 0;

    return joyGetPosEx(JOYSTICKID1 + index, &joyInfo) == JOYERR_NOERROR;
}


////////////////////////////////////////////////////////////
bool JoystickImpl::Open(unsigned int index)
{
    // No explicit "open" action is required
    myIndex = JOYSTICKID1 + index;

    // Store the joystick capabilities
    return joyGetDevCaps(myIndex, &myCaps, sizeof(myCaps)) == JOYERR_NOERROR;
}


////////////////////////////////////////////////////////////
void JoystickImpl::Close()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::GetCapabilities() const
{
    JoystickCaps caps;

    caps.ButtonCount = myCaps.wNumButtons;
    if (caps.ButtonCount > Joystick::ButtonCount)
        caps.ButtonCount = Joystick::ButtonCount;

    caps.Axes[Joystick::X]    = true;
    caps.Axes[Joystick::Y]    = true;
    caps.Axes[Joystick::Z]    = (myCaps.wCaps & JOYCAPS_HASZ) != 0;
    caps.Axes[Joystick::R]    = (myCaps.wCaps & JOYCAPS_HASR) != 0;
    caps.Axes[Joystick::U]    = (myCaps.wCaps & JOYCAPS_HASU) != 0;
    caps.Axes[Joystick::V]    = (myCaps.wCaps & JOYCAPS_HASV) != 0;
    caps.Axes[Joystick::PovX] = (myCaps.wCaps & JOYCAPS_HASPOV) != 0;
    caps.Axes[Joystick::PovY] = (myCaps.wCaps & JOYCAPS_HASPOV) != 0;

    return caps;
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::Update()
{
    JoystickState state;

    // Get the current joystick state
    JOYINFOEX pos;
    pos.dwFlags  = JOY_RETURNX | JOY_RETURNY | JOY_RETURNZ | JOY_RETURNR | JOY_RETURNU | JOY_RETURNV | JOY_RETURNBUTTONS;
    pos.dwFlags |= (myCaps.wCaps & JOYCAPS_POVCTS) ? JOY_RETURNPOVCTS : JOY_RETURNPOV;
    pos.dwSize   = sizeof(JOYINFOEX);
    if (joyGetPosEx(myIndex, &pos) == JOYERR_NOERROR)
    {
        // The joystick is connected
        state.Connected = true;

        // Axes
        state.Axes[Joystick::X] = (pos.dwXpos - (myCaps.wXmax + myCaps.wXmin) / 2.f) * 200.f / (myCaps.wXmax - myCaps.wXmin);
        state.Axes[Joystick::Y] = (pos.dwYpos - (myCaps.wYmax + myCaps.wYmin) / 2.f) * 200.f / (myCaps.wYmax - myCaps.wYmin);
        state.Axes[Joystick::Z] = (pos.dwZpos - (myCaps.wZmax + myCaps.wZmin) / 2.f) * 200.f / (myCaps.wZmax - myCaps.wZmin);
        state.Axes[Joystick::R] = (pos.dwRpos - (myCaps.wRmax + myCaps.wRmin) / 2.f) * 200.f / (myCaps.wRmax - myCaps.wRmin);
        state.Axes[Joystick::U] = (pos.dwUpos - (myCaps.wUmax + myCaps.wUmin) / 2.f) * 200.f / (myCaps.wUmax - myCaps.wUmin);
        state.Axes[Joystick::V] = (pos.dwVpos - (myCaps.wVmax + myCaps.wVmin) / 2.f) * 200.f / (myCaps.wVmax - myCaps.wVmin);

        // Special case for POV, it is given as an angle
        if (pos.dwPOV != 0xFFFF)
        {
            float angle = pos.dwPOV / 36000.f * 3.141592654f;
            state.Axes[Joystick::PovX] = std::cos(angle) * 100;
            state.Axes[Joystick::PovY] = std::sin(angle) * 100;
        }
        else
        {
            state.Axes[Joystick::PovX] = 0;
            state.Axes[Joystick::PovY] = 0;
        }

        // Buttons
        for (unsigned int i = 0; i < Joystick::ButtonCount; ++i)
            state.Buttons[i] = (pos.dwButtons & (1 << i)) != 0;
    }

    return state;
}

} // namespace priv

} // namespace sf

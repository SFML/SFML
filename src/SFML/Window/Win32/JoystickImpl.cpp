////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/System/Clock.hpp>
#include <SFML/System/Err.hpp>
#include <windows.h>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <regstr.h>
#include <tchar.h>
#include <string>
#include <sstream>
#include <vector>

namespace
{
    struct ConnectionCache
    {
        ConnectionCache() : connected(false) {}
        bool connected;
        sf::Clock timer;
    };

    const sf::Time connectionRefreshDelay = sf::milliseconds(500);
    ConnectionCache connectionCache[sf::Joystick::Count];
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void JoystickImpl::initialize()
{
    // Perform the initial scan and populate the connection cache
    for (unsigned int i = 0; i < Joystick::Count; ++i)
    {
        ConnectionCache& cache = connectionCache[i];

        // Check if the joystick is connected
        JOYINFOEX joyInfo;
        joyInfo.dwSize = sizeof(joyInfo);
        joyInfo.dwFlags = 0;
        cache.connected = joyGetPosEx(JOYSTICKID1 + i, &joyInfo) == JOYERR_NOERROR;

        // start the timeout
        cache.timer.restart();
    }
}


////////////////////////////////////////////////////////////
void JoystickImpl::cleanup()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
bool JoystickImpl::isConnected(unsigned int index)
{
    // We check the connection state of joysticks only every N milliseconds,
    // because of a strange (buggy?) behaviour of joyGetPosEx when joysticks
    // are just plugged/unplugged -- it takes really long and kills the app performances
    ConnectionCache& cache = connectionCache[index];
    if (cache.timer.getElapsedTime() > connectionRefreshDelay)
    {
        cache.timer.restart();

        JOYINFOEX joyInfo;
        joyInfo.dwSize = sizeof(joyInfo);
        joyInfo.dwFlags = 0;

        cache.connected = joyGetPosEx(JOYSTICKID1 + index, &joyInfo) == JOYERR_NOERROR;
        return cache.connected;
    }
    else
    {
        return cache.connected;
    }
}


////////////////////////////////////////////////////////////
bool JoystickImpl::open(unsigned int index)
{
    // No explicit "open" action is required
    m_index = JOYSTICKID1 + index;

    // Store the joystick capabilities
    bool success = joyGetDevCaps(m_index, &m_caps, sizeof(m_caps)) == JOYERR_NOERROR;

    if (success)
    {
        m_identification.name = getDeviceName(m_index, m_caps);
        m_identification.productId = m_caps.wPid;
        m_identification.vendorId = m_caps.wMid;
    }

    return success;
}


////////////////////////////////////////////////////////////
void JoystickImpl::close()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilities() const
{
    JoystickCaps caps;

    caps.buttonCount = m_caps.wNumButtons;
    if (caps.buttonCount > Joystick::ButtonCount)
        caps.buttonCount = Joystick::ButtonCount;

    caps.axes[Joystick::X]    = true;
    caps.axes[Joystick::Y]    = true;
    caps.axes[Joystick::Z]    = (m_caps.wCaps & JOYCAPS_HASZ) != 0;
    caps.axes[Joystick::R]    = (m_caps.wCaps & JOYCAPS_HASR) != 0;
    caps.axes[Joystick::U]    = (m_caps.wCaps & JOYCAPS_HASU) != 0;
    caps.axes[Joystick::V]    = (m_caps.wCaps & JOYCAPS_HASV) != 0;
    caps.axes[Joystick::PovX] = (m_caps.wCaps & JOYCAPS_HASPOV) != 0;
    caps.axes[Joystick::PovY] = (m_caps.wCaps & JOYCAPS_HASPOV) != 0;

    return caps;
}


////////////////////////////////////////////////////////////
Joystick::Identification JoystickImpl::getIdentification() const
{
    return m_identification;
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::update()
{
    JoystickState state;

    // Get the current joystick state
    JOYINFOEX pos;
    pos.dwFlags  = JOY_RETURNX | JOY_RETURNY | JOY_RETURNZ | JOY_RETURNR | JOY_RETURNU | JOY_RETURNV | JOY_RETURNBUTTONS;
    pos.dwFlags |= (m_caps.wCaps & JOYCAPS_POVCTS) ? JOY_RETURNPOVCTS : JOY_RETURNPOV;
    pos.dwSize   = sizeof(JOYINFOEX);
    if (joyGetPosEx(m_index, &pos) == JOYERR_NOERROR)
    {
        // The joystick is connected
        state.connected = true;

        // Axes
        state.axes[Joystick::X] = (pos.dwXpos - (m_caps.wXmax + m_caps.wXmin) / 2.f) * 200.f / (m_caps.wXmax - m_caps.wXmin);
        state.axes[Joystick::Y] = (pos.dwYpos - (m_caps.wYmax + m_caps.wYmin) / 2.f) * 200.f / (m_caps.wYmax - m_caps.wYmin);
        state.axes[Joystick::Z] = (pos.dwZpos - (m_caps.wZmax + m_caps.wZmin) / 2.f) * 200.f / (m_caps.wZmax - m_caps.wZmin);
        state.axes[Joystick::R] = (pos.dwRpos - (m_caps.wRmax + m_caps.wRmin) / 2.f) * 200.f / (m_caps.wRmax - m_caps.wRmin);
        state.axes[Joystick::U] = (pos.dwUpos - (m_caps.wUmax + m_caps.wUmin) / 2.f) * 200.f / (m_caps.wUmax - m_caps.wUmin);
        state.axes[Joystick::V] = (pos.dwVpos - (m_caps.wVmax + m_caps.wVmin) / 2.f) * 200.f / (m_caps.wVmax - m_caps.wVmin);

        // Special case for POV, it is given as an angle
        if (pos.dwPOV != 0xFFFF)
        {
            float angle = pos.dwPOV / 18000.f * 3.141592654f;
            state.axes[Joystick::PovX] = std::sin(angle) * 100;
            state.axes[Joystick::PovY] = std::cos(angle) * 100;
        }
        else
        {
            state.axes[Joystick::PovX] = 0;
            state.axes[Joystick::PovY] = 0;
        }

        // Buttons
        for (unsigned int i = 0; i < Joystick::ButtonCount; ++i)
            state.buttons[i] = (pos.dwButtons & (1 << i)) != 0;
    }

    return state;
}


////////////////////////////////////////////////////////////
sf::String JoystickImpl::getDeviceName(unsigned int index, JOYCAPS caps)
{
    // Give the joystick a default name
    sf::String joystickDescription = "Unknown Joystick";

    std::basic_ostringstream<TCHAR, std::char_traits<TCHAR> > ss;
    ss << REGSTR_PATH_JOYCONFIG << "\\" << caps.szRegKey << "\\" << REGSTR_KEY_JOYCURR;
    std::basic_string<TCHAR> subkey = ss.str().substr(0, 255);

    HKEY currentKey;
    LONG result;
    HKEY rootKey = HKEY_LOCAL_MACHINE;

    result = RegOpenKeyEx(rootKey, subkey.c_str(), 0, KEY_READ, &currentKey);
    if (result != ERROR_SUCCESS)
    {
        rootKey = HKEY_CURRENT_USER;
        result = RegOpenKeyEx(rootKey, subkey.c_str(), 0, KEY_READ, &currentKey);
    }

    if (result == ERROR_SUCCESS)
    {
        ss.clear();
        ss.str(_T(""));
        ss << "Joystick" << index + 1 << REGSTR_VAL_JOYOEMNAME;
        std::basic_string<TCHAR> keyName = ss.str().substr(0, 255);

        TCHAR keyData[256];
        DWORD keyNameSize = sizeof(keyData);
        result = RegQueryValueEx(currentKey, keyName.c_str(), NULL, NULL, (LPBYTE)keyData, &keyNameSize);
        RegCloseKey(currentKey);

        if (result == ERROR_SUCCESS)
        {
            ss.clear();
            ss.str(_T(""));
            ss << REGSTR_PATH_JOYOEM << "\\" << keyData;
            subkey = ss.str().substr(0, 255);

            result = RegOpenKeyEx(rootKey, subkey.c_str(), 0, KEY_READ, &currentKey);
            if (result == ERROR_SUCCESS)
            {
                keyNameSize = sizeof(keyData);
                unsigned int productKeyLength = keyNameSize / sizeof(TCHAR);
                std::vector<TCHAR> productKey(productKeyLength);

                result = RegQueryValueEx(currentKey, REGSTR_VAL_JOYOEMNAME, NULL, NULL, (LPBYTE) &productKey[0], &keyNameSize);
                if (result == ERROR_SUCCESS)
                {
                    while (productKeyLength > 0 && productKey[productKeyLength - 1] == 0)
                    {
                        --productKeyLength;
                    }

                    joystickDescription = std::basic_string<TCHAR>(&productKey[0], productKeyLength);
                }
                else
                {
                    err() << "Unable to query name for joystick at index " << index << ": " << getErrorString(GetLastError()).toAnsiString() << std::endl;
                }
                RegCloseKey(currentKey);
            }
            else
            {
                err() << "Unable to open registry key for joystick at index " << index << ": " << getErrorString(GetLastError()).toAnsiString() << std::endl;
            }
        }
        else
        {
            err() << "Unable to query registry key for joystick at index " << index << ": " << getErrorString(GetLastError()).toAnsiString() << std::endl;
        }
    }
    else
    {
        err() << "Unable to open registry for joystick at index " << index << ": " << getErrorString(GetLastError()).toAnsiString() << std::endl;
    }

    return joystickDescription;
}


////////////////////////////////////////////////////////////
sf::String JoystickImpl::getErrorString(DWORD /*errorCode*/)
{
    std::basic_ostringstream<TCHAR, std::char_traits<TCHAR> > ss;
    TCHAR errBuff[256];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, errBuff, sizeof(errBuff), NULL);
    ss << errBuff;
    sf::String errMsg(ss.str());

    return errMsg;
}

} // namespace priv

} // namespace sf

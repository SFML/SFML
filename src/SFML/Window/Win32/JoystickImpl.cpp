////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Win32/Utils.hpp>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Win32/WindowsHeader.hpp>

#include <algorithm>
#include <array>
#include <iomanip>
#include <ostream>
#include <regstr.h>
#include <sstream>
#include <string>
#include <tchar.h>
#include <vector>

#include <cmath>

// Windows-specific joystick-specific includes
#include <hidusage.h>
#include <combaseapi.h>

namespace sf::priv
{

std::vector<JoystickImpl> JoystickImpl::m_sJoysticks;
ATOM                      JoystickImpl::m_sJoystickAtom;
HWND                      JoystickImpl::m_sJoystickHwnd;
UINT_PTR                  JoystickImpl::m_sTimerHandle;

void JoystickImpl::DispatchDeviceConnected(LPARAM deviceHandle)
{
    err() << deviceHandle << std::endl;
}

void JoystickImpl::DispatchDeviceRemoved(LPARAM deviceHandle)
{
    err() << deviceHandle << std::endl;
}

void JoystickImpl::DispatchXInput()
{
    err() << "DispatchXInput Called" << std::endl;
}

DWORD WINAPI JoystickImpl::Win32JoystickDispatchThread(LPVOID lpParam)
{
    std::ignore = lpParam;
    // Required 
    auto coInitResult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (!SUCCEEDED(coInitResult))
    {
        // :(
        auto errorCode = GetLastError();
        err() << "CoInitializeEx returned 0, GetLastError: [" << errorCode << "], Win32 Joystick will not function."
              << std::endl;
        return errorCode;
    }

    WNDCLASSEX wndClass;
    wndClass.cbSize        = sizeof(wndClass);
    wndClass.hInstance     = GetModuleHandleW(nullptr);
    wndClass.lpfnWndProc   = Win32JoystickWndProc;
    wndClass.lpszClassName = L"SFML-Win32JoystickWndProc";

    m_sJoystickAtom = RegisterClassExW(&wndClass);
    if (m_sJoystickAtom == 0x0)
    {
        // :(
        auto errorCode = GetLastError();
        err() << "RegisterClassExW returned 0, GetLastError: [" << errorCode << "], Win32 Joystick will not function."
              << std::endl;
        return errorCode;
    }

    m_sJoystickHwnd = CreateWindowExW(0,
                                      // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
                                      // says it can be an ATOM OR the name of the window class. Atom is used here.
                                      reinterpret_cast<LPCWSTR>(m_sJoystickAtom),
                                      nullptr,
                                      WS_OVERLAPPEDWINDOW,
                                      CW_USEDEFAULT,
                                      CW_USEDEFAULT,
                                      CW_USEDEFAULT,
                                      CW_USEDEFAULT,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      nullptr);
    if (m_sJoystickHwnd == 0x0)
    {
        // :(
        auto errorCode = GetLastError();
        err() << "CreateWindowExW returned 0, GetLastError: [" << errorCode << "] Win32 Joystick will not function.";
        return errorCode;
    }

    // stack-allocation
    RAWINPUTDEVICE rids[3];
    ZeroMemory(rids, sizeof(RAWINPUTDEVICE) * 3);
    for (uintptr_t i = 0; i < 3; i++)
    {
        // We are using this, talking to "generic" input devices.
        rids[i].usUsagePage = HID_USAGE_PAGE_GENERIC;
        // We want to receive inputs even out-of-focus, and notify us when devices are added or removed.
        rids[i].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
        // And send notifications to THIS specific HWND, which we registered earlier.
        rids[i].hwndTarget = m_sJoystickHwnd;
    }

    rids[0].usUsage = HID_USAGE_GENERIC_GAMEPAD;
    rids[1].usUsage = HID_USAGE_GENERIC_JOYSTICK;
    rids[2].usUsage = HID_USAGE_GENERIC_MULTI_AXIS_CONTROLLER;

    auto rawInputRetCode = RegisterRawInputDevices(rids, 3, sizeof(RAWINPUTDEVICE));
    if (rawInputRetCode == 0x0)
    {
        // :(
        auto errorCode = GetLastError();
        err() << "RegisterRawInputDevices returned 0, GetLastError: [" << errorCode
              << "] Win32 Joystick will not function.";
        return errorCode;
    }

    m_sTimerHandle = SetTimer(m_sJoystickHwnd, NULL, 8, nullptr);

    MSG msg;

    while (GetMessageW(&msg, m_sJoystickHwnd, 0, 0) != 0x0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    // Cleanup code
    KillTimer(m_sJoystickHwnd, NULL);
    return S_OK;
}

////////////////////////////////////////////////////////////
void JoystickImpl::initialize()
{
    DWORD threadId;
    auto    threadHandle = CreateThread(NULL, 0, Win32JoystickDispatchThread, nullptr, 0, &threadId);
    if (threadHandle == NULL)
    {
        auto errorCode = GetLastError();
        err() << "CreateThread returned 0, GetLastError: [" << errorCode
              << "] Win32 Joystick will not function.";
    }
}


////////////////////////////////////////////////////////////
void JoystickImpl::cleanup()
{
    CloseWindow(m_sJoystickHwnd);
}


////////////////////////////////////////////////////////////
bool JoystickImpl::isConnected(unsigned int index)
{
    if (index < m_sJoysticks.size())
    {
        return m_sJoysticks.at(index).m_isConnected;
    }
    return false;
}

////////////////////////////////////////////////////////////
bool JoystickImpl::open(unsigned int index)
{
    if (index < m_sJoysticks.size())
    {
        return m_sJoysticks.at(index).m_isConnected;
    }
    return m_isConnected;
}


////////////////////////////////////////////////////////////
void JoystickImpl::close()
{
    // stub, nothing to do here, everything is done automatically
}

////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilities() const
{
    JoystickCaps caps;

    caps.axes[Joystick::Axis::X]    = true;
    caps.axes[Joystick::Axis::Y]    = true;
    caps.axes[Joystick::Axis::Z]    = true;
    caps.axes[Joystick::Axis::R]    = true;
    caps.axes[Joystick::Axis::U]    = true;
    caps.axes[Joystick::Axis::V]    = true;
    caps.axes[Joystick::Axis::PovX] = true;
    caps.axes[Joystick::Axis::PovY] = true;

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
    return m_state;
}

LRESULT JoystickImpl::Win32JoystickWndProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
{


    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

} // namespace sf::priv

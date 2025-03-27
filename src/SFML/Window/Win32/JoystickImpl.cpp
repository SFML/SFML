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
#include <Xinput.h>
#include <hidsdi.h>
#include <hidusage.h>

#include <combaseapi.h>

constexpr auto XINPUT_MAX_CONTROLLERS    = 4u;
constexpr auto SFML_RAW_INPUT_CHUNK_SIZE = 512;

// -- Raw Input Chunks

void*     preparsedDataChunk;
void*     valueCapsDataChunk;
void*     deviceNameDataChunk;
uint16_t* usageSizeDataChunk;
void*     deviceHumanNameDataChunk;

namespace sf::priv
{

std::vector<JoystickImpl> JoystickImpl::m_sJoysticks;
ATOM                      JoystickImpl::m_sJoystickAtom;
HWND                      JoystickImpl::m_sJoystickHwnd;
UINT_PTR                  JoystickImpl::m_sTimerHandle;
UINT                      JoystickImpl::m_joystickMsgAtom = 0x0;

void JoystickImpl::DispatchDeviceConnected(HANDLE deviceHandle)
{
    // TODO: Device identity discovery

    JoystickImpl jstkImpl       = {};
    jstkImpl.m_lastDeviceHandle = deviceHandle;
    jstkImpl.m_index            = (uint32_t)m_sJoysticks.size();
    jstkImpl.m_state.connected  = true;

    HRESULT success = {0};

    UINT nameSize = {0};
    // okay this looks weird, but the docs say to call it twice like this
    success = GetRawInputDeviceInfoW(deviceHandle, RIDI_DEVICENAME, nullptr, &nameSize);
    success = GetRawInputDeviceInfoW(deviceHandle, RIDI_DEVICENAME, deviceNameDataChunk, &nameSize);
    std::wstring devicePath(reinterpret_cast<LPCWSTR>(deviceNameDataChunk));
    auto         fileHandle = CreateFileW(devicePath.data(),
                                  GENERIC_READ | GENERIC_WRITE,
                                  FILE_SHARE_READ | FILE_SHARE_WRITE,
                                  NULL,
                                  OPEN_EXISTING,
                                  0x0,
                                  NULL);
    if (HidD_GetProductString(fileHandle, deviceHumanNameDataChunk, 512))
    {
        jstkImpl.m_identification.name = String{reinterpret_cast<wchar_t*>(deviceHumanNameDataChunk)};
        USHORT vid                     = {0};
        USHORT pid                     = {0};
        bool   isXInput                = false;
        if (ExtractVidPid(devicePath, vid, pid, isXInput))
        {
            jstkImpl.m_identification.vendorId  = vid;
            jstkImpl.m_identification.productId = pid;
            jstkImpl.m_useXInput                = true;
        }
    }

    // you can theoretically override use xinput here, but from my testing the xbox controllers are awful with rawinput 

    if (jstkImpl.m_useXInput)
    {
        // XInput has 14 Buttons and 6 Axes
        jstkImpl.m_caps.buttonCount = 14;
        const auto axes             = 6;
        for (auto index = 0; index < Joystick::AxisCount && index < axes; index++)
        {
            jstkImpl.m_caps.axes[GetAxis(index)] = true;
        }
    }
    else
    {
        // RawInput query capabilities
        UINT buffSize = {0};
        success       = GetRawInputDeviceInfoW(deviceHandle, RIDI_PREPARSEDDATA, NULL, &buffSize);
        if (FAILED(success))
        {
            // that didn't work!
            auto lastError = GetLastError();
            err() << "GetRawInputData returned [" << success << "] unexpectedly! GetLastError: [" << lastError << "]"
                  << std::endl;
            return;
        }
        success       = GetRawInputDeviceInfoW(deviceHandle, RIDI_PREPARSEDDATA, preparsedDataChunk, &buffSize);
        if (FAILED(success))
        {
            // that didn't work!
            auto lastError = GetLastError();
            err() << "GetRawInputData returned [" << success << "] unexpectedly! GetLastError: [" << lastError << "]"
                  << std::endl;
            return;
        }
        PHIDP_PREPARSED_DATA preparsedData = reinterpret_cast<PHIDP_PREPARSED_DATA>(preparsedDataChunk);
        HIDP_CAPS            hCaps         = {0};
        success = HidP_GetCaps(preparsedData, &hCaps);
        if (FAILED(success))
        {
            // that didn't work!
            auto lastError = GetLastError();
            err() << "HidP_GetCaps returned [" << success << "] unexpectedly! GetLastError: [" << lastError << "]"
                  << std::endl;
            return;
        }

        jstkImpl.m_caps.buttonCount = hCaps.NumberInputButtonCaps;
        auto axes                   = hCaps.NumberInputValueCaps;
        for (auto index = 0; index < Joystick::AxisCount && index < axes; index++)
        {
            jstkImpl.m_caps.axes[GetAxis(index)] = true;
        }
    }

    unsigned int xInputIndex = 0;
    for (size_t i = 0; i < m_sJoysticks.size(); i++)
    {
        auto& jstk = m_sJoysticks.at(i);
        if (jstk.m_xInputIndex != -1)
        {
            xInputIndex++;
        }

        if (m_sJoysticks.at(i).m_lastDeviceHandle == 0x0)
        {
            // slot is "free"
            if (jstkImpl.m_useXInput)
            {
                jstkImpl.m_xInputIndex = xInputIndex;
            }
            m_sJoysticks.at(i) = jstkImpl;
            break;
        }
    }

    if (fileHandle)
    {
        CloseHandle(fileHandle);
    }
}

Joystick::Axis JoystickImpl::GetAxis(int index)
{
    switch (index)
    {
        case 0:
            return Joystick::Axis::X;
        case 1:
            return Joystick::Axis::Y;
        case 2:
            return Joystick::Axis::Z;
        case 3:
            return Joystick::Axis::R;
        case 4:
            return Joystick::Axis::U;
        case 5:
            return Joystick::Axis::V;
        case 6:
            return Joystick::Axis::PovX;
        case 7:
            return Joystick::Axis::PovY;
        default:
            // eh, good enough
            return Joystick::Axis::X;
    }
}

bool JoystickImpl::ExtractVidPid(const std::wstring devicePath, USHORT& vid, USHORT& pid, bool& isXInput)
{
    // Find VID
    size_t vidPos = devicePath.find(L"VID_");
    if (vidPos == std::wstring::npos)
        return false;
    vidPos += 4;
    if (vidPos + 4 > devicePath.length())
        return false;
    std::wstring vidStr = devicePath.substr(vidPos, 4);

    // Find PID
    size_t pidPos = devicePath.find(L"PID_");
    if (pidPos == std::wstring::npos)
        return false;
    pidPos += 4;
    if (pidPos + 4 > devicePath.length())
        return false;
    std::wstring pidStr = devicePath.substr(pidPos, 4);

    // Check for XInput (IG_)
    if (devicePath.find(L"IG_") != std::wstring::npos)
    {
        isXInput = true;
    }

    // Convert hex strings to USHORT without exceptions
    WCHAR* endptr;
    errno                 = 0; // Reset errno
    unsigned long vidLong = wcstoul(vidStr.c_str(), &endptr, 16);
    if (errno != 0 || *endptr != L'\0' || vidLong > 0xFFFF)
    {
        sf::String vidStrSfml = {vidStr};
        err() << "Failed to parse VID: " << vidStrSfml.toUtf8().data() << std::endl;
        return false;
    }

    errno                 = 0;
    unsigned long pidLong = wcstoul(pidStr.c_str(), &endptr, 16);
    if (errno != 0 || *endptr != L'\0' || pidLong > 0xFFFF)
    {
        sf::String pidStrAnsi = {vidStr};
        err() << "Failed to parse PID: " << pidStrAnsi.toUtf8().data() << std::endl;
        return false;
    }

    vid = static_cast<USHORT>(vidLong);
    pid = static_cast<USHORT>(pidLong);
    return true;
}

void JoystickImpl::DispatchDeviceRemoved(HANDLE deviceHandle)
{
    for (auto index = 0; index < m_sJoysticks.size(); index++)
    {
        JoystickImpl& joystick = m_sJoysticks.at(index);
        if (joystick.m_lastDeviceHandle == deviceHandle)
        {
            joystick.m_state            = {0};
            joystick.m_lastDeviceHandle = 0x0;
            joystick.m_caps             = {0};
            break; // exit loop after found the right one.
        }
    }
}

void JoystickImpl::DispatchRawInput(HRAWINPUT inputDevice)
{
    HRESULT hresult    = {0};
    UINT    bufferSize = sizeof(RAWINPUT);
    UINT    uiResult   = {0};
    // a bigger stack-alloc, but since we spawned a separate thread for this we have plenty of stack memory
    RAWINPUT  rawInput  = {0};
    HIDP_CAPS hCaps     = {0};
    DWORD     lastError = 0x0;

    // It's nice being able to allocate an entire RAWINPUT on the stack, apparently RawInput has been around so long that folk used to malloc for it. 
    uiResult = GetRawInputData(inputDevice, RID_INPUT, &rawInput, &bufferSize, sizeof(RAWINPUTHEADER));
    if (uiResult == UINT_MAX || uiResult != bufferSize)
    {
        // that didn't work!
        lastError = GetLastError();
        err() << "GetRawInputData returned [" << uiResult << "] unexpectedly! GetLastError: [" << lastError << "]"
              << std::endl;
        return;
    }

    // Proceed only if this is a HID device.
    if (rawInput.header.dwType == RIM_TYPEHID)
    {
        // this is a HID device. Btw, HID stands for Human Interface Device. So it's a Human Interface Device Device.
        const HANDLE deviceHandle = rawInput.header.hDevice;

        JoystickImpl* jstl = nullptr;

        for (size_t deviceIndex = 0; deviceIndex < Joystick::Count; deviceIndex++)
        {
            JoystickImpl& thisJstk = m_sJoysticks.at(deviceIndex);
            if (thisJstk.m_lastDeviceHandle == deviceHandle)
            {
                jstl = &m_sJoysticks.at(deviceIndex);
                break;
            }
        }
        if (jstl == nullptr)
        {
            return;
        }

        JoystickImpl& targetJoystick = *jstl;

        if (targetJoystick.m_useXInput)
        {
            // we're getting the information from another API, return.
            return;
        }

        uiResult = GetRawInputDeviceInfoW(deviceHandle, RIDI_PREPARSEDDATA, NULL, &bufferSize);
        if (uiResult == UINT_MAX)
        {
            // that didn't work!
            lastError = GetLastError();
            err() << "GetRawInputDeviceInfoW returned [" << uiResult << "] unexpectedly! GetLastError: [" << lastError
                  << "]" << std::endl;
            return;
        }

        uiResult = GetRawInputDeviceInfoW(deviceHandle, RIDI_PREPARSEDDATA, preparsedDataChunk, &bufferSize);
        if (uiResult == UINT_MAX)
        {
            // that didn't work!
            lastError = GetLastError();
            err() << "GetRawInputDeviceInfoW returned [" << uiResult << "] unexpectedly! GetLastError: [" << lastError
                  << "]" << std::endl;
            return;
        }

        const PHIDP_PREPARSED_DATA preparsedData = (PHIDP_PREPARSED_DATA)preparsedDataChunk;

        if (FAILED(hresult = HidP_GetCaps(preparsedData, &hCaps)))
        {
            // that didn't work!
            lastError = GetLastError();
            err() << "GetRawInputDeviceInfoW returned [" << hresult << "] unexpectedly! GetLastError: [" << lastError
                  << "]" << std::endl;
            return;
        }

        uint16_t         capsLength  = hCaps.NumberInputButtonCaps;
        HIDP_BUTTON_CAPS hButtonCaps = {0};
        if (FAILED(hresult = HidP_GetButtonCaps(HidP_Input, &hButtonCaps, &capsLength, preparsedData)))
        {
            // that didn't work!
            lastError = GetLastError();
            err() << "HidP_GetButtonCaps returned [" << hresult << "] unexpectedly! GetLastError: [" << lastError << "]"
                  << std::endl;
            return;
        }

        ULONG buttonCount = hButtonCaps.Range.UsageMax - hButtonCaps.Range.UsageMin + 1u;

        if (FAILED(hresult = HidP_GetUsages(HidP_Input,
                                            hButtonCaps.UsagePage,
                                            0,
                                            usageSizeDataChunk,
                                            &buttonCount,
                                            preparsedData,
                                            (PCHAR)rawInput.data.hid.bRawData,
                                            rawInput.data.hid.dwSizeHid)))
        {
            // that didn't work!
            lastError = GetLastError();
            err() << "HidP_GetUsages returned [" << hresult << "] unexpectedly! GetLastError: [" << lastError << "]"
                  << std::endl;
            return;
        }

        if (buttonCount == 0)
        {
            // INFO: No buttons are down.

            for (size_t buttonIndex = 0; buttonIndex < Joystick::ButtonCount; buttonIndex++)
            {
                targetJoystick.m_state.buttons[buttonIndex] = false;
            }
        }
        else
        {
            size_t rawInputButtonIndex = 0;
            for (size_t buttonIndex = 0; buttonIndex < Joystick::ButtonCount; buttonIndex++)
            {
                auto rawInputButton = usageSizeDataChunk[rawInputButtonIndex] - hButtonCaps.Range.UsageMin;
                if (rawInputButton == buttonIndex)
                {
                    targetJoystick.m_state.buttons[buttonIndex] = true;
                    rawInputButtonIndex++;
                }
                else
                {
                    targetJoystick.m_state.buttons[buttonIndex] = false;
                }
            }
        }

        PHIDP_VALUE_CAPS pValueCaps = reinterpret_cast<PHIDP_VALUE_CAPS>(valueCapsDataChunk);
        if (FAILED(HidP_GetValueCaps(HidP_Input, pValueCaps, &hCaps.NumberInputValueCaps, preparsedData)))
        {
            // that didn't work!
            lastError = GetLastError();
            err() << "HidP_GetValueCaps returned [" << hresult << "] unexpectedly! GetLastError: [" << lastError << "]"
                  << std::endl;
            return;
        }

        for (int i = 0; i < hCaps.NumberInputValueCaps; i++)
        {
            ULONG rawValue = {0};
            if (FAILED(HidP_GetUsageValue(HidP_Input,
                                          pValueCaps[i].UsagePage,
                                          0,
                                          pValueCaps[i].Range.UsageMin,
                                          &rawValue,
                                          preparsedData,
                                          reinterpret_cast<PCHAR>(rawInput.data.hid.bRawData),
                                          rawInput.data.hid.dwSizeHid)))
            {
                // that didn't work!
                lastError = GetLastError();
                err() << "HidP_GetUsageValue returned [" << hresult << "] unexpectedly! GetLastError: [" << lastError
                      << "]" << std::endl;
                return;
            }

            auto bitSize = pValueCaps[i].BitSize;

            // funky-wunky bitSize 
            ULONG expectedMax = (1ULL << bitSize) - 1;

            // looks weird, but the BitSize tells us how many bits they're actually using and which ones to ignore 
            LONG logicalMax = expectedMax & pValueCaps[i].LogicalMax;
            LONG logicalMin = expectedMax & pValueCaps[i].LogicalMin;

            float min = (float)logicalMin;
            float max = (float)logicalMax;

            float outputValue = 0.0f;

            // TODO: Remove this comment when you've finished testing the RawInput axes of non-xinput controllers. 
            // err() << "[" << i << "] BitSize: [" << pValueCaps[i].BitSize << "] Min: [" << min << "] Max: [" << max << "] Value: [" << rawValue << "]" << std::endl;

            // Map value from [min, max] to [-100, 100]
            if (max != min)
            { // Avoid division by zero
                outputValue = -100.0f + (static_cast<float>(rawValue) - min) * 200.0f / (max - min);
            }

            targetJoystick.m_state.axes[GetAxis(i)] = outputValue;
        }
    }
}

void JoystickImpl::DispatchXInput()
{

    for (DWORD xinputIndex = 0; xinputIndex < XINPUT_MAX_CONTROLLERS; xinputIndex++)
    {
        XINPUT_STATE xinputState  = {0};
        auto xinputResult    = XInputGetState(xinputIndex, &xinputState);
        if (xinputResult != 0x0)
        {
            // Probably not connected, just move on.
            continue;
        }

        for (DWORD joystickIndex = 0; joystickIndex < m_sJoysticks.size(); joystickIndex++)
        {
            JoystickImpl& joystick = m_sJoysticks.at(joystickIndex);
            if (joystick.m_xInputIndex == xinputIndex)
            {
                if (joystick.m_xInputPacketNumber != xinputState.dwPacketNumber)
                {
                    joystick.m_xInputPacketNumber = xinputState.dwPacketNumber;

                    auto& state       = joystick.m_state;
                    auto& gamepad     = xinputState.Gamepad;
                    state.buttons[0]  = !!(gamepad.wButtons & XINPUT_GAMEPAD_A);
                    state.buttons[1]  = !!(gamepad.wButtons & XINPUT_GAMEPAD_B);
                    state.buttons[2]  = !!(gamepad.wButtons & XINPUT_GAMEPAD_X);
                    state.buttons[3]  = !!(gamepad.wButtons & XINPUT_GAMEPAD_Y);
                    state.buttons[4]  = !!(gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
                    state.buttons[5]  = !!(gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
                    state.buttons[6]  = !!(gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
                    state.buttons[7]  = !!(gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
                    state.buttons[8]  = !!(gamepad.wButtons & XINPUT_GAMEPAD_START);
                    state.buttons[9]  = !!(gamepad.wButtons & XINPUT_GAMEPAD_BACK);
                    state.buttons[10] = !!(gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
                    state.buttons[11] = !!(gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
                    state.buttons[12] = !!(gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
                    state.buttons[13] = !!(gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);

                    SHORT deadzone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / 4;

                    state.axes[Joystick::Axis::X] = (abs(gamepad.sThumbLX) < deadzone) ? 0.0f : gamepad.sThumbLX / 327.670f;
                    state.axes[Joystick::Axis::Y] = (abs(gamepad.sThumbLY) < deadzone) ? 0.0f : gamepad.sThumbLY / 327.670f;
                    state.axes[Joystick::Axis::Z] = (abs(gamepad.sThumbRX) < deadzone) ? 0.0f : gamepad.sThumbRX / 327.670f;
                    state.axes[Joystick::Axis::R] = (abs(gamepad.sThumbRY) < deadzone) ? 0.0f : gamepad.sThumbRY / 327.670f;
                    state.axes[Joystick::Axis::U] = gamepad.bLeftTrigger / 2.550f;
                    state.axes[Joystick::Axis::V] = gamepad.bRightTrigger / 2.55f;
                }
                else
                {
                    // INFO: The state of the controller has not changed at all since last poll.
                }

                // break out of inner loop, not all for-loops.
                break;
            }
        }
    }
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

    WNDCLASSEXW wndClass   = {0};
    wndClass.cbSize        = sizeof(WNDCLASSEXW);
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
    // we'll be churning through data routinely, so it's simply more efficient to allocate a chunk once and reuse it rather than mallocing over and over again.
    // 2.5kB-ish in total heap memory is more than enough to hold the longest name and largest joystick imaginable.
    preparsedDataChunk       = malloc(SFML_RAW_INPUT_CHUNK_SIZE);
    valueCapsDataChunk       = malloc(SFML_RAW_INPUT_CHUNK_SIZE);
    deviceNameDataChunk      = malloc(SFML_RAW_INPUT_CHUNK_SIZE);
    usageSizeDataChunk       = (uint16_t*)malloc(SFML_RAW_INPUT_CHUNK_SIZE);
    deviceHumanNameDataChunk = malloc(SFML_RAW_INPUT_CHUNK_SIZE);

    for (size_t index = 0; index < Joystick::Count; index++)
    {
        JoystickImpl joystick      = {};
        joystick.m_state.connected = false;
        joystick.m_index           = (uint32_t)index;
        m_sJoysticks.push_back(joystick);
    }

    m_joystickMsgAtom = RegisterWindowMessageW(L"SFML-3-Joystick-Dispatch-Message");
    DWORD threadId;
    auto  threadHandle = CreateThread(NULL, 0, Win32JoystickDispatchThread, nullptr, 0, &threadId);
    if (threadHandle == NULL)
    {
        auto errorCode = GetLastError();
        err() << "CreateThread returned 0, GetLastError: [" << errorCode << "] Win32 Joystick will not function.";
    }
}

UINT JoystickImpl::GetJoystickMsgAtom()
{
    return m_sJoystickAtom;
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
        return m_sJoysticks.at(index).m_state.connected;
    }
    return false;
}

////////////////////////////////////////////////////////////
bool JoystickImpl::open(unsigned int index)
{
    if (index < m_sJoysticks.size())
    {
        return m_sJoysticks.at(index).m_state.connected;
    }
    return false;
}


////////////////////////////////////////////////////////////
void JoystickImpl::close()
{
    // stub, nothing to do here, everything is done automatically
}

////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilities() const
{
    return m_sJoysticks.at(m_index).m_caps;
}


////////////////////////////////////////////////////////////
Joystick::Identification JoystickImpl::getIdentification() const
{
    return m_sJoysticks.at(m_index).m_identification;
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::update()
{
    return m_sJoysticks.at(m_index).m_state;
}

LRESULT JoystickImpl::Win32JoystickWndProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_INPUT_DEVICE_CHANGE:
        {
            switch (wParam)
            {
                case GIDC_ARRIVAL:
                {
                    // INFO: Device Added
                    DispatchDeviceConnected(reinterpret_cast<HANDLE>(lParam));
                    break;
                }
                case GIDC_REMOVAL:
                {
                    // INFO: Device Removed
                    DispatchDeviceRemoved(reinterpret_cast<HANDLE>(lParam));
                    break;
                }
            }
            break;
        }
        case WM_INPUT:
        {
            DispatchRawInput((HRAWINPUT)lParam);
            break;
        }
        case WM_TIMER:
        {
            DispatchXInput();
            break;
        }
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

} // namespace sf::priv

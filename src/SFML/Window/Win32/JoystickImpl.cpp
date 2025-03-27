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
#include <cstddef>

// Joystick-specific includes
#include <Xinput.h>
#include <hidsdi.h>
#include <hidusage.h>

#include <combaseapi.h>

namespace
{
std::vector<sf::priv::JoystickImpl> joysticks;
ATOM                                joystickAtom{};
HWND                                joystickHwnd{};
UINT_PTR                            timerHandle{};
UINT                                joystickMsgAtom{};

// Raw Input Chunks
constexpr auto             rawInputChunkSize = 512;
std::vector<std::byte>     preparsedDataChunk;
std::vector<std::byte>     valueCapsDataChunk;
std::vector<std::byte>     deviceNameDataChunk;
std::vector<std::uint16_t> usageSizeDataChunk;
std::vector<std::byte>     deviceHumanNameDataChunk;
} // namespace

namespace sf::priv
{
////////////////////////////////////////////////////////////
void JoystickImpl::DispatchDeviceConnected(HANDLE deviceHandle)
{
    // TODO: Device identity discovery

    JoystickImpl jstkImpl{};
    jstkImpl.m_lastDeviceHandle = deviceHandle;
    jstkImpl.m_index            = static_cast<std::uint32_t>(joysticks.size());
    jstkImpl.m_state.connected  = true;

    UINT nameSize{};
    // okay this looks weird, but the docs say to call it twice like this
    GetRawInputDeviceInfoW(deviceHandle, RIDI_DEVICENAME, nullptr, &nameSize);
    GetRawInputDeviceInfoW(deviceHandle, RIDI_DEVICENAME, deviceNameDataChunk.data(), &nameSize);
    std::wstring devicePath(reinterpret_cast<LPCWSTR>(deviceNameDataChunk.data()));
    auto*        fileHandle = CreateFileW(devicePath.data(),
                                   GENERIC_READ | GENERIC_WRITE,
                                   FILE_SHARE_READ | FILE_SHARE_WRITE,
                                   nullptr,
                                   OPEN_EXISTING,
                                   0x0,
                                   nullptr);
    if (HidD_GetProductString(fileHandle, deviceHumanNameDataChunk.data(), rawInputChunkSize))
    {
        jstkImpl.m_identification.name = String{reinterpret_cast<wchar_t*>(deviceHumanNameDataChunk.data())};
        USHORT vid{};
        USHORT pid{};
        bool   isXInput = false;
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
        constexpr auto axes         = 6;
        for (std::size_t i = 0; i < Joystick::AxisCount && i < axes; ++i)
            jstkImpl.m_caps.axes[GetAxis(static_cast<int>(i))] = true;
    }
    else
    {
        // RawInput query capabilities
        UINT buffSize{};
        if (const auto result = GetRawInputDeviceInfoW(deviceHandle, RIDI_PREPARSEDDATA, nullptr, &buffSize);
            FAILED(result))
        {
            // that didn't work!
            auto lastError = GetLastError();
            err() << "GetRawInputData returned [" << result << "] unexpectedly! GetLastError: [" << lastError << "]"
                  << std::endl;
            return;
        }
        if (const auto result = GetRawInputDeviceInfoW(deviceHandle, RIDI_PREPARSEDDATA, preparsedDataChunk.data(), &buffSize);
            FAILED(result))
        {
            // that didn't work!
            auto lastError = GetLastError();
            err() << "GetRawInputData returned [" << result << "] unexpectedly! GetLastError: [" << lastError << "]"
                  << std::endl;
            return;
        }
        auto*     preparsedData = reinterpret_cast<PHIDP_PREPARSED_DATA>(preparsedDataChunk.data());
        HIDP_CAPS hCaps{};
        if (const auto result = HidP_GetCaps(preparsedData, &hCaps); FAILED(result))
        {
            // that didn't work!
            auto lastError = GetLastError();
            err() << "HidP_GetCaps returned [" << result << "] unexpectedly! GetLastError: [" << lastError << "]"
                  << std::endl;
            return;
        }

        jstkImpl.m_caps.buttonCount = hCaps.NumberInputButtonCaps;
        auto axes                   = hCaps.NumberInputValueCaps;
        for (std::size_t i = 0; i < Joystick::AxisCount && i < axes; ++i)
            jstkImpl.m_caps.axes[GetAxis(static_cast<int>(i))] = true;
    }

    unsigned int xInputIndex = 0;
    for (auto& joystick : joysticks)
    {
        if (joystick.m_xInputIndex != -1)
            ++xInputIndex;

        if (joystick.m_lastDeviceHandle == nullptr)
        {
            // slot is "free"
            if (jstkImpl.m_useXInput)
                jstkImpl.m_xInputIndex = xInputIndex;

            joystick = jstkImpl;
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


////////////////////////////////////////////////////////////
bool JoystickImpl::ExtractVidPid(const std::wstring& devicePath, USHORT& vid, USHORT& pid, bool& isXInput)
{
    // Find VID
    std::size_t vidPos = devicePath.find(L"VID_");
    if (vidPos == std::wstring::npos)
        return false;
    vidPos += 4;
    if (vidPos + 4 > devicePath.length())
        return false;
    const std::wstring vidStr = devicePath.substr(vidPos, 4);

    // Find PID
    std::size_t pidPos = devicePath.find(L"PID_");
    if (pidPos == std::wstring::npos)
        return false;
    pidPos += 4;
    if (pidPos + 4 > devicePath.length())
        return false;
    const std::wstring pidStr = devicePath.substr(pidPos, 4);

    // Check for XInput (IG_)
    if (devicePath.find(L"IG_") != std::wstring::npos)
    {
        isXInput = true;
    }

    // Convert hex strings to USHORT without exceptions
    WCHAR* endptr               = nullptr;
    errno                       = 0; // Reset errno
    const unsigned long vidLong = wcstoul(vidStr.c_str(), &endptr, 16);
    if (errno != 0 || *endptr != L'\0' || vidLong > 0xFFFF)
    {
        const sf::String vidStrSfml = {vidStr};
        err() << "Failed to parse VID: " << vidStrSfml.toUtf8().data() << std::endl;
        return false;
    }

    errno                       = 0;
    const unsigned long pidLong = wcstoul(pidStr.c_str(), &endptr, 16);
    if (errno != 0 || *endptr != L'\0' || pidLong > 0xFFFF)
    {
        const sf::String pidStrAnsi = {vidStr};
        err() << "Failed to parse PID: " << pidStrAnsi.toUtf8().data() << std::endl;
        return false;
    }

    vid = static_cast<USHORT>(vidLong);
    pid = static_cast<USHORT>(pidLong);
    return true;
}


////////////////////////////////////////////////////////////
void JoystickImpl::DispatchDeviceRemoved(HANDLE deviceHandle)
{
    for (auto& joystick : joysticks)
    {
        if (joystick.m_lastDeviceHandle == deviceHandle)
        {
            joystick.m_state            = {};
            joystick.m_lastDeviceHandle = nullptr;
            joystick.m_caps             = {};
            break; // exit loop after found the right one.
        }
    }
}


////////////////////////////////////////////////////////////
void JoystickImpl::DispatchRawInput(HRAWINPUT inputDevice)
{
    HRESULT hresult{};
    UINT    bufferSize = sizeof(RAWINPUT);
    UINT    uiResult{};
    // a bigger stack-alloc, but since we spawned a separate thread for this we have plenty of stack memory
    RAWINPUT  rawInput{};
    HIDP_CAPS hCaps{};
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

        for (std::size_t deviceIndex = 0; deviceIndex < Joystick::Count; ++deviceIndex)
        {
            const JoystickImpl& thisJstk = joysticks.at(deviceIndex);
            if (thisJstk.m_lastDeviceHandle == deviceHandle)
            {
                jstl = &joysticks.at(deviceIndex);
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

        uiResult = GetRawInputDeviceInfoW(deviceHandle, RIDI_PREPARSEDDATA, nullptr, &bufferSize);
        if (uiResult == UINT_MAX)
        {
            // that didn't work!
            lastError = GetLastError();
            err() << "GetRawInputDeviceInfoW returned [" << uiResult << "] unexpectedly! GetLastError: [" << lastError
                  << "]" << std::endl;
            return;
        }

        uiResult = GetRawInputDeviceInfoW(deviceHandle, RIDI_PREPARSEDDATA, preparsedDataChunk.data(), &bufferSize);
        if (uiResult == UINT_MAX)
        {
            // that didn't work!
            lastError = GetLastError();
            err() << "GetRawInputDeviceInfoW returned [" << uiResult << "] unexpectedly! GetLastError: [" << lastError
                  << "]" << std::endl;
            return;
        }

        auto* preparsedData = reinterpret_cast<PHIDP_PREPARSED_DATA>(preparsedDataChunk.data());

        if (FAILED(hresult = HidP_GetCaps(preparsedData, &hCaps)))
        {
            // that didn't work!
            lastError = GetLastError();
            err() << "GetRawInputDeviceInfoW returned [" << hresult << "] unexpectedly! GetLastError: [" << lastError
                  << "]" << std::endl;
            return;
        }

        std::uint16_t    capsLength = hCaps.NumberInputButtonCaps;
        HIDP_BUTTON_CAPS hButtonCaps{};
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
                                            usageSizeDataChunk.data(),
                                            &buttonCount,
                                            preparsedData,
                                            reinterpret_cast<PCHAR>(rawInput.data.hid.bRawData),
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

            for (std::size_t buttonIndex = 0; buttonIndex < Joystick::ButtonCount; ++buttonIndex)
            {
                targetJoystick.m_state.buttons[buttonIndex] = false;
            }
        }
        else
        {
            std::size_t rawInputButtonIndex = 0;
            for (std::size_t buttonIndex = 0; buttonIndex < Joystick::ButtonCount; ++buttonIndex)
            {
                auto rawInputButton = usageSizeDataChunk[rawInputButtonIndex] - hButtonCaps.Range.UsageMin;
                if (static_cast<std::size_t>(rawInputButton) == buttonIndex)
                {
                    targetJoystick.m_state.buttons[buttonIndex] = true;
                    ++rawInputButtonIndex;
                }
                else
                {
                    targetJoystick.m_state.buttons[buttonIndex] = false;
                }
            }
        }

        auto* pValueCaps = reinterpret_cast<PHIDP_VALUE_CAPS>(valueCapsDataChunk.data());
        if (FAILED(HidP_GetValueCaps(HidP_Input, pValueCaps, &hCaps.NumberInputValueCaps, preparsedData)))
        {
            // that didn't work!
            lastError = GetLastError();
            err() << "HidP_GetValueCaps returned [" << hresult << "] unexpectedly! GetLastError: [" << lastError << "]"
                  << std::endl;
            return;
        }

        for (int i = 0; i < hCaps.NumberInputValueCaps; ++i)
        {
            ULONG rawValue{};
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

            const auto bitSize = pValueCaps[i].BitSize;

            // funky-wunky bitSize
            const ULONG expectedMax = (1ULL << bitSize) - 1;

            // looks weird, but the BitSize tells us how many bits they're actually using and which ones to ignore
            const LONG logicalMax = expectedMax & pValueCaps[i].LogicalMax;
            const LONG logicalMin = expectedMax & pValueCaps[i].LogicalMin;

            const auto min = static_cast<float>(logicalMin);
            const auto max = static_cast<float>(logicalMax);

            float outputValue = 0.0f;

            // TODO: Remove this comment when you've finished testing the RawInput axes of non-xinput controllers.
            // err() << "[" << i << "] BitSize: [" << pValueCaps[i].BitSize << "] Min: [" << min << "] Max: [" << max << "] Value: [" << rawValue << "]" << std::endl;

            // Map value from [min, max] to [-100, 100]
            // Avoid division by zero
            if (max != min)
                outputValue = -100.0f + (static_cast<float>(rawValue) - min) * 200.0f / (max - min);

            targetJoystick.m_state.axes[GetAxis(i)] = outputValue;
        }
    }
}


////////////////////////////////////////////////////////////
void JoystickImpl::DispatchXInput()
{
    static constexpr auto xinputMaxControllers = 4u;
    for (DWORD xinputIndex = 0; xinputIndex < xinputMaxControllers; ++xinputIndex)
    {
        XINPUT_STATE xinputState{};
        auto         xinputResult = XInputGetState(xinputIndex, &xinputState);
        if (xinputResult != 0x0)
        {
            // Probably not connected, just move on.
            continue;
        }

        for (auto& joystick : joysticks)
        {
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

                    const SHORT deadzone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / 4;

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


////////////////////////////////////////////////////////////
DWORD WINAPI JoystickImpl::Win32JoystickDispatchThread(LPVOID /* lpParam */)
{
    // Required
    auto coInitResult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (!SUCCEEDED(coInitResult))
    {
        // :(
        auto errorCode = GetLastError();
        err() << "CoInitializeEx returned 0, GetLastError: [" << errorCode << "], Win32 Joystick will not function."
              << std::endl;
        return errorCode;
    }

    WNDCLASSEXW wndClass{};
    wndClass.cbSize        = sizeof(WNDCLASSEXW);
    wndClass.hInstance     = GetModuleHandleW(nullptr);
    wndClass.lpfnWndProc   = reinterpret_cast<WNDPROC>(Win32JoystickWndProc);
    wndClass.lpszClassName = L"SFML-Win32JoystickWndProc";

    joystickAtom = RegisterClassExW(&wndClass);
    if (joystickAtom == 0x0)
    {
        // :(
        auto errorCode = GetLastError();
        err() << "RegisterClassExW returned 0, GetLastError: [" << errorCode << "], Win32 Joystick will not function."
              << std::endl;
        return errorCode;
    }

    joystickHwnd = CreateWindowExW(0,
                                   // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
                                   // says it can be an ATOM OR the name of the window class. Atom is used here.
                                   reinterpret_cast<LPCWSTR>(joystickAtom),
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
    if (joystickHwnd == nullptr)
    {
        // :(
        auto errorCode = GetLastError();
        err() << "CreateWindowExW returned 0, GetLastError: [" << errorCode << "] Win32 Joystick will not function.";
        return errorCode;
    }

    // stack-allocation
    std::array<RAWINPUTDEVICE, 3> rids{};
    ZeroMemory(rids.data(), sizeof(rids));
    for (auto& rid : rids)
    {
        // We are using this, talking to "generic" input devices.
        rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
        // We want to receive inputs even out-of-focus, and notify us when devices are added or removed.
        rid.dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
        // And send notifications to THIS specific HWND, which we registered earlier.
        rid.hwndTarget = joystickHwnd;
    }

    rids[0].usUsage = HID_USAGE_GENERIC_GAMEPAD;
    rids[1].usUsage = HID_USAGE_GENERIC_JOYSTICK;
    rids[2].usUsage = HID_USAGE_GENERIC_MULTI_AXIS_CONTROLLER;

    auto rawInputRetCode = RegisterRawInputDevices(rids.data(), static_cast<UINT>(rids.size()), sizeof(RAWINPUTDEVICE));
    if (rawInputRetCode == 0x0)
    {
        // :(
        auto errorCode = GetLastError();
        err() << "RegisterRawInputDevices returned 0, GetLastError: [" << errorCode
              << "] Win32 Joystick will not function.";
        return errorCode;
    }

    timerHandle = SetTimer(joystickHwnd, 0, 8, nullptr);

    MSG msg;

    while (GetMessageW(&msg, joystickHwnd, 0, 0) != 0x0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    // Cleanup code
    KillTimer(joystickHwnd, 0);
    return S_OK;
}


////////////////////////////////////////////////////////////
void JoystickImpl::initialize()
{
    // We'll be churning through data routinely, so it's simply more efficient to
    // allocate a chunk once and reuse it rather than allocating over and over again.
    // 2.5kB-ish in total heap memory is more than enough to hold the longest name
    // and largest joystick imaginable.
    preparsedDataChunk.resize(rawInputChunkSize);
    valueCapsDataChunk.resize(rawInputChunkSize);
    deviceNameDataChunk.resize(rawInputChunkSize);
    usageSizeDataChunk.resize(rawInputChunkSize);
    deviceHumanNameDataChunk.resize(rawInputChunkSize);

    for (std::size_t i = 0; i < Joystick::Count; ++i)
    {
        JoystickImpl joystick      = {};
        joystick.m_state.connected = false;
        joystick.m_index           = static_cast<std::uint32_t>(i);
        joysticks.push_back(joystick);
    }

    joystickMsgAtom          = RegisterWindowMessageW(L"SFML-3-Joystick-Dispatch-Message");
    DWORD       threadId     = 0;
    const auto* threadHandle = CreateThread(nullptr, 0, Win32JoystickDispatchThread, nullptr, 0, &threadId);
    if (threadHandle == nullptr)
        err() << "CreateThread returned 0, GetLastError: [" << GetLastError() << "] Win32 Joystick will not function.";
}


////////////////////////////////////////////////////////////
UINT JoystickImpl::GetJoystickMsgAtom()
{
    return joystickAtom;
}


////////////////////////////////////////////////////////////
void JoystickImpl::cleanup()
{
    CloseWindow(joystickHwnd);
}


////////////////////////////////////////////////////////////
bool JoystickImpl::isConnected(unsigned int index)
{
    if (index < joysticks.size())
        return joysticks.at(index).m_state.connected;
    return false;
}


////////////////////////////////////////////////////////////
bool JoystickImpl::open(unsigned int index)
{
    if (index < joysticks.size())
        return joysticks.at(index).m_state.connected;
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
    return joysticks.at(m_index).m_caps;
}


////////////////////////////////////////////////////////////
Joystick::Identification JoystickImpl::getIdentification() const
{
    return joysticks.at(m_index).m_identification;
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::update() const
{
    return joysticks.at(m_index).m_state;
}


////////////////////////////////////////////////////////////
LRESULT JoystickImpl::Win32JoystickWndProc(HWND hwnd, std::uint32_t msg, WPARAM wParam, LPARAM lParam)
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
            DispatchRawInput(reinterpret_cast<HRAWINPUT>(lParam));
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

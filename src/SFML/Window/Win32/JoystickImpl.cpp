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
#include <memory>
#include <optional>
#include <ostream>
#include <regstr.h>
#include <sstream>
#include <string>
#include <tchar.h>
#include <vector>

#include <cmath>

// Used for XInput matching
#include <Xinput.h>
#include <oleauto.h>
#include <wbemidl.h>

////////////////////////////////////////////////////////////
// DirectInput
////////////////////////////////////////////////////////////

#ifndef DIDFT_OPTIONAL
#define DIDFT_OPTIONAL 0x80000000
#endif

// Xinput header has these under some OS build version guards
#ifndef XINPUT_CAPS_WIRELESS
#define XINPUT_CAPS_WIRELESS 0x0002
#endif


namespace
{
namespace guids
{
// NOLINTBEGIN(readability-identifier-naming)
constexpr GUID IID_IDirectInput8W = {0xbf798031, 0x483a, 0x4da2, {0xaa, 0x99, 0x5d, 0x64, 0xed, 0x36, 0x97, 0x00}};

constexpr GUID GUID_XAxis  = {0xa36d02e0, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
constexpr GUID GUID_YAxis  = {0xa36d02e1, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
constexpr GUID GUID_ZAxis  = {0xa36d02e2, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
constexpr GUID GUID_RzAxis = {0xa36d02e3, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
constexpr GUID GUID_Slider = {0xa36d02e4, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};

constexpr GUID GUID_POV = {0xa36d02f2, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};

constexpr GUID GUID_RxAxis = {0xa36d02f4, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
constexpr GUID GUID_RyAxis = {0xa36d02f5, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
// NOLINTEND(readability-identifier-naming)
} // namespace guids

HMODULE         dinput8dll  = nullptr;
IDirectInput8W* directInput = nullptr;

struct JoystickRecord
{
    GUID         guid{};
    unsigned int index{};
    bool         plugged{};
    bool         xInputDevice{};
};

using JoystickList = std::vector<JoystickRecord>;
JoystickList joystickList;

struct JoystickBlacklistEntry
{
    unsigned int vendorId{};
    unsigned int productId{};
};

using JoystickBlacklist = std::vector<JoystickBlacklistEntry>;
JoystickBlacklist joystickBlacklist;

const DWORD directInputEventBufferSize = 32;

struct XInputJoystickEntry
{
    bool                    connected{};
    std::optional<DWORD>    xInputIndex{}; // Cannot be zero-means-null because 0 is a valid index
    sf::priv::JoystickImpl* joystick{};
    XINPUT_STATE            state{};
    unsigned int            joystickIndex{};
    WORD                    vendorId{};
    WORD                    productId{};
};

// This struct ordinarily would be defined in a Windows-header-file.
// See also: https://stackoverflow.com/a/68879988/4928207
// This is used to ensure that the device we're looking at matches the product and vendor id
// This struct is essentially "foreign" to SFML, so don't reorder its members. (ABI Compat)
struct XINPUT_CAPABILITIES_EX
{
    XINPUT_CAPABILITIES capabilities{};
    WORD                vendorId{};
    WORD                productId{};
    WORD                revisionId{};
    DWORD               a4{}; // Unknown
};

bool                               directInputNeedsInvalidation = false;
std::array<XInputJoystickEntry, 4> xInputDevices{};

struct BstrDeleter
{
    void operator()(BSTR bstr) const
    {
        if (bstr)
            SysFreeString(bstr);
    }
};

struct XInputCleanupData
{
    VARIANT                               var{};
    IWbemLocator*                         wbemLocator{};
    IEnumWbemClassObject*                 enumDevices{};
    std::array<IWbemClassObject*, 20>     devices{};
    IWbemServices*                        wbemServices{};
    std::unique_ptr<OLECHAR, BstrDeleter> namespaceStr;
    std::unique_ptr<OLECHAR, BstrDeleter> deviceId;
    std::unique_ptr<OLECHAR, BstrDeleter> className;
    HRESULT                               comInitResult{};

    XInputCleanupData()
    {
        VariantInit(&var);
    }

    ~XInputCleanupData()
    {
        VariantClear(&var);

        for (auto* device : devices)
            if (device)
                device->Release();

        if (enumDevices)
            enumDevices->Release();
        if (wbemLocator)
            wbemLocator->Release();
        if (wbemServices)
            wbemServices->Release();
        if (SUCCEEDED(comInitResult))
            CoUninitialize();
    }
};

// NOLINTBEGIN(readability-identifier-naming)
// Keeping GUIDs and UUIDs consistent
// Define CLSID_WbemLocator
const CLSID CLSID_WbemLocator = {0x4590f811, 0x1d3a, 0x11d0, {0x89, 0x1f, 0x00, 0xaa, 0x00, 0x4b, 0x2e, 0x24}};

// Define IID_IWbemLocator
const IID IID_IWbemLocator = {0xdc12a687, 0x737f, 0x11cf, {0x88, 0x4d, 0x00, 0xaa, 0x00, 0x4b, 0x2e, 0x24}};
// NOLINTEND(readability-identifier-naming)

// Function pointer type for XInputGetState
using XInputGetStateFunc = DWORD(WINAPI*)(DWORD dwUserIndex, XINPUT_STATE* pState);
// Function pointer type for XInputGetCapabilitiesEx
using XInputGetCapabilitiesExFunc =
    DWORD(WINAPI*)(DWORD a1, DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES_EX* pCapabilities);

XInputGetCapabilitiesExFunc xInputGetCapabilitiesEx = nullptr;
XInputGetStateFunc          xInputGetState          = nullptr;

[[nodiscard]] std::optional<DWORD> getXInputIndexFromVidPid(WORD vid, WORD pid) noexcept
{
    for (const DWORD xInputSlot : {0u, 1u, 2u, 3u})
    {
        XINPUT_CAPABILITIES_EX capsEx{};
        if (xInputGetCapabilitiesEx(1, xInputSlot, 0, &capsEx) == ERROR_SUCCESS)
        {
            if (capsEx.vendorId == 0x045e && capsEx.productId == 0 && capsEx.capabilities.Flags & XINPUT_CAPS_WIRELESS)
                capsEx.productId = 0x02a1;

            if (capsEx.vendorId == vid && capsEx.productId == pid)
            {
                auto& entry = xInputDevices[xInputSlot];
                // guard against multiple identical devices
                if (entry.joystick == nullptr)
                    return xInputSlot;
            }
        }
    }
    return std::nullopt;
}

// See also https://learn.microsoft.com/en-us/windows/win32/xinput/xinput-and-directinput?redirectedfrom=MSDN
[[nodiscard]] BOOL isXInputDevice(const GUID& guidProductFromDirectInput)
{
    XInputCleanupData data;
    data.comInitResult = CoInitialize(nullptr);
    if (FAILED(data.comInitResult))
        return false;

    // Create WMI
    auto hr = CoCreateInstance(CLSID_WbemLocator,
                               nullptr,
                               CLSCTX_INPROC_SERVER,
                               IID_IWbemLocator,
                               reinterpret_cast<LPVOID*>(&data.wbemLocator));
    if (FAILED(hr) || data.wbemLocator == nullptr)
        return false;

    data.namespaceStr.reset(SysAllocString(LR"(\\.\root\cimv2)"));
    if (data.namespaceStr == nullptr)
        return false;

    data.className.reset(SysAllocString(L"Win32_PNPEntity"));
    if (data.className == nullptr)
        return false;

    data.deviceId.reset(SysAllocString(L"DeviceID"));
    if (data.deviceId == nullptr)
        return false;

    // Connect to WMI
    hr = data.wbemLocator->ConnectServer(data.namespaceStr.get(), nullptr, nullptr, nullptr, 0, nullptr, nullptr, &data.wbemServices);
    if (FAILED(hr) || data.wbemServices == nullptr)
        return false;

    // Switch security level to IMPERSONATE.
    hr = CoSetProxyBlanket(data.wbemServices,
                           RPC_C_AUTHN_WINNT,
                           RPC_C_AUTHZ_NONE,
                           nullptr,
                           RPC_C_AUTHN_LEVEL_CALL,
                           RPC_C_IMP_LEVEL_IMPERSONATE,
                           nullptr,
                           EOAC_NONE);
    if (FAILED(hr))
        return false;

    hr = data.wbemServices->CreateInstanceEnum(data.className.get(), 0, nullptr, &data.enumDevices);
    if (FAILED(hr) || data.enumDevices == nullptr)
        return false;

    // Loop over all devices
    for (;;)
    {
        ULONG returned = 0;
        hr = data.enumDevices->Next(10'000, static_cast<ULONG>(data.devices.size()), data.devices.data(), &returned);
        if (FAILED(hr))
            return false;

        if (returned == 0)
            break;

        for (std::size_t i = 0; i < returned; ++i)
        {
            // For each device, get its device ID
            hr = data.devices[i]->Get(data.deviceId.get(), 0, &data.var, nullptr, nullptr);
            if (SUCCEEDED(hr) && data.var.vt == VT_BSTR && data.var.bstrVal != nullptr)
            {
                // Check if the device ID contains "IG_".  If it does, then it's an XInput device
                // This information cannot be found from DirectInput
                if (std::wcsstr(data.var.bstrVal, L"IG_"))
                {
                    // If it does, then get the VID/PID from var.bstrVal
                    WCHAR* strVid = std::wcsstr(data.var.bstrVal, L"VID_");
                    DWORD  dwVid  = 0;
                    if (strVid && swscanf_s(strVid, L"VID_%4X", &dwVid) != 1)
                        dwVid = 0;
                    WCHAR* strPid = std::wcsstr(data.var.bstrVal, L"PID_");
                    DWORD  dwPid  = 0;
                    if (strPid && swscanf_s(strPid, L"PID_%4X", &dwPid) != 1)
                        dwPid = 0;

                    // Compare the VID/PID to the DInput device
                    const auto dwVidPid = static_cast<DWORD>(MAKELONG(dwVid, dwPid));
                    if (dwVidPid == guidProductFromDirectInput.Data1)
                        return true;
                }
            }

            VariantClear(&data.var);
            if (auto* device = data.devices[i])
            {
                device->Release();
                // important to prevent a double-free in cleanup data destructor
                data.devices[i] = nullptr;
            }
        }
    }
    return false;
}


////////////////////////////////////////////////////////////
// Legacy joystick API
////////////////////////////////////////////////////////////
struct ConnectionCache
{
    bool      connected{};
    sf::Clock timer;
};

std::array<ConnectionCache, sf::Joystick::Count> connectionCache{};

// If true, will only update when WM_DEVICECHANGE message is received
bool lazyUpdates = false;

// Get the joystick's name
sf::String getDeviceName(unsigned int index, JOYCAPS caps)
{
    // Give the joystick a default name
    static const sf::String joystickDescription = "Unknown Joystick";

    LONG                     result     = 0;
    HKEY                     rootKey    = nullptr;
    HKEY                     currentKey = nullptr;
    std::basic_string<TCHAR> subkey;

    subkey = REGSTR_PATH_JOYCONFIG;
    subkey += TEXT('\\');
    subkey += caps.szRegKey;
    subkey += TEXT('\\');
    subkey += REGSTR_KEY_JOYCURR;

    rootKey = HKEY_CURRENT_USER;
    result  = RegOpenKeyEx(rootKey, subkey.c_str(), 0, KEY_READ, &currentKey);

    if (result != ERROR_SUCCESS)
    {
        rootKey = HKEY_LOCAL_MACHINE;
        result  = RegOpenKeyEx(rootKey, subkey.c_str(), 0, KEY_READ, &currentKey);

        if (result != ERROR_SUCCESS)
        {
            sf::err() << "Unable to open registry for joystick at index " << index << ": "
                      << sf::priv::getErrorString(static_cast<DWORD>(result)) << std::endl;
            return joystickDescription;
        }
    }

    std::basic_ostringstream<TCHAR> indexString;
    indexString << index + 1;

    subkey = TEXT("Joystick");
    subkey += indexString.str();
    subkey += REGSTR_VAL_JOYOEMNAME;

    std::array<TCHAR, 256> keyData{};
    DWORD                  keyDataSize = sizeof(keyData);

    result = RegQueryValueEx(currentKey, subkey.c_str(), nullptr, nullptr, reinterpret_cast<LPBYTE>(keyData.data()), &keyDataSize);
    RegCloseKey(currentKey);

    if (result != ERROR_SUCCESS)
    {
        sf::err() << "Unable to query registry key for joystick at index " << index << ": "
                  << sf::priv::getErrorString(static_cast<DWORD>(result)) << std::endl;
        return joystickDescription;
    }

    subkey = REGSTR_PATH_JOYOEM;
    subkey += TEXT('\\');
    subkey.append(keyData.data(), keyDataSize / sizeof(TCHAR));

    result = RegOpenKeyEx(rootKey, subkey.c_str(), 0, KEY_READ, &currentKey);

    if (result != ERROR_SUCCESS)
    {
        sf::err() << "Unable to open registry key for joystick at index " << index << ": "
                  << sf::priv::getErrorString(static_cast<DWORD>(result)) << std::endl;
        return joystickDescription;
    }

    keyDataSize = sizeof(keyData);

    result = RegQueryValueEx(currentKey, REGSTR_VAL_JOYOEMNAME, nullptr, nullptr, reinterpret_cast<LPBYTE>(keyData.data()), &keyDataSize);
    RegCloseKey(currentKey);

    if (result != ERROR_SUCCESS)
    {
        sf::err() << "Unable to query name for joystick at index " << index << ": "
                  << sf::priv::getErrorString(static_cast<DWORD>(result)) << std::endl;
        return joystickDescription;
    }

    keyData.back() = TEXT('\0'); // Ensure null terminator in case the data is too long.
    return keyData.data();
}
} // namespace

namespace sf::priv
{
////////////////////////////////////////////////////////////
void JoystickImpl::initialize()
{
    HMODULE xInputModule = LoadLibraryA("XInput1_4.dll");
    if (!xInputModule)
    {
        // this always succeeds.
        xInputModule            = LoadLibraryA("XINPUT9_1_0.DLL");
        xInputGetCapabilitiesEx = nullptr;
    }
    else
    {
        xInputGetCapabilitiesEx = reinterpret_cast<XInputGetCapabilitiesExFunc>(
            reinterpret_cast<void*>(GetProcAddress(xInputModule, reinterpret_cast<char*>(108))));
    }
    assert(xInputModule);
    xInputGetState = reinterpret_cast<XInputGetStateFunc>(
        reinterpret_cast<void*>(GetProcAddress(xInputModule, "XInputGetState")));

    // set the indexes of xinput buffered state correctly. 
    for (size_t xInputIndex = 0; xInputIndex < xInputDevices.size(); xInputIndex++)
        xInputDevices[xInputIndex].xInputIndex = static_cast<DWORD>(xInputIndex);

    // Try to initialize DirectInput
    initializeDInput();

    if (!directInput)
        err() << "DirectInput not available, falling back to Windows joystick API" << std::endl;

    // Perform the initial scan and populate the connection cache
    updateConnections();
}


////////////////////////////////////////////////////////////
void JoystickImpl::cleanup()
{
    // Clean up DirectInput
    cleanupDInput();
}


////////////////////////////////////////////////////////////
bool JoystickImpl::isConnected(unsigned int index)
{
    if (directInputNeedsInvalidation)
    {
        directInputNeedsInvalidation = false;
        updateConnections();
    }

    if (directInput)
        return isConnectedDInput(index);

    ConnectionCache& cache                  = connectionCache[index];
    constexpr Time   connectionRefreshDelay = milliseconds(500);
    if (!lazyUpdates && cache.timer.getElapsedTime() > connectionRefreshDelay)
    {
        JOYINFOEX joyInfo;
        joyInfo.dwSize  = sizeof(joyInfo);
        joyInfo.dwFlags = 0;
        cache.connected = joyGetPosEx(JOYSTICKID1 + index, &joyInfo) == JOYERR_NOERROR;

        cache.timer.restart();
    }
    return cache.connected;
}


////////////////////////////////////////////////////////////
void JoystickImpl::setLazyUpdates(bool status)
{
    lazyUpdates = status;
}


////////////////////////////////////////////////////////////
void JoystickImpl::invalidateDevices()
{
    directInputNeedsInvalidation = true;
}


////////////////////////////////////////////////////////////
void JoystickImpl::updateConnections()
{
    if (directInput)
    {
        updateConnectionsDInput();
        return;
    }

    for (unsigned int i = 0; i < Joystick::Count; ++i)
    {
        JOYINFOEX joyInfo;
        joyInfo.dwSize         = sizeof(joyInfo);
        joyInfo.dwFlags        = 0;
        ConnectionCache& cache = connectionCache[i];
        cache.connected        = joyGetPosEx(JOYSTICKID1 + i, &joyInfo) == JOYERR_NOERROR;

        cache.timer.restart();
    }
}


////////////////////////////////////////////////////////////
bool JoystickImpl::open(unsigned int index)
{
    if (openXInput(index))
    {
        m_useXInput = true;
        return true;
    }
    if (directInput)
    {
        auto result = openDInput(index);
        return result;
    }

    // No explicit "open" action is required
    m_index = JOYSTICKID1 + index;

    // Store the joystick capabilities
    const bool success = joyGetDevCaps(m_index, &m_caps, sizeof(m_caps)) == JOYERR_NOERROR;

    if (success)
    {
        m_identification.name      = getDeviceName(m_index, m_caps);
        m_identification.productId = m_caps.wPid;
        m_identification.vendorId  = m_caps.wMid;
    }

    return success;
}


////////////////////////////////////////////////////////////
void JoystickImpl::close()
{
    if (directInput)
        closeDInput();
    if (m_useXInput && m_xInputIndex.has_value())
        xInputDevices[m_xInputIndex.value()].joystick = nullptr;
    m_xInputIndex.reset();
    m_useXInput   = false;
}


////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilities() const
{
    if (m_useXInput)
    {
        // XInput has 10 Buttons (since we exclude the DPad) and all 8 Axes
        JoystickCaps caps{0};
        caps.buttonCount = 10;
        caps.axes.fill(true);
        return caps;
    }

    if (directInput)
        return getCapabilitiesDInput();

    JoystickCaps caps;

    caps.buttonCount = std::min(m_caps.wNumButtons, Joystick::ButtonCount);

    caps.axes[Joystick::Axis::X]    = true;
    caps.axes[Joystick::Axis::Y]    = true;
    caps.axes[Joystick::Axis::Z]    = (m_caps.wCaps & JOYCAPS_HASZ) != 0;
    caps.axes[Joystick::Axis::R]    = (m_caps.wCaps & JOYCAPS_HASR) != 0;
    caps.axes[Joystick::Axis::U]    = (m_caps.wCaps & JOYCAPS_HASU) != 0;
    caps.axes[Joystick::Axis::V]    = (m_caps.wCaps & JOYCAPS_HASV) != 0;
    caps.axes[Joystick::Axis::PovX] = (m_caps.wCaps & JOYCAPS_HASPOV) != 0;
    caps.axes[Joystick::Axis::PovY] = (m_caps.wCaps & JOYCAPS_HASPOV) != 0;

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
    if (directInputNeedsInvalidation)
    {
        directInputNeedsInvalidation = false;
        updateConnections();
    }

    // XInput state is buffered because XInput is a polling protocol (125Hz)
    if (m_useXInput && m_xInputIndex.has_value())
    {
        auto device = xInputDevices[m_xInputIndex.value()];
        if (!device.connected)
        {
            // empty (disconnected) state. 
            return JoystickState{};
        }
        return updateXInput(device.state);
    }

    if (directInput)
    {
        if (m_buffered)
        {
            return updateDInputBuffered();
        }

        return updateDInputPolled();
    }

    JoystickState state;

    // Get the current joystick state
    JOYINFOEX pos;
    pos.dwFlags = JOY_RETURNX | JOY_RETURNY | JOY_RETURNZ | JOY_RETURNR | JOY_RETURNU | JOY_RETURNV | JOY_RETURNBUTTONS;
    pos.dwFlags |= (m_caps.wCaps & JOYCAPS_POVCTS) ? JOY_RETURNPOVCTS : JOY_RETURNPOV;
    pos.dwSize = sizeof(JOYINFOEX);
    if (joyGetPosEx(m_index, &pos) == JOYERR_NOERROR)
    {
        // The joystick is connected
        state.connected = true;

        // Axes
        state.axes[Joystick::Axis::X] = (static_cast<float>(pos.dwXpos) -
                                         static_cast<float>(m_caps.wXmax + m_caps.wXmin) / 2.f) *
                                        200.f / static_cast<float>(m_caps.wXmax - m_caps.wXmin);
        state.axes[Joystick::Axis::Y] = (static_cast<float>(pos.dwYpos) -
                                         static_cast<float>(m_caps.wYmax + m_caps.wYmin) / 2.f) *
                                        200.f / static_cast<float>(m_caps.wYmax - m_caps.wYmin);
        state.axes[Joystick::Axis::Z] = (static_cast<float>(pos.dwZpos) -
                                         static_cast<float>(m_caps.wZmax + m_caps.wZmin) / 2.f) *
                                        200.f / static_cast<float>(m_caps.wZmax - m_caps.wZmin);
        state.axes[Joystick::Axis::R] = (static_cast<float>(pos.dwRpos) -
                                         static_cast<float>(m_caps.wRmax + m_caps.wRmin) / 2.f) *
                                        200.f / static_cast<float>(m_caps.wRmax - m_caps.wRmin);
        state.axes[Joystick::Axis::U] = (static_cast<float>(pos.dwUpos) -
                                         static_cast<float>(m_caps.wUmax + m_caps.wUmin) / 2.f) *
                                        200.f / static_cast<float>(m_caps.wUmax - m_caps.wUmin);
        state.axes[Joystick::Axis::V] = (static_cast<float>(pos.dwVpos) -
                                         static_cast<float>(m_caps.wVmax + m_caps.wVmin) / 2.f) *
                                        200.f / static_cast<float>(m_caps.wVmax - m_caps.wVmin);

        // Special case for POV, it is given as an angle
        if (pos.dwPOV != 0xFFFF)
        {
            const float angle                = static_cast<float>(pos.dwPOV) / 18000.f * 3.141592654f;
            state.axes[Joystick::Axis::PovX] = std::sin(angle) * 100;
            state.axes[Joystick::Axis::PovY] = std::cos(angle) * 100;
        }
        else
        {
            state.axes[Joystick::Axis::PovX] = 0;
            state.axes[Joystick::Axis::PovY] = 0;
        }

        // Buttons
        for (unsigned int i = 0; i < Joystick::ButtonCount; ++i)
            state.buttons[i] = (pos.dwButtons & (1u << i)) != 0;
    }

    return state;
}


////////////////////////////////////////////////////////////
void JoystickImpl::initializeDInput()
{
    // Try to load dinput8.dll
    dinput8dll = LoadLibraryA("dinput8.dll");

    if (dinput8dll)
    {
        // Try to get the address of the DirectInput8Create entry point
        using DirectInput8CreateFunc = HRESULT(WINAPI*)(HINSTANCE, DWORD, const IID&, LPVOID*, LPUNKNOWN);
        auto directInput8Create      = reinterpret_cast<DirectInput8CreateFunc>(
            reinterpret_cast<void*>(GetProcAddress(dinput8dll, "DirectInput8Create")));

        if (directInput8Create)
        {
            // Try to acquire a DirectInput 8.x interface
            const HRESULT result = directInput8Create(GetModuleHandleW(nullptr),
                                                      0x0800,
                                                      guids::IID_IDirectInput8W,
                                                      reinterpret_cast<void**>(&directInput),
                                                      nullptr);

            if (FAILED(result))
            {
                // De-initialize everything
                directInput = nullptr;
                FreeLibrary(dinput8dll);
                dinput8dll = nullptr;

                err() << "Failed to initialize DirectInput: " << result << std::endl;
            }
        }
        else
        {
            // Unload dinput8.dll
            FreeLibrary(dinput8dll);
            dinput8dll = nullptr;
        }
    }
}


////////////////////////////////////////////////////////////
void JoystickImpl::cleanupDInput()
{
    // Release the DirectInput interface
    if (directInput)
    {
        directInput->Release();
        directInput = nullptr;
    }

    // Unload dinput8.dll
    if (dinput8dll)
        FreeLibrary(dinput8dll);
}


////////////////////////////////////////////////////////////
bool JoystickImpl::isConnectedDInput(unsigned int index)
{
    // Check if a joystick with the given index is in the connected list
    return std::any_of(joystickList.cbegin(),
                       joystickList.cend(),
                       [index](const JoystickRecord& record) { return record.index == index; });
}


////////////////////////////////////////////////////////////
void JoystickImpl::updateConnectionsDInput()
{
    // Clear plugged flags so we can determine which devices were added/removed
    for (JoystickRecord& record : joystickList)
        record.plugged = false;

    // Enumerate devices
    const HRESULT result = directInput->EnumDevices(DI8DEVCLASS_GAMECTRL,
                                                    &JoystickImpl::deviceEnumerationCallback,
                                                    nullptr,
                                                    DIEDFL_ATTACHEDONLY);

    // Remove devices that were not connected during the enumeration
    joystickList.erase(std::remove_if(joystickList.begin(),
                                      joystickList.end(),
                                      [](const JoystickRecord& joystickRecord) { return !joystickRecord.plugged; }),
                       joystickList.end());

    if (FAILED(result))
    {
        err() << "Failed to enumerate DirectInput devices: " << result << std::endl;

        return;
    }

    // Assign unused joystick indices to devices that were newly connected
    for (unsigned int i = 0; i < Joystick::Count; ++i)
    {
        for (JoystickRecord& record : joystickList)
        {
            if (record.index == i)
                break;

            if (record.index == Joystick::Count)
            {
                record.index = i;
                break;
            }
        }
    }
}


////////////////////////////////////////////////////////////
bool JoystickImpl::openDInput(unsigned int index)
{
    // Initialize DirectInput members
    m_device = nullptr;

    m_axes.fill(-1);
    m_buttons.fill(-1);

    m_deviceCaps        = {};
    m_deviceCaps.dwSize = sizeof(DIDEVCAPS);
    m_state             = JoystickState();
    m_buffered          = false;

    // Search for a joystick with the given index in the connected list
    for (const JoystickRecord& record : joystickList)
    {
        if (record.index == index)
        {
            m_useXInput = record.xInputDevice;
            // Create device
            HRESULT result = directInput->CreateDevice(record.guid, &m_device, nullptr);

            if (FAILED(result))
            {
                err() << "Failed to create DirectInput device: " << result << std::endl;

                return false;
            }

            // Get vendor and product id of the device
            auto property              = DIPROPDWORD();
            property.diph.dwSize       = sizeof(property);
            property.diph.dwHeaderSize = sizeof(property.diph);
            property.diph.dwHow        = DIPH_DEVICE;

            if (SUCCEEDED(m_device->GetProperty(DIPROP_VIDPID, &property.diph)))
            {
                m_identification.productId = HIWORD(property.dwData);
                m_identification.vendorId  = LOWORD(property.dwData);

                // Check if device is already blacklisted
                if (m_identification.productId && m_identification.vendorId)
                {
                    for (const JoystickBlacklistEntry& blacklistEntry : joystickBlacklist)
                    {
                        if ((m_identification.productId == blacklistEntry.productId) &&
                            (m_identification.vendorId == blacklistEntry.vendorId))
                        {
                            // Device is blacklisted
                            m_device->Release();
                            m_device = nullptr;

                            return false;
                        }
                    }
                }
            }

            // Get friendly product name of the device
            auto stringProperty              = DIPROPSTRING();
            stringProperty.diph.dwSize       = sizeof(stringProperty);
            stringProperty.diph.dwHeaderSize = sizeof(stringProperty.diph);
            stringProperty.diph.dwHow        = DIPH_DEVICE;
            stringProperty.diph.dwObj        = 0;

            if (SUCCEEDED(m_device->GetProperty(DIPROP_PRODUCTNAME, &stringProperty.diph)))
                m_identification.name = stringProperty.wsz;

            if (m_useXInput)
            {
                // Use XInput instead of DirectInput for obtaining caps and data
                return true;
            }

            static bool         formatInitialized = false;
            static DIDATAFORMAT format;

            if (!formatInitialized)
            {
                const DWORD axisType   = DIDFT_AXIS | DIDFT_OPTIONAL | DIDFT_ANYINSTANCE;
                const DWORD povType    = DIDFT_POV | DIDFT_OPTIONAL | DIDFT_ANYINSTANCE;
                const DWORD buttonType = DIDFT_BUTTON | DIDFT_OPTIONAL | DIDFT_ANYINSTANCE;

                static std::array<DIOBJECTDATAFORMAT, 8 * 4 + 4 + Joystick::ButtonCount> data{};

                for (std::size_t i = 0; i < 4; ++i)
                {
                    data[8 * i + 0].pguid = &guids::GUID_XAxis;
                    data[8 * i + 1].pguid = &guids::GUID_YAxis;
                    data[8 * i + 2].pguid = &guids::GUID_ZAxis;
                    data[8 * i + 3].pguid = &guids::GUID_RxAxis;
                    data[8 * i + 4].pguid = &guids::GUID_RyAxis;
                    data[8 * i + 5].pguid = &guids::GUID_RzAxis;
                    data[8 * i + 6].pguid = &guids::GUID_Slider;
                    data[8 * i + 7].pguid = &guids::GUID_Slider;
                }

                data[0].dwOfs  = DIJOFS_X;
                data[1].dwOfs  = DIJOFS_Y;
                data[2].dwOfs  = DIJOFS_Z;
                data[3].dwOfs  = DIJOFS_RX;
                data[4].dwOfs  = DIJOFS_RY;
                data[5].dwOfs  = DIJOFS_RZ;
                data[6].dwOfs  = DIJOFS_SLIDER(0);
                data[7].dwOfs  = DIJOFS_SLIDER(1);
                data[8].dwOfs  = FIELD_OFFSET(DIJOYSTATE2, lVX);
                data[9].dwOfs  = FIELD_OFFSET(DIJOYSTATE2, lVY);
                data[10].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lVZ);
                data[11].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lVRx);
                data[12].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lVRy);
                data[13].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lVRz);
                data[14].dwOfs = FIELD_OFFSET(DIJOYSTATE2, rglVSlider[0]);
                data[15].dwOfs = FIELD_OFFSET(DIJOYSTATE2, rglVSlider[1]);
                data[16].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lAX);
                data[17].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lAY);
                data[18].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lAZ);
                data[19].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lARx);
                data[20].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lARy);
                data[21].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lARz);
                data[22].dwOfs = FIELD_OFFSET(DIJOYSTATE2, rglASlider[0]);
                data[23].dwOfs = FIELD_OFFSET(DIJOYSTATE2, rglASlider[1]);
                data[24].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lFX);
                data[25].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lFY);
                data[26].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lFZ);
                data[27].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lFRx);
                data[28].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lFRy);
                data[29].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lFRz);
                data[30].dwOfs = FIELD_OFFSET(DIJOYSTATE2, rglFSlider[0]);
                data[31].dwOfs = FIELD_OFFSET(DIJOYSTATE2, rglFSlider[1]);

                for (std::size_t i = 0; i < 8 * 4; ++i)
                {
                    data[i].dwType  = axisType;
                    data[i].dwFlags = 0;
                }

                for (std::size_t i = 0; i < 4; ++i)
                {
                    data[8 * 4 + i].pguid   = &guids::GUID_POV;
                    data[8 * 4 + i].dwOfs   = static_cast<DWORD>(DIJOFS_POV(static_cast<unsigned int>(i)));
                    data[8 * 4 + i].dwType  = povType;
                    data[8 * 4 + i].dwFlags = 0;
                }

                for (unsigned int i = 0; i < Joystick::ButtonCount; ++i)
                {
                    data[8 * 4 + 4 + i].pguid   = nullptr;
                    data[8 * 4 + 4 + i].dwOfs   = static_cast<DWORD>(DIJOFS_BUTTON(i));
                    data[8 * 4 + 4 + i].dwType  = buttonType;
                    data[8 * 4 + 4 + i].dwFlags = 0;
                }

                format.dwSize     = sizeof(DIDATAFORMAT);
                format.dwObjSize  = sizeof(DIOBJECTDATAFORMAT);
                format.dwFlags    = DIDFT_ABSAXIS;
                format.dwDataSize = sizeof(DIJOYSTATE2);
                format.dwNumObjs  = 8 * 4 + 4 + Joystick::ButtonCount;
                format.rgodf      = data.data();

                formatInitialized = true;
            }

            // Set device data format
            result = m_device->SetDataFormat(&format);

            if (FAILED(result))
            {
                err() << "Failed to set DirectInput device data format: " << result << std::endl;

                m_device->Release();
                m_device = nullptr;

                return false;
            }

            // Get device capabilities
            result = m_device->GetCapabilities(&m_deviceCaps);

            if (FAILED(result))
            {
                err() << "Failed to get DirectInput device capabilities: " << result << std::endl;

                m_device->Release();
                m_device = nullptr;

                return false;
            }

            // Enumerate device objects (axes/povs/buttons)
            result = m_device->EnumObjects(&JoystickImpl::deviceObjectEnumerationCallback,
                                           this,
                                           DIDFT_AXIS | DIDFT_BUTTON | DIDFT_POV);

            if (FAILED(result))
            {
                err() << "Failed to enumerate DirectInput device objects: " << result << std::endl;

                m_device->Release();
                m_device = nullptr;

                return false;
            }

            // Set device's axis mode to absolute if the device reports having at least one axis
            for (const int axis : m_axes)
            {
                if (axis != -1)
                {
                    property                   = {};
                    property.diph.dwSize       = sizeof(property);
                    property.diph.dwHeaderSize = sizeof(property.diph);
                    property.diph.dwHow        = DIPH_DEVICE;
                    property.diph.dwObj        = 0;

                    result = m_device->GetProperty(DIPROP_AXISMODE, &property.diph);

                    if (FAILED(result))
                    {
                        err() << "Failed to get DirectInput device axis mode for device "
                              << std::quoted(m_identification.name.toAnsiString()) << ": " << result << std::endl;

                        m_device->Release();
                        m_device = nullptr;

                        return false;
                    }

                    // If the axis mode is already set to absolute we don't need to set it again ourselves
                    if (property.dwData == DIPROPAXISMODE_ABS)
                        break;

                    property                   = {};
                    property.diph.dwSize       = sizeof(property);
                    property.diph.dwHeaderSize = sizeof(property.diph);
                    property.diph.dwHow        = DIPH_DEVICE;
                    property.dwData            = DIPROPAXISMODE_ABS;

                    m_device->SetProperty(DIPROP_AXISMODE, &property.diph);

                    // Check if the axis mode has been set to absolute
                    property                   = {};
                    property.diph.dwSize       = sizeof(property);
                    property.diph.dwHeaderSize = sizeof(property.diph);
                    property.diph.dwHow        = DIPH_DEVICE;
                    property.diph.dwObj        = 0;

                    result = m_device->GetProperty(DIPROP_AXISMODE, &property.diph);

                    if (FAILED(result))
                    {
                        err() << "Failed to verify DirectInput device axis mode for device "
                              << std::quoted(m_identification.name.toAnsiString()) << ": " << result << std::endl;

                        m_device->Release();
                        m_device = nullptr;

                        return false;
                    }

                    // If the axis mode hasn't been set to absolute fail here and blacklist the device
                    if (property.dwData != DIPROPAXISMODE_ABS)
                    {
                        if (m_identification.vendorId && m_identification.productId)
                        {
                            JoystickBlacklistEntry entry{};
                            entry.vendorId  = m_identification.vendorId;
                            entry.productId = m_identification.productId;

                            joystickBlacklist.push_back(entry);
                            joystickBlacklist.shrink_to_fit();
                        }

                        m_device->Release();
                        m_device = nullptr;

                        return false;
                    }

                    break;
                }
            }

            // Try to enable buffering by setting the buffer size
            property                   = {};
            property.diph.dwSize       = sizeof(property);
            property.diph.dwHeaderSize = sizeof(property.diph);
            property.diph.dwHow        = DIPH_DEVICE;
            property.dwData            = directInputEventBufferSize;

            result = m_device->SetProperty(DIPROP_BUFFERSIZE, &property.diph);

            if (result == DI_OK)
            {
                // Buffering supported
                m_buffered = true;
            }
            else if (result == DI_POLLEDDEVICE)
            {
                // Only polling supported
                m_buffered = false;
            }
            else
            {
                err() << "Failed to set DirectInput device buffer size for device "
                      << std::quoted(m_identification.name.toAnsiString()) << ": " << result << std::endl;

                m_device->Release();
                m_device = nullptr;

                return false;
            }

            return true;
        }
    }

    return false;
}


////////////////////////////////////////////////////////////
bool JoystickImpl::openXInput(unsigned int index)
{
    for (auto& device : xInputDevices)
    {
        if (device.connected && !device.joystick)
        {
            if (xInputGetCapabilitiesEx != nullptr)
            {
                if (const auto slot = getXInputIndexFromVidPid(static_cast<WORD>(m_identification.vendorId),
                                                               static_cast<WORD>(m_identification.productId)))
                {
                    device.joystickIndex = index;
                    device.joystick      = this;
                    device.xInputIndex   = *slot;
                    m_xInputIndex        = device.xInputIndex.value();
                    m_identification.name = "Generic XInput Device Slot [" + std::to_string(m_xInputIndex.value()) + "]";
                    return true;
                }
            }
            else
            {
                device.joystick       = this;
                device.joystickIndex  = index;
                m_xInputIndex         = device.xInputIndex.value();
                m_identification.name = "Generic XInput Device Slot [" + std::to_string(m_xInputIndex.value()) + "]";
                return true;
            }
        }
    }
    return false;
}


////////////////////////////////////////////////////////////
void JoystickImpl::closeDInput()
{
    if (m_device)
    {
        // Release the device
        m_device->Release();
        m_device = nullptr;
    }
}


////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilitiesDInput() const
{
    JoystickCaps caps;

    // Count how many buttons have valid offsets
    caps.buttonCount = 0;

    for (const int button : m_buttons)
    {
        if (button != -1)
            ++caps.buttonCount;
    }

    // Check which axes have valid offsets
    for (unsigned int i = 0; i < Joystick::AxisCount; ++i)
    {
        const auto axis = static_cast<Joystick::Axis>(i);
        caps.axes[axis] = (m_axes[axis] != -1);
    }

    return caps;
}


////////////////////////////////////////////////////////////
void JoystickImpl::pollXInput()
{
    for (DWORD xInputIndex = 0; xInputIndex < xInputDevices.size(); ++xInputIndex)
    {
        auto&        destState = xInputDevices[xInputIndex];
        XINPUT_STATE xInputState{};
        if (xInputGetState(xInputIndex, &xInputState) == 0)
        {
            destState.connected   = true;
            destState.state       = xInputState;
            destState.xInputIndex = xInputIndex;
        }
        else
        {
            destState.connected   = false;
            destState.state       = {};
            destState.xInputIndex = xInputIndex;
        }
    }
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::updateXInput(XINPUT_STATE& xInputState)
{
    // After consideration, the Directional Pad will be exposed as PovX and PovY axes for consistency with PS5 DualSense controllers.

    auto& state      = m_state;
    auto& gamepad    = xInputState.Gamepad;
    state.buttons[0] = 0 != (gamepad.wButtons & XINPUT_GAMEPAD_A);
    state.buttons[1] = 0 != (gamepad.wButtons & XINPUT_GAMEPAD_B);
    state.buttons[2] = 0 != (gamepad.wButtons & XINPUT_GAMEPAD_X);
    state.buttons[3] = 0 != (gamepad.wButtons & XINPUT_GAMEPAD_Y);
    state.buttons[4] = 0 != (gamepad.wButtons & XINPUT_GAMEPAD_START);
    state.buttons[5] = 0 != (gamepad.wButtons & XINPUT_GAMEPAD_BACK);
    state.buttons[6] = 0 != (gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
    state.buttons[7] = 0 != (gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
    state.buttons[8] = 0 != (gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
    state.buttons[9] = 0 != (gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);

    // The standard deadzone felt, too... dead. This feels reasonable, but should it be configurable?
    // Threshold is ... different, and NOT a deadzone, but instead a required delta between inputs to event an update.
    static constexpr SHORT deadzone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / 2;

    // XInput thumbsticks range from -32767 to 32767 - to scale them to -100.0f .. 100.0f divide by the below factor
    static constexpr auto thumbstickScaleFactor = 327.670f;

    state.axes[Joystick::Axis::X] = (std::abs(gamepad.sThumbLX) < deadzone) ? 0.0f : gamepad.sThumbLX / thumbstickScaleFactor;
    state.axes[Joystick::Axis::Y] = (std::abs(gamepad.sThumbLY) < deadzone) ? 0.0f : gamepad.sThumbLY / thumbstickScaleFactor;
    state.axes[Joystick::Axis::Z] = (std::abs(gamepad.sThumbRX) < deadzone) ? 0.0f : gamepad.sThumbRX / thumbstickScaleFactor;
    state.axes[Joystick::Axis::R] = (std::abs(gamepad.sThumbRY) < deadzone) ? 0.0f : gamepad.sThumbRY / thumbstickScaleFactor;

    // D-pad as axes (PovX and PovY) in Cartesian form: 100, -100, or 0
    const bool dpadUp    = 0 != (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
    const bool dpadDown  = 0 != (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
    const bool dpadLeft  = 0 != (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
    const bool dpadRight = 0 != (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);

    // PovX: Right = 100, Left = -100, both/neither = 0
    if (dpadLeft && dpadRight)
        state.axes[Joystick::Axis::PovX] = 0.0f;
    else if (dpadRight)
        state.axes[Joystick::Axis::PovX] = 100.0f;
    else if (dpadLeft)
        state.axes[Joystick::Axis::PovX] = -100.0f;
    else
        state.axes[Joystick::Axis::PovX] = 0.0f;


    // PovY: Up = 100, Down = -100, both/neither = 0
    if (dpadUp && dpadDown)
        state.axes[Joystick::Axis::PovY] = 0.0f;
    else if (dpadUp)
        state.axes[Joystick::Axis::PovY] = 100.0f;
    else if (dpadDown)
        state.axes[Joystick::Axis::PovY] = -100.0f;
    else
        state.axes[Joystick::Axis::PovY] = 0.0f;

    // XInput triggers range between 0 and 255 - to scale them to -100.0f .. 100.0f divide by the below factor (255 / 200)
    static constexpr auto triggerScaleFactor = 1.275f;
    state.axes[Joystick::Axis::U]            = (gamepad.bLeftTrigger / triggerScaleFactor) - 100.0f;
    state.axes[Joystick::Axis::V]            = (gamepad.bRightTrigger / triggerScaleFactor) - 100.0f;

    state.connected = true;
    return state;
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::updateDInputBuffered()
{
    // If we don't make it to the end of this function, mark the device as disconnected
    m_state.connected = false;

    if (!m_device)
        return m_state;

    std::array<DIDEVICEOBJECTDATA, directInputEventBufferSize> events{};
    DWORD                                                      eventCount = directInputEventBufferSize;

    // Try to get the device data
    HRESULT result = m_device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), events.data(), &eventCount, 0);

    // If we have not acquired or have lost the device, attempt to (re-)acquire it and get the device data again
    if ((result == DIERR_NOTACQUIRED) || (result == DIERR_INPUTLOST))
    {
        m_device->Acquire();
        result = m_device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), events.data(), &eventCount, 0);
    }

    // If we still can't get the device data, assume it has been disconnected
    if ((result == DIERR_NOTACQUIRED) || (result == DIERR_INPUTLOST))
    {
        m_device->Release();
        m_device = nullptr;

        return m_state;
    }

    if (FAILED(result))
    {
        err() << "Failed to get DirectInput device data: " << result << std::endl;

        return m_state;
    }

    // Iterate through all buffered events
    for (DWORD i = 0; i < eventCount; ++i)
    {
        bool eventHandled = false;

        // Get the current state of each axis
        for (unsigned int j = 0; j < Joystick::AxisCount; ++j)
        {
            const auto axis = static_cast<Joystick::Axis>(j);
            if (m_axes[axis] == static_cast<int>(events[i].dwOfs))
            {
                if ((axis == Joystick::Axis::PovX) || (axis == Joystick::Axis::PovY))
                {
                    const unsigned short value = LOWORD(events[i].dwData);

                    if (value != 0xFFFF)
                    {
                        const float angle = (static_cast<float>(value)) * 3.141592654f / DI_DEGREES / 180.f;

                        m_state.axes[Joystick::Axis::PovX] = std::sin(angle) * 100.f;
                        m_state.axes[Joystick::Axis::PovY] = std::cos(angle) * 100.f;
                    }
                    else
                    {
                        m_state.axes[Joystick::Axis::PovX] = 0.f;
                        m_state.axes[Joystick::Axis::PovY] = 0.f;
                    }
                }
                else
                {
                    m_state.axes[axis] = (static_cast<float>(static_cast<short>(events[i].dwData)) + 0.5f) * 100.f / 32767.5f;
                }

                eventHandled = true;

                break;
            }
        }

        if (eventHandled)
            continue;

        // Get the current state of each button
        for (unsigned int j = 0; j < Joystick::ButtonCount; ++j)
        {
            if (m_buttons[j] == static_cast<int>(events[i].dwOfs))
                m_state.buttons[j] = (events[i].dwData != 0);
        }
    }

    m_state.connected = true;

    return m_state;
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::updateDInputPolled()
{
    JoystickState state;

    if (m_device)
    {
        // Poll the device
        m_device->Poll();

        DIJOYSTATE2 joystate;

        // Try to get the device state
        HRESULT result = m_device->GetDeviceState(sizeof(joystate), &joystate);

        // If we have not acquired or have lost the device, attempt to (re-)acquire it and get the device state again
        if ((result == DIERR_NOTACQUIRED) || (result == DIERR_INPUTLOST))
        {
            m_device->Acquire();
            m_device->Poll();
            result = m_device->GetDeviceState(sizeof(joystate), &joystate);
        }

        // If we still can't get the device state, assume it has been disconnected
        if ((result == DIERR_NOTACQUIRED) || (result == DIERR_INPUTLOST))
        {
            m_device->Release();
            m_device = nullptr;

            return state;
        }

        if (FAILED(result))
        {
            err() << "Failed to get DirectInput device state: " << result << std::endl;

            return state;
        }

        // Get the current state of each axis
        for (unsigned int i = 0; i < Joystick::AxisCount; ++i)
        {
            const auto axis = static_cast<Joystick::Axis>(i);
            if (m_axes[axis] != -1)
            {
                if ((axis == Joystick::Axis::PovX) || (axis == Joystick::Axis::PovY))
                {
                    const unsigned short value = LOWORD(
                        *reinterpret_cast<const DWORD*>(reinterpret_cast<const char*>(&joystate) + m_axes[axis]));

                    if (value != 0xFFFF)
                    {
                        const float angle = (static_cast<float>(value)) * 3.141592654f / DI_DEGREES / 180.f;

                        state.axes[Joystick::Axis::PovX] = std::sin(angle) * 100.f;
                        state.axes[Joystick::Axis::PovY] = std::cos(angle) * 100.f;
                    }
                    else
                    {
                        state.axes[Joystick::Axis::PovX] = 0.f;
                        state.axes[Joystick::Axis::PovY] = 0.f;
                    }
                }
                else
                {
                    state.axes[axis] = (static_cast<float>(*reinterpret_cast<const LONG*>(
                                            reinterpret_cast<const char*>(&joystate) + m_axes[axis])) +
                                        0.5f) *
                                       100.f / 32767.5f;
                }
            }
            else
            {
                state.axes[axis] = 0.f;
            }
        }

        // Get the current state of each button
        for (unsigned int i = 0; i < Joystick::ButtonCount; ++i)
        {
            if (m_buttons[i] != -1)
            {
                const BYTE value = *reinterpret_cast<const BYTE*>(reinterpret_cast<const char*>(&joystate) + m_buttons[i]);

                state.buttons[i] = ((value & 0x80) != 0);
            }
            else
            {
                state.buttons[i] = false;
            }
        }

        state.connected = true;
    }

    return state;
}


////////////////////////////////////////////////////////////
BOOL CALLBACK JoystickImpl::deviceEnumerationCallback(const DIDEVICEINSTANCE* deviceInstance, void*)
{
    for (JoystickRecord& record : joystickList)
    {
        if (record.guid == deviceInstance->guidInstance)
        {
            if (isXInputDevice(deviceInstance->guidProduct))
                record.xInputDevice = true;

            record.plugged = true;

            return DIENUM_CONTINUE;
        }
    }

    JoystickRecord record = {deviceInstance->guidInstance, Joystick::Count, true};
    if (isXInputDevice(deviceInstance->guidProduct))
        record.xInputDevice = true;
    
    joystickList.push_back(record);

    return DIENUM_CONTINUE;
}


////////////////////////////////////////////////////////////
BOOL CALLBACK JoystickImpl::deviceObjectEnumerationCallback(const DIDEVICEOBJECTINSTANCE* deviceObjectInstance, void* userData)
{
    JoystickImpl& joystick = *reinterpret_cast<JoystickImpl*>(userData);

    if (DIDFT_GETTYPE(deviceObjectInstance->dwType) & DIDFT_AXIS)
    {
        // Axes
        if (deviceObjectInstance->guidType == guids::GUID_XAxis)
            joystick.m_axes[Joystick::Axis::X] = DIJOFS_X;
        else if (deviceObjectInstance->guidType == guids::GUID_YAxis)
            joystick.m_axes[Joystick::Axis::Y] = DIJOFS_Y;
        else if (deviceObjectInstance->guidType == guids::GUID_ZAxis)
            joystick.m_axes[Joystick::Axis::Z] = DIJOFS_Z;
        else if (deviceObjectInstance->guidType == guids::GUID_RzAxis)
            joystick.m_axes[Joystick::Axis::R] = DIJOFS_RZ;
        else if (deviceObjectInstance->guidType == guids::GUID_RxAxis)
            joystick.m_axes[Joystick::Axis::U] = DIJOFS_RX;
        else if (deviceObjectInstance->guidType == guids::GUID_RyAxis)
            joystick.m_axes[Joystick::Axis::V] = DIJOFS_RY;
        else if (deviceObjectInstance->guidType == guids::GUID_Slider)
        {
            if (joystick.m_axes[Joystick::Axis::U] == -1)
                joystick.m_axes[Joystick::Axis::U] = DIJOFS_SLIDER(0);
            else
                joystick.m_axes[Joystick::Axis::V] = DIJOFS_SLIDER(1);
        }
        else
            return DIENUM_CONTINUE;

        // Set the axis' value range to that of a signed short: [-32768, 32767]
        auto propertyRange              = DIPROPRANGE();
        propertyRange.diph.dwSize       = sizeof(propertyRange);
        propertyRange.diph.dwHeaderSize = sizeof(propertyRange.diph);
        propertyRange.diph.dwObj        = deviceObjectInstance->dwType;
        propertyRange.diph.dwHow        = DIPH_BYID;
        propertyRange.lMin              = -32768;
        propertyRange.lMax              = 32767;

        const HRESULT result = joystick.m_device->SetProperty(DIPROP_RANGE, &propertyRange.diph);

        if (result != DI_OK)
            err() << "Failed to set DirectInput device axis property range: " << result << std::endl;

        return DIENUM_CONTINUE;
    }
    if (DIDFT_GETTYPE(deviceObjectInstance->dwType) & DIDFT_POV)
    {
        // POVs
        if (deviceObjectInstance->guidType == guids::GUID_POV)
        {
            if (joystick.m_axes[Joystick::Axis::PovX] == -1)
            {
                joystick.m_axes[Joystick::Axis::PovX] = DIJOFS_POV(0);
                joystick.m_axes[Joystick::Axis::PovY] = DIJOFS_POV(0);
            }
        }

        return DIENUM_CONTINUE;
    }
    if (DIDFT_GETTYPE(deviceObjectInstance->dwType) & DIDFT_BUTTON)
    {
        // Buttons
        for (unsigned int i = 0; i < Joystick::ButtonCount; ++i)
        {
            if (joystick.m_buttons[i] == -1)
            {
                joystick.m_buttons[i] = DIJOFS_BUTTON(static_cast<int>(i));
                break;
            }
        }

        return DIENUM_CONTINUE;
    }

    return DIENUM_CONTINUE;
}

} // namespace sf::priv

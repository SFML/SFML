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


////////////////////////////////////////////////////////////
// DirectInput
////////////////////////////////////////////////////////////


#ifndef DIDFT_OPTIONAL
#define DIDFT_OPTIONAL 0x80000000
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
} // namespace


////////////////////////////////////////////////////////////
// Legacy joystick API
////////////////////////////////////////////////////////////
namespace
{
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
    if (directInput)
        return isConnectedDInput(index);

    ConnectionCache&   cache                  = connectionCache[index];
    constexpr sf::Time connectionRefreshDelay = sf::milliseconds(500);
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
    if (directInput)
        return openDInput(index);

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
}

////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilities() const
{
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

            static bool         formatInitialized = false;
            static DIDATAFORMAT format;

            if (!formatInitialized)
            {
                const DWORD axisType   = DIDFT_AXIS | DIDFT_OPTIONAL | DIDFT_ANYINSTANCE;
                const DWORD povType    = DIDFT_POV | DIDFT_OPTIONAL | DIDFT_ANYINSTANCE;
                const DWORD buttonType = DIDFT_BUTTON | DIDFT_OPTIONAL | DIDFT_ANYINSTANCE;

                static std::array<DIOBJECTDATAFORMAT, 8 * 4 + 4 + sf::Joystick::ButtonCount> data{};

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

                for (unsigned int i = 0; i < sf::Joystick::ButtonCount; ++i)
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
                format.dwNumObjs  = 8 * 4 + 4 + sf::Joystick::ButtonCount;
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
            record.plugged = true;

            return DIENUM_CONTINUE;
        }
    }

    const JoystickRecord record = {deviceInstance->guidInstance, sf::Joystick::Count, true};
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

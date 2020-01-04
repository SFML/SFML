////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2019 Laurent Gomila (laurent@sfml-dev.org)
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
#include <tchar.h>
#include <regstr.h>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>



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
        const GUID IID_IDirectInput8W = {0xbf798031, 0x483a, 0x4da2, {0xaa, 0x99, 0x5d, 0x64, 0xed, 0x36, 0x97, 0x00}};

        const GUID GUID_XAxis         = {0xa36d02e0, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
        const GUID GUID_YAxis         = {0xa36d02e1, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
        const GUID GUID_ZAxis         = {0xa36d02e2, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
        const GUID GUID_RzAxis        = {0xa36d02e3, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
        const GUID GUID_Slider        = {0xa36d02e4, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};

        const GUID GUID_POV           = {0xa36d02f2, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};

        const GUID GUID_RxAxis        = {0xa36d02f4, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
        const GUID GUID_RyAxis        = {0xa36d02f5, 0xc9f3, 0x11cf, {0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
    }

    HMODULE dinput8dll = NULL;
    IDirectInput8W* directInput = NULL;

    struct JoystickRecord
    {
        GUID guid;
        unsigned int index;
        bool plugged;
    };

    typedef std::vector<JoystickRecord> JoystickList;
    JoystickList joystickList;

    struct JoystickBlacklistEntry
    {
        unsigned int vendorId;
        unsigned int productId;
    };

    typedef std::vector<JoystickBlacklistEntry> JoystickBlacklist;
    JoystickBlacklist joystickBlacklist;

    const DWORD directInputEventBufferSize = 32;
}


////////////////////////////////////////////////////////////
// Legacy joystick API
////////////////////////////////////////////////////////////
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

    // If true, will only update when WM_DEVICECHANGE message is received
    bool lazyUpdates = false;

    // Get a system error string from an error code
    std::string getErrorString(DWORD error)
    {
        PTCHAR buffer;

        if (FormatMessage(FORMAT_MESSAGE_MAX_WIDTH_MASK | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, 0, reinterpret_cast<PTCHAR>(&buffer), 0, NULL) == 0)
            return "Unknown error.";

        sf::String message = buffer;
        LocalFree(buffer);
        return message.toAnsiString();
    }

    // Get the joystick's name
    sf::String getDeviceName(unsigned int index, JOYCAPS caps)
    {
        // Give the joystick a default name
        sf::String joystickDescription = "Unknown Joystick";

        LONG result;
        HKEY rootKey;
        HKEY currentKey;
        std::basic_string<TCHAR> subkey;

        subkey  = REGSTR_PATH_JOYCONFIG;
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
                sf::err() << "Unable to open registry for joystick at index " << index << ": " << getErrorString(result) << std::endl;
                return joystickDescription;
            }
        }

        std::basic_ostringstream<TCHAR> indexString;
        indexString << index + 1;

        subkey  = TEXT("Joystick");
        subkey += indexString.str();
        subkey += REGSTR_VAL_JOYOEMNAME;

        TCHAR keyData[256];
        DWORD keyDataSize = sizeof(keyData);

        result = RegQueryValueEx(currentKey, subkey.c_str(), NULL, NULL, reinterpret_cast<LPBYTE>(keyData), &keyDataSize);
        RegCloseKey(currentKey);

        if (result != ERROR_SUCCESS)
        {
            sf::err() << "Unable to query registry key for joystick at index " << index << ": " << getErrorString(result) << std::endl;
            return joystickDescription;
        }

        subkey  = REGSTR_PATH_JOYOEM;
        subkey += TEXT('\\');
        subkey.append(keyData, keyDataSize / sizeof(TCHAR));

        result = RegOpenKeyEx(rootKey, subkey.c_str(), 0, KEY_READ, &currentKey);

        if (result != ERROR_SUCCESS)
        {
            sf::err() << "Unable to open registry key for joystick at index " << index << ": " << getErrorString(result) << std::endl;
            return joystickDescription;
        }

        keyDataSize = sizeof(keyData);

        result = RegQueryValueEx(currentKey, REGSTR_VAL_JOYOEMNAME, NULL, NULL, reinterpret_cast<LPBYTE>(keyData), &keyDataSize);
        RegCloseKey(currentKey);

        if (result != ERROR_SUCCESS)
        {
            sf::err() << "Unable to query name for joystick at index " << index << ": " << getErrorString(result) << std::endl;
            return joystickDescription;
        }

        keyData[255] = TEXT('\0'); // Ensure null terminator in case the data is too long.
        joystickDescription = keyData;

        return joystickDescription;
    }
}

namespace sf
{
namespace priv
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

    ConnectionCache& cache = connectionCache[index];
    if (!lazyUpdates && cache.timer.getElapsedTime() > connectionRefreshDelay)
    {
        JOYINFOEX joyInfo;
        joyInfo.dwSize = sizeof(joyInfo);
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
        return updateConnectionsDInput();

    for (unsigned int i = 0; i < Joystick::Count; ++i)
    {
        JOYINFOEX joyInfo;
        joyInfo.dwSize = sizeof(joyInfo);
        joyInfo.dwFlags = 0;
        ConnectionCache& cache = connectionCache[i];
        cache.connected = joyGetPosEx(JOYSTICKID1 + i, &joyInfo) == JOYERR_NOERROR;

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
    bool success = joyGetDevCaps(m_index, &m_caps, sizeof(m_caps)) == JOYERR_NOERROR;

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
    if (directInput)
    {
        if (m_buffered)
        {
            return updateDInputBuffered();
        }
        else
        {
            return updateDInputPolled();
        }
    }

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
void JoystickImpl::initializeDInput()
{
    // Try to load dinput8.dll
    dinput8dll = LoadLibraryA("dinput8.dll");

    if (dinput8dll)
    {
        // Try to get the address of the DirectInput8Create entry point
        typedef HRESULT(WINAPI *DirectInput8CreateFunc)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
        DirectInput8CreateFunc directInput8Create = reinterpret_cast<DirectInput8CreateFunc>(GetProcAddress(dinput8dll, "DirectInput8Create"));

        if (directInput8Create)
        {
            // Try to acquire a DirectInput 8.x interface
            HRESULT result = directInput8Create(GetModuleHandleW(NULL), 0x0800, guids::IID_IDirectInput8W, reinterpret_cast<void**>(&directInput), NULL);

            if (FAILED(result))
            {
                // De-initialize everything
                directInput = NULL;
                FreeLibrary(dinput8dll);
                dinput8dll = NULL;

                err() << "Failed to initialize DirectInput: " << result << std::endl;
            }
        }
        else
        {
            // Unload dinput8.dll
            FreeLibrary(dinput8dll);
            dinput8dll = NULL;
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
        directInput = NULL;
    }

    // Unload dinput8.dll
    if (dinput8dll)
        FreeLibrary(dinput8dll);
}


////////////////////////////////////////////////////////////
bool JoystickImpl::isConnectedDInput(unsigned int index)
{
    // Check if a joystick with the given index is in the connected list
    for (std::vector<JoystickRecord>::iterator i = joystickList.begin(); i != joystickList.end(); ++i)
    {
        if (i->index == index)
            return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
void JoystickImpl::updateConnectionsDInput()
{
    // Clear plugged flags so we can determine which devices were added/removed
    for (std::size_t i = 0; i < joystickList.size(); ++i)
        joystickList[i].plugged = false;

    // Enumerate devices
    HRESULT result = directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, &JoystickImpl::deviceEnumerationCallback, NULL, DIEDFL_ATTACHEDONLY);

    // Remove devices that were not connected during the enumeration
    for (std::vector<JoystickRecord>::iterator i = joystickList.begin(); i != joystickList.end();)
    {
        if (!i->plugged)
            i = joystickList.erase(i);
        else
            ++i;
    }

    if (FAILED(result))
    {
        err() << "Failed to enumerate DirectInput devices: " << result << std::endl;

        return;
    }

    // Assign unused joystick indices to devices that were newly connected
    for (unsigned int i = 0; i < Joystick::Count; ++i)
    {
        for (std::vector<JoystickRecord>::iterator j = joystickList.begin(); j != joystickList.end(); ++j)
        {
            if (j->index == i)
                break;

            if (j->index == Joystick::Count)
            {
                j->index = i;
                break;
            }
        }
    }
}


////////////////////////////////////////////////////////////
bool JoystickImpl::openDInput(unsigned int index)
{
    // Initialize DirectInput members
    m_device = NULL;

    for (int i = 0; i < Joystick::AxisCount; ++i)
        m_axes[i] = -1;

    for (int i = 0; i < Joystick::ButtonCount; ++i)
        m_buttons[i] = -1;

    std::memset(&m_deviceCaps, 0, sizeof(DIDEVCAPS));
    m_deviceCaps.dwSize = sizeof(DIDEVCAPS);
    m_state = JoystickState();
    m_buffered = false;

    // Search for a joystick with the given index in the connected list
    for (std::vector<JoystickRecord>::iterator i = joystickList.begin(); i != joystickList.end(); ++i)
    {
        if (i->index == index)
        {
            // Create device
            HRESULT result = directInput->CreateDevice(i->guid, &m_device, NULL);

            if (FAILED(result))
            {
                err() << "Failed to create DirectInput device: " << result << std::endl;

                return false;
            }

            // Get vendor and product id of the device
            DIPROPDWORD property;
            std::memset(&property, 0, sizeof(property));
            property.diph.dwSize = sizeof(property);
            property.diph.dwHeaderSize = sizeof(property.diph);
            property.diph.dwHow = DIPH_DEVICE;

            if (SUCCEEDED(m_device->GetProperty(DIPROP_VIDPID, &property.diph)))
            {
                m_identification.productId = HIWORD(property.dwData);
                m_identification.vendorId = LOWORD(property.dwData);

                // Check if device is already blacklisted
                if (m_identification.productId && m_identification.vendorId)
                {
                    for (JoystickBlacklist::const_iterator iter = joystickBlacklist.begin(); iter != joystickBlacklist.end(); ++iter)
                    {
                        if ((m_identification.productId == iter->productId) &&
                            (m_identification.vendorId  == iter->vendorId))
                        {
                            // Device is blacklisted
                            m_device->Release();
                            m_device = NULL;

                            return false;
                        }
                    }
                }
            }

            // Get friendly product name of the device
            DIPROPSTRING stringProperty;
            std::memset(&stringProperty, 0, sizeof(stringProperty));
            stringProperty.diph.dwSize = sizeof(stringProperty);
            stringProperty.diph.dwHeaderSize = sizeof(stringProperty.diph);
            stringProperty.diph.dwHow = DIPH_DEVICE;
            stringProperty.diph.dwObj = 0;

            if (SUCCEEDED(m_device->GetProperty(DIPROP_PRODUCTNAME, &stringProperty.diph)))
                m_identification.name = stringProperty.wsz;

            static bool formatInitialized = false;
            static DIDATAFORMAT format;

            if (!formatInitialized)
            {
                const DWORD axisType   = DIDFT_AXIS   | DIDFT_OPTIONAL | DIDFT_ANYINSTANCE;
                const DWORD povType    = DIDFT_POV    | DIDFT_OPTIONAL | DIDFT_ANYINSTANCE;
                const DWORD buttonType = DIDFT_BUTTON | DIDFT_OPTIONAL | DIDFT_ANYINSTANCE;

                static DIOBJECTDATAFORMAT data[8 * 4 + 4 + sf::Joystick::ButtonCount];

                for (int i = 0; i < 4; ++i)
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

                data[ 0].dwOfs = DIJOFS_X;
                data[ 1].dwOfs = DIJOFS_Y;
                data[ 2].dwOfs = DIJOFS_Z;
                data[ 3].dwOfs = DIJOFS_RX;
                data[ 4].dwOfs = DIJOFS_RY;
                data[ 5].dwOfs = DIJOFS_RZ;
                data[ 6].dwOfs = DIJOFS_SLIDER(0);
                data[ 7].dwOfs = DIJOFS_SLIDER(1);
                data[ 8].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lVX);
                data[ 9].dwOfs = FIELD_OFFSET(DIJOYSTATE2, lVY);
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

                for (int i = 0; i < 8 * 4; ++i)
                {
                    data[i].dwType = axisType;
                    data[i].dwFlags = 0;
                }

                for (int i = 0; i < 4; ++i)
                {
                    data[8 * 4 + i].pguid = &guids::GUID_POV;
                    data[8 * 4 + i].dwOfs = static_cast<DWORD>(DIJOFS_POV(i));
                    data[8 * 4 + i].dwType = povType;
                    data[8 * 4 + i].dwFlags = 0;
                }

                for (int i = 0; i < sf::Joystick::ButtonCount; ++i)
                {
                    data[8 * 4 + 4 + i].pguid = NULL;
                    data[8 * 4 + 4 + i].dwOfs = static_cast<DWORD>(DIJOFS_BUTTON(i));
                    data[8 * 4 + 4 + i].dwType = buttonType;
                    data[8 * 4 + 4 + i].dwFlags = 0;
                }

                format.dwSize = sizeof(DIDATAFORMAT);
                format.dwObjSize = sizeof(DIOBJECTDATAFORMAT);
                format.dwFlags = DIDFT_ABSAXIS;
                format.dwDataSize = sizeof(DIJOYSTATE2);
                format.dwNumObjs = 8 * 4 + 4 + sf::Joystick::ButtonCount;
                format.rgodf = data;

                formatInitialized = true;
            }

            // Set device data format
            result = m_device->SetDataFormat(&format);

            if (FAILED(result))
            {
                err() << "Failed to set DirectInput device data format: " << result << std::endl;

                m_device->Release();
                m_device = NULL;

                return false;
            }

            // Get device capabilities
            result = m_device->GetCapabilities(&m_deviceCaps);

            if (FAILED(result))
            {
                err() << "Failed to get DirectInput device capabilities: " << result << std::endl;

                m_device->Release();
                m_device = NULL;

                return false;
            }

            // Enumerate device objects (axes/povs/buttons)
            result = m_device->EnumObjects(&JoystickImpl::deviceObjectEnumerationCallback, this, DIDFT_AXIS | DIDFT_BUTTON | DIDFT_POV);

            if (FAILED(result))
            {
                err() << "Failed to enumerate DirectInput device objects: " << result << std::endl;

                m_device->Release();
                m_device = NULL;

                return false;
            }

            // Set device's axis mode to absolute if the device reports having at least one axis
            for (int i = 0; i < Joystick::AxisCount; ++i)
            {
                if (m_axes[i] != -1)
                {
                    std::memset(&property, 0, sizeof(property));
                    property.diph.dwSize = sizeof(property);
                    property.diph.dwHeaderSize = sizeof(property.diph);
                    property.diph.dwHow = DIPH_DEVICE;
                    property.diph.dwObj = 0;

                    result = m_device->GetProperty(DIPROP_AXISMODE, &property.diph);

                    if (FAILED(result))
                    {
                        err() << "Failed to get DirectInput device axis mode for device \""
                              << m_identification.name.toAnsiString() << "\": " << result << std::endl;

                        m_device->Release();
                        m_device = NULL;

                        return false;
                    }

                    // If the axis mode is already set to absolute we don't need to set it again ourselves
                    if (property.dwData == DIPROPAXISMODE_ABS)
                        break;

                    std::memset(&property, 0, sizeof(property));
                    property.diph.dwSize = sizeof(property);
                    property.diph.dwHeaderSize = sizeof(property.diph);
                    property.diph.dwHow = DIPH_DEVICE;
                    property.dwData = DIPROPAXISMODE_ABS;

                    m_device->SetProperty(DIPROP_AXISMODE, &property.diph);

                    // Check if the axis mode has been set to absolute
                    std::memset(&property, 0, sizeof(property));
                    property.diph.dwSize = sizeof(property);
                    property.diph.dwHeaderSize = sizeof(property.diph);
                    property.diph.dwHow = DIPH_DEVICE;
                    property.diph.dwObj = 0;

                    result = m_device->GetProperty(DIPROP_AXISMODE, &property.diph);

                    if (FAILED(result))
                    {
                        err() << "Failed to verify DirectInput device axis mode for device \""
                            << m_identification.name.toAnsiString() << "\": " << result << std::endl;

                        m_device->Release();
                        m_device = NULL;

                        return false;
                    }

                    // If the axis mode hasn't been set to absolute fail here and blacklist the device
                    if (property.dwData != DIPROPAXISMODE_ABS)
                    {
                        if (m_identification.vendorId && m_identification.productId)
                        {
                            JoystickBlacklistEntry entry;

                            entry.vendorId = m_identification.vendorId;
                            entry.productId = m_identification.productId;

                            joystickBlacklist.push_back(entry);

                            // Pre-C++11 shrink_to_fit()
                            JoystickBlacklist(joystickBlacklist.begin(), joystickBlacklist.end()).swap(joystickBlacklist);
                        }

                        m_device->Release();
                        m_device = NULL;

                        return false;
                    }

                    break;
                }
            }

            // Try to enable buffering by setting the buffer size
            std::memset(&property, 0, sizeof(property));
            property.diph.dwSize = sizeof(property);
            property.diph.dwHeaderSize = sizeof(property.diph);
            property.diph.dwHow = DIPH_DEVICE;
            property.dwData = directInputEventBufferSize;

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
                err() << "Failed to set DirectInput device buffer size for device \""
                      << m_identification.name.toAnsiString() << "\": " << result << std::endl;

                m_device->Release();
                m_device = NULL;

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
        m_device = NULL;
    }
}


////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilitiesDInput() const
{
    JoystickCaps caps;

    // Count how many buttons have valid offsets
    caps.buttonCount = 0;

    for (int i = 0; i < Joystick::ButtonCount; ++i)
    {
        if (m_buttons[i] != -1)
            ++caps.buttonCount;
    }

    // Check which axes have valid offsets
    for (int i = 0; i < Joystick::AxisCount; ++i)
        caps.axes[i] = (m_axes[i] != -1);

    return caps;
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::updateDInputBuffered()
{
    // If we don't make it to the end of this function, mark the device as disconnected
    m_state.connected = false;

    if (!m_device)
        return m_state;

    DIDEVICEOBJECTDATA events[directInputEventBufferSize];
    DWORD eventCount = directInputEventBufferSize;

    // Try to get the device data
    HRESULT result = m_device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), events, &eventCount, 0);

    // If we have not acquired or have lost the device, attempt to (re-)acquire it and get the device data again
    if ((result == DIERR_NOTACQUIRED) || (result == DIERR_INPUTLOST))
    {
        m_device->Acquire();
        result = m_device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), events, &eventCount, 0);
    }

    // If we still can't get the device data, assume it has been disconnected
    if ((result == DIERR_NOTACQUIRED) || (result == DIERR_INPUTLOST))
    {
        m_device->Release();
        m_device = NULL;

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
        for (int j = 0; j < Joystick::AxisCount; ++j)
        {
            if (m_axes[j] == events[i].dwOfs)
            {
                if (j == Joystick::PovX)
                {
                    unsigned short value = LOWORD(events[i].dwData);

                    if (value != 0xFFFF)
                    {
                        float angle = (static_cast<float>(value)) * 3.141592654f / DI_DEGREES / 180.f;

                        m_state.axes[j] = std::sin(angle) * 100.f;
                    }
                    else
                    {
                        m_state.axes[j] = 0;
                    }
                }
                else if (j == Joystick::PovY)
                {
                    unsigned short value = LOWORD(events[i].dwData);

                    if (value != 0xFFFF)
                    {
                        float angle = (static_cast<float>(value)) * 3.141592654f / DI_DEGREES / 180.f;

                        m_state.axes[j] = std::cos(angle) * 100.f;
                    }
                    else
                    {
                        m_state.axes[j] = 0.f;
                    }
                }
                else
                {
                    m_state.axes[j] = (static_cast<float>(static_cast<short>(events[i].dwData)) + 0.5f) * 100.f / 32767.5f;
                }

                eventHandled = true;

                break;
            }
        }

        if (eventHandled)
            continue;

        // Get the current state of each button
        for (int j = 0; j < Joystick::ButtonCount; ++j)
        {
            if (m_buttons[j] == events[i].dwOfs)
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
            m_device = NULL;

            return state;
        }

        if (FAILED(result))
        {
            err() << "Failed to get DirectInput device state: " << result << std::endl;

            return state;
        }

        // Get the current state of each axis
        for (int i = 0; i < Joystick::AxisCount; ++i)
        {
            if (m_axes[i] != -1)
            {
                if (i == Joystick::PovX)
                {
                    unsigned short value = LOWORD(*reinterpret_cast<const DWORD*>(reinterpret_cast<const char*>(&joystate) + m_axes[i]));

                    if (value != 0xFFFF)
                    {
                        float angle = (static_cast<float>(value)) * 3.141592654f / DI_DEGREES / 180.f;

                        state.axes[i] = std::sin(angle) * 100.f;
                    }
                    else
                    {
                        state.axes[i] = 0;
                    }
                }
                else if (i == Joystick::PovY)
                {
                    unsigned short value = LOWORD(*reinterpret_cast<const DWORD*>(reinterpret_cast<const char*>(&joystate) + m_axes[i]));

                    if (value != 0xFFFF)
                    {
                        float angle = (static_cast<float>(value)) * 3.141592654f / DI_DEGREES / 180.f;

                        state.axes[i] = std::cos(angle) * 100.f;
                    }
                    else
                    {
                        state.axes[i] = 0.f;
                    }
                }
                else
                {
                    state.axes[i] = (static_cast<float>(*reinterpret_cast<const LONG*>(reinterpret_cast<const char*>(&joystate) + m_axes[i])) + 0.5f) * 100.f / 32767.5f;
                }
            }
            else
            {
                state.axes[i] = 0.f;
            }
        }

        // Get the current state of each button
        for (int i = 0; i < Joystick::ButtonCount; ++i)
        {
            if (m_buttons[i] != -1)
            {
                BYTE value = *reinterpret_cast<const BYTE*>(reinterpret_cast<const char*>(&joystate) + m_buttons[i]);

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
    for (std::size_t i = 0; i < joystickList.size(); ++i)
    {
        if (joystickList[i].guid == deviceInstance->guidInstance)
        {
            joystickList[i].plugged = true;

            return DIENUM_CONTINUE;
        }
    }

    JoystickRecord record = { deviceInstance->guidInstance, sf::Joystick::Count, true };
    joystickList.push_back(record);

    return DIENUM_CONTINUE;
}


////////////////////////////////////////////////////////////
BOOL CALLBACK JoystickImpl::deviceObjectEnumerationCallback(const DIDEVICEOBJECTINSTANCE* deviceObjectInstance, void* userData)
{
    sf::priv::JoystickImpl& joystick = *reinterpret_cast<sf::priv::JoystickImpl*>(userData);

    if (DIDFT_GETTYPE(deviceObjectInstance->dwType) & DIDFT_AXIS)
    {
        // Axes
        if (deviceObjectInstance->guidType == guids::GUID_XAxis)
            joystick.m_axes[Joystick::X] = DIJOFS_X;
        else if (deviceObjectInstance->guidType == guids::GUID_YAxis)
            joystick.m_axes[Joystick::Y] = DIJOFS_Y;
        else if (deviceObjectInstance->guidType == guids::GUID_ZAxis)
            joystick.m_axes[Joystick::Z] = DIJOFS_Z;
        else if (deviceObjectInstance->guidType == guids::GUID_RzAxis)
            joystick.m_axes[Joystick::R] = DIJOFS_RZ;
        else if (deviceObjectInstance->guidType == guids::GUID_RxAxis)
            joystick.m_axes[Joystick::U] = DIJOFS_RX;
        else if (deviceObjectInstance->guidType == guids::GUID_RyAxis)
            joystick.m_axes[Joystick::V] = DIJOFS_RY;
        else if (deviceObjectInstance->guidType == guids::GUID_Slider)
        {
            if(joystick.m_axes[Joystick::U] == -1)
                joystick.m_axes[Joystick::U] = DIJOFS_SLIDER(0);
            else
                joystick.m_axes[Joystick::V] = DIJOFS_SLIDER(1);
        }
        else
            return DIENUM_CONTINUE;

        // Set the axis' value range to that of a signed short: [-32768, 32767]
        DIPROPRANGE propertyRange;

        std::memset(&propertyRange, 0, sizeof(propertyRange));
        propertyRange.diph.dwSize = sizeof(propertyRange);
        propertyRange.diph.dwHeaderSize = sizeof(propertyRange.diph);
        propertyRange.diph.dwObj = deviceObjectInstance->dwType;
        propertyRange.diph.dwHow = DIPH_BYID;
        propertyRange.lMin = -32768;
        propertyRange.lMax =  32767;

        HRESULT result = joystick.m_device->SetProperty(DIPROP_RANGE, &propertyRange.diph);

        if (result != DI_OK)
            err() << "Failed to set DirectInput device axis property range: " << result << std::endl;

        return DIENUM_CONTINUE;
    }
    else if (DIDFT_GETTYPE(deviceObjectInstance->dwType) & DIDFT_POV)
    {
        // POVs
        if (deviceObjectInstance->guidType == guids::GUID_POV)
        {
            if (joystick.m_axes[Joystick::PovX] == -1)
            {
                joystick.m_axes[Joystick::PovX] = DIJOFS_POV(0);
                joystick.m_axes[Joystick::PovY] = DIJOFS_POV(0);
            }
        }

        return DIENUM_CONTINUE;
    }
    else if (DIDFT_GETTYPE(deviceObjectInstance->dwType) & DIDFT_BUTTON)
    {
        // Buttons
        for (int i = 0; i < Joystick::ButtonCount; ++i)
        {
            if (joystick.m_buttons[i] == -1)
            {
                joystick.m_buttons[i] = DIJOFS_BUTTON(i);
                break;
            }
        }

        return DIENUM_CONTINUE;
    }

    return DIENUM_CONTINUE;
}

} // namespace priv

} // namespace sf

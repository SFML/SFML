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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/EnumArray.hpp>
#include <SFML/System/Win32/WindowsHeader.hpp>

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>
#include <mmsystem.h>


namespace sf::priv
{
////////////////////////////////////////////////////////////
class JoystickImpl
{
public:
    ////////////////////////////////////////////////////////////
    static void initialize();

    ////////////////////////////////////////////////////////////
    static void cleanup();

    ////////////////////////////////////////////////////////////
    static bool isConnected(unsigned int index);

    ////////////////////////////////////////////////////////////
    static void setLazyUpdates(bool status);

    ////////////////////////////////////////////////////////////
    static void updateConnections();

    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool open(unsigned int index);

    ////////////////////////////////////////////////////////////
    void close();

    ////////////////////////////////////////////////////////////
    [[nodiscard]] JoystickCaps getCapabilities() const;

    ////////////////////////////////////////////////////////////
    [[nodiscard]] Joystick::Identification getIdentification() const;

    ////////////////////////////////////////////////////////////
    [[nodiscard]] JoystickState update();

    ////////////////////////////////////////////////////////////
    static void initializeDInput();

    ////////////////////////////////////////////////////////////
    static void cleanupDInput();

    ////////////////////////////////////////////////////////////
    static bool isConnectedDInput(unsigned int index);

    ////////////////////////////////////////////////////////////
    static void updateConnectionsDInput();

    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool openDInput(unsigned int index);

    ////////////////////////////////////////////////////////////
    void closeDInput();

    ////////////////////////////////////////////////////////////
    [[nodiscard]] JoystickCaps getCapabilitiesDInput() const;

    ////////////////////////////////////////////////////////////
    [[nodiscard]] JoystickState updateDInputBuffered();

    ////////////////////////////////////////////////////////////
    [[nodiscard]] JoystickState updateDInputPolled();

private:
    ////////////////////////////////////////////////////////////
    /// \brief Device enumeration callback function passed to EnumDevices in updateConnections
    ///
    /// \param deviceInstance Device object instance
    /// \param userData       User data (unused)
    ///
    /// \return DIENUM_CONTINUE to continue enumerating devices or DIENUM_STOP to stop
    ///
    ////////////////////////////////////////////////////////////
    static BOOL CALLBACK deviceEnumerationCallback(const DIDEVICEINSTANCE* deviceInstance, void* userData);

    ////////////////////////////////////////////////////////////
    /// \brief Device object enumeration callback function passed to EnumObjects in open
    ///
    /// \param deviceObjectInstance Device object instance
    /// \param userData             User data (pointer to our JoystickImpl object)
    ///
    /// \return DIENUM_CONTINUE to continue enumerating objects or DIENUM_STOP to stop
    ///
    ////////////////////////////////////////////////////////////
    static BOOL CALLBACK deviceObjectEnumerationCallback(const DIDEVICEOBJECTINSTANCE* deviceObjectInstance, void* userData);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int          m_index{};                              //!< Index of the joystick
    JOYCAPS               m_caps{};                               //!< Joystick capabilities
    IDirectInputDevice8W* m_device{};                             //!< DirectInput 8.x device
    DIDEVCAPS             m_deviceCaps{};                         //!< DirectInput device capabilities
    EnumArray<Joystick::Axis, int, Joystick::AxisCount> m_axes{}; //!< Offsets to the bytes containing the axes states, -1 if not available
    std::array<int, Joystick::ButtonCount> m_buttons{}; //!< Offsets to the bytes containing the button states, -1 if not available
    Joystick::Identification m_identification; //!< Joystick identification
    JoystickState            m_state;          //!< Buffered joystick state
    bool                     m_buffered{}; //!< `true` if the device uses buffering, `false` if the device uses polling
};

} // namespace sf::priv

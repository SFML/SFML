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

#include <dinput.h>
#include <mmsystem.h>


namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief Windows implementation of joysticks
///
////////////////////////////////////////////////////////////
class JoystickImpl
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Perform the global initialization of the joystick module
    ///
    ////////////////////////////////////////////////////////////
    static void initialize();

    ////////////////////////////////////////////////////////////
    /// \brief Perform the global cleanup of the joystick module
    ///
    ////////////////////////////////////////////////////////////
    static void cleanup();

    ////////////////////////////////////////////////////////////
    /// \brief Check if a joystick is currently connected
    ///
    /// \param index Index of the joystick to check
    ///
    /// \return `true` if the joystick is connected, `false` otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isConnected(unsigned int index);

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable lazy enumeration updates
    ///
    /// \param status Whether to rely on windows triggering enumeration updates
    ///
    ////////////////////////////////////////////////////////////
    static void setLazyUpdates(bool status);

    ////////////////////////////////////////////////////////////
    /// \brief Update the connection status of all joysticks
    ///
    ////////////////////////////////////////////////////////////
    static void updateConnections();

    ////////////////////////////////////////////////////////////
    /// \brief Open the joystick
    ///
    /// \param index Index assigned to the joystick
    ///
    /// \return `true` on success, `false` on failure
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool open(unsigned int index);

    ////////////////////////////////////////////////////////////
    /// \brief Close the joystick
    ///
    ////////////////////////////////////////////////////////////
    void close();

    ////////////////////////////////////////////////////////////
    /// \brief Get the joystick capabilities
    ///
    /// \return Joystick capabilities
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] JoystickCaps getCapabilities() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the joystick identification
    ///
    /// \return Joystick identification
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Joystick::Identification getIdentification() const;

    ////////////////////////////////////////////////////////////
    /// \brief Update the joystick and get its new state
    ///
    /// \return Joystick state
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] JoystickState update();

    ////////////////////////////////////////////////////////////
    /// \brief Perform the global initialization of the joystick module (DInput)
    ///
    ////////////////////////////////////////////////////////////
    static void initializeDInput();

    ////////////////////////////////////////////////////////////
    /// \brief Perform the global cleanup of the joystick module (DInput)
    ///
    ////////////////////////////////////////////////////////////
    static void cleanupDInput();

    ////////////////////////////////////////////////////////////
    /// \brief Check if a joystick is currently connected (DInput)
    ///
    /// \param index Index of the joystick to check
    ///
    /// \return `true` if the joystick is connected, `false` otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isConnectedDInput(unsigned int index);

    ////////////////////////////////////////////////////////////
    /// \brief Update the connection status of all joysticks (DInput)
    ///
    ////////////////////////////////////////////////////////////
    static void updateConnectionsDInput();

    ////////////////////////////////////////////////////////////
    /// \brief Open the joystick (DInput)
    ///
    /// \param index Index assigned to the joystick
    ///
    /// \return `true` on success, `false` on failure
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool openDInput(unsigned int index);

    ////////////////////////////////////////////////////////////
    /// \brief Close the joystick (DInput)
    ///
    ////////////////////////////////////////////////////////////
    void closeDInput();

    ////////////////////////////////////////////////////////////
    /// \brief Get the joystick capabilities (DInput)
    ///
    /// \return Joystick capabilities
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] JoystickCaps getCapabilitiesDInput() const;

    ////////////////////////////////////////////////////////////
    /// \brief Update the joystick and get its new state (DInput, Buffered)
    ///
    /// \return Joystick state
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] JoystickState updateDInputBuffered();

    ////////////////////////////////////////////////////////////
    /// \brief Update the joystick and get its new state (DInput, Polled)
    ///
    /// \return Joystick state
    ///
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

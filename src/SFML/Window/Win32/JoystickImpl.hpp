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

#ifndef SFML_JOYSTICKIMPLWIN32_HPP
#define SFML_JOYSTICKIMPLWIN32_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#ifdef _WIN32_WINDOWS
    #undef _WIN32_WINDOWS
#endif
#ifdef _WIN32_WINNT
    #undef _WIN32_WINNT
#endif
#define _WIN32_WINDOWS      0x0501
#define _WIN32_WINNT        0x0501
#define DIRECTINPUT_VERSION 0x0800
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/JoystickImpl.hpp>
#include <SFML/System/String.hpp>
#include <windows.h>
#include <mmsystem.h>
#include <dinput.h>


namespace sf
{
namespace priv
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
    /// \return True if the joystick is connected, false otherwise
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
    /// \return True on success, false on failure
    ///
    ////////////////////////////////////////////////////////////
    bool open(unsigned int index);

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
    JoystickCaps getCapabilities() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the joystick identification
    ///
    /// \return Joystick identification
    ///
    ////////////////////////////////////////////////////////////
    Joystick::Identification getIdentification() const;

    ////////////////////////////////////////////////////////////
    /// \brief Update the joystick and get its new state
    ///
    /// \return Joystick state
    ///
    ////////////////////////////////////////////////////////////
    JoystickState update();

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
    /// \return True if the joystick is connected, false otherwise
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
    /// \return True on success, false on failure
    ///
    ////////////////////////////////////////////////////////////
    bool openDInput(unsigned int index);

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
    JoystickCaps getCapabilitiesDInput() const;

    ////////////////////////////////////////////////////////////
    /// \brief Update the joystick and get its new state (DInput, Buffered)
    ///
    /// \return Joystick state
    ///
    ////////////////////////////////////////////////////////////
    JoystickState updateDInputBuffered();

    ////////////////////////////////////////////////////////////
    /// \brief Update the joystick and get its new state (DInput, Polled)
    ///
    /// \return Joystick state
    ///
    ////////////////////////////////////////////////////////////
    JoystickState updateDInputPolled();

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
    unsigned int             m_index;                          //!< Index of the joystick
    JOYCAPS                  m_caps;                           //!< Joystick capabilities
    IDirectInputDevice8W*    m_device;                         //!< DirectInput 8.x device
    DIDEVCAPS                m_deviceCaps;                     //!< DirectInput device capabilities
    int                      m_axes[Joystick::AxisCount];      //!< Offsets to the bytes containing the axes states, -1 if not available
    int                      m_buttons[Joystick::ButtonCount]; //!< Offsets to the bytes containing the button states, -1 if not available
    Joystick::Identification m_identification;                 //!< Joystick identification
    JoystickState            m_state;                          //!< Buffered joystick state
    bool                     m_buffered;                       //!< true if the device uses buffering, false if the device uses polling
};

} // namespace priv

} // namespace sf


#endif // SFML_JOYSTICKIMPLWIN32_HPP

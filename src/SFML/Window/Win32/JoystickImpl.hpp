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

#include <vector>

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
    [[nodiscard]] JoystickState update() const;

private:
    /// \brief informs system of device added
    static void DispatchDeviceConnected(HANDLE deviceHandle);
    /// \brief informs system of device removed
    static void DispatchDeviceRemoved(HANDLE deviceHandle);
    /// \brief Performs XInput Operations
    static void DispatchXInput();
    /// \brief Performs RawInput Operations
    static void DispatchRawInput(HRAWINPUT inputDevice);

    static UINT GetJoystickMsgAtom();

    static LRESULT      Win32JoystickWndProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);
    static DWORD WINAPI Win32JoystickDispatchThread(LPVOID lpParam);

    static bool           ExtractVidPid(const std::wstring& devicePath, USHORT& vid, USHORT& pid, bool& isXInput);
    static Joystick::Axis GetAxis(int index);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int m_index{};                 //!< Index of the joystick
    bool         m_useXInput{};             //!< True if it's an XInput device, false for RawInput
    unsigned int m_xInputIndex{0xFFFFFFFF}; //!< The XInput index of the device.
    DWORD        m_xInputPacketNumber{};    //!< The last packet number for XInput polling operations.
    HANDLE       m_lastDeviceHandle{};      //!< The last device handle that device connected with.
    // !!: The same device will have different handles each time it disconnects and reconnects.
    JoystickCaps             m_caps{};           //!< The capabilities of the joystick
    Joystick::Identification m_identification{}; //!< The identification of the joystick
    JoystickState            m_state{};          //!< The last state of the joystick (buffered!)
};

} // namespace sf::priv

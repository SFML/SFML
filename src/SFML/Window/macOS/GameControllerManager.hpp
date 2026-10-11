////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2026 Marco Antognini (antognini.marco@gmail.com),
//                         Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Joystick.hpp>

#include <IOKit/hid/IOHIDDevice.h>
#include <array>

namespace sf::priv
{
struct JoystickCaps;
struct JoystickState;

////////////////////////////////////////////////////////////
/// \brief Poll controllers handled by Apple's GameController framework
///
////////////////////////////////////////////////////////////
class GameControllerManager
{
public:
    GameControllerManager(const GameControllerManager&)            = delete;
    GameControllerManager& operator=(const GameControllerManager&) = delete;

    ////////////////////////////////////////////////////////////
    static GameControllerManager& getInstance();

    ////////////////////////////////////////////////////////////
    /// \brief Check whether GameController owns a HID device
    ///
    /// The caller must cache this relatively expensive query per device.
    ///
    ////////////////////////////////////////////////////////////
    static bool supportsHIDDevice(IOHIDDeviceRef device);

    ////////////////////////////////////////////////////////////
    /// \brief Check an assigned slot or reserve an unassigned controller
    ///
    /// Call only for slots that are not occupied by a HID joystick.
    /// A disconnected controller releases its slot without replacing it in this call.
    ///
    ////////////////////////////////////////////////////////////
    bool isConnected(unsigned int slot);

    ////////////////////////////////////////////////////////////
    bool open(unsigned int slot);

    ////////////////////////////////////////////////////////////
    void close(unsigned int slot);

    ////////////////////////////////////////////////////////////
    [[nodiscard]] JoystickCaps getCapabilities(unsigned int slot) const;

    ////////////////////////////////////////////////////////////
    [[nodiscard]] Joystick::Identification getIdentification(unsigned int slot) const;

    ////////////////////////////////////////////////////////////
    [[nodiscard]] JoystickState update(unsigned int slot);

private:
    GameControllerManager();
    ~GameControllerManager();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::array<void*, Joystick::Count> m_controllers{}; ///< Retained GCController objects in stable SFML slots
    bool m_monitoredBackgroundEvents{};                 ///< Original application-wide background monitoring setting
};

} // namespace sf::priv

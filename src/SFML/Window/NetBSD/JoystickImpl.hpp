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
#include <dev/usb/usb.h>
#include <dev/usb/usbhid.h>
#include <usbhid.h>
#include <vector>

namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \note This code has been tested on FreeBSD 9.1 only.
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
    [[nodiscard]] bool open(unsigned int index);

    ////////////////////////////////////////////////////////////
    void close();

    ////////////////////////////////////////////////////////////
    [[nodiscard]] JoystickCaps getCapabilities() const;

    ////////////////////////////////////////////////////////////
    [[nodiscard]] Joystick::Identification getIdentification() const;

    ////////////////////////////////////////////////////////////
    [[nodiscard]] JoystickState update();

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    int                      m_file;           ///< File descriptor of the joystick
    report_desc_t            m_desc;           ///< USB report descriptor
    int                      m_id;             ///< USB id
    std::vector<char>        m_buffer;         ///< USB HID buffer
    int                      m_length;         ///< Buffer length
    Joystick::Identification m_identification; ///< Joystick identification
    JoystickState            m_state;          ///< Current state of the joystick
};

} // namespace sf::priv

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
//               2013-2013 David Demelier (demelier.david@gmail.com)
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

#include <dirent.h>
#include <fcntl.h>
#include <optional>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <unordered_map>
#include <utility>

#include <cstring>

////////////////////////////////////////////////////////////
/// \brief This file implements NetBSD driver joystick
///
////////////////////////////////////////////////////////////


namespace
{
std::unordered_map<unsigned int, std::string> plugged;
std::unordered_map<int, std::pair<int, int>>  hatValueMap;

bool isJoystick(const char* name)
{
    int file = ::open(name, O_RDONLY | O_NONBLOCK);
    int id;

    if (file < 0)
        return false;

    report_desc_t desc = hid_get_report_desc(file);

    if (!desc)
    {
        ::close(file);
        return false;
    }

    if (ioctl(file, USB_GET_REPORT_ID, &id) < 0)
    {
        ::close(file);
        return false;
    }

    hid_data_t data = hid_start_parse(desc, 1 << hid_input, id);

    if (!data)
    {
        hid_dispose_report_desc(desc);
        ::close(file);
        return false;
    }

    hid_item_t item;

    // Assume it isn't
    bool result = false;

    while (hid_get_item(data, &item) > 0)
    {
        if ((item.kind == hid_collection) && (HID_PAGE(item.usage) == HUP_GENERIC_DESKTOP))
        {
            if ((HID_USAGE(item.usage) == HUG_JOYSTICK) || (HID_USAGE(item.usage) == HUG_GAME_PAD))
            {
                result = true;
            }
        }
    }

    hid_end_parse(data);
    hid_dispose_report_desc(desc);
    ::close(file);

    return result;
}

void updatePluggedList()
{
    /*
     * Devices /dev/uhid<x> are shared between joystick and any other
     * human interface device. We need to iterate over all found devices
     * and check if they are joysticks. The index of JoystickImpl::open
     * does not match the /dev/uhid<index> device!
     */
    if (DIR* directory = opendir("/dev"))
    {
        unsigned int   joystickCount  = 0;
        struct dirent* directoryEntry = readdir(directory);

        while (directoryEntry && joystickCount < sf::Joystick::Count)
        {
            if (!std::strncmp(directoryEntry->d_name, "uhid", 4))
            {
                std::string name("/dev/");
                name += directoryEntry->d_name;

                if (isJoystick(name.c_str()))
                    plugged[joystickCount++] = name;
            }

            directoryEntry = readdir(directory);
        }

        closedir(directory);
    }
}

std::optional<sf::Joystick::Axis> usageToAxis(int usage)
{
    switch (usage)
    {
        case HUG_X:
            return sf::Joystick::Axis::X;
        case HUG_Y:
            return sf::Joystick::Axis::Y;
        case HUG_Z:
            return sf::Joystick::Axis::Z;
        case HUG_RZ:
            return sf::Joystick::Axis::R;
        case HUG_RX:
            return sf::Joystick::Axis::U;
        case HUG_RY:
            return sf::Joystick::Axis::V;
        default:
            return std::nullopt;
    }
}

void hatValueToSfml(int value, sf::priv::JoystickState& state)
{
    state.axes[sf::Joystick::Axis::PovX] = static_cast<float>(hatValueMap[value].first);
    state.axes[sf::Joystick::Axis::PovY] = static_cast<float>(hatValueMap[value].second);
}
} // namespace


namespace sf::priv
{
////////////////////////////////////////////////////////////
void JoystickImpl::initialize()
{
    hid_init(nullptr);

    // Do an initial scan
    updatePluggedList();

    // Map of hat values
    hatValueMap[0] = std::make_pair(0, 0); // center

    hatValueMap[1] = std::make_pair(0, -100); // top
    hatValueMap[3] = std::make_pair(100, 0);  // right
    hatValueMap[5] = std::make_pair(0, 100);  // bottom
    hatValueMap[7] = std::make_pair(-100, 0); // left

    hatValueMap[2] = std::make_pair(100, -100);  // top-right
    hatValueMap[4] = std::make_pair(100, 100);   // bottom-right
    hatValueMap[6] = std::make_pair(-100, 100);  // bottom-left
    hatValueMap[8] = std::make_pair(-100, -100); // top-left
}


////////////////////////////////////////////////////////////
void JoystickImpl::cleanup()
{
}


////////////////////////////////////////////////////////////
bool JoystickImpl::isConnected(unsigned int index)
{
    return plugged.find(index) != plugged.end();
}


////////////////////////////////////////////////////////////
bool JoystickImpl::open(unsigned int index)
{
    if (isConnected(index))
    {
        // Open the joystick's file descriptor (read-only and non-blocking)
        m_file = ::open(plugged[index].c_str(), O_RDONLY | O_NONBLOCK);
        if (m_file >= 0)
        {
            // Reset the joystick state
            m_state = JoystickState();

            // Get the report descriptor
            m_desc = hid_get_report_desc(m_file);
            if (!m_desc)
            {
                ::close(m_file);
                return false;
            }

            // And the id
            if (ioctl(m_file, USB_GET_REPORT_ID, &m_id) < 0)
            {
                ::close(m_file);
                return false;
            }

            // Then allocate a buffer for data retrieval
            m_length = hid_report_size(m_desc, hid_input, m_id);
            m_buffer.resize(m_length);

            m_state.connected = true;

            return true;
        }
    }

    return false;
}


////////////////////////////////////////////////////////////
void JoystickImpl::close()
{
    ::close(m_file);
    hid_dispose_report_desc(m_desc);
}


////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilities() const
{
    JoystickCaps caps;
    hid_item_t   item;

    hid_data_t data = hid_start_parse(m_desc, 1 << hid_input, m_id);

    while (hid_get_item(data, &item))
    {
        if (item.kind == hid_input)
        {
            int usage = HID_USAGE(item.usage);

            if (usage == HUP_BUTTON)
            {
                ++caps.buttonCount;
                break;
            }
            else if (usage == HUP_GENERIC_DESKTOP)
            {
                if (usage == HUG_HAT_SWITCH)
                {
                    caps.axes[Joystick::Axis::PovX] = true;
                    caps.axes[Joystick::Axis::PovY] = true;
                }
                else if (const std::optional<Joystick::Axis> axis = usageToAxis(usage))
                {
                    caps.axes[*axis] = true;
                }
            }
        }
    }

    hid_end_parse(data);

    return caps;
}


////////////////////////////////////////////////////////////
Joystick::Identification JoystickImpl::getIdentification() const
{
    return m_identification;
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::JoystickImpl::update()
{
    while (read(m_file, m_buffer.data(), m_length) == m_length)
    {
        hid_data_t data = hid_start_parse(m_desc, 1 << hid_input, m_id);

        // No memory?
        if (!data)
            continue;

        int        buttonIndex = 0;
        hid_item_t item;

        while (hid_get_item(data, &item))
        {
            if (item.kind == hid_input)
            {
                int usage = HID_USAGE(item.usage);

                if (usage == HUP_BUTTON)
                {
                    m_state.buttons[buttonIndex++] = hid_get_data(m_buffer.data(), &item);
                }
                else if (usage == HUP_GENERIC_DESKTOP)
                {
                    int value = hid_get_data(m_buffer.data(), &item);

                    if (usage == HUG_HAT_SWITCH)
                    {
                        hatValueToSfml(value, m_state);
                    }
                    else if (const std::optional<Joystick::Axis> axis = usageToAxis(usage))
                    {
                        int minimum = item.logical_minimum;
                        int maximum = item.logical_maximum;

                        value               = (value - minimum) * 200 / (maximum - minimum) - 100;
                        m_state.axes[*axis] = value;
                    }
                }
            }
        }

        hid_end_parse(data);
    }

    return m_state;
}

} // namespace sf::priv

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/System/Err.hpp>

#include <sys/stat.h>

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <utility>

////////////////////////////////////////////////////////////
/// \brief This file implements FreeBSD driver joystick
///
/// It has been tested on a Saitek gamepad with 12 buttons,
/// 2 analog axis and one hat.
///
/// Note: old joy(4) drivers are not supported and no one use that
/// anymore.
////////////////////////////////////////////////////////////


namespace
{
    std::map<unsigned int, std::string> plugged;
    std::map<int, std::pair<int, int> > hatmap;

    bool isJoystick(const char *name)
    {
        int fd, id;
        bool ret;
        report_desc_t desc = NULL;
        hid_data_t data = NULL;
        hid_item_t item;

        // Assume it isn't
        ret = false;

        if ((fd = ::open(name, O_RDONLY | O_NONBLOCK)) < 0)
            return false;

        if ((desc = hid_get_report_desc(fd)) == NULL)
            goto end;

        id = hid_get_report_id(fd);
        if ((data = hid_start_parse(desc, 1 << hid_input, id)) == NULL)
            goto end;

        while (hid_get_item(data, &item) > 0) {
            switch (item.kind) {
            case hid_collection:
                switch (HID_PAGE(item.usage)) {
                case HUP_GENERIC_DESKTOP:
                    switch (HID_USAGE(item.usage)) {
                    case HUG_JOYSTICK:
                    case HUG_GAME_PAD:
                        ret = true;
                        break;
                    default:
                        break;
                    }
                default:
                    break;
                }
            default:
                break;
            }
        }

end:
        if (desc != NULL)
            hid_dispose_report_desc(desc);
        if (data != NULL)
            hid_end_parse(data);
        close(fd);

        return ret;
    }

    void updatePluggedList()
    {
        DIR *dp;
        struct dirent *entry;

        /*
         * Devices /dev/uhid<x> are shared between joystick and any other
         * human interface device. We need to iterate over all found devices
         * and check if they are joysticks. The index of JoystickImpl::open
         * does not match the /dev/uhid<index> device!
         */
        if ((dp = opendir("/dev")) != NULL) {
            char name[FILENAME_MAX];
            int jc = 0;

            while ((entry = readdir(dp)) != NULL && jc < sf::Joystick::Count) {
                if (strncmp(entry->d_name, "uhid", 4) == 0) {
                    std::sprintf(name, "/dev/%s", entry->d_name);

                    if (isJoystick(name))
                        plugged[jc++] = std::string(name);
                }
            }

            closedir(dp);
        }
    }

    int usageToAxis(int usage)
    {
        int axis;

        switch (usage) {
        case HUG_X:
            axis = sf::Joystick::X;
            break;
        case HUG_Y:
            axis = sf::Joystick::Y;
            break;
        case HUG_Z:
            axis = sf::Joystick::Z;
            break;
        case HUG_RZ:
            axis = sf::Joystick::R;
            break;
        case HUG_RX:
            axis = sf::Joystick::U;
            break;
        case HUG_RY:
            axis = sf::Joystick::V;
            break;
        default:
            axis = -1;
            break;
        }

        return axis;
    }

    void hatvalToSFML(int value, sf::priv::JoystickState &state)
    {
        state.axes[sf::Joystick::PovX] = hatmap[value].first;
        state.axes[sf::Joystick::PovY] = hatmap[value].second;
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void JoystickImpl::initialize()
{
    hid_init(NULL);

    // Do an initial scan
    updatePluggedList();

    // Map of hat values
    hatmap[0] = std::make_pair<int, int>(0, 0);         // center

    hatmap[1] = std::make_pair<int, int>(0, -100);      // top
    hatmap[3] = std::make_pair<int, int>(100, 0);       // right
    hatmap[5] = std::make_pair<int, int>(0, 100);       // bottom
    hatmap[7] = std::make_pair<int, int>(-100, 0);      // left

    hatmap[2] = std::make_pair<int, int>(100, -100);    // top-right
    hatmap[4] = std::make_pair<int, int>(100, 100);     // bottom-right
    hatmap[6] = std::make_pair<int, int>(-100, 100);    // bottom-left
    hatmap[8] = std::make_pair<int, int>(-100, -100);   // top-left
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
            if (m_desc == NULL) {
                ::close(m_file);
                return false;
            }

            // And the id
            m_id = hid_get_report_id(m_file);

            // Then allocate a buffer for data retrievement
            m_length = hid_report_size(m_desc, hid_input, m_id);
            m_buffer = std::calloc(1, m_length);

            if (m_buffer == NULL) {
                ::close(m_file);
                ::hid_dispose_report_desc(m_desc);

                return false;
            }

            return m_state.connected = true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
void JoystickImpl::close()
{
    ::close(m_file);
    ::hid_dispose_report_desc(m_desc);
    ::free(m_buffer);
}


////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilities() const
{
    JoystickCaps caps;
    hid_data_t data;
    hid_item_t item;

    data = hid_start_parse(m_desc, 1 << hid_input, m_id);

    while (hid_get_item(data, &item)) {
        switch (item.kind) {
        case hid_input:
            switch (HID_PAGE(item.usage)) {
                case HUP_BUTTON:
                    caps.buttonCount ++;
                    break;
                case HUP_GENERIC_DESKTOP:
                {
                    int usage = HID_USAGE(item.usage);
                    int axis;

                    if (usage == HUG_HAT_SWITCH) {
                        caps.axes[Joystick::PovX] = true;
                        caps.axes[Joystick::PovY] = true;
                    }
                    else if ((axis = usageToAxis(usage)) != -1)
                    {
                            caps.axes[axis] = true;
                    }
                    break;
                }
                default:
                    break;
            }
        default:
            break;
        }
    }

    hid_end_parse(data);

    return caps;
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::JoystickImpl::update()
{
    while (read(m_file, m_buffer, m_length) == m_length) {
        hid_data_t data;
        hid_item_t item;

        data = hid_start_parse(m_desc, 1 << hid_input, m_id);

        // No memory?
        if (data == NULL)
            continue;

        int but = 0;
        while (hid_get_item(data, &item)) {
            switch (item.kind) {
            case hid_input:
                switch (HID_PAGE(item.usage)) {
                    case HUP_BUTTON:
                        m_state.buttons[but++] = hid_get_data(m_buffer, &item);
                        break;
                    case HUP_GENERIC_DESKTOP:
                    {
                        int usage = HID_USAGE(item.usage);
                        int v = hid_get_data(m_buffer, &item);
                        int axis;

                        if (usage == HUG_HAT_SWITCH)
                            hatvalToSFML(v, m_state);
                        else if ((axis = usageToAxis(usage)) != -1)
                        {
                            int &min = item.logical_minimum;
                            int &max = item.logical_maximum;

                            v = (v - min) * (200) / (max - min) -100;
                            m_state.axes[axis] = v;
                        }

                        break;
                    }
                    default:
                         break;
                }
            default:
                break;
            }
        }

        hid_end_parse(data);
    }

    return m_state;
}

} // namespace priv

} // namespace sf

// vim: set expandtab tabstop=4 softtabstop=4 shiftwidth=4:

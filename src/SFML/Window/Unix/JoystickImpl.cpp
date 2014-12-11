////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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
#include <sys/inotify.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <errno.h>
#include <libudev.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <sstream>

namespace
{
    int notifyFd = -1;
    int inputFd = -1;
    bool plugged[sf::Joystick::Count];

    void updatePluggedList()
    {
        udev* udevContext = udev_new();

        for (unsigned int i = 0; i < sf::Joystick::Count; ++i)
        {
            std::ostringstream name;
            name << "js" << i;
            std::string nameString = name.str();

            int file = ::open(("/dev/input/" + nameString).c_str(), O_RDONLY);

            if (file < 0)
            {
                plugged[i] = false;
                continue;
            }

            ::close(file);

            // Check if the device is really a joystick or an
            // accelerometer by inspecting whether
            // ID_INPUT_ACCELEROMETER is present
            if (!udevContext)
            {
                // Go safe and assume it is if udev isn't available
                plugged[i] = true;
                continue;
            }

            udev_device* udevDevice = udev_device_new_from_subsystem_sysname(udevContext, "input", nameString.c_str());

            if (!udevDevice)
            {
                // Go safe and assume it is if we can't get the device
                plugged[i] = true;
                continue;
            }

            if (udev_device_get_property_value(udevDevice, "ID_INPUT_ACCELEROMETER"))
            {
                // This device is an accelerometer
                plugged[i] = false;
            }
            else
            {
                // This device is not an accelerometer
                // Assume it's a joystick
                plugged[i] = true;
            }

            udev_device_unref(udevDevice);
        }

        if (udevContext)
            udev_unref(udevContext);
    }

    bool hasInotifyEvent()
    {
        fd_set descriptorSet;
        FD_ZERO(&descriptorSet);
        FD_SET(notifyFd, &descriptorSet);
        timeval timeout = {0, 0};

        return (select(notifyFd + 1, &descriptorSet, NULL, NULL, &timeout) > 0) &&
               FD_ISSET(notifyFd, &descriptorSet);
    }

    // Get the joystick name
    std::string getJoystickName(int file, unsigned int index)
    {
        // Get the name
        char name[128];

        if (ioctl(file, JSIOCGNAME(sizeof(name)), name) >= 0)
            return std::string(name);

        sf::err() << "Unable to get name for joystick at index " << index << std::endl;

        return std::string("Unknown Joystick");
    }

    // Get a system attribute from a udev device as an unsigned int
    unsigned int getUdevAttributeUint(udev_device* device, unsigned int index, const std::string& attributeName)
    {
        udev_device* udevDeviceParent = udev_device_get_parent_with_subsystem_devtype(device, "usb", "usb_device");

        if (!udevDeviceParent)
        {
            sf::err() << "Unable to get joystick attribute. "
                      << "Could not find parent USB device for joystick at index " << index << "." << std::endl;
            return 0;
        }

        const char* attributeString = udev_device_get_sysattr_value(udevDeviceParent, attributeName.c_str());

        if (!attributeString)
        {
            sf::err() << "Unable to get joystick attribute '" << attributeName << "'. "
                      << "Attribute does not exist for joystick at index " << index << "." << std::endl;
            return 0;
        }

        return static_cast<unsigned int>(std::strtoul(attributeString, NULL, 16));
    }

    // Get a system attribute for a joystick at index as an unsigned int
    unsigned int getAttributeUint(unsigned int index, const std::string& attributeName)
    {
        udev* udevContext = udev_new();

        if (!udevContext)
        {
            sf::err() << "Unable to get joystick attribute. "
                      << "Could not create udev context." << std::endl;
            return 0;
        }

        std::ostringstream sysname("js");
        sysname << index;

        udev_device* udevDevice = udev_device_new_from_subsystem_sysname(udevContext, "input", sysname.str().c_str());

        if (!udevDevice)
        {
            sf::err() << "Unable to get joystick attribute. "
                      << "Could not find USB device for joystick at index " << index << "." << std::endl;
            udev_unref(udevContext);
            return 0;
        }

        unsigned int attribute = getUdevAttributeUint(udevDevice, index, attributeName);

        udev_device_unref(udevDevice);
        udev_unref(udevContext);
        return attribute;
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void JoystickImpl::initialize()
{
    // Reset the array of plugged joysticks
    std::fill(plugged, plugged + Joystick::Count, false);

    // Do an initial scan
    updatePluggedList();

    // Create the inotify instance
    notifyFd = inotify_init();
    if (notifyFd < 0)
    {
        err() << "Failed to initialize inotify, joystick connections and disconnections won't be notified" << std::endl;
        return;
    }

    // Watch nodes created and deleted in the /dev/input directory
    inputFd = inotify_add_watch(notifyFd, "/dev/input", IN_CREATE | IN_DELETE);
    if (inputFd < 0)
    {
        err() << "Failed to initialize inotify, joystick connections and disconnections won't be notified" << std::endl;

        // No need to hang on to the inotify handle in this case
        ::close(notifyFd);
        notifyFd = -1;
    }
}


////////////////////////////////////////////////////////////
void JoystickImpl::cleanup()
{
    // Stop watching the /dev/input directory
    if (inputFd >= 0)
        inotify_rm_watch(notifyFd, inputFd);

    // Close the inotify file descriptor
    if (notifyFd >= 0)
        ::close(notifyFd);
}


////////////////////////////////////////////////////////////
bool JoystickImpl::isConnected(unsigned int index)
{
    // See if we can skip scanning if inotify is available
    if (notifyFd < 0)
    {
        // inotify is not available, perform a scan every query
        updatePluggedList();
    }
    else if (hasInotifyEvent())
    {
        // Check if new joysticks were added/removed since last update
        // Don't bother decomposing and interpreting the filename, just do a full scan
        updatePluggedList();

        // Flush all the pending events
        if (lseek(notifyFd, 0, SEEK_END) < 0)
            err() << "Failed to flush inotify of all pending joystick events." << std::endl;
    }

    // Then check if the joystick is connected
    return plugged[index];
}

////////////////////////////////////////////////////////////
bool JoystickImpl::open(unsigned int index)
{
    if (plugged[index])
    {
        std::ostringstream name;
        name << "/dev/input/js" << index;

        // Open the joystick's file descriptor (read-only and non-blocking)
        m_file = ::open(name.str().c_str(), O_RDONLY | O_NONBLOCK);
        if (m_file >= 0)
        {
            // Retrieve the axes mapping
            ioctl(m_file, JSIOCGAXMAP, m_mapping);

            // Get info
            m_identification.name      = getJoystickName(m_file, index);
            m_identification.vendorId  = getAttributeUint(index, "idVendor");
            m_identification.productId = getAttributeUint(index, "idProduct");

            // Reset the joystick state
            m_state = JoystickState();

            return true;
        }
    }

    return false;
}


////////////////////////////////////////////////////////////
void JoystickImpl::close()
{
    ::close(m_file);
}


////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilities() const
{
    JoystickCaps caps;

    // Get the number of buttons
    char buttonCount;
    ioctl(m_file, JSIOCGBUTTONS, &buttonCount);
    caps.buttonCount = buttonCount;
    if (caps.buttonCount > Joystick::ButtonCount)
        caps.buttonCount = Joystick::ButtonCount;

    // Get the supported axes
    char axesCount;
    ioctl(m_file, JSIOCGAXES, &axesCount);
    for (int i = 0; i < axesCount; ++i)
    {
        switch (m_mapping[i])
        {
            case ABS_X:        caps.axes[Joystick::X]    = true; break;
            case ABS_Y:        caps.axes[Joystick::Y]    = true; break;
            case ABS_Z:
            case ABS_THROTTLE: caps.axes[Joystick::Z]    = true; break;
            case ABS_RZ:
            case ABS_RUDDER:   caps.axes[Joystick::R]    = true; break;
            case ABS_RX:       caps.axes[Joystick::U]    = true; break;
            case ABS_RY:       caps.axes[Joystick::V]    = true; break;
            case ABS_HAT0X:    caps.axes[Joystick::PovX] = true; break;
            case ABS_HAT0Y:    caps.axes[Joystick::PovY] = true; break;
            default:           break;
        }
    }

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
    // pop events from the joystick file
    js_event joyState;
    while (read(m_file, &joyState, sizeof(joyState)) > 0)
    {
        switch (joyState.type & ~JS_EVENT_INIT)
        {
            // An axis was moved
            case JS_EVENT_AXIS:
            {
                float value = joyState.value * 100.f / 32767.f;
                switch (m_mapping[joyState.number])
                {
                    case ABS_X:        m_state.axes[Joystick::X]    = value; break;
                    case ABS_Y:        m_state.axes[Joystick::Y]    = value; break;
                    case ABS_Z:
                    case ABS_THROTTLE: m_state.axes[Joystick::Z]    = value; break;
                    case ABS_RZ:
                    case ABS_RUDDER:   m_state.axes[Joystick::R]    = value; break;
                    case ABS_RX:       m_state.axes[Joystick::U]    = value; break;
                    case ABS_RY:       m_state.axes[Joystick::V]    = value; break;
                    case ABS_HAT0X:    m_state.axes[Joystick::PovX] = value; break;
                    case ABS_HAT0Y:    m_state.axes[Joystick::PovY] = value; break;
                    default:           break;
                }
                break;
            }

            // A button was pressed
            case JS_EVENT_BUTTON:
            {
                if (joyState.number < Joystick::ButtonCount)
                    m_state.buttons[joyState.number] = (joyState.value != 0);
                break;
            }
        }
    }

    // Check the connection state of the joystick (read() fails with an error != EGAIN if it's no longer connected)
    m_state.connected = (errno == EAGAIN);

    return m_state;
}

} // namespace priv

} // namespace sf

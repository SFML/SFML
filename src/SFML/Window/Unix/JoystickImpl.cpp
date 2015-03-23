////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
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
#include <linux/joystick.h>
#include <libudev.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sstream>
#include <cstring>

namespace
{
    struct udev* udevContext = 0;
    struct udev_monitor* udevMonitor = 0;
    bool plugged[sf::Joystick::Count];

    struct udev_device* getUdevDevice(const std::string& node)
    {
        struct stat statBuffer;

        if (stat(node.c_str(), &statBuffer) < 0)
            return NULL;

        char deviceType;

        if (S_ISBLK(statBuffer.st_mode))
            deviceType = 'b';
        else if (S_ISCHR(statBuffer.st_mode))
            deviceType = 'c';
        else
            return NULL;

        return udev_device_new_from_devnum(udevContext, deviceType, statBuffer.st_rdev);
    }

    int getJoystickNumberFromNode(std::string node)
    {
        std::string::size_type n = node.find("/js");

        if (n != std::string::npos)
            node = node.substr(n + 3);
        else
            return -1;

        std::stringstream sstr(node);

        int i = -1;

        if (!(sstr >> i))
            i = -1;

        return i;
    }

    bool isJoystick(udev_device* udevDevice)
    {
        // If anything goes wrong, we go safe and return true

        // No device to check, assume joystick
        if (!udevDevice)
            return true;

        // Check if this device is a joystick
        if (udev_device_get_property_value(udevDevice, "ID_INPUT_JOYSTICK"))
            return true;

        // Check if this device is something that isn't a joystick
        // We do this because the absence of any ID_INPUT_ property doesn't
        // necessarily mean that the device isn't a joystick, whereas the
        // presence of any ID_INPUT_ property that isn't ID_INPUT_JOYSTICK does
        if (udev_device_get_property_value(udevDevice, "ID_INPUT_ACCELEROMETER") ||
            udev_device_get_property_value(udevDevice, "ID_INPUT_KEY") ||
            udev_device_get_property_value(udevDevice, "ID_INPUT_KEYBOARD") ||
            udev_device_get_property_value(udevDevice, "ID_INPUT_MOUSE") ||
            udev_device_get_property_value(udevDevice, "ID_INPUT_TABLET") ||
            udev_device_get_property_value(udevDevice, "ID_INPUT_TOUCHPAD") ||
            udev_device_get_property_value(udevDevice, "ID_INPUT_TOUCHSCREEN"))
            return false;

        // On some platforms (older udev), ID_INPUT_ properties are not present, instead
        // the system makes use of the ID_CLASS property to identify the device class
        const char* idClass = udev_device_get_property_value(udevDevice, "ID_CLASS");

        if (idClass)
        {
            // Check if the device class matches joystick
            if (std::strstr(idClass, "joystick"))
                return true;

            // Check if the device class matches something that isn't a joystick
            // Rationale same as above
            if (std::strstr(idClass, "accelerometer") ||
                std::strstr(idClass, "key") ||
                std::strstr(idClass, "keyboard") ||
                std::strstr(idClass, "mouse") ||
                std::strstr(idClass, "tablet") ||
                std::strstr(idClass, "touchpad") ||
                std::strstr(idClass, "touchscreen"))
                return false;
        }

        // At this point, assume it's a joystick
        return true;
    }

    void updatePluggedList(udev_device* udevDevice = NULL)
    {
        if (udevDevice)
        {
            const char* node = udev_device_get_devnode(udevDevice);

            if (node)
            {
                int i = getJoystickNumberFromNode(node);

                if ((i >= 0) && (i < sf::Joystick::Count))
                {
                    int file = ::open(node, O_RDONLY);

                    if (file < 0)
                    {
                        plugged[i] = false;
                    }
                    else
                    {
                        ::close(file);
                        plugged[i] = isJoystick(udevDevice);
                        return;
                    }
                }
            }
        }

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

            // Now we check if the device is really a joystick

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

            plugged[i] = isJoystick(udevDevice);

            udev_device_unref(udevDevice);
        }
    }

    bool hasMonitorEvent()
    {
        // This will not fail since we make sure udevMonitor is valid
        int monitorFd = udev_monitor_get_fd(udevMonitor);

        fd_set descriptorSet;
        FD_ZERO(&descriptorSet);
        FD_SET(monitorFd, &descriptorSet);
        timeval timeout = {0, 0};

        return (select(monitorFd + 1, &descriptorSet, NULL, NULL, &timeout) > 0) &&
               FD_ISSET(monitorFd, &descriptorSet);
    }

    // Get a property value from a udev device
    const char* getUdevAttribute(udev_device* udevDevice, const std::string& attributeName)
    {
        return udev_device_get_property_value(udevDevice, attributeName.c_str());
    }

    // Get a system attribute from a USB device
    const char* getUsbAttribute(udev_device* udevDevice, const std::string& attributeName)
    {
        udev_device* udevDeviceParent = udev_device_get_parent_with_subsystem_devtype(udevDevice, "usb", "usb_device");

        if (!udevDeviceParent)
            return NULL;

        return udev_device_get_sysattr_value(udevDeviceParent, attributeName.c_str());
    }

    // Get a system attribute for a joystick devnode as an unsigned int
    unsigned int getUsbAttributeUint(const std::string& node, const std::string& attributeName)
    {
        udev_device* udevDevice = getUdevDevice(node);

        if (!udevDevice)
        {
            sf::err() << "Unable to get joystick attribute. "
                      << "Could not get udev device for joystick " << node << std::endl;
            return 0;
        }

        const char* attribute = getUsbAttribute(udevDevice, attributeName);
        unsigned int value = 0;

        if (attribute)
            value = static_cast<unsigned int>(std::strtoul(attribute, NULL, 16));

        udev_device_unref(udevDevice);
        return value;
    }

    // Get a property value for a joystick devnode as an unsigned int
    unsigned int getUdevAttributeUint(const std::string& node, const std::string& attributeName)
    {
        udev_device* udevDevice = getUdevDevice(node);

        if (!udevDevice)
        {
            sf::err() << "Unable to get joystick attribute. "
                      << "Could not get udev device for joystick " << node << std::endl;
            return 0;
        }

        const char* attribute = getUdevAttribute(udevDevice, attributeName);
        unsigned int value = 0;

        if (attribute)
            value = static_cast<unsigned int>(std::strtoul(attribute, NULL, 16));

        udev_device_unref(udevDevice);
        return value;
    }

    // Get vendor id for a joystick devnode
    unsigned int getJoystickVendorId(const std::string& node)
    {
        unsigned int id = 0;

        // First try using udev
        id = getUdevAttributeUint(node, "ID_VENDOR_ID");

        if (id)
            return id;

        // Fall back to using USB attribute
        id = getUsbAttributeUint(node, "idVendor");

        if (id)
            return id;

        sf::err() << "Failed to get vendor ID of joystick " << node << std::endl;

        return 0;
    }

    // Get product id for a joystick devnode
    unsigned int getJoystickProductId(const std::string& node)
    {
        unsigned int id = 0;

        // First try using udev
        id = getUdevAttributeUint(node, "ID_MODEL_ID");

        if (id)
            return id;

        // Fall back to using USB attribute
        id = getUsbAttributeUint(node, "idProduct");

        if (id)
            return id;

        sf::err() << "Failed to get product ID of joystick " << node << std::endl;

        return 0;
    }

    // Get the joystick name
    std::string getJoystickName(const std::string& node)
    {
        // First try using ioctl with JSIOCGNAME
        int fd = ::open(node.c_str(), O_RDONLY | O_NONBLOCK);

        if (fd >= 0)
        {
            // Get the name
            char name[128];
            std::memset(name, 0, sizeof(name));

            int result = ioctl(fd, JSIOCGNAME(sizeof(name)), name);

            ::close(fd);

            if (result >= 0)
                return std::string(name);
        }

        // Fall back to manual USB chain walk via udev
        if (udevContext)
        {
            udev_device* udevDevice = getUdevDevice(node);

            if (udevDevice)
            {
                const char* product = getUsbAttribute(udevDevice, "product");
                udev_device_unref(udevDevice);

                if (product)
                    return std::string(product);
            }
        }

        sf::err() << "Unable to get name for joystick " << node << std::endl;

        return std::string("Unknown Joystick");
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

    udev* udevContext = udev_new();

    if (!udevContext)
        sf::err() << "Failed to create udev context, getting joystick attributes not available" << std::endl;
    else
        udevMonitor = udev_monitor_new_from_netlink(udevContext, "udev");

    if (!udevMonitor)
    {
        err() << "Failed to create udev monitor, joystick connections and disconnections won't be notified" << std::endl;
    }
    else
    {
        int error = udev_monitor_filter_add_match_subsystem_devtype(udevMonitor, "input", NULL);

        if (error < 0)
        {
            err() << "Failed to add udev monitor filter, joystick connections and disconnections won't be notified: " << error << std::endl;

            udev_monitor_unref(udevMonitor);
            udevMonitor = 0;
        }
        else
        {
            error = udev_monitor_enable_receiving(udevMonitor);

            if (error < 0)
            {
                err() << "Failed to enable udev monitor, joystick connections and disconnections won't be notified: " << error << std::endl;

                udev_monitor_unref(udevMonitor);
                udevMonitor = 0;
            }
        }
    }

    // Do an initial scan
    updatePluggedList();
}


////////////////////////////////////////////////////////////
void JoystickImpl::cleanup()
{
    // Unreference the udev monitor to destroy it
    if (udevMonitor)
    {
        udev_monitor_unref(udevMonitor);
        udevMonitor = 0;
    }

    // Unreference the udev context to destroy it
    if (udevContext)
    {
        udev_unref(udevContext);
        udevContext = 0;
    }
}


////////////////////////////////////////////////////////////
bool JoystickImpl::isConnected(unsigned int index)
{
    // See if we can skip scanning if udev monitor is available
    if (!udevMonitor)
    {
        // udev monitor is not available, perform a scan every query
        updatePluggedList();
    }
    else if (hasMonitorEvent())
    {
        // Check if new joysticks were added/removed since last update
        struct udev_device* udevDevice = udev_monitor_receive_device(udevMonitor);

        // If we can get the specific device, we check that,
        // otherwise just do a full scan if udevDevice == NULL
        updatePluggedList(udevDevice);

        if (udevDevice)
            udev_device_unref(udevDevice);
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
            m_identification.name = getJoystickName(name.str());

            if (udevContext)
            {
                m_identification.vendorId  = getJoystickVendorId(name.str());
                m_identification.productId = getJoystickProductId(name.str());
            }

            // Reset the joystick state
            m_state = JoystickState();

            return true;
        }
        else
        {
            err() << "Failed to open joystick " << name.str() << ": " << errno << std::endl;
        }
    }

    return false;
}


////////////////////////////////////////////////////////////
void JoystickImpl::close()
{
    ::close(m_file);
    m_file = -1;
}


////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilities() const
{
    JoystickCaps caps;

    if (m_file < 0)
        return caps;

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
    if (m_file < 0)
    {
        m_state = JoystickState();
        return m_state;
    }

    // pop events from the joystick file
    js_event joyState;
    int result = read(m_file, &joyState, sizeof(joyState));
    while (result > 0)
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

        result = read(m_file, &joyState, sizeof(joyState));
    }

    // Check the connection state of the joystick
    // read() returns -1 and errno != EGAIN if it's no longer connected
    // We need to check the result of read() as well, since errno could
    // have been previously set by some other function call that failed
    // result can be either negative or 0 at this point
    // If result is 0, assume the joystick is still connected
    // If result is negative, check errno and disconnect if it is not EAGAIN
    m_state.connected = (!result || (errno == EAGAIN));

    return m_state;
}

} // namespace priv

} // namespace sf

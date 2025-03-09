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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/AudioDevice.hpp>
#include <SFML/Audio/PlaybackDevice.hpp>

#include <algorithm>


namespace sf::PlaybackDevice
{
////////////////////////////////////////////////////////////
std::vector<std::string> getAvailableDevices()
{
    const auto devices = priv::AudioDevice::getAvailableDevices();

    std::vector<std::string> deviceNameList;
    deviceNameList.reserve(devices.size());

    for (const auto& device : devices)
        deviceNameList.emplace_back(device.name);

    return deviceNameList;
}


////////////////////////////////////////////////////////////
std::optional<std::string> getDefaultDevice()
{
    for (const auto& device : priv::AudioDevice::getAvailableDevices())
    {
        if (device.isDefault)
            return device.name;
    }

    return std::nullopt;
}


////////////////////////////////////////////////////////////
bool setDevice(const std::string& name)
{
    // Perform a sanity check to make sure the user isn't passing us a non-existent device name
    const auto devices = priv::AudioDevice::getAvailableDevices();
    if (std::find_if(devices.begin(), devices.end(), [&name](const auto& device) { return device.name == name; }) ==
        devices.end())
        return false;

    return priv::AudioDevice::setDevice(name);
}


////////////////////////////////////////////////////////////
std::optional<std::string> getDevice()
{
    return priv::AudioDevice::getDevice();
}

} // namespace sf::PlaybackDevice

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2026 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter this and redistribute it freely,
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
// 3. This notice may not be removed or altered from this source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Monitor.hpp>
#include <SFML/Window/MonitorImpl.hpp>
#include <SFML/Window/VideoMode.hpp>


namespace sf::priv
{
////////////////////////////////////////////////////////////
std::vector<Monitor> MonitorImpl::getAvailableMonitors()
{
    std::vector<Monitor> monitors;

    // Android typically has a single display
    // Screen dimensions would come from ActivityStates in a full implementation

    Monitor monitor;
    monitor.m_primary          = true;
    monitor.m_name             = "Android Display";
    monitor.m_identifier       = "android_0";
    monitor.m_position         = {0, 0};
    monitor.m_resolution       = {1920, 1080}; // Default, would be queried from ActivityStates
    monitor.m_refreshRate      = 60;
    monitor.m_scaledResolution = {1920, 1080};
    monitor.m_workArea         = IntRect(0, 0, 1920, 1080);

    monitors.push_back(monitor);

    return monitors;
}


////////////////////////////////////////////////////////////
Monitor MonitorImpl::getPrimary()
{
    Monitor monitor;
    monitor.m_primary          = true;
    monitor.m_name             = "Android Display";
    monitor.m_identifier       = "android_0";
    monitor.m_position         = {0, 0};
    monitor.m_resolution       = {1920, 1080};
    monitor.m_refreshRate      = 60;
    monitor.m_scaledResolution = {1920, 1080};
    monitor.m_workArea         = IntRect(0, 0, 1920, 1080);

    return monitor;
}


////////////////////////////////////////////////////////////
std::vector<VideoMode> MonitorImpl::getAvailableVideoModesForMonitor(const String& monitorIdentifier)
{
    std::vector<VideoMode> modes;

    // Android doesn't really support multiple video modes
    // Return the current display resolution as the only available mode
    if (monitorIdentifier == "android_0")
    {
        const VideoMode mode(Vector2u(1920, 1080)); // Default resolution
        modes.push_back(mode);
    }

    return modes;
}

} // namespace sf::priv

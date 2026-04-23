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

#include <UIKit/UIKit.h>


namespace sf::priv
{
////////////////////////////////////////////////////////////
std::vector<Monitor> MonitorImpl::getAvailableMonitors()
{
    std::vector<Monitor> monitors;

    // iOS typically has a single display
    // Get the main screen
    const UIScreen* const mainScreen = [UIScreen mainScreen];

    if (mainScreen)
    {
        Monitor monitor;
        monitor.m_primary    = true;
        monitor.m_name       = "iOS Display";
        monitor.m_identifier = "ios_0";
        monitor.m_position   = {0, 0};

        // Get screen bounds
        const CGRect bounds    = mainScreen.bounds;
        monitor.m_resolution.x = static_cast<unsigned int>(bounds.size.width * mainScreen.scale);
        monitor.m_resolution.y = static_cast<unsigned int>(bounds.size.height * mainScreen.scale);

        monitor.m_refreshRate = static_cast<unsigned int>(mainScreen.maximumFramesPerSecond);

        // Scale factor is the DPI scaling (e.g., 2.0 for Retina, 1.0 for standard)
        // Scaled resolution accounts for this scaling
        monitor.m_scaledResolution = Vector2u(static_cast<unsigned int>(bounds.size.width),
                                              static_cast<unsigned int>(bounds.size.height));

        // Work area (iOS typically uses full screen)
        monitor.m_workArea = IntRect(0, 0, static_cast<int>(bounds.size.width), static_cast<int>(bounds.size.height));

        monitors.push_back(monitor);
    }

    // Fallback
    if (monitors.empty())
    {
        Monitor defaultMonitor;
        defaultMonitor.m_primary          = true;
        defaultMonitor.m_name             = "iOS Display";
        defaultMonitor.m_identifier       = "ios_0";
        defaultMonitor.m_position         = {0, 0};
        defaultMonitor.m_resolution       = {1920, 1080};
        defaultMonitor.m_refreshRate      = 60;
        defaultMonitor.m_scaledResolution = {1920, 1080};
        defaultMonitor.m_workArea         = IntRect(0, 0, 1920, 1080);

        monitors.push_back(defaultMonitor);
    }

    return monitors;
}


////////////////////////////////////////////////////////////
Monitor MonitorImpl::getPrimary()
{
    const UIScreen* const mainScreen = [UIScreen mainScreen];

    Monitor monitor;
    monitor.m_primary    = true;
    monitor.m_name       = "iOS Display";
    monitor.m_identifier = "ios_0";
    monitor.m_position   = {0, 0};

    if (mainScreen)
    {
        const CGRect bounds        = mainScreen.bounds;
        monitor.m_resolution.x     = static_cast<unsigned int>(bounds.size.width * mainScreen.scale);
        monitor.m_resolution.y     = static_cast<unsigned int>(bounds.size.height * mainScreen.scale);
        monitor.m_refreshRate      = static_cast<unsigned int>(mainScreen.maximumFramesPerSecond);
        monitor.m_scaledResolution = Vector2u(static_cast<unsigned int>(bounds.size.width),
                                              static_cast<unsigned int>(bounds.size.height));
        monitor.m_workArea = IntRect(0, 0, static_cast<int>(bounds.size.width), static_cast<int>(bounds.size.height));
    }
    else
    {
        monitor.m_resolution       = {1920, 1080};
        monitor.m_refreshRate      = 60;
        monitor.m_scaledResolution = {1920, 1080};
        monitor.m_workArea         = IntRect(0, 0, 1920, 1080);
    }

    return monitor;
}


////////////////////////////////////////////////////////////
std::vector<VideoMode> MonitorImpl::getAvailableVideoModesForMonitor(const String& monitorIdentifier)
{
    std::vector<VideoMode> modes;

    // iOS doesn't really support multiple video modes
    // Return the current display resolution as the only available mode
    if (monitorIdentifier == "ios_0")
    {
        const UIScreen* const mainScreen = [UIScreen mainScreen];
        if (mainScreen)
        {
            const CGRect    bounds = mainScreen.bounds;
            const VideoMode mode(Vector2u(static_cast<unsigned int>(bounds.size.width * mainScreen.scale),
                                          static_cast<unsigned int>(bounds.size.height * mainScreen.scale)));
            modes.push_back(mode);
        }
    }

    return modes;
}

} // namespace sf::priv

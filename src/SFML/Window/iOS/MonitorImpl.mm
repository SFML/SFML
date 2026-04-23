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
#include <SFML/Window/MonitorImpl.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <UIKit/UIKit.h>


namespace sf::priv
{
////////////////////////////////////////////////////////////
std::vector<VideoMode> MonitorImpl::getAvailableVideoModes() const
{
    std::vector<VideoMode> modes;

    // iOS doesn't really support multiple video modes
    // Return the current display resolution as the only available mode
    if (identifier == "ios_0")
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


namespace
{
// Build a MonitorImpl describing the iOS main screen
MonitorImpl getMainScreenMonitorImpl()
{
    MonitorImpl impl;

    impl.primary    = true;
    impl.name       = "iOS Display";
    impl.identifier = "ios_0";
    impl.position   = {0, 0};

    const UIScreen* const mainScreen = [UIScreen mainScreen];

    if (mainScreen)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
        impl.nativeHandle = (__bridge MonitorHandle)mainScreen;
#pragma GCC diagnostic pop

        // Get screen bounds
        const CGRect bounds = mainScreen.bounds;
        impl.resolution.x   = static_cast<unsigned int>(bounds.size.width * mainScreen.scale);
        impl.resolution.y   = static_cast<unsigned int>(bounds.size.height * mainScreen.scale);

        impl.refreshRate = static_cast<unsigned int>(mainScreen.maximumFramesPerSecond);

        // Scale factor is the DPI scaling (e.g., 2.0 for Retina, 1.0 for standard)
        // Scaled resolution accounts for this scaling
        impl.scaledResolution = Vector2u(static_cast<unsigned int>(bounds.size.width),
                                         static_cast<unsigned int>(bounds.size.height));

        // Work area (iOS typically uses full screen)
        impl.workAreaPosition = {0, 0};
        impl.workAreaSize     = impl.scaledResolution;
    }
    else
    {
        impl.resolution       = {1920, 1080};
        impl.refreshRate      = 60;
        impl.scaledResolution = {1920, 1080};
        impl.workAreaPosition = {0, 0};
        impl.workAreaSize     = {1920, 1080};
    }

    return impl;
}
} // namespace


////////////////////////////////////////////////////////////
std::vector<MonitorImpl> MonitorImpl::getAvailableMonitors()
{
    // iOS typically has a single display
    return {getMainScreenMonitorImpl()};
}


////////////////////////////////////////////////////////////
MonitorImpl MonitorImpl::getPrimary()
{
    return getMainScreenMonitorImpl();
}

} // namespace sf::priv

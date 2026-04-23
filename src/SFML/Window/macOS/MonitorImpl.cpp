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

#include <IOKit/graphics/IOGraphicsLib.h>
#include <algorithm>
#include <array>
#include <iomanip>
#include <mach/mach.h>
#include <sstream>

#include <CoreGraphics/CoreGraphics.h>
#include <cmath>


namespace sf::priv
{
////////////////////////////////////////////////////////////
std::vector<VideoMode> MonitorImpl::getAvailableVideoModes() const
{
    std::vector<VideoMode> modes;

    // Get all connected displays
    std::array<CGDirectDisplayID, 128> displays    = {};
    uint32_t                           numDisplays = 0;

    if (CGGetOnlineDisplayList(128, displays.data(), &numDisplays) != kCGErrorSuccess)
    {
        numDisplays = 1;
        displays[0] = CGMainDisplayID();
    }

    // Extract display ID from identifier
    CGDirectDisplayID targetDisplay = 0;
    if (std::sscanf(identifier.toAnsiString().c_str(), "Display_%x", &targetDisplay) == 1)
    {
        for (uint32_t i = 0; i < numDisplays; ++i)
        {
            if (displays[i] == targetDisplay)
            {
                // Get available modes for this display
                CFArrayRef modeArray = CGDisplayCopyAllDisplayModes(displays[i], nullptr);
                if (modeArray)
                {
                    const CFIndex modeCount = CFArrayGetCount(modeArray);
                    for (CFIndex m = 0; m < modeCount; ++m)
                    {
                        auto* modeRef = const_cast<CGDisplayModeRef>(
                            static_cast<const CGDisplayMode*>(CFArrayGetValueAtIndex(modeArray, m)));
                        if (modeRef)
                        {
                            const size_t width  = CGDisplayModeGetWidth(modeRef);
                            const size_t height = CGDisplayModeGetHeight(modeRef);

                            const VideoMode mode(
                                Vector2u(static_cast<unsigned int>(width), static_cast<unsigned int>(height)));

                            // Avoid duplicates
                            if (std::find(modes.begin(), modes.end(), mode) == modes.end())
                            {
                                modes.push_back(mode);
                            }
                        }
                    }

                    CFRelease(modeArray);
                }
                break;
            }
        }
    }

    // Sort modes from best to worst
    std::sort(modes.begin(), modes.end(), std::greater<>());

    return modes;
}


namespace
{
// Get model identifier for a display
String getDisplayModel(CGDirectDisplayID displayID)
{
    std::ostringstream oss;
    oss << "Display_" << std::hex << std::setfill('0') << std::setw(8) << displayID;
    return String(oss.str().c_str());
}

// Get display name
String getDisplayName(CGDirectDisplayID displayID)
{
    std::string name = getDisplayModel(displayID).toAnsiString();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    const io_registry_entry_t displayPort = CGDisplayIOServicePort(displayID);
#pragma clang diagnostic pop
    if (displayPort != MACH_PORT_NULL)
    {
        CFDictionaryRef displayInfo = IODisplayCreateInfoDictionary(displayPort, kIODisplayOnlyPreferredName);
        if (displayInfo)
        {
            const auto* localizedNames = static_cast<CFDictionaryRef>(
                CFDictionaryGetValue(displayInfo, CFSTR(kDisplayProductName)));

            if (localizedNames && CFGetTypeID(localizedNames) == CFDictionaryGetTypeID())
            {
                const auto* englishName = static_cast<CFStringRef>(CFDictionaryGetValue(localizedNames, CFSTR("en_US")));

                if (!englishName)
                    englishName = static_cast<CFStringRef>(CFDictionaryGetValue(localizedNames, CFSTR("en")));

                if (englishName)
                {
                    if (CFStringGetCString(englishName, nullptr, 0, kCFStringEncodingUTF8) > 1)
                    {
                        std::string displayName(static_cast<size_t>(
                                                    CFStringGetMaximumSizeForEncoding(CFStringGetLength(englishName),
                                                                                      kCFStringEncodingUTF8)),
                                                ' ');
                        CFStringGetCString(englishName,
                                           displayName.data(),
                                           static_cast<CFIndex>(displayName.size() + 1),
                                           kCFStringEncodingUTF8);
                        name = displayName;
                    }
                }
            }

            CFRelease(displayInfo);
        }

        mach_port_deallocate(mach_task_self(), displayPort);
    }

    return String(name.c_str());
}

// Create MonitorImpl from display ID
MonitorImpl getMonitorImplFromDisplay(CGDirectDisplayID displayID)
{
    MonitorImpl impl;

    impl.identifier   = getDisplayModel(displayID);
    impl.name         = getDisplayName(displayID);
    impl.primary      = (displayID == CGMainDisplayID());
    impl.nativeHandle = displayID;

    // Get bounds
    const CGRect bounds = CGDisplayBounds(displayID);
    impl.position.x     = static_cast<int>(bounds.origin.x);
    impl.position.y     = static_cast<int>(bounds.origin.y);
    impl.resolution.x   = static_cast<unsigned int>(bounds.size.width);
    impl.resolution.y   = static_cast<unsigned int>(bounds.size.height);

    // Work area on macOS - for now, same as full bounds
    impl.workAreaPosition.x = static_cast<int>(bounds.origin.x);
    impl.workAreaPosition.y = static_cast<int>(bounds.origin.y);
    impl.workAreaSize.x     = static_cast<unsigned int>(bounds.size.width);
    impl.workAreaSize.y     = static_cast<unsigned int>(bounds.size.height);

    // Get refresh rate
    CGDisplayModeRef mode = CGDisplayCopyDisplayMode(displayID);
    if (mode)
    {
        const double refreshRate = CGDisplayModeGetRefreshRate(mode);
        impl.refreshRate         = (refreshRate > 0) ? static_cast<unsigned int>(std::lround(refreshRate)) : 60;
        CFRelease(mode);
    }
    else
    {
        impl.refreshRate = 60;
    }

    // Calculate scaled resolution based on DPI
    const CGFloat dpiScale = CGDisplayScreenSize(displayID).width > 0
                                 ? (bounds.size.width * 25.4) / CGDisplayScreenSize(displayID).width
                                 : 96.0;
    impl.scaledResolution  = Vector2u(static_cast<unsigned int>(impl.resolution.x * 96.0 / dpiScale),
                                      static_cast<unsigned int>(impl.resolution.y * 96.0 / dpiScale));

    return impl;
}

// Fallback monitor used when no display could be queried
MonitorImpl getDefaultMonitor()
{
    MonitorImpl defaultMonitor;
    defaultMonitor.primary          = true;
    defaultMonitor.name             = "Display";
    defaultMonitor.identifier       = "Display_0";
    defaultMonitor.resolution       = {1920, 1080};
    defaultMonitor.position         = {0, 0};
    defaultMonitor.refreshRate      = 60;
    defaultMonitor.scaledResolution = {1920, 1080};
    defaultMonitor.workAreaPosition = {0, 0};
    defaultMonitor.workAreaSize     = {1920, 1080};
    return defaultMonitor;
}
} // namespace


////////////////////////////////////////////////////////////
std::vector<MonitorImpl> MonitorImpl::getAvailableMonitors()
{
    std::vector<MonitorImpl> monitors;

    // Get all connected displays
    std::array<CGDirectDisplayID, 128> displays    = {};
    uint32_t                           numDisplays = 0;

    if (CGGetOnlineDisplayList(128, displays.data(), &numDisplays) != kCGErrorSuccess)
    {
        numDisplays = 1;
        displays[0] = CGMainDisplayID();
    }

    for (uint32_t i = 0; i < numDisplays; ++i)
    {
        const CGDirectDisplayID displayID = displays[i];

        // Skip mirrored displays
        if (CGDisplayIsInMirrorSet(displayID))
        {
            if (CGDisplayPrimaryDisplay(displayID) != displayID)
                continue;
        }

        monitors.push_back(getMonitorImplFromDisplay(displayID));
    }

    // Fallback: if no monitors found, create a default one
    if (monitors.empty())
        monitors.push_back(getDefaultMonitor());

    return monitors;
}


////////////////////////////////////////////////////////////
MonitorImpl MonitorImpl::getPrimary()
{
    return getMonitorImplFromDisplay(CGMainDisplayID());
}

} // namespace sf::priv

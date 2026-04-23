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
namespace
{
// Get model identifier for a display
std::string getDisplayModel(CGDirectDisplayID displayID)
{
    std::ostringstream oss;
    oss << "Display_" << std::hex << std::setfill('0') << std::setw(8) << displayID;
    return oss.str();
}

// Get display name
std::string getDisplayName(CGDirectDisplayID displayID)
{
    // Try to get the monitor name from IOKit
    std::string name = getDisplayModel(displayID);

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

    return name;
}
} // namespace


////////////////////////////////////////////////////////////
std::vector<Monitor> MonitorImpl::getAvailableMonitors()
{
    std::vector<Monitor> monitors;

    // Get all connected displays
    std::array<CGDirectDisplayID, 128> displays    = {};
    uint32_t                           numDisplays = 0;

    if (CGGetOnlineDisplayList(128, displays.data(), &numDisplays) != kCGErrorSuccess)
    {
        // Fallback to main display
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

        Monitor monitor;

        // Get display identifier
        monitor.m_identifier = getDisplayModel(displayID);
        monitor.m_name       = String(getDisplayName(displayID).c_str());
        monitor.m_primary    = (displayID == CGMainDisplayID());

        // Get bounds
        const CGRect bounds    = CGDisplayBounds(displayID);
        monitor.m_position.x   = static_cast<int>(bounds.origin.x);
        monitor.m_position.y   = static_cast<int>(bounds.origin.y);
        monitor.m_resolution.x = static_cast<unsigned int>(bounds.size.width);
        monitor.m_resolution.y = static_cast<unsigned int>(bounds.size.height);

        // Work area on macOS - for now, same as full bounds
        // (proper implementation would use NSScreen.visibleFrame)
        monitor.m_workArea = IntRect(static_cast<int>(bounds.origin.x),
                                     static_cast<int>(bounds.origin.y),
                                     static_cast<int>(bounds.size.width),
                                     static_cast<int>(bounds.size.height));

        // Get refresh rate - get current mode and extract refresh rate
        CGDisplayModeRef mode = CGDisplayCopyDisplayMode(displayID);
        if (mode)
        {
            const double refreshRate = CGDisplayModeGetRefreshRate(mode);
            monitor.m_refreshRate    = (refreshRate > 0) ? static_cast<unsigned int>(std::lround(refreshRate)) : 60;
            CFRelease(mode);
        }
        else
        {
            monitor.m_refreshRate = 60; // Default
        }

        // Calculate scaled resolution based on DPI
        // macOS typically has standard DPI, we can use the scale factor
        const CGFloat dpiScale     = CGDisplayScreenSize(displayID).width > 0
                                         ? (bounds.size.width * 25.4) / CGDisplayScreenSize(displayID).width
                                         : 96.0;
        monitor.m_scaledResolution = Vector2u(static_cast<unsigned int>(monitor.m_resolution.x * 96.0 / dpiScale),
                                              static_cast<unsigned int>(monitor.m_resolution.y * 96.0 / dpiScale));

        monitors.push_back(monitor);
    }

    // Fallback: if no monitors found, create a default one
    if (monitors.empty())
    {
        Monitor defaultMonitor;
        defaultMonitor.m_primary          = true;
        defaultMonitor.m_name             = "Display";
        defaultMonitor.m_identifier       = "Display_0";
        defaultMonitor.m_resolution       = {1920, 1080};
        defaultMonitor.m_position         = {0, 0};
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
    const CGDirectDisplayID primaryDisplay = CGMainDisplayID();

    Monitor monitor;

    monitor.m_identifier = getDisplayModel(primaryDisplay);
    monitor.m_name       = String(getDisplayName(primaryDisplay).c_str());
    monitor.m_primary    = true;

    const CGRect bounds    = CGDisplayBounds(primaryDisplay);
    monitor.m_position.x   = static_cast<int>(bounds.origin.x);
    monitor.m_position.y   = static_cast<int>(bounds.origin.y);
    monitor.m_resolution.x = static_cast<unsigned int>(bounds.size.width);
    monitor.m_resolution.y = static_cast<unsigned int>(bounds.size.height);

    // Work area
    monitor.m_workArea = IntRect(static_cast<int>(bounds.origin.x),
                                 static_cast<int>(bounds.origin.y),
                                 static_cast<int>(bounds.size.width),
                                 static_cast<int>(bounds.size.height));

    CGDisplayModeRef mode = CGDisplayCopyDisplayMode(primaryDisplay);
    if (mode)
    {
        const double refreshRate = CGDisplayModeGetRefreshRate(mode);
        monitor.m_refreshRate    = (refreshRate > 0) ? static_cast<unsigned int>(std::lround(refreshRate)) : 60;
        CFRelease(mode);
    }
    else
    {
        monitor.m_refreshRate = 60;
    }

    const CGFloat dpiScale     = CGDisplayScreenSize(primaryDisplay).width > 0
                                     ? (bounds.size.width * 25.4) / CGDisplayScreenSize(primaryDisplay).width
                                     : 96.0;
    monitor.m_scaledResolution = Vector2u(static_cast<unsigned int>(monitor.m_resolution.x * 96.0 / dpiScale),
                                          static_cast<unsigned int>(monitor.m_resolution.y * 96.0 / dpiScale));

    return monitor;
}


////////////////////////////////////////////////////////////
std::vector<VideoMode> MonitorImpl::getAvailableVideoModesForMonitor(const String& monitorIdentifier)
{
    std::vector<VideoMode> modes;

    // Get all connected displays
    std::array<CGDirectDisplayID, 128> displays    = {};
    uint32_t                           numDisplays = 0;

    if (CGGetOnlineDisplayList(128, displays.data(), &numDisplays) != kCGErrorSuccess)
    {
        // Fallback to main display
        numDisplays = 1;
        displays[0] = CGMainDisplayID();
    }

    const std::string monitorIdStr = monitorIdentifier.toAnsiString();

    // Find the matching display
    for (uint32_t i = 0; i < numDisplays; ++i)
    {
        const CGDirectDisplayID displayID = displays[i];

        // Skip mirrored displays
        if (CGDisplayIsInMirrorSet(displayID))
        {
            if (CGDisplayPrimaryDisplay(displayID) != displayID)
                continue;
        }

        // Check if this is the display we're looking for
        const std::string displayModel = getDisplayModel(displayID);
        if (displayModel == monitorIdStr)
        {
            // Get available modes for this display
            CFArrayRef modeArray = CGDisplayCopyAllDisplayModes(displayID, nullptr);
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

                        const VideoMode mode(Vector2u(static_cast<unsigned int>(width), static_cast<unsigned int>(height)));

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

    // Sort modes from best to worst
    std::sort(modes.begin(), modes.end(), std::greater<>());

    return modes;
}

} // namespace sf::priv

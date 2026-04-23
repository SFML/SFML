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
#include <SFML/Window/Unix/Display.hpp>
#include <SFML/Window/Unix/Utils.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <SFML/System/Err.hpp>

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#include <algorithm>
#include <iomanip>
#include <sstream>

#include <cmath>
#include <cstring>


namespace sf::priv
{
////////////////////////////////////////////////////////////
// Helper template specializations for XRandR types
////////////////////////////////////////////////////////////
template <>
struct XDeleter<XRRScreenResources>
{
    void operator()(XRRScreenResources* resources) const
    {
        XRRFreeScreenResources(resources);
    }
};

template <>
struct XDeleter<XRROutputInfo>
{
    void operator()(XRROutputInfo* info) const
    {
        XRRFreeOutputInfo(info);
    }
};

template <>
struct XDeleter<XRRCrtcInfo>
{
    void operator()(XRRCrtcInfo* info) const
    {
        XRRFreeCrtcInfo(info);
    }
};

////////////////////////////////////////////////////////////
std::vector<Monitor> MonitorImpl::getAvailableMonitors()
{
    std::vector<Monitor> monitors;

    // Open a connection with the X server
    if (const auto display = openDisplay())
    {
        // Get the default screen
        const int    screen = DefaultScreen(display.get());
        const Window root   = RootWindow(display.get(), screen);

        // Get screen resources (outputs, crtcs, modes)
        if (const auto resources = X11Ptr<XRRScreenResources>(XRRGetScreenResources(display.get(), root)))
        {
            // Iterate through all outputs
            for (int i = 0; i < resources->noutput; ++i)
            {
                const RROutput output = resources->outputs[i];

                // Get output information
                if (const auto outputInfo = X11Ptr<XRROutputInfo>(XRRGetOutputInfo(display.get(), resources.get(), output)))
                {
                    // Only process connected outputs
                    if (outputInfo->connection != RR_Connected)
                        continue;

                    Monitor monitor;

                    // Get output name
                    if (outputInfo->name && outputInfo->nameLen > 0)
                    {
                        monitor.m_name       = String(outputInfo->name);
                        monitor.m_identifier = String(outputInfo->name);
                    }
                    else
                    {
                        continue; // Skip outputs with no name
                    }

                    // Get CRTC information if the output is active
                    if (outputInfo->crtc != None)
                    {
                        if (const auto crtcInfo = X11Ptr<XRRCrtcInfo>(
                                XRRGetCrtcInfo(display.get(), resources.get(), outputInfo->crtc)))
                        {
                            // Get position
                            monitor.m_position.x = static_cast<int>(crtcInfo->x);
                            monitor.m_position.y = static_cast<int>(crtcInfo->y);

                            // Get resolution
                            monitor.m_resolution.x = crtcInfo->width;
                            monitor.m_resolution.y = crtcInfo->height;

                            // Handle rotation
                            if (crtcInfo->rotation == RR_Rotate_90 || crtcInfo->rotation == RR_Rotate_270)
                                std::swap(monitor.m_resolution.x, monitor.m_resolution.y);

                            // Get refresh rate from mode
                            if (crtcInfo->mode != None)
                            {
                                for (int m = 0; m < resources->nmode; ++m)
                                {
                                    if (resources->modes[m].id == crtcInfo->mode)
                                    {
                                        // Calculate refresh rate
                                        if (resources->modes[m].hTotal > 0 && resources->modes[m].vTotal > 0)
                                        {
                                            const double refreshRate = static_cast<double>(resources->modes[m].dotClock) /
                                                                       (resources->modes[m].hTotal *
                                                                        resources->modes[m].vTotal);
                                            monitor.m_refreshRate = static_cast<unsigned int>(std::lround(refreshRate));
                                        }
                                        else
                                        {
                                            monitor.m_refreshRate = 60; // Default
                                        }
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                monitor.m_refreshRate = 60; // Default
                            }
                        }
                    }

                    // Set scaled resolution accounting for DPI
                    if (outputInfo->mm_width > 0 && outputInfo->mm_height > 0)
                    {
                        // DPI = (pixels * 25.4) / mm
                        const double dpiX = (monitor.m_resolution.x * 25.4) / static_cast<double>(outputInfo->mm_width);
                        const double dpiScale = dpiX / 96.0;
                        monitor.m_scaledResolution = Vector2u(static_cast<unsigned int>(monitor.m_resolution.x / dpiScale),
                                                              static_cast<unsigned int>(monitor.m_resolution.y / dpiScale));
                    }
                    else
                    {
                        monitor.m_scaledResolution = monitor.m_resolution; // No DPI info, use physical resolution
                    }

                    // Work area (X11 typically uses full resolution for now)
                    monitor.m_workArea = IntRect(monitor.m_position.x,
                                                 monitor.m_position.y,
                                                 static_cast<int>(monitor.m_resolution.x),
                                                 static_cast<int>(monitor.m_resolution.y));

                    // Check if this is the primary monitor
                    monitor.m_primary = (output == resources->outputs[0]); // First output is typically primary

                    monitors.push_back(monitor);
                }
            }
        }
    }

    // Fallback: if no monitors found, create a default one
    if (monitors.empty())
    {
        Monitor defaultMonitor;
        defaultMonitor.m_primary          = true;
        defaultMonitor.m_name             = "Default";
        defaultMonitor.m_identifier       = "Default";
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
    auto monitors = getAvailableMonitors();

    // First output is typically the primary
    if (!monitors.empty())
        return monitors[0];

    // Fallback
    Monitor defaultMonitor;
    defaultMonitor.m_primary          = true;
    defaultMonitor.m_name             = "Default";
    defaultMonitor.m_identifier       = "Default";
    defaultMonitor.m_resolution       = {1920, 1080};
    defaultMonitor.m_position         = {0, 0};
    defaultMonitor.m_refreshRate      = 60;
    defaultMonitor.m_scaledResolution = {1920, 1080};
    defaultMonitor.m_workArea         = IntRect(0, 0, 1920, 1080);

    return defaultMonitor;
}


////////////////////////////////////////////////////////////
std::vector<VideoMode> MonitorImpl::getAvailableVideoModesForMonitor(const String& monitorIdentifier)
{
    std::vector<VideoMode> modes;

    // Open a connection with the X server
    if (const auto display = openDisplay())
    {
        // Get the default screen
        const int    screen = DefaultScreen(display.get());
        const Window root   = RootWindow(display.get(), screen);

        // Get screen resources (outputs, crtcs, modes)
        if (const auto resources = X11Ptr<XRRScreenResources>(XRRGetScreenResources(display.get(), root)))
        {
            // Find the output matching the identifier
            for (int i = 0; i < resources->noutput; ++i)
            {
                const RROutput output = resources->outputs[i];

                // Get output information
                if (const auto outputInfo = X11Ptr<XRROutputInfo>(XRRGetOutputInfo(display.get(), resources.get(), output)))
                {
                    // Check if this is the monitor we're looking for
                    if (outputInfo->name && outputInfo->nameLen > 0)
                    {
                        const String outputName(outputInfo->name);
                        if (outputName == monitorIdentifier && outputInfo->connection == RR_Connected)
                        {
                            // Found the monitor, collect all available modes
                            for (int m = 0; m < outputInfo->nmode; ++m)
                            {
                                const RRMode modeId = outputInfo->modes[m];

                                // Find the mode details in the resources
                                for (int r = 0; r < resources->nmode; ++r)
                                {
                                    if (resources->modes[r].id == modeId)
                                    {
                                        const VideoMode mode(
                                            Vector2u(resources->modes[r].width, resources->modes[r].height));
                                        modes.push_back(mode);
                                        break;
                                    }
                                }
                            }

                            // Sort modes from best to worst
                            std::sort(modes.begin(), modes.end(), std::greater<>());
                            return modes;
                        }
                    }
                }
            }
        }
    }

    return modes;
}

} // namespace sf::priv

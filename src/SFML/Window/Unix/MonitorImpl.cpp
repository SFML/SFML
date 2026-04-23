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
#include <optional>


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
std::vector<VideoMode> MonitorImpl::getAvailableVideoModes() const
{
    std::vector<VideoMode> modes;

    if (const auto display = openDisplay())
    {
        const int    screen = DefaultScreen(display.get());
        const Window root   = RootWindow(display.get(), screen);

        if (const auto resources = X11Ptr<XRRScreenResources>(XRRGetScreenResources(display.get(), root)))
        {
            const std::string idStr = identifier.toAnsiString();

            // Find the output matching the identifier
            for (int i = 0; i < resources->noutput; ++i)
            {
                const RROutput output = resources->outputs[i];

                if (const auto outputInfo = X11Ptr<XRROutputInfo>(XRRGetOutputInfo(display.get(), resources.get(), output)))
                {
                    if (outputInfo->name && outputInfo->nameLen > 0)
                    {
                        const String outputName(outputInfo->name);
                        if (outputName.toAnsiString() == idStr && outputInfo->connection == RR_Connected)
                        {
                            // Found the monitor, collect all available modes
                            for (int m = 0; m < outputInfo->nmode; ++m)
                            {
                                const RRMode modeId = outputInfo->modes[m];

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


namespace
{
// Create MonitorImpl from output information
std::optional<MonitorImpl> getMonitorImplFromOutput(
    Display*            display,
    XRRScreenResources* resources,
    RROutput            output,
    XRROutputInfo*      outputInfo,
    bool                isPrimary)
{
    if (!outputInfo || outputInfo->connection != RR_Connected)
        return std::nullopt;

    if (!outputInfo->name || outputInfo->nameLen <= 0)
        return std::nullopt;

    MonitorImpl impl;

    // Get output name
    impl.name         = String(outputInfo->name);
    impl.identifier   = String(outputInfo->name);
    impl.primary      = isPrimary;
    impl.nativeHandle = output;

    // Get CRTC information if the output is active
    if (outputInfo->crtc != None)
    {
        if (const auto crtcInfo = X11Ptr<XRRCrtcInfo>(XRRGetCrtcInfo(display, resources, outputInfo->crtc)))
        {
            impl.position.x = static_cast<int>(crtcInfo->x);
            impl.position.y = static_cast<int>(crtcInfo->y);

            impl.resolution.x = crtcInfo->width;
            impl.resolution.y = crtcInfo->height;

            // Handle rotation
            if (crtcInfo->rotation == RR_Rotate_90 || crtcInfo->rotation == RR_Rotate_270)
                std::swap(impl.resolution.x, impl.resolution.y);

            // Get refresh rate from mode
            if (crtcInfo->mode != None)
            {
                for (int m = 0; m < resources->nmode; ++m)
                {
                    if (resources->modes[m].id == crtcInfo->mode)
                    {
                        if (resources->modes[m].hTotal > 0 && resources->modes[m].vTotal > 0)
                        {
                            const double refreshRate = static_cast<double>(resources->modes[m].dotClock) /
                                                       (resources->modes[m].hTotal * resources->modes[m].vTotal);
                            impl.refreshRate         = static_cast<unsigned int>(std::lround(refreshRate));
                        }
                        else
                        {
                            impl.refreshRate = 60;
                        }
                        break;
                    }
                }
            }
            else
            {
                impl.refreshRate = 60;
            }
        }
    }

    // Set scaled resolution accounting for DPI
    if (outputInfo->mm_width > 0 && outputInfo->mm_height > 0)
    {
        const double dpiX     = (impl.resolution.x * 25.4) / static_cast<double>(outputInfo->mm_width);
        const double dpiScale = dpiX / 96.0;
        impl.scaledResolution = Vector2u(static_cast<unsigned int>(impl.resolution.x / dpiScale),
                                         static_cast<unsigned int>(impl.resolution.y / dpiScale));
    }
    else
    {
        impl.scaledResolution = impl.resolution;
    }

    // Work area
    impl.workAreaPosition.x = impl.position.x;
    impl.workAreaPosition.y = impl.position.y;
    impl.workAreaSize.x     = static_cast<unsigned int>(impl.resolution.x);
    impl.workAreaSize.y     = static_cast<unsigned int>(impl.resolution.y);

    return impl;
}

// Fallback monitor used when XRandR is unavailable or reports nothing
MonitorImpl getDefaultMonitor()
{
    MonitorImpl defaultMonitor;
    defaultMonitor.primary          = true;
    defaultMonitor.name             = "Default";
    defaultMonitor.identifier       = "Default";
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

    if (const auto display = openDisplay())
    {
        const int    screen = DefaultScreen(display.get());
        const Window root   = RootWindow(display.get(), screen);

        if (const auto resources = X11Ptr<XRRScreenResources>(XRRGetScreenResources(display.get(), root)))
        {
            // Iterate through all outputs
            for (int i = 0; i < resources->noutput; ++i)
            {
                const RROutput output = resources->outputs[i];

                if (const auto outputInfo = X11Ptr<XRROutputInfo>(XRRGetOutputInfo(display.get(), resources.get(), output)))
                {
                    if (outputInfo->connection != RR_Connected)
                        continue;

                    const bool isPrimary = (i == 0); // First output is typically primary
                    if (auto impl = getMonitorImplFromOutput(display.get(), resources.get(), output, outputInfo.get(), isPrimary))
                    {
                        monitors.push_back(*impl);
                    }
                }
            }
        }
    }

    // Fallback
    if (monitors.empty())
        monitors.push_back(getDefaultMonitor());

    return monitors;
}


////////////////////////////////////////////////////////////
MonitorImpl MonitorImpl::getPrimary()
{
    const auto monitors = getAvailableMonitors();

    if (!monitors.empty())
        return monitors[0];

    return getDefaultMonitor();
}

} // namespace sf::priv

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
#include <SFML/Window/DRM/DRMContext.hpp>
#include <SFML/Window/Monitor.hpp>
#include <SFML/Window/MonitorImpl.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <SFML/System/Err.hpp>

#include <algorithm>
#include <array>
#include <fcntl.h>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <xf86drmMode.h>


namespace sf::priv
{
namespace
{
////////////////////////////////////////////////////////////
/// \brief Query the DRM device for available connectors and create Monitor objects
///
/// \return Vector of available monitors
///
////////////////////////////////////////////////////////////
std::vector<Monitor> queryDRMMonitors()
{
    std::vector<Monitor> monitors;

    // Open DRM device - typically /dev/dri/card0
    constexpr std::array<const char*, 3> drmDevices = {{
        "/dev/dri/card0",
        "/dev/dri/card1",
        "/dev/dri/card2",
    }};

    int fd = -1;
    for (const char* device : drmDevices)
    {
        fd = open(device, O_RDWR | O_CLOEXEC);
        if (fd >= 0)
            break;
    }

    if (fd < 0)
    {
        err() << "Could not open DRM device" << std::endl;
        // Return a default monitor if we can't open DRM
        Monitor defaultMonitor;
        defaultMonitor.m_primary          = true;
        defaultMonitor.m_name             = "DRM Display (Default)";
        defaultMonitor.m_identifier       = "drm_0";
        defaultMonitor.m_position         = {0, 0};
        defaultMonitor.m_resolution       = {1920, 1080};
        defaultMonitor.m_refreshRate      = 60;
        defaultMonitor.m_scaledResolution = {1920, 1080};
        defaultMonitor.m_workArea         = IntRect(0, 0, 1920, 1080);
        monitors.push_back(defaultMonitor);
        return monitors;
    }

    // Get the DRM resources (connectors, encoders, CRTCs, etc.)
    drmModeResPtr resources = drmModeGetResources(fd);
    if (!resources)
    {
        err() << "Could not get DRM resources" << std::endl;
        close(fd);
        return monitors;
    }

    bool foundPrimary = false;

    // Iterate through all connectors
    for (int i = 0; i < resources->count_connectors; ++i)
    {
        drmModeConnectorPtr connector = drmModeGetConnector(fd, resources->connectors[i]);
        if (!connector)
            continue;

        // Only process connected connectors with modes
        if (connector->connection == DRM_MODE_CONNECTED && connector->count_modes > 0)
        {
            Monitor monitor;

            // Set primary flag for the first connected monitor
            monitor.m_primary = !foundPrimary;
            foundPrimary      = true;

            // Create a human-readable name from connector type and index
            std::ostringstream nameStream;
            const char*        typeName = "Unknown";
            switch (connector->connector_type)
            {
                case DRM_MODE_CONNECTOR_VGA:
                    typeName = "VGA";
                    break;
                case DRM_MODE_CONNECTOR_DVII:
                    typeName = "DVI-I";
                    break;
                case DRM_MODE_CONNECTOR_DVID:
                    typeName = "DVI-D";
                    break;
                case DRM_MODE_CONNECTOR_DVIA:
                    typeName = "DVI-A";
                    break;
                case DRM_MODE_CONNECTOR_Composite:
                    typeName = "Composite";
                    break;
                case DRM_MODE_CONNECTOR_SVIDEO:
                    typeName = "S-Video";
                    break;
                case DRM_MODE_CONNECTOR_LVDS:
                    typeName = "LVDS";
                    break;
                case DRM_MODE_CONNECTOR_Component:
                    typeName = "Component";
                    break;
                case DRM_MODE_CONNECTOR_9PinDIN:
                    typeName = "9-Pin DIN";
                    break;
                case DRM_MODE_CONNECTOR_DisplayPort:
                    typeName = "DisplayPort";
                    break;
                case DRM_MODE_CONNECTOR_HDMIA:
                    typeName = "HDMI-A";
                    break;
                case DRM_MODE_CONNECTOR_HDMIB:
                    typeName = "HDMI-B";
                    break;
                case DRM_MODE_CONNECTOR_TV:
                    typeName = "TV";
                    break;
                case DRM_MODE_CONNECTOR_eDP:
                    typeName = "eDP";
                    break;
                case DRM_MODE_CONNECTOR_VIRTUAL:
                    typeName = "Virtual";
                    break;
                case DRM_MODE_CONNECTOR_DSI:
                    typeName = "DSI";
                    break;
            }
            nameStream << typeName << "-" << connector->connector_type_id;
            monitor.m_name = nameStream.str();

            // Create identifier string from connector ID
            std::ostringstream idStream;
            idStream << "drm_" << std::hex << std::setfill('0') << std::setw(8) << connector->connector_id;
            monitor.m_identifier = idStream.str();

            // Use the first mode (highest resolution) for monitor properties
            drmModeModeInfoPtr mode = &connector->modes[0];
            monitor.m_resolution.x  = mode->hdisplay;
            monitor.m_resolution.y  = mode->vdisplay;
            monitor.m_refreshRate   = mode->vrefresh ? mode->vrefresh : 60;

            // Get the CRTC to determine position
            monitor.m_position = {0, 0};
            if (connector->encoder_id)
            {
                drmModeEncoderPtr encoder = drmModeGetEncoder(fd, connector->encoder_id);
                if (encoder && encoder->crtc_id)
                {
                    drmModeCrtcPtr crtc = drmModeGetCrtc(fd, encoder->crtc_id);
                    if (crtc)
                    {
                        monitor.m_position.x = static_cast<int>(crtc->x);
                        monitor.m_position.y = static_cast<int>(crtc->y);
                        drmModeFreeCrtc(crtc);
                    }
                    drmModeFreeEncoder(encoder);
                }
            }

            // DRM typically doesn't have detailed DPI info readily available
            // In a real implementation, would read physical dimensions from EDID
            // For now, assume no scaling (1:1 logical to physical pixels)
            monitor.m_scaledResolution = Vector2u(monitor.m_resolution.x, monitor.m_resolution.y);

            // Work area (DRM typically uses full connector area)
            monitor.m_workArea = IntRect(monitor.m_position.x,
                                         monitor.m_position.y,
                                         static_cast<int>(monitor.m_resolution.x),
                                         static_cast<int>(monitor.m_resolution.y));

            monitors.push_back(monitor);
        }

        drmModeFreeConnector(connector);
    }

    drmModeFreeResources(resources);
    close(fd);

    // If no monitors were found, return a default one
    if (monitors.empty())
    {
        Monitor defaultMonitor;
        defaultMonitor.m_primary          = true;
        defaultMonitor.m_name             = "DRM Display (Default)";
        defaultMonitor.m_identifier       = "drm_0";
        defaultMonitor.m_position         = {0, 0};
        defaultMonitor.m_resolution       = {1920, 1080};
        defaultMonitor.m_refreshRate      = 60;
        defaultMonitor.m_scaledResolution = {1920, 1080};
        defaultMonitor.m_workArea         = IntRect(0, 0, 1920, 1080);
        monitors.push_back(defaultMonitor);
    }

    return monitors;
}
} // anonymous namespace


////////////////////////////////////////////////////////////
std::vector<Monitor> MonitorImpl::getAvailableMonitors()
{
    return queryDRMMonitors();
}


////////////////////////////////////////////////////////////
Monitor MonitorImpl::getPrimary()
{
    std::vector<Monitor> monitors = getAvailableMonitors();

    if (monitors.empty())
    {
        // Fallback to a default monitor
        Monitor monitor;
        monitor.m_primary          = true;
        monitor.m_name             = "DRM Display";
        monitor.m_identifier       = "drm_0";
        monitor.m_position         = {0, 0};
        monitor.m_resolution       = {1920, 1080};
        monitor.m_refreshRate      = 60;
        monitor.m_scaledResolution = {1920, 1080};
        monitor.m_workArea         = IntRect(0, 0, 1920, 1080);
        return monitor;
    }

    // Return the first connected monitor (or the one marked as primary)
    for (const auto& monitor : monitors)
    {
        if (monitor.m_primary)
            return monitor;
    }

    return monitors[0];
}


////////////////////////////////////////////////////////////
std::vector<VideoMode> MonitorImpl::getAvailableVideoModesForMonitor(const String& monitorIdentifier)
{
    std::vector<VideoMode> modes;

    // Open DRM device
    constexpr std::array<const char*, 3> drmDevices = {{
        "/dev/dri/card0",
        "/dev/dri/card1",
        "/dev/dri/card2",
    }};

    int fd = -1;
    for (const char* device : drmDevices)
    {
        fd = open(device, O_RDWR | O_CLOEXEC);
        if (fd >= 0)
            break;
    }

    if (fd < 0)
        return modes;

    // Get the DRM resources
    drmModeResPtr resources = drmModeGetResources(fd);
    if (!resources)
    {
        close(fd);
        return modes;
    }

    const std::string monitorIdStr = monitorIdentifier.toAnsiString();

    // Find the matching connector and collect its modes
    for (int i = 0; i < resources->count_connectors; ++i)
    {
        drmModeConnectorPtr connector = drmModeGetConnector(fd, resources->connectors[i]);
        if (!connector)
            continue;

        // Check if this is the connector we're looking for
        std::ostringstream idStream;
        idStream << "drm_" << std::hex << std::setfill('0') << std::setw(8) << connector->connector_id;

        if (idStream.str() == monitorIdStr && connector->connection == DRM_MODE_CONNECTED)
        {
            // Collect all available modes for this connector
            for (int m = 0; m < connector->count_modes; ++m)
            {
                drmModeModeInfoPtr modeInfo = &connector->modes[m];
                const VideoMode    mode(Vector2u(modeInfo->hdisplay, modeInfo->vdisplay));

                // Avoid duplicates
                if (std::find(modes.begin(), modes.end(), mode) == modes.end())
                {
                    modes.push_back(mode);
                }
            }

            drmModeFreeConnector(connector);
            break;
        }

        drmModeFreeConnector(connector);
    }

    drmModeFreeResources(resources);
    close(fd);

    // Sort modes from best to worst
    std::sort(modes.begin(), modes.end(), std::greater<>());

    return modes;
}

} // namespace sf::priv

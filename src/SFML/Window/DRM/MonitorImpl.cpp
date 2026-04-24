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
// 3. This notice may not be altered from this source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/DRM/DRMContext.hpp>
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
////////////////////////////////////////////////////////////
MonitorImpl::MonitorImpl() = default;


////////////////////////////////////////////////////////////
MonitorImpl::~MonitorImpl() = default;


////////////////////////////////////////////////////////////
std::vector<VideoMode> MonitorImpl::getAvailableVideoModes() const
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

    drmModeResPtr resources = drmModeGetResources(fd);
    if (!resources)
    {
        close(fd);
        return modes;
    }

    const std::string monitorIdStr = m_identifier.toAnsiString();

    // Find the matching connector and collect its modes
    for (int i = 0; i < resources->count_connectors; ++i)
    {
        drmModeConnectorPtr connector = drmModeGetConnector(fd, resources->connectors[i]);
        if (!connector)
            continue;

        std::ostringstream idStream;
        idStream << "drm_" << std::hex << std::setfill('0') << std::setw(8) << connector->connector_id;

        if (idStream.str() == monitorIdStr && connector->connection == DRM_MODE_CONNECTED)
        {
            for (int m = 0; m < connector->count_modes; ++m)
            {
                drmModeModeInfoPtr modeInfo = &connector->modes[m];
                const VideoMode    mode(Vector2u(modeInfo->hdisplay, modeInfo->vdisplay));

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

    std::sort(modes.begin(), modes.end(), std::greater<>());

    return modes;
}


namespace
{
// Build MonitorImpl from a DRM connector
std::shared_ptr<MonitorImpl> getMonitorImplFromConnector(int fd, drmModeConnectorPtr connector, bool isPrimary)
{
    auto impl = std::make_shared<MonitorImpl>();

    if (!connector || connector->connection != DRM_MODE_CONNECTED || connector->count_modes == 0)
        return nullptr;

    impl->m_primary = isPrimary;

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
    impl->m_name = String(nameStream.str().c_str());

    std::ostringstream idStream;
    idStream << "drm_" << std::hex << std::setfill('0') << std::setw(8) << connector->connector_id;
    impl->m_identifier = String(idStream.str().c_str());

    // Use the first mode (highest resolution) for monitor properties
    drmModeModeInfoPtr mode = &connector->modes[0];
    impl->m_resolution.x    = mode->hdisplay;
    impl->m_resolution.y    = mode->vdisplay;
    impl->m_refreshRate     = mode->vrefresh ? mode->vrefresh : 60;

    // Get the CRTC to determine position
    impl->m_position = {0, 0};
    if (connector->encoder_id)
    {
        drmModeEncoderPtr encoder = drmModeGetEncoder(fd, connector->encoder_id);
        if (encoder && encoder->crtc_id)
        {
            drmModeCrtcPtr crtc = drmModeGetCrtc(fd, encoder->crtc_id);
            if (crtc)
            {
                impl->m_position.x = static_cast<int>(crtc->x);
                impl->m_position.y = static_cast<int>(crtc->y);
                drmModeFreeCrtc(crtc);
            }
            drmModeFreeEncoder(encoder);
        }
    }

    // DRM typically doesn't have detailed DPI info readily available
    impl->m_scaledResolution = Vector2u(impl->m_resolution.x, impl->m_resolution.y);

    // Work area (DRM typically uses full connector area)
    impl->m_workAreaPosition = {impl->m_position.x, impl->m_position.y};
    impl->m_workAreaSize     = {static_cast<unsigned int>(impl->m_resolution.x),
                                static_cast<unsigned int>(impl->m_resolution.y)};

    return impl;
}

// Query the DRM device for available connectors
std::vector<std::shared_ptr<MonitorImpl>> queryDRMMonitors()
{
    std::vector<std::shared_ptr<MonitorImpl>> monitors;

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
        auto defaultMonitor                = std::make_shared<MonitorImpl>();
        defaultMonitor->m_primary          = true;
        defaultMonitor->m_name             = "DRM Display (Default)";
        defaultMonitor->m_identifier       = "drm_0";
        defaultMonitor->m_position         = {0, 0};
        defaultMonitor->m_resolution       = {1920, 1080};
        defaultMonitor->m_refreshRate      = 60;
        defaultMonitor->m_scaledResolution = {1920, 1080};
        defaultMonitor->m_workAreaPosition = {0, 0};
        defaultMonitor->m_workAreaSize     = {1920, 1080};
        monitors.push_back(defaultMonitor);
        return monitors;
    }

    drmModeResPtr resources = drmModeGetResources(fd);
    if (!resources)
    {
        err() << "Could not get DRM resources" << std::endl;
        close(fd);
        auto defaultMonitor                = std::make_shared<MonitorImpl>();
        defaultMonitor->m_primary          = true;
        defaultMonitor->m_name             = "DRM Display (Default)";
        defaultMonitor->m_identifier       = "drm_0";
        defaultMonitor->m_position         = {0, 0};
        defaultMonitor->m_resolution       = {1920, 1080};
        defaultMonitor->m_refreshRate      = 60;
        defaultMonitor->m_scaledResolution = {1920, 1080};
        defaultMonitor->m_workAreaPosition = {0, 0};
        defaultMonitor->m_workAreaSize     = {1920, 1080};
        monitors.push_back(defaultMonitor);
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
            const bool isPrimary = !foundPrimary;
            if (auto impl = getMonitorImplFromConnector(fd, connector, isPrimary))
            {
                monitors.push_back(impl);
                foundPrimary = true;
            }
        }

        drmModeFreeConnector(connector);
    }

    drmModeFreeResources(resources);
    close(fd);

    // If no monitors were found, return a default one
    if (monitors.empty())
    {
        auto defaultMonitor                = std::make_shared<MonitorImpl>();
        defaultMonitor->m_primary          = true;
        defaultMonitor->m_name             = "DRM Display (Default)";
        defaultMonitor->m_identifier       = "drm_0";
        defaultMonitor->m_position         = {0, 0};
        defaultMonitor->m_resolution       = {1920, 1080};
        defaultMonitor->m_refreshRate      = 60;
        defaultMonitor->m_scaledResolution = {1920, 1080};
        defaultMonitor->m_workAreaPosition = {0, 0};
        defaultMonitor->m_workAreaSize     = {1920, 1080};
        monitors.push_back(defaultMonitor);
    }

    return monitors;
}
} // namespace


////////////////////////////////////////////////////////////
std::vector<std::shared_ptr<MonitorImpl>> MonitorImpl::getAvailableMonitors()
{
    return queryDRMMonitors();
}


////////////////////////////////////////////////////////////
std::shared_ptr<MonitorImpl> MonitorImpl::getPrimary()
{
    const auto monitors = getAvailableMonitors();

    if (monitors.empty())
    {
        auto defaultMonitor                = std::make_shared<MonitorImpl>();
        defaultMonitor->m_primary          = true;
        defaultMonitor->m_name             = "DRM Display";
        defaultMonitor->m_identifier       = "drm_0";
        defaultMonitor->m_position         = {0, 0};
        defaultMonitor->m_resolution       = {1920, 1080};
        defaultMonitor->m_refreshRate      = 60;
        defaultMonitor->m_scaledResolution = {1920, 1080};
        defaultMonitor->m_workAreaPosition = {0, 0};
        defaultMonitor->m_workAreaSize     = {1920, 1080};
        return defaultMonitor;
    }

    // Return the first connected monitor
    return monitors[0];
}

} // namespace sf::priv

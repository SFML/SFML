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

#include <SFML/System/Win32/WindowsHeader.hpp>

#include <algorithm>
#include <iomanip>
#include <sstream>

// For GetDpiForMonitor
#ifndef MONITOR_DPI_TYPE
#define MONITOR_DPI_TYPE  INT
#define MDT_EFFECTIVE_DPI 0
#endif


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
    const std::string deviceNameStr = m_identifier.toAnsiString();
    const char*       deviceName    = deviceNameStr.c_str();

    // Enumerate all display modes for this monitor
    DEVMODE devMode;
    devMode.dmSize        = sizeof(DEVMODE);
    devMode.dmDriverExtra = 0;

    for (DWORD modeIndex = 0; EnumDisplaySettingsEx(reinterpret_cast<LPCWSTR>(deviceName), modeIndex, &devMode, 0);
         ++modeIndex)
    {
        // Only consider 32-bit color modes
        if (devMode.dmBitsPerPel == 32)
        {
            const VideoMode mode(Vector2u(devMode.dmPelsWidth, devMode.dmPelsHeight), devMode.dmBitsPerPel);

            // Avoid duplicates
            if (std::find(modes.begin(), modes.end(), mode) == modes.end())
            {
                modes.push_back(mode);
            }
        }
    }

    // Sort modes from best to worst
    std::sort(modes.begin(), modes.end(), std::greater<>());

    return modes;
}


namespace
{
// Helper function to get monitor data from HMONITOR handle
std::shared_ptr<MonitorImpl> getMonitorImplFromHandle(HMONITOR hMonitor)
{
    auto impl = std::make_shared<MonitorImpl>();

    MONITORINFOEX monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFOEX);

    if (!GetMonitorInfo(hMonitor, &monitorInfo))
        return nullptr;

    // Store identifier from device name
    impl->m_identifier = reinterpret_cast<const char*>(monitorInfo.szDevice);
    impl->m_name       = impl->m_identifier;
    impl->m_primary    = (monitorInfo.dwFlags & MONITORINFOF_PRIMARY) != 0;

    // Get position and resolution from monitor bounds
    impl->m_position.x   = monitorInfo.rcMonitor.left;
    impl->m_position.y   = monitorInfo.rcMonitor.top;
    impl->m_resolution.x = static_cast<unsigned int>(monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left);
    impl->m_resolution.y = static_cast<unsigned int>(monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top);

    // Get work area (usable area excluding taskbars, etc.)
    impl->m_workAreaPosition.x = monitorInfo.rcWork.left;
    impl->m_workAreaPosition.y = monitorInfo.rcWork.top;
    impl->m_workAreaSize.x     = static_cast<unsigned int>(monitorInfo.rcWork.right - monitorInfo.rcWork.left);
    impl->m_workAreaSize.y     = static_cast<unsigned int>(monitorInfo.rcWork.bottom - monitorInfo.rcWork.top);

    // Get refresh rate using EnumDisplaySettingsEx
    DEVMODE devMode;
    devMode.dmSize        = sizeof(DEVMODE);
    devMode.dmDriverExtra = 0;

    if (EnumDisplaySettingsEx(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &devMode, 0))
    {
        impl->m_refreshRate = devMode.dmDisplayFrequency;
    }
    else
    {
        impl->m_refreshRate = 60; // Default fallback
    }

    // Get DPI using GetDpiForMonitor if available
    UINT dpiX = 96; // Default DPI
    UINT dpiY = 96; // Default DPI

    // Try to use GetDpiForMonitor (Windows 8.1+)
    using GetDpiForMonitorFunc = HRESULT(WINAPI*)(HMONITOR, MONITOR_DPI_TYPE, UINT*, UINT*);

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
#pragma warning(disable : 4191) // unsafe conversion from FARPROC
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-function-type-mismatch"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
    static auto getDpiForMonitor = reinterpret_cast<GetDpiForMonitorFunc>(
        GetProcAddress(GetModuleHandle(TEXT("shcore.dll")), "GetDpiForMonitor"));
#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#elif defined(__clang__)
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif

    if (getDpiForMonitor)
    {
        getDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
    }

    // Store DPI as a scale factor (DPI / 96)
    const float dpiScale     = static_cast<float>(dpiX) / 96.0f;
    impl->m_scaledResolution = Vector2u(static_cast<unsigned int>(static_cast<float>(impl->m_resolution.x) / dpiScale),
                                        static_cast<unsigned int>(static_cast<float>(impl->m_resolution.y) / dpiScale));

    return impl;
}

// Helper structure to collect monitors during enumeration
struct MonitorEnumData
{
    std::vector<std::shared_ptr<MonitorImpl>> monitors;
};

// Callback function for EnumDisplayMonitors
BOOL CALLBACK monitorEnumProc(HMONITOR hMonitor, HDC, LPRECT, LPARAM lParam)
{
    auto* pData = reinterpret_cast<MonitorEnumData*>(lParam);

    if (auto impl = getMonitorImplFromHandle(hMonitor))
    {
        pData->monitors.push_back(impl);
    }

    return TRUE; // Continue enumeration
}
} // namespace


////////////////////////////////////////////////////////////
std::vector<std::shared_ptr<MonitorImpl>> MonitorImpl::getAvailableMonitors()
{
    MonitorEnumData data;

    // Enumerate all monitors
    EnumDisplayMonitors(nullptr, nullptr, monitorEnumProc, reinterpret_cast<LPARAM>(&data));

    return data.monitors;
}


////////////////////////////////////////////////////////////
std::shared_ptr<MonitorImpl> MonitorImpl::getPrimary()
{
    const auto monitors = getAvailableMonitors();

    // Find and return the primary monitor
    for (const auto& monitor : monitors)
    {
        if (monitor->isPrimary())
            return monitor;
    }

    // Fallback: return first monitor if no primary found
    if (!monitors.empty())
        return monitors[0];

    // Fallback: create a default monitor
    auto defaultMonitor                = std::make_shared<MonitorImpl>();
    defaultMonitor->m_primary          = true;
    defaultMonitor->m_name             = "Primary Display";
    defaultMonitor->m_identifier       = R"(\\?\DISPLAY1)";
    defaultMonitor->m_resolution       = {1920, 1080};
    defaultMonitor->m_refreshRate      = 60;
    defaultMonitor->m_scaledResolution = {1920, 1080};
    defaultMonitor->m_workAreaPosition = {0, 0};
    defaultMonitor->m_workAreaSize     = {1920, 1080};

    return defaultMonitor;
}

} // namespace sf::priv

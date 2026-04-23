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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/MonitorHandle.hpp>

#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>


namespace sf
{
class VideoMode;
} // namespace sf

namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief OS-specific monitor enumeration and data snapshot
///
/// MonitorImpl is a plain value type populated by each platform
/// backend and consumed by sf::Monitor, which copies its fields
/// into its own value representation.
///
////////////////////////////////////////////////////////////
class MonitorImpl
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Get all connected monitors
    ///
    /// \return Vector of all connected monitors
    ///
    ////////////////////////////////////////////////////////////
    static std::vector<MonitorImpl> getAvailableMonitors();

    ////////////////////////////////////////////////////////////
    /// \brief Get the primary monitor
    ///
    /// \return Primary monitor
    ///
    ////////////////////////////////////////////////////////////
    static MonitorImpl getPrimary();

    ////////////////////////////////////////////////////////////
    /// \brief Get available video modes for this monitor
    ///
    /// \return Vector of available video modes for the monitor
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::vector<VideoMode> getAvailableVideoModes() const;

    ////////////////////////////////////////////////////////////
    // Member data - public for platform implementations to populate
    ////////////////////////////////////////////////////////////
    String        name;
    String        identifier;
    bool          primary = false;
    Vector2i      position;
    Vector2u      resolution;
    unsigned int  refreshRate = 0;
    Vector2u      scaledResolution;
    Vector2i      workAreaPosition;
    Vector2u      workAreaSize;
    MonitorHandle nativeHandle{}; // OS-specific handle for this monitor, e.g. HMONITOR/RROutput/CGDirectDisplayID
};

} // namespace sf::priv

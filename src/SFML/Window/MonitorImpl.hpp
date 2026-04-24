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
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>

#include <memory>
#include <vector>


namespace sf
{
struct IntRect;
class VideoMode;
} // namespace sf

namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief OS-specific implementation of monitor
///
/// MonitorImpl is the pImpl (pointer to implementation) for Monitor.
/// Each MonitorImpl instance represents a specific monitor and holds
/// all of its properties.
///
////////////////////////////////////////////////////////////
class MonitorImpl
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ////////////////////////////////////////////////////////////
    MonitorImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~MonitorImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy constructor
    ///
    ////////////////////////////////////////////////////////////
    MonitorImpl(const MonitorImpl&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy assignment
    ///
    ////////////////////////////////////////////////////////////
    MonitorImpl& operator=(const MonitorImpl&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Get all connected monitors
    ///
    /// \return Vector of all connected monitors
    ///
    ////////////////////////////////////////////////////////////
    static std::vector<std::shared_ptr<MonitorImpl>> getAvailableMonitors();

    ////////////////////////////////////////////////////////////
    /// \brief Get the primary monitor
    ///
    /// \return Primary monitor
    ///
    ////////////////////////////////////////////////////////////
    static std::shared_ptr<MonitorImpl> getPrimary();

    ////////////////////////////////////////////////////////////
    /// \brief Get available video modes for this monitor
    ///
    /// \return Vector of available video modes for the monitor
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::vector<VideoMode> getAvailableVideoModes() const;

    // Accessor methods
    [[nodiscard]] String getName() const
    {
        return m_name;
    }

    [[nodiscard]] String getIdentifier() const
    {
        return m_identifier;
    }

    [[nodiscard]] bool isPrimary() const
    {
        return m_primary;
    }

    [[nodiscard]] Vector2i getPosition() const
    {
        return m_position;
    }

    [[nodiscard]] Vector2u getResolution() const
    {
        return m_resolution;
    }

    [[nodiscard]] unsigned int getRefreshRate() const
    {
        return m_refreshRate;
    }

    [[nodiscard]] Vector2u getScaledResolution() const
    {
        return m_scaledResolution;
    }

    [[nodiscard]] Vector2i getWorkAreaPosition() const;
    [[nodiscard]] Vector2u getWorkAreaSize() const;

public:
    // Member data - public for platform implementations to populate
    String       m_name;
    String       m_identifier;
    bool         m_primary = false;
    Vector2i     m_position;
    Vector2u     m_resolution;
    unsigned int m_refreshRate = 0;
    Vector2u     m_scaledResolution;
    Vector2i     m_workAreaPosition;
    Vector2u     m_workAreaSize;
};

} // namespace sf::priv

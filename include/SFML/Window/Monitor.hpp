////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2026 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
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
// 3. This notice may not be removed or altered from source distribution.
//
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Export.hpp>

#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Simple integer rectangle for work area representation
///
////////////////////////////////////////////////////////////
struct IntRect
{
    int left   = 0; //!< Left coordinate of the rectangle
    int top    = 0; //!< Top coordinate of the rectangle
    int width  = 0; //!< Width of the rectangle
    int height = 0; //!< Height of the rectangle

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    constexpr IntRect() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Constructor with parameters
    ///
    ////////////////////////////////////////////////////////////
    constexpr IntRect(int l, int t, int w, int h) : left(l), top(t), width(w), height(h)
    {
    }
};
////////////////////////////////////////////////////////////
// Forward declarations
////////////////////////////////////////////////////////////
class VideoMode;

namespace priv
{
class MonitorImpl;
}

////////////////////////////////////////////////////////////
/// \brief Monitor represents a connected display
///
////////////////////////////////////////////////////////////
class SFML_WINDOW_API Monitor
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor initializes all members to default values.
    ///
    ////////////////////////////////////////////////////////////
    Monitor() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Get the name/identifier of the monitor
    ///
    /// \return Monitor name (platform-dependent, e.g., "HDMI-1", "DP-2")
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] String getName() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a unique identifier for the monitor
    ///
    /// \return Unique identifier string (platform-specific)
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] String getIdentifier() const;

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether this is the primary monitor
    ///
    /// \return `true` if this is the primary monitor
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool isPrimary() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the position of the monitor in the virtual desktop
    ///
    /// \return Position of the top-left corner in pixels
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Vector2i getPosition() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current resolution of the monitor
    ///
    /// \return Current resolution in pixels
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Vector2u getResolution() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the scaled resolution of the monitor
    ///
    /// This accounts for DPI scaling and returns the actual number of
    /// logical pixels available on the display.
    ///
    /// \return Scaled resolution in logical pixels
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Vector2u getScaledResolution() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the work area of the monitor
    ///
    /// The work area is the usable portion of the monitor, excluding
    /// taskbars, menu bars, or other system UI elements.
    ///
    /// \return Work area as a rectangle (position and size in pixels)
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] IntRect getWorkArea() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current refresh rate of the monitor
    ///
    /// \return Refresh rate in Hz
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] unsigned int getRefreshRate() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the desktop video mode (current resolution)
    ///
    /// \return Desktop video mode for this monitor
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] VideoMode getDesktopVideoMode() const;

    ////////////////////////////////////////////////////////////
    /// \brief Retrieve all video modes supported by this monitor
    ///
    /// The returned array is sorted from best to worst.
    ///
    /// \return Array containing all supported video modes
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::vector<VideoMode> getAvailableVideoModes() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the primary monitor
    ///
    /// \return Primary monitor
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static Monitor getPrimary();

    ////////////////////////////////////////////////////////////
    /// \brief Retrieve all available monitors
    ///
    /// \return Array of all connected monitors
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static std::vector<Monitor> getAvailableMonitors();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    // NOLINTBEGIN(readability-identifier-naming)
    String       m_name;             //!< Monitor name/identifier
    String       m_identifier;       //!< Unique monitor identifier
    bool         m_primary = false;  //!< Whether this is primary monitor
    Vector2i     m_position;         //!< Top-left position in virtual desktop
    Vector2u     m_resolution;       //!< Current resolution in pixels
    unsigned int m_refreshRate = 0;  //!< Refresh rate in Hz
    Vector2u     m_scaledResolution; //!< Scaled resolution accounting for DPI scaling
    IntRect      m_workArea;         //!< Work area (usable area excluding taskbars, etc.)
    // NOLINTEND(readability-identifier-naming)
};

////////////////////////////////////////////////////////////
/// \relates Monitor
/// \brief Overload of `operator==` to compare two monitors
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return `true` if monitors are equal
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API bool operator==(const Monitor& left, const Monitor& right);

////////////////////////////////////////////////////////////
/// \relates Monitor
/// \brief Overload of `operator!=` to compare two monitors
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return `true` if monitors are different
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API bool operator!=(const Monitor& left, const Monitor& right);

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::Monitor
/// \ingroup window
///
/// sf::Monitor represents a connected display device.
/// It provides information about the monitor's position,
/// resolution, refresh rate, and available video modes.
///
/// The main usage of sf::Monitor is to:
/// - Enumerate available displays in a multi-monitor setup
/// - Get monitor-specific information
/// - Create windows on specific monitors
/// - Query the monitor of an existing window
///
/// Usage example:
/// \code
/// // List all connected monitors
/// std::vector<sf::Monitor> monitors = sf::Monitor::getAvailableMonitors();
/// for (const auto& monitor : monitors)
/// {
///     std::cout << monitor.getName() << ": "
///               << monitor.getResolution().x << "x"
///               << monitor.getResolution().y << " @ "
///               << monitor.getRefreshRate() << "Hz"
///               << (monitor.isPrimary() ? " (primary)" : "") << "\n";
/// }
///
/// // Create a window on a specific monitor
/// sf::Monitor targetMonitor = sf::Monitor::getPrimary();
/// sf::VideoMode mode(1920, 1080);
/// sf::RenderWindow window(mode, "SFML Window", sf::Style::Default,
///                         sf::ContextSettings(), targetMonitor);
///
/// // Query which monitor a window is on
/// sf::Monitor windowMonitor = window.getMonitor();
/// \endcode
///
////////////////////////////////////////////////////////////

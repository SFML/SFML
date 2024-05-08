////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Export.hpp>

#include <SFML/System/Vector2.hpp>

#include <vector>
#include <memory>


namespace sf
{
class VideoMode;

namespace priv
{
class MonitorImpl;
}

////////////////////////////////////////////////////////////
/// \brief Monitor represents a monitor made available by the OS
///
////////////////////////////////////////////////////////////
class SFML_WINDOW_API Monitor
{
    ////////////////////////////////////////////////////////////
    /// \brief Construct monitor with a MonitorImpl pointer
    ///
    ////////////////////////////////////////////////////////////
    Monitor(std::unique_ptr<priv::MonitorImpl>&& impl);

public:
    ~Monitor();

    ////////////////////////////////////////////////////////////
    /// \brief Get the primary monitor
    ///
    /// \return Primary monitor
    ///
    ////////////////////////////////////////////////////////////
    static Monitor getPrimaryMonitor();

    ////////////////////////////////////////////////////////////
    /// \brief Retrieve all the fullscreen video modes this monitor supports
    ///
    /// When creating a fullscreen window, the video mode is restricted
    /// to be compatible with what the graphics driver and monitor
    /// support. This function returns the complete list of all video
    /// modes that can be used in fullscreen mode.
    /// The returned array is sorted from best to worst, so that
    /// the first element will always give the best mode (higher
    /// width, height and bits-per-pixel).
    ///
    /// \return Array containing all  the fullscreen video modes this monitor supports
    ///
    ////////////////////////////////////////////////////////////
    const std::vector<VideoMode>& getFullscreenModes();

    ////////////////////////////////////////////////////////////
    /// \brief Get the current desktop video mode of this monitor
    ///
    /// \return Current desktop video mode of this monitor
    ///
    ////////////////////////////////////////////////////////////
    VideoMode getDesktopMode();

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the video mode is valid on this monitor
    ///
    /// The validity of video modes is only relevant when using
    /// fullscreen windows; otherwise any video mode can be used
    /// with no restriction.
    ///
    /// \return True if the video mode is valid for fullscreen mode on this monitor
    ///
    ////////////////////////////////////////////////////////////
    bool isValid(const VideoMode& mode);

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    const std::unique_ptr<priv::MonitorImpl> m_impl; //!< Platform-specific implementation of the monitor
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::Monitor
/// \ingroup window
///
/// A monitor is an abstraction to represent a display.
///
/// A monitor object of the primary can be obtained using
/// the static function getPrimaryMonitor().
///
/// sf::Monitor provides a function for retrieving the list of
/// all the video modes supported by that monitor and the system:
/// getFullscreenModes().
///
/// A custom video mode can also be checked directly for
/// fullscreen compatibility with its isValid(sf::VideoMode) function.
///
/// Additionally, sf::Monitor provides a function to get the mode
/// currently used by the desktop for that monitor: getDesktopMode().
/// This allows to build windows with the same size or pixel
/// depth as the current resolution.
///
/// Usage example:
/// \code
/// // Display the list of all the video modes available for fullscreen
/// std::vector<sf::VideoMode> modes = sf::Monitor::getPrimaryMonitor().getFullscreenModes();
/// for (std::size_t i = 0; i < modes.size(); ++i)
/// {
///     sf::VideoMode mode = modes[i];
///     std::cout << "Mode #" << i << ": "
///               << mode.width << "x" << mode.height << " - "
///               << mode.bitsPerPixel << " bpp" << std::endl;
/// }
///
/// // Create a window with the same pixel depth as the desktop
/// sf::VideoMode desktop = sf::Monitor::getPrimaryMonitor().getDesktopMode();
/// window.create(sf::VideoMode({1024, 768}, desktop.bitsPerPixel), "SFML window");
/// \endcode
///
////////////////////////////////////////////////////////////

///
/// Usage example:
/// \code
/// // Display the list of all the video modes available for fullscreen for the primary monitor
/// sf::Monitor monitor = sf::Monitor::getPrimaryMonitor();
///
/// std::vector<sf::VideoMode> modes = monitor.getFullscreenModes();
/// for (std::size_t i = 0; i < modes.size(); ++i)
/// {
///     sf::VideoMode mode = modes[i];
///     std::cout << "Mode #" << i << ": "
///               << mode.width << "x" << mode.height << " - "
///               << mode.bitsPerPixel << " bpp" << std::endl;
/// }
///
/// // Create a window with the same pixel depth as the desktop
/// sf::VideoMode desktopMode = monitor.getDesktopMode();
/// window.create(sf::VideoMode(desktopMode, "SFML window");
/// \endcode
///
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_VIDEOMODE_HPP
#define SFML_VIDEOMODE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <cstdlib>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief VideoMode defines a video mode (width, height, bpp)
///
////////////////////////////////////////////////////////////
class SFML_API VideoMode
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructors initializes all members to 0.
    ///
    ////////////////////////////////////////////////////////////
    VideoMode();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the video mode with its attributes
    ///
    /// \param width        Width in pixels
    /// \param height       Height in pixels
    /// \param bitsPerPixel Pixel depths in bits per pixel
    ///
    ////////////////////////////////////////////////////////////
    VideoMode(unsigned int width, unsigned int height, unsigned int bitsPerPixel = 32);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current desktop video mode
    ///
    /// \return Current desktop video mode
    ///
    ////////////////////////////////////////////////////////////
    static VideoMode GetDesktopMode();

    ////////////////////////////////////////////////////////////
    /// \brief Get a valid video mode
    ///
    /// The parameter \a index must be in the range [0, GetModesCount()[.
    /// Modes are sorted from best to worst, so that
    /// sf::VideoMode::GetMode(0) will always give the best.
    ///
    /// \param index Index of video mode to get
    ///
    /// \return Corresponding video mode (invalid mode if index is out of range)
    ///
    ////////////////////////////////////////////////////////////
    static VideoMode GetMode(std::size_t index);

    ////////////////////////////////////////////////////////////
    /// \brief Get the total number of valid video modes available
    ///
    /// The validity of video modes is only relevant when using
    /// fullscreen windows; otherwise any video mode can be used
    /// with no restriction.
    ///
    /// \return Number of valid video modes available for fullscreen mode
    ///
    ////////////////////////////////////////////////////////////
    static std::size_t GetModesCount();

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the video mode is valid
    ///
    /// The validity of video modes is only relevant when using
    /// fullscreen windows; otherwise any video mode can be used
    /// with no restriction.
    ///
    /// \return True if the video mode is valid for fullscreen mode
    ///
    ////////////////////////////////////////////////////////////
    bool IsValid() const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int Width;        ///< Video mode width, in pixels
    unsigned int Height;       ///< Video mode height, in pixels
    unsigned int BitsPerPixel; ///< Video mode pixel depth, in bits per pixels
};

////////////////////////////////////////////////////////////
/// \brief Overload of binary == operator
///
/// This operator compares strict equality between two modes,
/// i.e. it checks if all their members are equal.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return True if modes are equal
///
////////////////////////////////////////////////////////////
SFML_API bool operator ==(const VideoMode& left, const VideoMode& right);

////////////////////////////////////////////////////////////
/// \brief Overload of binary != operator
///
/// This operator compares strict difference between two modes,
/// i.e. it checks if not all their members are equal.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return True if modes are different
///
////////////////////////////////////////////////////////////
SFML_API bool operator !=(const VideoMode& left, const VideoMode& right);

} // namespace sf


#endif // SFML_VIDEOMODE_HPP


////////////////////////////////////////////////////////////
/// \class sf::VideoMode
///
/// A video mode is defined by a width and a height (in pixels)
/// and a depth (in bits per pixel). Video modes are used to
/// setup windows (sf::Window) at creation time.
///
/// The main usage of video modes is for fullscreen mode:
/// indeed you must use one of the valid video modes
/// allowed by the OS (which are defined by what the monitor
/// and the graphics card support), otherwise your window
/// creation will just fail.
///
/// sf::VideoMode provides two static functions for retrieving
/// the list of all the video modes supported by the system:
/// GetModesCount() to get the number of video modes in the
/// list, and GetMode() to retrieve a particular mode
/// from the list.
///
/// A custom video mode can also be checked directly with
/// its IsValid() function.
///
/// Additionnally, sf::VideoMode provides a static function
/// to get the mode currently used by the desktop: GetDesktopMode().
/// This allows to build windows with the same size or pixel
/// depth as the current resolution.
///
/// Usage example:
/// \code
/// // Display the list of all the video modes available for fullscreen
/// for (std::size_t i = 0; i < sf::VideoMode::GetModesCount(); ++i)
/// {
///     sf::VideoMode mode = sf::VideoMode::GetMode(i);
///     std::cout << "Mode #" << i << ": "
///               << mode.Width << "x" << mode.Height << " - "
///               << mode.BitsPerPixel << " bpp" << std::endl;
/// }
///
/// // Create a window with the same pixel depth as the desktop
/// sf::VideoMode desktop = sf::VideoMode::GetDesktopMode();
/// window.Create(sf::VideoMode(1024, 768, desktop.BitsPerPixel), "SFML window");
/// \endcode
///
////////////////////////////////////////////////////////////

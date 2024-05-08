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


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief VideoMode defines a video mode (width, height, bpp)
///
////////////////////////////////////////////////////////////
class SFML_WINDOW_API VideoMode
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructors initializes all members to 0.
    ///
    ////////////////////////////////////////////////////////////
    VideoMode() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Construct the video mode with its attributes
    ///
    /// \param modeSize         Width and height in pixels
    /// \param modeBitsPerPixel Pixel depths in bits per pixel
    ///
    ////////////////////////////////////////////////////////////
    explicit VideoMode(const Vector2u& modeSize, unsigned int modeBitsPerPixel = 32);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Vector2u     size;           //!< Video mode width and height, in pixels
    unsigned int bitsPerPixel{}; //!< Video mode pixel depth, in bits per pixels
};

////////////////////////////////////////////////////////////
/// \relates VideoMode
/// \brief Overload of == operator to compare two video modes
///
/// \param left  Left operand (a video mode)
/// \param right Right operand (a video mode)
///
/// \return True if modes are equal
///
////////////////////////////////////////////////////////////
SFML_WINDOW_API bool operator==(const VideoMode& left, const VideoMode& right);

////////////////////////////////////////////////////////////
/// \relates VideoMode
/// \brief Overload of != operator to compare two video modes
///
/// \param left  Left operand (a video mode)
/// \param right Right operand (a video mode)
///
/// \return True if modes are different
///
////////////////////////////////////////////////////////////
SFML_WINDOW_API bool operator!=(const VideoMode& left, const VideoMode& right);

////////////////////////////////////////////////////////////
/// \relates VideoMode
/// \brief Overload of < operator to compare video modes
///
/// \param left  Left operand (a video mode)
/// \param right Right operand (a video mode)
///
/// \return True if \a left is lesser than \a right
///
////////////////////////////////////////////////////////////
SFML_WINDOW_API bool operator<(const VideoMode& left, const VideoMode& right);

////////////////////////////////////////////////////////////
/// \relates VideoMode
/// \brief Overload of > operator to compare video modes
///
/// \param left  Left operand (a video mode)
/// \param right Right operand (a video mode)
///
/// \return True if \a left is greater than \a right
///
////////////////////////////////////////////////////////////
SFML_WINDOW_API bool operator>(const VideoMode& left, const VideoMode& right);

////////////////////////////////////////////////////////////
/// \relates VideoMode
/// \brief Overload of <= operator to compare video modes
///
/// \param left  Left operand (a video mode)
/// \param right Right operand (a video mode)
///
/// \return True if \a left is lesser or equal than \a right
///
////////////////////////////////////////////////////////////
SFML_WINDOW_API bool operator<=(const VideoMode& left, const VideoMode& right);

////////////////////////////////////////////////////////////
/// \relates VideoMode
/// \brief Overload of >= operator to compare video modes
///
/// \param left  Left operand (a video mode)
/// \param right Right operand (a video mode)
///
/// \return True if \a left is greater or equal than \a right
///
////////////////////////////////////////////////////////////
SFML_WINDOW_API bool operator>=(const VideoMode& left, const VideoMode& right);

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::VideoMode
/// \ingroup window
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
/// See also: sf::Monitor.
///
////////////////////////////////////////////////////////////

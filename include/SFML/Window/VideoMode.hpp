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
/// VideoMode defines a video mode (width, height, bpp, frequency)
/// and provides static functions for getting modes supported
/// by the display device
////////////////////////////////////////////////////////////
class SFML_API VideoMode
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    VideoMode();

    ////////////////////////////////////////////////////////////
    /// Construct the video mode with its attributes
    ///
    /// \param ModeWidth :  Width in pixels
    /// \param ModeHeight : Height in pixels
    /// \param ModeBpp :    Pixel depths in bits per pixel (32 by default)
    ///
    ////////////////////////////////////////////////////////////
    VideoMode(unsigned int ModeWidth, unsigned int ModeHeight, unsigned int ModeBpp = 32);

    ////////////////////////////////////////////////////////////
    /// Get the current desktop video mode
    ///
    /// \return Current desktop video mode
    ///
    ////////////////////////////////////////////////////////////
    static VideoMode GetDesktopMode();

    ////////////////////////////////////////////////////////////
    /// Get a valid video mode
    /// Index must be in range [0, GetModesCount()[
    /// Modes are sorted from best to worst
    ///
    /// \param Index : Index of video mode to get
    ///
    /// \return Corresponding video mode (invalid mode if index is out of range)
    ///
    ////////////////////////////////////////////////////////////
    static VideoMode GetMode(std::size_t Index);

    ////////////////////////////////////////////////////////////
    /// Get valid video modes count
    ///
    /// \return Number of valid video modes available
    ///
    ////////////////////////////////////////////////////////////
    static std::size_t GetModesCount();

    ////////////////////////////////////////////////////////////
    /// Tell whether or not the video mode is supported
    ///
    /// \return True if video mode is supported, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool IsValid() const;

    ////////////////////////////////////////////////////////////
    /// Comparison operator overload -- tell if two video modes are equal
    ///
    /// \param Other : Video mode to compare
    ///
    /// \return True if modes are equal
    ///
    ////////////////////////////////////////////////////////////
    bool operator ==(const VideoMode& Other) const;

    ////////////////////////////////////////////////////////////
    /// Comparison operator overload -- tell if two video modes are different
    ///
    /// \param Other : Video mode to compare
    ///
    /// \return True if modes are different
    ///
    ////////////////////////////////////////////////////////////
    bool operator !=(const VideoMode& Other) const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int Width;        ///< Video mode width, in pixels
    unsigned int Height;       ///< Video mode height, in pixels
    unsigned int BitsPerPixel; ///< Video mode pixel depth, in bits per pixels

private :

    ////////////////////////////////////////////////////////////
    /// Get and sort valid video modes
    ////////////////////////////////////////////////////////////
    static void InitializeModes();
};

} // namespace sf


#endif // SFML_VIDEOMODE_HPP

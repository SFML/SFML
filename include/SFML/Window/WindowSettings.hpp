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

#ifndef SFML_WINDOWSETTINGS_HPP
#define SFML_WINDOWSETTINGS_HPP


namespace sf
{
////////////////////////////////////////////////////////////
/// Structure defining the creation settings of windows
////////////////////////////////////////////////////////////
struct WindowSettings
{
    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    /// \param Depth :        Depth buffer bits (24 by default)
    /// \param Stencil :      Stencil buffer bits (8 by default)
    /// \param Antialiasing : Antialiasing level (0 by default)
    ///
    ////////////////////////////////////////////////////////////
    explicit WindowSettings(unsigned int Depth = 24, unsigned int Stencil = 8, unsigned int Antialiasing = 0) :
    DepthBits        (Depth),
    StencilBits      (Stencil),
    AntialiasingLevel(Antialiasing)
    {
    }

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int DepthBits;         ///< Bits of the depth buffer
    unsigned int StencilBits;       ///< Bits of the stencil buffer
    unsigned int AntialiasingLevel; ///< Level of antialiasing
};

} // namespace sf


#endif // SFML_WINDOWSETTINGS_HPP

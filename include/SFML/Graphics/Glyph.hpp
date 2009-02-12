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

#ifndef SFML_GLYPH_HPP
#define SFML_GLYPH_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/Graphics/Rect.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// Structure describing a glyph (a visual character)
////////////////////////////////////////////////////////////
class SFML_API Glyph
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Glyph() : Advance(0) {}

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    int       Advance;   ///< Offset to move horizontically to the next character
    IntRect   Rectangle; ///< Bounding rectangle of the glyph, in relative coordinates
    FloatRect TexCoords; ///< Texture coordinates of the glyph inside the bitmap font
};

} // namespace sf


#endif // SFML_GLYPH_HPP

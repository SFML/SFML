////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/Export.hpp>

#include <SFML/Graphics/Rect.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Structure describing a glyph
///
////////////////////////////////////////////////////////////
struct SFML_GRAPHICS_API Glyph
{
    float     advance{};   //!< Offset to move horizontally to the next character
    int       lsbDelta{};  //!< Left offset after forced autohint. Internally used by getKerning()
    int       rsbDelta{};  //!< Right offset after forced autohint. Internally used by getKerning()
    FloatRect bounds;      //!< Bounding rectangle of the glyph, in coordinates relative to the baseline
    IntRect   textureRect; //!< Texture coordinates of the glyph inside the font's texture
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \struct sf::Glyph
/// \ingroup graphics
///
/// A glyph is the visual representation of a character.
///
/// The `sf::Glyph` structure provides the information needed
/// to handle the glyph:
/// \li its coordinates in the font's texture
/// \li its bounding rectangle
/// \li the offset to apply to get the starting position of the next glyph
///
/// \see `sf::Font`
///
////////////////////////////////////////////////////////////

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

#ifndef SFML_FONT_HPP
#define SFML_FONT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Resource.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Unicode.hpp>
#include <SFML/Graphics/Glyph.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <map>
#include <string>


namespace sf
{
class String;

namespace priv
{
class FontLoader;
}
////////////////////////////////////////////////////////////
/// Font is the low-level class for loading and
/// manipulating character fonts. This class is meant to
/// be used by sf::String
////////////////////////////////////////////////////////////
class SFML_API Font : public Resource<Font>
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Font();

    ////////////////////////////////////////////////////////////
    /// Load the font from a file
    ///
    /// \param Filename : Font file to load
    /// \param CharSize : Size of characters in bitmap - the bigger, the higher quality (30 by default)
    /// \param Charset :  Characters set to generate (by default, contains the ISO-8859-1 printable characters)
    ///
    /// \return True if loading was successful
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromFile(const std::string& Filename, unsigned int CharSize = 30, const Unicode::Text& Charset = ourDefaultCharset);

    ////////////////////////////////////////////////////////////
    /// Load the font from a file in memory
    ///
    /// \param Data :        Pointer to the data to load
    /// \param SizeInBytes : Size of the data, in bytes
    /// \param CharSize :    Size of characters in bitmap - the bigger, the higher quality (30 by default)
    /// \param Charset :     Characters set to generate (by default, contains the ISO-8859-1 printable characters)
    ///
    /// \return True if loading was successful
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromMemory(const char* Data, std::size_t SizeInBytes, unsigned int CharSize = 30, const Unicode::Text& Charset = ourDefaultCharset);

    ////////////////////////////////////////////////////////////
    /// Get the base size of characters in the font;
    /// All glyphs dimensions are based on this value
    ///
    /// \return Base size of characters
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetCharacterSize() const;

    ////////////////////////////////////////////////////////////
    /// Get the description of a glyph (character)
    /// given by its unicode value
    ///
    /// \param CodePoint : Unicode value of the character to get
    ///
    /// \return Glyph's visual settings, or an invalid glyph if character not found
    ///
    ////////////////////////////////////////////////////////////
    const Glyph& GetGlyph(Uint32 CodePoint) const;

    ////////////////////////////////////////////////////////////
    /// Get the image containing the rendered characters (glyphs)
    ///
    /// \return Image containing glyphs
    ///
    ////////////////////////////////////////////////////////////
    const Image& GetImage() const;

    ////////////////////////////////////////////////////////////
    /// Get the SFML default built-in font (Arial)
    ///
    /// \return Instance of the default font
    ///
    ////////////////////////////////////////////////////////////
    static const Font& GetDefaultFont();

private :

    friend class priv::FontLoader;

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    static Uint32 ourDefaultCharset[]; ///< The default charset (all printable ISO-8859-1 characters)

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Image                   myTexture;  ///< Texture holding the bitmap font
    unsigned int            myCharSize; ///< Size of characters in the bitmap font
    std::map<Uint32, Glyph> myGlyphs;   ///< Rendering settings of each character (glyph)
};

} // namespace sf


#endif // SFML_FONT_HPP

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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Font.h>
#include <SFML/Graphics/FontStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Create a new font from a file
////////////////////////////////////////////////////////////
sfFont* sfFont_CreateFromFile(const char* filename)
{
    sfFont* font = new sfFont;
    if (!font->This.LoadFromFile(filename))
    {
        delete font;
        font = NULL;
    }

    return font;
}


////////////////////////////////////////////////////////////
/// Create a new font from a file in memory
////////////////////////////////////////////////////////////
sfFont* sfFont_CreateFromMemory(const void* data, size_t sizeInBytes)
{
    sfFont* font = new sfFont;
    if (!font->This.LoadFromMemory(data, sizeInBytes))
    {
        delete font;
        font = NULL;
    }

    return font;
}


////////////////////////////////////////////////////////////
/// Copy an existing font
////////////////////////////////////////////////////////////
sfFont* sfFont_Copy(sfFont* font)
{
    CSFML_CHECK_RETURN(font, NULL);

    return new sfFont(*font);
}


////////////////////////////////////////////////////////////
/// Destroy an existing font
////////////////////////////////////////////////////////////
void sfFont_Destroy(sfFont* font)
{
    delete font;
}


////////////////////////////////////////////////////////////
/// Get a glyph in a font
////////////////////////////////////////////////////////////
sfGlyph sfFont_GetGlyph(sfFont* font, sfUint32 codePoint, unsigned int characterSize, sfBool bold)
{
    sfGlyph glyph = {0, {0, 0, 0, 0}, {0, 0, 0, 0}};
    CSFML_CHECK_RETURN(font, glyph);

    sf::Glyph SFMLGlyph = font->This.GetGlyph(codePoint, characterSize, bold == sfTrue);

    glyph.Advance        = SFMLGlyph.Advance;
    glyph.Bounds.Left    = SFMLGlyph.Bounds.Left;
    glyph.Bounds.Top     = SFMLGlyph.Bounds.Top;
    glyph.Bounds.Width   = SFMLGlyph.Bounds.Width;
    glyph.Bounds.Height  = SFMLGlyph.Bounds.Height;
    glyph.SubRect.Left   = SFMLGlyph.SubRect.Left;
    glyph.SubRect.Top    = SFMLGlyph.SubRect.Top;
    glyph.SubRect.Width  = SFMLGlyph.SubRect.Width;
    glyph.SubRect.Height = SFMLGlyph.SubRect.Height;

    return glyph;
}


////////////////////////////////////////////////////////////
/// Get the kerning value corresponding to a given pair of characters in a font
////////////////////////////////////////////////////////////
int sfFont_GetKerning(sfFont* font, sfUint32 first, sfUint32 second, unsigned int characterSize)
{
    CSFML_CALL_RETURN(font, GetKerning(first, second, characterSize), 0);
}


////////////////////////////////////////////////////////////
/// Get the line spacing value
////////////////////////////////////////////////////////////
int sfFont_GetLineSpacing(sfFont* font, unsigned int characterSize)
{
    CSFML_CALL_RETURN(font, GetLineSpacing(characterSize), 0);
}


////////////////////////////////////////////////////////////
/// Get the image containing the glyphs of a given size in a font
////////////////////////////////////////////////////////////
const sfImage* sfFont_GetImage(sfFont* font, unsigned int characterSize)
{
    CSFML_CHECK_RETURN(font, NULL);

    *font->Images[characterSize].This = font->This.GetImage(characterSize);

    return &font->Images[characterSize];
}


////////////////////////////////////////////////////////////
/// Get the built-in default font (Arial)
////////////////////////////////////////////////////////////
const sfFont* sfFont_GetDefaultFont(void)
{
    static sfFont defaultFont = {sf::Font::GetDefaultFont(), std::map<unsigned int, sfImage>()};

    return &defaultFont;
}

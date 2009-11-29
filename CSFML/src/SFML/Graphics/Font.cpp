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
sfFont* sfFont_CreateFromFile(const char* filename, unsigned int charSize, const sfUint32* charset)
{
    sfFont* font = new sfFont;

    bool success = false;
    if (charset)
        success = font->This.LoadFromFile(filename, charSize, charset);
    else
        success = font->This.LoadFromFile(filename, charSize);

    if (!success)
    {
        delete font;
        font = NULL;
    }

    return font;
}


////////////////////////////////////////////////////////////
/// Create a new font from a file in memory
////////////////////////////////////////////////////////////
sfFont* sfFont_CreateFromMemory(const char* data, size_t sizeInBytes, unsigned int charSize, const sfUint32* charset)
{
    sfFont* font = new sfFont;

    bool success = false;
    if (charset)
        success = font->This.LoadFromMemory(data, sizeInBytes, charSize, charset);
    else
        success = font->This.LoadFromMemory(data, sizeInBytes, charSize);

    if (!success)
    {
        delete font;
        font = NULL;
    }

    return font;
}


////////////////////////////////////////////////////////////
/// Destroy an existing font
////////////////////////////////////////////////////////////
void sfFont_Destroy(sfFont* font)
{
    delete font;
}


////////////////////////////////////////////////////////////
/// Get the base size of characters in a font;
/// All glyphs dimensions are based on this value
////////////////////////////////////////////////////////////
unsigned int sfFont_GetCharacterSize(const sfFont* font)
{
    CSFML_CALL_RETURN(font, GetCharacterSize(), 0);
}


////////////////////////////////////////////////////////////
/// Get the built-in default font (Arial)
////////////////////////////////////////////////////////////
const sfFont* sfFont_GetDefaultFont()
{
    static sfFont defaultFont = {sf::Font::GetDefaultFont()};

    return &defaultFont;
}

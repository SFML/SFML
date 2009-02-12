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
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/FontLoader.hpp>
#include <iostream>


namespace sf
{
////////////////////////////////////////////////////////////
// Static member data
////////////////////////////////////////////////////////////
Uint32 Font::ourDefaultCharset[] =
{
    // Printable characters in ASCII range
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E,

    // Printable characters in extended ASCII range
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0x2A, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE,

    // To make it a valid string
    0x00
};


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Font::Font() :
myCharSize(0)
{

}


////////////////////////////////////////////////////////////
/// Load the font from a file
////////////////////////////////////////////////////////////
bool Font::LoadFromFile(const std::string& Filename, unsigned int CharSize, const Unicode::Text& Charset)
{
    // Clear the previous character map
    myGlyphs.clear();

    // Always add these special characters
    Unicode::UTF32String UTFCharset = Charset;
    if (UTFCharset.find(L' ')  != Unicode::UTF32String::npos) UTFCharset += L' ';
    if (UTFCharset.find(L'\n') != Unicode::UTF32String::npos) UTFCharset += L'\n';
    if (UTFCharset.find(L'\v') != Unicode::UTF32String::npos) UTFCharset += L'\v';
    if (UTFCharset.find(L'\t') != Unicode::UTF32String::npos) UTFCharset += L'\t';

    return priv::FontLoader::GetInstance().LoadFontFromFile(Filename, CharSize, UTFCharset, *this);
}


////////////////////////////////////////////////////////////
/// Load the font from a file in memory
////////////////////////////////////////////////////////////
bool Font::LoadFromMemory(const char* Data, std::size_t SizeInBytes, unsigned int CharSize, const Unicode::Text& Charset)
{
    // Clear the previous character map
    myGlyphs.clear();

    // Check parameters
    if (!Data || (SizeInBytes == 0))
    {
        std::cerr << "Failed to load font from memory, no data provided" << std::endl;
        return false;
    }

    // Always add these special characters
    Unicode::UTF32String UTFCharset = Charset;
    if (UTFCharset.find(L' ')  != Unicode::UTF32String::npos) UTFCharset += L' ';
    if (UTFCharset.find(L'\n') != Unicode::UTF32String::npos) UTFCharset += L'\n';
    if (UTFCharset.find(L'\v') != Unicode::UTF32String::npos) UTFCharset += L'\v';
    if (UTFCharset.find(L'\t') != Unicode::UTF32String::npos) UTFCharset += L'\t';

    return priv::FontLoader::GetInstance().LoadFontFromMemory(Data, SizeInBytes, CharSize, UTFCharset, *this);
}


////////////////////////////////////////////////////////////
/// Get the base size of characters in the font;
/// All glyphs dimensions are based on this value
////////////////////////////////////////////////////////////
unsigned int Font::GetCharacterSize() const
{
    return myCharSize;
}


////////////////////////////////////////////////////////////
/// Get the description of a glyph (character)
/// given by its unicode value
////////////////////////////////////////////////////////////
const Glyph& Font::GetGlyph(Uint32 CodePoint) const
{
    std::map<Uint32, Glyph>::const_iterator It = myGlyphs.find(CodePoint);
    if (It != myGlyphs.end())
    {
        // Valid glyph
        return It->second;
    }
    else
    {
        // Invalid glyph -- return an invalid glyph
        static const Glyph InvalidGlyph;
        return InvalidGlyph;
    }
}


////////////////////////////////////////////////////////////
/// Get the image containing the rendered characters (glyphs)
////////////////////////////////////////////////////////////
const Image& Font::GetImage() const
{
    return myTexture;
}


////////////////////////////////////////////////////////////
/// Get the SFML default built-in font (Arial)
////////////////////////////////////////////////////////////
const Font& Font::GetDefaultFont()
{
    static Font       DefaultFont;
    static bool       DefaultFontLoaded = false;
    static const char DefaultFontData[] =
    {
        #include <SFML/Graphics/Arial.hpp>
    };

    // Load the default font on first call
    if (!DefaultFontLoaded)
    {
        DefaultFont.LoadFromMemory(DefaultFontData, sizeof(DefaultFontData), 30);
        DefaultFontLoaded = true;
    }

    return DefaultFont;
}

} // namespace sf

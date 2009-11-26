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

#ifndef SFML_FONTLOADER_HPP
#define SFML_FONTLOADER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/String.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>


namespace sf
{
class Font;

namespace priv
{
////////////////////////////////////////////////////////////
/// FontLoader loads and saves character fonts
////////////////////////////////////////////////////////////
class FontLoader : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// Get the unique instance of the class
    ///
    /// \return Reference to the FontLoader instance
    ///
    ////////////////////////////////////////////////////////////
    static FontLoader& GetInstance();

    ////////////////////////////////////////////////////////////
    /// Load a font from a file
    ///
    /// \param filename : Path of the font file to load
    /// \param charSize : Size of characters in bitmap - the bigger, the higher quality
    /// \param charset :  Characters set to generate
    /// \param font :     Font object to fill up
    ///
    /// \return True if loading was successful
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFontFromFile(const std::string& filename, unsigned int charSize, const String& charset, Font& font);

    ////////////////////////////////////////////////////////////
    /// Load the font from a file in memory
    ///
    /// \param data :        Pointer to the data to load
    /// \param sizeInBytes : Size of the data, in bytes
    /// \param charSize :    Size of characters in bitmap - the bigger, the higher quality
    /// \param charset :     Characters set to generate
    /// \param font :  Font object to fill up
    ///
    /// \return True if loading was successful
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFontFromMemory(const char* data, std::size_t sizeInBytes, unsigned int charSize, const String& charset, Font& font);

private :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    FontLoader();

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~FontLoader();

    ////////////////////////////////////////////////////////////
    /// Create a bitmap font from a font face and a characters set
    ///
    /// \param face :     Font face containing the loaded font
    /// \param charSize : Size of characters in bitmap
    /// \param charset :  Characters set to generate
    /// \param font :     Font object to fill up
    ///
    ////////////////////////////////////////////////////////////
    FT_Error CreateBitmapFont(FT_Face face, unsigned int charSize, const String& charset, Font& font);

    ////////////////////////////////////////////////////////////
    /// Get a description from a FT error code
    ///
    /// \param error : FreeType error code
    ///
    /// \return Error description
    ///
    ////////////////////////////////////////////////////////////
    static std::string GetErrorDesc(FT_Error error);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    FT_Library myLibrary; ///< Handle to the Freetype library
};

} // namespace priv

} // namespace sf


#endif // SFML_FONTLOADER_HPP

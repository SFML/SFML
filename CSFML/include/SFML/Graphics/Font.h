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

#ifndef SFML_FONT_H
#define SFML_FONT_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Glyph.h>
#include <SFML/Graphics/Types.h>


////////////////////////////////////////////////////////////
/// Create a new empty font
///
/// \return A new sfFont object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfFont* sfFont_Create();

////////////////////////////////////////////////////////////
/// Create a new font from a file
///
/// \param Filename : Path of the font file to load
/// \param CharSize : Size of characters in bitmap - the bigger, the higher quality
/// \param Charset :  Characters set to generate (just pass NULL to get the default charset)
///
/// \return A new sfFont object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfFont* sfFont_CreateFromFile(const char* Filename, unsigned int CharSize, const sfUint32* Charset);

////////////////////////////////////////////////////////////
/// Create a new image font a file in memory
///
/// \param Data :        Pointer to the file data in memory
/// \param SizeInBytes : Size of the data to load, in bytes
/// \param CharSize :    Size of characters in bitmap - the bigger, the higher quality
/// \param Charset :     Characters set to generate (just pass NULL to get the default charset)
///
/// \return A new sfFont object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfFont* sfFont_CreateFromMemory(const char* Data, size_t SizeInBytes, unsigned int CharSize, const sfUint32* Charset);

////////////////////////////////////////////////////////////
/// Destroy an existing font
///
/// \param Font : Font to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfFont_Destroy(sfFont* Font);

////////////////////////////////////////////////////////////
/// Get the base size of characters in a font;
/// All glyphs dimensions are based on this value
///
/// \param Font : Font object
///
/// \return Base size of characters
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfFont_GetCharacterSize(sfFont* Font);

////////////////////////////////////////////////////////////
/// Get the built-in default font (Arial)
///
/// \return Pointer to the default font
///
////////////////////////////////////////////////////////////
CSFML_API sfFont* sfFont_GetDefaultFont();


#endif // SFML_IMAGE_H

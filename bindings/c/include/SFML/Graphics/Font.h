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
/// Create a new font from a file
///
/// \param filename : Path of the font file to load
///
/// \return A new sfFont object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfFont* sfFont_CreateFromFile(const char* filename);

////////////////////////////////////////////////////////////
/// Create a new image font a file in memory
///
/// \param data :        Pointer to the file data in memory
/// \param sizeInBytes : Size of the data to load, in bytes
///
/// \return A new sfFont object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfFont* sfFont_CreateFromMemory(const void* data, size_t sizeInBytes);

////////////////////////////////////////////////////////////
/// Copy an existing font
///
/// \param font : Font to copy
///
/// \return Copied object
///
////////////////////////////////////////////////////////////
CSFML_API sfFont* sfFont_Copy(sfFont* font);

////////////////////////////////////////////////////////////
/// Destroy an existing font
///
/// \param font : Font to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfFont_Destroy(sfFont* font);

////////////////////////////////////////////////////////////
/// Get a glyph in a font
///
/// \param font :          Source font
/// \param codePoint :     Unicode code point of the character to get
/// \param characterSize : Character size, in pixels
/// \param bold            Retrieve the bold version or the regular one?
///
/// \return The corresponding glyph
///
////////////////////////////////////////////////////////////
CSFML_API sfGlyph sfFont_GetGlyph(sfFont* font, sfUint32 codePoint, unsigned int characterSize, sfBool bold);

////////////////////////////////////////////////////////////
/// Get the kerning value corresponding to a given pair of characters in a font
///
/// \param font :          Source font
/// \param first :         Unicode code point of the first character
/// \param second :        Unicode code point of the second character
/// \param characterSize : Character size, in pixels
///
/// \return Kerning offset, in pixels
///
////////////////////////////////////////////////////////////
CSFML_API int sfFont_GetKerning(sfFont* font, sfUint32 first, sfUint32 second, unsigned int characterSize);

////////////////////////////////////////////////////////////
/// Get the line spacing value
///
/// \param font :          Source font
/// \param codePoint :     Unicode code point of the character to get
/// \param characterSize : Character size, in pixels
///
/// \return Line spacing, in pixels
///
////////////////////////////////////////////////////////////
CSFML_API int sfFont_GetLineSpacing(sfFont* font, unsigned int characterSize);

////////////////////////////////////////////////////////////
/// Get the image containing the glyphs of a given size in a font
///
/// \param font :          Source font
/// \param characterSize : Character size, in pixels
///
/// \return Read-only pointer to the image
///
////////////////////////////////////////////////////////////
CSFML_API const sfImage* sfFont_GetImage(sfFont* font, unsigned int characterSize);

////////////////////////////////////////////////////////////
/// Get the built-in default font (Arial)
///
/// \return Pointer to the default font
///
////////////////////////////////////////////////////////////
CSFML_API const sfFont* sfFont_GetDefaultFont(void);


#endif // SFML_IMAGE_H

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

#ifndef SFML_STRING_H
#define SFML_STRING_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Graphics/BlendMode.h>
#include <SFML/Graphics/Color.h>
#include <SFML/Graphics/Rect.h>
#include <SFML/Graphics/Types.h>


////////////////////////////////////////////////////////////
/// sfString styles
////////////////////////////////////////////////////////////
typedef enum
{
    sfStringRegular    = 0,      ///< Regular characters, no style
    sfStringBold       = 1 << 0, ///< Characters are bold
    sfStringItalic     = 1 << 1, ///< Characters are in italic
    sfStringUnderlined = 1 << 2  ///< Characters are underlined
} sfStringStyle;


////////////////////////////////////////////////////////////
/// Create a new string
///
/// \return A new sfString object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfString* sfString_Create();

////////////////////////////////////////////////////////////
/// Destroy an existing string
///
/// \param string : String to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_Destroy(sfString* string);

////////////////////////////////////////////////////////////
/// Set the X position of a string
///
/// \param string : String to modify
/// \param x :      New X coordinate
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_SetX(sfString* string, float x);

////////////////////////////////////////////////////////////
/// Set the Y position of a string
///
/// \param string : String to modify
/// \param y :      New Y coordinate
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_SetY(sfString* string, float y);

////////////////////////////////////////////////////////////
/// Set the position of a string
///
/// \param string : String to modify
/// \param x :      New X coordinate
/// \param y :      New Y coordinate
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_SetPosition(sfString* string, float x, float y);

////////////////////////////////////////////////////////////
/// Set the horizontal scale of a string
///
/// \param string : String to modify
/// \param scale :  New scale (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_SetScaleX(sfString* string, float scale);

////////////////////////////////////////////////////////////
/// Set the vertical scale of a string
///
/// \param string : String to modify
/// \param scale :  New scale (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_SetScaleY(sfString* string, float scale);

////////////////////////////////////////////////////////////
/// Set the scale of a string
///
/// \param string : String to modify
/// \param scaleX : New horizontal scale (must be strictly positive)
/// \param scaleY : New vertical scale (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_SetScale(sfString* string, float scaleX, float scaleY);

////////////////////////////////////////////////////////////
/// Set the orientation of a string
///
/// \param string :   String to modify
/// \param rotation : Angle of rotation, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_SetRotation(sfString* string, float rotation);

////////////////////////////////////////////////////////////
/// Set the local origin of a string, in coordinates
/// relative to its left-top corner
///
/// \param string : String to modify
/// \param x :      X coordinate of the origin
/// \param y :      Y coordinate of the origin
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_SetOrigin(sfString* string, float x, float y);

////////////////////////////////////////////////////////////
/// Set the color of a string
///
/// \param string : String to modify
/// \param color :  New color
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_SetColor(sfString* string, sfColor color);

////////////////////////////////////////////////////////////
/// Set the blending mode for a string
///
/// \param string : String to modify
/// \param mode :   New blending mode
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_SetBlendMode(sfString* string, sfBlendMode mode);

////////////////////////////////////////////////////////////
/// Get the X position of a string
///
/// \param string : String to read
///
/// \return Current X position
///
////////////////////////////////////////////////////////////
CSFML_API float sfString_GetX(sfString* string);

////////////////////////////////////////////////////////////
/// Get the top Y of a string
///
/// \param string : String to read
///
/// \return Current Y position
///
////////////////////////////////////////////////////////////
CSFML_API float sfString_GetY(sfString* string);

////////////////////////////////////////////////////////////
/// Get the horizontal scale of a string
///
/// \param string : String to read
///
/// \return Current X scale factor (always positive)
///
////////////////////////////////////////////////////////////
CSFML_API float sfString_GetScaleX(sfString* string);

////////////////////////////////////////////////////////////
/// Get the vertical scale of a string
///
/// \param string : String to read
///
/// \return Current Y scale factor (always positive)
///
////////////////////////////////////////////////////////////
CSFML_API float sfString_GetScaleY(sfString* string);

////////////////////////////////////////////////////////////
/// Get the orientation of a string
///
/// \param string : String to read
///
/// \return Current rotation, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API float sfString_GetRotation(sfString* string);

////////////////////////////////////////////////////////////
/// Get the X position of the origin a string
///
/// \param string : String to read
///
/// \return Current X origin position
///
////////////////////////////////////////////////////////////
CSFML_API float sfString_GetOriginX(sfString* string);

////////////////////////////////////////////////////////////
/// Get the top Y of the origin of a string
///
/// \param string : String to read
///
/// \return Current Y origin position
///
////////////////////////////////////////////////////////////
CSFML_API float sfString_GetOriginY(sfString* string);

////////////////////////////////////////////////////////////
/// Get the color of a string
///
/// \param string : String to read
///
/// \return Current color
///
////////////////////////////////////////////////////////////
CSFML_API sfColor sfString_GetColor(sfString* string);

////////////////////////////////////////////////////////////
/// Get the current blending mode of a string
///
/// \param string : String to read
///
/// \return Current blending mode
///
////////////////////////////////////////////////////////////
CSFML_API sfBlendMode sfString_GetBlendMode(sfString* string);

////////////////////////////////////////////////////////////
/// Move a string
///
/// \param string :  String to modify
/// \param offsetX : Offset on the X axis
/// \param offsetY : Offset on the Y axis
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_Move(sfString* string, float offsetX, float offsetY);

////////////////////////////////////////////////////////////
/// Scale a string
///
/// \param string :  String to modify
/// \param factorX : Horizontal scaling factor (must be strictly positive)
/// \param factorY : Vertical scaling factor (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_Scale(sfString* string, float factorX, float factorY);

////////////////////////////////////////////////////////////
/// Rotate a string
///
/// \param string : String to modify
/// \param angle :  Angle of rotation, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_Rotate(sfString* string, float angle);

////////////////////////////////////////////////////////////
/// Transform a point from global coordinates into the string's local coordinates
/// (ie it applies the inverse of object's origin, translation, rotation and scale to the point)
///
/// \param string : String object
/// \param pointX : X coordinate of the point to transform
/// \param pointY : Y coordinate of the point to transform
/// \param x :      Value to fill with the X coordinate of the converted point
/// \param y :      Value to fill with the y coordinate of the converted point
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_TransformToLocal(sfString* string, float pointX, float pointY, float* x, float* y);

////////////////////////////////////////////////////////////
/// Transform a point from the string's local coordinates into global coordinates
/// (ie it applies the object's origin, translation, rotation and scale to the point)
///
/// \param string : String object
/// \param pointX : X coordinate of the point to transform
/// \param pointY : Y coordinate of the point to transform
/// \param x :      Value to fill with the X coordinate of the converted point
/// \param y :      Value to fill with the y coordinate of the converted point
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_TransformToGlobal(sfString* string, float pointX, float pointY, float* X, float* y);

////////////////////////////////////////////////////////////
/// Set the text of a string (from a multibyte string)
///
/// \param string : String to modify
/// \param text :   New text
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_SetText(sfString* string, const char* text);

////////////////////////////////////////////////////////////
/// Set the text of a string (from a unicode string)
///
/// \param string : String to modify
/// \param text :   New text
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_SetUnicodeText(sfString* string, const sfUint32* text);

////////////////////////////////////////////////////////////
/// Set the font of a string
///
/// \param string : String to modify
/// \param font :   Font to use
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_SetFont(sfString* string, sfFont* font);

////////////////////////////////////////////////////////////
/// Set the size of a string
///
/// \param string : String to modify
/// \param size :   New size, in pixels
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_SetSize(sfString* string, float size);

////////////////////////////////////////////////////////////
/// Set the style of a string
///
/// \param string : String to modify
/// \param style :  New style (see sfStringStyle enum)
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_SetStyle(sfString* string, unsigned long style);

////////////////////////////////////////////////////////////
/// Get the text of a string (returns a unicode string)
///
/// \param string : String to read
///
/// \return Text as UTF-32
///
////////////////////////////////////////////////////////////
CSFML_API const sfUint32* sfString_GetUnicodeText(sfString* string);

////////////////////////////////////////////////////////////
/// Get the text of a string (returns an ANSI string)
///
/// \param string : String to read
///
/// \return Text an a locale-dependant ANSI string
///
////////////////////////////////////////////////////////////
CSFML_API const char* sfString_GetText(sfString* string);

////////////////////////////////////////////////////////////
/// Get the font used by a string
///
/// \param string : String to read
///
/// \return Pointer to the font
///
////////////////////////////////////////////////////////////
CSFML_API sfFont* sfString_GetFont(sfString* string);

////////////////////////////////////////////////////////////
/// Get the size of the characters of a string
///
/// \param string : String to read
///
/// \return Size of the characters
///
////////////////////////////////////////////////////////////
CSFML_API float sfString_GetSize(sfString* string);

////////////////////////////////////////////////////////////
/// Get the style of a string
///
/// \param string : String to read
///
/// \return Current string style (see sfStringStyle enum)
///
////////////////////////////////////////////////////////////
CSFML_API unsigned long sfString_GetStyle(sfString* string);

////////////////////////////////////////////////////////////
/// Return the visual position of the Index-th character of the string,
/// in coordinates relative to the string
/// (note : translation, origin, rotation and scale are not applied)
///
/// \param string : String to read
/// \param index :  Index of the character
/// \param x :      Value to fill with the X coordinate of the position
/// \param y :      Value to fill with the y coordinate of the position
///
////////////////////////////////////////////////////////////
CSFML_API void sfString_GetCharacterPos(sfString* string, size_t index, float* x, float* y);

////////////////////////////////////////////////////////////
/// Get the bounding rectangle of a string on screen
///
/// \param string : String to read
///
/// \return Rectangle contaning the string in screen coordinates
///
////////////////////////////////////////////////////////////
CSFML_API sfFloatRect sfString_GetRect(sfString* string);


#endif // SFML_STRING_H

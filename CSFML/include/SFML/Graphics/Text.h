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

#ifndef SFML_TEXT_H
#define SFML_TEXT_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Graphics/BlendMode.h>
#include <SFML/Graphics/Color.h>
#include <SFML/Graphics/Rect.h>
#include <SFML/Graphics/Types.h>


////////////////////////////////////////////////////////////
/// sfText styles
////////////////////////////////////////////////////////////
typedef enum
{
    sfTextRegular    = 0,      ///< Regular characters, no style
    sfTextBold       = 1 << 0, ///< Characters are bold
    sfTextItalic     = 1 << 1, ///< Characters are in italic
    sfTextUnderlined = 1 << 2  ///< Characters are underlined
} sfTextStyle;


////////////////////////////////////////////////////////////
/// Create a new text
///
/// \return A new sfText object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfText* sfText_Create(void);

////////////////////////////////////////////////////////////
/// Copy an existing text
///
/// \param text : Text to copy
///
/// \return Copied object
///
////////////////////////////////////////////////////////////
CSFML_API sfText* sfText_Copy(sfText* text);

////////////////////////////////////////////////////////////
/// Destroy an existing text
///
/// \param text : Text to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_Destroy(sfText* text);

////////////////////////////////////////////////////////////
/// Set the X position of a text
///
/// \param text : String to modify
/// \param x :    New X coordinate
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_SetX(sfText* text, float x);

////////////////////////////////////////////////////////////
/// Set the Y position of a text
///
/// \param text : String to modify
/// \param y :    New Y coordinate
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_SetY(sfText* text, float y);

////////////////////////////////////////////////////////////
/// Set the position of a text
///
/// \param text : String to modify
/// \param x :    New X coordinate
/// \param y :    New Y coordinate
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_SetPosition(sfText* text, float x, float y);

////////////////////////////////////////////////////////////
/// Set the horizontal scale of a text
///
/// \param text :  String to modify
/// \param scale : New scale (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_SetScaleX(sfText* text, float scale);

////////////////////////////////////////////////////////////
/// Set the vertical scale of a text
///
/// \param text :  String to modify
/// \param scale : New scale (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_SetScaleY(sfText* text, float scale);

////////////////////////////////////////////////////////////
/// Set the scale of a text
///
/// \param text :   String to modify
/// \param scaleX : New horizontal scale (must be strictly positive)
/// \param scaleY : New vertical scale (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_SetScale(sfText* text, float scaleX, float scaleY);

////////////////////////////////////////////////////////////
/// Set the orientation of a text
///
/// \param text :     String to modify
/// \param rotation : Angle of rotation, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_SetRotation(sfText* text, float rotation);

////////////////////////////////////////////////////////////
/// Set the local origin of a text, in coordinates
/// relative to its left-top corner
///
/// \param text : String to modify
/// \param x :    X coordinate of the origin
/// \param y :    Y coordinate of the origin
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_SetOrigin(sfText* text, float x, float y);

////////////////////////////////////////////////////////////
/// Set the color of a text
///
/// \param text :  String to modify
/// \param color : New color
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_SetColor(sfText* text, sfColor color);

////////////////////////////////////////////////////////////
/// Set the blending mode for a text
///
/// \param text : String to modify
/// \param mode : New blending mode
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_SetBlendMode(sfText* text, sfBlendMode mode);

////////////////////////////////////////////////////////////
/// Get the X position of a text
///
/// \param text : String to read
///
/// \return Current X position
///
////////////////////////////////////////////////////////////
CSFML_API float sfText_GetX(const sfText* text);

////////////////////////////////////////////////////////////
/// Get the top Y of a text
///
/// \param text : String to read
///
/// \return Current Y position
///
////////////////////////////////////////////////////////////
CSFML_API float sfText_GetY(const sfText* text);

////////////////////////////////////////////////////////////
/// Get the horizontal scale of a text
///
/// \param text : String to read
///
/// \return Current X scale factor (always positive)
///
////////////////////////////////////////////////////////////
CSFML_API float sfText_GetScaleX(const sfText* text);

////////////////////////////////////////////////////////////
/// Get the vertical scale of a text
///
/// \param text : String to read
///
/// \return Current Y scale factor (always positive)
///
////////////////////////////////////////////////////////////
CSFML_API float sfText_GetScaleY(const sfText* text);

////////////////////////////////////////////////////////////
/// Get the orientation of a text
///
/// \param text : String to read
///
/// \return Current rotation, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API float sfText_GetRotation(const sfText* text);

////////////////////////////////////////////////////////////
/// Get the X position of the origin a text
///
/// \param text : String to read
///
/// \return Current X origin position
///
////////////////////////////////////////////////////////////
CSFML_API float sfText_GetOriginX(const sfText* text);

////////////////////////////////////////////////////////////
/// Get the top Y of the origin of a text
///
/// \param text : String to read
///
/// \return Current Y origin position
///
////////////////////////////////////////////////////////////
CSFML_API float sfText_GetOriginY(const sfText* text);

////////////////////////////////////////////////////////////
/// Get the color of a text
///
/// \param text : String to read
///
/// \return Current color
///
////////////////////////////////////////////////////////////
CSFML_API sfColor sfText_GetColor(const sfText* text);

////////////////////////////////////////////////////////////
/// Get the current blending mode of a text
///
/// \param text : String to read
///
/// \return Current blending mode
///
////////////////////////////////////////////////////////////
CSFML_API sfBlendMode sfText_GetBlendMode(const sfText* text);

////////////////////////////////////////////////////////////
/// Move a text
///
/// \param text :    String to modify
/// \param offsetX : Offset on the X axis
/// \param offsetY : Offset on the Y axis
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_Move(sfText* text, float offsetX, float offsetY);

////////////////////////////////////////////////////////////
/// Scale a text
///
/// \param text :    String to modify
/// \param factorX : Horizontal scaling factor (must be strictly positive)
/// \param factorY : Vertical scaling factor (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_Scale(sfText* text, float factorX, float factorY);

////////////////////////////////////////////////////////////
/// Rotate a text
///
/// \param text :  String to modify
/// \param angle : Angle of rotation, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_Rotate(sfText* text, float angle);

////////////////////////////////////////////////////////////
/// Transform a point from global coordinates into the string's local coordinates
/// (ie it applies the inverse of object's origin, translation, rotation and scale to the point)
///
/// \param text :   String object
/// \param pointX : X coordinate of the point to transform
/// \param pointY : Y coordinate of the point to transform
/// \param x :      Value to fill with the X coordinate of the converted point
/// \param y :      Value to fill with the y coordinate of the converted point
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_TransformToLocal(const sfText* text, float pointX, float pointY, float* x, float* y);

////////////////////////////////////////////////////////////
/// Transform a point from the string's local coordinates into global coordinates
/// (ie it applies the object's origin, translation, rotation and scale to the point)
///
/// \param text :   String object
/// \param pointX : X coordinate of the point to transform
/// \param pointY : Y coordinate of the point to transform
/// \param x :      Value to fill with the X coordinate of the converted point
/// \param y :      Value to fill with the y coordinate of the converted point
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_TransformToGlobal(const sfText* text, float pointX, float pointY, float* X, float* y);

////////////////////////////////////////////////////////////
/// Set the string of a text (from a multibyte string)
///
/// \param text :   Text to modify
/// \param string : New string
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_SetString(sfText* text, const char* string);

////////////////////////////////////////////////////////////
/// Set the string of a text (from a unicode string)
///
/// \param text :   Text to modify
/// \param string : New string
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_SetUnicodeString(sfText* text, const sfUint32* string);

////////////////////////////////////////////////////////////
/// Set the font of a text
///
/// \param text : String to modify
/// \param font : Font to use
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_SetFont(sfText* text, const sfFont* font);

////////////////////////////////////////////////////////////
/// Set the size of a text
///
/// \param text : String to modify
/// \param size : New size, in pixels
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_SetCharacterSize(sfText* text, unsigned int size);

////////////////////////////////////////////////////////////
/// Set the style of a text
///
/// \param text :  String to modify
/// \param style : New style (see sfTextStyle enum)
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_SetStyle(sfText* text, unsigned long style);

////////////////////////////////////////////////////////////
/// Get the string of a text (returns a unicode string)
///
/// \param text : String to read
///
/// \return String as UTF-32
///
////////////////////////////////////////////////////////////
CSFML_API const sfUint32* sfText_GetUnicodeString(const sfText* text);

////////////////////////////////////////////////////////////
/// Get the text of a text (returns an ANSI string)
///
/// \param text : String to read
///
/// \return String an a locale-dependant ANSI string
///
////////////////////////////////////////////////////////////
CSFML_API const char* sfText_GetString(const sfText* text);

////////////////////////////////////////////////////////////
/// Get the font used by a text
///
/// \param text : String to read
///
/// \return Pointer to the font
///
////////////////////////////////////////////////////////////
CSFML_API const sfFont* sfText_GetFont(const sfText* text);

////////////////////////////////////////////////////////////
/// Get the size of the characters of a text
///
/// \param text : String to read
///
/// \return Size of the characters
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfText_GetCharacterSize(const sfText* text);

////////////////////////////////////////////////////////////
/// Get the style of a text
///
/// \param text : String to read
///
/// \return Current string style (see sfTextStyle enum)
///
////////////////////////////////////////////////////////////
CSFML_API unsigned long sfText_GetStyle(const sfText* text);

////////////////////////////////////////////////////////////
/// Return the visual position of the Index-th character of the text,
/// in coordinates relative to the string
/// (note : translation, origin, rotation and scale are not applied)
///
/// \param text :  String to read
/// \param index : Index of the character
/// \param x :     Value to fill with the X coordinate of the position
/// \param y :     Value to fill with the y coordinate of the position
///
////////////////////////////////////////////////////////////
CSFML_API void sfText_GetCharacterPos(const sfText* text, size_t index, float* x, float* y);

////////////////////////////////////////////////////////////
/// Get the bounding rectangle of a text on screen
///
/// \param text : String to read
///
/// \return Rectangle contaning the string in screen coordinates
///
////////////////////////////////////////////////////////////
CSFML_API sfFloatRect sfText_GetRect(const sfText* text);


#endif // SFML_TEXT_H

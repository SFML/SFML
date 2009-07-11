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
#include <SFML/Graphics/String.h>
#include <SFML/Graphics/StringStruct.h>
#include <SFML/Graphics/Font.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Create a new string
////////////////////////////////////////////////////////////
sfString* sfString_Create()
{
    sfString* string = new sfString;
    string->Font = sfFont_GetDefaultFont();

    return string;
}


////////////////////////////////////////////////////////////
/// Destroy an existing string
////////////////////////////////////////////////////////////
void sfString_Destroy(sfString* string)
{
    delete string;
}


////////////////////////////////////////////////////////////
/// Set the X position of a string
////////////////////////////////////////////////////////////
void sfString_SetX(sfString* string, float x)
{
    CSFML_CALL(string, SetX(x))
}


////////////////////////////////////////////////////////////
/// Set the Y position of a string
////////////////////////////////////////////////////////////
void sfString_SetY(sfString* string, float y)
{
    CSFML_CALL(string, SetY(y))
}


////////////////////////////////////////////////////////////
/// Set the position of a string
////////////////////////////////////////////////////////////
void sfString_SetPosition(sfString* string, float x, float y)
{
    CSFML_CALL(string, SetPosition(sf::Vector2f(x, y)))
}


////////////////////////////////////////////////////////////
/// Set the horizontal scale of a string
////////////////////////////////////////////////////////////
void sfString_SetScaleX(sfString* string, float scale)
{
    CSFML_CALL(string, SetScaleX(scale))
}


////////////////////////////////////////////////////////////
/// Set the vertical scale of a string
////////////////////////////////////////////////////////////
void sfString_SetScaleY(sfString* string, float scale)
{
    CSFML_CALL(string, SetScaleY(scale))
}


////////////////////////////////////////////////////////////
/// Set the scale of a string
////////////////////////////////////////////////////////////
void sfString_SetScale(sfString* string, float scaleX, float scaleY)
{
    CSFML_CALL(string, SetScale(sf::Vector2f(scaleX, scaleY)))
}


////////////////////////////////////////////////////////////
/// Set the orientation of a string
////////////////////////////////////////////////////////////
void sfString_SetRotation(sfString* string, float rotation)
{
    CSFML_CALL(string, SetRotation(rotation))
}


////////////////////////////////////////////////////////////
/// Set the local origin of a string, in coordinates
/// relative to its left-top corner
////////////////////////////////////////////////////////////
void sfString_SetOrigin(sfString* string, float x, float y)
{
    CSFML_CALL(string, SetOrigin(sf::Vector2f(x, y)))
}


////////////////////////////////////////////////////////////
/// Set the color of a string
////////////////////////////////////////////////////////////
void sfString_SetColor(sfString* string, sfColor color)
{
    CSFML_CALL(string, SetColor(sf::Color(color.r, color.g, color.b, color.a)))
}


////////////////////////////////////////////////////////////
/// Set the blending mode for a string
////////////////////////////////////////////////////////////
void sfString_SetBlendMode(sfString* string, sfBlendMode mode)
{
    CSFML_CALL(string, SetBlendMode(static_cast<sf::Blend::Mode>(mode)))
}


////////////////////////////////////////////////////////////
/// Get the X position of a string
////////////////////////////////////////////////////////////
float sfString_GetX(sfString* string)
{
    CSFML_CALL_RETURN(string, GetPosition().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the Y position of a string
////////////////////////////////////////////////////////////
float sfString_GetY(sfString* string)
{
    CSFML_CALL_RETURN(string, GetPosition().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the horizontal scale of a string
////////////////////////////////////////////////////////////
float sfString_GetScaleX(sfString* string)
{
    CSFML_CALL_RETURN(string, GetScale().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the vertical scale of a string
////////////////////////////////////////////////////////////
float sfString_GetScaleY(sfString* string)
{
    CSFML_CALL_RETURN(string, GetScale().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the orientation of a string
////////////////////////////////////////////////////////////
float sfString_GetRotation(sfString* string)
{
    CSFML_CALL_RETURN(string, GetRotation(), 0.f)
}


////////////////////////////////////////////////////////////
/// Get the X position of the origin a string
////////////////////////////////////////////////////////////
float sfString_GetOriginX(sfString* string)
{
    CSFML_CALL_RETURN(string, GetOrigin().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the top Y of the origin of a string
////////////////////////////////////////////////////////////
float sfString_GetOriginY(sfString* string)
{
    CSFML_CALL_RETURN(string, GetOrigin().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the color of a string
////////////////////////////////////////////////////////////
sfColor sfString_GetColor(sfString* string)
{
    sfColor color = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(string, color)

    sf::Color SFMLColor = string->This.GetColor();
    return sfColor_FromRGBA(SFMLColor.r, SFMLColor.g, SFMLColor.b, SFMLColor.a);
}


////////////////////////////////////////////////////////////
/// Get the current blending mode of a string
////////////////////////////////////////////////////////////
sfBlendMode sfString_GetBlendMode(sfString* string)
{
    CSFML_CHECK_RETURN(string, sfBlendNone)

    return static_cast<sfBlendMode>(string->This.GetBlendMode());
}


////////////////////////////////////////////////////////////
/// Move a string
////////////////////////////////////////////////////////////
void sfString_Move(sfString* string, float offsetX, float offsetY)
{
    CSFML_CALL(string, Move(sf::Vector2f(offsetX, offsetY)))
}


////////////////////////////////////////////////////////////
/// Scale a string
////////////////////////////////////////////////////////////
void sfString_Scale(sfString* string, float factorX, float factorY)
{
    CSFML_CALL(string, Scale(sf::Vector2f(factorX, factorY)))
}


////////////////////////////////////////////////////////////
/// Rotate a string
////////////////////////////////////////////////////////////
void sfString_Rotate(sfString* string, float angle)
{
    CSFML_CALL(string, Rotate(angle))
}


////////////////////////////////////////////////////////////
/// Transform a point from global coordinates into the string's local coordinates
/// (ie it applies the inverse of object's origin, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
void sfString_TransformToLocal(sfString* string, float pointX, float pointY, float* x, float* y)
{
    CSFML_CHECK(string)

    sf::Vector2f point = string->This.TransformToLocal(sf::Vector2f(pointX, pointY));
    if (x) *x = point.x;
    if (y) *y = point.y;
}


////////////////////////////////////////////////////////////
/// Transform a point from the string's local coordinates into global coordinates
/// (ie it applies the object's origin, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
void sfString_TransformToGlobal(sfString* string, float pointX, float pointY, float* x, float* y)
{
    CSFML_CHECK(string)

    sf::Vector2f point = string->This.TransformToGlobal(sf::Vector2f(pointX, pointY));
    if (x) *x = point.x;
    if (y) *y = point.y;
}


////////////////////////////////////////////////////////////
/// Set the text of a string (from a multibyte string)
////////////////////////////////////////////////////////////
void sfString_SetText(sfString* string, const char* text)
{
    CSFML_CALL(string, SetText(text))
}


////////////////////////////////////////////////////////////
/// Set the text of a string (from a unicode string)
////////////////////////////////////////////////////////////
void sfString_SetUnicodeText(sfString* string, const sfUint32* text)
{
    sf::Unicode::UTF32String UTF32Text = text;
    CSFML_CALL(string, SetText(UTF32Text))
}


////////////////////////////////////////////////////////////
/// Set the font of a string
////////////////////////////////////////////////////////////
void sfString_SetFont(sfString* string, sfFont* font)
{
    CSFML_CHECK(font);

    CSFML_CALL(string, SetFont(font->This))
    string->Font = font;
}


////////////////////////////////////////////////////////////
/// Set the size of a string
////////////////////////////////////////////////////////////
void sfString_SetSize(sfString* string, float size)
{
    CSFML_CALL(string, SetSize(size))
}


////////////////////////////////////////////////////////////
/// Set the style of a string
////////////////////////////////////////////////////////////
void sfString_SetStyle(sfString* string, unsigned long style)
{
    CSFML_CALL(string, SetStyle(style))
}


////////////////////////////////////////////////////////////
/// Get the text of a string (returns a unicode string)
////////////////////////////////////////////////////////////
const sfUint32* sfString_GetUnicodeText(sfString* string)
{
    CSFML_CHECK_RETURN(string, NULL)

    return static_cast<const sf::Unicode::UTF32String&>(string->This.GetText()).c_str();
}


////////////////////////////////////////////////////////////
/// Get the text of a string (returns an ANSI string)
////////////////////////////////////////////////////////////
const char* sfString_GetText(sfString* string)
{
    CSFML_CHECK_RETURN(string, NULL)

    string->Text = string->This.GetText();

    return string->Text.c_str();
}


////////////////////////////////////////////////////////////
/// Get the font used by a string
////////////////////////////////////////////////////////////
sfFont* sfString_GetFont(sfString* string)
{
    CSFML_CHECK_RETURN(string, NULL)

    return string->Font;
}


////////////////////////////////////////////////////////////
/// Get the size of the characters of a string
////////////////////////////////////////////////////////////
float sfString_GetSize(sfString* string)
{
    CSFML_CALL_RETURN(string, GetSize(), 0.f)
}


////////////////////////////////////////////////////////////
/// Get the style of a string
////////////////////////////////////////////////////////////
unsigned long sfString_GetStyle(sfString* string)
{
    CSFML_CALL_RETURN(string, GetStyle(), 0)
}


////////////////////////////////////////////////////////////
/// Return the visual position of the Index-th character of the string,
/// in coordinates relative to the string
/// (note : translation, origin, rotation and scale are not applied)
////////////////////////////////////////////////////////////
void sfString_GetCharacterPos(sfString* string, size_t index, float* x, float* y)
{
    CSFML_CHECK(string);

    sf::Vector2f pos = string->This.GetCharacterPos(index);
    if (x) *x = pos.x;
    if (y) *y = pos.y;
}


////////////////////////////////////////////////////////////
/// Get the bounding rectangle of a string on screen
////////////////////////////////////////////////////////////
sfFloatRect sfString_GetRect(sfString* string)
{
    sfFloatRect rect = {0.f, 0.f, 0.f, 0.f};
    CSFML_CHECK_RETURN(string, rect)

    sf::FloatRect SFMLRect = string->This.GetRect();
    string->Rect.Left   = SFMLRect.Left;
    string->Rect.Top    = SFMLRect.Top;
    string->Rect.Right  = SFMLRect.Right;
    string->Rect.Bottom = SFMLRect.Bottom;

    return string->Rect;
}

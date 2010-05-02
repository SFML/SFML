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
#include <SFML/Graphics/Text.h>
#include <SFML/Graphics/TextStruct.h>
#include <SFML/Graphics/Font.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Create a new text
////////////////////////////////////////////////////////////
sfText* sfText_Create(void)
{
    sfText* text = new sfText;
    text->Font = sfFont_GetDefaultFont();

    return text;
}


////////////////////////////////////////////////////////////
/// Copy an existing text
////////////////////////////////////////////////////////////
sfText* sfText_Copy(sfText* text)
{
    CSFML_CHECK_RETURN(text, NULL);

    return new sfText(*text);
}


////////////////////////////////////////////////////////////
/// Destroy an existing text
////////////////////////////////////////////////////////////
void sfText_Destroy(sfText* text)
{
    delete text;
}


////////////////////////////////////////////////////////////
/// Set the X position of a text
////////////////////////////////////////////////////////////
void sfText_SetX(sfText* text, float x)
{
    CSFML_CALL(text, SetX(x))
}


////////////////////////////////////////////////////////////
/// Set the Y position of a text
////////////////////////////////////////////////////////////
void sfText_SetY(sfText* text, float y)
{
    CSFML_CALL(text, SetY(y))
}


////////////////////////////////////////////////////////////
/// Set the position of a text
////////////////////////////////////////////////////////////
void sfText_SetPosition(sfText* text, float x, float y)
{
    CSFML_CALL(text, SetPosition(sf::Vector2f(x, y)))
}


////////////////////////////////////////////////////////////
/// Set the horizontal scale of a text
////////////////////////////////////////////////////////////
void sfText_SetScaleX(sfText* text, float scale)
{
    CSFML_CALL(text, SetScaleX(scale))
}


////////////////////////////////////////////////////////////
/// Set the vertical scale of a text
////////////////////////////////////////////////////////////
void sfText_SetScaleY(sfText* text, float scale)
{
    CSFML_CALL(text, SetScaleY(scale))
}


////////////////////////////////////////////////////////////
/// Set the scale of a text
////////////////////////////////////////////////////////////
void sfText_SetScale(sfText* text, float scaleX, float scaleY)
{
    CSFML_CALL(text, SetScale(sf::Vector2f(scaleX, scaleY)))
}


////////////////////////////////////////////////////////////
/// Set the orientation of a text
////////////////////////////////////////////////////////////
void sfText_SetRotation(sfText* text, float rotation)
{
    CSFML_CALL(text, SetRotation(rotation))
}


////////////////////////////////////////////////////////////
/// Set the local origin of a text, in coordinates
/// relative to its left-top corner
////////////////////////////////////////////////////////////
void sfText_SetOrigin(sfText* text, float x, float y)
{
    CSFML_CALL(text, SetOrigin(sf::Vector2f(x, y)))
}


////////////////////////////////////////////////////////////
/// Set the color of a text
////////////////////////////////////////////////////////////
void sfText_SetColor(sfText* text, sfColor color)
{
    CSFML_CALL(text, SetColor(sf::Color(color.r, color.g, color.b, color.a)))
}


////////////////////////////////////////////////////////////
/// Set the blending mode for a text
////////////////////////////////////////////////////////////
void sfText_SetBlendMode(sfText* text, sfBlendMode mode)
{
    CSFML_CALL(text, SetBlendMode(static_cast<sf::Blend::Mode>(mode)))
}


////////////////////////////////////////////////////////////
/// Get the X position of a text
////////////////////////////////////////////////////////////
float sfText_GetX(const sfText* text)
{
    CSFML_CALL_RETURN(text, GetPosition().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the Y position of a text
////////////////////////////////////////////////////////////
float sfText_GetY(const sfText* text)
{
    CSFML_CALL_RETURN(text, GetPosition().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the horizontal scale of a text
////////////////////////////////////////////////////////////
float sfText_GetScaleX(const sfText* text)
{
    CSFML_CALL_RETURN(text, GetScale().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the vertical scale of a text
////////////////////////////////////////////////////////////
float sfText_GetScaleY(const sfText* text)
{
    CSFML_CALL_RETURN(text, GetScale().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the orientation of a text
////////////////////////////////////////////////////////////
float sfText_GetRotation(const sfText* text)
{
    CSFML_CALL_RETURN(text, GetRotation(), 0.f)
}


////////////////////////////////////////////////////////////
/// Get the X position of the origin a text
////////////////////////////////////////////////////////////
float sfText_GetOriginX(const sfText* text)
{
    CSFML_CALL_RETURN(text, GetOrigin().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the top Y of the origin of a text
////////////////////////////////////////////////////////////
float sfText_GetOriginY(const sfText* text)
{
    CSFML_CALL_RETURN(text, GetOrigin().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the color of a text
////////////////////////////////////////////////////////////
sfColor sfText_GetColor(const sfText* text)
{
    sfColor color = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(text, color)

    sf::Color SFMLColor = text->This.GetColor();
    return sfColor_FromRGBA(SFMLColor.r, SFMLColor.g, SFMLColor.b, SFMLColor.a);
}


////////////////////////////////////////////////////////////
/// Get the current blending mode of a text
////////////////////////////////////////////////////////////
sfBlendMode sfText_GetBlendMode(const sfText* text)
{
    CSFML_CHECK_RETURN(text, sfBlendNone)

    return static_cast<sfBlendMode>(text->This.GetBlendMode());
}


////////////////////////////////////////////////////////////
/// Move a text
////////////////////////////////////////////////////////////
void sfText_Move(sfText* text, float offsetX, float offsetY)
{
    CSFML_CALL(text, Move(sf::Vector2f(offsetX, offsetY)))
}


////////////////////////////////////////////////////////////
/// Scale a text
////////////////////////////////////////////////////////////
void sfText_Scale(sfText* text, float factorX, float factorY)
{
    CSFML_CALL(text, Scale(sf::Vector2f(factorX, factorY)))
}


////////////////////////////////////////////////////////////
/// Rotate a text
////////////////////////////////////////////////////////////
void sfText_Rotate(sfText* text, float angle)
{
    CSFML_CALL(text, Rotate(angle))
}


////////////////////////////////////////////////////////////
/// Transform a point from global coordinates into the text's local coordinates
/// (ie it applies the inverse of object's origin, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
void sfText_TransformToLocal(const sfText* text, float pointX, float pointY, float* x, float* y)
{
    CSFML_CHECK(text)

    sf::Vector2f point = text->This.TransformToLocal(sf::Vector2f(pointX, pointY));
    if (x) *x = point.x;
    if (y) *y = point.y;
}


////////////////////////////////////////////////////////////
/// Transform a point from the text's local coordinates into global coordinates
/// (ie it applies the object's origin, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
void sfText_TransformToGlobal(const sfText* text, float pointX, float pointY, float* x, float* y)
{
    CSFML_CHECK(text)

    sf::Vector2f point = text->This.TransformToGlobal(sf::Vector2f(pointX, pointY));
    if (x) *x = point.x;
    if (y) *y = point.y;
}


////////////////////////////////////////////////////////////
/// Set the string of a text (from a multibyte string)
////////////////////////////////////////////////////////////
void sfText_SetString(sfText* text, const char* string)
{
    CSFML_CALL(text, SetString(string))
}


////////////////////////////////////////////////////////////
/// Set the string of a text (from a unicode string)
////////////////////////////////////////////////////////////
void sfText_SetUnicodeString(sfText* text, const sfUint32* string)
{
    sf::String UTF32Text = string;
    CSFML_CALL(text, SetString(UTF32Text))
}


////////////////////////////////////////////////////////////
/// Set the font of a string
////////////////////////////////////////////////////////////
void sfText_SetFont(sfText* text, const sfFont* font)
{
    CSFML_CHECK(font);

    CSFML_CALL(text, SetFont(font->This))
    text->Font = font;
}


////////////////////////////////////////////////////////////
/// Set the size of a string
////////////////////////////////////////////////////////////
void sfText_SetCharacterSize(sfText* text, unsigned int size)
{
    CSFML_CALL(text, SetCharacterSize(size))
}


////////////////////////////////////////////////////////////
/// Set the style of a string
////////////////////////////////////////////////////////////
void sfText_SetStyle(sfText* text, unsigned long style)
{
    CSFML_CALL(text, SetStyle(style))
}


////////////////////////////////////////////////////////////
/// Get the string of a text (returns a unicode string)
////////////////////////////////////////////////////////////
const sfUint32* sfText_GetUnicodeString(const sfText* text)
{
    CSFML_CHECK_RETURN(text, NULL)

    return text->This.GetString().GetData();
}


////////////////////////////////////////////////////////////
/// Get the string of a text (returns an ANSI string)
////////////////////////////////////////////////////////////
const char* sfText_GetString(const sfText* text)
{
    CSFML_CHECK_RETURN(text, NULL)

    text->String = text->This.GetString().ToAnsiString();

    return text->String.c_str();
}


////////////////////////////////////////////////////////////
/// Get the font used by a text
////////////////////////////////////////////////////////////
const sfFont* sfText_GetFont(const sfText* text)
{
    CSFML_CHECK_RETURN(text, NULL)

    return text->Font;
}


////////////////////////////////////////////////////////////
/// Get the size of the characters of a text
////////////////////////////////////////////////////////////
unsigned int sfText_GetCharacterSize(const sfText* text)
{
    CSFML_CALL_RETURN(text, GetCharacterSize(), 0)
}


////////////////////////////////////////////////////////////
/// Get the style of a text
////////////////////////////////////////////////////////////
unsigned long sfText_GetStyle(const sfText* text)
{
    CSFML_CALL_RETURN(text, GetStyle(), 0)
}


////////////////////////////////////////////////////////////
/// Return the visual position of the Index-th character of the text,
/// in coordinates relative to the text
/// (note : translation, origin, rotation and scale are not applied)
////////////////////////////////////////////////////////////
void sfText_GetCharacterPos(const sfText* text, size_t index, float* x, float* y)
{
    CSFML_CHECK(text);

    sf::Vector2f pos = text->This.GetCharacterPos(index);
    if (x) *x = pos.x;
    if (y) *y = pos.y;
}


////////////////////////////////////////////////////////////
/// Get the bounding rectangle of a text on screen
////////////////////////////////////////////////////////////
sfFloatRect sfText_GetRect(const sfText* text)
{
    sfFloatRect rect = {0.f, 0.f, 0.f, 0.f};
    CSFML_CHECK_RETURN(text, rect)

    sf::FloatRect SFMLRect = text->This.GetRect();
    text->Rect.Left   = SFMLRect.Left;
    text->Rect.Top    = SFMLRect.Top;
    text->Rect.Width  = SFMLRect.Width;
    text->Rect.Height = SFMLRect.Height;

    return text->Rect;
}

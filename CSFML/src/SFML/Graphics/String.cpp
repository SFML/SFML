////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2008 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Graphics/String.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Internal.h>


// WARNING : this structure must always be the SAME as in Graphics/Font.h
struct sfFont
{
    sf::Font This;
};

struct sfString
{
    sf::String  This;
    std::string Text;
    sfFont*     Font;
    sfFloatRect Rect;
};


////////////////////////////////////////////////////////////
/// Create a new string
////////////////////////////////////////////////////////////
sfString* sfString_Create()
{
    sfString* String = new sfString;
    String->Font = sfFont_GetDefaultFont();

    return String;
}


////////////////////////////////////////////////////////////
/// Destroy an existing string
////////////////////////////////////////////////////////////
void sfString_Destroy(sfString* String)
{
    delete String;
}


////////////////////////////////////////////////////////////
/// Set the X position of a string
////////////////////////////////////////////////////////////
void sfString_SetX(sfString* String, float X)
{
    CSFML_CALL(String, SetX(X))
}


////////////////////////////////////////////////////////////
/// Set the Y position of a string
////////////////////////////////////////////////////////////
void sfString_SetY(sfString* String, float Y)
{
    CSFML_CALL(String, SetY(Y))
}


////////////////////////////////////////////////////////////
/// Set the position of a string
////////////////////////////////////////////////////////////
void sfString_SetPosition(sfString* String, float X, float Y)
{
    CSFML_CALL(String, SetPosition(sf::Vector2f(X, Y)))
}


////////////////////////////////////////////////////////////
/// Set the horizontal scale of a string
////////////////////////////////////////////////////////////
void sfString_SetScaleX(sfString* String, float Scale)
{
    CSFML_CALL(String, SetScaleX(Scale))
}


////////////////////////////////////////////////////////////
/// Set the vertical scale of a string
////////////////////////////////////////////////////////////
void sfString_SetScaleY(sfString* String, float Scale)
{
    CSFML_CALL(String, SetScaleY(Scale))
}


////////////////////////////////////////////////////////////
/// Set the scale of a string
////////////////////////////////////////////////////////////
void sfString_SetScale(sfString* String, float ScaleX, float ScaleY)
{
    CSFML_CALL(String, SetScale(sf::Vector2f(ScaleX, ScaleY)))
}


////////////////////////////////////////////////////////////
/// Set the orientation of a string
////////////////////////////////////////////////////////////
void sfString_SetRotation(sfString* String, float Rotation)
{
    CSFML_CALL(String, SetRotation(Rotation))
}


////////////////////////////////////////////////////////////
/// Set the center of a string, in coordinates
/// relative to its left-top corner
////////////////////////////////////////////////////////////
void sfString_SetCenter(sfString* String, float X, float Y)
{
    CSFML_CALL(String, SetCenter(sf::Vector2f(X, Y)))
}


////////////////////////////////////////////////////////////
/// Set the color of a string
////////////////////////////////////////////////////////////
void sfString_SetColor(sfString* String, sfColor Color)
{
    CSFML_CALL(String, SetColor(sf::Color(Color.r, Color.g, Color.b, Color.a)))
}


////////////////////////////////////////////////////////////
/// Set the blending mode for a string
////////////////////////////////////////////////////////////
void sfString_SetBlendMode(sfString* String, sfBlendMode Mode)
{
    CSFML_CALL(String, SetBlendMode(static_cast<sf::Blend::Mode>(Mode)))
}


////////////////////////////////////////////////////////////
/// Get the X position of a string
////////////////////////////////////////////////////////////
float sfString_GetX(sfString* String)
{
    CSFML_CALL_RETURN(String, GetPosition().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the Y position of a string
////////////////////////////////////////////////////////////
float sfString_GetY(sfString* String)
{
    CSFML_CALL_RETURN(String, GetPosition().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the horizontal scale of a string
////////////////////////////////////////////////////////////
float sfString_GetScaleX(sfString* String)
{
    CSFML_CALL_RETURN(String, GetScale().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the vertical scale of a string
////////////////////////////////////////////////////////////
float sfString_GetScaleY(sfString* String)
{
    CSFML_CALL_RETURN(String, GetScale().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the orientation of a string
////////////////////////////////////////////////////////////
float sfString_GetRotation(sfString* String)
{
    CSFML_CALL_RETURN(String, GetRotation(), 0.f)
}


////////////////////////////////////////////////////////////
/// Get the X position of the center a string
////////////////////////////////////////////////////////////
float sfString_GetCenterX(sfString* String)
{
    CSFML_CALL_RETURN(String, GetCenter().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the top Y of the center of a string
////////////////////////////////////////////////////////////
float sfString_GetCenterY(sfString* String)
{
    CSFML_CALL_RETURN(String, GetCenter().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the color of a string
////////////////////////////////////////////////////////////
sfColor sfString_GetColor(sfString* String)
{
    sfColor Color = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(String, Color)

    sf::Color SFMLColor = String->This.GetColor();
    return sfColor_FromRGBA(SFMLColor.r, SFMLColor.g, SFMLColor.b, SFMLColor.a);
}


////////////////////////////////////////////////////////////
/// Get the current blending mode of a string
////////////////////////////////////////////////////////////
sfBlendMode sfString_GetBlendMode(sfString* String)
{
    CSFML_CHECK_RETURN(String, sfBlendNone)

    return static_cast<sfBlendMode>(String->This.GetBlendMode());
}


////////////////////////////////////////////////////////////
/// Move a string
////////////////////////////////////////////////////////////
void sfString_Move(sfString* String, float OffsetX, float OffsetY)
{
    CSFML_CALL(String, Move(sf::Vector2f(OffsetX, OffsetY)))
}


////////////////////////////////////////////////////////////
/// Scale a string
////////////////////////////////////////////////////////////
void sfString_Scale(sfString* String, float FactorX, float FactorY)
{
    CSFML_CALL(String, Scale(sf::Vector2f(FactorX, FactorY)))
}


////////////////////////////////////////////////////////////
/// Rotate a string
////////////////////////////////////////////////////////////
void sfString_Rotate(sfString* String, float Angle)
{
    CSFML_CALL(String, Rotate(Angle))
}


////////////////////////////////////////////////////////////
/// Transform a point from global coordinates into the string's local coordinates
/// (ie it applies the inverse of object's center, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
void sfString_TransformToLocal(sfString* String, float PointX, float PointY, float* X, float* Y)
{
    CSFML_CHECK(String)

    sf::Vector2f Point = String->This.TransformToLocal(sf::Vector2f(PointX, PointY));
    if (X) *X = Point.x;
    if (Y) *Y = Point.y;
}


////////////////////////////////////////////////////////////
/// Transform a point from the string's local coordinates into global coordinates
/// (ie it applies the object's center, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
void sfString_TransformToGlobal(sfString* String, float PointX, float PointY, float* X, float* Y)
{
    CSFML_CHECK(String)

    sf::Vector2f Point = String->This.TransformToGlobal(sf::Vector2f(PointX, PointY));
    if (X) *X = Point.x;
    if (Y) *Y = Point.y;
}


////////////////////////////////////////////////////////////
/// Set the text of a string (from a multibyte string)
////////////////////////////////////////////////////////////
void sfString_SetText(sfString* String, const char* Text)
{
    CSFML_CALL(String, SetText(Text))
}


////////////////////////////////////////////////////////////
/// Set the text of a string (from a unicode string)
////////////////////////////////////////////////////////////
void sfString_SetUnicodeText(sfString* String, const sfUint32* Text)
{
    sf::Unicode::UTF32String UTF32Text = Text;
    CSFML_CALL(String, SetText(UTF32Text))
}


////////////////////////////////////////////////////////////
/// Set the font of a string
////////////////////////////////////////////////////////////
void sfString_SetFont(sfString* String, sfFont* Font)
{
    CSFML_CHECK(Font);

    CSFML_CALL(String, SetFont(Font->This))
    String->Font = Font;
}


////////////////////////////////////////////////////////////
/// Set the size of a string
////////////////////////////////////////////////////////////
void sfString_SetSize(sfString* String, float Size)
{
    CSFML_CALL(String, SetSize(Size))
}


////////////////////////////////////////////////////////////
/// Set the style of a string
////////////////////////////////////////////////////////////
void sfString_SetStyle(sfString* String, unsigned long Style)
{
    CSFML_CALL(String, SetStyle(Style))
}


////////////////////////////////////////////////////////////
/// Get the text of a string (returns a unicode string)
////////////////////////////////////////////////////////////
const sfUint32* sfString_GetUnicodeText(sfString* String)
{
    CSFML_CHECK_RETURN(String, NULL)

    return static_cast<const sf::Unicode::UTF32String&>(String->This.GetText()).c_str();
}


////////////////////////////////////////////////////////////
/// Get the text of a string (returns an ANSI string)
////////////////////////////////////////////////////////////
const char* sfString_GetText(sfString* String)
{
    CSFML_CHECK_RETURN(String, NULL)

    String->Text = String->This.GetText();

    return String->Text.c_str();
}


////////////////////////////////////////////////////////////
/// Get the font used by a string
////////////////////////////////////////////////////////////
sfFont* sfString_GetFont(sfString* String)
{
    CSFML_CHECK_RETURN(String, NULL)

    return String->Font;
}


////////////////////////////////////////////////////////////
/// Get the size of the characters of a string
////////////////////////////////////////////////////////////
float sfString_GetSize(sfString* String)
{
    CSFML_CALL_RETURN(String, GetSize(), 0.f)
}


////////////////////////////////////////////////////////////
/// Get the style of a string
////////////////////////////////////////////////////////////
unsigned long sfString_GetStyle(sfString* String)
{
    CSFML_CALL_RETURN(String, GetStyle(), 0)
}


////////////////////////////////////////////////////////////
/// Return the visual position of the Index-th character of the string,
/// in coordinates relative to the string
/// (note : translation, center, rotation and scale are not applied)
////////////////////////////////////////////////////////////
void sfString_GetCharacterPos(sfString* String, size_t Index, float* X, float* Y)
{
    CSFML_CHECK(String);

    sf::Vector2f Pos = String->This.GetCharacterPos(Index);
    if (X) *X = Pos.x;
    if (Y) *Y = Pos.y;
}


////////////////////////////////////////////////////////////
/// Get the bounding rectangle of a string on screen
////////////////////////////////////////////////////////////
sfFloatRect sfString_GetRect(sfString* String)
{
    sfFloatRect Rect = {0.f, 0.f, 0.f, 0.f};
    CSFML_CHECK_RETURN(String, Rect)

    sf::FloatRect SFMLRect = String->This.GetRect();
    String->Rect.Left   = SFMLRect.Left;
    String->Rect.Top    = SFMLRect.Top;
    String->Rect.Right  = SFMLRect.Right;
    String->Rect.Bottom = SFMLRect.Bottom;

    return String->Rect;
}

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
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Internal.h>


// WARNING : this structure must always be the SAME as in Graphics/Image.h
struct sfImage
{
    sf::Image This;
};
struct sfSprite
{
    sf::Sprite This;
    sfImage*   Image;
    sfIntRect  SubRect;
};


////////////////////////////////////////////////////////////
/// Create a new sprite
////////////////////////////////////////////////////////////
sfSprite* sfSprite_Create()
{
    sfSprite* Sprite       = new sfSprite;
    Sprite->Image          = NULL;
    Sprite->SubRect.Left   = Sprite->This.GetSubRect().Left;
    Sprite->SubRect.Top    = Sprite->This.GetSubRect().Top;
    Sprite->SubRect.Right  = Sprite->This.GetSubRect().Right;
    Sprite->SubRect.Bottom = Sprite->This.GetSubRect().Bottom;

    return Sprite;
}


////////////////////////////////////////////////////////////
/// Destroy an existing sprite
////////////////////////////////////////////////////////////
void sfSprite_Destroy(sfSprite* Sprite)
{
    delete Sprite;
}


////////////////////////////////////////////////////////////
/// Set the X position of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetX(sfSprite* Sprite, float X)
{
    CSFML_CALL(Sprite, SetX(X))
}


////////////////////////////////////////////////////////////
/// Set the Y position of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetY(sfSprite* Sprite, float Y)
{
    CSFML_CALL(Sprite, SetY(Y))
}


////////////////////////////////////////////////////////////
/// Set the position of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetPosition(sfSprite* Sprite, float X, float Y)
{
    CSFML_CALL(Sprite, SetPosition(sf::Vector2f(X, Y)))
}


////////////////////////////////////////////////////////////
/// Set the horizontal scale of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetScaleX(sfSprite* Sprite, float Scale)
{
    CSFML_CALL(Sprite, SetScaleX(Scale))
}


////////////////////////////////////////////////////////////
/// Set the vertical scale of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetScaleY(sfSprite* Sprite, float Scale)
{
    CSFML_CALL(Sprite, SetScaleY(Scale))
}


////////////////////////////////////////////////////////////
/// Set the scale of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetScale(sfSprite* Sprite, float ScaleX, float ScaleY)
{
    CSFML_CALL(Sprite, SetScale(sf::Vector2f(ScaleX, ScaleY)))
}


////////////////////////////////////////////////////////////
/// Set the orientation of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetRotation(sfSprite* Sprite, float Rotation)
{
    CSFML_CALL(Sprite, SetRotation(Rotation))
}


////////////////////////////////////////////////////////////
/// Set the center of a sprite, in coordinates
/// relative to its left-top corner
////////////////////////////////////////////////////////////
void sfSprite_SetCenter(sfSprite* Sprite, float X, float Y)
{
    CSFML_CALL(Sprite, SetCenter(sf::Vector2f(X, Y)))
}


////////////////////////////////////////////////////////////
/// Set the color of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetColor(sfSprite* Sprite, sfColor Color)
{
    CSFML_CALL(Sprite, SetColor(sf::Color(Color.r, Color.g, Color.b, Color.a)))
}


////////////////////////////////////////////////////////////
/// Set the blending mode for a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetBlendMode(sfSprite* Sprite, sfBlendMode Mode)
{
    CSFML_CALL(Sprite, SetBlendMode(static_cast<sf::Blend::Mode>(Mode)))
}


////////////////////////////////////////////////////////////
/// Get the X position of a sprite
////////////////////////////////////////////////////////////
float sfSprite_GetX(sfSprite* Sprite)
{
    CSFML_CALL_RETURN(Sprite, GetPosition().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the Y position of a sprite
////////////////////////////////////////////////////////////
float sfSprite_GetY(sfSprite* Sprite)
{
    CSFML_CALL_RETURN(Sprite, GetPosition().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the horizontal scale of a sprite
////////////////////////////////////////////////////////////
float sfSprite_GetScaleX(sfSprite* Sprite)
{
    CSFML_CALL_RETURN(Sprite, GetScale().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the vertical scale of a sprite
////////////////////////////////////////////////////////////
float sfSprite_GetScaleY(sfSprite* Sprite)
{
    CSFML_CALL_RETURN(Sprite, GetScale().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the orientation of a sprite
////////////////////////////////////////////////////////////
float sfSprite_GetRotation(sfSprite* Sprite)
{
    CSFML_CALL_RETURN(Sprite, GetRotation(), 0.f)
}


////////////////////////////////////////////////////////////
/// Get the X position of the center a sprite
////////////////////////////////////////////////////////////
float sfSprite_GetCenterX(sfSprite* Sprite)
{
    CSFML_CALL_RETURN(Sprite, GetCenter().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the Y position of the center a sprite
////////////////////////////////////////////////////////////
float sfSprite_GetCenterY(sfSprite* Sprite)
{
    CSFML_CALL_RETURN(Sprite, GetCenter().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the color of a sprite
////////////////////////////////////////////////////////////
sfColor sfSprite_GetColor(sfSprite* Sprite)
{
    sfColor Color = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(Sprite, Color)

    sf::Color SFMLColor = Sprite->This.GetColor();
    return sfColor_FromRGBA(SFMLColor.r, SFMLColor.g, SFMLColor.b, SFMLColor.a);
}


////////////////////////////////////////////////////////////
/// Get the current blending mode of a sprite
////////////////////////////////////////////////////////////
sfBlendMode sfSprite_GetBlendMode(sfSprite* Sprite)
{
    CSFML_CHECK_RETURN(Sprite, sfBlendNone)

    return static_cast<sfBlendMode>(Sprite->This.GetBlendMode());
}


////////////////////////////////////////////////////////////
/// Move a sprite
////////////////////////////////////////////////////////////
void sfSprite_Move(sfSprite* Sprite, float OffsetX, float OffsetY)
{
    CSFML_CALL(Sprite, Move(sf::Vector2f(OffsetX, OffsetY)))
}


////////////////////////////////////////////////////////////
/// Scale a sprite
////////////////////////////////////////////////////////////
void sfSprite_Scale(sfSprite* Sprite, float FactorX, float FactorY)
{
    CSFML_CALL(Sprite, Scale(sf::Vector2f(FactorX, FactorY)))
}


////////////////////////////////////////////////////////////
/// Rotate a sprite
////////////////////////////////////////////////////////////
void sfSprite_Rotate(sfSprite* Sprite, float Angle)
{
    CSFML_CALL(Sprite, Rotate(Angle))
}


////////////////////////////////////////////////////////////
/// Transform a point from global coordinates into the sprite's local coordinates
/// (ie it applies the inverse of object's center, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
void sfSprite_TransformToLocal(sfSprite* Sprite, float PointX, float PointY, float* X, float* Y)
{
    CSFML_CHECK(Sprite)

    sf::Vector2f Point = Sprite->This.TransformToLocal(sf::Vector2f(PointX, PointY));
    if (X) *X = Point.x;
    if (Y) *Y = Point.y;
}


////////////////////////////////////////////////////////////
/// Transform a point from the sprite's local coordinates into global coordinates
/// (ie it applies the object's center, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
void sfSprite_TransformToGlobal(sfSprite* Sprite, float PointX, float PointY, float* X, float* Y)
{
    CSFML_CHECK(Sprite)

    sf::Vector2f Point = Sprite->This.TransformToGlobal(sf::Vector2f(PointX, PointY));
    if (X) *X = Point.x;
    if (Y) *Y = Point.y;
}


////////////////////////////////////////////////////////////
/// Change the image of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetImage(sfSprite* Sprite, sfImage* Image)
{
    if (Image)
    {
        CSFML_CALL(Sprite, SetImage(Image->This))
        Sprite->Image = Image;
    }
}


////////////////////////////////////////////////////////////
/// Set the sub-rectangle of a sprite inside the source image
////////////////////////////////////////////////////////////
void sfSprite_SetSubRect(sfSprite* Sprite, sfIntRect SubRect)
{
    CSFML_CALL(Sprite, SetSubRect(sf::IntRect(SubRect.Left, SubRect.Top, SubRect.Right, SubRect.Bottom)))
    Sprite->SubRect = SubRect;
}


////////////////////////////////////////////////////////////
/// Resize a sprite (by changing its scale factors)
////////////////////////////////////////////////////////////
void sfSprite_Resize(sfSprite* Sprite, float Width, float Height)
{
    CSFML_CALL(Sprite, Resize(sf::Vector2f(Width, Height)))
}


////////////////////////////////////////////////////////////
/// Flip a sprite horizontally
////////////////////////////////////////////////////////////
void sfSprite_FlipX(sfSprite* Sprite, sfBool Flipped)
{
    CSFML_CALL(Sprite, FlipX(Flipped == sfTrue))
}


////////////////////////////////////////////////////////////
/// Flip a sprite vertically
////////////////////////////////////////////////////////////
void sfSprite_FlipY(sfSprite* Sprite, sfBool Flipped)
{
    CSFML_CALL(Sprite, FlipY(Flipped == sfTrue))
}


////////////////////////////////////////////////////////////
/// Get the source image of a sprite
////////////////////////////////////////////////////////////
sfImage* sfSprite_GetImage(sfSprite* Sprite)
{
    CSFML_CHECK_RETURN(Sprite, NULL)

    return Sprite->Image;
}


////////////////////////////////////////////////////////////
/// Get the sub-rectangle of a sprite inside the source image
////////////////////////////////////////////////////////////
sfIntRect sfSprite_GetSubRect(sfSprite* Sprite)
{
    sfIntRect Rect = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(Sprite, Rect)

    return Sprite->SubRect;
}


////////////////////////////////////////////////////////////
/// Get a sprite width
////////////////////////////////////////////////////////////
float sfSprite_GetWidth(sfSprite* Sprite)
{
    CSFML_CALL_RETURN(Sprite, GetSize().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get a sprite height
////////////////////////////////////////////////////////////
float sfSprite_GetHeight(sfSprite* Sprite)
{
    CSFML_CALL_RETURN(Sprite, GetSize().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the color of a given pixel in a sprite
////////////////////////////////////////////////////////////
sfColor sfSprite_GetPixel(sfSprite* Sprite, unsigned int X, unsigned int Y)
{
    sfColor Color = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(Sprite, Color)

    sf::Color SFMLColor = Sprite->This.GetPixel(X, Y);
    return sfColor_FromRGBA(SFMLColor.r, SFMLColor.g, SFMLColor.b, SFMLColor.a);
}

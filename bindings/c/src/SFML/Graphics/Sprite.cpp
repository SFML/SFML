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
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/SpriteStruct.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Create a new sprite
////////////////////////////////////////////////////////////
sfSprite* sfSprite_Create(void)
{
    sfSprite* sprite       = new sfSprite;
    sprite->Image          = NULL;
    sprite->SubRect.Left   = sprite->This.GetSubRect().Left;
    sprite->SubRect.Top    = sprite->This.GetSubRect().Top;
    sprite->SubRect.Width  = sprite->This.GetSubRect().Width;
    sprite->SubRect.Height = sprite->This.GetSubRect().Height;

    return sprite;
}


////////////////////////////////////////////////////////////
/// Copy an existing sprite
////////////////////////////////////////////////////////////
sfSprite* sfSprite_Copy(sfSprite* sprite)
{
    CSFML_CHECK_RETURN(sprite, NULL);

    return new sfSprite(*sprite);
}


////////////////////////////////////////////////////////////
/// Destroy an existing sprite
////////////////////////////////////////////////////////////
void sfSprite_Destroy(sfSprite* sprite)
{
    delete sprite;
}


////////////////////////////////////////////////////////////
/// Set the X position of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetX(sfSprite* sprite, float x)
{
    CSFML_CALL(sprite, SetX(x))
}


////////////////////////////////////////////////////////////
/// Set the Y position of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetY(sfSprite* sprite, float y)
{
    CSFML_CALL(sprite, SetY(y))
}


////////////////////////////////////////////////////////////
/// Set the position of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetPosition(sfSprite* sprite, float x, float y)
{
    CSFML_CALL(sprite, SetPosition(sf::Vector2f(x, y)))
}


////////////////////////////////////////////////////////////
/// Set the horizontal scale of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetScaleX(sfSprite* sprite, float scale)
{
    CSFML_CALL(sprite, SetScaleX(scale))
}


////////////////////////////////////////////////////////////
/// Set the vertical scale of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetScaleY(sfSprite* sprite, float scale)
{
    CSFML_CALL(sprite, SetScaleY(scale))
}


////////////////////////////////////////////////////////////
/// Set the scale of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetScale(sfSprite* sprite, float scaleX, float scaleY)
{
    CSFML_CALL(sprite, SetScale(sf::Vector2f(scaleX, scaleY)))
}


////////////////////////////////////////////////////////////
/// Set the orientation of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetRotation(sfSprite* sprite, float rotation)
{
    CSFML_CALL(sprite, SetRotation(rotation))
}


////////////////////////////////////////////////////////////
/// Set the local origin of a sprite, in coordinates
/// relative to its left-top corner
////////////////////////////////////////////////////////////
void sfSprite_SetOrigin(sfSprite* sprite, float x, float y)
{
    CSFML_CALL(sprite, SetOrigin(sf::Vector2f(x, y)))
}


////////////////////////////////////////////////////////////
/// Set the color of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetColor(sfSprite* sprite, sfColor color)
{
    CSFML_CALL(sprite, SetColor(sf::Color(color.r, color.g, color.b, color.a)))
}


////////////////////////////////////////////////////////////
/// Set the blending mode for a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetBlendMode(sfSprite* sprite, sfBlendMode mode)
{
    CSFML_CALL(sprite, SetBlendMode(static_cast<sf::Blend::Mode>(mode)))
}


////////////////////////////////////////////////////////////
/// Get the X position of a sprite
////////////////////////////////////////////////////////////
float sfSprite_GetX(const sfSprite* sprite)
{
    CSFML_CALL_RETURN(sprite, GetPosition().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the Y position of a sprite
////////////////////////////////////////////////////////////
float sfSprite_GetY(const sfSprite* sprite)
{
    CSFML_CALL_RETURN(sprite, GetPosition().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the horizontal scale of a sprite
////////////////////////////////////////////////////////////
float sfSprite_GetScaleX(const sfSprite* sprite)
{
    CSFML_CALL_RETURN(sprite, GetScale().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the vertical scale of a sprite
////////////////////////////////////////////////////////////
float sfSprite_GetScaleY(const sfSprite* sprite)
{
    CSFML_CALL_RETURN(sprite, GetScale().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the orientation of a sprite
////////////////////////////////////////////////////////////
float sfSprite_GetRotation(const sfSprite* sprite)
{
    CSFML_CALL_RETURN(sprite, GetRotation(), 0.f)
}


////////////////////////////////////////////////////////////
/// Get the X position of the origin a sprite
////////////////////////////////////////////////////////////
float sfSprite_GetOriginX(const sfSprite* sprite)
{
    CSFML_CALL_RETURN(sprite, GetOrigin().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the Y position of the origin a sprite
////////////////////////////////////////////////////////////
float sfSprite_GetOriginY(const sfSprite* sprite)
{
    CSFML_CALL_RETURN(sprite, GetOrigin().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the color of a sprite
////////////////////////////////////////////////////////////
sfColor sfSprite_GetColor(const sfSprite* sprite)
{
    sfColor color = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(sprite, color)

    sf::Color SFMLColor = sprite->This.GetColor();
    return sfColor_FromRGBA(SFMLColor.r, SFMLColor.g, SFMLColor.b, SFMLColor.a);
}


////////////////////////////////////////////////////////////
/// Get the current blending mode of a sprite
////////////////////////////////////////////////////////////
sfBlendMode sfSprite_GetBlendMode(const sfSprite* sprite)
{
    CSFML_CHECK_RETURN(sprite, sfBlendNone)

    return static_cast<sfBlendMode>(sprite->This.GetBlendMode());
}


////////////////////////////////////////////////////////////
/// Move a sprite
////////////////////////////////////////////////////////////
void sfSprite_Move(sfSprite* sprite, float offsetX, float offsetY)
{
    CSFML_CALL(sprite, Move(sf::Vector2f(offsetX, offsetY)))
}


////////////////////////////////////////////////////////////
/// Scale a sprite
////////////////////////////////////////////////////////////
void sfSprite_Scale(sfSprite* sprite, float factorX, float factorY)
{
    CSFML_CALL(sprite, Scale(sf::Vector2f(factorX, factorY)))
}


////////////////////////////////////////////////////////////
/// Rotate a sprite
////////////////////////////////////////////////////////////
void sfSprite_Rotate(sfSprite* sprite, float angle)
{
    CSFML_CALL(sprite, Rotate(angle))
}


////////////////////////////////////////////////////////////
/// Transform a point from global coordinates into the sprite's local coordinates
/// (ie it applies the inverse of object's origin, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
void sfSprite_TransformToLocal(const sfSprite* sprite, float pointX, float pointY, float* x, float* y)
{
    CSFML_CHECK(sprite)

    sf::Vector2f point = sprite->This.TransformToLocal(sf::Vector2f(pointX, pointY));
    if (x) *x = point.x;
    if (y) *y = point.y;
}


////////////////////////////////////////////////////////////
/// Transform a point from the sprite's local coordinates into global coordinates
/// (ie it applies the object's origin, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
void sfSprite_TransformToGlobal(const sfSprite* sprite, float pointX, float pointY, float* x, float* y)
{
    CSFML_CHECK(sprite)

    sf::Vector2f point = sprite->This.TransformToGlobal(sf::Vector2f(pointX, pointY));
    if (x) *x = point.x;
    if (y) *y = point.y;
}


////////////////////////////////////////////////////////////
/// Change the image of a sprite
////////////////////////////////////////////////////////////
void sfSprite_SetImage(sfSprite* sprite, const sfImage* image, sfBool adjustToNewSize)
{
    if (image)
    {
        CSFML_CALL(sprite, SetImage(*image->This, adjustToNewSize == sfTrue))
        sprite->Image = image;
    }
}


////////////////////////////////////////////////////////////
/// Set the sub-rectangle of a sprite inside the source image
////////////////////////////////////////////////////////////
void sfSprite_SetSubRect(sfSprite* sprite, sfIntRect rectangle)
{
    CSFML_CALL(sprite, SetSubRect(sf::IntRect(rectangle.Left, rectangle.Top, rectangle.Width, rectangle.Height)))
    sprite->SubRect = rectangle;
}


////////////////////////////////////////////////////////////
/// Resize a sprite (by changing its scale factors)
////////////////////////////////////////////////////////////
void sfSprite_Resize(sfSprite* sprite, float width, float height)
{
    CSFML_CALL(sprite, Resize(sf::Vector2f(width, height)))
}


////////////////////////////////////////////////////////////
/// Flip a sprite horizontally
////////////////////////////////////////////////////////////
void sfSprite_FlipX(sfSprite* sprite, sfBool flipped)
{
    CSFML_CALL(sprite, FlipX(flipped == sfTrue))
}


////////////////////////////////////////////////////////////
/// Flip a sprite vertically
////////////////////////////////////////////////////////////
void sfSprite_FlipY(sfSprite* sprite, sfBool flipped)
{
    CSFML_CALL(sprite, FlipY(flipped == sfTrue))
}


////////////////////////////////////////////////////////////
/// Get the source image of a sprite
////////////////////////////////////////////////////////////
const sfImage* sfSprite_GetImage(const sfSprite* sprite)
{
    CSFML_CHECK_RETURN(sprite, NULL)

    return sprite->Image;
}


////////////////////////////////////////////////////////////
/// Get the sub-rectangle of a sprite inside the source image
////////////////////////////////////////////////////////////
sfIntRect sfSprite_GetSubRect(const sfSprite* sprite)
{
    sfIntRect rect = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(sprite, rect)

    return sprite->SubRect;
}


////////////////////////////////////////////////////////////
/// Get a sprite width
////////////////////////////////////////////////////////////
float sfSprite_GetWidth(const sfSprite* sprite)
{
    CSFML_CALL_RETURN(sprite, GetSize().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get a sprite height
////////////////////////////////////////////////////////////
float sfSprite_GetHeight(const sfSprite* sprite)
{
    CSFML_CALL_RETURN(sprite, GetSize().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the color of a given pixel in a sprite
////////////////////////////////////////////////////////////
sfColor sfSprite_GetPixel(const sfSprite* sprite, unsigned int x, unsigned int y)
{
    sfColor color = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(sprite, color)

    sf::Color SFMLColor = sprite->This.GetPixel(x, y);
    return sfColor_FromRGBA(SFMLColor.r, SFMLColor.g, SFMLColor.b, SFMLColor.a);
}

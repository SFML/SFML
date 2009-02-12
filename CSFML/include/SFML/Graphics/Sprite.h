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

#ifndef SFML_SPRITE_H
#define SFML_SPRITE_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Graphics/BlendMode.h>
#include <SFML/Graphics/Color.h>
#include <SFML/Graphics/Rect.h>
#include <SFML/Graphics/Types.h>


////////////////////////////////////////////////////////////
/// Create a new sprite
///
/// \return A new sfSprite object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfSprite* sfSprite_Create();

////////////////////////////////////////////////////////////
/// Destroy an existing sprite
///
/// \param Sprite : Sprite to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_Destroy(sfSprite* Sprite);

////////////////////////////////////////////////////////////
/// Set the X position of a sprite
///
/// \param Sprite : Sprite to modify
/// \param X :      New X coordinate
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetX(sfSprite* Sprite, float X);

////////////////////////////////////////////////////////////
/// Set the T position of a sprite
///
/// \param Sprite : Sprite to modify
/// \param Y :      New Y coordinate
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetY(sfSprite* Sprite, float Y);

////////////////////////////////////////////////////////////
/// Set the position of a sprite
///
/// \param Sprite : Sprite to modify
/// \param X :      New X coordinate
/// \param Y :      New Y coordinate
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetPosition(sfSprite* Sprite, float X, float Y);

////////////////////////////////////////////////////////////
/// Set the horizontal scale of a sprite
///
/// \param Sprite : Sprite to modify
/// \param Scale :  New scale (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetScaleX(sfSprite* Sprite, float Scale);

////////////////////////////////////////////////////////////
/// Set the vertical scale of a sprite
///
/// \param Sprite : Sprite to modify
/// \param Scale :  New scale (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetScaleY(sfSprite* Sprite, float Scale);

////////////////////////////////////////////////////////////
/// Set the scale of a sprite
///
/// \param Sprite : Sprite to modify
/// \param ScaleX : New horizontal scale (must be strictly positive)
/// \param ScaleY : New vertical scale (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetScale(sfSprite* Sprite, float ScaleX, float ScaleY);

////////////////////////////////////////////////////////////
/// Set the orientation of a sprite
///
/// \param Sprite :   Sprite to modify
/// \param Rotation : Angle of rotation, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetRotation(sfSprite* Sprite, float Rotation);

////////////////////////////////////////////////////////////
/// Set the center of a sprite, in coordinates relative to
/// its left-top corner
///
/// \param Sprite : Sprite to modify
/// \param X :      X coordinate of the center
/// \param Y :      Y coordinate of the center
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetCenter(sfSprite* Sprite, float X, float Y);

////////////////////////////////////////////////////////////
/// Set the color of a sprite
///
/// \param Sprite : Sprite to modify
/// \param Color :  New color
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetColor(sfSprite* Sprite, sfColor Color);

////////////////////////////////////////////////////////////
/// Set the blending mode for a sprite
///
/// \param Sprite : Sprite to modify
/// \param Mode :   New blending mode
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetBlendMode(sfSprite* Sprite, sfBlendMode Mode);

////////////////////////////////////////////////////////////
/// Get the X position of a sprite
///
/// \param Sprite : Sprite to read
///
/// \return Current X position
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetX(sfSprite* Sprite);

////////////////////////////////////////////////////////////
/// Get the Y position of a sprite
///
/// \param Sprite : Sprite to read
///
/// \return Current Y position
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetY(sfSprite* Sprite);

////////////////////////////////////////////////////////////
/// Get the horizontal scale of a sprite
///
/// \param Sprite : Sprite to read
///
/// \return Current X scale factor (always positive)
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetScaleX(sfSprite* Sprite);

////////////////////////////////////////////////////////////
/// Get the vertical scale of a sprite
///
/// \param Sprite : Sprite to read
///
/// \return Current Y scale factor (always positive)
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetScaleY(sfSprite* Sprite);

////////////////////////////////////////////////////////////
/// Get the orientation of a sprite
///
/// \param Sprite : Sprite to read
///
/// \return Current rotation, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetRotation(sfSprite* Sprite);

////////////////////////////////////////////////////////////
/// Get the X position of the center a sprite
///
/// \param Sprite : Sprite to read
///
/// \return Current X center
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetCenterX(sfSprite* Sprite);

////////////////////////////////////////////////////////////
/// Get the Y position of the center a sprite
///
/// \param Sprite : Sprite to read
///
/// \return Current Y center
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetCenterY(sfSprite* Sprite);

////////////////////////////////////////////////////////////
/// Get the color of a sprite
///
/// \param Sprite : Sprite to read
///
/// \return Current color
///
////////////////////////////////////////////////////////////
CSFML_API sfColor sfSprite_GetColor(sfSprite* Sprite);

////////////////////////////////////////////////////////////
/// Get the current blending mode of a sprite
///
/// \param Sprite : Sprite to read
///
/// \return Current blending mode
///
////////////////////////////////////////////////////////////
CSFML_API sfBlendMode sfSprite_GetBlendMode(sfSprite* Sprite);

////////////////////////////////////////////////////////////
/// Move a sprite
///
/// \param Sprite :  Sprite to modify
/// \param OffsetX : Offset on the X axis
/// \param OffsetY : Offset on the Y axis
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_Move(sfSprite* Sprite, float OffsetX, float OffsetY);

////////////////////////////////////////////////////////////
/// Scale a sprite
///
/// \param Sprite :  Sprite to modify
/// \param FactorX : Horizontal scaling factor (must be strictly positive)
/// \param FactorY : Vertical scaling factor (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_Scale(sfSprite* Sprite, float FactorX, float FactorY);

////////////////////////////////////////////////////////////
/// Rotate a sprite
///
/// \param Sprite : Sprite to modify
/// \param Angle :  Angle of rotation, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_Rotate(sfSprite* Sprite, float Angle);

////////////////////////////////////////////////////////////
/// Transform a point from global coordinates into the sprite's local coordinates
/// (ie it applies the inverse of object's center, translation, rotation and scale to the point)
///
/// \param Sprite : Sprite object
/// \param PointX : X coordinate of the point to transform
/// \param PointY : Y coordinate of the point to transform
/// \param X :      Value to fill with the X coordinate of the converted point
/// \param Y :      Value to fill with the y coordinate of the converted point
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_TransformToLocal(sfSprite* Sprite, float PointX, float PointY, float* X, float* Y);

////////////////////////////////////////////////////////////
/// Transform a point from the sprite's local coordinates into global coordinates
/// (ie it applies the object's center, translation, rotation and scale to the point)
///
/// \param Sprite : Sprite object
/// \param PointX : X coordinate of the point to transform
/// \param PointY : Y coordinate of the point to transform
/// \param X :      Value to fill with the X coordinate of the converted point
/// \param Y :      Value to fill with the y coordinate of the converted point
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_TransformToGlobal(sfSprite* Sprite, float PointX, float PointY, float* X, float* Y);

////////////////////////////////////////////////////////////
/// Change the image of a sprite
///
/// \param Sprite : Sprite to modify
/// \param Image :  New image
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetImage(sfSprite* Sprite, sfImage* Image);

////////////////////////////////////////////////////////////
/// Set the sub-rectangle of a sprite inside the source image
///
/// \param Sprite :  Sprite to modify
/// \param SubRect : New sub-rectangle
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetSubRect(sfSprite* Sprite, sfIntRect SubRect);

////////////////////////////////////////////////////////////
/// Resize a sprite (by changing its scale factors)
///
/// \param Sprite : Sprite to modify
/// \param Width :  New width (must be strictly positive)
/// \param Height : New height (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_Resize(sfSprite* Sprite, float Width, float Height);

////////////////////////////////////////////////////////////
/// Flip a sprite horizontally
///
/// \param Sprite :  Sprite to modify
/// \param Flipped : sfTrue to flip the sprite
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_FlipX(sfSprite* Sprite, sfBool Flipped);

////////////////////////////////////////////////////////////
/// Flip a sprite vertically
///
/// \param Sprite :  Sprite to modify
/// \param Flipped : sfTrue to flip the sprite
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_FlipY(sfSprite* Sprite, sfBool Flipped);

////////////////////////////////////////////////////////////
/// Get the source image of a sprite
///
/// \param Sprite : Sprite to read
///
/// \return Pointer to the image (can be NULL)
///
////////////////////////////////////////////////////////////
CSFML_API sfImage* sfSprite_GetImage(sfSprite* Sprite);

////////////////////////////////////////////////////////////
/// Get the sub-rectangle of a sprite inside the source image
///
/// \param Sprite : Sprite to read
///
/// \return Sub-rectangle
///
////////////////////////////////////////////////////////////
CSFML_API sfIntRect sfSprite_GetSubRect(sfSprite* Sprite);

////////////////////////////////////////////////////////////
/// Get a sprite width
///
/// \param Sprite : Sprite to read
///
/// \return Width of the sprite
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetWidth(sfSprite* Sprite);

////////////////////////////////////////////////////////////
/// Get a sprite height
///
/// \param Sprite : Sprite to read
///
/// \return Height of the sprite
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetHeight(sfSprite* Sprite);

////////////////////////////////////////////////////////////
/// Get the color of a given pixel in a sprite
///
/// \param Sprite : Sprite to read
/// \param X :      X coordinate of the pixel to get
/// \param Y :      Y coordinate of the pixel to get
///
/// \return Color of pixel (X, Y)
///
////////////////////////////////////////////////////////////
CSFML_API sfColor sfSprite_GetPixel(sfSprite* Sprite, unsigned int X, unsigned int Y);


#endif // SFML_SPRITE_H

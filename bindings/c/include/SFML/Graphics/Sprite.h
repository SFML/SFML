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
CSFML_API sfSprite* sfSprite_Create(void);

////////////////////////////////////////////////////////////
/// Copy an existing sprite
///
/// \param sprite : Sprite to copy
///
/// \return Copied object
///
////////////////////////////////////////////////////////////
CSFML_API sfSprite* sfSprite_Copy(sfSprite* sprite);

////////////////////////////////////////////////////////////
/// Destroy an existing sprite
///
/// \param sprite : Sprite to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_Destroy(sfSprite* sprite);

////////////////////////////////////////////////////////////
/// Set the X position of a sprite
///
/// \param sprite : Sprite to modify
/// \param x :      New X coordinate
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetX(sfSprite* sprite, float x);

////////////////////////////////////////////////////////////
/// Set the T position of a sprite
///
/// \param sprite : Sprite to modify
/// \param y :      New Y coordinate
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetY(sfSprite* sprite, float y);

////////////////////////////////////////////////////////////
/// Set the position of a sprite
///
/// \param sprite : Sprite to modify
/// \param x :      New X coordinate
/// \param y :      New Y coordinate
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetPosition(sfSprite* sprite, float x, float y);

////////////////////////////////////////////////////////////
/// Set the horizontal scale of a sprite
///
/// \param sprite : Sprite to modify
/// \param scale :  New scale (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetScaleX(sfSprite* sprite, float scale);

////////////////////////////////////////////////////////////
/// Set the vertical scale of a sprite
///
/// \param sprite : Sprite to modify
/// \param scale :  New scale (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetScaleY(sfSprite* sprite, float scale);

////////////////////////////////////////////////////////////
/// Set the scale of a sprite
///
/// \param sprite : Sprite to modify
/// \param scaleX : New horizontal scale (must be strictly positive)
/// \param scaleY : New vertical scale (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetScale(sfSprite* sprite, float scaleX, float scaleY);

////////////////////////////////////////////////////////////
/// Set the orientation of a sprite
///
/// \param sprite :   Sprite to modify
/// \param rotation : Angle of rotation, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetRotation(sfSprite* sprite, float rotation);

////////////////////////////////////////////////////////////
/// Set the local origin of a sprite, in coordinates relative to
/// its left-top corner
///
/// \param sprite : Sprite to modify
/// \param x :      X coordinate of the origin
/// \param y :      Y coordinate of the origin
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetOrigin(sfSprite* sprite, float x, float y);

////////////////////////////////////////////////////////////
/// Set the color of a sprite
///
/// \param sprite : Sprite to modify
/// \param color :  New color
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetColor(sfSprite* sprite, sfColor color);

////////////////////////////////////////////////////////////
/// Set the blending mode for a sprite
///
/// \param sprite : Sprite to modify
/// \param mode :   New blending mode
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetBlendMode(sfSprite* sprite, sfBlendMode mode);

////////////////////////////////////////////////////////////
/// Get the X position of a sprite
///
/// \param sprite : Sprite to read
///
/// \return Current X position
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetX(const sfSprite* sprite);

////////////////////////////////////////////////////////////
/// Get the Y position of a sprite
///
/// \param sprite : Sprite to read
///
/// \return Current Y position
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetY(const sfSprite* sprite);

////////////////////////////////////////////////////////////
/// Get the horizontal scale of a sprite
///
/// \param sprite : Sprite to read
///
/// \return Current X scale factor (always positive)
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetScaleX(const sfSprite* sprite);

////////////////////////////////////////////////////////////
/// Get the vertical scale of a sprite
///
/// \param sprite : Sprite to read
///
/// \return Current Y scale factor (always positive)
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetScaleY(const sfSprite* sprite);

////////////////////////////////////////////////////////////
/// Get the orientation of a sprite
///
/// \param sprite : Sprite to read
///
/// \return Current rotation, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetRotation(const sfSprite* sprite);

////////////////////////////////////////////////////////////
/// Get the X position of the origin a sprite
///
/// \param sprite : Sprite to read
///
/// \return Current X origin
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetOriginX(const sfSprite* sprite);

////////////////////////////////////////////////////////////
/// Get the Y position of the origin a sprite
///
/// \param sprite : Sprite to read
///
/// \return Current Y origin
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetOriginY(const sfSprite* sprite);

////////////////////////////////////////////////////////////
/// Get the color of a sprite
///
/// \param sprite : Sprite to read
///
/// \return Current color
///
////////////////////////////////////////////////////////////
CSFML_API sfColor sfSprite_GetColor(const sfSprite* sprite);

////////////////////////////////////////////////////////////
/// Get the current blending mode of a sprite
///
/// \param sprite : Sprite to read
///
/// \return Current blending mode
///
////////////////////////////////////////////////////////////
CSFML_API sfBlendMode sfSprite_GetBlendMode(const sfSprite* sprite);

////////////////////////////////////////////////////////////
/// Move a sprite
///
/// \param sprite :  Sprite to modify
/// \param offsetX : Offset on the X axis
/// \param offsetY : Offset on the Y axis
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_Move(sfSprite* sprite, float offsetX, float offsetY);

////////////////////////////////////////////////////////////
/// Scale a sprite
///
/// \param sprite :  Sprite to modify
/// \param factorX : Horizontal scaling factor (must be strictly positive)
/// \param factorY : Vertical scaling factor (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_Scale(sfSprite* sprite, float factorX, float factorY);

////////////////////////////////////////////////////////////
/// Rotate a sprite
///
/// \param sprite : Sprite to modify
/// \param angle :  Angle of rotation, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_Rotate(sfSprite* sprite, float angle);

////////////////////////////////////////////////////////////
/// Transform a point from global coordinates into the sprite's local coordinates
/// (ie it applies the inverse of object's origin, translation, rotation and scale to the point)
///
/// \param sprite : Sprite object
/// \param pointX : X coordinate of the point to transform
/// \param pointY : Y coordinate of the point to transform
/// \param x :      Value to fill with the X coordinate of the converted point
/// \param y :      Value to fill with the y coordinate of the converted point
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_TransformToLocal(const sfSprite* sprite, float pointX, float pointY, float* x, float* y);

////////////////////////////////////////////////////////////
/// Transform a point from the sprite's local coordinates into global coordinates
/// (ie it applies the object's origin, translation, rotation and scale to the point)
///
/// \param sprite : Sprite object
/// \param pointX : X coordinate of the point to transform
/// \param pointY : Y coordinate of the point to transform
/// \param x :      Value to fill with the X coordinate of the converted point
/// \param y :      Value to fill with the y coordinate of the converted point
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_TransformToGlobal(const sfSprite* sprite, float pointX, float pointY, float* x, float* y);

////////////////////////////////////////////////////////////
/// Change the image of a sprite
///
/// \param sprite :          Sprite to modify
/// \param image :           New image
/// \param adjustToNewSize : If true, the SubRect of the sprite will be adjusted to the size of the new image
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetImage(sfSprite* sprite, const sfImage* image, sfBool adjustToNewSize);

////////////////////////////////////////////////////////////
/// Set the sub-rectangle of a sprite inside the source image
///
/// \param sprite :    Sprite to modify
/// \param rectangle : New sub-rectangle
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_SetSubRect(sfSprite* sprite, sfIntRect rectangle);

////////////////////////////////////////////////////////////
/// Resize a sprite (by changing its scale factors)
///
/// \param sprite : Sprite to modify
/// \param width :  New width (must be strictly positive)
/// \param height : New height (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_Resize(sfSprite* sprite, float width, float height);

////////////////////////////////////////////////////////////
/// Flip a sprite horizontally
///
/// \param sprite :  Sprite to modify
/// \param flipped : sfTrue to flip the sprite
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_FlipX(sfSprite* sprite, sfBool flipped);

////////////////////////////////////////////////////////////
/// Flip a sprite vertically
///
/// \param sprite :  Sprite to modify
/// \param flipped : sfTrue to flip the sprite
///
////////////////////////////////////////////////////////////
CSFML_API void sfSprite_FlipY(sfSprite* sprite, sfBool flipped);

////////////////////////////////////////////////////////////
/// Get the source image of a sprite
///
/// \param sprite : Sprite to read
///
/// \return Pointer to the image (can be NULL)
///
////////////////////////////////////////////////////////////
CSFML_API const sfImage* sfSprite_GetImage(const sfSprite* sprite);

////////////////////////////////////////////////////////////
/// Get the sub-rectangle of a sprite inside the source image
///
/// \param sprite : Sprite to read
///
/// \return Sub-rectangle
///
////////////////////////////////////////////////////////////
CSFML_API sfIntRect sfSprite_GetSubRect(const sfSprite* sprite);

////////////////////////////////////////////////////////////
/// Get a sprite width
///
/// \param sprite : Sprite to read
///
/// \return Width of the sprite
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetWidth(const sfSprite* sprite);

////////////////////////////////////////////////////////////
/// Get a sprite height
///
/// \param sprite : Sprite to read
///
/// \return Height of the sprite
///
////////////////////////////////////////////////////////////
CSFML_API float sfSprite_GetHeight(const sfSprite* sprite);

////////////////////////////////////////////////////////////
/// Get the color of a given pixel in a sprite
///
/// \param sprite : Sprite to read
/// \param x :      X coordinate of the pixel to get
/// \param y :      Y coordinate of the pixel to get
///
/// \return Color of pixel (X, Y)
///
////////////////////////////////////////////////////////////
CSFML_API sfColor sfSprite_GetPixel(const sfSprite* sprite, unsigned int x, unsigned int y);


#endif // SFML_SPRITE_H

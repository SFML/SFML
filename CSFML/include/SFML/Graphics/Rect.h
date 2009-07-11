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

#ifndef SFML_RECT_H
#define SFML_RECT_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>


////////////////////////////////////////////////////////////
/// sfFloatRect and sfIntRect are utility classes for
/// manipulating rectangles.
////////////////////////////////////////////////////////////
typedef struct
{
    float Left;
    float Top;
    float Right;
    float Bottom;
} sfFloatRect;

typedef struct
{
    int Left;
    int Top;
    int Right;
    int Bottom;
} sfIntRect;


////////////////////////////////////////////////////////////
/// Move a rectangle by the given offset
///
/// \param rect :    Rectangle to move
/// \param offsetX : Horizontal offset
/// \param offsetY : Vertical offset
///
////////////////////////////////////////////////////////////
CSFML_API void sfFloatRect_Offset(sfFloatRect* rect, float offsetX, float offsetY);
CSFML_API void sfIntRect_Offset(sfIntRect* rect, int offsetX, int offsetY);

////////////////////////////////////////////////////////////
/// Check if a point is inside a rectangle's area
///
/// \param rect : Rectangle to test
/// \param x :    X coordinate of the point to test
/// \param y :    Y coordinate of the point to test
///
/// \return sfTrue if the point is inside
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfFloatRect_Contains(sfFloatRect* rect, float x, float y);
CSFML_API sfBool sfIntRect_Contains(sfIntRect* rect, int x, int y);

////////////////////////////////////////////////////////////
/// Check intersection between two rectangles
///
/// \param rect1 :        First rectangle to test
/// \param rect2 :        Second rectangle to test
/// \param intersection : Rectangle to be filled with overlapping rect (can be NULL)
///
/// \return sfTrue if rectangles overlap
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfFloatRect_Intersects(sfFloatRect* rect1, sfFloatRect* rect2, sfFloatRect* intersection);
CSFML_API sfBool sfIntRect_Intersects(sfIntRect* rect1, sfIntRect* rect2, sfIntRect* intersection);


#endif // SFML_RECT_H

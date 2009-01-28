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
/// \param Rect :    Rectangle to move
/// \param OffsetX : Horizontal offset
/// \param OffsetY : Vertical offset
///
////////////////////////////////////////////////////////////
CSFML_API void sfFloatRect_Offset(sfFloatRect* Rect, float OffsetX, float OffsetY);
CSFML_API void sfIntRect_Offset(sfIntRect* Rect, int OffsetX, int OffsetY);

////////////////////////////////////////////////////////////
/// Check if a point is inside a rectangle's area
///
/// \param Rect : Rectangle to test
/// \param X :    X coordinate of the point to test
/// \param Y :    Y coordinate of the point to test
///
/// \return sfTrue if the point is inside
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfFloatRect_Contains(sfFloatRect* Rect, float X, float Y);
CSFML_API sfBool sfIntRect_Contains(sfIntRect* Rect, int X, int Y);

////////////////////////////////////////////////////////////
/// Check intersection between two rectangles
///
/// \param Rect1 :           First rectangle to test
/// \param Rect2 :           Second rectangle to test
/// \param OverlappingRect : Rectangle to be filled with overlapping rect (can be NULL)
///
/// \return sfTrue if rectangles overlap
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfFloatRect_Intersects(sfFloatRect* Rect1, sfFloatRect* Rect2, sfFloatRect* OverlappingRect);
CSFML_API sfBool sfIntRect_Intersects(sfIntRect* Rect1, sfIntRect* Rect2, sfIntRect* OverlappingRect);


#endif // SFML_RECT_H

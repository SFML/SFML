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
#include <SFML/Graphics/Rect.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Move a rectangle by the given offset
////////////////////////////////////////////////////////////
void sfFloatRect_Offset(sfFloatRect* Rect, float OffsetX, float OffsetY)
{
    CSFML_CHECK(Rect)
    Rect->Left   += OffsetX;
    Rect->Right  += OffsetX;
    Rect->Top    += OffsetY;
    Rect->Bottom += OffsetY;
}
void sfIntRect_Offset(sfIntRect* Rect, int OffsetX, int OffsetY)
{
    CSFML_CHECK(Rect)
    Rect->Left   += OffsetX;
    Rect->Right  += OffsetX;
    Rect->Top    += OffsetY;
    Rect->Bottom += OffsetY;
}


////////////////////////////////////////////////////////////
/// Check if a point is inside a rectangle's area
////////////////////////////////////////////////////////////
sfBool sfFloatRect_Contains(sfFloatRect* Rect, float X, float Y)
{
    CSFML_CHECK_RETURN(Rect, sfFalse)
    return sf::FloatRect(Rect->Left, Rect->Top, Rect->Right, Rect->Bottom).Contains(X, Y);
}
sfBool sfIntRect_Contains(sfIntRect* Rect, int X, int Y)
{
    CSFML_CHECK_RETURN(Rect, sfFalse)
    return sf::IntRect(Rect->Left, Rect->Top, Rect->Right, Rect->Bottom).Contains(X, Y);
}


////////////////////////////////////////////////////////////
/// Check intersection between two rectangles
////////////////////////////////////////////////////////////
sfBool sfFloatRect_Intersects(sfFloatRect* Rect1, sfFloatRect* Rect2, sfFloatRect* OverlappingRect)
{
    CSFML_CHECK_RETURN(Rect1, sfFalse)
    CSFML_CHECK_RETURN(Rect2, sfFalse)

    sf::FloatRect SFMLRect1(Rect1->Left, Rect1->Top, Rect1->Right, Rect1->Bottom);
    sf::FloatRect SFMLRect2(Rect2->Left, Rect2->Top, Rect2->Right, Rect2->Bottom);

    if (OverlappingRect)
    {
        sf::FloatRect Overlap;
        bool Intersect = SFMLRect1.Intersects(SFMLRect2, &Overlap);

        OverlappingRect->Left   = Overlap.Left;
        OverlappingRect->Top    = Overlap.Top;
        OverlappingRect->Right  = Overlap.Right;
        OverlappingRect->Bottom = Overlap.Bottom;

        return Intersect;
    }
    else
    {
        return SFMLRect1.Intersects(SFMLRect2);
    }
}
sfBool sfIntRect_Intersects(sfIntRect* Rect1, sfIntRect* Rect2, sfIntRect* OverlappingRect)
{
    CSFML_CHECK_RETURN(Rect1, sfFalse)
    CSFML_CHECK_RETURN(Rect2, sfFalse)

    sf::IntRect SFMLRect1(Rect1->Left, Rect1->Top, Rect1->Right, Rect1->Bottom);
    sf::IntRect SFMLRect2(Rect2->Left, Rect2->Top, Rect2->Right, Rect2->Bottom);

    if (OverlappingRect)
    {
        sf::IntRect Overlap;
        bool Intersect = SFMLRect1.Intersects(SFMLRect2, &Overlap);

        OverlappingRect->Left   = Overlap.Left;
        OverlappingRect->Top    = Overlap.Top;
        OverlappingRect->Right  = Overlap.Right;
        OverlappingRect->Bottom = Overlap.Bottom;

        return Intersect;
    }
    else
    {
        return SFMLRect1.Intersects(SFMLRect2);
    }
}

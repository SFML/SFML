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
/// Check if a point is inside a rectangle's area
////////////////////////////////////////////////////////////
sfBool sfFloatRect_Contains(const sfFloatRect* rect, float x, float y)
{
    CSFML_CHECK_RETURN(rect, sfFalse)
    return sf::FloatRect(rect->Left, rect->Top, rect->Width, rect->Height).Contains(x, y);
}
sfBool sfIntRect_Contains(const sfIntRect* rect, int x, int y)
{
    CSFML_CHECK_RETURN(rect, sfFalse)
    return sf::IntRect(rect->Left, rect->Top, rect->Width, rect->Height).Contains(x, y);
}


////////////////////////////////////////////////////////////
/// Check intersection between two rectangles
////////////////////////////////////////////////////////////
sfBool sfFloatRect_Intersects(const sfFloatRect* rect1, const sfFloatRect* rect2, sfFloatRect* intersection)
{
    CSFML_CHECK_RETURN(rect1, sfFalse)
    CSFML_CHECK_RETURN(rect2, sfFalse)

    sf::FloatRect SFMLRect1(rect1->Left, rect1->Top, rect1->Width, rect1->Height);
    sf::FloatRect SFMLRect2(rect2->Left, rect2->Top, rect2->Width, rect2->Height);

    if (intersection)
    {
        sf::FloatRect overlap;
        bool intersects = SFMLRect1.Intersects(SFMLRect2, overlap);

        intersection->Left   = overlap.Left;
        intersection->Top    = overlap.Top;
        intersection->Width  = overlap.Width;
        intersection->Height = overlap.Height;

        return intersects;
    }
    else
    {
        return SFMLRect1.Intersects(SFMLRect2);
    }
}
sfBool sfIntRect_Intersects(const sfIntRect* rect1, const sfIntRect* rect2, sfIntRect* intersection)
{
    CSFML_CHECK_RETURN(rect1, sfFalse)
    CSFML_CHECK_RETURN(rect2, sfFalse)

    sf::IntRect SFMLRect1(rect1->Left, rect1->Top, rect1->Width, rect1->Height);
    sf::IntRect SFMLRect2(rect2->Left, rect2->Top, rect2->Width, rect2->Height);

    if (intersection)
    {
        sf::IntRect overlap;
        bool intersects = SFMLRect1.Intersects(SFMLRect2, overlap);

        intersection->Left   = overlap.Left;
        intersection->Top    = overlap.Top;
        intersection->Width  = overlap.Width;
        intersection->Height = overlap.Height;

        return intersects;
    }
    else
    {
        return SFMLRect1.Intersects(SFMLRect2);
    }
}

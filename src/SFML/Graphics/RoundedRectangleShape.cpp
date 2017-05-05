////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2012 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Graphics/RoundedRectangleShape.hpp>
#include <cmath>

namespace sf
{
////////////////////////////////////////////////////////////
RoundedRectangleShape::RoundedRectangleShape(const Vector2f& size, float radius, unsigned int cornerPointCount)
{
    mySize = size;
    myRadius = radius;
    myCornerPointCount = cornerPointCount;
    Update();
}

////////////////////////////////////////////////////////////
void RoundedRectangleShape::setSize(const Vector2f& size)
{
    mySize = size;
    Update();
}

////////////////////////////////////////////////////////////
const Vector2f& RoundedRectangleShape::getSize() const
{
    return mySize;
}

////////////////////////////////////////////////////////////
void RoundedRectangleShape::setCornersRadius(float radius)
{
    myRadius = radius;
    Update();
}

////////////////////////////////////////////////////////////
const float RoundedRectangleShape::getCornersRadius() const
{
    return myRadius;
}

////////////////////////////////////////////////////////////
void RoundedRectangleShape::setCornerPointCount(unsigned int count)
{
    myCornerPointCount = count;
    Update();
}

////////////////////////////////////////////////////////////
unsigned int RoundedRectangleShape::getPointCount() const
{
    return myCornerPointCount*4;
}

////////////////////////////////////////////////////////////
sf::Vector2f RoundedRectangleShape::getPoint(unsigned int index) const
{
    if(index >= myCornerPointCount*4)
        return sf::Vector2f(0,0);

    float deltaAngle = 90.0f/(myCornerPointCount-1);
    sf::Vector2f center;
    unsigned int centerIndex = index/myCornerPointCount;
    unsigned int offset = 0;
    static const float pi = 3.141592654f;

    switch(centerIndex)
    {
        case 0: center.x = mySize.x - myRadius;     center.y = myRadius;            break;
        case 1: center.x = myRadius;                center.y = myRadius;            break;
        case 2: center.x = myRadius;                center.y = mySize.y - myRadius; break;
        case 3: center.x = mySize.x - myRadius;     center.y = mySize.y - myRadius; break;
    }

    return sf::Vector2f(myRadius*cos(deltaAngle*(index-centerIndex)*pi/180)+center.x,
                        myRadius*sin(deltaAngle*(index-centerIndex)*pi/180)-center.y);
}
}//namespace sf

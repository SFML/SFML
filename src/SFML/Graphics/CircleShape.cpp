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
#include <SFML/Graphics/CircleShape.hpp>
#include <cmath>


namespace sf
{
////////////////////////////////////////////////////////////
CircleShape::CircleShape(float radius, unsigned int pointCount) :
myRadius    (radius),
myPointCount(pointCount)
{
    Update();
}


////////////////////////////////////////////////////////////
void CircleShape::SetRadius(float radius)
{
    myRadius = radius;
    Update();
}


////////////////////////////////////////////////////////////
float CircleShape::GetRadius() const
{
    return myRadius;
}


////////////////////////////////////////////////////////////
void CircleShape::SetPointCount(unsigned int count)
{
    myPointCount = count;
    Update();
}

////////////////////////////////////////////////////////////
unsigned int CircleShape::GetPointCount() const
{
    return myPointCount;
}


////////////////////////////////////////////////////////////
Vector2f CircleShape::GetPoint(unsigned int index) const
{
    static const float pi = 3.141592654f;

    float angle = index * 2 * pi / myPointCount - pi / 2;
    float x = std::cos(angle) * myRadius;
    float y = std::sin(angle) * myRadius;

    return Vector2f(myRadius + x, myRadius + y);
}

} // namespace sf

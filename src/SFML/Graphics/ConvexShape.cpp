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
#include <SFML/Graphics/ConvexShape.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
ConvexShape::ConvexShape(unsigned int pointCount)
{
    setPointCount(pointCount);
}


////////////////////////////////////////////////////////////
void ConvexShape::setPointCount(unsigned int count)
{
    m_points.resize(count);
    update();
}


////////////////////////////////////////////////////////////
unsigned int ConvexShape::getPointCount() const
{
    return static_cast<unsigned int>(m_points.size());
}


////////////////////////////////////////////////////////////
void ConvexShape::setPoint(unsigned int index, const Vector2f& point)
{
    m_points[index] = point;
    update();
}


////////////////////////////////////////////////////////////
Vector2f ConvexShape::getPoint(unsigned int index) const
{
    return m_points[index];
}

} // namespace sf

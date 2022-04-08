////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/LineShape.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
LineShape::LineShape(const Vector2f& beginning, const Vector2f& end, float thickness) :
m_direction(end - beginning),
m_thickness(thickness)
{
    setPosition(beginning);
    update();
}


////////////////////////////////////////////////////////////
void LineShape::setThickness(float thickness)
{
    m_thickness = thickness;
    update();
}


////////////////////////////////////////////////////////////
void LineShape::setEndPoint(const Vector2f& point)
{
    m_direction = point - getPosition();
    update();
}


////////////////////////////////////////////////////////////
float LineShape::getThickness() const
{
    return m_thickness;
}


////////////////////////////////////////////////////////////
Vector2f LineShape::getEndPoint() const
{
    return getPosition() + m_direction;
}


////////////////////////////////////////////////////////////
float LineShape::getLength() const
{
    return m_direction.length();
}


////////////////////////////////////////////////////////////
std::size_t LineShape::getPointCount() const
{
    return 4;
}


////////////////////////////////////////////////////////////
Vector2f LineShape::getPoint(const std::size_t index) const
{
    auto offset = Vector2f();
    if (m_direction != Vector2f(0, 0))
        offset = (m_thickness / 2.f) * m_direction.normalized().perpendicular();

    switch (index)
    {
        default:
        case 0:
            return offset;
        case 1:
            return m_direction + offset;
        case 2:
            return m_direction - offset;
        case 3:
            return -offset;
    }
}

} // namespace sf

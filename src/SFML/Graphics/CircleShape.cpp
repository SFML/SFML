////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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

#include <SFML/System/Angle.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
CircleShape::CircleShape(float radius, std::size_t pointCount) : m_radius(radius), m_pointCount(pointCount)
{
    update();
}


////////////////////////////////////////////////////////////
void CircleShape::setRadius(float radius)
{
    m_radius = radius;
    update();
}


////////////////////////////////////////////////////////////
float CircleShape::getRadius() const
{
    return m_radius;
}


////////////////////////////////////////////////////////////
void CircleShape::setPointCount(std::size_t count)
{
    m_pointCount = count;
    update();
}

////////////////////////////////////////////////////////////
std::size_t CircleShape::getPointCount() const
{
    return m_pointCount;
}


////////////////////////////////////////////////////////////
Vector2f CircleShape::getPoint(std::size_t index) const
{
    const Angle angle = static_cast<float>(index) / static_cast<float>(m_pointCount) * degrees(360.f) - degrees(90.f);
    return Vector2f(m_radius, m_radius) + Vector2f(m_radius, angle);
}


////////////////////////////////////////////////////////////
Vector2f CircleShape::getGeometricCenter() const
{
    return {m_radius, m_radius};
}

} // namespace sf

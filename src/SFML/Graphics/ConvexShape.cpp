////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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
ConvexShape::ConvexShape(std::size_t pointCount)
{
    setPointCount(pointCount);
}


////////////////////////////////////////////////////////////
void ConvexShape::setPointCount(std::size_t count)
{
    m_points.resize(count);
    update();
}


////////////////////////////////////////////////////////////
std::size_t ConvexShape::getPointCount() const
{
    return m_points.size();
}


////////////////////////////////////////////////////////////
void ConvexShape::setPoint(std::size_t index, const Vector2f& point)
{
    m_points[index] = point;
    update();
}


////////////////////////////////////////////////////////////
Vector2f ConvexShape::getPoint(std::size_t index) const
{
    return m_points[index];
}

////////////////////////////////////////////////////////////
Vector2f ConvexShape::getGeometricCenter() const
{
    if (m_points.size() > 2)
    {
        const auto& off  = m_points.front();
        const auto  off2 = 2.f * off;
        Vector2f    xy{};
        float       twicearea = 0.f;

        float f;

        for (std::size_t i = 0, j = m_points.size() - 1; i < m_points.size(); j = i++)
        {
            const auto& p1 = m_points[i];
            const auto& p2 = m_points[j];

            f = (p1.x - off.x) * (p2.y - off.y) - (p2.x - off.x) * (p1.y - off.y);
            twicearea += f;
            xy += ((p1 + p2) - off2) * f;
        }

        f = twicearea * 3.f;

        return xy / f + off;
    }

    if (m_points.size() == 2) // a line
    {
        return (m_points[0] + m_points[1]) / 2.f;
    }

    if (!m_points.empty()) // a single point
    {
        return m_points.front();
    }

    return Vector2f{}; // empty or exception or UB or optional?
}

} // namespace sf

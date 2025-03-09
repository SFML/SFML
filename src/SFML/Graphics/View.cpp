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
#include <SFML/Graphics/View.hpp>

#include <cassert>
#include <cmath>


namespace sf
{
////////////////////////////////////////////////////////////
View::View(const FloatRect& rectangle) : m_center(rectangle.getCenter()), m_size(rectangle.size)
{
}


////////////////////////////////////////////////////////////
View::View(Vector2f center, Vector2f size) : m_center(center), m_size(size)
{
}


////////////////////////////////////////////////////////////
void View::setCenter(Vector2f center)
{
    m_center              = center;
    m_transformUpdated    = false;
    m_invTransformUpdated = false;
}


////////////////////////////////////////////////////////////
void View::setSize(Vector2f size)
{
    m_size = size;

    m_transformUpdated    = false;
    m_invTransformUpdated = false;
}


////////////////////////////////////////////////////////////
void View::setRotation(Angle angle)
{
    m_rotation = angle.wrapUnsigned();

    m_transformUpdated    = false;
    m_invTransformUpdated = false;
}


////////////////////////////////////////////////////////////
void View::setViewport(const FloatRect& viewport)
{
    m_viewport = viewport;
}


////////////////////////////////////////////////////////////
void View::setScissor(const FloatRect& scissor)
{
    assert(scissor.position.x >= 0.0f && scissor.position.x <= 1.0f && "scissor.position.x must lie within [0, 1]");
    assert(scissor.position.y >= 0.0f && scissor.position.y <= 1.0f && "scissor.position.y must lie within [0, 1]");
    assert(scissor.size.x >= 0.0f && "scissor.size.x must lie within [0, 1]");
    assert(scissor.size.y >= 0.0f && "scissor.size.y must lie within [0, 1]");
    assert(scissor.position.x + scissor.size.x <= 1.0f && "scissor.position.x + scissor.size.x must lie within [0, 1]");
    assert(scissor.position.y + scissor.size.y <= 1.0f && "scissor.position.y + scissor.size.y must lie within [0, 1]");

    m_scissor = scissor;
}


////////////////////////////////////////////////////////////
Vector2f View::getCenter() const
{
    return m_center;
}


////////////////////////////////////////////////////////////
Vector2f View::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
Angle View::getRotation() const
{
    return m_rotation;
}


////////////////////////////////////////////////////////////
const FloatRect& View::getViewport() const
{
    return m_viewport;
}


////////////////////////////////////////////////////////////
const FloatRect& View::getScissor() const
{
    return m_scissor;
}


////////////////////////////////////////////////////////////
void View::move(Vector2f offset)
{
    setCenter(m_center + offset);
}


////////////////////////////////////////////////////////////
void View::rotate(Angle angle)
{
    setRotation(m_rotation + angle);
}


////////////////////////////////////////////////////////////
void View::zoom(float factor)
{
    setSize(m_size * factor);
}


////////////////////////////////////////////////////////////
const Transform& View::getTransform() const
{
    // Recompute the matrix if needed
    if (!m_transformUpdated)
    {
        // Rotation components
        const float angle  = m_rotation.asRadians();
        const float cosine = std::cos(angle);
        const float sine   = std::sin(angle);
        const float tx     = -m_center.x * cosine - m_center.y * sine + m_center.x;
        const float ty     = m_center.x * sine - m_center.y * cosine + m_center.y;

        // Projection components
        const float a = 2.f / m_size.x;
        const float b = -2.f / m_size.y;
        const float c = -a * m_center.x;
        const float d = -b * m_center.y;

        // Rebuild the projection matrix
        // clang-format off
        m_transform = Transform( a * cosine, a * sine,   a * tx + c,
                                -b * sine,   b * cosine, b * ty + d,
                                 0.f,        0.f,        1.f);
        // clang-format on
        m_transformUpdated = true;
    }

    return m_transform;
}


////////////////////////////////////////////////////////////
const Transform& View::getInverseTransform() const
{
    // Recompute the matrix if needed
    if (!m_invTransformUpdated)
    {
        m_inverseTransform    = getTransform().getInverse();
        m_invTransformUpdated = true;
    }

    return m_inverseTransform;
}

} // namespace sf

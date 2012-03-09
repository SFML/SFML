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
#include <SFML/Graphics/View.hpp>
#include <cmath>


namespace sf
{
////////////////////////////////////////////////////////////
View::View() :
m_center             (),
m_size               (),
m_rotation           (0),
m_viewport           (0, 0, 1, 1),
m_transformUpdated   (false),
m_invTransformUpdated(false)
{
    Reset(FloatRect(0, 0, 1000, 1000));
}


////////////////////////////////////////////////////////////
View::View(const FloatRect& rectangle) :
m_center             (),
m_size               (),
m_rotation           (0),
m_viewport           (0, 0, 1, 1),
m_transformUpdated   (false),
m_invTransformUpdated(false)
{
    Reset(rectangle);
}


////////////////////////////////////////////////////////////
View::View(const Vector2f& center, const Vector2f& size) :
m_center             (center),
m_size               (size),
m_rotation           (0),
m_viewport           (0, 0, 1, 1),
m_transformUpdated   (false),
m_invTransformUpdated(false)
{

}

////////////////////////////////////////////////////////////
void View::SetCenter(float x, float y)
{
    m_center.x = x;
    m_center.y = y;

    m_transformUpdated    = false;
    m_invTransformUpdated = false;
}


////////////////////////////////////////////////////////////
void View::SetCenter(const Vector2f& center)
{
    SetCenter(center.x, center.y);
}


////////////////////////////////////////////////////////////
void View::SetSize(float width, float height)
{
    m_size.x = width;
    m_size.y = height;

    m_transformUpdated    = false;
    m_invTransformUpdated = false;
}


////////////////////////////////////////////////////////////
void View::SetSize(const Vector2f& size)
{
    SetSize(size.x, size.y);
}


////////////////////////////////////////////////////////////
void View::SetRotation(float angle)
{
    m_rotation = static_cast<float>(fmod(angle, 360));
    if (m_rotation < 0)
        m_rotation += 360.f;

    m_transformUpdated    = false;
    m_invTransformUpdated = false;
}


////////////////////////////////////////////////////////////
void View::SetViewport(const FloatRect& viewport)
{
    m_viewport = viewport;
}


////////////////////////////////////////////////////////////
void View::Reset(const FloatRect& rectangle)
{
    m_center.x = rectangle.Left + rectangle.Width / 2.f;
    m_center.y = rectangle.Top + rectangle.Height / 2.f;
    m_size.x   = rectangle.Width;
    m_size.y   = rectangle.Height;
    m_rotation = 0;

    m_transformUpdated    = false;
    m_invTransformUpdated = false;
}


////////////////////////////////////////////////////////////
const Vector2f& View::GetCenter() const
{
    return m_center;
}


////////////////////////////////////////////////////////////
const Vector2f& View::GetSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
float View::GetRotation() const
{
    return m_rotation;
}


////////////////////////////////////////////////////////////
const FloatRect& View::GetViewport() const
{
    return m_viewport;
}


////////////////////////////////////////////////////////////
void View::Move(float offsetX, float offsetY)
{
    SetCenter(m_center.x + offsetX, m_center.y + offsetY);
}


////////////////////////////////////////////////////////////
void View::Move(const Vector2f& offset)
{
    SetCenter(m_center + offset);
}


////////////////////////////////////////////////////////////
void View::Rotate(float angle)
{
    SetRotation(m_rotation + angle);
}


////////////////////////////////////////////////////////////
void View::Zoom(float factor)
{
    SetSize(m_size.x * factor, m_size.y * factor);
}


////////////////////////////////////////////////////////////
const Transform& View::GetTransform() const
{
    // Recompute the matrix if needed
    if (!m_transformUpdated)
    {
        // Rotation components
        float angle  = m_rotation * 3.141592654f / 180.f;
        float cosine = static_cast<float>(std::cos(angle));
        float sine   = static_cast<float>(std::sin(angle));
        float tx     = -m_center.x * cosine - m_center.y * sine + m_center.x;
        float ty     =  m_center.x * sine - m_center.y * cosine + m_center.y;

        // Projection components
        float a =  2.f / m_size.x;
        float b = -2.f / m_size.y;
        float c = -a * m_center.x;
        float d = -b * m_center.y;

        // Rebuild the projection matrix
        m_transform = Transform( a * cosine, a * sine,   a * tx + c,
                                -b * sine,   b * cosine, b * ty + d,
                                 0.f,        0.f,        1.f);
        m_transformUpdated = true;
    }

    return m_transform;
}


////////////////////////////////////////////////////////////
const Transform& View::GetInverseTransform() const
{
    // Recompute the matrix if needed
    if (!m_invTransformUpdated)
    {
        m_inverseTransform = GetTransform().GetInverse();
        m_invTransformUpdated = true;
    }

    return m_inverseTransform;
}

} // namespace sf

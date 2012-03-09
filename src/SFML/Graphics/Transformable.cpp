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
#include <SFML/Graphics/Transformable.hpp>
#include <cmath>


namespace sf
{
////////////////////////////////////////////////////////////
Transformable::Transformable() :
m_origin                    (0, 0),
m_position                  (0, 0),
m_rotation                  (0),
m_scale                     (1, 1),
m_transform                 (),
m_transformNeedUpdate       (true),
m_inverseTransform          (),
m_inverseTransformNeedUpdate(true)
{
}


////////////////////////////////////////////////////////////
Transformable::~Transformable()
{
}


////////////////////////////////////////////////////////////
void Transformable::SetPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::SetPosition(const Vector2f& position)
{
    SetPosition(position.x, position.y);
}


////////////////////////////////////////////////////////////
void Transformable::SetRotation(float angle)
{
    m_rotation = angle;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::SetScale(float factorX, float factorY)
{
    m_scale.x = factorX;
    m_scale.y = factorY;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::SetScale(const Vector2f& factors)
{
    SetScale(factors.x, factors.y);
}


////////////////////////////////////////////////////////////
void Transformable::SetOrigin(float x, float y)
{
    m_origin.x = x;
    m_origin.y = y;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::SetOrigin(const Vector2f& origin)
{
    SetOrigin(origin.x, origin.y);
}


////////////////////////////////////////////////////////////
const Vector2f& Transformable::GetPosition() const
{
    return m_position;
}


////////////////////////////////////////////////////////////
float Transformable::GetRotation() const
{
    return m_rotation;
}


////////////////////////////////////////////////////////////
const Vector2f& Transformable::GetScale() const
{
    return m_scale;
}


////////////////////////////////////////////////////////////
const Vector2f& Transformable::GetOrigin() const
{
    return m_origin;
}


////////////////////////////////////////////////////////////
void Transformable::Move(float offsetX, float offsetY)
{
    SetPosition(m_position.x + offsetX, m_position.y + offsetY);
}


////////////////////////////////////////////////////////////
void Transformable::Move(const Vector2f& offset)
{
    SetPosition(m_position.x + offset.x, m_position.y + offset.y);
}


////////////////////////////////////////////////////////////
void Transformable::Rotate(float angle)
{
    SetRotation(m_rotation + angle);
}


////////////////////////////////////////////////////////////
void Transformable::Scale(float factorX, float factorY)
{
    SetScale(m_scale.x * factorX, m_scale.y * factorY);
}


////////////////////////////////////////////////////////////
void Transformable::Scale(const Vector2f& factor)
{
    SetScale(m_scale.x * factor.x, m_scale.y * factor.y);
}


////////////////////////////////////////////////////////////
const Transform& Transformable::GetTransform() const
{
    // Recompute the combined transform if needed
    if (m_transformNeedUpdate)
    {
        float angle  = -m_rotation * 3.141592654f / 180.f;
        float cosine = static_cast<float>(std::cos(angle));
        float sine   = static_cast<float>(std::sin(angle));
        float sxc    = m_scale.x * cosine;
        float syc    = m_scale.y * cosine;
        float sxs    = m_scale.x * sine;
        float sys    = m_scale.y * sine;
        float tx     = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
        float ty     =  m_origin.x * sxs - m_origin.y * syc + m_position.y;

        m_transform = Transform( sxc, sys, tx,
                                -sxs, syc, ty,
                                 0.f, 0.f, 1.f);
        m_transformNeedUpdate = false;
    }

    return m_transform;
}


////////////////////////////////////////////////////////////
const Transform& Transformable::GetInverseTransform() const
{
    // Recompute the inverse transform if needed
    if (m_inverseTransformNeedUpdate)
    {
        m_inverseTransform = GetTransform().GetInverse();
        m_inverseTransformNeedUpdate = false;
    }

    return m_inverseTransform;
}

} // namespace sf

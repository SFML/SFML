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
myOrigin                    (0, 0),
myPosition                  (0, 0),
myRotation                  (0),
myScale                     (1, 1),
myTransform                 (),
myTransformNeedUpdate       (true),
myInverseTransform          (),
myInverseTransformNeedUpdate(true)
{
}


////////////////////////////////////////////////////////////
Transformable::~Transformable()
{
}


////////////////////////////////////////////////////////////
void Transformable::SetPosition(float x, float y)
{
    myPosition.x = x;
    myPosition.y = y;
    myTransformNeedUpdate = true;
    myInverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::SetPosition(const Vector2f& position)
{
    SetPosition(position.x, position.y);
}


////////////////////////////////////////////////////////////
void Transformable::SetRotation(float angle)
{
    myRotation = angle;
    myTransformNeedUpdate = true;
    myInverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::SetScale(float factorX, float factorY)
{
    myScale.x = factorX;
    myScale.y = factorY;
    myTransformNeedUpdate = true;
    myInverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::SetScale(const Vector2f& factors)
{
    SetScale(factors.x, factors.y);
}


////////////////////////////////////////////////////////////
void Transformable::SetOrigin(float x, float y)
{
    myOrigin.x = x;
    myOrigin.y = y;
    myTransformNeedUpdate = true;
    myInverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::SetOrigin(const Vector2f& origin)
{
    SetOrigin(origin.x, origin.y);
}


////////////////////////////////////////////////////////////
const Vector2f& Transformable::GetPosition() const
{
    return myPosition;
}


////////////////////////////////////////////////////////////
float Transformable::GetRotation() const
{
    return myRotation;
}


////////////////////////////////////////////////////////////
const Vector2f& Transformable::GetScale() const
{
    return myScale;
}


////////////////////////////////////////////////////////////
const Vector2f& Transformable::GetOrigin() const
{
    return myOrigin;
}


////////////////////////////////////////////////////////////
void Transformable::Move(float offsetX, float offsetY)
{
    SetPosition(myPosition.x + offsetX, myPosition.y + offsetY);
}


////////////////////////////////////////////////////////////
void Transformable::Move(const Vector2f& offset)
{
    SetPosition(myPosition.x + offset.x, myPosition.y + offset.y);
}


////////////////////////////////////////////////////////////
void Transformable::Rotate(float angle)
{
    SetRotation(myRotation + angle);
}


////////////////////////////////////////////////////////////
void Transformable::Scale(float factorX, float factorY)
{
    SetScale(myScale.x * factorX, myScale.y * factorY);
}


////////////////////////////////////////////////////////////
void Transformable::Scale(const Vector2f& factor)
{
    SetScale(myScale.x * factor.x, myScale.y * factor.y);
}


////////////////////////////////////////////////////////////
const Transform& Transformable::GetTransform() const
{
    // Recompute the combined transform if needed
    if (myTransformNeedUpdate)
    {
        float angle  = -myRotation * 3.141592654f / 180.f;
        float cosine = static_cast<float>(std::cos(angle));
        float sine   = static_cast<float>(std::sin(angle));
        float sxc    = myScale.x * cosine;
        float syc    = myScale.y * cosine;
        float sxs    = myScale.x * sine;
        float sys    = myScale.y * sine;
        float tx     = -myOrigin.x * sxc - myOrigin.y * sys + myPosition.x;
        float ty     =  myOrigin.x * sxs - myOrigin.y * syc + myPosition.y;

        myTransform = Transform( sxc, sys, tx,
                                -sxs, syc, ty,
                                 0.f, 0.f, 1.f);
        myTransformNeedUpdate = false;
    }

    return myTransform;
}


////////////////////////////////////////////////////////////
const Transform& Transformable::GetInverseTransform() const
{
    // Recompute the inverse transform if needed
    if (myInverseTransformNeedUpdate)
    {
        myInverseTransform = GetTransform().GetInverse();
        myInverseTransformNeedUpdate = false;
    }

    return myInverseTransform;
}

} // namespace sf

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/System/Angle.hpp>
#include <cmath>

namespace
{
    const float pi = 3.141592654f;
}


namespace sf
{
////////////////////////////////////////////////////////////
const Angle Angle::Zero;


////////////////////////////////////////////////////////////
Angle::Angle() :
m_radians(0)
{
}


////////////////////////////////////////////////////////////
float Angle::asDegrees() const
{
    return m_radians * 180 / ::pi;
}


////////////////////////////////////////////////////////////
float Angle::asRadians() const
{
    return m_radians;
}


////////////////////////////////////////////////////////////
Angle::Angle(float angle)
{
    // Normalize to [0, 360[
    angle = static_cast<float>(std::fmod(angle, 360));
    if (angle < 0)
        angle += 360;
    
    m_radians = angle * ::pi / 180;
}


////////////////////////////////////////////////////////////
Angle degrees(float amount)
{
    return Angle(amount);
}


////////////////////////////////////////////////////////////
Angle radians(float amount)
{
    return Angle(amount * 180 / ::pi);
}


////////////////////////////////////////////////////////////
bool operator ==(Angle left, Angle right)
{
    return left.asRadians() == right.asRadians();
}


////////////////////////////////////////////////////////////
bool operator !=(Angle left, Angle right)
{
    return left.asRadians() != right.asRadians();
}


////////////////////////////////////////////////////////////
bool operator <(Angle left, Angle right)
{
    return left.asRadians() < right.asRadians();
}


////////////////////////////////////////////////////////////
bool operator >(Angle left, Angle right)
{
    return left.asRadians() > right.asRadians();
}


////////////////////////////////////////////////////////////
bool operator <=(Angle left, Angle right)
{
    return left.asRadians() <= right.asRadians();
}


////////////////////////////////////////////////////////////
bool operator >=(Angle left, Angle right)
{
    return left.asRadians() >= right.asRadians();
}


////////////////////////////////////////////////////////////
Angle operator -(Angle right)
{
    return radians(-right.asRadians());
}


////////////////////////////////////////////////////////////
Angle operator +(Angle left, Angle right)
{
    return radians(left.asRadians() + right.asRadians());
}


////////////////////////////////////////////////////////////
Angle& operator +=(Angle& left, Angle right)
{
    return left = left + right;
}


////////////////////////////////////////////////////////////
Angle operator -(Angle left, Angle right)
{
    return radians(left.asRadians() - right.asRadians());
}


////////////////////////////////////////////////////////////
Angle& operator -=(Angle& left, Angle right)
{
    return left = left - right;
}


////////////////////////////////////////////////////////////
Angle operator *(Angle left, float right)
{
    return radians(left.asRadians() * right);
}


////////////////////////////////////////////////////////////
Angle operator *(Angle left, Int64 right)
{
    return radians(left.asRadians() * right);
}


////////////////////////////////////////////////////////////
Angle operator *(float left, Angle right)
{
    return right * left;
}


////////////////////////////////////////////////////////////
Angle operator *(Int64 left, Angle right)
{
    return right * left;
}


////////////////////////////////////////////////////////////
Angle& operator *=(Angle& left, float right)
{
    return left = left * right;
}


////////////////////////////////////////////////////////////
Angle& operator *=(Angle& left, Int64 right)
{
    return left = left * right;
}


////////////////////////////////////////////////////////////
Angle operator /(Angle left, float right)
{
    return radians(left.asRadians() / right);
}


////////////////////////////////////////////////////////////
Angle operator /(Angle left, Int64 right)
{
    return radians(left.asRadians() / right);
}


////////////////////////////////////////////////////////////
Angle& operator /=(Angle& left, float right)
{
    return left = left / right;
}


////////////////////////////////////////////////////////////
Angle& operator /=(Angle& left, Int64 right)
{
    return left = left / right;
}


////////////////////////////////////////////////////////////
float operator /(Angle left, Angle right)
{
    return left.asRadians() / right.asRadians();
}


////////////////////////////////////////////////////////////
Angle operator %(Angle left, Angle right)
{
    return radians(static_cast<float>(std::fmod(left.asRadians(), right.asRadians())));
}


////////////////////////////////////////////////////////////
Angle& operator %=(Angle& left, Angle right)
{
    return left = left % right;
}

} // namespace sf

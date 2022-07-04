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

namespace priv
{
constexpr float pi = 3.141592654f;

constexpr float positiveRemainder(float a, float b)
{
    assert(b > 0.0f);
    const float val = a - static_cast<float>(static_cast<int>(a / b)) * b;
    if (val >= 0.f)
        return val;
    else
        return val + b;
}
} // namespace priv


////////////////////////////////////////////////////////////
constexpr Angle::Angle() : m_degrees(0.0f)
{
}


////////////////////////////////////////////////////////////
constexpr float Angle::asDegrees() const
{
    return m_degrees;
}


////////////////////////////////////////////////////////////
constexpr float Angle::asRadians() const
{
    return m_degrees * (priv::pi / 180);
}


////////////////////////////////////////////////////////////
constexpr Angle Angle::wrapSigned() const
{
    return degrees(priv::positiveRemainder(m_degrees + 180, 360) - 180);
}


////////////////////////////////////////////////////////////
constexpr Angle Angle::wrapUnsigned() const
{
    return degrees(priv::positiveRemainder(m_degrees, 360));
}


////////////////////////////////////////////////////////////
constexpr Angle::Angle(float degrees) : m_degrees(degrees)
{
}


////////////////////////////////////////////////////////////
constexpr Angle degrees(float angle)
{
    return Angle(angle);
}


////////////////////////////////////////////////////////////
constexpr Angle radians(float angle)
{
    return Angle(angle * (180 / priv::pi));
}


////////////////////////////////////////////////////////////
constexpr bool operator==(Angle left, Angle right)
{
    return left.asDegrees() == right.asDegrees();
}


////////////////////////////////////////////////////////////
constexpr bool operator!=(Angle left, Angle right)
{
    return left.asDegrees() != right.asDegrees();
}


////////////////////////////////////////////////////////////
constexpr bool operator<(Angle left, Angle right)
{
    return left.asDegrees() < right.asDegrees();
}


////////////////////////////////////////////////////////////
constexpr bool operator>(Angle left, Angle right)
{
    return left.asDegrees() > right.asDegrees();
}


////////////////////////////////////////////////////////////
constexpr bool operator<=(Angle left, Angle right)
{
    return left.asDegrees() <= right.asDegrees();
}


////////////////////////////////////////////////////////////
constexpr bool operator>=(Angle left, Angle right)
{
    return left.asDegrees() >= right.asDegrees();
}


////////////////////////////////////////////////////////////
constexpr Angle operator-(Angle right)
{
    return degrees(-right.asDegrees());
}


////////////////////////////////////////////////////////////
constexpr Angle operator+(Angle left, Angle right)
{
    return degrees(left.asDegrees() + right.asDegrees());
}


////////////////////////////////////////////////////////////
constexpr Angle& operator+=(Angle& left, Angle right)
{
    return left = left + right;
}


////////////////////////////////////////////////////////////
constexpr Angle operator-(Angle left, Angle right)
{
    return degrees(left.asDegrees() - right.asDegrees());
}


////////////////////////////////////////////////////////////
constexpr Angle& operator-=(Angle& left, Angle right)
{
    return left = left - right;
}


////////////////////////////////////////////////////////////
constexpr Angle operator*(Angle left, float right)
{
    return degrees(left.asDegrees() * right);
}


////////////////////////////////////////////////////////////
constexpr Angle operator*(float left, Angle right)
{
    return right * left;
}


////////////////////////////////////////////////////////////
constexpr Angle& operator*=(Angle& left, float right)
{
    return left = left * right;
}


////////////////////////////////////////////////////////////
constexpr Angle operator/(Angle left, float right)
{
    return degrees(left.asDegrees() / right);
}


////////////////////////////////////////////////////////////
constexpr Angle& operator/=(Angle& left, float right)
{
    return left = left / right;
}


////////////////////////////////////////////////////////////
constexpr float operator/(Angle left, Angle right)
{
    return left.asDegrees() / right.asDegrees();
}


////////////////////////////////////////////////////////////
constexpr Angle operator%(Angle left, Angle right)
{
    return degrees(priv::positiveRemainder(left.asDegrees(), right.asDegrees()));
}


////////////////////////////////////////////////////////////
constexpr Angle& operator%=(Angle& left, Angle right)
{
    return left = left % right;
}

namespace Literals
{

////////////////////////////////////////////////////////////
constexpr Angle operator"" _deg(long double angle)
{
    return degrees(static_cast<float>(angle));
}


////////////////////////////////////////////////////////////
constexpr Angle operator"" _deg(unsigned long long angle)
{
    return degrees(static_cast<float>(angle));
}


////////////////////////////////////////////////////////////
constexpr Angle operator"" _rad(long double angle)
{
    return radians(static_cast<float>(angle));
}


////////////////////////////////////////////////////////////
constexpr Angle operator"" _rad(unsigned long long angle)
{
    return radians(static_cast<float>(angle));
}

} // namespace Literals


////////////////////////////////////////////////////////////
// Static member data
////////////////////////////////////////////////////////////

// Note: the 'inline' keyword here is technically not required, but VS2019 fails
// to compile with a bogus "multiple definition" error if not explicitly used.
inline constexpr Angle Angle::Zero;

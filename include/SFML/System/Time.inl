////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2021 Laurent Gomila (laurent@sfml-dev.org)
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
constexpr Time::Time() :
m_microseconds(0)
{
}


////////////////////////////////////////////////////////////
constexpr float Time::asSeconds() const
{
    return static_cast<float>(static_cast<double>(m_microseconds) / 1000000.0);
}


////////////////////////////////////////////////////////////
constexpr Int32 Time::asMilliseconds() const
{
    return static_cast<Int32>(m_microseconds / 1000);
}


////////////////////////////////////////////////////////////
constexpr Int64 Time::asMicroseconds() const
{
    return m_microseconds;
}


////////////////////////////////////////////////////////////
constexpr Time::Time(Int64 microseconds) :
m_microseconds(microseconds)
{
}


////////////////////////////////////////////////////////////
constexpr Time seconds(float amount)
{
    return Time(static_cast<Int64>(amount * 1000000));
}


////////////////////////////////////////////////////////////
constexpr Time milliseconds(Int32 amount)
{
    return Time(static_cast<Int64>(amount) * 1000);
}


////////////////////////////////////////////////////////////
constexpr Time microseconds(Int64 amount)
{
    return Time(amount);
}


////////////////////////////////////////////////////////////
constexpr bool operator ==(Time left, Time right)
{
    return left.asMicroseconds() == right.asMicroseconds();
}


////////////////////////////////////////////////////////////
constexpr bool operator !=(Time left, Time right)
{
    return left.asMicroseconds() != right.asMicroseconds();
}


////////////////////////////////////////////////////////////
constexpr bool operator <(Time left, Time right)
{
    return left.asMicroseconds() < right.asMicroseconds();
}


////////////////////////////////////////////////////////////
constexpr bool operator >(Time left, Time right)
{
    return left.asMicroseconds() > right.asMicroseconds();
}


////////////////////////////////////////////////////////////
constexpr bool operator <=(Time left, Time right)
{
    return left.asMicroseconds() <= right.asMicroseconds();
}


////////////////////////////////////////////////////////////
constexpr bool operator >=(Time left, Time right)
{
    return left.asMicroseconds() >= right.asMicroseconds();
}


////////////////////////////////////////////////////////////
constexpr Time operator -(Time right)
{
    return microseconds(-right.asMicroseconds());
}


////////////////////////////////////////////////////////////
constexpr Time operator +(Time left, Time right)
{
    return microseconds(left.asMicroseconds() + right.asMicroseconds());
}


////////////////////////////////////////////////////////////
constexpr Time& operator +=(Time& left, Time right)
{
    return left = left + right;
}


////////////////////////////////////////////////////////////
constexpr Time operator -(Time left, Time right)
{
    return microseconds(left.asMicroseconds() - right.asMicroseconds());
}


////////////////////////////////////////////////////////////
constexpr Time& operator -=(Time& left, Time right)
{
    return left = left - right;
}


////////////////////////////////////////////////////////////
constexpr Time operator *(Time left, float right)
{
    return seconds(left.asSeconds() * right);
}


////////////////////////////////////////////////////////////
constexpr Time operator *(Time left, Int64 right)
{
    return microseconds(left.asMicroseconds() * right);
}


////////////////////////////////////////////////////////////
constexpr Time operator *(float left, Time right)
{
    return right * left;
}


////////////////////////////////////////////////////////////
constexpr Time operator *(Int64 left, Time right)
{
    return right * left;
}


////////////////////////////////////////////////////////////
constexpr Time& operator *=(Time& left, float right)
{
    return left = left * right;
}


////////////////////////////////////////////////////////////
constexpr Time& operator *=(Time& left, Int64 right)
{
    return left = left * right;
}


////////////////////////////////////////////////////////////
constexpr Time operator /(Time left, float right)
{
    return seconds(left.asSeconds() / right);
}


////////////////////////////////////////////////////////////
constexpr Time operator /(Time left, Int64 right)
{
    return microseconds(left.asMicroseconds() / right);
}


////////////////////////////////////////////////////////////
constexpr Time& operator /=(Time& left, float right)
{
    return left = left / right;
}


////////////////////////////////////////////////////////////
constexpr Time& operator /=(Time& left, Int64 right)
{
    return left = left / right;
}


////////////////////////////////////////////////////////////
constexpr float operator /(Time left, Time right)
{
    return left.asSeconds() / right.asSeconds();
}


////////////////////////////////////////////////////////////
constexpr Time operator %(Time left, Time right)
{
    return microseconds(left.asMicroseconds() % right.asMicroseconds());
}


////////////////////////////////////////////////////////////
constexpr Time& operator %=(Time& left, Time right)
{
    return left = left % right;
}

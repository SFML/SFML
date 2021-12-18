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
constexpr Color::Color() :
r(0),
g(0),
b(0),
a(255)
{

}


////////////////////////////////////////////////////////////
constexpr Color::Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) :
r(red),
g(green),
b(blue),
a(alpha)
{

}


////////////////////////////////////////////////////////////
constexpr Color::Color(Uint32 color) :
r(static_cast<Uint8>((color & 0xff000000) >> 24)),
g(static_cast<Uint8>((color & 0x00ff0000) >> 16)),
b(static_cast<Uint8>((color & 0x0000ff00) >> 8)),
a(static_cast<Uint8>(color & 0x000000ff))
{

}


////////////////////////////////////////////////////////////
constexpr Uint32 Color::toInteger() const
{
    return static_cast<Uint32>((r << 24) | (g << 16) | (b << 8) | a);
}


////////////////////////////////////////////////////////////
constexpr bool operator ==(const Color& left, const Color& right)
{
    return (left.r == right.r) &&
           (left.g == right.g) &&
           (left.b == right.b) &&
           (left.a == right.a);
}


////////////////////////////////////////////////////////////
constexpr bool operator !=(const Color& left, const Color& right)
{
    return !(left == right);
}


////////////////////////////////////////////////////////////
constexpr Color operator +(const Color& left, const Color& right)
{
    const auto clampedAdd = [](Uint8 lhs, Uint8 rhs) -> Uint8
    {
        const int intResult = static_cast<int>(lhs) + static_cast<int>(rhs);
        return static_cast<Uint8>(intResult < 255 ? intResult : 255);
    };

    return Color(clampedAdd(left.r, right.r),
                 clampedAdd(left.g, right.g),
                 clampedAdd(left.b, right.b),
                 clampedAdd(left.a, right.a));
}


////////////////////////////////////////////////////////////
constexpr Color operator -(const Color& left, const Color& right)
{
    const auto clampedSub = [](Uint8 lhs, Uint8 rhs) -> Uint8
    {
        const int intResult = static_cast<int>(lhs) - static_cast<int>(rhs);
        return static_cast<Uint8>(intResult > 0 ? intResult : 0);
    };

    return Color(clampedSub(left.r, right.r),
                 clampedSub(left.g, right.g),
                 clampedSub(left.b, right.b),
                 clampedSub(left.a, right.a));
}


////////////////////////////////////////////////////////////
constexpr Color operator *(const Color& left, const Color& right)
{
    const auto scaledMul = [](Uint8 lhs, Uint8 rhs) -> Uint8
    {
        const auto uint16Result = static_cast<Uint16>(static_cast<Uint16>(lhs) * static_cast<Uint16>(rhs));
        return static_cast<Uint8>(uint16Result / 255u);
    };

    return Color(scaledMul(left.r, right.r),
                 scaledMul(left.g, right.g),
                 scaledMul(left.b, right.b),
                 scaledMul(left.a, right.a));
}


////////////////////////////////////////////////////////////
constexpr Color& operator +=(Color& left, const Color& right)
{
    return left = left + right;
}


////////////////////////////////////////////////////////////
constexpr Color& operator -=(Color& left, const Color& right)
{
    return left = left - right;
}


////////////////////////////////////////////////////////////
constexpr Color& operator *=(Color& left, const Color& right)
{
    return left = left * right;
}

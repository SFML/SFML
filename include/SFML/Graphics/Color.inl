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


////////////////////////////////////////////////////////////
constexpr Color::Color() : r(0), g(0), b(0), a(255)
{
}


////////////////////////////////////////////////////////////
constexpr Color::Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha) :
r(red),
g(green),
b(blue),
a(alpha)
{
}


////////////////////////////////////////////////////////////
constexpr Color::Color(std::uint32_t color) :
r(static_cast<std::uint8_t>((color & 0xff000000) >> 24)),
g(static_cast<std::uint8_t>((color & 0x00ff0000) >> 16)),
b(static_cast<std::uint8_t>((color & 0x0000ff00) >> 8)),
a(static_cast<std::uint8_t>(color & 0x000000ff))
{
}


////////////////////////////////////////////////////////////
constexpr std::uint32_t Color::toInteger() const
{
    return static_cast<std::uint32_t>((r << 24) | (g << 16) | (b << 8) | a);
}


////////////////////////////////////////////////////////////
constexpr bool operator==(const Color& left, const Color& right)
{
    return (left.r == right.r) && (left.g == right.g) && (left.b == right.b) && (left.a == right.a);
}


////////////////////////////////////////////////////////////
constexpr bool operator!=(const Color& left, const Color& right)
{
    return !(left == right);
}


////////////////////////////////////////////////////////////
constexpr Color operator+(const Color& left, const Color& right)
{
    const auto clampedAdd = [](std::uint8_t lhs, std::uint8_t rhs) -> std::uint8_t
    {
        const int intResult = static_cast<int>(lhs) + static_cast<int>(rhs);
        return static_cast<std::uint8_t>(intResult < 255 ? intResult : 255);
    };

    return Color(clampedAdd(left.r, right.r),
                 clampedAdd(left.g, right.g),
                 clampedAdd(left.b, right.b),
                 clampedAdd(left.a, right.a));
}


////////////////////////////////////////////////////////////
constexpr Color operator-(const Color& left, const Color& right)
{
    const auto clampedSub = [](std::uint8_t lhs, std::uint8_t rhs) -> std::uint8_t
    {
        const int intResult = static_cast<int>(lhs) - static_cast<int>(rhs);
        return static_cast<std::uint8_t>(intResult > 0 ? intResult : 0);
    };

    return Color(clampedSub(left.r, right.r),
                 clampedSub(left.g, right.g),
                 clampedSub(left.b, right.b),
                 clampedSub(left.a, right.a));
}


////////////////////////////////////////////////////////////
constexpr Color operator*(const Color& left, const Color& right)
{
    const auto scaledMul = [](std::uint8_t lhs, std::uint8_t rhs) -> std::uint8_t
    {
        const auto uint16Result = static_cast<std::uint16_t>(
            static_cast<std::uint16_t>(lhs) * static_cast<std::uint16_t>(rhs));
        return static_cast<std::uint8_t>(uint16Result / 255u);
    };

    return Color(scaledMul(left.r, right.r),
                 scaledMul(left.g, right.g),
                 scaledMul(left.b, right.b),
                 scaledMul(left.a, right.a));
}


////////////////////////////////////////////////////////////
constexpr Color& operator+=(Color& left, const Color& right)
{
    return left = left + right;
}


////////////////////////////////////////////////////////////
constexpr Color& operator-=(Color& left, const Color& right)
{
    return left = left - right;
}


////////////////////////////////////////////////////////////
constexpr Color& operator*=(Color& left, const Color& right)
{
    return left = left * right;
}


////////////////////////////////////////////////////////////
// Static member data
////////////////////////////////////////////////////////////

// Note: the 'inline' keyword here is technically not required, but VS2019 fails
// to compile with a bogus "multiple definition" error if not explicitly used.
inline constexpr Color Color::Black(0, 0, 0);
inline constexpr Color Color::White(255, 255, 255);
inline constexpr Color Color::Red(255, 0, 0);
inline constexpr Color Color::Green(0, 255, 0);
inline constexpr Color Color::Blue(0, 0, 255);
inline constexpr Color Color::Yellow(255, 255, 0);
inline constexpr Color Color::Magenta(255, 0, 255);
inline constexpr Color Color::Cyan(0, 255, 255);
inline constexpr Color Color::Transparent(0, 0, 0, 0);

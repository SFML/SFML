////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Graphics/Color.hpp>
#include <algorithm>


namespace sf
{
////////////////////////////////////////////////////////////
// Static member data
////////////////////////////////////////////////////////////
const Color Color::Black(0, 0, 0);
const Color Color::White(255, 255, 255);
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);
const Color Color::Blue(0, 0, 255);
const Color Color::Yellow(255, 255, 0);
const Color Color::Magenta(255, 0, 255);
const Color Color::Cyan(0, 255, 255);
const Color Color::Transparent(0, 0, 0, 0);


////////////////////////////////////////////////////////////
Color::Color() :
r(0),
g(0),
b(0),
a(255)
{

}


////////////////////////////////////////////////////////////
Color::Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) :
r(red),
g(green),
b(blue),
a(alpha)
{

}


////////////////////////////////////////////////////////////
bool operator ==(const Color& left, const Color& right)
{
    return (left.r == right.r) &&
           (left.g == right.g) &&
           (left.b == right.b) &&
           (left.a == right.a);
}


////////////////////////////////////////////////////////////
bool operator !=(const Color& left, const Color& right)
{
    return !(left == right);
}


////////////////////////////////////////////////////////////
Color operator +(const Color& left, const Color& right)
{
    return Color(static_cast<Uint8>(std::min(left.r + right.r, 255)),
                 static_cast<Uint8>(std::min(left.g + right.g, 255)),
                 static_cast<Uint8>(std::min(left.b + right.b, 255)),
                 static_cast<Uint8>(std::min(left.a + right.a, 255)));
}


////////////////////////////////////////////////////////////
Color operator *(const Color& left, const Color& right)
{
    return Color(static_cast<Uint8>(left.r * right.r / 255),
                 static_cast<Uint8>(left.g * right.g / 255),
                 static_cast<Uint8>(left.b * right.b / 255),
                 static_cast<Uint8>(left.a * right.a / 255));
}


////////////////////////////////////////////////////////////
Color& operator +=(Color& left, const Color& right)
{
    return left = left + right;
}


////////////////////////////////////////////////////////////
Color& operator *=(Color& left, const Color& right)
{
    return left = left * right;
}

} // namespace sf

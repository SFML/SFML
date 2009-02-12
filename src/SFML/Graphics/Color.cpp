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


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Color::Color() :
r(0),
g(0),
b(0),
a(255)
{

}


////////////////////////////////////////////////////////////
/// Construct the color from its 4 RGBA components
////////////////////////////////////////////////////////////
Color::Color(Uint8 R, Uint8 G, Uint8 B, Uint8 A) :
r(R),
g(G),
b(B),
a(A)
{

}


////////////////////////////////////////////////////////////
/// Operator += overload to add a color
////////////////////////////////////////////////////////////
Color& Color::operator +=(const Color& Other)
{
    r = static_cast<Uint8>(std::min(r + Other.r, 255));
    g = static_cast<Uint8>(std::min(g + Other.g, 255));
    b = static_cast<Uint8>(std::min(b + Other.b, 255));
    a = static_cast<Uint8>(std::min(a + Other.a, 255));

    return *this;
}


////////////////////////////////////////////////////////////
/// Operator *= overload to modulate a color
////////////////////////////////////////////////////////////
Color& Color::operator *=(const Color& Other)
{
    r = static_cast<Uint8>(r * Other.r / 255);
    g = static_cast<Uint8>(g * Other.g / 255);
    b = static_cast<Uint8>(b * Other.b / 255);
    a = static_cast<Uint8>(a * Other.a / 255);

    return *this;
}


////////////////////////////////////////////////////////////
/// Compare two colors (for equality)
////////////////////////////////////////////////////////////
bool Color::operator ==(const Color& Other) const
{
    return (r == Other.r) && (g == Other.g) && (b == Other.b) && (a == Other.a);
}


////////////////////////////////////////////////////////////
/// Compare two colors (for difference)
////////////////////////////////////////////////////////////
bool Color::operator !=(const Color& Other) const
{
    return (r != Other.r) || (g != Other.g) || (b != Other.b) || (a != Other.a);
}


////////////////////////////////////////////////////////////
/// Operator + overload to add two colors
////////////////////////////////////////////////////////////
Color operator +(const Color& Color1, const Color& Color2)
{
    Color c = Color1;
    c += Color2;

    return c;
}


////////////////////////////////////////////////////////////
/// Operator * overload to modulate two colors
////////////////////////////////////////////////////////////
Color operator *(const Color& Color1, const Color& Color2)
{
    Color c = Color1;
    c *= Color2;

    return c;
}

} // namespace sf

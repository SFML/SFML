////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2008 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Graphics/Color.h>
#include <SFML/Internal.h>
#include <algorithm>


////////////////////////////////////////////////////////////
/// Define some common colors
////////////////////////////////////////////////////////////
sfColor sfBlack   = {  0,   0,   0, 255};
sfColor sfWhite   = {255, 255, 255, 255};
sfColor sfRed     = {255,   0,   0, 255};
sfColor sfGreen   = {  0, 255,   0, 255};
sfColor sfBlue    = {  0,   0, 255, 255};
sfColor sfYellow  = {255, 255,   0, 255};
sfColor sfMagenta = {255,   0, 255, 255};
sfColor sfCyan    = {  0, 255, 255, 255};


////////////////////////////////////////////////////////////
/// Construct a color from its 3 RGB components
////////////////////////////////////////////////////////////
sfColor sfColor_FromRGB(sfUint8 R, sfUint8 G, sfUint8 B)
{
    return sfColor_FromRGBA(R, G, B, 255);
}


////////////////////////////////////////////////////////////
/// Construct a color from its 4 RGBA components
////////////////////////////////////////////////////////////
sfColor sfColor_FromRGBA(sfUint8 R, sfUint8 G, sfUint8 B, sfUint8 A)
{
    sfColor Color;

    Color.r = R;
    Color.g = G;
    Color.b = B;
    Color.a = A;

    return Color;
}


////////////////////////////////////////////////////////////
/// Add two colors
////////////////////////////////////////////////////////////
sfColor sfColor_Add(sfColor Color1, sfColor Color2)
{
    int R = std::min(Color1.r + Color2.r, 255);
    int G = std::min(Color1.g + Color2.g, 255);
    int B = std::min(Color1.b + Color2.b, 255);
    int A = std::min(Color1.a + Color2.a, 255);

    return sfColor_FromRGBA(static_cast<sfUint8>(R), static_cast<sfUint8>(G), static_cast<sfUint8>(B), static_cast<sfUint8>(A));
}


////////////////////////////////////////////////////////////
/// Modulate two colors
////////////////////////////////////////////////////////////
sfColor sfColor_Modulate(sfColor Color1, sfColor Color2)
{
    int R = Color1.r * Color2.r / 255;
    int G = Color1.g * Color2.g / 255;
    int B = Color1.b * Color2.b / 255;
    int A = Color1.a * Color2.a / 255;

    return sfColor_FromRGBA(static_cast<sfUint8>(R), static_cast<sfUint8>(G), static_cast<sfUint8>(B), static_cast<sfUint8>(A));
}

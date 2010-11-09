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
#include <SFML/Graphics/Color.h>
#include <SFML/Internal.h>
#include <algorithm>


////////////////////////////////////////////////////////////
/// Define some common colors
////////////////////////////////////////////////////////////
sfColor sfBlack   = sfColor_FromRGB(  0,   0,   0);
sfColor sfWhite   = sfColor_FromRGB(255, 255, 255);
sfColor sfRed     = sfColor_FromRGB(255,   0,   0);
sfColor sfGreen   = sfColor_FromRGB(  0, 255,   0);
sfColor sfBlue    = sfColor_FromRGB(  0,   0, 255);
sfColor sfYellow  = sfColor_FromRGB(255, 255,   0);
sfColor sfMagenta = sfColor_FromRGB(255,   0, 255);
sfColor sfCyan    = sfColor_FromRGB(  0, 255, 255);


////////////////////////////////////////////////////////////
/// Construct a color from its 3 RGB components
////////////////////////////////////////////////////////////
sfColor sfColor_FromRGB(sfUint8 red, sfUint8 green, sfUint8 blue)
{
    return sfColor_FromRGBA(red, green, blue, 255);
}


////////////////////////////////////////////////////////////
/// Construct a color from its 4 RGBA components
////////////////////////////////////////////////////////////
sfColor sfColor_FromRGBA(sfUint8 red, sfUint8 green, sfUint8 blue, sfUint8 alpha)
{
    sfColor color;

    color.r = red;
    color.g = green;
    color.b = blue;
    color.a = alpha;

    return color;
}


////////////////////////////////////////////////////////////
/// Add two colors
////////////////////////////////////////////////////////////
sfColor sfColor_Add(sfColor color1, sfColor color2)
{
    int red   = std::min(color1.r + color2.r, 255);
    int green = std::min(color1.g + color2.g, 255);
    int blue  = std::min(color1.b + color2.b, 255);
    int alpha = std::min(color1.a + color2.a, 255);

    return sfColor_FromRGBA(static_cast<sfUint8>(red),
                            static_cast<sfUint8>(green),
                            static_cast<sfUint8>(blue),
                            static_cast<sfUint8>(alpha));
}


////////////////////////////////////////////////////////////
/// Modulate two colors
////////////////////////////////////////////////////////////
sfColor sfColor_Modulate(sfColor color1, sfColor color2)
{
    int red   = color1.r * color2.r / 255;
    int green = color1.g * color2.g / 255;
    int blue  = color1.b * color2.b / 255;
    int alpha = color1.a * color2.a / 255;

    return sfColor_FromRGBA(static_cast<sfUint8>(red),
                            static_cast<sfUint8>(green),
                            static_cast<sfUint8>(blue),
                            static_cast<sfUint8>(alpha));
}

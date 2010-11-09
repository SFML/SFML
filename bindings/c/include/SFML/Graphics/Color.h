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

#ifndef SFML_COLOR_H
#define SFML_COLOR_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>


////////////////////////////////////////////////////////////
/// sfColor is an utility class for manipulating colors
////////////////////////////////////////////////////////////
typedef struct
{
    sfUint8 r;
    sfUint8 g;
    sfUint8 b;
    sfUint8 a;
} sfColor;


////////////////////////////////////////////////////////////
/// Define some common colors
////////////////////////////////////////////////////////////
CSFML_API sfColor sfBlack;
CSFML_API sfColor sfWhite;
CSFML_API sfColor sfRed;
CSFML_API sfColor sfGreen;
CSFML_API sfColor sfBlue;
CSFML_API sfColor sfYellow;
CSFML_API sfColor sfMagenta;
CSFML_API sfColor sfCyan;


////////////////////////////////////////////////////////////
/// Construct a color from its 3 RGB components
///
/// \param red :   Red component   (0 .. 255)
/// \param green : Green component (0 .. 255)
/// \param blue :  Blue component  (0 .. 255)
///
/// \return sfColor constructed from the components
///
////////////////////////////////////////////////////////////
CSFML_API sfColor sfColor_FromRGB(sfUint8 red, sfUint8 green, sfUint8 blue);

////////////////////////////////////////////////////////////
/// Construct a color from its 4 RGBA components
///
/// \param red :   Red component   (0 .. 255)
/// \param green : Green component (0 .. 255)
/// \param blue :  Blue component  (0 .. 255)
/// \param alpha : Alpha component (0 .. 255)
///
/// \return sfColor constructed from the components
///
////////////////////////////////////////////////////////////
CSFML_API sfColor sfColor_FromRGBA(sfUint8 red, sfUint8 green, sfUint8 blue, sfUint8 alpha);

////////////////////////////////////////////////////////////
/// Add two colors
///
/// \param color1 : First color
/// \param color2 : Second color
///
/// \return Component-wise saturated addition of the two colors
///
////////////////////////////////////////////////////////////
CSFML_API sfColor sfColor_Add(sfColor color1, sfColor color2);

////////////////////////////////////////////////////////////
/// Modulate two colors
///
/// \param color1 : First color
/// \param color2 : Second color
///
/// \return Component-wise multiplication of the two colors
///
////////////////////////////////////////////////////////////
CSFML_API sfColor sfColor_Modulate(sfColor color1, sfColor color2);


#endif // SFML_COLOR_H

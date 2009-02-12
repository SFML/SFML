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

#ifndef SFML_COLOR_HPP
#define SFML_COLOR_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// Color is an utility class for manipulating
/// 32-bits RGBA colors
////////////////////////////////////////////////////////////
class SFML_API Color
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Color();

    ////////////////////////////////////////////////////////////
    /// Construct the color from its 4 RGBA components
    ///
    /// \param R : Red component   (0 .. 255)
    /// \param G : Green component (0 .. 255)
    /// \param B : Blue component  (0 .. 255)
    /// \param A : Alpha component (0 .. 255) (255 by default)
    ///
    ////////////////////////////////////////////////////////////
    Color(Uint8 R, Uint8 G, Uint8 B, Uint8 A = 255);

    ////////////////////////////////////////////////////////////
    /// Operator += overload to add a color
    ///
    /// \param Other : Color to add
    ///
    /// \return Component-wise saturated addition of the two colors
    ///
    ////////////////////////////////////////////////////////////
    Color& operator +=(const Color& Other);

    ////////////////////////////////////////////////////////////
    /// Operator *= overload to modulate a color
    ///
    /// \param Other : Color to modulate
    ///
    /// \return Component-wise multiplication of the two colors
    ///
    ////////////////////////////////////////////////////////////
    Color& operator *=(const Color& Other);

    ////////////////////////////////////////////////////////////
    /// Compare two colors (for equality)
    ///
    /// \param Other : Color to compare
    ///
    /// \return True if colors are equal
    ///
    ////////////////////////////////////////////////////////////
    bool operator ==(const Color& Other) const;

    ////////////////////////////////////////////////////////////
    /// Compare two colors (for difference)
    ///
    /// \param Other : Color to compare
    ///
    /// \return True if colors are different
    ///
    ////////////////////////////////////////////////////////////
    bool operator !=(const Color& Other) const;

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    static const Color Black;   ///< Black predefined color
    static const Color White;   ///< White predefined color
    static const Color Red;     ///< Red predefined color
    static const Color Green;   ///< Green predefined color
    static const Color Blue;    ///< Blue predefined color
    static const Color Yellow;  ///< Yellow predefined color
    static const Color Magenta; ///< Magenta predefined color
    static const Color Cyan;    ///< Cyan predefined color

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Uint8 r; ///< Red component
    Uint8 g; ///< Green component
    Uint8 b; ///< Blue component
    Uint8 a; ///< Alpha (transparency) component
};

////////////////////////////////////////////////////////////
/// Operator + overload to add two colors
///
/// \param Color1 : First color
/// \param Color2 : Second color
///
/// \return Component-wise saturated addition of the two colors
///
////////////////////////////////////////////////////////////
SFML_API Color operator +(const Color& Color1, const Color& Color2);

////////////////////////////////////////////////////////////
/// Operator * overload to modulate two colors
///
/// \param Color1 : First color
/// \param Color2 : Second color
///
/// \return Component-wise multiplication of the two colors
///
////////////////////////////////////////////////////////////
SFML_API Color operator *(const Color& Color1, const Color& Color2);

} // namespace sf


#endif // SFML_COLOR_HPP

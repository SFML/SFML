////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2017 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/Export.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Utility class for manipulating RGBA colors
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API Color
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Constructs an opaque black color. It is equivalent to
    /// sf::Color(0, 0, 0, 255).
    ///
    ////////////////////////////////////////////////////////////
    Color();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the color from its 4 RGBA components
    ///
    /// \param red   Red component (in the range [0, 255])
    /// \param green Green component (in the range [0, 255])
    /// \param blue  Blue component (in the range [0, 255])
    /// \param alpha Alpha (opacity) component (in the range [0, 255])
    ///
    ////////////////////////////////////////////////////////////
    Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the color from 32-bit unsigned integer
    ///
    /// \param color Number containing the RGBA components (in that order)
    ///
    ////////////////////////////////////////////////////////////
    explicit Color(Uint32 color);

    ////////////////////////////////////////////////////////////
    /// \brief Retrieve the color as a 32-bit unsigned integer
    ///
    /// \return Color represented as a 32-bit unsigned integer
    ///
    ////////////////////////////////////////////////////////////
    Uint32 toInteger() const;

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    static const Color Transparent                 ///< Transparent (black) predefined color
    static const Color AliceBlue                   ///< AliceBlue predefined color
    static const Color AntiqueWhite                ///< AntiqueWhite predefined color
    static const Color Aqua                        ///< Aqua predefined color
    static const Color Aquamarine                  ///< Aquamarine predefined color
    static const Color Azure                       ///< Azure predefined color
    static const Color Beige                       ///< Beige predefined color
    static const Color Bisque                      ///< Bisque predefined color
    static const Color Black                       ///< Black predefined color
    static const Color BlanchedAlmond              ///< BlanchedAlmond predefined color
    static const Color Blue                        ///< Blue predefined color
    static const Color BlueViolet                  ///< BlueViolet predefined color
    static const Color Brown                       ///< Brown predefined color
    static const Color BurlyWood                   ///< BurlyWood predefined color
    static const Color CadetBlue                   ///< CadetBlue predefined color
    static const Color Chartreuse                  ///< Chartreuse predefined color
    static const Color Chocolate                   ///< Chocolate predefined color
    static const Color Coral                       ///< Coral predefined color
    static const Color CornflowerBlue              ///< CornflowerBlue predefined color
    static const Color Cornsilk                    ///< Cornsilk predefined color
    static const Color Crimson                     ///< Crimson predefined color
    static const Color Cyan                        ///< Cyan predefined color
    static const Color DarkBlue                    ///< DarkBlue predefined color
    static const Color DarkCyan                    ///< DarkCyan predefined color
    static const Color DarkGoldenrod               ///< DarkGoldenrod predefined color
    static const Color DarkGray                    ///< DarkGray predefined color
    static const Color DarkGreen                   ///< DarkGreen predefined color
    static const Color DarkKhaki                   ///< DarkKhaki predefined color
    static const Color DarkMagenta                 ///< DarkMagenta predefined color
    static const Color DarkOliveGreen              ///< DarkOliveGreen predefined color
    static const Color DarkOrange                  ///< DarkOrange predefined color
    static const Color DarkOrchid                  ///< DarkOrchid predefined color
    static const Color DarkRed                     ///< DarkRed predefined color
    static const Color DarkSalmon                  ///< DarkSalmon predefined color
    static const Color DarkSeaGreen                ///< DarkSeaGreen predefined color
    static const Color DarkSlateBlue               ///< DarkSlateBlue predefined color
    static const Color DarkSlateGray               ///< DarkSlateGray predefined color
    static const Color DarkTurquoise               ///< DarkTurquoise predefined color
    static const Color DarkViolet                  ///< DarkViolet predefined color
    static const Color DeepPink                    ///< DeepPink predefined color
    static const Color DeepSkyBlue                 ///< DeepSkyBlue predefined color
    static const Color DimGray                     ///< DimGray predefined color
    static const Color DodgerBlue                  ///< DodgerBlue predefined color
    static const Color Firebrick                   ///< Firebrick predefined color
    static const Color FloralWhite                 ///< FloralWhite predefined color
    static const Color ForestGreen                 ///< ForestGreen predefined color
    static const Color Fuchsia                     ///< Fuchsia predefined color
    static const Color Gainsboro                   ///< Gainsboro predefined color
    static const Color GhostWhite                  ///< GhostWhite predefined color
    static const Color Gold                        ///< Gold predefined color
    static const Color Goldenrod                   ///< Goldenrod predefined color
    static const Color Gray                        ///< Gray predefined color
    static const Color Green                       ///< Green predefined color
    static const Color GreenYellow                 ///< GreenYellow predefined color
    static const Color Honeydew                    ///< Honeydew predefined color
    static const Color HotPink                     ///< HotPink predefined color
    static const Color IndianRed                   ///< IndianRed predefined color
    static const Color Indigo                      ///< Indigo predefined color
    static const Color Ivory                       ///< Ivory predefined color
    static const Color Khaki                       ///< Khaki predefined color
    static const Color Lavender                    ///< Lavender predefined color
    static const Color LavenderBlush               ///< LavenderBlush predefined color
    static const Color LawnGreen                   ///< LawnGreen predefined color
    static const Color LemonChiffon                ///< LemonChiffon predefined color
    static const Color LightBlue                   ///< LightBlue predefined color
    static const Color LightCoral                  ///< LightCoral predefined color
    static const Color LightCyan                   ///< LightCyan predefined color
    static const Color LightGoldenrodYellow        ///< LightGoldenrodYellow predefined color
    static const Color LightGreen                  ///< LightGreen predefined color
    static const Color LightGray                   ///< LightGray predefined color
    static const Color LightPink                   ///< LightPink predefined color
    static const Color LightSalmon                 ///< LightSalmon predefined color
    static const Color LightSeaGreen               ///< LightSeaGreen predefined color
    static const Color LightSkyBlue                ///< LightSkyBlue predefined color
    static const Color LightSlateGray              ///< LightSlateGray predefined color
    static const Color LightSteelBlue              ///< LightSteelBlue predefined color
    static const Color LightYellow                 ///< LightYellow predefined color
    static const Color Lime                        ///< Lime predefined color
    static const Color LimeGreen                   ///< LimeGreen predefined color
    static const Color Linen                       ///< Linen predefined color
    static const Color Magenta                     ///< Magenta predefined color
    static const Color Maroon                      ///< Maroon predefined color
    static const Color MediumAquamarine            ///< MediumAquamarine predefined color
    static const Color MediumBlue                  ///< MediumBlue predefined color
    static const Color MediumOrchid                ///< MediumOrchid predefined color
    static const Color MediumPurple                ///< MediumPurple predefined color
    static const Color MediumSeaGreen              ///< MediumSeaGreen predefined color
    static const Color MediumSlateBlue             ///< MediumSlateBlue predefined color
    static const Color MediumSpringGreen           ///< MediumSpringGreen predefined color
    static const Color MediumTurquoise             ///< MediumTurquoise predefined color
    static const Color MediumVioletRed             ///< MediumVioletRed predefined color
    static const Color MidnightBlue                ///< MidnightBlue predefined color
    static const Color MintCream                   ///< MintCream predefined color
    static const Color MistyRose                   ///< MistyRose predefined color
    static const Color Moccasin                    ///< Moccasin predefined color
    static const Color NavajoWhite                 ///< NavajoWhite predefined color
    static const Color Navy                        ///< Navy predefined color
    static const Color OldLace                     ///< OldLace predefined color
    static const Color Olive                       ///< Olive predefined color
    static const Color OliveDrab                   ///< OliveDrab predefined color
    static const Color Orange                      ///< Orange predefined color
    static const Color OrangeRed                   ///< OrangeRed predefined color
    static const Color Orchid                      ///< Orchid predefined color
    static const Color PaleGoldenrod               ///< PaleGoldenrod predefined color
    static const Color PaleGreen                   ///< PaleGreen predefined color
    static const Color PaleTurquoise               ///< PaleTurquoise predefined color
    static const Color PaleVioletRed               ///< PaleVioletRed predefined color
    static const Color PapayaWhip                  ///< PapayaWhip predefined color
    static const Color PeachPuff                   ///< PeachPuff predefined color
    static const Color Peru                        ///< Peru predefined color
    static const Color Pink                        ///< Pink predefined color
    static const Color Plum                        ///< Plum predefined color
    static const Color PowderBlue                  ///< PowderBlue predefined color
    static const Color Purple                      ///< Purple predefined color
    static const Color Red                         ///< Red predefined color
    static const Color RosyBrown                   ///< RosyBrown predefined color
    static const Color RoyalBlue                   ///< RoyalBlue predefined color
    static const Color SaddleBrown                 ///< SaddleBrown predefined color
    static const Color Salmon                      ///< Salmon predefined color
    static const Color SandyBrown                  ///< SandyBrown predefined color
    static const Color SeaGreen                    ///< SeaGreen predefined color
    static const Color SeaShell                    ///< SeaShell predefined color
    static const Color Sienna                      ///< Sienna predefined color
    static const Color Silver                      ///< Silver predefined color
    static const Color SkyBlue                     ///< SkyBlue predefined color
    static const Color SlateBlue                   ///< SlateBlue predefined color
    static const Color SlateGray                   ///< SlateGray predefined color
    static const Color Snow                        ///< Snow predefined color
    static const Color SpringGreen                 ///< SpringGreen predefined color
    static const Color SteelBlue                   ///< SteelBlue predefined color
    static const Color Tan                         ///< Tan predefined color
    static const Color Teal                        ///< Teal predefined color
    static const Color Thistle                     ///< Thistle predefined color
    static const Color Tomato                      ///< Tomato predefined color
    static const Color Turquoise                   ///< Turquoise predefined color
    static const Color Violet                      ///< Violet predefined color
    static const Color Wheat                       ///< Wheat predefined color
    static const Color White                       ///< White predefined color
    static const Color WhiteSmoke                  ///< WhiteSmoke predefined color
    static const Color Yellow                      ///< Yellow predefined color
    static const Color YellowGreen                 ///< YellowGreen predefined color

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Uint8 r; ///< Red component
    Uint8 g; ///< Green component
    Uint8 b; ///< Blue component
    Uint8 a; ///< Alpha (opacity) component
};

////////////////////////////////////////////////////////////
/// \relates Color
/// \brief Overload of the == operator
///
/// This operator compares two colors and check if they are equal.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return True if colors are equal, false if they are different
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API bool operator ==(const Color& left, const Color& right);

////////////////////////////////////////////////////////////
/// \relates Color
/// \brief Overload of the != operator
///
/// This operator compares two colors and check if they are different.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return True if colors are different, false if they are equal
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API bool operator !=(const Color& left, const Color& right);

////////////////////////////////////////////////////////////
/// \relates Color
/// \brief Overload of the binary + operator
///
/// This operator returns the component-wise sum of two colors.
/// Components that exceed 255 are clamped to 255.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return Result of \a left + \a right
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API Color operator +(const Color& left, const Color& right);

////////////////////////////////////////////////////////////
/// \relates Color
/// \brief Overload of the binary - operator
///
/// This operator returns the component-wise subtraction of two colors.
/// Components below 0 are clamped to 0.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return Result of \a left - \a right
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API Color operator -(const Color& left, const Color& right);

////////////////////////////////////////////////////////////
/// \relates Color
/// \brief Overload of the binary * operator
///
/// This operator returns the component-wise multiplication
/// (also called "modulation") of two colors.
/// Components are then divided by 255 so that the result is
/// still in the range [0, 255].
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return Result of \a left * \a right
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API Color operator *(const Color& left, const Color& right);

////////////////////////////////////////////////////////////
/// \relates Color
/// \brief Overload of the binary += operator
///
/// This operator computes the component-wise sum of two colors,
/// and assigns the result to the left operand.
/// Components that exceed 255 are clamped to 255.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API Color& operator +=(Color& left, const Color& right);

////////////////////////////////////////////////////////////
/// \relates Color
/// \brief Overload of the binary -= operator
///
/// This operator computes the component-wise subtraction of two colors,
/// and assigns the result to the left operand.
/// Components below 0 are clamped to 0.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API Color& operator -=(Color& left, const Color& right);

////////////////////////////////////////////////////////////
/// \relates Color
/// \brief Overload of the binary *= operator
///
/// This operator returns the component-wise multiplication
/// (also called "modulation") of two colors, and assigns
/// the result to the left operand.
/// Components are then divided by 255 so that the result is
/// still in the range [0, 255].
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API Color& operator *=(Color& left, const Color& right);

} // namespace sf


#endif // SFML_COLOR_HPP


////////////////////////////////////////////////////////////
/// \class sf::Color
/// \ingroup graphics
///
/// sf::Color is a simple color class composed of 4 components:
/// \li Red
/// \li Green
/// \li Blue
/// \li Alpha (opacity)
///
/// Each component is a public member, an unsigned integer in
/// the range [0, 255]. Thus, colors can be constructed and
/// manipulated very easily:
///
/// \code
/// sf::Color color(255, 0, 0); // red
/// color.r = 0;                // make it black
/// color.b = 128;              // make it dark blue
/// \endcode
///
/// The fourth component of colors, named "alpha", represents
/// the opacity of the color. A color with an alpha value of
/// 255 will be fully opaque, while an alpha value of 0 will
/// make a color fully transparent, whatever the value of the
/// other components is.
///
/// The most common colors are already defined as static variables:
/// \code
/// sf::Color black       = sf::Color::Black;
/// sf::Color white       = sf::Color::White;
/// sf::Color red         = sf::Color::Red;
/// sf::Color green       = sf::Color::Green;
/// sf::Color blue        = sf::Color::Blue;
/// sf::Color yellow      = sf::Color::Yellow;
/// sf::Color magenta     = sf::Color::Magenta;
/// sf::Color cyan        = sf::Color::Cyan;
/// sf::Color transparent = sf::Color::Transparent;
/// \endcode
///
/// Colors can also be added and modulated (multiplied) using the
/// overloaded operators + and *.
///
////////////////////////////////////////////////////////////

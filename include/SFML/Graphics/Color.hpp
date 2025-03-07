////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstdint>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Utility class for manipulating RGBA colors
///
////////////////////////////////////////////////////////////
class Color
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Constructs an opaque black color. It is equivalent to
    /// `sf::Color(0, 0, 0, 255)`.
    ///
    ////////////////////////////////////////////////////////////
    constexpr Color() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Construct the color from its 4 RGBA components
    ///
    /// \param red   Red component (in the range [0, 255])
    /// \param green Green component (in the range [0, 255])
    /// \param blue  Blue component (in the range [0, 255])
    /// \param alpha Alpha (opacity) component (in the range [0, 255])
    ///
    ////////////////////////////////////////////////////////////
    constexpr Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha = 255);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the color from 32-bit unsigned integer
    ///
    /// \param color Number containing the RGBA components (in that order)
    ///
    ////////////////////////////////////////////////////////////
    constexpr explicit Color(std::uint32_t color);

    ////////////////////////////////////////////////////////////
    /// \brief Retrieve the color as a 32-bit unsigned integer
    ///
    /// \return Color represented as a 32-bit unsigned integer
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] constexpr std::uint32_t toInteger() const;

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    // NOLINTBEGIN(readability-identifier-naming)
    static const Color Black;       //!< Black predefined color
    static const Color White;       //!< White predefined color
    static const Color Red;         //!< Red predefined color
    static const Color Green;       //!< Green predefined color
    static const Color Blue;        //!< Blue predefined color
    static const Color Yellow;      //!< Yellow predefined color
    static const Color Magenta;     //!< Magenta predefined color
    static const Color Cyan;        //!< Cyan predefined color
    static const Color Transparent; //!< Transparent (black) predefined color
    // NOLINTEND(readability-identifier-naming)

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::uint8_t r{};    //!< Red component
    std::uint8_t g{};    //!< Green component
    std::uint8_t b{};    //!< Blue component
    std::uint8_t a{255}; //!< Alpha (opacity) component
};

////////////////////////////////////////////////////////////
/// \relates Color
/// \brief Overload of the `operator==`
///
/// This operator compares two colors and check if they are equal.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return `true` if colors are equal, `false` if they are different
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr bool operator==(Color left, Color right);

////////////////////////////////////////////////////////////
/// \relates Color
/// \brief Overload of the `operator!=`
///
/// This operator compares two colors and check if they are different.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return `true` if colors are different, `false` if they are equal
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr bool operator!=(Color left, Color right);

////////////////////////////////////////////////////////////
/// \relates Color
/// \brief Overload of the binary `operator+`
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
[[nodiscard]] constexpr Color operator+(Color left, Color right);

////////////////////////////////////////////////////////////
/// \relates Color
/// \brief Overload of the binary `operator-`
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
[[nodiscard]] constexpr Color operator-(Color left, Color right);

////////////////////////////////////////////////////////////
/// \relates Color
/// \brief Overload of the binary `operator*`
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
[[nodiscard]] constexpr Color operator*(Color left, Color right);

////////////////////////////////////////////////////////////
/// \relates Color
/// \brief Overload of the binary `operator+=`
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
constexpr Color& operator+=(Color& left, Color right);

////////////////////////////////////////////////////////////
/// \relates Color
/// \brief Overload of the binary `operator-=`
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
constexpr Color& operator-=(Color& left, Color right);

////////////////////////////////////////////////////////////
/// \relates Color
/// \brief Overload of the binary `operator*=`
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
constexpr Color& operator*=(Color& left, Color right);

} // namespace sf

#include <SFML/Graphics/Color.inl>


////////////////////////////////////////////////////////////
/// \class sf::Color
/// \ingroup graphics
///
/// `sf::Color` is a simple color class composed of 4 components:
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

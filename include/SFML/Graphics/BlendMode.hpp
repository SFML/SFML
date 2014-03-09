////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_BLENDMODE_HPP
#define SFML_BLENDMODE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>


namespace sf
{

////////////////////////////////////////////////////////////
/// \ingroup graphics
/// \brief Blending modes for drawing
///
////////////////////////////////////////////////////////////
struct SFML_GRAPHICS_API BlendMode
{
    ////////////////////////////////////////////////////////
    /// \ingroup graphics
    /// \brief Enumeration of the blending factors
    ///
    ////////////////////////////////////////////////////////
    enum BlendFactor
    {
        Zero,             ///< (0,0,0,0)
        One,              ///< (1,1,1,1)
        SrcColor,         ///< (src.r,src.g,src.b,src.a)
        OneMinusSrcColor, ///< (1,1,1,1) - (src.r,src.g,src.b,src.a)
        DstColor,         ///< (dst.r,dst.g,dst.b,dst.a)
        OneMinusDstColor, ///< (1,1,1,1) - (dst.r,dst.g,dst.b,dst.a)
        SrcAlpha,         ///< (src.a,src.a,src.a,src.a)
        OneMinusSrcAlpha, ///< (1,1,1,1) - (src.a,src.a,src.a,src.a)
        DstAlpha,         ///< (dst.a,dst.a,dst.a,dst.a)
        OneMinusDstAlpha, ///< (1,1,1,1) - (dst.a,dst.a,dst.a,dst.a)
    };

    ////////////////////////////////////////////////////////
    /// \ingroup graphics
    /// \brief Enumeration of the blending equations
    ///
    ////////////////////////////////////////////////////////
    enum BlendEquation
    {
        Add,             ///< Pixel = Source * SourceFactor + Dst * DstFactor
        Subtract,        ///< Pixel = Source * SourceFactor - Dst * DstFactor
    };

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Constructs a blending mode that does alpha blending.
    ///
    ////////////////////////////////////////////////////////////
    BlendMode();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the blend mode given the factors and equation.
    ///
    /// \param colorSourceFactor  Specifies how to compute the source factor for the color channels.
    /// \param colorDstFactor    Specifies how to compute the destination factor for the color channels.
    /// \param colorBlendEquation Specifies how to combine the source and destination colors.
    /// \param alphaSourceFactor  Specifies how to compute the source factor.
    /// \param alphaDstFactor    Specifies how to compute the destination factor.
    /// \param alphaBlendEquation Specifies how to combine the source and destination alphas.
    /// 
    ///
    ////////////////////////////////////////////////////////////
    BlendMode(BlendFactor colorSourceFactor, BlendFactor colorDstFactor,
        BlendEquation colorBlendEquation, BlendFactor alphaSourceFactor,
        BlendFactor alphaDstFactor, BlendEquation alphaBlendEquation);

    ////////////////////////////////////////////////////////////
    // Member Data
    ////////////////////////////////////////////////////////////
    BlendFactor   colorSrcFactor;  ///< Source blending factor for the color channels
    BlendFactor   colorDstFactor;  ///< Destination blending factor for the color channels
    BlendEquation colorEquation;   ///< Blending equation for the color channels
    BlendFactor   alphaSrcFactor;  ///< Source blending factor for the alpha channel
    BlendFactor   alphaDstFactor;  ///< Destination blending factor for the alpha channel
    BlendEquation alphaEquation;   ///< Blending equation for the alpha channel
};

////////////////////////////////////////////////////////////
/// \relates BlendMode
/// \brief Overload of the == operator
///
/// This operator compares two blending modes and checks if they are equal.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return True if blending modes are equal, false if they are different
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API bool operator ==(const BlendMode& left, const BlendMode& right);

////////////////////////////////////////////////////////////
/// \relates BlendMode
/// \brief Overload of the != operator
///
/// This operator compares two blending modes and checks if they are different.
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return True if blending modes are different, false if they are equal
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API bool operator !=(const BlendMode& left, const BlendMode& right);


////////////////////////////////////////////////////////////
// Commonly used blending modes
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API extern const BlendMode BlendAlpha;
SFML_GRAPHICS_API extern const BlendMode BlendAdd;
SFML_GRAPHICS_API extern const BlendMode BlendMultiply;
SFML_GRAPHICS_API extern const BlendMode BlendNone;

} // namespace sf


#endif // SFML_BLENDMODE_HPP


////////////////////////////////////////////////////////////
/// \class sf::BlendMode
/// \ingroup graphics
///
/// sf::BlendMode is a class that represents a blend mode. A
/// blend mode is composed of 6 components:
/// \li %Color Source Factor (colorSrcFactor)
/// \li %Color Destination Factor (colorDstFactor)
/// \li %Color Blend Equation (colorEquation)
/// \li Alpha Source Factor (alphaSrcFactor)
/// \li Alpha Destination Factor (alphaDstFactor)
/// \li Alpha Blend Equation (alphaEquation)
///
/// Each component has its own setter function. These make
/// modifying a blending mode rather easy:
///
/// \code
/// sf::BlendMode blendMode;                           // Standard alpha blending
/// blendMode.colorSrcFactor = sf::BlendMode::One;     // Pre-multiplied alpha blending
/// blendMode.colorEquation = sf::BlendMode::Subtract; // An exotic subtraction blending mode
/// \endcode
///
/// The most common blending modes are defined as const
/// variables for convenience and compatibility with older
/// code:
///
/// \code
/// sf::BlendMode alphaBlending          = sf::BlendAlpha;
/// sf::BlendMode additiveBlending       = sf::BlendAdd;
/// sf::BlendMode multiplicativeBlending = sf::BlendMultipy;
/// sf::BlendMode noBlending             = sf::BlendNone;
/// \endcode
///
////////////////////////////////////////////////////////////

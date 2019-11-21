////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_STENCILMODE_HPP
#define SFML_STENCILMODE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>


namespace sf
{

////////////////////////////////////////////////////////////
/// \brief Stencil modes for drawing
///
////////////////////////////////////////////////////////////
struct SFML_GRAPHICS_API StencilMode
{
    ////////////////////////////////////////////////////////
    /// \brief Enumeration of the stencil test comparisons that can be performed
    ///
    /// The comparisons are mapped directly to their OpenGL equivalents,
    /// specified by glStencilFunc().
    ////////////////////////////////////////////////////////
    enum Comparison
    {
        Never,        ///< The stencil test never passes
        Less,         ///< The stencil test passes if the new value is less than the value in the stencil buffer
        LessEqual,    ///< The stencil test passes if the new value is less than or equal to the value in the stencil buffer
        Greater,      ///< The stencil test passes if the new value is greater than the value in the stencil buffer
        GreaterEqual, ///< The stencil test passes if the new value is greater than or equal to the value in the stencil buffer
        Equal,        ///< The stencil test passes if the new value is strictly equal to the value in the stencil buffer
        NotEqual,     ///< The stencil test passes if the new value is strictly inequal to the value in the stencil buffer
        Always        ///< The stencil test always passes
    };

    ////////////////////////////////////////////////////////
    /// \brief Enumeration of the stencil buffer update operations
    ///
    /// The update operations are mapped directly to their OpenGL equivalents,
    /// specified by glStencilOp().
    ////////////////////////////////////////////////////////
    enum UpdateOperation
    {
        Keep,      ///< If the stencil test passes, the value in the stencil buffer is not modified
        Zero,      ///< If the stencil test passes, the value in the stencil buffer is set to zero
        Replace,   ///< If the stencil test passes, the value in the stencil buffer is set to the new value
        Increment, ///< If the stencil test passes, the value in the stencil buffer is incremented and if required clamped
        Decrement, ///< If the stencil test passes, the value in the stencil buffer is decremented and if required clamped
        Invert,    ///< If the stencil test passes, the value in the stencil buffer is bitwise inverted
    };

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Constructs a stencil mode that disables stencil testing
    ///
    ////////////////////////////////////////////////////////////
    StencilMode();

    ////////////////////////////////////////////////////////////
    /// \brief Construct a stencil mode given the function, operation and reference value.
    ///
    /// \param stencilFunction  Specifies the kind of test to perform against the value in the stencil buffer
    /// \param stencilOperation Specifies how the stencil buffer is updated if the test passes
    /// \param stencilReference Specifies the reference value to perform the stencil test with and to write into stencil buffer if the operation is Replace and the test passes
    /// \param stencilMask      Specifies the mask value to apply to both the reference value and the value in the stencil buffer when comparing and updating
    /// \param stencilOnly      True to update only the stencil buffer, false to update both stencil and color buffers
    ///
    ////////////////////////////////////////////////////////////
    StencilMode(Comparison stencilFunction, UpdateOperation stencilOperation, unsigned int stencilReference, unsigned int stencilMask, bool stencilOnly);

    ////////////////////////////////////////////////////////////
    // Member Data
    ////////////////////////////////////////////////////////////
    Comparison      stencilComparison;      ///< The comparison we're performing the stencil test with
    UpdateOperation stencilUpdateOperation; ///< The update operation to perform if the stencil test passes
    unsigned int    stencilReference;       ///< The reference value we're performing the stencil test with
    unsigned int    stencilMask;            ///< The mask to apply to both the reference value and the value in the stencil buffer
    bool            stencilOnly;            ///< Whether we should update the color buffer in addition to the stencil buffer
};

////////////////////////////////////////////////////////////
/// \relates StencilMode
/// \brief Overload of the == operator
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return True if stencil modes are equal, false if they are different
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API bool operator ==(const StencilMode& left, const StencilMode& right);

////////////////////////////////////////////////////////////
/// \relates StencilMode
/// \brief Overload of the != operator
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return True if stencil modes are different, false if they are equal
///
////////////////////////////////////////////////////////////
SFML_GRAPHICS_API bool operator !=(const StencilMode& left, const StencilMode& right);

} // namespace sf


#endif // SFML_STENCILMODE_HPP


////////////////////////////////////////////////////////////
/// \class sf::StencilMode
/// \ingroup graphics
///
/// sf::StencilMode is a class that controls stencil testing.
///
/// In addition to drawing to the visible portion of a render target,
/// there is the possibility to "draw" to a so-called stencil buffer.
/// The stencil buffer is a special non-visible buffer that can contain
/// a single value per pixel that is drawn. This can be thought of as a
/// fifth value in addition to red, green, blue and alpha values. The maximum
/// value that can be represented depends on what is supported by the system.
/// Typically support for a 8-bit stencil buffer should always be available.
/// This will also have to be requested when creating a render target via
/// the sf::ContextSettings that is passed during creation. Stencil testing
/// will not work if there is no stencil buffer available in the target
/// that is being drawn to.
///
/// Initially, just like with the visible color buffer, the stencil value of
/// each pixel is set to an undefined value. Calling sf::RenderTarget::clear
/// will set each pixel's stencil value to 0. sf::RenderTarget::clear can be
/// called at any time to reset the stencil values back to 0.
///
/// When drawing an object, before each pixel of the color buffer is updated
/// with its new color value, the stencil test is performed. During this test
/// 2 values are compared with each other: the reference value that is passed
/// via sf::StencilMode and the value that is currently in the stencil buffer.
/// The arithmetic comparison that is performed on the 2 values can also be
/// controlled via sf::StencilMode. Depending on whether the test passes i.e.
/// the comparison yields true, the color buffer is updated with its new RGBA
/// value and if set in sf::StencilMode the stencil buffer is updated
/// accordingly. The new stencil value will be used during stencil testing the
/// next time the pixel is drawn to.
///
/// The class is composed of 4 components, each of which has its
/// own public member variable:
/// \li Stencil Comparison (@ref stencilComparison)
/// \li Stencil Update Operation (@ref stencilUpdateOperation)
/// \li Stencil Reference Value (@ref stencilReference)
/// \li Stencil Mask Value (@ref stencilMask)
/// \li Stencil Only Update (@ref stencilOnly)
///
/// The stencil comparison specifies the comparison that is performed between
/// the reference value of the currently active sf::StencilMode and the value
/// that is currently in the stencil buffer. This comparison determines whether
/// the stencil test passes or fails.
///
/// The stencil update operation specifies how the stencil buffer is updated if
/// the stencil test passes. If the stencil test fails, neither the color or
/// stencil buffers will be modified. If incrementing or decrementing the
/// stencil value the new value will be clamped to the range from 0 to the
/// maximum representable value given the bit width of the stencil buffer
/// e.g. 255 if an 8-bit stencil buffer is being used.
///
/// The reference value is used both during the comparison with the current
/// stencil buffer value and as the new value to be written when the operation
/// is set to Replace.
///
/// The mask value is used to mask the bits of both the reference value and
/// the value in the stencil buffer during the comparison and when updating.
/// The mask can be used to e.g. segment the stencil value bits into separate
/// regions that are used for different purposes.
///
/// In certain situations, it might make sense to only write to the stencil
/// buffer and not the color buffer during a draw. The written stencil buffer
/// value can then be used in subsequent draws as a masking region.
///
/// In SFML, a stencil mode can be specified every time you draw a sf::Drawable
/// object to a render target. It is part of the sf::RenderStates compound
/// that is passed to the member function sf::RenderTarget::draw().
///
/// \see sf::RenderStates, sf::RenderTarget
///
////////////////////////////////////////////////////////////

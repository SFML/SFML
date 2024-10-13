////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/Export.hpp>


namespace sf
{

////////////////////////////////////////////////////////
/// \brief Enumeration of the stencil test comparisons that can be performed
///
/// The comparisons are mapped directly to their OpenGL equivalents,
/// specified by `glStencilFunc()`.
////////////////////////////////////////////////////////
enum class StencilComparison
{
    Never,        //!< The stencil test never passes
    Less,         //!< The stencil test passes if the new value is less than the value in the stencil buffer
    LessEqual,    //!< The stencil test passes if the new value is less than or equal to the value in the stencil buffer
    Greater,      //!< The stencil test passes if the new value is greater than the value in the stencil buffer
    GreaterEqual, //!< The stencil test passes if the new value is greater than or equal to the value in the stencil buffer
    Equal,        //!< The stencil test passes if the new value is strictly equal to the value in the stencil buffer
    NotEqual,     //!< The stencil test passes if the new value is strictly unequal to the value in the stencil buffer
    Always        //!< The stencil test always passes
};

////////////////////////////////////////////////////////
/// \brief Enumeration of the stencil buffer update operations
///
/// The update operations are mapped directly to their OpenGL equivalents,
/// specified by `glStencilOp()`.
////////////////////////////////////////////////////////
enum class StencilUpdateOperation
{
    Keep,      //!< If the stencil test passes, the value in the stencil buffer is not modified
    Zero,      //!< If the stencil test passes, the value in the stencil buffer is set to zero
    Replace,   //!< If the stencil test passes, the value in the stencil buffer is set to the new value
    Increment, //!< If the stencil test passes, the value in the stencil buffer is incremented and if required clamped
    Decrement, //!< If the stencil test passes, the value in the stencil buffer is decremented and if required clamped
    Invert,    //!< If the stencil test passes, the value in the stencil buffer is bitwise inverted
};

////////////////////////////////////////////////////////
/// \brief Stencil value type (also used as a mask)
///
////////////////////////////////////////////////////////
struct SFML_GRAPHICS_API StencilValue
{
    ////////////////////////////////////////////////////////////
    /// \brief Construct a stencil value from a signed integer
    ///
    /// \param theValue Signed integer value to use
    ///
    ////////////////////////////////////////////////////////////
    StencilValue(int theValue);

    ////////////////////////////////////////////////////////////
    /// \brief Construct a stencil value from an unsigned integer
    ///
    /// \param theValue Unsigned integer value to use
    ///
    ////////////////////////////////////////////////////////////
    StencilValue(unsigned int theValue);

    ////////////////////////////////////////////////////////////
    /// \brief Disable construction from any other type
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    StencilValue(T) = delete;

    unsigned int value{}; //!< The stored stencil value
};

////////////////////////////////////////////////////////////
/// \brief Stencil modes for drawing
///
////////////////////////////////////////////////////////////
struct SFML_GRAPHICS_API StencilMode
{
    StencilComparison stencilComparison{StencilComparison::Always}; //!< The comparison we're performing the stencil test with
    StencilUpdateOperation stencilUpdateOperation{
        StencilUpdateOperation::Keep}; //!< The update operation to perform if the stencil test passes
    StencilValue stencilReference{0};  //!< The reference value we're performing the stencil test with
    StencilValue stencilMask{~0u}; //!< The mask to apply to both the reference value and the value in the stencil buffer
    bool stencilOnly{};            //!< Whether we should update the color buffer in addition to the stencil buffer
};

////////////////////////////////////////////////////////////
/// \relates StencilMode
/// \brief Overload of the `operator==`
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return `true` if stencil modes are equal, `false` if they are different
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_GRAPHICS_API bool operator==(const StencilMode& left, const StencilMode& right);

////////////////////////////////////////////////////////////
/// \relates StencilMode
/// \brief Overload of the `operator!=`
///
/// \param left  Left operand
/// \param right Right operand
///
/// \return `true` if stencil modes are different, `false` if they are equal
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_GRAPHICS_API bool operator!=(const StencilMode& left, const StencilMode& right);

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::StencilMode
/// \ingroup graphics
///
/// `sf::StencilMode` is a class that controls stencil testing.
///
/// In addition to drawing to the visible portion of a render target,
/// there is the possibility to "draw" to a so-called stencil buffer.
/// The stencil buffer is a special non-visible buffer that can contain
/// a single value per pixel that is drawn. This can be thought of as a
/// fifth value in addition to red, green, blue and alpha values. The maximum
/// value that can be represented depends on what is supported by the system.
/// Typically support for a 8-bit stencil buffer should always be available.
/// This will also have to be requested when creating a render target via
/// the `sf::ContextSettings` that is passed during creation. Stencil testing
/// will not work if there is no stencil buffer available in the target
/// that is being drawn to.
///
/// Initially, just like with the visible color buffer, the stencil value of
/// each pixel is set to an undefined value. Calling `sf::RenderTarget::clear`
/// will set each pixel's stencil value to 0. `sf::RenderTarget::clear` can be
/// called at any time to reset the stencil values back to 0.
///
/// When drawing an object, before each pixel of the color buffer is updated
/// with its new color value, the stencil test is performed. During this test
/// 2 values are compared with each other: the reference value that is passed
/// via `sf::StencilMode` and the value that is currently in the stencil buffer.
/// The arithmetic comparison that is performed on the 2 values can also be
/// controlled via `sf::StencilMode`. Depending on whether the test passes i.e.
/// the comparison yields `true`, the color buffer is updated with its new RGBA
/// value and if set in `sf::StencilMode` the stencil buffer is updated
/// accordingly. The new stencil value will be used during stencil testing the
/// next time the pixel is drawn to.
///
/// The class is composed of 5 components, each of which has its
/// own public member variable:
/// \li Stencil Comparison (\ref stencilComparison)
/// \li Stencil Update Operation (\ref stencilUpdateOperation)
/// \li Stencil Reference Value (\ref stencilReference)
/// \li Stencil Mask Value (\ref stencilMask)
/// \li Stencil Only Update (\ref stencilOnly)
///
/// The stencil comparison specifies the comparison that is performed between
/// the reference value of the currently active `sf::StencilMode` and the value
/// that is currently in the stencil buffer. This comparison determines whether
/// the stencil test passes or fails.
///
/// The stencil update operation specifies how the stencil buffer is updated if
/// the stencil test passes. If the stencil test fails, neither the color or
/// stencil buffers will be modified. If incrementing or decrementing the
/// stencil value, the new value will be clamped to the range from 0 to the
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
/// In SFML, a stencil mode can be specified every time you draw a `sf::Drawable`
/// object to a render target. It is part of the `sf::RenderStates` compound
/// that is passed to the member function `sf::RenderTarget::draw()`.
///
/// Usage example:
/// \code
/// // Make sure we create a RenderTarget with a stencil buffer by specifying it via the context settings
/// sf::RenderWindow window(sf::VideoMode({250, 200}), "Stencil Window", sf::Style::Default, sf::ContextSettings{0, 8});
///
/// ...
///
/// // Left circle
/// sf::CircleShape left(100.f);
/// left.setFillColor(sf::Color::Green);
/// left.setPosition({0, 0});
///
/// // Middle circle
/// sf::CircleShape middle(100.f);
/// middle.setFillColor(sf::Color::Yellow);
/// middle.setPosition({25, 0});
///
/// // Right circle
/// sf::CircleShape right(100.f);
/// right.setFillColor(sf::Color::Red);
/// right.setPosition({50, 0});
///
/// ...
///
/// // Clear the stencil buffer to 0 at the start of every frame
/// window.clear(sf::Color::Black, 0);
///
/// ...
///
/// // Draw the middle circle in a stencil-only pass and write the value 1
/// // to the stencil buffer for every pixel the circle would have affected
/// window.draw(middle, sf::StencilMode{sf::StencilComparison::Always, sf::StencilUpdateOperation::Replace, 1, 0xFF, true});
///
/// // Draw the left and right circles
/// // Only allow rendering to pixels whose stencil value is not
/// // equal to 1 i.e. weren't written when drawing the middle circle
/// window.draw(left, sf::StencilMode{sf::StencilComparison::NotEqual, sf::StencilUpdateOperation::Keep, 1, 0xFF, false});
/// window.draw(right, sf::StencilMode{sf::StencilComparison::NotEqual, sf::StencilUpdateOperation::Keep, 1, 0xFF, false});
/// \endcode
///
/// \see `sf::RenderStates`, `sf::RenderTarget`
///
////////////////////////////////////////////////////////////

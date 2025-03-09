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
#include <SFML/Graphics/Export.hpp>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Transform.hpp>

#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief 2D camera that defines what region is shown on screen
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API View
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor creates a default view of (0, 0, 1000, 1000)
    ///
    ////////////////////////////////////////////////////////////
    View() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Construct the view from a rectangle
    ///
    /// \param rectangle Rectangle defining the zone to display
    ///
    ////////////////////////////////////////////////////////////
    explicit View(const FloatRect& rectangle);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the view from its center and size
    ///
    /// \param center Center of the zone to display
    /// \param size   Size of zone to display
    ///
    ////////////////////////////////////////////////////////////
    View(Vector2f center, Vector2f size);

    ////////////////////////////////////////////////////////////
    /// \brief Set the center of the view
    ///
    /// \param center New center
    ///
    /// \see `setSize`, `getCenter`
    ///
    ////////////////////////////////////////////////////////////
    void setCenter(Vector2f center);

    ////////////////////////////////////////////////////////////
    /// \brief Set the size of the view
    ///
    /// \param size New size
    ///
    /// \see `setCenter`, `getCenter`
    ///
    ////////////////////////////////////////////////////////////
    void setSize(Vector2f size);

    ////////////////////////////////////////////////////////////
    /// \brief Set the orientation of the view
    ///
    /// The default rotation of a view is 0 degree.
    ///
    /// \param angle New angle
    ///
    /// \see `getRotation`
    ///
    ////////////////////////////////////////////////////////////
    void setRotation(Angle angle);

    ////////////////////////////////////////////////////////////
    /// \brief Set the target viewport
    ///
    /// The viewport is the rectangle into which the contents of the
    /// view are displayed, expressed as a factor (between 0 and 1)
    /// of the size of the RenderTarget to which the view is applied.
    /// For example, a view which takes the left side of the target would
    /// be defined with `view.setViewport(sf::FloatRect({0.f, 0.f}, {0.5f, 1.f}))`.
    /// By default, a view has a viewport which covers the entire target.
    ///
    /// \param viewport New viewport rectangle
    ///
    /// \see `getViewport`
    ///
    ////////////////////////////////////////////////////////////
    void setViewport(const FloatRect& viewport);

    ////////////////////////////////////////////////////////////
    /// \brief Set the target scissor rectangle
    ///
    /// The scissor rectangle, expressed as a factor (between 0 and 1) of
    /// the RenderTarget, specifies the region of the RenderTarget whose
    /// pixels are able to be modified by draw or clear operations.
    /// Any pixels which lie outside of the scissor rectangle will
    /// not be modified by draw or clear operations.
    /// For example, a scissor rectangle which only allows modifications
    /// to the right side of the target would be defined
    /// with `view.setScissor(sf::FloatRect({0.5f, 0.f}, {0.5f, 1.f}))`.
    /// By default, a view has a scissor rectangle which allows
    /// modifications to the entire target. This is equivalent to
    /// disabling the scissor test entirely. Passing the default
    /// scissor rectangle to this function will also disable
    /// scissor testing.
    ///
    /// \param scissor New scissor rectangle
    ///
    /// \see `getScissor`
    ///
    ////////////////////////////////////////////////////////////
    void setScissor(const FloatRect& scissor);

    ////////////////////////////////////////////////////////////
    /// \brief Get the center of the view
    ///
    /// \return Center of the view
    ///
    /// \see `getSize`, `setCenter`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Vector2f getCenter() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the size of the view
    ///
    /// \return Size of the view
    ///
    /// \see `getCenter`, `setSize`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Vector2f getSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current orientation of the view
    ///
    /// \return Rotation angle of the view
    ///
    /// \see `setRotation`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Angle getRotation() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the target viewport rectangle of the view
    ///
    /// \return Viewport rectangle, expressed as a factor of the target size
    ///
    /// \see `setViewport`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] const FloatRect& getViewport() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the scissor rectangle of the view
    ///
    /// \return Scissor rectangle, expressed as a factor of the target size
    ///
    /// \see `setScissor`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] const FloatRect& getScissor() const;

    ////////////////////////////////////////////////////////////
    /// \brief Move the view relative to its current position
    ///
    /// \param offset Move offset
    ///
    /// \see `setCenter`, `rotate`, `zoom`
    ///
    ////////////////////////////////////////////////////////////
    void move(Vector2f offset);

    ////////////////////////////////////////////////////////////
    /// \brief Rotate the view relative to its current orientation
    ///
    /// \param angle Angle to rotate
    ///
    /// \see `setRotation`, `move`, `zoom`
    ///
    ////////////////////////////////////////////////////////////
    void rotate(Angle angle);

    ////////////////////////////////////////////////////////////
    /// \brief Resize the view rectangle relative to its current size
    ///
    /// Resizing the view simulates a zoom, as the zone displayed on
    /// screen grows or shrinks.
    /// \a factor is a multiplier:
    /// \li 1 keeps the size unchanged
    /// \li > 1 makes the view bigger (objects appear smaller)
    /// \li < 1 makes the view smaller (objects appear bigger)
    ///
    /// \param factor Zoom factor to apply
    ///
    /// \see `setSize`, `move`, `rotate`
    ///
    ////////////////////////////////////////////////////////////
    void zoom(float factor);

    ////////////////////////////////////////////////////////////
    /// \brief Get the projection transform of the view
    ///
    /// This function is meant for internal use only.
    ///
    /// \return Projection transform defining the view
    ///
    /// \see `getInverseTransform`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] const Transform& getTransform() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the inverse projection transform of the view
    ///
    /// This function is meant for internal use only.
    ///
    /// \return Inverse of the projection transform defining the view
    ///
    /// \see `getTransform`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] const Transform& getInverseTransform() const;

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Vector2f  m_center{500, 500};           //!< Center of the view, in scene coordinates
    Vector2f  m_size{1000, 1000};           //!< Size of the view, in scene coordinates
    Angle     m_rotation;                   //!< Angle of rotation of the view rectangle
    FloatRect m_viewport{{0, 0}, {1, 1}};   //!< Viewport rectangle, expressed as a factor of the render-target's size
    FloatRect m_scissor{{0, 0}, {1, 1}};    //!< Scissor rectangle, expressed as a factor of the render-target's size
    mutable Transform m_transform;          //!< Precomputed projection transform corresponding to the view
    mutable Transform m_inverseTransform;   //!< Precomputed inverse projection transform corresponding to the view
    mutable bool      m_transformUpdated{}; //!< Internal state telling if the transform needs to be updated
    mutable bool      m_invTransformUpdated{}; //!< Internal state telling if the inverse transform needs to be updated
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::View
/// \ingroup graphics
///
/// `sf::View` defines a camera in the 2D scene. This is a
/// very powerful concept: you can scroll, rotate or zoom
/// the entire scene without altering the way that your
/// drawable objects are drawn.
///
/// A view is composed of a source rectangle, which defines
/// what part of the 2D scene is shown, and a target viewport,
/// which defines where the contents of the source rectangle
/// will be displayed on the render target (window or texture).
///
/// The viewport allows to map the scene to a custom part
/// of the render target, and can be used for split-screen
/// or for displaying a minimap, for example. If the source
/// rectangle doesn't have the same size as the viewport, its
/// contents will be stretched to fit in.
///
/// The scissor rectangle allows for specifying regions of the
/// render target to which modifications can be made by draw
/// and clear operations. Only pixels that are within the region
/// will be able to be modified. Pixels outside of the region will
/// not be modified by draw or clear operations.
///
/// Certain effects can be created by either using the viewport or
/// scissor rectangle. While the results appear identical, there
/// can be times where one method should be preferred over the other.
/// Viewport transformations are applied during the vertex processing
/// stage of the graphics pipeline, before the primitives are
/// rasterized into fragments for fragment processing. Since
/// viewport processing has to be performed and cannot be disabled,
/// effects that are performed using the viewport transform are
/// basically free performance-wise. Scissor testing is performed in
/// the per-sample processing stage of the graphics pipeline, after
/// fragment processing has been performed. Because per-sample
/// processing is performed at the last stage of the pipeline,
/// fragments that are discarded at this stage will cause the
/// highest waste of GPU resources compared to any method that
/// would have discarded vertices or fragments earlier in the
/// pipeline. There are situations in which scissor testing has
/// to be used to control whether fragments are discarded or not.
/// An example of such a situation is when performing the viewport
/// transform on vertices is necessary but a subset of the generated
/// fragments should not have an effect on the stencil buffer or
/// blend with the color buffer.
//
/// To apply a view, you have to assign it to the render target.
/// Then, objects drawn in this render target will be
/// affected by the view until you use another view.
///
/// Usage example:
/// \code
/// sf::RenderWindow window;
///
/// // Initialize the view to a rectangle located at (100, 100) and with a size of 400x200
/// sf::View view(sf::FloatRect({100, 100}, {400, 200}));
///
/// // Rotate it by 45 degrees
/// view.rotate(sf::degrees(45));
///
/// // Set its target viewport to be half of the window
/// view.setViewport(sf::FloatRect({0.f, 0.f}, {0.5f, 1.f}));
///
/// // Apply it
/// window.setView(view);
///
/// // Render stuff
/// window.draw(someSprite);
///
/// // Set the default view back
/// window.setView(window.getDefaultView());
///
/// // Render stuff not affected by the view
/// window.draw(someText);
/// \endcode
///
/// See also the note on coordinates and undistorted rendering in `sf::Transformable`.
///
/// \see `sf::RenderWindow`, `sf::RenderTexture`
///
////////////////////////////////////////////////////////////

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

#ifndef SFML_RENDERTARGET_HPP
#define SFML_RENDERTARGET_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Renderer.hpp>
#include <SFML/Graphics/View.hpp>


namespace sf
{
class Drawable;
class Shader;

////////////////////////////////////////////////////////////
/// \brief Base class for all render targets (window, image, ...)
///
////////////////////////////////////////////////////////////
class SFML_API RenderTarget : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~RenderTarget();

    ////////////////////////////////////////////////////////////
    /// \brief Clear the entire target with a single color
    ///
    /// This function is usually called once every frame,
    /// to clear the previous contents of the target.
    ///
    /// \param color Fill color to use to clear the render target
    ///
    ////////////////////////////////////////////////////////////
    void Clear(const Color& color = Color(0, 0, 0, 255));

    ////////////////////////////////////////////////////////////
    /// \brief Draw an object into the target
    ///
    /// This function draws anything that inherits from the
    /// sf::Drawable base class (sf::Sprite, sf::Shape, sf::Text,
    /// or even your own derived classes).
    ///
    /// \param object Object to draw
    ///
    ////////////////////////////////////////////////////////////
    void Draw(const Drawable& object);

    ////////////////////////////////////////////////////////////
    /// \brief Draw an object into the target with a shader
    ///
    /// This function draws anything that inherits from the
    /// sf::Drawable base class (sf::Sprite, sf::Shape, sf::Text,
    /// or even your own derived classes).
    /// The shader alters the way that the pixels are processed
    /// right before being written to the render target.
    ///
    /// \param object Object to draw
    /// \param shader Shader to use for drawing the object
    ///
    ////////////////////////////////////////////////////////////
    void Draw(const Drawable& object, const Shader& shader);

    ////////////////////////////////////////////////////////////
    /// \brief Return the width of the rendering region of the target
    ///
    /// \return Width in pixels
    ///
    /// \see GetHeight
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int GetWidth() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Return the height of the rendering region of the target
    ///
    /// \return Height in pixels
    ///
    /// \see GetWidth
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int GetHeight() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Change the current active view
    ///
    /// The new view will affect everything that is drawn, until
    /// another view is activated.
    /// The render target keeps its own copy of the view object,
    /// so it is not necessary to keep the original one alive
    /// as long as it is in use.
    /// To restore the original view of the target, you can pass
    /// the result of GetDefaultView() to this function.
    ///
    /// \param view New view to use
    ///
    /// \see GetView, GetDefaultView
    ///
    ////////////////////////////////////////////////////////////
    void SetView(const View& view);

    ////////////////////////////////////////////////////////////
    /// \brief Retrieve the view currently in use in the render target
    ///
    /// \return The view object that is currently used
    ///
    /// \see SetView, GetDefaultView
    ///
    ////////////////////////////////////////////////////////////
    const View& GetView() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the default view of the render target
    ///
    /// The default view has the initial size of the render target,
    /// and never changes after the target has been created.
    ///
    /// \return The default view of the render target
    ///
    /// \see SetView, GetView
    ///
    ////////////////////////////////////////////////////////////
    const View& GetDefaultView() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the viewport of a view, applied to this render target
    ///
    /// The viewport is defined in the view as a ratio, this function
    /// simply applies this ratio to the current dimensions of the
    /// render target to calculate the pixels rectangle that the viewport
    /// actually covers in the target.
    ///
    /// \param view The view for which we want to compute the viewport
    ///
    /// \return Viewport rectangle, expressed in pixels
    ///
    ////////////////////////////////////////////////////////////
    IntRect GetViewport(const View& view) const;

    ////////////////////////////////////////////////////////////
    /// \brief Convert a point from target coordinates to view coordinates
    ///
    /// Initially, a unit of the 2D world matches a pixel of the
    /// render target. But if you define a custom view, this
    /// assertion is not true anymore, ie. a point located at
    /// (10, 50) in your render target (for example a window) may
    /// map to the point (150, 75) in your 2D world -- for example
    /// if the view is translated by (140, 25).
    ///
    /// For render windows, this function is typically used to find
    /// which point (or object) is located below the mouse cursor.
    ///
    /// This version uses the current view of the render target.
    /// See the other overload to specify a custom view.
    ///
    /// \param x X coordinate of the point to convert, relative to the render target
    /// \param y Y coordinate of the point to convert, relative to the render target
    ///
    /// \return The converted point, in "world" units
    ///
    ////////////////////////////////////////////////////////////
    Vector2f ConvertCoords(unsigned int x, unsigned int y) const;

    ////////////////////////////////////////////////////////////
    /// \brief Convert a point from target coordinates to view coordinates
    ///
    /// Initially, a unit of the 2D world matches a pixel of the
    /// render target. But if you define a custom view, this
    /// assertion is not true anymore, ie. a point located at
    /// (10, 50) in your render target (for example a window) may
    /// map to the point (150, 75) in your 2D world -- for example
    /// if the view is translated by (140, 25).
    ///
    /// For render windows, this function is typically used to find
    /// which point (or object) is located below the mouse cursor.
    ///
    /// This version uses a custom view for calculations, see the other
    /// overload of the function to use the current view of the render
    /// target.
    ///
    /// \param x    X coordinate of the point to convert, relative to the render target
    /// \param y    Y coordinate of the point to convert, relative to the render target
    /// \param view The view to use for converting the point
    ///
    /// \return The converted point, in "world" units
    ///
    ////////////////////////////////////////////////////////////
    Vector2f ConvertCoords(unsigned int x, unsigned int y, const View& view) const;

    ////////////////////////////////////////////////////////////
    /// \brief Save the current OpenGL render states and matrices
    ///
    /// This function can be used when you mix SFML drawing
    /// and direct OpenGL rendering. Combined with RestoreGLStates,
    /// it ensures that:
    /// \li SFML's internal states are not messed up by your OpenGL code
    /// \li your OpenGL states are not modified by a call to a SFML function
    ///
    /// More specifically, it must be used around code that
    /// calls Draw functions. Example:
    /// \code
    /// // OpenGL code here...
    /// window.SaveGLStates();
    /// window.Draw(...);
    /// window.Draw(...);
    /// window.RestoreGLStates();
    /// // OpenGL code here...
    /// \endcode
    ///
    /// Note that this function is quite expensive and should be
    /// used wisely. It is provided for convenience, and the best
    /// results will be achieved if you handle OpenGL states
    /// yourself (because you really know which states have really
    /// changed, and need to be saved / restored).
    ///
    /// \see RestoreGLStates
    ///
    ////////////////////////////////////////////////////////////
    void SaveGLStates();

    ////////////////////////////////////////////////////////////
    /// \brief Restore the previously saved OpenGL render states and matrices
    ///
    /// See the description of SaveGLStates to get a detailed
    /// description of these functions.
    ///
    /// \see SaveGLStates
    ///
    ////////////////////////////////////////////////////////////
    void RestoreGLStates();

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    RenderTarget();

    ////////////////////////////////////////////////////////////
    /// \brief Performs the common initialization step after creation
    ///
    /// The derived classes must call this function after the
    /// target is created and ready for drawing.
    ///
    ////////////////////////////////////////////////////////////
    void Initialize();

private :

    ////////////////////////////////////////////////////////////
    /// \brief Activate the target for rendering
    ///
    /// This function must be implemented by derived classes to make
    /// their OpenGL context current; it is called by the base class
    /// everytime it's going to use OpenGL calls.
    ///
    /// \param active True to make the target active, false to deactivate it
    ///
    /// \return True if the function succeeded
    ///
    ////////////////////////////////////////////////////////////
    virtual bool Activate(bool active) = 0;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Renderer myRenderer;       ///< Renderer that will process the rendering commands of the window
    View     myDefaultView;    ///< Default view
    View     myCurrentView;    ///< Current active view
    bool     myStatesSaved;    ///< Are we between a SaveGLStates and a RestoreGLStates?
    bool     myViewHasChanged; ///< Has the current view changed?
};

} // namespace sf


#endif // SFML_RENDERTARGET_HPP


////////////////////////////////////////////////////////////
/// \class sf::RenderTarget
/// \ingroup graphics
///
/// sf::RenderTarget defines the common behaviour of all the
/// 2D render targets usable in the graphics module. It makes
/// it possible to draw 2D entities like sprites, shapes, text
/// without using any OpenGL command directly.
///
/// A sf::RenderTarget is also able to use views (sf::View),
/// which are a kind of 2D cameras. With views you can globally
/// scroll, rotate or zoom everything that is drawn,
/// without having to transform every single entity. See the
/// documentation of sf::View for more details and sample pieces of
/// code about this class.
///
/// On top of that, render targets are still able to render direct
/// OpenGL stuff. It is even possible to mix together OpenGL calls
/// and regular SFML drawing commands. When doing so, make sure that
/// OpenGL states are not messed up by calling the SaveGLStates /
/// RestoreGLStates functions.
///
/// \see sf::RenderWindow, sf::RenderImage, sf::View
///
////////////////////////////////////////////////////////////

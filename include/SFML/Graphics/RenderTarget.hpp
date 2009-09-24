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
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderQueue.hpp>
#include <SFML/Graphics/View.hpp>


namespace sf
{
class Drawable;

////////////////////////////////////////////////////////////
/// Base class for all render targets (window, image, ...)
////////////////////////////////////////////////////////////
class SFML_API RenderTarget
{
public :

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~RenderTarget();

    ////////////////////////////////////////////////////////////
    /// Clear the entire target with a single color
    ///
    /// \param color : Color to use to clear the render target
    ///
    ////////////////////////////////////////////////////////////
    void Clear(const Color& color = Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// Draw something into the target
    ///
    /// \param object : Object to draw
    ///
    ////////////////////////////////////////////////////////////
    virtual void Draw(const Drawable& object);

    ////////////////////////////////////////////////////////////
    /// Make sure that what has been drawn so far is rendered
    ///
    /// Use this function if you use OpenGL rendering commands,
    /// and you want to make sure that things will appear on top
    /// of all the SFML objects that have been drawn so far.
    /// This is needed because SFML doesn't use immediate rendering,
    /// it first accumulates drawables into a queue and
    /// trigger the actual rendering afterwards.
    ///
    /// You don't need to call this function if you're not
    /// dealing with OpenGL directly.
    ///
    ////////////////////////////////////////////////////////////
    void Flush();

    ////////////////////////////////////////////////////////////
    /// Get the width of the rendering region of the target
    ///
    /// \return Width in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int GetWidth() const = 0;

    ////////////////////////////////////////////////////////////
    /// Get the height of the rendering region of the target
    ///
    /// \return Height in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int GetHeight() const = 0;

    ////////////////////////////////////////////////////////////
    /// Change the current active view.
    ///
    /// \param view : New view to use (pass GetDefaultView() to set the default view)
    ///
    ////////////////////////////////////////////////////////////
    void SetView(const View& view);

    ////////////////////////////////////////////////////////////
    /// Get the current view
    ///
    /// \return Current view active in the window
    ///
    ////////////////////////////////////////////////////////////
    const View& GetView() const;

    ////////////////////////////////////////////////////////////
    /// Get the default view of the window for read / write
    ///
    /// \return Default view
    ///
    ////////////////////////////////////////////////////////////
    View& GetDefaultView();

    ////////////////////////////////////////////////////////////
    /// Get the viewport of a view applied to this target
    ///
    /// \param view Target view
    ///
    /// \return Viewport rectangle, expressed in pixels in the current target
    ///
    ////////////////////////////////////////////////////////////
    IntRect GetViewport(const View& view) const;

    ////////////////////////////////////////////////////////////
    /// Convert a point in target coordinates into view coordinates
    /// This version uses the current view of the window
    ///
    /// \param x : X coordinate of the point to convert, relative to the target
    /// \param y : Y coordinate of the point to convert, relative to the target
    ///
    /// \return Converted point
    ///
    ////////////////////////////////////////////////////////////
    sf::Vector2f ConvertCoords(unsigned int x, unsigned int y) const;

    ////////////////////////////////////////////////////////////
    /// Convert a point in target coordinates into view coordinates
    /// This version uses the given view
    ///
    /// \param x :    X coordinate of the point to convert, relative to the target
    /// \param y :    Y coordinate of the point to convert, relative to the target
    /// \param view : Target view to convert the point to
    ///
    /// \return Converted point
    ///
    ////////////////////////////////////////////////////////////
    sf::Vector2f ConvertCoords(unsigned int x, unsigned int y, const View& view) const;

protected :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    RenderTarget();

    ////////////////////////////////////////////////////////////
    /// Called by the derived class when it's ready to be initialized
    ///
    ////////////////////////////////////////////////////////////
    void Initialize();

private :

    ////////////////////////////////////////////////////////////
    /// Activate the target for rendering
    ///
    /// \param active : True to activate rendering, false to deactivate
    ///
    /// \return True if activation succeeded
    ///
    ////////////////////////////////////////////////////////////
    virtual bool Activate(bool active) = 0;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    RenderQueue myRenderQueue; ///< Rendering queue storing render commands
    View        myDefaultView; ///< Default view
    const View* myCurrentView; ///< Current active view
};

} // namespace sf


#endif // SFML_RENDERTARGET_HPP

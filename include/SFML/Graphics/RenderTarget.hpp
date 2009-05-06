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
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Rect.hpp>


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
    /// \param FillColor : Color to use to clear the render target
    ///
    ////////////////////////////////////////////////////////////
    void Clear(const Color& FillColor = Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// Draw something into the target
    ///
    /// \param Object : Object to draw
    ///
    ////////////////////////////////////////////////////////////
    virtual void Draw(const Drawable& Object);

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
    /// \param NewView : New view to use (pass GetDefaultView() to set the default view)
    ///
    ////////////////////////////////////////////////////////////
    void SetView(const View& NewView);

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
    /// Tell SFML to preserve external OpenGL states, at the expense of
    /// more CPU charge. Use this function if you don't want SFML
    /// to mess up your own OpenGL states (if any).
    /// Don't enable state preservation if not needed, as it will allow
    /// SFML to do internal optimizations and improve performances.
    /// This parameter is false by default
    ///
    /// \param Preserve : True to preserve OpenGL states, false to let SFML optimize
    ///
    ////////////////////////////////////////////////////////////
    void PreserveOpenGLStates(bool Preserve);

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
    /// \param Active : True to activate rendering, false to deactivate
    ///
    /// \return True if activation succeeded
    ///
    ////////////////////////////////////////////////////////////
    virtual bool Activate(bool Active) = 0;

    ////////////////////////////////////////////////////////////
    /// Set the OpenGL render states needed for the SFML rendering
    ///
    ////////////////////////////////////////////////////////////
    void SetRenderStates();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    View        myDefaultView;    ///< Default view
    const View* myCurrentView;    ///< Current active view
    bool        myPreserveStates; ///< Should we preserve external OpenGL states ?
    bool        myIsDrawing;      ///< True when Draw is called from inside, to allow some renderstates optimizations
};

} // namespace sf


#endif // SFML_RENDERTARGET_HPP

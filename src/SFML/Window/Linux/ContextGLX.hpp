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

#ifndef SFML_CONTEXTGLX_HPP
#define SFML_CONTEXTGLX_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/ContextGL.hpp>
#include <X11/Xlib.h>
#include <GL/glx.h>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Linux (GLX) implementation of OpenGL contexts
////////////////////////////////////////////////////////////
class ContextGLX : public ContextGL
{
public :

    ////////////////////////////////////////////////////////////
    /// Create a new context, not associated to a window
    ///
    /// \param shared : Context to share the new one with (can be NULL)
    ///
    ////////////////////////////////////////////////////////////
    ContextGLX(ContextGLX* shared);

    ////////////////////////////////////////////////////////////
    /// Create a new context attached to a window
    ///
    /// \param shared :       Context to share the new one with (can be NULL)
    /// \param owner :        Pointer to the owner window
    /// \param bitsPerPixel : Pixel depth (in bits per pixel)
    /// \param settings :     Creation parameters
    ///
    ////////////////////////////////////////////////////////////
    ContextGLX(ContextGLX* shared, const WindowImpl* owner, unsigned int bitsPerPixel, const ContextSettings& settings);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~ContextGLX();

    ////////////////////////////////////////////////////////////
    /// \see Context::MakeCurrent
    ///
    ////////////////////////////////////////////////////////////
    virtual bool MakeCurrent(bool active);

    ////////////////////////////////////////////////////////////
    /// \see Context::Display
    ///
    ////////////////////////////////////////////////////////////
    virtual void Display();

    ////////////////////////////////////////////////////////////
    /// \see Context::UseVerticalSync
    ///
    ////////////////////////////////////////////////////////////
    virtual void UseVerticalSync(bool enabled);

    ////////////////////////////////////////////////////////////
    /// Check if a context is active on the current thread
    ///
    /// \return True if there's an active context, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool IsContextActive();

private :

    ////////////////////////////////////////////////////////////
    /// Create the context
    ///
    /// \param shared :       Context to share the new one with (can be NULL)
    /// \param bitsPerPixel : Pixel depth, in bits per pixel
    /// \param settings :     Creation parameters
    ///
    ////////////////////////////////////////////////////////////
    void CreateContext(ContextGLX* shared, unsigned int bitsPerPixel, const ContextSettings& settings);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    ::Display* myDisplay;    ///< Connection to the X server
    ::Window   myWindow;     ///< Window to which the context is attached
    GLXContext myContext;    ///< OpenGL context
    bool       myOwnsWindow; ///< Do we own the window associated to the context?
};

} // namespace priv

} // namespace sf

#endif // SFML_CONTEXTGLX_HPP

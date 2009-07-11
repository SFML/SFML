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

#ifndef SFML_CONTEXTWGL_HPP
#define SFML_CONTEXTWGL_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/ContextGL.hpp>
#include <windows.h>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Windows (WGL) implementation of OpenGL contexts
////////////////////////////////////////////////////////////
class ContextWGL : public ContextGL
{
public :

    ////////////////////////////////////////////////////////////
    /// Create a new context, not associated to a window
    ///
    /// \param shared : Context to share the new one with (can be NULL)
    ///
    ////////////////////////////////////////////////////////////
    ContextWGL(ContextWGL* shared);

    ////////////////////////////////////////////////////////////
    /// Create a new context attached to a window
    ///
    /// \param shared :       Context to share the new one with (can be NULL)
    /// \param owner :        Pointer to the owner window
    /// \param bitsPerPixel : Pixel depth (in bits per pixel)
    /// \param settings :     Creation parameters
    ///
    ////////////////////////////////////////////////////////////
    ContextWGL(ContextWGL* shared, const WindowImpl* owner, unsigned int bitsPerPixel, const ContextSettings& settings);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~ContextWGL();

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
    void CreateContext(ContextWGL* shared, unsigned int bitsPerPixel, const ContextSettings& settings);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    HWND  myWindow;     ///< Window to which the context is attached
    HDC   myDC;         ///< Device context of the window
    HGLRC myContext;    ///< OpenGL context
    bool  myOwnsWindow; ///< Did we create the host window?
};

} // namespace priv

} // namespace sf

#endif // SFML_CONTEXTWGL_HPP

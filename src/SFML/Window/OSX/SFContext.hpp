////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Marco Antognini (antognini.marco@gmail.com),
//                         Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_SFCONTEXT_HPP
#define SFML_SFCONTEXT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/GlContext.hpp>

////////////////////////////////////////////////////////////
/// Predefine OBJC classes
////////////////////////////////////////////////////////////
#ifdef __OBJC__

@class NSOpenGLContext;
using NSOpenGLContextRef = NSOpenGLContext*;

@class NSOpenGLView;
using NSOpenGLViewRef = NSOpenGLView*;

@class NSWindow;
using NSWindowRef = NSWindow*;

#else // If C++

using NSOpenGLContextRef = void*;
using NSOpenGLViewRef = void*;
using NSWindowRef = void*;

#endif


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief OSX (Cocoa) implementation of OpenGL contexts
///
////////////////////////////////////////////////////////////
class SFContext : public GlContext
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Create a new context, not associated to a window
    ///
    /// \param shared Context to share the new one with (can be a null pointer)
    ///
    ////////////////////////////////////////////////////////////
    SFContext(SFContext* shared);

    ////////////////////////////////////////////////////////////
    /// \brief Create a new context attached to a window
    ///
    /// \param shared       Context to share the new one with
    /// \param settings     Creation parameters
    /// \param owner        Pointer to the owner window
    /// \param bitsPerPixel Pixel depth, in bits per pixel
    ///
    ////////////////////////////////////////////////////////////
    SFContext(SFContext* shared, const ContextSettings& settings,
              const WindowImpl& owner, unsigned int bitsPerPixel);

    ////////////////////////////////////////////////////////////
    /// \brief Create a new context that embeds its own rendering target
    ///
    /// \param shared   Context to share the new one with
    /// \param settings Creation parameters
    /// \param width    Back buffer width, in pixels
    /// \param height   Back buffer height, in pixels
    ///
    ////////////////////////////////////////////////////////////
    SFContext(SFContext* shared, const ContextSettings& settings,
              unsigned int width, unsigned int height);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~SFContext();

    ////////////////////////////////////////////////////////////
    /// \brief Get the address of an OpenGL function
    ///
    /// \param name Name of the function to get the address of
    ///
    /// \return Address of the OpenGL function, 0 on failure
    ///
    ////////////////////////////////////////////////////////////
    static GlFunctionPointer getFunction(const char* name);

    ////////////////////////////////////////////////////////////
    /// \brief Display what has been rendered to the context so far
    ///
    ////////////////////////////////////////////////////////////
    void display() override;

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable vertical synchronization
    ///
    /// Activating vertical synchronization will limit the number
    /// of frames displayed to the refresh rate of the monitor.
    /// This can avoid some visual artifacts, and limit the framerate
    /// to a good value (but not constant across different computers).
    ///
    /// \param enabled True to enable v-sync, false to deactivate
    ///
    ////////////////////////////////////////////////////////////
    void setVerticalSyncEnabled(bool enabled) override;

protected:
    ////////////////////////////////////////////////////////////
    /// \brief Activate the context as the current target
    ///        for rendering
    ///
    /// \param current Whether to make the context current or no longer current
    ///
    /// \return True on success, false if any error happened
    ///
    ////////////////////////////////////////////////////////////
    bool makeCurrent(bool current) override;

private:
    ////////////////////////////////////////////////////////////
    /// \brief Create the context
    /// \note Must only be called from Ctor.
    ///
    /// \param shared       Context to share the new one with (can be a null pointer)
    /// \param bitsPerPixel bpp
    /// \param settings     Creation parameters
    ///
    ////////////////////////////////////////////////////////////
    void createContext(SFContext* shared,
                       unsigned int bitsPerPixel,
                       const ContextSettings& settings);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    NSOpenGLContextRef    m_context;       ///< OpenGL context.
    NSOpenGLViewRef       m_view;          ///< Only for offscreen context.
    NSWindowRef           m_window;        ///< Only for offscreen context.
};

} // namespace priv

} // namespace sf

#endif // SFML_SFCONTEXT_HPP

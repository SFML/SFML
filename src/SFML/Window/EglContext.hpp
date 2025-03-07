////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/EGLCheck.hpp>
#include <SFML/Window/GlContext.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp> // Prevent conflict with macro None from Xlib

#include <glad/egl.h>
#if defined(SFML_SYSTEM_LINUX) && !defined(SFML_USE_DRM)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

namespace sf::priv
{
class EglContext : public GlContext
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Create a new context, not associated to a window
    ///
    /// \param shared Context to share the new one with (can be a null pointer)
    ///
    ////////////////////////////////////////////////////////////
    EglContext(EglContext* shared);

    ////////////////////////////////////////////////////////////
    /// \brief Create a new context attached to a window
    ///
    /// \param shared       Context to share the new one with
    /// \param settings     Creation parameters
    /// \param owner        Pointer to the owner window
    /// \param bitsPerPixel Pixel depth, in bits per pixel
    ///
    ////////////////////////////////////////////////////////////
    EglContext(EglContext* shared, const ContextSettings& settings, const WindowImpl& owner, unsigned int bitsPerPixel);

    ////////////////////////////////////////////////////////////
    /// \brief Create a new context that embeds its own rendering target
    /// \warning This constructor is currently not implemented; use a different overload.
    ///
    /// \param shared   Context to share the new one with
    /// \param settings Creation parameters
    /// \param size     Back buffer width and height, in pixels
    ///
    ////////////////////////////////////////////////////////////
    EglContext(EglContext* shared, const ContextSettings& settings, Vector2u size);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~EglContext() override;

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
    /// \brief Activate the context as the current target
    ///        for rendering
    ///
    /// \param current Whether to make the context current or no longer current
    ///
    /// \return `true` on success, `false` if any error happened
    ///
    ////////////////////////////////////////////////////////////
    bool makeCurrent(bool current) override;

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
    /// \param enabled: `true` to enable v-sync, `false` to deactivate
    ///
    ////////////////////////////////////////////////////////////
    void setVerticalSyncEnabled(bool enabled) override;

    ////////////////////////////////////////////////////////////
    /// \brief Create the context
    ///
    /// \param shared       Context to share the new one with (can be a null pointer)
    /// \param bitsPerPixel Pixel depth, in bits per pixel
    /// \param settings     Creation parameters
    ///
    ////////////////////////////////////////////////////////////
    void createContext(EglContext* shared);

    ////////////////////////////////////////////////////////////
    /// \brief Create the EGL surface
    ///
    /// This function must be called when the activity (re)start, or
    /// when the orientation change.
    ///
    /// \param window: The native window type
    ///
    ////////////////////////////////////////////////////////////
    void createSurface(EGLNativeWindowType window);

    ////////////////////////////////////////////////////////////
    /// \brief Destroy the EGL surface
    ///
    /// This function must be called when the activity is stopped, or
    /// when the orientation change.
    ///
    ////////////////////////////////////////////////////////////
    void destroySurface();

    ////////////////////////////////////////////////////////////
    /// \brief Get the best EGL visual for a given set of video settings
    ///
    /// \param display      EGL display
    /// \param bitsPerPixel Pixel depth, in bits per pixel
    /// \param settings     Requested context settings
    ///
    /// \return The best EGL config
    ///
    ////////////////////////////////////////////////////////////
    static EGLConfig getBestConfig(EGLDisplay display, unsigned int bitsPerPixel, const ContextSettings& settings);

#if defined(SFML_SYSTEM_LINUX) && !defined(SFML_USE_DRM)
    ////////////////////////////////////////////////////////////
    /// \brief Select the best EGL visual for a given set of settings
    ///
    /// \param display      X display
    /// \param bitsPerPixel Pixel depth, in bits per pixel
    /// \param settings     Requested context settings
    ///
    /// \return The best visual
    ///
    ////////////////////////////////////////////////////////////
    static XVisualInfo selectBestVisual(::Display* display, unsigned int bitsPerPixel, const ContextSettings& settings);
#endif

private:
    ////////////////////////////////////////////////////////////
    /// \brief Helper to copy the picked EGL configuration
    ////////////////////////////////////////////////////////////
    void updateSettings();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    EGLDisplay m_display{EGL_NO_DISPLAY}; //!< The internal EGL display
    EGLContext m_context{EGL_NO_CONTEXT}; //!< The internal EGL context
    EGLSurface m_surface{EGL_NO_SURFACE}; //!< The internal EGL surface
    EGLConfig  m_config{};                //!< The internal EGL config
};

} // namespace sf::priv

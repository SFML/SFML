////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2024-2025 Andrew Mickelson
//               2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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

#include <glad/egl.h>

#include <gbm.h>
#include <xf86drmMode.h>


namespace sf::priv
{
struct Drm
{
    int fileDescriptor{};

    drmModeModeInfoPtr mode{};
    std::uint32_t      crtcId{};
    std::uint32_t      connectorId{};

    drmModeCrtcPtr originalCrtc{};

    drmModeConnectorPtr savedConnector{};
    drmModeEncoderPtr   savedEncoder{};
};

class WindowImplDRM;

class DRMContext : public GlContext
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Create a new context, not associated to a window
    ///
    /// \param shared Context to share the new one with (can be `nullptr`)
    ///
    ////////////////////////////////////////////////////////////
    DRMContext(DRMContext* shared);

    ////////////////////////////////////////////////////////////
    /// \brief Create a new context attached to a window
    ///
    /// \param shared       Context to share the new one with
    /// \param settings     Creation parameters
    /// \param owner        Pointer to the owner window
    /// \param bitsPerPixel Pixel depth, in bits per pixel
    ///
    ////////////////////////////////////////////////////////////
    DRMContext(DRMContext* shared, const ContextSettings& settings, const WindowImpl& owner, unsigned int bitsPerPixel);

    ////////////////////////////////////////////////////////////
    /// \brief Create a new context that embeds its own rendering target
    ///
    /// \param shared   Context to share the new one with
    /// \param settings Creation parameters
    /// \param size     Back buffer width and height, in pixels
    ///
    ////////////////////////////////////////////////////////////
    DRMContext(DRMContext* shared, const ContextSettings& settings, Vector2u size);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~DRMContext() override;

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
    /// \brief Create the EGL context
    ///
    /// \param shared       Context to share the new one with (can be `nullptr`)
    /// \param bitsPerPixel Pixel depth, in bits per pixel
    /// \param settings     Creation parameters
    ///
    ////////////////////////////////////////////////////////////
    void createContext(DRMContext* shared);

    ////////////////////////////////////////////////////////////
    /// \brief Create the EGL surface
    ///
    /// \param size    Back buffer width and height, in pixels
    /// \param scanout True to present the surface to the screen
    ///
    ////////////////////////////////////////////////////////////
    void createSurface(Vector2u size, bool scanout);

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
    /// \param settings     Requested context settings
    ///
    /// \return The best EGL config
    ///
    ////////////////////////////////////////////////////////////
    static EGLConfig getBestConfig(EGLDisplay display, const ContextSettings& settings);

    ////////////////////////////////////////////////////////////
    /// \brief Get the address of an OpenGL function
    ///
    /// \param name Name of the function to get the address of
    ///
    /// \return Address of the OpenGL function, 0 on failure
    ///
    ////////////////////////////////////////////////////////////
    static GlFunctionPointer getFunction(const char* name);

protected:
    friend class VideoModeImpl;
    friend class WindowImplDRM;

    ////////////////////////////////////////////////////////////
    /// \brief Get Direct Rendering Manager pointer
    ///
    ////////////////////////////////////////////////////////////
    static Drm& getDRM();

private:
    ////////////////////////////////////////////////////////////
    /// \brief Helper to copy the picked EGL configuration
    ///
    ////////////////////////////////////////////////////////////
    void updateSettings();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    EGLDisplay m_display{EGL_NO_DISPLAY}; ///< The internal EGL display
    EGLContext m_context{EGL_NO_CONTEXT}; ///< The internal EGL context
    EGLSurface m_surface{EGL_NO_SURFACE}; ///< The internal EGL surface
    EGLConfig  m_config{};                ///< The internal EGL config

    gbm_bo*      m_currentBO{};
    gbm_bo*      m_nextBO{};
    gbm_surface* m_gbmSurface{};
    bool         m_shown{};
    bool         m_scanOut{};
};

} // namespace sf::priv

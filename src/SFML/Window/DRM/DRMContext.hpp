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
    DRMContext(DRMContext* shared);

    ////////////////////////////////////////////////////////////
    DRMContext(DRMContext* shared, const ContextSettings& settings, const WindowImpl& owner, unsigned int bitsPerPixel);

    ////////////////////////////////////////////////////////////
    DRMContext(DRMContext* shared, const ContextSettings& settings, Vector2u size);

    ////////////////////////////////////////////////////////////
    ~DRMContext() override;

    ////////////////////////////////////////////////////////////
    bool makeCurrent(bool current) override;

    ////////////////////////////////////////////////////////////
    void display() override;

    ////////////////////////////////////////////////////////////
    void setVerticalSyncEnabled(bool enabled) override;

    ////////////////////////////////////////////////////////////
    void createContext(DRMContext* shared);

    ////////////////////////////////////////////////////////////
    void createSurface(Vector2u size, bool scanout);

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
    static GlFunctionPointer getFunction(const char* name);

protected:
    friend class VideoModeImpl;
    friend class WindowImplDRM;

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

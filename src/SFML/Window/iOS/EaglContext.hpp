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
#include <SFML/Window/GlContext.hpp>
#include <SFML/Window/iOS/ObjCType.hpp>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>

#include <glad/gl.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

SFML_DECLARE_OBJC_CLASS(EAGLContext);
SFML_DECLARE_OBJC_CLASS(SFView);

namespace sf::priv
{
class WindowImplUIKit;

////////////////////////////////////////////////////////////
class EaglContext : public GlContext
{
public:
    ////////////////////////////////////////////////////////////
    EaglContext(EaglContext* shared);

    ////////////////////////////////////////////////////////////
    EaglContext(EaglContext* shared, const ContextSettings& settings, const WindowImpl& owner, unsigned int bitsPerPixel);

    ////////////////////////////////////////////////////////////
    EaglContext(EaglContext* shared, const ContextSettings& settings, Vector2u size);

    ////////////////////////////////////////////////////////////
    ~EaglContext() override;

    ////////////////////////////////////////////////////////////
    static GlFunctionPointer getFunction(const char* name);

    ////////////////////////////////////////////////////////////
    /// \brief Recreate the render buffers of the context
    ///
    /// This function must be called whenever the containing view
    /// changes (typically after an orientation change)
    ///
    /// \param glView: Container of the context
    ///
    ////////////////////////////////////////////////////////////
    void recreateRenderBuffers(SFView* glView);

    ////////////////////////////////////////////////////////////
    void display() override;

    ////////////////////////////////////////////////////////////
    void setVerticalSyncEnabled(bool enabled) override;

protected:
    ////////////////////////////////////////////////////////////
    bool makeCurrent(bool current) override;

private:
    ////////////////////////////////////////////////////////////
    void createContext(EaglContext*           shared,
                       const WindowImplUIKit& window,
                       unsigned int           bitsPerPixel,
                       const ContextSettings& settings);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    EAGLContext* m_context;        ///< The internal context
    GLuint       m_framebuffer{};  ///< Frame buffer associated to the context
    GLuint       m_colorbuffer{};  ///< Color render buffer
    GLuint       m_depthbuffer{};  ///< Depth render buffer
    bool         m_vsyncEnabled{}; ///< Vertical sync activation flag
    Clock        m_clock;          ///< Measures the elapsed time for the fake v-sync implementation
};

} // namespace sf::priv

#pragma GCC diagnostic pop

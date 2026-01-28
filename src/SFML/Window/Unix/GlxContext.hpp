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
#include <SFML/Window/WindowEnums.hpp> // Prevent conflict with macro None from Xlib

#include <X11/Xlib.h>
#include <glad/glx.h>

#include <memory>


namespace sf::priv
{
////////////////////////////////////////////////////////////
class GlxContext : public GlContext
{
public:
    ////////////////////////////////////////////////////////////
    explicit GlxContext(GlxContext* shared);

    ////////////////////////////////////////////////////////////
    GlxContext(GlxContext* shared, const ContextSettings& settings, const WindowImpl& owner, unsigned int bitsPerPixel);

    ////////////////////////////////////////////////////////////
    GlxContext(GlxContext* shared, const ContextSettings& settings, Vector2u size);

    ////////////////////////////////////////////////////////////
    ~GlxContext() override;

    ////////////////////////////////////////////////////////////
    static GlFunctionPointer getFunction(const char* name);

    ////////////////////////////////////////////////////////////
    bool makeCurrent(bool current) override;

    ////////////////////////////////////////////////////////////
    void display() override;

    ////////////////////////////////////////////////////////////
    void setVerticalSyncEnabled(bool enabled) override;

    ////////////////////////////////////////////////////////////
    static XVisualInfo selectBestVisual(::Display* display, unsigned int bitsPerPixel, const ContextSettings& settings);

private:
    ////////////////////////////////////////////////////////////
    void updateSettingsFromVisualInfo(XVisualInfo* visualInfo);

    ////////////////////////////////////////////////////////////
    void updateSettingsFromWindow();

    ////////////////////////////////////////////////////////////
    void createSurface(GlxContext* shared, Vector2u size, unsigned int bitsPerPixel);

    ////////////////////////////////////////////////////////////
    void createSurface(::Window window);

    ////////////////////////////////////////////////////////////
    void createContext(GlxContext* shared);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::shared_ptr<Display> m_display;      ///< Connection to the X server
    ::Window                 m_window{};     ///< Window to which the context is attached
    GLXContext               m_context{};    ///< OpenGL context
    GLXPbuffer               m_pbuffer{};    ///< GLX pbuffer ID if one was created
    bool                     m_ownsWindow{}; ///< Do we own the window associated to the context?
};

} // namespace sf::priv

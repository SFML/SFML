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
#include <SFML/Window/Context.hpp>
#include <SFML/Window/GlContext.hpp>

#include <SFML/System/Vector2.hpp>

#include <glad/wgl.h>


namespace sf
{
struct ContextSettings;

namespace priv
{
class WindowImpl;

////////////////////////////////////////////////////////////
class WglContext : public GlContext
{
public:
    ////////////////////////////////////////////////////////////
    WglContext(WglContext* shared);

    ////////////////////////////////////////////////////////////
    WglContext(WglContext* shared, const ContextSettings& settings, const WindowImpl& owner, unsigned int bitsPerPixel);

    ////////////////////////////////////////////////////////////
    WglContext(WglContext* shared, const ContextSettings& settings, Vector2u size);

    ////////////////////////////////////////////////////////////
    ~WglContext() override;

    ////////////////////////////////////////////////////////////
    static GlFunctionPointer getFunction(const char* name);

    ////////////////////////////////////////////////////////////
    bool makeCurrent(bool current) override;

    ////////////////////////////////////////////////////////////
    void display() override;

    ////////////////////////////////////////////////////////////
    void setVerticalSyncEnabled(bool enabled) override;

    ////////////////////////////////////////////////////////////
    static int selectBestPixelFormat(HDC                    deviceContext,
                                     unsigned int           bitsPerPixel,
                                     const ContextSettings& settings,
                                     bool                   pbuffer = false);

private:
    ////////////////////////////////////////////////////////////
    void setDevicePixelFormat(unsigned int bitsPerPixel);

    ////////////////////////////////////////////////////////////
    void updateSettingsFromPixelFormat();

    ////////////////////////////////////////////////////////////
    void createSurface(WglContext* shared, Vector2u size, unsigned int bitsPerPixel);

    ////////////////////////////////////////////////////////////
    void createSurface(HWND window, unsigned int bitsPerPixel);

    ////////////////////////////////////////////////////////////
    void createContext(WglContext* shared);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    HWND        m_window{};        //!< Window to which the context is attached
    HPBUFFERARB m_pbuffer{};       //!< Handle to a pbuffer if one was created
    HDC         m_deviceContext{}; //!< Device context associated to the context
    HGLRC       m_context{};       //!< OpenGL context
    bool        m_ownsWindow{};    //!< Do we own the target window?
    bool        m_isGeneric{};     //!< Is this context provided by the generic GDI implementation?
};

} // namespace priv
} // namespace sf

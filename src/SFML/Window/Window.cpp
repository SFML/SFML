////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2021 Laurent Gomila (laurent@sfml-dev.org)
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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Window.hpp>
#include <SFML/Window/GlContext.hpp>
#include <SFML/Window/WindowImpl.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Err.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
Window::Window() :
m_context       (NULL),
m_frameTimeLimit(Time::Zero)
{

}


////////////////////////////////////////////////////////////
Window::Window(VideoMode mode, const String& title, Uint32 style, const ContextSettings& settings) :
m_context       (NULL),
m_frameTimeLimit(Time::Zero)
{
    Window::create(mode, title, style, settings);
}


////////////////////////////////////////////////////////////
Window::Window(WindowHandle handle, const ContextSettings& settings) :
m_context       (NULL),
m_frameTimeLimit(Time::Zero)
{
    Window::create(handle, settings);
}


////////////////////////////////////////////////////////////
Window::~Window()
{
    close();
}


////////////////////////////////////////////////////////////
void Window::create(VideoMode mode, const String& title, Uint32 style)
{
    Window::create(mode, title, style, ContextSettings());
}


////////////////////////////////////////////////////////////
void Window::create(VideoMode mode, const String& title, Uint32 style, const ContextSettings& settings)
{
    // Destroy the previous window implementation
    close();

    // Fullscreen style requires some tests
    if (style & Style::Fullscreen)
    {
        // Make sure there's not already a fullscreen window (only one is allowed)
        if (getFullscreenWindow())
        {
            err() << "Creating two fullscreen windows is not allowed, switching to windowed mode" << std::endl;
            style &= ~Style::Fullscreen;
        }
        else
        {
            // Make sure that the chosen video mode is compatible
            if (!mode.isValid())
            {
                err() << "The requested video mode is not available, switching to a valid mode" << std::endl;
                mode = VideoMode::getFullscreenModes()[0];
            }

            // Update the fullscreen window
            setFullscreenWindow(this);
        }
    }

    // Check validity of style according to the underlying platform
    #if defined(SFML_SYSTEM_IOS) || defined(SFML_SYSTEM_ANDROID)
        if (style & Style::Fullscreen)
            style &= ~Style::Titlebar;
        else
            style |= Style::Titlebar;
    #else
        if ((style & Style::Close) || (style & Style::Resize))
            style |= Style::Titlebar;
    #endif

    // Recreate the window implementation
    m_impl = priv::WindowImpl::create(mode, title, style, settings);

    // Recreate the context
    m_context = priv::GlContext::create(settings, m_impl, mode.bitsPerPixel);

    // Perform common initializations
    initialize();
}


////////////////////////////////////////////////////////////
void Window::create(WindowHandle handle)
{
    Window::create(handle, ContextSettings());
}


////////////////////////////////////////////////////////////
void Window::create(WindowHandle handle, const ContextSettings& settings)
{
    // Destroy the previous window implementation
    close();

    // Recreate the window implementation
    WindowBase::create(handle);

    // Recreate the context
    m_context = priv::GlContext::create(settings, m_impl, VideoMode::getDesktopMode().bitsPerPixel);

    // Perform common initializations
    initialize();
}


////////////////////////////////////////////////////////////
void Window::close()
{
    // Delete the context
    delete m_context;
    m_context = NULL;

    // Close the base window
    WindowBase::close();
}


////////////////////////////////////////////////////////////
const ContextSettings& Window::getSettings() const
{
    static const ContextSettings empty(0, 0, 0);

    return m_context ? m_context->getSettings() : empty;
}


////////////////////////////////////////////////////////////
void Window::setVerticalSyncEnabled(bool enabled)
{
    if (setActive())
        m_context->setVerticalSyncEnabled(enabled);
}


////////////////////////////////////////////////////////////
void Window::setFramerateLimit(unsigned int limit)
{
    if (limit > 0)
        m_frameTimeLimit = seconds(1.f / limit);
    else
        m_frameTimeLimit = Time::Zero;
}


////////////////////////////////////////////////////////////
bool Window::setActive(bool active) const
{
    if (m_context)
    {
        if (m_context->setActive(active))
        {
            return true;
        }
        else
        {
            err() << "Failed to activate the window's context" << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
void Window::display()
{
    // Display the backbuffer on screen
    if (setActive())
        m_context->display();

    // Limit the framerate if needed
    if (m_frameTimeLimit != Time::Zero)
    {
        sleep(m_frameTimeLimit - m_clock.getElapsedTime());
        m_clock.restart();
    }
}


////////////////////////////////////////////////////////////
void Window::initialize()
{
    // Setup default behaviors (to get a consistent behavior across different implementations)
    setVerticalSyncEnabled(false);
    setFramerateLimit(0);

    // Reset frame time
    m_clock.restart();

    // Activate the window
    setActive();

    WindowBase::initialize();
}

} // namespace sf

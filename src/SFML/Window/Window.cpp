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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/GlContext.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/WindowImpl.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Sleep.hpp>

#include <ostream>


namespace sf
{
////////////////////////////////////////////////////////////
Window::Window() = default;


////////////////////////////////////////////////////////////
Window::Window(VideoMode mode, const String& title, std::uint32_t style, State state, const ContextSettings& settings)
{
    Window::create(mode, title, style, state, settings);
}


////////////////////////////////////////////////////////////
Window::Window(VideoMode mode, const String& title, State state, const ContextSettings& settings)
{
    Window::create(mode, title, sf::Style::Default, state, settings);
}


////////////////////////////////////////////////////////////
Window::Window(WindowHandle handle, const ContextSettings& settings)
{
    Window::create(handle, settings);
}


////////////////////////////////////////////////////////////
Window::~Window() = default;


////////////////////////////////////////////////////////////
Window::Window(Window&&) noexcept = default;


////////////////////////////////////////////////////////////
Window& Window::operator=(Window&&) noexcept = default;


////////////////////////////////////////////////////////////
void Window::create(VideoMode mode, const String& title, std::uint32_t style, State state)
{
    Window::create(mode, title, style, state, ContextSettings{});
}


////////////////////////////////////////////////////////////
void Window::create(VideoMode mode, const String& title, std::uint32_t style, State state, const ContextSettings& settings)
{
    // Ensure the open window is closed first
    close();

    // Recreate the window implementation
    m_impl = priv::WindowImpl::create(mode, title, style, state, settings);

    // Recreate the context
    m_context = priv::GlContext::create(settings, *m_impl, mode.bitsPerPixel);

    // Perform common initializations
    initialize();
}


////////////////////////////////////////////////////////////
void Window::create(VideoMode mode, const String& title, State state)
{
    Window::create(mode, title, sf::Style::Default, state, ContextSettings{});
}


////////////////////////////////////////////////////////////
void Window::create(VideoMode mode, const String& title, State state, const ContextSettings& settings)
{
    Window::create(mode, title, sf::Style::Default, state, settings);
}


////////////////////////////////////////////////////////////
void Window::create(WindowHandle handle)
{
    Window::create(handle, ContextSettings{});
}


////////////////////////////////////////////////////////////
void Window::create(WindowHandle handle, const ContextSettings& settings)
{
    // Ensure the open window is closed first
    close();

    // Recreate the window implementation
    m_impl = priv::WindowImpl::create(handle);

    // Recreate the context
    m_context = priv::GlContext::create(settings, *m_impl, VideoMode::getDesktopMode().bitsPerPixel);

    // Perform common initializations
    initialize();
}


////////////////////////////////////////////////////////////
void Window::close()
{
    // Delete the context
    m_context.reset();

    // Close the base window
    WindowBase::close();
}


////////////////////////////////////////////////////////////
const ContextSettings& Window::getSettings() const
{
    static constexpr ContextSettings empty{/* depthBits */ 0, /* stencilBits */ 0, /* antiAliasingLevel */ 0};

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
        m_frameTimeLimit = seconds(1.f / static_cast<float>(limit));
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

        err() << "Failed to activate the window's context" << std::endl;
        return false;
    }

    return false;
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
    if (!setActive())
    {
        err() << "Failed to set window as active during initialization" << std::endl;
    }

    WindowBase::initialize();
}

} // namespace sf

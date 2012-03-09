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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Window.hpp>
#include <SFML/Window/GlContext.hpp>
#include <SFML/Window/WindowImpl.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Err.hpp>


namespace
{
    const sf::Window* fullscreenWindow = NULL;
}


namespace sf
{
////////////////////////////////////////////////////////////
Window::Window() :
m_impl          (NULL),
m_context       (NULL),
m_frameTimeLimit(Time::Zero)
{

}


////////////////////////////////////////////////////////////
Window::Window(VideoMode mode, const std::string& title, Uint32 style, const ContextSettings& settings) :
m_impl          (NULL),
m_context       (NULL),
m_frameTimeLimit(Time::Zero)
{
    Create(mode, title, style, settings);
}


////////////////////////////////////////////////////////////
Window::Window(WindowHandle handle, const ContextSettings& settings) :
m_impl          (NULL),
m_context       (NULL),
m_frameTimeLimit(Time::Zero)
{
    Create(handle, settings);
}


////////////////////////////////////////////////////////////
Window::~Window()
{
    Close();
}


////////////////////////////////////////////////////////////
void Window::Create(VideoMode mode, const std::string& title, Uint32 style, const ContextSettings& settings)
{
    // Destroy the previous window implementation
    Close();

    // Fullscreen style requires some tests
    if (style & Style::Fullscreen)
    {
        // Make sure there's not already a fullscreen window (only one is allowed)
        if (fullscreenWindow)
        {
            Err() << "Creating two fullscreen windows is not allowed, switching to windowed mode" << std::endl;
            style &= ~Style::Fullscreen;
        }
        else
        {
            // Make sure that the chosen video mode is compatible
            if (!mode.IsValid())
            {
                Err() << "The requested video mode is not available, switching to a valid mode" << std::endl;
                mode = VideoMode::GetFullscreenModes()[0];
            }

            // Update the fullscreen window
            fullscreenWindow = this;
        }
    }

    // Check validity of style
    if ((style & Style::Close) || (style & Style::Resize))
        style |= Style::Titlebar;

    // Recreate the window implementation
    m_impl = priv::WindowImpl::Create(mode, title, style);

    // Recreate the context
    m_context = priv::GlContext::Create(settings, m_impl, mode.BitsPerPixel);

    // Perform common initializations
    Initialize();
}


////////////////////////////////////////////////////////////
void Window::Create(WindowHandle handle, const ContextSettings& settings)
{
    // Destroy the previous window implementation
    Close();

    // Recreate the window implementation
    m_impl = priv::WindowImpl::Create(handle);

    // Recreate the context
    m_context = priv::GlContext::Create(settings, m_impl, VideoMode::GetDesktopMode().BitsPerPixel);

    // Perform common initializations
    Initialize();
}


////////////////////////////////////////////////////////////
void Window::Close()
{
    if (m_context)
    {
        // Delete the context
        delete m_context;
        m_context = NULL;
    }

    if (m_impl)
    {
        // Delete the window implementation
        delete m_impl;
        m_impl = NULL;
    }

    // Update the fullscreen window
    if (this == fullscreenWindow)
        fullscreenWindow = NULL;
}


////////////////////////////////////////////////////////////
bool Window::IsOpen() const
{
    return m_impl != NULL;
}


////////////////////////////////////////////////////////////
const ContextSettings& Window::GetSettings() const
{
    static const ContextSettings empty(0, 0, 0);

    return m_context ? m_context->GetSettings() : empty;
}


////////////////////////////////////////////////////////////
bool Window::PollEvent(Event& event)
{
    if (m_impl && m_impl->PopEvent(event, false))
    {
        return FilterEvent(event);
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
bool Window::WaitEvent(Event& event)
{
    if (m_impl && m_impl->PopEvent(event, true))
    {
        return FilterEvent(event);
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
Vector2i Window::GetPosition() const
{
    return m_impl ? m_impl->GetPosition() : Vector2i();
}


////////////////////////////////////////////////////////////
void Window::SetPosition(const Vector2i& position)
{
    if (m_impl)
        m_impl->SetPosition(position);
}


////////////////////////////////////////////////////////////
Vector2u Window::GetSize() const
{
    return m_impl ? m_impl->GetSize() : Vector2u();
}


////////////////////////////////////////////////////////////
void Window::SetSize(const Vector2u size)
{
    if (m_impl)
        m_impl->SetSize(size);
}


////////////////////////////////////////////////////////////
void Window::SetTitle(const std::string& title)
{
    if (m_impl)
        m_impl->SetTitle(title);
}


////////////////////////////////////////////////////////////
void Window::SetIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    if (m_impl)
        m_impl->SetIcon(width, height, pixels);
}


////////////////////////////////////////////////////////////
void Window::SetVisible(bool visible)
{
    if (m_impl)
        m_impl->SetVisible(visible);
}


////////////////////////////////////////////////////////////
void Window::SetVerticalSyncEnabled(bool enabled)
{
    if (SetActive())
        m_context->SetVerticalSyncEnabled(enabled);
}


////////////////////////////////////////////////////////////
void Window::SetMouseCursorVisible(bool visible)
{
    if (m_impl)
        m_impl->SetMouseCursorVisible(visible);
}


////////////////////////////////////////////////////////////
void Window::SetKeyRepeatEnabled(bool enabled)
{
    if (m_impl)
        m_impl->SetKeyRepeatEnabled(enabled);
}


////////////////////////////////////////////////////////////
void Window::SetFramerateLimit(unsigned int limit)
{
    if (limit > 0)
        m_frameTimeLimit = Seconds(1.f / limit);
    else
        m_frameTimeLimit = Time::Zero;
}


////////////////////////////////////////////////////////////
void Window::SetJoystickThreshold(float threshold)
{
    if (m_impl)
        m_impl->SetJoystickThreshold(threshold);
}


////////////////////////////////////////////////////////////
bool Window::SetActive(bool active) const
{
    if (m_context)
    {
        if (m_context->SetActive(active))
        {
            return true;
        }
        else
        {
            Err() << "Failed to activate the window's context" << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
void Window::Display()
{
    // Display the backbuffer on screen
    if (SetActive())
        m_context->Display();

    // Limit the framerate if needed
    if (m_frameTimeLimit != Time::Zero)
    {
        Sleep(m_frameTimeLimit - m_clock.GetElapsedTime());
        m_clock.Restart();
    }
}


////////////////////////////////////////////////////////////
WindowHandle Window::GetSystemHandle() const
{
    return m_impl ? m_impl->GetSystemHandle() : 0;
}


////////////////////////////////////////////////////////////
void Window::OnCreate()
{
    // Nothing by default
}


////////////////////////////////////////////////////////////
void Window::OnResize()
{
    // Nothing by default
}


////////////////////////////////////////////////////////////
bool Window::FilterEvent(const Event& event)
{
    // Notify resize events to the derived class
    if (event.Type == Event::Resized)
        OnResize();

    return true;
}


////////////////////////////////////////////////////////////
void Window::Initialize()
{
    // Setup default behaviours (to get a consistent behaviour across different implementations)
    SetVisible(true);
    SetMouseCursorVisible(true);
    SetVerticalSyncEnabled(false);
    SetKeyRepeatEnabled(true);

    // Reset frame time
    m_clock.Restart();

    // Activate the window
    SetActive();

    // Notify the derived class
    OnCreate();
}

} // namespace sf

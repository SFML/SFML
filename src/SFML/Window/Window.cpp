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
myImpl          (NULL),
myContext       (NULL),
myFrameTimeLimit(Time::Zero)
{

}


////////////////////////////////////////////////////////////
Window::Window(VideoMode mode, const std::string& title, Uint32 style, const ContextSettings& settings) :
myImpl          (NULL),
myContext       (NULL),
myFrameTimeLimit(Time::Zero)
{
    Create(mode, title, style, settings);
}


////////////////////////////////////////////////////////////
Window::Window(WindowHandle handle, const ContextSettings& settings) :
myImpl          (NULL),
myContext       (NULL),
myFrameTimeLimit(Time::Zero)
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
    myImpl = priv::WindowImpl::New(mode, title, style);

    // Recreate the context
    myContext = priv::GlContext::New(settings, myImpl, mode.BitsPerPixel);

    // Perform common initializations
    Initialize();
}


////////////////////////////////////////////////////////////
void Window::Create(WindowHandle handle, const ContextSettings& settings)
{
    // Destroy the previous window implementation
    Close();

    // Recreate the window implementation
    myImpl = priv::WindowImpl::New(handle);

    // Recreate the context
    myContext = priv::GlContext::New(settings, myImpl, VideoMode::GetDesktopMode().BitsPerPixel);

    // Perform common initializations
    Initialize();
}


////////////////////////////////////////////////////////////
void Window::Close()
{
    if (myContext)
    {
        // Delete the context
        delete myContext;
        myContext = NULL;
    }

    if (myImpl)
    {
        // Delete the window implementation
        delete myImpl;
        myImpl = NULL;
    }

    // Update the fullscreen window
    if (this == fullscreenWindow)
        fullscreenWindow = NULL;
}


////////////////////////////////////////////////////////////
bool Window::IsOpen() const
{
    return myImpl != NULL;
}


////////////////////////////////////////////////////////////
unsigned int Window::GetWidth() const
{
    return myImpl ? myImpl->GetWidth() : 0;
}


////////////////////////////////////////////////////////////
unsigned int Window::GetHeight() const
{
    return myImpl ? myImpl->GetHeight() : 0;
}


////////////////////////////////////////////////////////////
const ContextSettings& Window::GetSettings() const
{
    static const ContextSettings empty(0, 0, 0);

    return myContext ? myContext->GetSettings() : empty;
}


////////////////////////////////////////////////////////////
bool Window::PollEvent(Event& event)
{
    if (myImpl && myImpl->PopEvent(event, false))
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
    if (myImpl && myImpl->PopEvent(event, true))
    {
        return FilterEvent(event);
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
void Window::EnableVerticalSync(bool enabled)
{
    if (SetActive())
        myContext->EnableVerticalSync(enabled);
}


////////////////////////////////////////////////////////////
void Window::ShowMouseCursor(bool show)
{
    if (myImpl)
        myImpl->ShowMouseCursor(show);
}


////////////////////////////////////////////////////////////
void Window::SetPosition(int x, int y)
{
    if (myImpl)
        myImpl->SetPosition(x, y);
}


////////////////////////////////////////////////////////////
void Window::SetSize(unsigned int width, unsigned int height)
{
    if (myImpl)
        myImpl->SetSize(width, height);
}


////////////////////////////////////////////////////////////
void Window::SetTitle(const std::string& title)
{
    if (myImpl)
        myImpl->SetTitle(title);
}


////////////////////////////////////////////////////////////
void Window::Show(bool show)
{
    if (myImpl)
        myImpl->Show(show);
}


////////////////////////////////////////////////////////////
void Window::EnableKeyRepeat(bool enabled)
{
    if (myImpl)
        myImpl->EnableKeyRepeat(enabled);
}


////////////////////////////////////////////////////////////
void Window::SetIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    if (myImpl)
        myImpl->SetIcon(width, height, pixels);
}


////////////////////////////////////////////////////////////
bool Window::SetActive(bool active) const
{
    if (myContext)
    {
        if (myContext->SetActive(active))
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
        myContext->Display();

    // Limit the framerate if needed
    if (myFrameTimeLimit != Time::Zero)
    {
        Sleep(myFrameTimeLimit - myClock.GetElapsedTime());
        myClock.Restart();
    }
}


////////////////////////////////////////////////////////////
void Window::SetFramerateLimit(unsigned int limit)
{
    if (limit > 0)
        myFrameTimeLimit = Seconds(1.f / limit);
    else
        myFrameTimeLimit = Time::Zero;
}


////////////////////////////////////////////////////////////
void Window::SetJoystickThreshold(float threshold)
{
    if (myImpl)
        myImpl->SetJoystickThreshold(threshold);
}


////////////////////////////////////////////////////////////
WindowHandle Window::GetSystemHandle() const
{
    return myImpl ? myImpl->GetSystemHandle() : 0;
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
    Show(true);
    ShowMouseCursor(true);
    EnableVerticalSync(false);
    EnableKeyRepeat(true);

    // Reset frame time
    myClock.Restart();

    // Activate the window
    SetActive();

    // Notify the derived class
    OnCreate();
}

} // namespace sf

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


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    const sf::Window* fullscreenWindow = NULL;
}


namespace sf
{
////////////////////////////////////////////////////////////
Window::Window() :
myWindow        (NULL),
myContext       (NULL),
myLastFrameTime (0.f),
myFramerateLimit(0),
mySetCursorPosX (0xFFFF),
mySetCursorPosY (0xFFFF)
{

}


////////////////////////////////////////////////////////////
Window::Window(VideoMode mode, const std::string& title, unsigned long style, const ContextSettings& settings) :
myWindow        (NULL),
myContext       (NULL),
myLastFrameTime (0.f),
myFramerateLimit(0),
mySetCursorPosX (0xFFFF),
mySetCursorPosY (0xFFFF)
{
    Create(mode, title, style, settings);
}


////////////////////////////////////////////////////////////
Window::Window(WindowHandle handle, const ContextSettings& settings) :
myWindow        (NULL),
myContext       (NULL),
myLastFrameTime (0.f),
myFramerateLimit(0),
mySetCursorPosX (0xFFFF),
mySetCursorPosY (0xFFFF)
{
    Create(handle, settings);
}


////////////////////////////////////////////////////////////
Window::~Window()
{
    Close();
}


////////////////////////////////////////////////////////////
void Window::Create(VideoMode mode, const std::string& title, unsigned long style, const ContextSettings& settings)
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
    myWindow = priv::WindowImpl::New(mode, title, style);

    // Recreate the context
    myContext = priv::GlContext::New(myWindow, mode.BitsPerPixel, settings);

    // Perform common initializations
    Initialize();
}


////////////////////////////////////////////////////////////
void Window::Create(WindowHandle handle, const ContextSettings& settings)
{
    // Destroy the previous window implementation
    Close();

    // Recreate the window implementation
    myWindow = priv::WindowImpl::New(handle);

    // Recreate the context
    myContext = priv::GlContext::New(myWindow, VideoMode::GetDesktopMode().BitsPerPixel, settings);

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

    if (myWindow)
    {
        // Delete the window implementation
        delete myWindow;
        myWindow = NULL;
    }

    // Update the fullscreen window
    if (this == fullscreenWindow)
        fullscreenWindow = NULL;
}


////////////////////////////////////////////////////////////
bool Window::IsOpened() const
{
    return myWindow != NULL;
}


////////////////////////////////////////////////////////////
unsigned int Window::GetWidth() const
{
    return myWindow ? myWindow->GetWidth() : 0;
}


////////////////////////////////////////////////////////////
unsigned int Window::GetHeight() const
{
    return myWindow ? myWindow->GetHeight() : 0;
}


////////////////////////////////////////////////////////////
const ContextSettings& Window::GetSettings() const
{
    static const ContextSettings empty(0, 0, 0);

    return myContext ? myContext->GetSettings() : empty;
}


////////////////////////////////////////////////////////////
bool Window::GetEvent(Event& event)
{
    if (myWindow && myWindow->PopEvent(event, false))
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
    if (myWindow && myWindow->PopEvent(event, true))
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
    if (myWindow)
        myWindow->ShowMouseCursor(show);
}


////////////////////////////////////////////////////////////
void Window::SetCursorPosition(unsigned int x, unsigned int y)
{
    if (myWindow)
    {
        // Keep coordinates for later checking (to reject the generated MouseMoved event)
        mySetCursorPosX = x;
        mySetCursorPosY = y;

        myWindow->SetCursorPosition(x, y);
    }
}


////////////////////////////////////////////////////////////
void Window::SetPosition(int x, int y)
{
    if (myWindow)
        myWindow->SetPosition(x, y);
}


////////////////////////////////////////////////////////////
void Window::SetSize(unsigned int width, unsigned int height)
{
    if (myWindow)
        myWindow->SetSize(width, height);
}


////////////////////////////////////////////////////////////
void Window::SetTitle(const std::string& title)
{
    if (myWindow)
        myWindow->SetTitle(title);
}


////////////////////////////////////////////////////////////
void Window::Show(bool show)
{
    if (myWindow)
        myWindow->Show(show);
}


////////////////////////////////////////////////////////////
void Window::EnableKeyRepeat(bool enabled)
{
    if (myWindow)
        myWindow->EnableKeyRepeat(enabled);
}


////////////////////////////////////////////////////////////
void Window::SetIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    if (myWindow)
        myWindow->SetIcon(width, height, pixels);
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
    // Limit the framerate if needed
    if (myFramerateLimit > 0)
    {
        float remainingTime = 1.f / myFramerateLimit - myClock.GetElapsedTime();
        if (remainingTime > 0)
            Sleep(remainingTime);
    }

    // Measure the time elapsed since last frame
    myLastFrameTime = myClock.GetElapsedTime();
    myClock.Reset();

    // Display the backbuffer on screen
    if (SetActive())
        myContext->Display();
}


////////////////////////////////////////////////////////////
const Input& Window::GetInput() const
{
    return myInput;
}


////////////////////////////////////////////////////////////
void Window::SetFramerateLimit(unsigned int limit)
{
    myFramerateLimit = limit;
}


////////////////////////////////////////////////////////////
float Window::GetFrameTime() const
{
    return myLastFrameTime;
}


////////////////////////////////////////////////////////////
void Window::SetJoystickThreshold(float threshold)
{
    if (myWindow)
        myWindow->SetJoystickThreshold(threshold);
}


////////////////////////////////////////////////////////////
WindowHandle Window::GetSystemHandle() const
{
    return myWindow ? myWindow->GetSystemHandle() : 0;
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
    // Notify the input object
    myInput.OnEvent(event);

    // Notify resize events to the derived class
    if (event.Type == Event::Resized)
        OnResize();

    // Don't forward to the user MouseMove events generated by SetCursorPosition
    if ((event.Type        == Event::MouseMoved) &&
        (event.MouseMove.X == mySetCursorPosX)   &&
        (event.MouseMove.Y == mySetCursorPosY))
    {
        mySetCursorPosX = 0xFFFF;
        mySetCursorPosY = 0xFFFF;
        return false;
    }

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
    myClock.Reset();
    myLastFrameTime = 0.f;

    // Activate the window
    SetActive();

    // Notify the derived class
    OnCreate();
}

} // namespace sf

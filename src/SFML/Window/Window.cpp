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
#include <SFML/Window/ContextGL.hpp>
#include <SFML/Window/Context.hpp>
#include <SFML/Window/WindowImpl.hpp>
#include <SFML/System/Sleep.hpp>
#include <iostream>


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
/// Default constructor
////////////////////////////////////////////////////////////
Window::Window() :
myWindow        (NULL),
myContext       (NULL),
myLastFrameTime (0.f),
myIsExternal    (false),
myFramerateLimit(0),
mySetCursorPosX (0xFFFF),
mySetCursorPosY (0xFFFF)
{

}


////////////////////////////////////////////////////////////
/// Construct a new window
////////////////////////////////////////////////////////////
Window::Window(VideoMode mode, const std::string& title, unsigned long style, const ContextSettings& settings) :
myWindow        (NULL),
myContext       (NULL),
myLastFrameTime (0.f),
myIsExternal    (false),
myFramerateLimit(0),
mySetCursorPosX (0xFFFF),
mySetCursorPosY (0xFFFF)
{
    Create(mode, title, style, settings);
}


////////////////////////////////////////////////////////////
/// Construct the window from an existing control
////////////////////////////////////////////////////////////
Window::Window(WindowHandle handle, const ContextSettings& settings) :
myWindow        (NULL),
myContext       (NULL),
myLastFrameTime (0.f),
myIsExternal    (true),
myFramerateLimit(0),
mySetCursorPosX (0xFFFF),
mySetCursorPosY (0xFFFF)
{
    Create(handle, settings);
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
Window::~Window()
{
    Close();
}


////////////////////////////////////////////////////////////
/// Create the window
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
            std::cerr << "Creating two fullscreen windows is not allowed, switching to windowed mode" << std::endl;
            style &= ~Style::Fullscreen;
        }
        else
        {
            // Make sure the chosen video mode is compatible
            if (!mode.IsValid())
            {
                std::cerr << "The requested video mode is not available, switching to a valid mode" << std::endl;
                mode = VideoMode::GetMode(0);
            }

            // Update the fullscreen window
            fullscreenWindow = this;
        }
    }

    // Check validity of style
    if ((style & Style::Close) || (style & Style::Resize))
        style |= Style::Titlebar;

    // Recreate the window implementation
    delete myWindow;
    myWindow = priv::WindowImpl::New(mode, title, style);

    // Make sure another context is bound, so that:
    // - the context creation can request OpenGL extensions if necessary
    // - myContext can safely be destroyed (it's no longer bound)
    Context context;

    // Recreate the context
    delete myContext;
    myContext = priv::ContextGL::New(myWindow, mode.BitsPerPixel, settings);

    // Perform common initializations
    Initialize();
}


////////////////////////////////////////////////////////////
/// Create the window from an existing control
////////////////////////////////////////////////////////////
void Window::Create(WindowHandle handle, const ContextSettings& settings)
{
    // Recreate the window implementation
    Close();
    myWindow = priv::WindowImpl::New(handle);

    // Make sure another context is bound, so that:
    // - the context creation can request OpenGL extensions if necessary
    // - myContext can safely be destroyed (it's no longer bound)
    Context context;

    // Recreate the context
    delete myContext;
    myContext = priv::ContextGL::New(myWindow, VideoMode::GetDesktopMode().BitsPerPixel, settings);

    // Perform common initializations
    Initialize();
}


////////////////////////////////////////////////////////////
/// Close (destroy) the window.
/// The sf::Window instance remains valid and you can call
/// Create to recreate the window
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
/// Tell whether or not the window is opened (ie. has been created).
/// Note that a hidden window (Show(false))
/// will still return true
////////////////////////////////////////////////////////////
bool Window::IsOpened() const
{
    return myWindow != NULL;
}


////////////////////////////////////////////////////////////
/// Get the width of the rendering region of the window
////////////////////////////////////////////////////////////
unsigned int Window::GetWidth() const
{
    return myWindow ? myWindow->GetWidth() : 0;
}


////////////////////////////////////////////////////////////
/// Get the height of the rendering region of the window
////////////////////////////////////////////////////////////
unsigned int Window::GetHeight() const
{
    return myWindow ? myWindow->GetHeight() : 0;
}


////////////////////////////////////////////////////////////
/// Get the creation settings of the window
////////////////////////////////////////////////////////////
const ContextSettings& Window::GetSettings() const
{
    static const ContextSettings empty(0, 0, 0);

    return myContext ? myContext->GetSettings() : empty;
}


////////////////////////////////////////////////////////////
/// Get the event on top of events stack, if any
////////////////////////////////////////////////////////////
bool Window::GetEvent(Event& event)
{
    // Let the window implementation process incoming events if the events queue is empty
    if (myWindow && myEvents.empty())
        myWindow->DoEvents();

    // Pop first event of queue, if not empty
    if (!myEvents.empty())
    {
        event = myEvents.front();
        myEvents.pop();

        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
/// Enable / disable vertical synchronization
////////////////////////////////////////////////////////////
void Window::UseVerticalSync(bool enabled)
{
    if (SetActive())
        myContext->UseVerticalSync(enabled);
}


////////////////////////////////////////////////////////////
/// Show or hide the mouse cursor
////////////////////////////////////////////////////////////
void Window::ShowMouseCursor(bool show)
{
    if (myWindow)
        myWindow->ShowMouseCursor(show);
}


////////////////////////////////////////////////////////////
/// Change the position of the mouse cursor
////////////////////////////////////////////////////////////
void Window::SetCursorPosition(unsigned int left, unsigned int top)
{
    if (myWindow)
    {
        // Keep coordinates for later checking (to reject the generated MouseMoved event)
        mySetCursorPosX = left;
        mySetCursorPosY = top;

        myWindow->SetCursorPosition(left, top);
    }
}


////////////////////////////////////////////////////////////
/// Change the position of the window on screen
////////////////////////////////////////////////////////////
void Window::SetPosition(int left, int top)
{
    if (myWindow)
        myWindow->SetPosition(left, top);
}


////////////////////////////////////////////////////////////
/// Change the size of the rendering region of the window
////////////////////////////////////////////////////////////
void Window::SetSize(unsigned int width, unsigned int height)
{
    if (myWindow)
        myWindow->SetSize(width, height);
}


////////////////////////////////////////////////////////////
/// Show or hide the window
////////////////////////////////////////////////////////////
void Window::Show(bool show)
{
    if (myWindow)
        myWindow->Show(show);
}


////////////////////////////////////////////////////////////
/// Enable or disable automatic key-repeat.
/// Automatic key-repeat is enabled by default
////////////////////////////////////////////////////////////
void Window::EnableKeyRepeat(bool enabled)
{
    if (myWindow)
        myWindow->EnableKeyRepeat(enabled);
}


////////////////////////////////////////////////////////////
/// Change the window's icon
////////////////////////////////////////////////////////////
void Window::SetIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    if (myWindow)
        myWindow->SetIcon(width, height, pixels);
}


////////////////////////////////////////////////////////////
/// Activate or deactivate the window as the current target
/// for rendering
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
            std::cerr << "Failed to activate the window's context" << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
/// Display the window on screen
////////////////////////////////////////////////////////////
void Window::Display()
{
    // Notify the derived class
    OnDisplay();

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
    if (SetActive(true))
    {
        myContext->Display();
        SetActive(false);
    }
}


////////////////////////////////////////////////////////////
/// Get the input manager of the window
////////////////////////////////////////////////////////////
const Input& Window::GetInput() const
{
    return myInput;
}


////////////////////////////////////////////////////////////
/// Set the framerate at a fixed frequency
////////////////////////////////////////////////////////////
void Window::SetFramerateLimit(unsigned int limit)
{
    myFramerateLimit = limit;
}


////////////////////////////////////////////////////////////
/// Get time elapsed since last frame
////////////////////////////////////////////////////////////
float Window::GetFrameTime() const
{
    return myLastFrameTime;
}


////////////////////////////////////////////////////////////
/// Change the joystick threshold, ie. the value below which
/// no move event will be generated
////////////////////////////////////////////////////////////
void Window::SetJoystickThreshold(float threshold)
{
    if (myWindow)
        myWindow->SetJoystickThreshold(threshold);
}


////////////////////////////////////////////////////////////
/// Called after the window has been created
////////////////////////////////////////////////////////////
void Window::OnCreate()
{
    // Nothing by default
}


////////////////////////////////////////////////////////////
/// Called before the window has been displayed
////////////////////////////////////////////////////////////
void Window::OnDisplay()
{
    // Nothing by default
}


////////////////////////////////////////////////////////////
/// Receive an event from window
////////////////////////////////////////////////////////////
void Window::OnEvent(const Event& event)
{
    // Discard MouseMove events generated by SetCursorPosition
    if ((event.Type        == Event::MouseMoved) &&
        (event.MouseMove.X == mySetCursorPosX)   &&
        (event.MouseMove.Y == mySetCursorPosY))
    {
        mySetCursorPosX = 0xFFFF;
        mySetCursorPosY = 0xFFFF;
        return;
    }

    myEvents.push(event);
}


////////////////////////////////////////////////////////////
/// Do some common internal initializations
////////////////////////////////////////////////////////////
void Window::Initialize()
{
    // Listen to events from the new window
    myWindow->AddListener(this);
    myWindow->AddListener(&myInput);

    // Setup default behaviours (to get a consistent behaviour across different implementations)
    Show(true);
    UseVerticalSync(false);
    ShowMouseCursor(true);
    EnableKeyRepeat(true);

    // Reset frame time
    myClock.Reset();
    myLastFrameTime = 0.f;

    // Activate the window
    SetActive(true);

    // Notify the derived class
    OnCreate();
}

} // namespace sf

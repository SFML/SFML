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
#include <SFML/Window/XXX/WindowImplXXX.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <GL/gl.h>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Default constructor
/// (creates a dummy window to provide a valid OpenGL context)
////////////////////////////////////////////////////////////
WindowImplXXX::WindowImplXXX()
{
    // Create a dummy window (with the fewest attributes -- it's just to have a valid support for an OpenGL context)

    // Initialize myWidth and myHeight members from base class with the window size

    // Create an OpenGL context in this window and DO NOT make it active
}


////////////////////////////////////////////////////////////
/// Create the window implementation from an existing control
////////////////////////////////////////////////////////////
WindowImplXXX::WindowImplXXX(WindowHandle Handle, WindowSettings& Params)
{
    // Make sure we'll be able to catch all the events of the given window

    // Initialize myWidth and myHeight members from base class with the window size

    // Create an OpenGL context in this window and make it active
}


////////////////////////////////////////////////////////////
/// Create the window implementation
////////////////////////////////////////////////////////////
WindowImplXXX::WindowImplXXX(VideoMode Mode, const std::string& Title, unsigned long WindowStyle, WindowSettings& Params) :
{
    // Create a new window with given size, title and style

    // Initialize myWidth and myHeight members from base class with the window size

    // Create an OpenGL context in this window and make it active
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
WindowImplXXX::~WindowImplXXX()
{
    // Destroy the OpenGL context, the window and every resource allocated by this class
}


////////////////////////////////////////////////////////////
/// Check if there's an active context on the current thread
////////////////////////////////////////////////////////////
bool WindowImplXXX::IsContextActive()
{
    // Should return whether xxxGetCurrentContext() is NULL or not;
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::Display
////////////////////////////////////////////////////////////
void WindowImplXXX::Display()
{
    // Swap OpenGL buffers (should be a call to xxxSwapBuffers)
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::ProcessEvents
////////////////////////////////////////////////////////////
void WindowImplXXX::ProcessEvents()
{
    // Process every event for this window

    // Generate a sf::Event and call SendEvent(Evt) for each event
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetActive
////////////////////////////////////////////////////////////
void WindowImplXXX::SetActive(bool Active) const
{
    // Bind / unbind OpenGL context (should be a call to xxxMakeCurrent)
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::UseVerticalSync
////////////////////////////////////////////////////////////
void WindowImplXXX::UseVerticalSync(bool Enabled)
{
    // Activate / deactivate vertical synchronization
    // usually using an OpenGL extension (should be a call to xxxSwapInterval)
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::ShowMouseCursor
////////////////////////////////////////////////////////////
void WindowImplXXX::ShowMouseCursor(bool Show)
{
    // Show or hide the system cursor in this window
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetCursorPosition
////////////////////////////////////////////////////////////
void WindowImplXXX::SetCursorPosition(unsigned int Left, unsigned int Top)
{
    // Change the cursor position (Left and Top are relative to this window)
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetPosition
////////////////////////////////////////////////////////////
void WindowImplXXX::SetPosition(int Left, int Top)
{
    // Change the window position
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetSize
////////////////////////////////////////////////////////////
void WindowImplWin32::SetSize(unsigned int Width, unsigned int Height)
{
    // Change the window size
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::Show
////////////////////////////////////////////////////////////
void WindowImplXXX::Show(bool State)
{
    // Show or hide the window
}

////////////////////////////////////////////////////////////
/// /see WindowImpl::EnableKeyRepeat
////////////////////////////////////////////////////////////
void WindowImplXXX::EnableKeyRepeat(bool Enabled)
{
    // Enable or disable automatic key-repeat for keydown events
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetIcon
////////////////////////////////////////////////////////////
void WindowImplXXX::SetIcon(unsigned int Width, unsigned int Height, const Uint8* Pixels)
{
    // Change all the necessary icons of the window (titlebar, task bar, ...) with the
    // provided array of 32 bits RGBA pixels
}


/*===========================================================
            STRATEGY FOR OPENGL CONTEXT CREATION

- If the requested level of anti-aliasing is not supported and is greater than 2, try with 2
  --> if level 2 fails, disable anti-aliasing
  --> it's important not to generate an error if anti-aliasing is not supported

- Use a matching pixel mode, or the best of all available pixel modes if no perfect match ;
  You should use the function EvaluateConfig to get a score for a given configuration

- Don't forget to fill Params (see constructors) back with the actual parameters we got from the chosen pixel format

- IMPORTANT : all OpenGL contexts must be shared (usually a call to xxxShareLists)

===========================================================*/


/*===========================================================
               STRATEGY FOR EVENT HANDLING

- Process any event matching with the ones in sf::Event::EventType
  --> Create a sf::Event, fill the members corresponding to the event type
  --> No need to handle joystick events, they are handled by WindowImpl::ProcessJoystickEvents
  --> Event::TextEntered must provide UTF-16 characters
      (see http://www.unicode.org/Public/PROGRAMS/CVTUTF/ for unicode conversions)
  --> Don't forget to process any destroy-like event (ie. when the window is destroyed externally)

- Use SendEvent function from base class to propagate the created events

===========================================================*/


} // namespace priv

} // namespace sf

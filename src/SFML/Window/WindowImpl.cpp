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
#include <SFML/Window/WindowImpl.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <cmath>

#if defined(SFML_SYSTEM_WINDOWS)

    #include <SFML/Window/Win32/WindowImplWin32.hpp>
    typedef sf::priv::WindowImplWin32 WindowImplType;

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD)

    #include <SFML/Window/Linux/WindowImplX11.hpp>
    typedef sf::priv::WindowImplX11 WindowImplType;

#elif defined(SFML_SYSTEM_MACOS)

	#include <SFML/Window/OSX/WindowImplCocoa.hpp>
	typedef sf::priv::WindowImplCocoa WindowImplType;

#endif


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
WindowImpl* WindowImpl::New(VideoMode mode, const std::string& title, unsigned long style)
{
    return new WindowImplType(mode, title, style);
}


////////////////////////////////////////////////////////////
WindowImpl* WindowImpl::New(WindowHandle handle)
{
    return new WindowImplType(handle);
}


////////////////////////////////////////////////////////////
WindowImpl::WindowImpl() :
myWidth       (0),
myHeight      (0),
myJoyThreshold(0.1f)
{
    // Initialize the joysticks
    for (unsigned int i = 0; i < Joy::Count; ++i)
    {
        myJoysticks[i].Initialize(i);
        myJoyStates[i] = myJoysticks[i].UpdateState();
    }
}


////////////////////////////////////////////////////////////
WindowImpl::~WindowImpl()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
unsigned int WindowImpl::GetWidth() const
{
    return myWidth;
}


////////////////////////////////////////////////////////////
unsigned int WindowImpl::GetHeight() const
{
    return myHeight;
}


////////////////////////////////////////////////////////////
void WindowImpl::SetJoystickThreshold(float threshold)
{
    myJoyThreshold = threshold;
}


////////////////////////////////////////////////////////////
bool WindowImpl::PopEvent(Event& event, bool block)
{
    // If the event queue is empty, let's first check if new events are available from the OS
    if (myEvents.empty())
    {
        // Special handling of joystick events (we must use polling)
        ProcessJoystickEvents();

        if (block)
        {
            // If we are blocking, loop until we actually received a SFML event
            // (there may be OS events that make ProcessEvents(true) return, but
            // which don't translate to SFML events)
            while (myEvents.empty())
                ProcessEvents(true);
        }
        else
        {
            // If we are not blocking, just process the pending events
            ProcessEvents(false);
        }
    }

    // Pop the first event of the queue, if it is not empty
    if (!myEvents.empty())
    {
        event = myEvents.front();
        myEvents.pop();

        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
void WindowImpl::PushEvent(const Event& event)
{
    myEvents.push(event);
}


////////////////////////////////////////////////////////////
void WindowImpl::ProcessJoystickEvents()
{
    for (unsigned int i = 0; i < Joy::Count; ++i)
    {
        // Copy the previous state of the joystick and get the new one
        JoystickState previousState = myJoyStates[i];
        myJoyStates[i] = myJoysticks[i].UpdateState();

        // Axis
        for (unsigned int j = 0; j < Joy::AxisCount; ++j)
        {
            Joy::Axis axis = static_cast<Joy::Axis>(j);
            if (myJoysticks[i].HasAxis(axis))
            {
                float prevPos = previousState.Axis[axis];
                float currPos = myJoyStates[i].Axis[axis];
                if (fabs(currPos - prevPos) >= myJoyThreshold)
                {
                    Event event;
                    event.Type               = Event::JoyMoved;
                    event.JoyMove.JoystickId = i;
                    event.JoyMove.Axis       = axis;
                    event.JoyMove.Position   = currPos;
                    PushEvent(event);
                }
            }
        }

        // Buttons
        for (unsigned int j = 0; j < myJoysticks[i].GetButtonsCount(); ++j)
        {
            bool prevPressed = previousState.Buttons[j];
            bool currPressed = myJoyStates[i].Buttons[j];

            if ((!prevPressed && currPressed) || (prevPressed && !currPressed))
            {
                Event event;
                event.Type                 = currPressed ? Event::JoyButtonPressed : Event::JoyButtonReleased;
                event.JoyButton.JoystickId = i;
                event.JoyButton.Button     = j;
                PushEvent(event);
            }
        }
    }
}


} // namespace priv

} // namespace sf

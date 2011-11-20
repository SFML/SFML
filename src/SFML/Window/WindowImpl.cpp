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
#include <SFML/Window/JoystickManager.hpp>
#include <SFML/System/Sleep.hpp>
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
WindowImpl* WindowImpl::New(VideoMode mode, const std::string& title, Uint32 style)
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
    // Get the initial joystick states
    JoystickManager::GetInstance().Update();
    for (unsigned int i = 0; i < Joystick::Count; ++i)
        myJoyStates[i] = JoystickManager::GetInstance().GetState(i);
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
        if (!block)
        {
            // Non-blocking mode: process events and continue
            ProcessJoystickEvents();
            ProcessEvents();
        }
        else
        {
            // Blocking mode: process events until one is triggered

            // Here we use a manual wait loop instead of the optimized
            // wait-event provided by the OS, so that we don't skip joystick
            // events (which require polling)
            while (myEvents.empty())
            {
                ProcessJoystickEvents();
                ProcessEvents();
                Sleep(10);
            }
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
    // First update the global joystick states
    JoystickManager::GetInstance().Update();

    for (unsigned int i = 0; i < Joystick::Count; ++i)
    {
        // Copy the previous state of the joystick and get the new one
        JoystickState previousState = myJoyStates[i];
        myJoyStates[i] = JoystickManager::GetInstance().GetState(i);
        JoystickCaps caps = JoystickManager::GetInstance().GetCapabilities(i);

        // Connection state
        bool connected = myJoyStates[i].Connected;
        if (previousState.Connected ^ connected)
        {
            Event event;
            event.Type = connected ? Event::JoystickConnected : Event::JoystickDisconnected;
            event.JoystickButton.JoystickId = i;
            PushEvent(event);
        }

        if (connected)
        {
            // Axes
            for (unsigned int j = 0; j < Joystick::AxisCount; ++j)
            {
                if (caps.Axes[j])
                {
                    Joystick::Axis axis = static_cast<Joystick::Axis>(j);
                    float prevPos = previousState.Axes[axis];
                    float currPos = myJoyStates[i].Axes[axis];
                    if (fabs(currPos - prevPos) >= myJoyThreshold)
                    {
                        Event event;
                        event.Type = Event::JoystickMoved;
                        event.JoystickMove.JoystickId = i;
                        event.JoystickMove.Axis = axis;
                        event.JoystickMove.Position = currPos;
                        PushEvent(event);
                    }
                }
            }

            // Buttons
            for (unsigned int j = 0; j < caps.ButtonCount; ++j)
            {
                bool prevPressed = previousState.Buttons[j];
                bool currPressed = myJoyStates[i].Buttons[j];

                if (prevPressed ^ currPressed)
                {
                    Event event;
                    event.Type = currPressed ? Event::JoystickButtonPressed : Event::JoystickButtonReleased;
                    event.JoystickButton.JoystickId = i;
                    event.JoystickButton.Button = j;
                    PushEvent(event);
                }
            }
        }
    }
}


} // namespace priv

} // namespace sf

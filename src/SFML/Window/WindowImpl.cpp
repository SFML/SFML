////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2012 Laurent Gomila (laurent.gom@gmail.com)
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
WindowImpl* WindowImpl::create(VideoMode mode, const String& title, Uint32 style)
{
    return new WindowImplType(mode, title, style);
}


////////////////////////////////////////////////////////////
WindowImpl* WindowImpl::create(WindowHandle handle)
{
    return new WindowImplType(handle);
}


////////////////////////////////////////////////////////////
WindowImpl::WindowImpl() :
m_joyThreshold(0.1f)
{
    // Get the initial joystick states
    JoystickManager::getInstance().update();
    for (unsigned int i = 0; i < Joystick::Count; ++i)
        m_joyStates[i] = JoystickManager::getInstance().getState(i);
}


////////////////////////////////////////////////////////////
WindowImpl::~WindowImpl()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
void WindowImpl::setJoystickThreshold(float threshold)
{
    m_joyThreshold = threshold;
}


////////////////////////////////////////////////////////////
bool WindowImpl::popEvent(Event& event, bool block)
{
    // If the event queue is empty, let's first check if new events are available from the OS
    if (m_events.empty())
    {
        if (!block)
        {
            // Non-blocking mode: process events and continue
            processJoystickEvents();
            processEvents();
        }
        else
        {
            // Blocking mode: process events until one is triggered

            // Here we use a manual wait loop instead of the optimized
            // wait-event provided by the OS, so that we don't skip joystick
            // events (which require polling)
            while (m_events.empty())
            {
                processJoystickEvents();
                processEvents();
                sleep(milliseconds(10));
            }
        }
    }

    // Pop the first event of the queue, if it is not empty
    if (!m_events.empty())
    {
        event = m_events.front();
        m_events.pop();

        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
void WindowImpl::pushEvent(const Event& event)
{
    m_events.push(event);
}


////////////////////////////////////////////////////////////
void WindowImpl::processJoystickEvents()
{
    // First update the global joystick states
    JoystickManager::getInstance().update();

    for (unsigned int i = 0; i < Joystick::Count; ++i)
    {
        // Copy the previous state of the joystick and get the new one
        JoystickState previousState = m_joyStates[i];
        m_joyStates[i] = JoystickManager::getInstance().getState(i);
        JoystickCaps caps = JoystickManager::getInstance().getCapabilities(i);

        // Connection state
        bool connected = m_joyStates[i].connected;
        if (previousState.connected ^ connected)
        {
            Event event;
            event.type = connected ? Event::JoystickConnected : Event::JoystickDisconnected;
            event.joystickButton.joystickId = i;
            pushEvent(event);
        }

        if (connected)
        {
            // Axes
            for (unsigned int j = 0; j < Joystick::AxisCount; ++j)
            {
                if (caps.axes[j])
                {
                    Joystick::Axis axis = static_cast<Joystick::Axis>(j);
                    float prevPos = previousState.axes[axis];
                    float currPos = m_joyStates[i].axes[axis];
                    if (fabs(currPos - prevPos) >= m_joyThreshold)
                    {
                        Event event;
                        event.type = Event::JoystickMoved;
                        event.joystickMove.joystickId = i;
                        event.joystickMove.axis = axis;
                        event.joystickMove.position = currPos;
                        pushEvent(event);
                    }
                }
            }

            // Buttons
            for (unsigned int j = 0; j < caps.buttonCount; ++j)
            {
                bool prevPressed = previousState.buttons[j];
                bool currPressed = m_joyStates[i].buttons[j];

                if (prevPressed ^ currPressed)
                {
                    Event event;
                    event.type = currPressed ? Event::JoystickButtonPressed : Event::JoystickButtonReleased;
                    event.joystickButton.joystickId = i;
                    event.joystickButton.button = j;
                    pushEvent(event);
                }
            }
        }
    }
}


} // namespace priv

} // namespace sf

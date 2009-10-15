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
#include <SFML/Window/WindowListener.hpp>
#include <algorithm>
#include <cmath>

#if defined(SFML_SYSTEM_WINDOWS)

    #include <SFML/Window/Win32/WindowImplWin32.hpp>
    typedef sf::priv::WindowImplWin32 WindowImplType;

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD)

    #include <SFML/Window/Linux/WindowImplX11.hpp>
    typedef sf::priv::WindowImplX11 WindowImplType;

#elif defined(SFML_SYSTEM_MACOS)

	#include <SFML/Window/Cocoa/WindowImplCocoa.hpp>
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
    for (unsigned int i = 0; i < JoysticksCount; ++i)
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
void WindowImpl::AddListener(WindowListener* listener)
{
    if (listener)
        myListeners.insert(listener);
}


////////////////////////////////////////////////////////////
void WindowImpl::RemoveListener(WindowListener* listener)
{
    myListeners.erase(listener);
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
void WindowImpl::DoEvents()
{
    // Read the joysticks state and generate the appropriate events
    ProcessJoystickEvents();

    // Let the derived class process other events
    ProcessEvents();
}


////////////////////////////////////////////////////////////
void WindowImpl::SendEvent(const Event& event)
{
    for (std::set<WindowListener*>::iterator i = myListeners.begin(); i != myListeners.end(); ++i)
    {
        (*i)->OnEvent(event);
    }
}


////////////////////////////////////////////////////////////
void WindowImpl::ProcessJoystickEvents()
{
    for (unsigned int i = 0; i < JoysticksCount; ++i)
    {
        // Copy the previous state of the joystick and get the new one
        JoystickState previousState = myJoyStates[i];
        myJoyStates[i] = myJoysticks[i].UpdateState();

        // Axis
        for (unsigned int j = 0; j < myJoysticks[i].GetAxesCount(); ++j)
        {
            float prevPos = previousState.Axis[j];
            float currPos = myJoyStates[i].Axis[j];
            if (fabs(currPos - prevPos) >= myJoyThreshold)
            {
                Event event;
                event.Type               = Event::JoyMoved;
                event.JoyMove.JoystickId = i;
                event.JoyMove.Axis       = static_cast<Joy::Axis>(j);
                event.JoyMove.Position   = currPos;
                SendEvent(event);
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
                SendEvent(event);
            }
        }
    }
}


} // namespace priv

} // namespace sf

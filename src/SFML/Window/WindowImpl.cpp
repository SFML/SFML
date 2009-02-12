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
/// Create a new window depending on the current OS
////////////////////////////////////////////////////////////
WindowImpl* WindowImpl::New()
{
    return new WindowImplType();
}


////////////////////////////////////////////////////////////
/// Create a new window depending on the current OS
////////////////////////////////////////////////////////////
WindowImpl* WindowImpl::New(VideoMode Mode, const std::string& Title, unsigned long WindowStyle, WindowSettings& Params)
{
    return new WindowImplType(Mode, Title, WindowStyle, Params);
}


////////////////////////////////////////////////////////////
/// Create a new window depending on the current OS
////////////////////////////////////////////////////////////
WindowImpl* WindowImpl::New(WindowHandle Handle, WindowSettings& Params)
{
    return new WindowImplType(Handle, Params);
}


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
WindowImpl::WindowImpl() :
myWidth       (0),
myHeight      (0),
myJoyThreshold(0.1f)
{
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
WindowImpl::~WindowImpl()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
/// Add a listener to the window
////////////////////////////////////////////////////////////
void WindowImpl::AddListener(WindowListener* Listener)
{
    if (Listener)
        myListeners.insert(Listener);
}


////////////////////////////////////////////////////////////
/// Remove a listener from the window
////////////////////////////////////////////////////////////
void WindowImpl::RemoveListener(WindowListener* Listener)
{
    myListeners.erase(Listener);
}


////////////////////////////////////////////////////////////
/// Initialize window's states that can't be done at construction
////////////////////////////////////////////////////////////
void WindowImpl::Initialize()
{
    // Initialize the joysticks
    for (unsigned int i = 0; i < JoysticksCount; ++i)
    {
        myJoysticks[i].Initialize(i);
        myJoyStates[i] = myJoysticks[i].UpdateState();
    }
}


////////////////////////////////////////////////////////////
/// Get the client width of the window
////////////////////////////////////////////////////////////
unsigned int WindowImpl::GetWidth() const
{
    return myWidth;
}


////////////////////////////////////////////////////////////
/// Get the client height of the window
////////////////////////////////////////////////////////////
unsigned int WindowImpl::GetHeight() const
{
    return myHeight;
}


////////////////////////////////////////////////////////////
/// Change the joystick threshold, ie. the value below which
/// no move event will be generated
////////////////////////////////////////////////////////////
void WindowImpl::SetJoystickThreshold(float Threshold)
{
    myJoyThreshold = Threshold;
}


////////////////////////////////////////////////////////////
/// Process incoming events from operating system
////////////////////////////////////////////////////////////
void WindowImpl::DoEvents()
{
    // Read the joysticks state and generate the appropriate events
    ProcessJoystickEvents();

    // Let the derived class process other events
    ProcessEvents();
}


////////////////////////////////////////////////////////////
/// Check if there's an active context on the current thread
////////////////////////////////////////////////////////////
bool WindowImpl::IsContextActive()
{
    return WindowImplType::IsContextActive();
}


////////////////////////////////////////////////////////////
/// Send an event to listeners
////////////////////////////////////////////////////////////
void WindowImpl::SendEvent(const Event& EventToSend)
{
    for (std::set<WindowListener*>::iterator i = myListeners.begin(); i != myListeners.end(); ++i)
    {
        (*i)->OnEvent(EventToSend);
    }
}


////////////////////////////////////////////////////////////
/// Evaluate a pixel format configuration.
/// This functions can be used by implementations that have
/// several valid formats and want to get the best one
////////////////////////////////////////////////////////////
int WindowImpl::EvaluateConfig(const VideoMode& Mode, const WindowSettings& Settings, int ColorBits, int DepthBits, int StencilBits, int Antialiasing)
{
    return abs(static_cast<int>(Mode.BitsPerPixel          - ColorBits))   +
           abs(static_cast<int>(Settings.DepthBits         - DepthBits))   +
           abs(static_cast<int>(Settings.StencilBits       - StencilBits)) +
           abs(static_cast<int>(Settings.AntialiasingLevel - Antialiasing));
}


////////////////////////////////////////////////////////////
/// Read the joysticks state and generate the appropriate events
////////////////////////////////////////////////////////////
void WindowImpl::ProcessJoystickEvents()
{
    for (unsigned int i = 0; i < JoysticksCount; ++i)
    {
        // Copy the previous state of the joystick and get the new one
        JoystickState PreviousState = myJoyStates[i];
        myJoyStates[i] = myJoysticks[i].UpdateState();

        // Axis
        for (unsigned int j = 0; j < myJoysticks[i].GetAxesCount(); ++j)
        {
            float PrevPos = PreviousState.Axis[j];
            float CurrPos = myJoyStates[i].Axis[j];
            if (fabs(CurrPos - PrevPos) >= myJoyThreshold)
            {
                Event Event;
                Event.Type               = Event::JoyMoved;
                Event.JoyMove.JoystickId = i;
                Event.JoyMove.Axis       = static_cast<Joy::Axis>(j);
                Event.JoyMove.Position   = CurrPos;
                SendEvent(Event);
            }
        }

        // Buttons
        for (unsigned int j = 0; j < myJoysticks[i].GetButtonsCount(); ++j)
        {
            bool PrevPressed = PreviousState.Buttons[j];
            bool CurrPressed = myJoyStates[i].Buttons[j];

            if ((!PrevPressed && CurrPressed) || (PrevPressed && !CurrPressed))
            {
                Event Event;
                Event.Type                 = CurrPressed ? Event::JoyButtonPressed : Event::JoyButtonReleased;
                Event.JoyButton.JoystickId = i;
                Event.JoyButton.Button     = j;
                SendEvent(Event);
            }
        }
    }
}


} // namespace priv

} // namespace sf

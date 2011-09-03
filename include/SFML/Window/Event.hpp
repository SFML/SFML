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

#ifndef SFML_EVENT_HPP
#define SFML_EVENT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Defines a system event and its parameters
///
////////////////////////////////////////////////////////////
class Event
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Size events parameters (Resized)
    ///
    ////////////////////////////////////////////////////////////
    struct SizeEvent
    {
        unsigned int Width;  ///< New width, in pixels
        unsigned int Height; ///< New height, in pixels
    };

    ////////////////////////////////////////////////////////////
    /// \brief Keyboard event parameters (KeyPressed, KeyReleased)
    ///
    ////////////////////////////////////////////////////////////
    struct KeyEvent
    {
        Keyboard::Key Code;    ///< Code of the key that has been pressed
        bool          Alt;     ///< Is the Alt key pressed?
        bool          Control; ///< Is the Control key pressed?
        bool          Shift;   ///< Is the Shift key pressed?
        bool          System;  ///< Is the System key pressed?
    };

    ////////////////////////////////////////////////////////////
    /// \brief Text event parameters (TextEntered)
    ///
    ////////////////////////////////////////////////////////////
    struct TextEvent
    {
        Uint32 Unicode; ///< UTF-32 unicode value of the character
    };

    ////////////////////////////////////////////////////////////
    /// \brief Mouse move event parameters (MouseMoved)
    ///
    ////////////////////////////////////////////////////////////
    struct MouseMoveEvent
    {
        int X; ///< X position of the mouse pointer, relative to the left of the owner window
        int Y; ///< Y position of the mouse pointer, relative to the top of the owner window
    };

    ////////////////////////////////////////////////////////////
    /// \brief Mouse buttons events parameters
    ///        (MouseButtonPressed, MouseButtonReleased)
    ///
    ////////////////////////////////////////////////////////////
    struct MouseButtonEvent
    {
        Mouse::Button Button; ///< Code of the button that has been pressed
        int           X;      ///< X position of the mouse pointer, relative to the left of the owner window
        int           Y;      ///< Y position of the mouse pointer, relative to the top of the owner window
    };

    ////////////////////////////////////////////////////////////
    /// \brief Mouse wheel events parameters (MouseWheelMoved)
    ///
    ////////////////////////////////////////////////////////////
    struct MouseWheelEvent
    {
        int Delta; ///< Number of ticks the wheel has moved (positive is up, negative is down)
        int X;     ///< X position of the mouse pointer, relative to the left of the owner window
        int Y;     ///< Y position of the mouse pointer, relative to the top of the owner window
    };

    ////////////////////////////////////////////////////////////
    /// \brief Joystick connection events parameters
    ///        (JoystickConnected, JoystickDisconnected)
    ///
    ////////////////////////////////////////////////////////////
    struct JoystickConnectEvent
    {
        unsigned int JoystickId; ///< Index of the joystick (in range [0 .. Joystick::Count - 1])
    };

    ////////////////////////////////////////////////////////////
    /// \brief Joystick axis move event parameters (JoystickMoved)
    ///
    ////////////////////////////////////////////////////////////
    struct JoystickMoveEvent
    {
        unsigned int   JoystickId; ///< Index of the joystick (in range [0 .. Joystick::Count - 1])
        Joystick::Axis Axis;       ///< Axis on which the joystick moved
        float          Position;   ///< New position on the axis (in range [-100 .. 100])
    };

    ////////////////////////////////////////////////////////////
    /// \brief Joystick buttons events parameters
    ///        (JoystickButtonPressed, JoystickButtonReleased)
    ///
    ////////////////////////////////////////////////////////////
    struct JoystickButtonEvent
    {
        unsigned int JoystickId; ///< Index of the joystick (in range [0 .. Joystick::Count - 1])
        unsigned int Button;     ///< Index of the button that has been pressed (in range [0 .. Joystick::ButtonCount - 1])
    };

    ////////////////////////////////////////////////////////////
    /// \brief Enumeration of the different types of events
    ///
    ////////////////////////////////////////////////////////////
    enum EventType
    {
        Closed,                 ///< The window requested to be closed
        Resized,                ///< The window was resized
        LostFocus,              ///< The window lost the focus
        GainedFocus,            ///< The window gained the focus
        TextEntered,            ///< A character was entered
        KeyPressed,             ///< A key was pressed
        KeyReleased,            ///< A key was released
        MouseWheelMoved,        ///< The mouse wheel was scrolled
        MouseButtonPressed,     ///< A mouse button was pressed
        MouseButtonReleased,    ///< A mouse button was released
        MouseMoved,             ///< The mouse cursor moved
        MouseEntered,           ///< The mouse cursor entered the area of the window
        MouseLeft,              ///< The mouse cursor left the area of the window
        JoystickButtonPressed,  ///< A joystick button was pressed
        JoystickButtonReleased, ///< A joystick button was released
        JoystickMoved,          ///< The joystick moved along an axis
        JoystickConnected,      ///< A joystick was connected
        JoystickDisconnected,   ///< A joystick was disconnected

        Count                   ///< Keep last -- the total number of event types
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    EventType Type; ///< Type of the event

    union
    {
        SizeEvent            Size;            ///< Size event parameters
        KeyEvent             Key;             ///< Key event parameters
        TextEvent            Text;            ///< Text event parameters
        MouseMoveEvent       MouseMove;       ///< Mouse move event parameters
        MouseButtonEvent     MouseButton;     ///< Mouse button event parameters
        MouseWheelEvent      MouseWheel;      ///< Mouse wheel event parameters
        JoystickMoveEvent    JoystickMove;    ///< Joystick move event parameters
        JoystickButtonEvent  JoystickButton;  ///< Joystick button event parameters
        JoystickConnectEvent JoystickConnect; ///< Joystick (dis)connect event parameters
    };
};

} // namespace sf


#endif // SFML_EVENT_HPP


////////////////////////////////////////////////////////////
/// \class sf::Event
/// \ingroup window
///
/// sf::Event holds all the informations about a system event
/// that just happened. Events are retrieved using the
/// sf::Window::PollEvent and sf::Window::WaitEvent functions.
///
/// A sf::Event instance contains the type of the event
/// (mouse moved, key pressed, window closed, ...) as well
/// as the details about this particular event. Please note that
/// the event parameters are defined in a union, which means that
/// only the member matching the type of the event will be properly
/// filled; all other members will have undefined values and must not
/// be read if the type of the event doesn't match. For example,
/// if you received a KeyPressed event, then you must read the
/// event.Key member, all other members such as event.MouseMove
/// or event.Text will have undefined values.
///
/// Usage example:
/// \code
/// sf::Event event;
/// while (window.PollEvent(event))
/// {
///     // Request for closing the window
///     if (event.Type == sf::Event::Closed)
///         window.Close();
///
///     // The escape key was pressed
///     if ((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Keyboard::Escape))
///         window.Close();
///
///     // The window was resized
///     if (event.Type == sf::Event::Resized)
///         DoSomethingWithTheNewSize(event.Size.Width, event.Size.Height);
///
///     // etc ...
/// }
/// \endcode
///
////////////////////////////////////////////////////////////

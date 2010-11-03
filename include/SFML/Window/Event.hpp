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


namespace sf
{
namespace Key
{
    ////////////////////////////////////////////////////////////
    /// \ingroup window
    /// \brief Definition of key codes for keyboard events
    ///
    ////////////////////////////////////////////////////////////
    enum Code
    {
        A = 'a',      ///< The A key
        B = 'b',      ///< The B key
        C = 'c',      ///< The C key
        D = 'd',      ///< The D key
        E = 'e',      ///< The E key
        F = 'f',      ///< The F key
        G = 'g',      ///< The G key
        H = 'h',      ///< The H key
        I = 'i',      ///< The I key
        J = 'j',      ///< The J key
        K = 'k',      ///< The K key
        L = 'l',      ///< The L key
        M = 'm',      ///< The M key
        N = 'n',      ///< The N key
        O = 'o',      ///< The O key
        P = 'p',      ///< The P key
        Q = 'q',      ///< The Q key
        R = 'r',      ///< The R key
        S = 's',      ///< The S key
        T = 't',      ///< The T key
        U = 'u',      ///< The U key
        V = 'v',      ///< The V key
        W = 'w',      ///< The W key
        X = 'x',      ///< The X key
        Y = 'y',      ///< The Y key
        Z = 'z',      ///< The Z key
        Num0 = '0',   ///< The 0 key
        Num1 = '1',   ///< The 1 key
        Num2 = '2',   ///< The 2 key
        Num3 = '3',   ///< The 3 key
        Num4 = '4',   ///< The 4 key
        Num5 = '5',   ///< The 5 key
        Num6 = '6',   ///< The 6 key
        Num7 = '7',   ///< The 7 key
        Num8 = '8',   ///< The 8 key
        Num9 = '9',   ///< The 9 key
        Escape = 256, ///< The Escape key
        LControl,     ///< The left Control key
        LShift,       ///< The left Shift key
        LAlt,         ///< The left Alt key
        LSystem,      ///< The left OS specific key : windows (Windows and Linux), apple (MacOS X), ...
        RControl,     ///< The right Control key
        RShift,       ///< The right Shift key
        RAlt,         ///< The right Alt key
        RSystem,      ///< The right OS specific key : windows (Windows and Linux), apple (MacOS X), ...
        Menu,         ///< The Menu key
        LBracket,     ///< The [ key
        RBracket,     ///< The ] key
        SemiColon,    ///< The ; key
        Comma,        ///< The , key
        Period,       ///< The . key
        Quote,        ///< The ' key
        Slash,        ///< The / key
        BackSlash,    ///< The \ key
        Tilde,        ///< The ~ key
        Equal,        ///< The = key
        Dash,         ///< The - key
        Space,        ///< The Space key
        Return,       ///< The Return key
        Back,         ///< The Backspace key
        Tab,          ///< The Tabulation key
        PageUp,       ///< The Page up key
        PageDown,     ///< The Page down key
        End,          ///< The End key
        Home,         ///< The Home key
        Insert,       ///< The Insert key
        Delete,       ///< The Delete key
        Add,          ///< +
        Subtract,     ///< -
        Multiply,     ///< *
        Divide,       ///< /
        Left,         ///< Left arrow
        Right,        ///< Right arrow
        Up,           ///< Up arrow
        Down,         ///< Down arrow
        Numpad0,      ///< The numpad 0 key
        Numpad1,      ///< The numpad 1 key
        Numpad2,      ///< The numpad 2 key
        Numpad3,      ///< The numpad 3 key
        Numpad4,      ///< The numpad 4 key
        Numpad5,      ///< The numpad 5 key
        Numpad6,      ///< The numpad 6 key
        Numpad7,      ///< The numpad 7 key
        Numpad8,      ///< The numpad 8 key
        Numpad9,      ///< The numpad 9 key
        F1,           ///< The F1 key
        F2,           ///< The F2 key
        F3,           ///< The F3 key
        F4,           ///< The F4 key
        F5,           ///< The F5 key
        F6,           ///< The F6 key
        F7,           ///< The F7 key
        F8,           ///< The F8 key
        F9,           ///< The F8 key
        F10,          ///< The F10 key
        F11,          ///< The F11 key
        F12,          ///< The F12 key
        F13,          ///< The F13 key
        F14,          ///< The F14 key
        F15,          ///< The F15 key
        Pause,        ///< The Pause key

        Count         ///< Keep last -- the total number of keyboard keys
    };
}


namespace Mouse
{
    ////////////////////////////////////////////////////////////
    /// \ingroup window
    /// \brief Definition of button codes for mouse events
    ///
    ////////////////////////////////////////////////////////////
    enum Button
    {
        Left,     ///< The left mouse button
        Right,    ///< The right mouse button
        Middle,   ///< The middle (wheel) mouse button
        XButton1, ///< The first extra mouse button
        XButton2, ///< The second extra mouse button

        ButtonCount     ///< Keep last -- the total number of mouse buttons
    };
}


namespace Joy
{
    ////////////////////////////////////////////////////////////
    /// \ingroup window
    /// \brief Definition of joystick axis for joystick events
    ///
    ////////////////////////////////////////////////////////////
    enum Axis
    {
        AxisX,   ///< The X axis
        AxisY,   ///< The Y axis
        AxisZ,   ///< The Z axis
        AxisR,   ///< The R axis
        AxisU,   ///< The U axis
        AxisV,   ///< The V axis
        AxisPOV, ///< The Point-Of-View axis (hat)

        AxisCount // Keep last -- total number of joystick axis
    };

    enum
    {
        Count       = 8, ///< Total number of supported joysticks
        ButtonCount = 32 ///< Total number of supported joystick buttons
    };
}


////////////////////////////////////////////////////////////
/// \brief Defines a system event and its parameters
///
////////////////////////////////////////////////////////////
class Event
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Keyboard event parameters (KeyPressed, KeyReleased)
    ///
    ////////////////////////////////////////////////////////////
    struct KeyEvent
    {
        Key::Code Code;    ///< Code of the key that has been pressed
        bool      Alt;     ///< Is the Alt key pressed?
        bool      Control; ///< Is the Control key pressed?
        bool      Shift;   ///< Is the Shift key pressed?
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
    /// \brief Joystick axis move event parameters (JoyMoved)
    ///
    ////////////////////////////////////////////////////////////
    struct JoyMoveEvent
    {
        unsigned int JoystickId; ///< Index of the joystick (0 or 1)
        Joy::Axis    Axis;       ///< Axis on which the joystick moved
        float        Position;   ///< New position on the axis (in range [-100, 100])
    };

    ////////////////////////////////////////////////////////////
    /// \brief Joystick buttons events parameters
    ///        (JoyButtonPressed, JoyButtonReleased)
    ///
    ////////////////////////////////////////////////////////////
    struct JoyButtonEvent
    {
        unsigned int JoystickId; ///< Index of the joystick (0 or 1)
        unsigned int Button;     ///< Index of the button that has been pressed
    };

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
    /// \brief Enumeration of the different types of events
    ///
    ////////////////////////////////////////////////////////////
    enum EventType
    {
        Closed,              ///< The window requested to be closed
        Resized,             ///< The window was resized
        LostFocus,           ///< The window lost the focus
        GainedFocus,         ///< The window gained the focus
        TextEntered,         ///< A character was entered
        KeyPressed,          ///< A key was pressed
        KeyReleased,         ///< A key was released
        MouseWheelMoved,     ///< The mouse wheel was scrolled
        MouseButtonPressed,  ///< A mouse button was pressed
        MouseButtonReleased, ///< A mouse button was released
        MouseMoved,          ///< The mouse cursor moved
        MouseEntered,        ///< The mouse cursor entered the area of the window
        MouseLeft,           ///< The mouse cursor left the area of the window
        JoyButtonPressed,    ///< A joystick button was pressed
        JoyButtonReleased,   ///< A joystick button was released
        JoyMoved,            ///< The joystick moved along an axis

        Count                ///< Keep last -- the total number of event types
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    EventType Type; ///< Type of the event

    union
    {
        KeyEvent         Key;         ///< Key event parameters
        TextEvent        Text;        ///< Text event parameters
        MouseMoveEvent   MouseMove;   ///< Mouse move event parameters
        MouseButtonEvent MouseButton; ///< Mouse button event parameters
        MouseWheelEvent  MouseWheel;  ///< Mouse wheel event parameters
        JoyMoveEvent     JoyMove;     ///< Joystick move event parameters
        JoyButtonEvent   JoyButton;   ///< Joystick button event parameters
        SizeEvent        Size;        ///< Size event parameters
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
/// sf::Window::GetEvent function.
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
/// while (window.GetEvent(event))
/// {
///     // Request for closing the window
///     if (event.Type == sf::Event::Closed)
///         window.Close();
///
///     // The escape key was pressed
///     if ((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Key::Escape))
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

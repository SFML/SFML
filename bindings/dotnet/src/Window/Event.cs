using System;
using System.Runtime.InteropServices;

namespace SFML
{
    namespace Window
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Definition of key codes for keyboard events
        /// </summary>
        ////////////////////////////////////////////////////////////
        public enum KeyCode
        {
            A = 'a',
            B = 'b',
            C = 'c',
            D = 'd',
            E = 'e',
            F = 'f',
            G = 'g',
            H = 'h',
            I = 'i',
            J = 'j',
            K = 'k',
            L = 'l',
            M = 'm',
            N = 'n',
            O = 'o',
            P = 'p',
            Q = 'q',
            R = 'r',
            S = 's',
            T = 't',
            U = 'u',
            V = 'v',
            W = 'w',
            X = 'x',
            Y = 'y',
            Z = 'z',
            Num0 = '0',
            Num1 = '1',
            Num2 = '2',
            Num3 = '3',
            Num4 = '4',
            Num5 = '5',
            Num6 = '6',
            Num7 = '7',
            Num8 = '8',
            Num9 = '9', 
            Escape = 256,
            LControl,
            LShift,
            LAlt,
            LSystem,      // OS specific key (left side) : windows (Win and Linux), apple (MacOS), ...
            RControl,
            RShift,
            RAlt,
            RSystem,      // OS specific key (right side) : windows (Win and Linux), apple (MacOS), ...
            Menu,
            LBracket,     // [
            RBracket,     // ]
            SemiColon,    // ;
            Comma,        // ,
            Period,       // .
            Quote,        // '
            Slash,        // /
            BackSlash,
            Tilde,        // ~
            Equal,        // =
            Dash,         // -
            Space,
            Return,
            Back,
            Tab,
            PageUp,
            PageDown,
            End,
            Home,
            Insert,
            Delete,
            Add,          // +
            Subtract,     // -
            Multiply,     // *
            Divide,       // /
            Left,         // Left arrow
            Right,        // Right arrow
            Up,           // Up arrow
            Down,         // Down arrow
            Numpad0,
            Numpad1,
            Numpad2,
            Numpad3,
            Numpad4,
            Numpad5,
            Numpad6,
            Numpad7,
            Numpad8,
            Numpad9,
            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,
            F13,
            F14,
            F15,
            Pause
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Definition of button codes for mouse events
        /// </summary>
        ////////////////////////////////////////////////////////////
        public enum MouseButton
        {
            /// <summary>Left mouse button</summary>
            Left,

            /// <summary>Right mouse button</summary>
            Right,

            /// <summary>Center (wheel) mouse button</summary>
            Middle,

            /// <summary>First extra button</summary>
            XButton1,

            /// <summary>Second extra button</summary>
            XButton2
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Definition of joystick axis for joystick events
        /// </summary>
        ////////////////////////////////////////////////////////////
        public enum JoyAxis
        {
            /// <summary>X axis</summary>
            AxisX,

            /// <summary>Y axis</summary>
            AxisY,

            /// <summary>Z axis</summary>
            AxisZ,

            /// <summary>R axis</summary>
            AxisR,

            /// <summary>U axis</summary>
            AxisU,

            /// <summary>V axis</summary>
            AxisV,

            /// <summary>Point of view</summary>
            AxisPOV
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Enumeration of the different types of events
        /// </summary>
        ////////////////////////////////////////////////////////////
        public enum EventType
        {
            /// <summary>Event triggered when a window is manually closed</summary>
            Closed,

            /// <summary>Event triggered when a window is resized</summary>
            Resized,

            /// <summary>Event triggered when a window loses the focus</summary>
            LostFocus,

            /// <summary>Event triggered when a window gains the focus</summary>
            GainedFocus,

            /// <summary>Event triggered when a valid character is entered</summary>
            TextEntered,

            /// <summary>Event triggered when a keyboard key is pressed</summary>
            KeyPressed,

            /// <summary>Event triggered when a keyboard key is released</summary>
            KeyReleased,

            /// <summary>Event triggered when the mouse wheel is scrolled</summary>
            MouseWheelMoved,

            /// <summary>Event triggered when a mouse button is pressed</summary>
            MouseButtonPressed,

            /// <summary>Event triggered when a mouse button is released</summary>
            MouseButtonReleased,

            /// <summary>Event triggered when the mouse moves within the area of a window</summary>
            MouseMoved,

            /// <summary>Event triggered when the mouse enters the area of a window</summary>
            MouseEntered,

            /// <summary>Event triggered when the mouse leaves the area of a window</summary>
            MouseLeft,

            /// <summary>Event triggered when a joystick button is pressed</summary>
            JoyButtonPressed,

            /// <summary>Event triggered when a joystick button is released</summary>
            JoyButtonReleased,

            /// <summary>Event triggered when a joystick axis moves</summary>
            JoyMoved
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Keyboard event parameters
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Sequential)]
        public struct KeyEvent
        {
            /// <summary>Code of the key (see KeyCode enum)</summary>
            public KeyCode Code;

            /// <summary>Is the Alt modifier pressed?</summary>
            public int Alt;

            /// <summary>Is the Control modifier pressed?</summary>
            public int Control;

            /// <summary>Is the Shift modifier pressed?</summary>
            public int Shift;
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Text event parameters
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Sequential)]
        public struct TextEvent
        {
            /// <summary>UTF-32 value of the character</summary>
            public uint Unicode;
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Mouse move event parameters
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Sequential)]
        public struct MouseMoveEvent
        {
            /// <summary>X coordinate of the mouse cursor</summary>
            public int X;

            /// <summary>Y coordinate of the mouse cursor</summary>
            public int Y;
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Mouse buttons event parameters
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Sequential)]
        public struct MouseButtonEvent
        {
            /// <summary>Code of the button (see MouseButton enum)</summary>
            public MouseButton Button;

            /// <summary>X coordinate of the mouse cursor</summary>
            public int X;

            /// <summary>Y coordinate of the mouse cursor</summary>
            public int Y;
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Mouse wheel event parameters
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Sequential)]
        public struct MouseWheelEvent
        {
            /// <summary>Scroll amount</summary>
            public int Delta;

            /// <summary>X coordinate of the mouse cursor</summary>
            public int X;

            /// <summary>Y coordinate of the mouse cursor</summary>
            public int Y;
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Joystick axis move event parameters
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Sequential)]
        public struct JoyMoveEvent
        {
            /// <summary>Index of the joystick which triggered the event</summary>
            public uint JoystickId;

            /// <summary>Joystick axis (see JoyAxis enum)</summary>
            public JoyAxis Axis;

            /// <summary>Current position of the axis</summary>
            public float Position;
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Joystick buttons event parameters
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Sequential)]
        public struct JoyButtonEvent
        {
            /// <summary>Index of the joystick which triggered the event</summary>
            public uint JoystickId;

            /// <summary>Index of the button</summary>
            public uint Button;
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Size event parameters
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Sequential)]
        public struct SizeEvent
        {
            /// <summary>New width of the window</summary>
            public uint Width;

            /// <summary>New height of the window</summary>
            public uint Height;
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Event defines a system event and its parameters
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Explicit, Size = 20)]
        public struct Event
        {
            /// <summary>Type of event (see EventType enum)</summary>
            [FieldOffset(0)]
            public EventType Type;

            /// <summary>Arguments for key events (KeyPressed, KeyReleased)</summary>
            [FieldOffset(4)]
            public KeyEvent Key;

            /// <summary>Arguments for text events (TextEntered)</summary>
            [FieldOffset(4)]
            public TextEvent Text;

            /// <summary>Arguments for mouse move events (MouseMoved)</summary>
            [FieldOffset(4)]
            public MouseMoveEvent MouseMove;

            /// <summary>Arguments for mouse button events (MouseButtonPressed, MouseButtonReleased)</summary>
            [FieldOffset(4)]
            public MouseButtonEvent MouseButton;

            /// <summary>Arguments for mouse wheel events (MouseWheelMoved)</summary>
            [FieldOffset(4)]
            public MouseWheelEvent MouseWheel;

            /// <summary>Arguments for joystick axis events (JoyMoved)</summary>
            [FieldOffset(4)]
            public JoyMoveEvent JoyMove;

            /// <summary>Arguments for joystick button events (JoyButtonPressed, JoyButtonReleased)</summary>
            [FieldOffset(4)]
            public JoyButtonEvent JoyButton;

            /// <summary>Arguments for size events (Resized)</summary>
            [FieldOffset(4)]
            public SizeEvent Size;
        }
    }
}

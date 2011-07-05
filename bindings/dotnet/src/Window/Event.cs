using System;
using System.Runtime.InteropServices;

namespace SFML
{
    namespace Window
    {
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
            JoystickButtonPressed,

            /// <summary>Event triggered when a joystick button is released</summary>
            JoystickButtonReleased,

            /// <summary>Event triggered when a joystick axis moves</summary>
            JoystickMoved,

            /// <summary>Event triggered when a joystick is connected</summary>
            JoystickConnected,

            /// <summary>Event triggered when a joystick is disconnected</summary>
            JoystickDisconnected
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
            public Keyboard.Key Code;

            /// <summary>Is the Alt modifier pressed?</summary>
            public int Alt;

            /// <summary>Is the Control modifier pressed?</summary>
            public int Control;

            /// <summary>Is the Shift modifier pressed?</summary>
            public int Shift;

            /// <summary>Is the System modifier pressed?</summary>
            public int System;
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
            public Mouse.Button Button;

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
        public struct JoystickMoveEvent
        {
            /// <summary>Index of the joystick which triggered the event</summary>
            public uint JoystickId;

            /// <summary>Joystick axis (see JoyAxis enum)</summary>
            public Joystick.Axis Axis;

            /// <summary>Current position of the axis</summary>
            public float Position;
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Joystick buttons event parameters
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Sequential)]
        public struct JoystickButtonEvent
        {
            /// <summary>Index of the joystick which triggered the event</summary>
            public uint JoystickId;

            /// <summary>Index of the button</summary>
            public uint Button;
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Joystick connect event parameters
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Sequential)]
        public struct JoystickConnectEvent
        {
            /// <summary>Index of the joystick which triggered the event</summary>
            public uint JoystickId;
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

            /// <summary>Arguments for size events (Resized)</summary>
            [FieldOffset(4)]
            public SizeEvent Size;

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

            /// <summary>Arguments for joystick axis events (JoystickMoved)</summary>
            [FieldOffset(4)]
            public JoystickMoveEvent JoystickMove;

            /// <summary>Arguments for joystick button events (JoystickButtonPressed, JoystickButtonReleased)</summary>
            [FieldOffset(4)]
            public JoystickButtonEvent JoystickButton;

            /// <summary>Arguments for joystick connect events (JoystickConnected, JoystickDisconnected)</summary>
            [FieldOffset(4)]
            public JoystickConnectEvent JoystickConnect;
        }
    }
}

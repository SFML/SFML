using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Security;

namespace SFML
{
    namespace Window
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Enumeration of window creation styles
        /// </summary>
        ////////////////////////////////////////////////////////////
        [Flags]
        public enum Styles
        {
            /// <summary>No border / title bar (this flag and all others are mutually exclusive)</summary>
            None = 0,

            /// <summary>Title bar + fixed border</summary>
            Titlebar = 1 << 0,

            /// <summary>Titlebar + resizable border + maximize button</summary>
            Resize = 1 << 1,

            /// <summary>Titlebar + close button</summary>
            Close = 1 << 2,

            /// <summary>Fullscreen mode (this flag and all others are mutually exclusive))</summary>
            Fullscreen = 1 << 3,

            /// <summary>Default window style (titlebar + resize + close)</summary>
            Default = Titlebar | Resize | Close
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Window is a rendering window ; it can create a new window
        /// or connect to an existing one
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class Window : ObjectBase
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create the window with default style and creation settings
            /// </summary>
            /// <param name="mode">Video mode to use</param>
            /// <param name="title">Title of the window</param>
            ////////////////////////////////////////////////////////////
            public Window(VideoMode mode, string title) :
                this(mode, title, Styles.Default, new ContextSettings(24, 8))
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create the window with default creation settings
            /// </summary>
            /// <param name="mode">Video mode to use</param>
            /// <param name="title">Title of the window</param>
            /// <param name="style">Window style (Resize | Close by default)</param>
            ////////////////////////////////////////////////////////////
            public Window(VideoMode mode, string title, Styles style) :
                this(mode, title, style, new ContextSettings(24, 8))
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create the window
            /// </summary>
            /// <param name="mode">Video mode to use</param>
            /// <param name="title">Title of the window</param>
            /// <param name="style">Window style (Resize | Close by default)</param>
            /// <param name="settings">Creation parameters</param>
            ////////////////////////////////////////////////////////////
            public Window(VideoMode mode, string title, Styles style, ContextSettings settings) :
                base(sfWindow_Create(mode, title, style, ref settings))
            {
                myInput = new Input(sfWindow_GetInput(This));
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create the window from an existing control with default creation settings
            /// </summary>
            /// <param name="handle">Platform-specific handle of the control</param>
            ////////////////////////////////////////////////////////////
            public Window(IntPtr handle) :
                this(handle, new ContextSettings(24, 8))
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create the window from an existing control
            /// </summary>
            /// <param name="Handle">Platform-specific handle of the control</param>
            /// <param name="settings">Creation parameters</param>
            ////////////////////////////////////////////////////////////
            public Window(IntPtr Handle, ContextSettings settings) :
                base(sfWindow_CreateFromHandle(Handle, ref settings))
            {
                myInput = new Input(sfWindow_GetInput(This));
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Input manager of the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            public Input Input
            {
                get {return myInput;}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Tell whether or not the window is opened (ie. has been created).
            /// Note that a hidden window (Show(false))
            /// will still return true
            /// </summary>
            /// <returns>True if the window is opened</returns>
            ////////////////////////////////////////////////////////////
            public virtual bool IsOpened()
            {
                return sfWindow_IsOpened(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Close (destroy) the window.
            /// The Window instance remains valid and you can call
            /// Create to recreate the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            public virtual void Close()
            {
                sfWindow_Close(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Display the window on screen
            /// </summary>
            ////////////////////////////////////////////////////////////
            public virtual void Display()
            {
                sfWindow_Display(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Width of the rendering region of the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            public virtual uint Width
            {
                get {return sfWindow_GetWidth(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Height of the rendering region of the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            public virtual uint Height
            {
                get {return sfWindow_GetHeight(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Creation settings of the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            public virtual ContextSettings Settings
            {
                get {return sfWindow_GetSettings(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Enable / disable vertical synchronization
            /// </summary>
            /// <param name="enable">True to enable v-sync, false to deactivate</param>
            ////////////////////////////////////////////////////////////
            public virtual void EnableVerticalSync(bool enable)
            {
                sfWindow_EnableVerticalSync(This, enable);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Show or hide the mouse cursor
            /// </summary>
            /// <param name="show">True to show, false to hide</param>
            ////////////////////////////////////////////////////////////
            public virtual void ShowMouseCursor(bool show)
            {
                sfWindow_ShowMouseCursor(This, show);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change the position of the mouse cursor
            /// </summary>
            /// <param name="x">Left coordinate of the cursor, relative to the window</param>
            /// <param name="y">Top coordinate of the cursor, relative to the window</param>
            ////////////////////////////////////////////////////////////
            public virtual void SetCursorPosition(uint x, uint y)
            {
                sfWindow_SetCursorPosition(This, x, y);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change the position of the window on screen.
            /// Only works for top-level windows
            /// </summary>
            /// <param name="x">Left position</param>
            /// <param name="y">Top position</param>
            ////////////////////////////////////////////////////////////
            public virtual void SetPosition(int x, int y)
            {
                sfWindow_SetPosition(This, x, y);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change the size of the rendering region of the window
            /// </summary>
            /// <param name="width">New width</param>
            /// <param name="height">New height</param>
            ////////////////////////////////////////////////////////////
            public virtual void SetSize(uint width, uint height)
            {
                sfWindow_SetSize(This, width, height);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Show or hide the window
            /// </summary>
            /// <param name="show">True to show, false to hide</param>
            ////////////////////////////////////////////////////////////
            public virtual void Show(bool show)
            {
                sfWindow_Show(This, show);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Enable or disable automatic key-repeat.
            /// Automatic key-repeat is enabled by default
            /// </summary>
            /// <param name="enable">True to enable, false to disable</param>
            ////////////////////////////////////////////////////////////
            public virtual void EnableKeyRepeat(bool enable)
            {
                sfWindow_EnableKeyRepeat(This, enable);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change the window's icon
            /// </summary>
            /// <param name="width">Icon's width, in pixels</param>
            /// <param name="height">Icon's height, in pixels</param>
            /// <param name="pixels">Array of pixels, format must be RGBA 32 bits</param>
            ////////////////////////////////////////////////////////////
            public virtual void SetIcon(uint width, uint height, byte[] pixels)
            {
                unsafe
                {
                    fixed (byte* PixelsPtr = pixels)
                    {
                        sfWindow_SetIcon(This, width, height, PixelsPtr);
                    }
                }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Activate the window as the current target
            /// for rendering
            /// </summary>
            /// <returns>True if operation was successful, false otherwise</returns>
            ////////////////////////////////////////////////////////////
            public virtual bool SetActive()
            {
                return SetActive(true);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Activate of deactivate the window as the current target
            /// for rendering
            /// </summary>
            /// <param name="active">True to activate, false to deactivate (true by default)</param>
            /// <returns>True if operation was successful, false otherwise</returns>
            ////////////////////////////////////////////////////////////
            public virtual bool SetActive(bool active)
            {
                return sfWindow_SetActive(This, active);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Limit the framerate to a maximum fixed frequency
            /// </summary>
            /// <param name="limit">Framerate limit, in frames per seconds (use 0 to disable limit)</param>
            ////////////////////////////////////////////////////////////
            public virtual void SetFramerateLimit(uint limit)
            {
                sfWindow_SetFramerateLimit(This, limit);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get time elapsed since last frame
            /// </summary>
            /// <returns>Time elapsed, in seconds</returns>
            ////////////////////////////////////////////////////////////
            public virtual float GetFrameTime()
            {
                return sfWindow_GetFrameTime(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change the joystick threshold, ie. the value below which
            /// no move event will be generated
            /// </summary>
            /// <param name="threshold">New threshold, in range [0, 100]</param>
            ////////////////////////////////////////////////////////////
            public virtual void SetJoystickThreshold(float threshold)
            {
                sfWindow_SetJoystickThreshold(This, threshold);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// OS-specific handle of the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            public virtual IntPtr SystemHandle
            {
                get {return sfWindow_GetSystemHandle(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Wait for a new event and dispatch it to the corresponding
            /// event handler
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void WaitAndDispatchEvents()
            {
                Event e;
                if (WaitEvent(out e))
                    CallEventHandler(e);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Call the event handlers for each pending event
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void DispatchEvents()
            {
                Event e;
                while (GetEvent(out e))
                    CallEventHandler(e);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Provide a string describing the object
            /// </summary>
            /// <returns>String description of the object</returns>
            ////////////////////////////////////////////////////////////
            public override string ToString()
            {
                return "[Window]" +
                       " Width(" + Width + ")" +
                       " Height(" + Height + ")" +
                       " Settings(" + Settings + ")";
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Constructor for derived classes
            /// </summary>
            /// <param name="thisPtr">Pointer to the internal object</param>
            /// <param name="dummy">Internal hack :)</param>
            ////////////////////////////////////////////////////////////
            protected Window(IntPtr thisPtr, int dummy) :
                base(thisPtr)
            {
                // TODO : find a cleaner way of separating this constructor from Window(IntPtr handle)
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Internal function to get the next event (non-blocking)
            /// </summary>
            /// <param name="eventToFill">Variable to fill with the raw pointer to the event structure</param>
            /// <returns>True if there was an event, false otherwise</returns>
            ////////////////////////////////////////////////////////////
            protected virtual bool GetEvent(out Event eventToFill)
            {
                return sfWindow_GetEvent(This, out eventToFill);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Internal function to get the next event (blocking)
            /// </summary>
            /// <param name="eventToFill">Variable to fill with the raw pointer to the event structure</param>
            /// <returns>False if any error occured</returns>
            ////////////////////////////////////////////////////////////
            protected virtual bool WaitEvent(out Event eventToFill)
            {
                return sfWindow_WaitEvent(This, out eventToFill);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Handle the destruction of the object
            /// </summary>
            /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
            ////////////////////////////////////////////////////////////
            protected override void Destroy(bool disposing)
            {
                sfWindow_Destroy(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Call the event handler for the given event
            /// </summary>
            /// <param name="e">Event to dispatch</param>
            ////////////////////////////////////////////////////////////
            private void CallEventHandler(Event e)
            {
                switch (e.Type)
                {
                    case EventType.Closed :
                        if (Closed != null)
                            Closed(this, EventArgs.Empty);
                        break;

                    case EventType.GainedFocus :
                        if (GainedFocus != null)
                            GainedFocus(this, EventArgs.Empty);
                        break;

                    case EventType.JoyButtonPressed :
                        if (JoyButtonPressed != null)
                            JoyButtonPressed(this, new JoyButtonEventArgs(e.JoyButton));
                        break;

                    case EventType.JoyButtonReleased :
                        if (JoyButtonReleased != null)
                            JoyButtonReleased(this, new JoyButtonEventArgs(e.JoyButton));
                        break;

                    case EventType.JoyMoved :
                        if (JoyMoved != null)
                            JoyMoved(this, new JoyMoveEventArgs(e.JoyMove));
                        break;

                    case EventType.KeyPressed :
                        if (KeyPressed != null)
                            KeyPressed(this, new KeyEventArgs(e.Key));
                        break;

                    case EventType.KeyReleased :
                        if (KeyReleased != null)
                            KeyReleased(this, new KeyEventArgs(e.Key));
                        break;

                    case EventType.LostFocus :
                        if (LostFocus != null)
                            LostFocus(this, EventArgs.Empty);
                        break;

                    case EventType.MouseButtonPressed :
                        if (MouseButtonPressed != null)
                            MouseButtonPressed(this, new MouseButtonEventArgs(e.MouseButton));
                        break;

                    case EventType.MouseButtonReleased :
                        if (MouseButtonReleased != null)
                            MouseButtonReleased(this, new MouseButtonEventArgs(e.MouseButton));
                        break;

                    case EventType.MouseEntered :
                        if (MouseEntered != null)
                            MouseEntered(this, EventArgs.Empty);
                        break;

                    case EventType.MouseLeft :
                        if (MouseLeft != null)
                            MouseLeft(this, EventArgs.Empty);
                        break;

                    case EventType.MouseMoved :
                        if (MouseMoved != null)
                            MouseMoved(this, new MouseMoveEventArgs(e.MouseMove));
                        break;

                    case EventType.MouseWheelMoved :
                        if (MouseWheelMoved != null)
                            MouseWheelMoved(this, new MouseWheelEventArgs(e.MouseWheel));
                        break;

                    case EventType.Resized :
                        if (Resized != null)
                            Resized(this, new SizeEventArgs(e.Size));
                        break;

                    case EventType.TextEntered :
                        if (TextEntered != null)
                            TextEntered(this, new TextEventArgs(e.Text));
                        break;
                }
            }

            /// <summary>Event handler for the Closed event</summary>
            public event EventHandler Closed = null;

            /// <summary>Event handler for the Resized event</summary>
            public event EventHandler<SizeEventArgs> Resized = null;

            /// <summary>Event handler for the LostFocus event</summary>
            public event EventHandler LostFocus = null;

            /// <summary>Event handler for the GainedFocus event</summary>
            public event EventHandler GainedFocus = null;

            /// <summary>Event handler for the TextEntered event</summary>
            public event EventHandler<TextEventArgs> TextEntered = null;

            /// <summary>Event handler for the KeyPressed event</summary>
            public event EventHandler<KeyEventArgs> KeyPressed = null;

            /// <summary>Event handler for the KeyReleased event</summary>
            public event EventHandler<KeyEventArgs> KeyReleased = null;

            /// <summary>Event handler for the MouseWheelMoved event</summary>
            public event EventHandler<MouseWheelEventArgs> MouseWheelMoved = null;

            /// <summary>Event handler for the MouseButtonPressed event</summary>
            public event EventHandler<MouseButtonEventArgs> MouseButtonPressed = null;

            /// <summary>Event handler for the MouseButtonReleased event</summary>
            public event EventHandler<MouseButtonEventArgs> MouseButtonReleased = null;

            /// <summary>Event handler for the MouseMoved event</summary>
            public event EventHandler<MouseMoveEventArgs> MouseMoved = null;

            /// <summary>Event handler for the MouseEntered event</summary>
            public event EventHandler MouseEntered = null;

            /// <summary>Event handler for the MouseLeft event</summary>
            public event EventHandler MouseLeft = null;

            /// <summary>Event handler for the JoyButtonPressed event</summary>
            public event EventHandler<JoyButtonEventArgs> JoyButtonPressed = null;

            /// <summary>Event handler for the JoyButtonReleased event</summary>
            public event EventHandler<JoyButtonEventArgs> JoyButtonReleased = null;

            /// <summary>Event handler for the JoyMoved event</summary>
            public event EventHandler<JoyMoveEventArgs> JoyMoved = null;

            protected Input myInput = null;

            #region Imports
            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfWindow_Create(VideoMode Mode, string Title, Styles Style, ref ContextSettings Params);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfWindow_CreateFromHandle(IntPtr Handle, ref ContextSettings Params);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfWindow_Destroy(IntPtr This);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfWindow_GetInput(IntPtr This);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfWindow_IsOpened(IntPtr This);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfWindow_Close(IntPtr This);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfWindow_GetEvent(IntPtr This, out Event Evt);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfWindow_WaitEvent(IntPtr This, out Event Evt);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfWindow_Display(IntPtr This);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern uint sfWindow_GetWidth(IntPtr This);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern uint sfWindow_GetHeight(IntPtr This);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern ContextSettings sfWindow_GetSettings(IntPtr This);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfWindow_EnableVerticalSync(IntPtr This, bool Enable);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfWindow_ShowMouseCursor(IntPtr This, bool Show);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfWindow_SetCursorPosition(IntPtr This, uint X, uint Y);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfWindow_SetPosition(IntPtr This, int X, int Y);
            
            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfWindow_SetSize(IntPtr This, uint Width, uint Height);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfWindow_Show(IntPtr This, bool Show);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfWindow_EnableKeyRepeat(IntPtr This, bool Enable);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            unsafe static extern void sfWindow_SetIcon(IntPtr This, uint Width, uint Height, byte* Pixels);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfWindow_SetActive(IntPtr This, bool Active);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfWindow_SetFramerateLimit(IntPtr This, uint Limit);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfWindow_GetFrameTime(IntPtr This);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfWindow_SetJoystickThreshold(IntPtr This, float Threshold);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfWindow_GetSystemHandle(IntPtr This);
            #endregion
        }
    }
}

using System;
using System.Runtime.InteropServices;
using System.Collections;
using System.Collections.Generic;
using System.Security;
using SFML.Window;

namespace SFML
{
    namespace Graphics
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Simple wrapper for Window that allows easy
        /// 2D rendering
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class RenderWindow : SFML.Window.Window, RenderTarget
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create the window with default style and creation settings
            /// </summary>
            /// <param name="mode">Video mode to use</param>
            /// <param name="title">Title of the window</param>
            ////////////////////////////////////////////////////////////
            public RenderWindow(VideoMode mode, string title) :
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
            public RenderWindow(VideoMode mode, string title, Styles style) :
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
            public RenderWindow(VideoMode mode, string title, Styles style, ContextSettings settings) :
                base(sfRenderWindow_Create(mode, title, style, ref settings), 0)
            {
                Initialize();
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create the window from an existing control with default creation settings
            /// </summary>
            /// <param name="handle">Platform-specific handle of the control</param>
            ////////////////////////////////////////////////////////////
            public RenderWindow(IntPtr handle) :
                this(handle, new ContextSettings(24, 8))
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create the window from an existing control
            /// </summary>
            /// <param name="handle">Platform-specific handle of the control</param>
            /// <param name="settings">Creation parameters</param>
            ////////////////////////////////////////////////////////////
            public RenderWindow(IntPtr handle, ContextSettings settings) :
                base(sfRenderWindow_CreateFromHandle(handle, ref settings), 0)
            {
                Initialize();
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Tell whether or not the window is opened (ie. has been created).
            /// Note that a hidden window (Show(false))
            /// will still return true
            /// </summary>
            /// <returns>True if the window is opened</returns>
            ////////////////////////////////////////////////////////////
            public override bool IsOpened()
            {
                return sfRenderWindow_IsOpened(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Close (destroy) the window.
            /// The Window instance remains valid and you can call
            /// Create to recreate the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override void Close()
            {
                sfRenderWindow_Close(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Display the window on screen
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override void Display()
            {
                sfRenderWindow_Display(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Width of the rendering region of the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override uint Width
            {
                get {return sfRenderWindow_GetWidth(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Height of the rendering region of the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override uint Height
            {
                get {return sfRenderWindow_GetHeight(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Creation settings of the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override ContextSettings Settings
            {
                get {return sfRenderWindow_GetSettings(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Enable / disable vertical synchronization
            /// </summary>
            /// <param name="enable">True to enable v-sync, false to deactivate</param>
            ////////////////////////////////////////////////////////////
            public override void EnableVerticalSync(bool enable)
            {
                sfRenderWindow_EnableVerticalSync(This, enable);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Show or hide the mouse cursor
            /// </summary>
            /// <param name="show">True to show, false to hide</param>
            ////////////////////////////////////////////////////////////
            public override void ShowMouseCursor(bool show)
            {
                sfRenderWindow_ShowMouseCursor(This, show);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change the position of the mouse cursor
            /// </summary>
            /// <param name="x">Left coordinate of the cursor, relative to the window</param>
            /// <param name="y">Top coordinate of the cursor, relative to the window</param>
            ////////////////////////////////////////////////////////////
            public override void SetCursorPosition(uint x, uint y)
            {
                sfRenderWindow_SetCursorPosition(This, x, y);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change the position of the window on screen.
            /// Only works for top-level windows
            /// </summary>
            /// <param name="x">Left position</param>
            /// <param name="y">Top position</param>
            ////////////////////////////////////////////////////////////
            public override void SetPosition(int x, int y)
            {
                sfRenderWindow_SetPosition(This, x, y);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change the size of the rendering region of the window
            /// </summary>
            /// <param name="width">New width</param>
            /// <param name="height">New height</param>
            ////////////////////////////////////////////////////////////
            public override void SetSize(uint width, uint height)
            {
                sfRenderWindow_SetSize(This, width, height);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Show or hide the window
            /// </summary>
            /// <param name="show">True to show, false to hide</param>
            ////////////////////////////////////////////////////////////
            public override void Show(bool show)
            {
                sfRenderWindow_Show(This, show);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Enable or disable automatic key-repeat.
            /// Automatic key-repeat is enabled by default
            /// </summary>
            /// <param name="enable">True to enable, false to disable</param>
            ////////////////////////////////////////////////////////////
            public override void EnableKeyRepeat(bool enable)
            {
                sfRenderWindow_EnableKeyRepeat(This, enable);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change the window's icon
            /// </summary>
            /// <param name="width">Icon's width, in pixels</param>
            /// <param name="height">Icon's height, in pixels</param>
            /// <param name="pixels">Array of pixels, format must be RGBA 32 bits</param>
            ////////////////////////////////////////////////////////////
            public override void SetIcon(uint width, uint height, byte[] pixels)
            {
                unsafe
                {
                    fixed (byte* PixelsPtr = pixels)
                    {
                        sfRenderWindow_SetIcon(This, width, height, PixelsPtr);
                    }
                }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Activate of deactivate the window as the current target
            /// for rendering
            /// </summary>
            /// <param name="active">True to activate, false to deactivate (true by default)</param>
            /// <returns>True if operation was successful, false otherwise</returns>
            ////////////////////////////////////////////////////////////
            public override bool SetActive(bool active)
            {
                return sfRenderWindow_SetActive(This, active);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Limit the framerate to a maximum fixed frequency
            /// </summary>
            /// <param name="limit">Framerate limit, in frames per seconds (use 0 to disable limit)</param>
            ////////////////////////////////////////////////////////////
            public override void SetFramerateLimit(uint limit)
            {
                sfRenderWindow_SetFramerateLimit(This, limit);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get time elapsed since last frame
            /// </summary>
            /// <returns>Time elapsed, in seconds</returns>
            ////////////////////////////////////////////////////////////
            public override float GetFrameTime()
            {
                return sfRenderWindow_GetFrameTime(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change the joystick threshold, ie. the value below which
            /// no move event will be generated
            /// </summary>
            /// <param name="threshold">New threshold, in range [0, 100]</param>
            ////////////////////////////////////////////////////////////
            public override void SetJoystickThreshold(float threshold)
            {
                sfRenderWindow_SetJoystickThreshold(This, threshold);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// OS-specific handle of the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override IntPtr SystemHandle
            {
                get {return sfRenderWindow_GetSystemHandle(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Default view of the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            public View DefaultView
            {
                get {return myDefaultView;}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Return the current active view
            /// </summary>
            /// <returns>The current view</returns>
            ////////////////////////////////////////////////////////////
            public View GetView()
            {
                return new View(sfRenderWindow_GetView(This));
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change the current active view
            /// </summary>
            /// <param name="view">New view</param>
            ////////////////////////////////////////////////////////////
            public void SetView(View view)
            {
                sfRenderWindow_SetView(This, view.This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get the viewport of a view applied to this target
            /// </summary>
            /// <param name="view">Target view</param>
            /// <returns>Viewport rectangle, expressed in pixels in the current target</returns>
            ////////////////////////////////////////////////////////////
            public IntRect GetViewport(View view)
            {
                return sfRenderWindow_GetViewport(This, view.This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Convert a point in target coordinates into view coordinates
            /// This version uses the current view of the window
            /// </summary>
            /// <param name="x">X coordinate of the point to convert, relative to the target</param>
            /// <param name="y">Y coordinate of the point to convert, relative to the target</param>
            /// <returns>Converted point</returns>
            ///
            ////////////////////////////////////////////////////////////
            public Vector2 ConvertCoords(uint x, uint y)
            {
                return ConvertCoords(x, y, GetView());
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Convert a point in target coordinates into view coordinates
            /// This version uses the given view
            /// </summary>
            /// <param name="x">X coordinate of the point to convert, relative to the target</param>
            /// <param name="y">Y coordinate of the point to convert, relative to the target</param>
            /// <param name="view">Target view to convert the point to</param>
            /// <returns>Converted point</returns>
            ///
            ////////////////////////////////////////////////////////////
            public Vector2 ConvertCoords(uint x, uint y, View view)
            {
                Vector2 point;
                sfRenderWindow_ConvertCoords(This, x, y, out point.X, out point.Y, view.This);

                return point;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Clear the entire window with black color
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void Clear()
            {
                sfRenderWindow_Clear(This, Color.Black);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Clear the entire window with a single color
            /// </summary>
            /// <param name="color">Color to use to clear the window</param>
            ////////////////////////////////////////////////////////////
            public void Clear(Color color)
            {
                sfRenderWindow_Clear(This, color);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Draw something into the window
            /// </summary>
            /// <param name="objectToDraw">Object to draw</param>
            ////////////////////////////////////////////////////////////
            public void Draw(Drawable objectToDraw)
            {
                objectToDraw.Render(this, null);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Draw something into the window with a shader
            /// </summary>
            /// <param name="objectToDraw">Object to draw</param>
            /// <param name="shader">Shader to apply</param>
            ////////////////////////////////////////////////////////////
            public void Draw(Drawable objectToDraw, Shader shader)
            {
                objectToDraw.Render(this, shader);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Save the current OpenGL render states and matrices
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void SaveGLStates()
            {
                sfRenderWindow_SaveGLStates(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Restore the previously saved OpenGL render states and matrices
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void RestoreGLStates()
            {
                sfRenderWindow_RestoreGLStates(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Provide a string describing the object
            /// </summary>
            /// <returns>String description of the object</returns>
            ////////////////////////////////////////////////////////////
            public override string ToString()
            {
                return "[RenderWindow]" +
                       " Width(" + Width + ")" +
                       " Height(" + Height + ")" +
                       " Settings(" + Settings + ")" +
                       " DefaultView(" + DefaultView + ")" +
                       " View(" + GetView() + ")";
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Internal function to get the next event
            /// </summary>
            /// <param name="eventToFill">Variable to fill with the raw pointer to the event structure</param>
            /// <returns>True if there was an event, false otherwise</returns>
            ////////////////////////////////////////////////////////////
            protected override bool GetEvent(out Event eventToFill)
            {
                return sfRenderWindow_GetEvent(This, out eventToFill);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Internal function to get the next event (blocking)
            /// </summary>
            /// <param name="eventToFill">Variable to fill with the raw pointer to the event structure</param>
            /// <returns>False if any error occured</returns>
            ////////////////////////////////////////////////////////////
            protected override bool WaitEvent(out Event eventToFill)
            {
                return sfRenderWindow_WaitEvent(This, out eventToFill);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Handle the destruction of the object
            /// </summary>
            /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
            ////////////////////////////////////////////////////////////
            protected override void Destroy(bool disposing)
            {
                sfRenderWindow_Destroy(This);

                if (disposing)
                    myDefaultView.Dispose();
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Do common initializations
            /// </summary>
            ////////////////////////////////////////////////////////////
            private void Initialize()
            {
                myInput       = new Input(sfRenderWindow_GetInput(This));
                myDefaultView = new View(sfRenderWindow_GetDefaultView(This));
                GC.SuppressFinalize(myDefaultView);
            }

            private View myDefaultView = null;

            #region Imports
            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfRenderWindow_Create(VideoMode Mode, string Title, Styles Style, ref ContextSettings Params);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfRenderWindow_CreateFromHandle(IntPtr Handle, ref ContextSettings Params);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_Destroy(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfRenderWindow_GetInput(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfRenderWindow_IsOpened(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_Close(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfRenderWindow_GetEvent(IntPtr This, out Event Evt);
            
            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfRenderWindow_WaitEvent(IntPtr This, out Event Evt);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_Clear(IntPtr This, Color ClearColor);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_Display(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern uint sfRenderWindow_GetWidth(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern uint sfRenderWindow_GetHeight(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern ContextSettings sfRenderWindow_GetSettings(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_EnableVerticalSync(IntPtr This, bool Enable);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_ShowMouseCursor(IntPtr This, bool Show);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_SetCursorPosition(IntPtr This, uint X, uint Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_SetPosition(IntPtr This, int X, int Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_SetSize(IntPtr This, uint Width, uint Height);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_Show(IntPtr This, bool Show);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_EnableKeyRepeat(IntPtr This, bool Enable);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            unsafe static extern void sfRenderWindow_SetIcon(IntPtr This, uint Width, uint Height, byte* Pixels);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfRenderWindow_SetActive(IntPtr This, bool Active);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfRenderWindow_SaveGLStates(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfRenderWindow_RestoreGLStates(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_SetFramerateLimit(IntPtr This, uint Limit);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfRenderWindow_GetFrameTime(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_SetJoystickThreshold(IntPtr This, float Threshold);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_SetView(IntPtr This, IntPtr View);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfRenderWindow_GetView(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfRenderWindow_GetDefaultView(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntRect sfRenderWindow_GetViewport(IntPtr This, IntPtr TargetView);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_ConvertCoords(IntPtr This, uint WindowX, uint WindowY, out float ViewX, out float ViewY, IntPtr TargetView);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfRenderWindow_GetSystemHandle(IntPtr This);

            #endregion
        }
    }
}

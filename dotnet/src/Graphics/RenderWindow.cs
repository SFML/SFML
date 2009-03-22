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
        public class RenderWindow : SFML.Window.Window
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create the window with default style and creation settings
            /// </summary>
            /// <param name="mode">Video mode to use</param>
            /// <param name="title">Title of the window</param>
            ////////////////////////////////////////////////////////////
            public RenderWindow(VideoMode mode, string title) :
                this(mode, title, Styles.Resize | Styles.Close, new WindowSettings(24, 8, 0))
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
                this(mode, title, style, new WindowSettings(24, 8, 0))
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
            public RenderWindow(VideoMode mode, string title, Styles style, WindowSettings settings) :
                base(sfRenderWindow_Create(mode, title, style, settings), 0)
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
                this(handle, new WindowSettings(24, 8, 0))
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create the window from an existing control
            /// </summary>
            /// <param name="handle">Platform-specific handle of the control</param>
            /// <param name="settings">Creation parameters</param>
            ////////////////////////////////////////////////////////////
            public RenderWindow(IntPtr handle, WindowSettings settings) :
                base(sfRenderWindow_CreateFromHandle(handle, settings), 0)
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
            public override WindowSettings Settings
            {
                get {return sfRenderWindow_GetSettings(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Enable / disable vertical synchronization
            /// </summary>
            /// <param name="enable">True to enable v-sync, false to deactivate</param>
            ////////////////////////////////////////////////////////////
            public override void UseVerticalSync(bool enable)
            {
                sfRenderWindow_UseVerticalSync(This, enable);
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
            /// Default view of the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            public View DefaultView
            {
                get {return myDefaultView;}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Current view active in the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            public View CurrentView
            {
                get {return myCurrentView;}
                set {myCurrentView = value; sfRenderWindow_SetView(This, value.This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Convert a point in window coordinates into view coordinates
            /// using the current view of the window
            /// </summary>
            /// <param name="windowX">X coordinate of the point to convert, relative to the window</param>
            /// <param name="windowY">Y coordinate of the point to convert, relative to the window</param>
            /// <returns>Converted point</returns>
            ////////////////////////////////////////////////////////////
            public Vector2 ConvertCoords(uint windowX, uint windowY)
            {
                return ConvertCoords(windowX, windowY, myCurrentView);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Convert a point in window coordinates into view coordinates
            /// </summary>
            /// <param name="windowX">X coordinate of the point to convert, relative to the window</param>
            /// <param name="windowY">Y coordinate of the point to convert, relative to the window</param>
            /// <param name="targetView">Target view to convert the point to</param>
            /// <returns>Converted point</returns>
            ////////////////////////////////////////////////////////////
            public Vector2 ConvertCoords(uint windowX, uint windowY, View targetView)
            {
                Vector2 Point;
                sfRenderWindow_ConvertCoords(This, windowX, windowY, out Point.X, out Point.Y, targetView.This);

                return Point;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Tell SFML to preserve external OpenGL states, at the expense of
            /// more CPU charge. Use this function if you don't want SFML
            /// to mess up your own OpenGL states (if any).
            /// Don't enable state preservation if not needed, as it will allow
            /// SFML to do internal optimizations and improve performances.
            /// This parameter is false by default
            /// </summary>
            /// <param name="preserve">True to preserve OpenGL states, false to let SFML optimize</param>
            ////////////////////////////////////////////////////////////
            public void PreserveOpenGLStates(bool preserve)
            {
                sfRenderWindow_PreserveOpenGLStates(This, preserve);
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
            /// Save the content of the window to an image
            /// </summary>
            /// <returns>Image instance containing the contents of the screen</returns>
            ////////////////////////////////////////////////////////////
            public Image Capture()
            {
                return new Image(sfRenderWindow_Capture(This));
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Draw something into the window
            /// </summary>
            /// <param name="objectToDraw">Object to draw</param>
            ////////////////////////////////////////////////////////////
            public void Draw(Drawable objectToDraw)
            {
                objectToDraw.Render(this);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Apply a post-fx to the window
            /// </summary>
            /// <param name="postFx">PostFx to apply</param>
            ////////////////////////////////////////////////////////////
            public void Draw(PostFx postFx)
            {
                sfRenderWindow_DrawPostFX(This, postFx != null ? postFx.This : IntPtr.Zero);
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
                myCurrentView = myDefaultView;
                GC.SuppressFinalize(myDefaultView);
            }

            private View myCurrentView = null;
            private View myDefaultView = null;

            #region Imports
            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfRenderWindow_Create(VideoMode Mode, string Title, Styles Style, WindowSettings Params);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfRenderWindow_CreateFromHandle(IntPtr Handle, WindowSettings Params);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_Destroy(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfRenderWindow_GetInput(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern bool sfRenderWindow_IsOpened(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_Close(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern bool sfRenderWindow_GetEvent(IntPtr This, out Event Evt);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfRenderWindow_Capture(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_Clear(IntPtr This, Color ClearColor);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_Display(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern uint sfRenderWindow_GetWidth(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern uint sfRenderWindow_GetHeight(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern WindowSettings sfRenderWindow_GetSettings(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_UseVerticalSync(IntPtr This, bool Enable);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_ShowMouseCursor(IntPtr This, bool Show);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_SetCursorPosition(IntPtr This, uint X, uint Y);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_SetPosition(IntPtr This, int X, int Y);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_SetSize(IntPtr This, uint Width, uint Height);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_Show(IntPtr This, bool Show);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_EnableKeyRepeat(IntPtr This, bool Enable);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            unsafe static extern void sfRenderWindow_SetIcon(IntPtr This, uint Width, uint Height, byte* Pixels);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern bool sfRenderWindow_SetActive(IntPtr This, bool Active);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_SetFramerateLimit(IntPtr This, uint Limit);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern float sfRenderWindow_GetFrameTime(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_SetJoystickThreshold(IntPtr This, float Threshold);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_SetView(IntPtr This, IntPtr View);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfRenderWindow_GetView(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfRenderWindow_GetDefaultView(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_ConvertCoords(IntPtr This, uint WindowX, uint WindowY, out float ViewX, out float ViewY, IntPtr TargetView);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_PreserveOpenGLStates(IntPtr This, bool Preserve);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_DrawPostFX(IntPtr This, IntPtr PostFx);

            #endregion
        }
    }
}

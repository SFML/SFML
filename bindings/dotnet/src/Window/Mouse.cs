using System;
using System.Runtime.InteropServices;
using System.Security;

namespace SFML
{
    namespace Window
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Give access to the real-time state of the mouse
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class Mouse
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Mouse buttons
            /// </summary>
            ////////////////////////////////////////////////////////////
            public enum Button
            {
                /// <summary>The left mouse button</summary>
                Left,

                /// <summary>The right mouse button</summary>
                Right,

                /// <summary>The middle (wheel) mouse button</summary>
                Middle,

                /// <summary>The first extra mouse button</summary>
                XButton1,

                /// <summary>The second extra mouse button</summary>
                XButton2,

                /// <summary>Keep last -- the total number of mouse buttons</summary>
                ButtonCount
            };

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Check if a mouse button is pressed
            /// </summary>
            /// <param name="button">Button to check</param>
            /// <returns>True if the button is pressed, false otherwise</returns>
            ////////////////////////////////////////////////////////////
            public static bool IsButtonPressed(Button button)
            {
                return sfMouse_IsButtonPressed(button);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get the current position of the mouse
            /// </summary>
            /// This function returns the current position of the mouse
            /// cursor.
            /// If the cursor is over a SFML window, the returned position
            /// is relative to this window. Otherwise, the returned position
            /// is in desktop coordinates.
            /// <returns>Current position of the mouse</returns>
            ////////////////////////////////////////////////////////////
            public static Vector2i GetPosition()
            {
                Vector2i position;
                sfMouse_GetPosition(out position.X, out position.Y);
                return position;
            }

            #region Imports
            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfMouse_IsButtonPressed(Button button);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfMouse_GetPosition(out int x, out int y);
            #endregion
        }
    }
}

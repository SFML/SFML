using System;
using System.Runtime.InteropServices;
using System.Security;

namespace SFML
{
    namespace Window
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Input handles real-time input from keyboard and mouse.
        /// Use it instead of events to handle continuous moves and more
        /// game-friendly inputs
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class Input : ObjectBase
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get the state of a key
            /// </summary>
            /// <param name="key">Key to check</param>
            /// <returns>True if key is down, false if key is up</returns>
            ////////////////////////////////////////////////////////////
            public bool IsKeyDown(KeyCode key)
            {
                return sfInput_IsKeyDown(This, key);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get the state of a mouse button
            /// </summary>
            /// <param name="button">Button to check</param>
            /// <returns>True if button is down, false if button is up</returns>
            ////////////////////////////////////////////////////////////
            public bool IsMouseButtonDown(MouseButton button)
            {
                return sfInput_IsMouseButtonDown(This, button);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get the state of a joystick button
            /// </summary>
            /// <param name="joystickId">Identifier of the joystick to check (0 or 1)</param>
            /// <param name="button">Button to check</param>
            /// <returns>True if button is down, false if button is up</returns>
            ////////////////////////////////////////////////////////////
            public bool IsJoystickButtonDown(uint joystickId, uint button)
            {
                return sfInput_IsJoystickButtonDown(This, joystickId, button);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get the mouse X position
            /// </summary>
            /// <returns>Current mouse left position, relative to owner window</returns>
            ////////////////////////////////////////////////////////////
            public int GetMouseX()
            {
                return sfInput_GetMouseX(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get the mouse Y position
            /// </summary>
            /// <returns>Current mouse top position, relative to owner window</returns>
            ////////////////////////////////////////////////////////////
            public int GetMouseY()
            {
                return sfInput_GetMouseY(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get a joystick axis position
            /// </summary>
            /// <param name="joystickId">Identifier of the joystick to check (0 or 1)</param>
            /// <param name="axis">Axis to get</param>
            /// <returns>Current axis position, in the range [-100, 100] (except for POV, which is [0, 360])</returns>
            ////////////////////////////////////////////////////////////
            public float GetJoystickAxis(uint joystickId, JoyAxis axis)
            {
                return sfInput_GetJoystickAxis(This, joystickId, axis);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// For internal use only, construct the instance from a direct pointer to the internal object
            /// </summary>
            /// <param name="thisPtr">Internal pointer to the input object</param>
            ////////////////////////////////////////////////////////////
            public Input(IntPtr thisPtr) :
                base(thisPtr)
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Provide a string describing the object
            /// </summary>
            /// <returns>String description of the object</returns>
            ////////////////////////////////////////////////////////////
            public override string ToString()
            {
                return "[Input]";
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Handle the destruction of the object
            /// </summary>
            /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
            ////////////////////////////////////////////////////////////
            protected override void Destroy(bool disposing)
            {
                // Nothing to do here, Input instances are owned by the C library
            }

            #region Imports
            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfInput_IsKeyDown(IntPtr This, KeyCode Key);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfInput_IsMouseButtonDown(IntPtr This, MouseButton Button);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfInput_IsJoystickButtonDown(IntPtr This, uint JoyId, uint Button);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern int sfInput_GetMouseX(IntPtr This);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern int sfInput_GetMouseY(IntPtr This);

            [DllImport("csfml-window-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfInput_GetJoystickAxis(IntPtr This, uint JoyId, JoyAxis Axis);
            #endregion
        }
    }
}

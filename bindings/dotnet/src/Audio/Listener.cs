using System;
using System.Runtime.InteropServices;
using System.Security;

namespace SFML
{
    namespace Audio
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Listener is a global interface for defining the audio
        /// listener properties ; the audio listener is the point in
        /// the scene from where all the sounds are heard
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class Listener
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Global volume of all sounds, in range [0 .. 100] (default is 100)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public static float GlobalVolume
            {
                get {return sfListener_GetGlobalVolume();}
                set {sfListener_SetGlobalVolume(value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// 3D position of the listener (default is (0, 0, 0))
            /// </summary>
            ////////////////////////////////////////////////////////////
            public static Vector3 Position
            {
                get {Vector3 v; sfListener_GetPosition(out v.X, out v.Y, out v.Z); return v;}
                set {sfListener_SetPosition(value.X, value.Y, value.Z);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// 3D direction of the listener (default is (0, 0, -1))
            /// </summary>
            ////////////////////////////////////////////////////////////
            public static Vector3 Direction
            {
                get {Vector3 v; sfListener_GetDirection(out v.X, out v.Y, out v.Z); return v;}
                set {sfListener_SetDirection(value.X, value.Y, value.Z);}
            }

            #region Imports
            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfListener_SetGlobalVolume(float Volume);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfListener_GetGlobalVolume();

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfListener_SetPosition(float X, float Y, float Z);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfListener_GetPosition(out float X, out float Y, out float Z);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfListener_SetDirection(float X, float Y, float Z);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfListener_GetDirection(out float X, out float Y, out float Z);
            #endregion
        }
    }
}

using System;
using System.Runtime.InteropServices;
using System.Security;

namespace SFML
{
    namespace Audio
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Enumeration of all possible sound states
        /// </summary>
        ////////////////////////////////////////////////////////////
        public enum SoundStatus
        {
            /// <summary>Sound is not playing</summary>
            Stopped,

            /// <summary>Sound is paused</summary>
            Paused,

            /// <summary>Sound is playing</summary>
            Playing
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Sound defines the properties of a sound such as position,
        /// volume, pitch, etc.
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class Sound : ObjectBase
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Default constructor (invalid sound)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public Sound() :
                base(sfSound_Create())
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the sound from a source buffer
            /// </summary>
            /// <param name="buffer">Sound buffer to play</param>
            ////////////////////////////////////////////////////////////
            public Sound(SoundBuffer buffer) :
                base(sfSound_Create())
            {
                SoundBuffer = buffer;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the sound from another source
            /// </summary>
            /// <param name="copy">Sound to copy</param>
            ////////////////////////////////////////////////////////////
            public Sound(Sound copy) :
                base(sfSound_Copy(copy.This))
            {
                SoundBuffer = copy.SoundBuffer;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Play the sound
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void Play()
            {
                sfSound_Play(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Pause the sound
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void Pause()
            {
                sfSound_Pause(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Stop the sound
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void Stop()
            {
                sfSound_Stop(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Buffer containing the sound data to play through the sound
            /// </summary>
            ////////////////////////////////////////////////////////////
            public SoundBuffer SoundBuffer
            {
                get {return myBuffer;}
                set {myBuffer = value; sfSound_SetBuffer(This, value != null ? value.This : IntPtr.Zero);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Current status of the sound (see SoundStatus enum)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public SoundStatus Status
            {
                get {return sfSound_GetStatus(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Loop state of the sound. Default value is false
            /// </summary>
            ////////////////////////////////////////////////////////////
            public bool Loop
            {
                get {return sfSound_GetLoop(This);}
                set {sfSound_SetLoop(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Pitch of the sound. Default value is 1
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float Pitch
            {
                get {return sfSound_GetPitch(This);}
                set {sfSound_SetPitch(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Volume of the sound, in range [0, 100]. Default value is 100
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float Volume
            {
                get {return sfSound_GetVolume(This);}
                set {sfSound_SetVolume(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Current playing position of the sound, in seconds
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float PlayingOffset
            {
                get {return sfSound_GetPlayingOffset(This);}
                set {sfSound_SetPlayingOffset(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// 3D position of the sound. Default value is (0, 0, 0)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public Vector3 Position
            {
                get {Vector3 v; sfSound_GetPosition(This, out v.X, out v.Y, out v.Z); return v;}
                set {sfSound_SetPosition(This, value.X, value.Y, value.Z);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Is the sound's position relative to the listener's position,
            /// or is it absolute?
            /// Default value is false (absolute)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public bool RelativeToListener
            {
                get {return sfSound_IsRelativeToListener(This);}
                set {sfSound_SetRelativeToListener(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Minimum distance of the sound. Closer than this distance,
            /// the listener will hear the sound at its maximum volume.
            /// The default value is 1
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float MinDistance
            {
                get {return sfSound_GetMinDistance(This);}
                set {sfSound_SetMinDistance(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Attenuation factor. The higher the attenuation, the
            /// more the sound will be attenuated with distance from listener.
            /// The default value is 1
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float Attenuation
            {
                get {return sfSound_GetAttenuation(This);}
                set {sfSound_SetAttenuation(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Provide a string describing the object
            /// </summary>
            /// <returns>String description of the object</returns>
            ////////////////////////////////////////////////////////////
            public override string ToString()
            {
                return "[Sound]" +
                       " Status(" + Status + ")" +
                       " Loop(" + Loop + ")" +
                       " Pitch(" + Pitch + ")" +
                       " Volume(" + Volume + ")" +
                       " Position(" + Position + ")" +
                       " RelativeToListener(" + RelativeToListener + ")" +
                       " MinDistance(" + MinDistance + ")" +
                       " Attenuation(" + Attenuation + ")" +
                       " PlayingOffset(" + PlayingOffset + ")" +
                       " SoundBuffer(" + SoundBuffer + ")";
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Handle the destruction of the object
            /// </summary>
            /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
            ////////////////////////////////////////////////////////////
            protected override void Destroy(bool disposing)
            {
                sfSound_Destroy(This);
            }

            private SoundBuffer myBuffer;

            #region Imports
            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfSound_Create();

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfSound_Copy(IntPtr Sound);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSound_Destroy(IntPtr Sound);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSound_Play(IntPtr Sound);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSound_Pause(IntPtr Sound);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSound_Stop(IntPtr Sound);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSound_SetBuffer(IntPtr Sound, IntPtr Buffer);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfSound_GetBuffer(IntPtr Sound);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSound_SetLoop(IntPtr Sound, bool Loop);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfSound_GetLoop(IntPtr Sound);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern SoundStatus sfSound_GetStatus(IntPtr Sound);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSound_SetPitch(IntPtr Sound, float Pitch);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSound_SetVolume(IntPtr Sound, float Volume);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSound_SetPosition(IntPtr Sound, float X, float Y, float Z);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSound_SetRelativeToListener(IntPtr Sound, bool Relative);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSound_SetMinDistance(IntPtr Sound, float MinDistance);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSound_SetAttenuation(IntPtr Sound, float Attenuation);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSound_SetPlayingOffset(IntPtr Sound, float TimeOffset);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSound_GetPitch(IntPtr Sound);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSound_GetVolume(IntPtr Sound);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSound_GetPosition(IntPtr Sound, out float X, out float Y, out float Z);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfSound_IsRelativeToListener(IntPtr Sound);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSound_GetMinDistance(IntPtr Sound);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSound_GetAttenuation(IntPtr Sound);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSound_GetPlayingOffset(IntPtr Sound);
            #endregion
        }
    }
}

using System;
using System.Runtime.InteropServices;
using System.Security;
using System.IO;

namespace SFML
{
    namespace Audio
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Music defines a big sound played using streaming,
        /// so usually what we call a music :)
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class Music : ObjectBase
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the music from a file
            /// </summary>
            /// <param name="filename">Path of the music file to load</param>
            ////////////////////////////////////////////////////////////
            public Music(string filename) :
                base(sfMusic_CreateFromFile(filename))
            {
                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("music", filename);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the music from a file in a stream
            /// </summary>
            /// <param name="stream">Stream containing the file contents</param>
            ////////////////////////////////////////////////////////////
            public Music(Stream stream) :
                base(IntPtr.Zero)
            {
                stream.Position = 0;
                byte[] StreamData = new byte[stream.Length];
                uint Read = (uint)stream.Read(StreamData, 0, StreamData.Length);
                unsafe
                {
                    fixed (byte* dataPtr = StreamData)
                    {
                        SetThis(sfMusic_CreateFromMemory((char*)dataPtr, Read));
                    }
                }
                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("music");
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Play the music
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void Play()
            {
                sfMusic_Play(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Pause the music
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void Pause()
            {
                sfMusic_Pause(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Stop the music
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void Stop()
            {
                sfMusic_Stop(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Samples rate, in samples per second
            /// </summary>
            ////////////////////////////////////////////////////////////
            public uint SampleRate
            {
                get {return sfMusic_GetSampleRate(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Number of channels (1 = mono, 2 = stereo)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public uint ChannelsCount
            {
                get {return sfMusic_GetChannelsCount(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Current status of the music (see SoundStatus enum)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public SoundStatus Status
            {
                get {return sfMusic_GetStatus(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Total duration of the music, in seconds
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float Duration
            {
                get {return sfMusic_GetDuration(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Loop state of the sound. Default value is false
            /// </summary>
            ////////////////////////////////////////////////////////////
            public bool Loop
            {
                get {return sfMusic_GetLoop(This);}
                set {sfMusic_SetLoop(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Pitch of the music. Default value is 1
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float Pitch
            {
                get {return sfMusic_GetPitch(This);}
                set {sfMusic_SetPitch(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Volume of the music, in range [0, 100]. Default value is 100
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float Volume
            {
                get {return sfMusic_GetVolume(This);}
                set {sfMusic_SetVolume(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// 3D position of the music. Default value is (0, 0, 0)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public Vector3 Position
            {
                get {Vector3 v; sfMusic_GetPosition(This, out v.X, out v.Y, out v.Z); return v;}
                set {sfMusic_SetPosition(This, value.X, value.Y, value.Z);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Is the music's position relative to the listener's position,
            /// or is it absolute?
            /// Default value is false (absolute)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public bool RelativeToListener
            {
                get {return sfMusic_IsRelativeToListener(This);}
                set {sfMusic_SetRelativeToListener(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Minimum distance of the music. Closer than this distance,
            /// the listener will hear the sound at its maximum volume.
            /// The default value is 1
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float MinDistance
            {
                get {return sfMusic_GetMinDistance(This);}
                set {sfMusic_SetMinDistance(This, value);}
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
                get {return sfMusic_GetAttenuation(This);}
                set {sfMusic_SetAttenuation(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Current playing position, in seconds
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float PlayingOffset
            {
                get {return sfMusic_GetPlayingOffset(This);}
                set {sfMusic_SetPlayingOffset(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Provide a string describing the object
            /// </summary>
            /// <returns>String description of the object</returns>
            ////////////////////////////////////////////////////////////
            public override string ToString()
            {
                return "[Music]" +
                       " SampleRate(" + SampleRate + ")" +
                       " ChannelsCount(" + ChannelsCount + ")" +
                       " Status(" + Status + ")" +
                       " Duration(" + Duration + ")" +
                       " Loop(" + Loop + ")" +
                       " Pitch(" + Pitch + ")" +
                       " Volume(" + Volume + ")" +
                       " Position(" + Position + ")" +
                       " RelativeToListener(" + RelativeToListener + ")" +
                       " MinDistance(" + MinDistance + ")" +
                       " Attenuation(" + Attenuation + ")" +
                       " PlayingOffset(" + PlayingOffset + ")";
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Handle the destruction of the object
            /// </summary>
            /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
            ////////////////////////////////////////////////////////////
            protected override void Destroy(bool disposing)
            {
                sfMusic_Destroy(This);
            }

            #region Imports
            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfMusic_CreateFromFile(string Filename);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            unsafe static extern IntPtr sfMusic_CreateFromMemory(char* Data, uint SizeInBytes);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfMusic_Destroy(IntPtr MusicStream);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfMusic_Play(IntPtr Music);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfMusic_Pause(IntPtr Music);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfMusic_Stop(IntPtr Music);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern SoundStatus sfMusic_GetStatus(IntPtr Music);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfMusic_GetDuration(IntPtr Music);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern uint sfMusic_GetChannelsCount(IntPtr Music);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern uint sfMusic_GetSampleRate(IntPtr Music);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfMusic_SetPitch(IntPtr Music, float Pitch);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfMusic_SetLoop(IntPtr Music, bool Loop);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfMusic_SetVolume(IntPtr Music, float Volume);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfMusic_SetPosition(IntPtr Music, float X, float Y, float Z);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfMusic_SetRelativeToListener(IntPtr Music, bool Relative);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfMusic_SetMinDistance(IntPtr Music, float MinDistance);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfMusic_SetAttenuation(IntPtr Music, float Attenuation);
            
            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfMusic_SetPlayingOffset(IntPtr Music, float TimeOffset);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfMusic_GetLoop(IntPtr Music);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfMusic_GetPitch(IntPtr Music);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfMusic_GetVolume(IntPtr Music);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfMusic_GetPosition(IntPtr Music, out float X, out float Y, out float Z);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfMusic_IsRelativeToListener(IntPtr Music);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfMusic_GetMinDistance(IntPtr Music);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfMusic_GetAttenuation(IntPtr Music);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfMusic_GetPlayingOffset(IntPtr Music);
            #endregion
        }
    }
}

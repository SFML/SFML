using System;
using System.Runtime.InteropServices;
using System.Security;

namespace SFML
{
    namespace Audio
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// SoundStream is a streamed sound, ie. samples are acquired
        /// while the sound is playing. Use it for big sounds that would
        /// require hundreds of MB in memory (see Music),
        /// or for streaming sound from the network
        /// </summary>
        ////////////////////////////////////////////////////////////
        public abstract class SoundStream : ObjectBase
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Default constructor
            /// </summary>
            ////////////////////////////////////////////////////////////
            public SoundStream() :
                base(IntPtr.Zero)
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Play the sound stream
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void Play()
            {
                sfSoundStream_Play(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Pause the sound stream
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void Pause()
            {
                sfSoundStream_Pause(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Stop the sound stream
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void Stop()
            {
                sfSoundStream_Stop(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Samples rate, in samples per second
            /// </summary>
            ////////////////////////////////////////////////////////////
            public uint SampleRate
            {
                get {return sfSoundStream_GetSampleRate(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Number of channels (1 = mono, 2 = stereo)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public uint ChannelsCount
            {
                get {return sfSoundStream_GetChannelsCount(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Current status of the sound stream (see SoundStatus enum)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public SoundStatus Status
            {
                get {return sfSoundStream_GetStatus(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Loop state of the sound stream. Default value is false
            /// </summary>
            ////////////////////////////////////////////////////////////
            public bool Loop
            {
                get {return sfSoundStream_GetLoop(This);}
                set {sfSoundStream_SetLoop(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Pitch of the sound stream. Default value is 1
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float Pitch
            {
                get {return sfSoundStream_GetPitch(This);}
                set {sfSoundStream_SetPitch(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Volume of the sound stream, in range [0, 100]. Default value is 100
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float Volume
            {
                get {return sfSoundStream_GetVolume(This);}
                set {sfSoundStream_SetVolume(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// 3D position of the sound stream. Default value is (0, 0, 0)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public Vector3 Position
            {
                get {Vector3 v; sfSoundStream_GetPosition(This, out v.X, out v.Y, out v.Z); return v;}
                set {sfSoundStream_SetPosition(This, value.X, value.Y, value.Z);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Is the sound stream's position relative to the listener's position,
            /// or is it absolute?
            /// Default value is false (absolute)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public bool RelativeToListener
            {
                get {return sfSoundStream_IsRelativeToListener(This);}
                set {sfSoundStream_SetRelativeToListener(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Minimum distance of the sound stream. Closer than this distance,
            /// the listener will hear the sound at its maximum volume.
            /// The default value is 1
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float MinDistance
            {
                get {return sfSoundStream_GetMinDistance(This);}
                set {sfSoundStream_SetMinDistance(This, value);}
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
                get {return sfSoundStream_GetAttenuation(This);}
                set {sfSoundStream_SetAttenuation(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Current playing position, in seconds
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float PlayingOffset
            {
                get {return sfSoundStream_GetPlayingOffset(This);}
                set {sfSoundStream_SetPlayingOffset(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Provide a string describing the object
            /// </summary>
            /// <returns>String description of the object</returns>
            ////////////////////////////////////////////////////////////
            public override string ToString()
            {
                return "[SoundStream]" +
                       " SampleRate(" + SampleRate + ")" +
                       " ChannelsCount(" + ChannelsCount + ")" +
                       " Status(" + Status + ")" +
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
            /// Set the audio stream parameters, you must call it before Play()
            /// </summary>
            /// <param name="sampleRate">Number of channels</param>
            /// <param name="channelsCount">Sample rate, in samples per second</param>
            ////////////////////////////////////////////////////////////
            protected void Initialize(uint channelsCount, uint sampleRate)
            {
                myGetDataCallback = new GetDataCallbackType(GetData);
                mySeekCallback    = new SeekCallbackType(Seek);
                SetThis(sfSoundStream_Create(myGetDataCallback, mySeekCallback, channelsCount, sampleRate, IntPtr.Zero));
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Virtual function called each time new audio data is needed to feed the stream
            /// </summary>
            /// <param name="samples">Array of samples to fill for the stream</param>
            /// <returns>True to continue playback, false to stop</returns>
            ////////////////////////////////////////////////////////////
            protected abstract bool OnGetData(out short[] samples);

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Virtual function called to seek into the stream
            /// </summary>
            /// <param name="timeOffset">New position, expressed in seconds</param>
            ////////////////////////////////////////////////////////////
            protected abstract void OnSeek(float timeOffset);

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Handle the destruction of the object
            /// </summary>
            /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
            ////////////////////////////////////////////////////////////
            protected override void Destroy(bool disposing)
            {
                sfSoundStream_Destroy(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Structure mapping the C library arguments passed to the data callback
            /// </summary>
            ////////////////////////////////////////////////////////////
            [StructLayout(LayoutKind.Sequential)]
            private struct Chunk
            {
                unsafe public short* samplesPtr;
                public uint          samplesCount;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Called each time new audio data is needed to feed the stream
            /// </summary>
            /// <param name="dataChunk">Data chunk to fill with new audio samples</param>
            /// <param name="userData">User data -- unused</param>
            /// <returns>True to continue playback, false to stop</returns>
            ////////////////////////////////////////////////////////////
            private bool GetData(ref Chunk dataChunk, IntPtr userData)
            {
                if (OnGetData(out myTempBuffer))
                {
                    unsafe
                    {
                        fixed (short* samplesPtr = myTempBuffer)
                        {
                            dataChunk.samplesPtr   = samplesPtr;
                            dataChunk.samplesCount = (uint)myTempBuffer.Length;
                        }
                    }

                    return true;
                }
                else
                {
                    return false;
                }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Called to seek in the stream
            /// </summary>
            /// <param name="timeOffset">New position, expressed in seconds</param>
            /// <param name="userData">User data -- unused</param>
            /// <returns>If false is returned, the playback is aborted</returns>
            ////////////////////////////////////////////////////////////
            private void Seek(float timeOffset, IntPtr userData)
            {
                OnSeek(timeOffset);
            }

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate bool GetDataCallbackType(ref Chunk dataChunk, IntPtr UserData);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void SeekCallbackType(float timeOffset, IntPtr UserData);

            private GetDataCallbackType myGetDataCallback;
            private SeekCallbackType    mySeekCallback;
            private short[]             myTempBuffer;

            #region Imports
            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfSoundStream_Create(GetDataCallbackType OnGetData, SeekCallbackType OnSeek, uint ChannelsCount, uint SampleRate, IntPtr UserData);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSoundStream_Destroy(IntPtr SoundStreamStream);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSoundStream_Play(IntPtr SoundStream);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSoundStream_Pause(IntPtr SoundStream);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSoundStream_Stop(IntPtr SoundStream);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern SoundStatus sfSoundStream_GetStatus(IntPtr SoundStream);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern uint sfSoundStream_GetChannelsCount(IntPtr SoundStream);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern uint sfSoundStream_GetSampleRate(IntPtr SoundStream);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSoundStream_SetLoop(IntPtr SoundStream, bool Loop);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSoundStream_SetPitch(IntPtr SoundStream, float Pitch);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSoundStream_SetVolume(IntPtr SoundStream, float Volume);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSoundStream_SetPosition(IntPtr SoundStream, float X, float Y, float Z);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSoundStream_SetRelativeToListener(IntPtr SoundStream, bool Relative);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSoundStream_SetMinDistance(IntPtr SoundStream, float MinDistance);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSoundStream_SetAttenuation(IntPtr SoundStream, float Attenuation);
            
            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSoundStream_SetPlayingOffset(IntPtr SoundStream, float TimeOffset);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfSoundStream_GetLoop(IntPtr SoundStream);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSoundStream_GetPitch(IntPtr SoundStream);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSoundStream_GetVolume(IntPtr SoundStream);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSoundStream_GetPosition(IntPtr SoundStream, out float X, out float Y, out float Z);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfSoundStream_IsRelativeToListener(IntPtr SoundStream);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSoundStream_GetMinDistance(IntPtr SoundStream);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSoundStream_GetAttenuation(IntPtr SoundStream);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSoundStream_GetPlayingOffset(IntPtr SoundStream);
            #endregion
        }
    }
}
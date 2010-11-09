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
        /// SoundBuffer is the low-level class for loading and manipulating
        /// sound buffers. A sound buffer holds audio data (samples)
        /// which can then be played by a Sound or saved to a file.
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class SoundBuffer : ObjectBase
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the sound buffer from a file
            /// </summary>
            /// <param name="filename">Path of the sound file to load</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public SoundBuffer(string filename) :
                base(sfSoundBuffer_CreateFromFile(filename))
            {
                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("sound buffer", filename);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the sound buffer from a file in a stream
            /// </summary>
            /// <param name="stream">Stream containing the file contents</param>
            ////////////////////////////////////////////////////////////
            public SoundBuffer(Stream stream) :
                base(IntPtr.Zero)
            {
                stream.Position = 0;
                byte[] StreamData = new byte[stream.Length];
                uint Read = (uint)stream.Read(StreamData, 0, StreamData.Length);
                unsafe
                {
                    fixed (byte* dataPtr = StreamData)
                    {
                        SetThis(sfSoundBuffer_CreateFromMemory((char*)dataPtr, Read));
                    }
                }
                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("sound buffer");
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the sound buffer from an array of samples
            /// </summary>
            /// <param name="samples">Array of samples</param>
            /// <param name="channelsCount">Channels count</param>
            /// <param name="sampleRate">Sample rate</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public SoundBuffer(short[] samples, uint channelsCount, uint sampleRate) :
                base(IntPtr.Zero)
            {
                unsafe
                {
                    fixed (short* SamplesPtr = samples)
                    {
                        SetThis(sfSoundBuffer_CreateFromSamples(SamplesPtr, (uint)samples.Length, channelsCount, sampleRate));
                    }
                }

                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("sound buffer");
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the sound buffer from another sound buffer
            /// </summary>
            /// <param name="copy">Sound buffer to copy</param>
            ////////////////////////////////////////////////////////////
            public SoundBuffer(SoundBuffer copy) :
                base(sfSoundBuffer_Copy(copy.This))
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Save the sound buffer to an audio file
            /// </summary>
            /// <param name="filename">Path of the sound file to write</param>
            /// <returns>True if saving has been successful</returns>
            ////////////////////////////////////////////////////////////
            public bool SaveToFile(string filename)
            {
                return sfSoundBuffer_SaveToFile(This, filename);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Samples rate, in samples per second
            /// </summary>
            ////////////////////////////////////////////////////////////
            public uint SampleRate
            {
                get {return sfSoundBuffer_GetSampleRate(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Number of channels (1 = mono, 2 = stereo)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public uint ChannelsCount
            {
                get {return sfSoundBuffer_GetChannelsCount(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Total duration of the buffer, in seconds
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float Duration
            {
                get {return sfSoundBuffer_GetDuration(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Array of samples contained in the buffer
            /// </summary>
            ////////////////////////////////////////////////////////////
            public short[] Samples
            {
                get
                {
                    short[] SamplesArray = new short[sfSoundBuffer_GetSamplesCount(This)];
                    Marshal.Copy(sfSoundBuffer_GetSamples(This), SamplesArray, 0, SamplesArray.Length);
                    return SamplesArray;
                }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Provide a string describing the object
            /// </summary>
            /// <returns>String description of the object</returns>
            ////////////////////////////////////////////////////////////
            public override string ToString()
            {
                return "[SoundBuffer]" +
                       " SampleRate(" + SampleRate + ")" +
                       " ChannelsCount(" + ChannelsCount + ")" +
                       " Duration(" + Duration + ")";
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Handle the destruction of the object
            /// </summary>
            /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
            ////////////////////////////////////////////////////////////
            protected override void Destroy(bool disposing)
            {
                sfSoundBuffer_Destroy(This);
            }

            #region Imports
            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfSoundBuffer_CreateFromFile(string Filename);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            unsafe static extern IntPtr sfSoundBuffer_CreateFromMemory(char* Data, uint SizeInBytes);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            unsafe static extern IntPtr sfSoundBuffer_CreateFromSamples(short* Samples, uint SamplesCount, uint ChannelsCount, uint SampleRate);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfSoundBuffer_Copy(IntPtr SoundBuffer);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSoundBuffer_Destroy(IntPtr SoundBuffer);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfSoundBuffer_SaveToFile(IntPtr SoundBuffer, string Filename);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfSoundBuffer_GetSamples(IntPtr SoundBuffer);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern uint sfSoundBuffer_GetSamplesCount(IntPtr SoundBuffer);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern uint sfSoundBuffer_GetSampleRate(IntPtr SoundBuffer);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern uint sfSoundBuffer_GetChannelsCount(IntPtr SoundBuffer);

            [DllImport("csfml-audio-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSoundBuffer_GetDuration(IntPtr SoundBuffer);
            #endregion
        }
    }
}

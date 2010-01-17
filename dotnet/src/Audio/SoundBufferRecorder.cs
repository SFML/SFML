using System;
using System.Runtime.InteropServices;
using System.Security;
using System.Collections.Generic;

namespace SFML
{
    namespace Audio
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Specialized SoundRecorder which saves the captured
        /// audio data into a sound buffer
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class SoundBufferRecorder : SoundRecorder
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Sound buffer containing the recorded data (invalid until the capture stops)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public SoundBuffer SoundBuffer
            {
                get
                {
                    return mySoundBuffer;
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
                return "[SoundBufferRecorder]" +
                       " SampleRate(" + SampleRate + ")" +
                       " SoundBuffer(" + SoundBuffer + ")";
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Called when a new capture starts
            /// </summary>
            /// <returns>False to abort recording audio data, true to continue</returns>
            ////////////////////////////////////////////////////////////
            protected override bool OnStart()
            {
                mySamplesArray.Clear();
                return true;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Process a new chunk of recorded samples
            /// </summary>
            /// <param name="samples">Array of samples to process</param>
            /// <returns>False to stop recording audio data, true to continue</returns>
            ////////////////////////////////////////////////////////////
            protected override bool OnProcessSamples(short[] samples)
            {
                mySamplesArray.AddRange(samples);
                return true;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Called when the current capture stops
            /// </summary>
            ////////////////////////////////////////////////////////////
            protected override void OnStop()
            {
                mySoundBuffer = new SoundBuffer(mySamplesArray.ToArray(), 1, SampleRate);
            }

            List<short> mySamplesArray = new List<short>();
            SoundBuffer mySoundBuffer  = null;
        }
    }
}

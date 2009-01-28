using System;
using System.Runtime.InteropServices;

namespace SFML
{
    namespace Window
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// VideoMode defines a video mode (width, height, bpp, frequency)
        /// and provides static functions for getting modes supported
        /// by the display device
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Sequential)]
        public struct VideoMode
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the video mode with its width and height
            /// </summary>
            /// <param name="width">Video mode width</param>
            /// <param name="height">Video mode height</param>
            ////////////////////////////////////////////////////////////
            public VideoMode(uint width, uint height) :
                this(width, height, 32)
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the video mode with its width, height and depth
            /// </summary>
            /// <param name="width">Video mode width</param>
            /// <param name="height">Video mode height</param>
            /// <param name="bpp">Video mode depth (bits per pixel)</param>
            ////////////////////////////////////////////////////////////
            public VideoMode(uint width, uint height, uint bpp)
            {
                Width        = width;
                Height       = height;
                BitsPerPixel = bpp;
            }

            /// <summary>Video mode width, in pixels</summary>
            public uint Width;

            /// <summary>Video mode height, in pixels</summary>
            public uint Height;

            /// <summary>Video mode depth, in bits per pixel</summary>
            public uint BitsPerPixel;
        }
    }
}

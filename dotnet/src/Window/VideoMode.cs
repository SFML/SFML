using System;
using System.Runtime.InteropServices;
using System.Security;

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

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Tell whether or not the video mode is supported
            /// </summary>
            /// <returns>True if the video mode is valid, false otherwise</returns>
            ////////////////////////////////////////////////////////////
            public bool IsValid()
            {
                return sfVideoMode_IsValid(this);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get the number of valid video modes
            /// <summary>
            ////////////////////////////////////////////////////////////
            public static uint ModesCount
            {
                get {return sfVideoMode_GetModesCount();}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get a valid video mode.
            /// Index must be in range [0, ModesCount[.
            /// Modes are sorted from best to worst
            /// </summary>
            /// <param name="index">Index of the video mode to get</param>
            /// <returns>index-th video mode</returns>
            ////////////////////////////////////////////////////////////
            public static VideoMode GetMode(uint index)
            {
                return sfVideoMode_GetMode(index);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get the current desktop video mode
            /// <summary>
            ////////////////////////////////////////////////////////////
            public static VideoMode DesktopMode
            {
                get {return sfVideoMode_GetDesktopMode();}
            }

            /// <summary>Video mode width, in pixels</summary>
            public uint Width;

            /// <summary>Video mode height, in pixels</summary>
            public uint Height;

            /// <summary>Video mode depth, in bits per pixel</summary>
            public uint BitsPerPixel;

            #region Imports
            [DllImport("csfml-window"), SuppressUnmanagedCodeSecurity]
            static extern VideoMode sfVideoMode_GetDesktopMode();

            [DllImport("csfml-window"), SuppressUnmanagedCodeSecurity]
            static extern uint sfVideoMode_GetModesCount();

            [DllImport("csfml-window"), SuppressUnmanagedCodeSecurity]
            static extern VideoMode sfVideoMode_GetMode(uint Index);

            [DllImport("csfml-window"), SuppressUnmanagedCodeSecurity]
            static extern bool sfVideoMode_IsValid(VideoMode Mode);
            #endregion
        }
    }
}

using System;
using System.Runtime.InteropServices;

namespace SFML
{
    namespace Window
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Structure defining the creation settings of OpenGL contexts
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Sequential)]
        public struct ContextSettings
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the settings from depth / stencil bits
            /// </summary>
            /// <param name="depthBits">Depth buffer bits</param>
            /// <param name="stencilBits">Stencil buffer bits</param>
            ////////////////////////////////////////////////////////////
            public ContextSettings(uint depthBits, uint stencilBits) :
                this(depthBits, stencilBits, 0)
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the settings from depth / stencil bits and antialiasing level
            /// </summary>
            /// <param name="depthBits">Depth buffer bits</param>
            /// <param name="stencilBits">Stencil buffer bits</param>
            /// <param name="antialiasingLevel">Antialiasing level</param>
            ////////////////////////////////////////////////////////////
            public ContextSettings(uint depthBits, uint stencilBits, uint antialiasingLevel)
            {
                DepthBits         = depthBits;
                StencilBits       = stencilBits;
                AntialiasingLevel = antialiasingLevel;
            }

            /// <summary>Depth buffer bits (0 is disabled)</summary>
            public uint DepthBits;

            /// <summary>Stencil buffer bits (0 is disabled)</summary>
            public uint StencilBits;

            /// <summary>Antialiasing level (0 is disabled)</summary>
            public uint AntialiasingLevel;
        }
    }
}

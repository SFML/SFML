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
            public ContextSettings(uint depthBits, uint stencilBits, uint antialiasingLevel) :
                this(depthBits, stencilBits, antialiasingLevel, 2, 0)
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the settings from depth / stencil bits and antialiasing level
            /// </summary>
            /// <param name="depthBits">Depth buffer bits</param>
            /// <param name="stencilBits">Stencil buffer bits</param>
            /// <param name="antialiasingLevel">Antialiasing level</param>
            /// <param name="majorVersion">Major number of the context version</param>
            /// <param name="minorVersion">Minor number of the context version</param>
            ////////////////////////////////////////////////////////////
            public ContextSettings(uint depthBits, uint stencilBits, uint antialiasingLevel, uint majorVersion, uint minorVersion)
            {
                DepthBits         = depthBits;
                StencilBits       = stencilBits;
                AntialiasingLevel = antialiasingLevel;
                MajorVersion      = majorVersion;
                MinorVersion      = minorVersion;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Provide a string describing the object
            /// </summary>
            /// <returns>String description of the object</returns>
            ////////////////////////////////////////////////////////////
            public override string ToString()
            {
                return "[ContextSettings]" +
                       " DepthBits(" + DepthBits + ")" +
                       " StencilBits(" + StencilBits + ")" +
                       " AntialiasingLevel(" + AntialiasingLevel + ")" +
                       " MajorVersion(" + MajorVersion + ")" +
                       " MinorVersion(" + MinorVersion + ")";
            }

            /// <summary>Depth buffer bits (0 is disabled)</summary>
            public uint DepthBits;

            /// <summary>Stencil buffer bits (0 is disabled)</summary>
            public uint StencilBits;

            /// <summary>Antialiasing level (0 is disabled)</summary>
            public uint AntialiasingLevel;

            /// <summary>Major number of the context version</summary>
            public uint MajorVersion;

            /// <summary>Minor number of the context version</summary>
            public uint MinorVersion;
        }
    }
}

using System;
using System.Runtime.InteropServices;

namespace SFML
{
    namespace Graphics
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Utility class for manipulating 32-bits RGBA colors
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Sequential)]
        public struct Color
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the color from its red, green and blue components
            /// </summary>
            /// <param name="red">Red component</param>
            /// <param name="green">Green component</param>
            /// <param name="blue">Blue component</param>
            ////////////////////////////////////////////////////////////
            public Color(byte red, byte green, byte blue) :
                this(red, green, blue, 255)
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the color from its red, green, blue and alpha components
            /// </summary>
            /// <param name="red">Red component</param>
            /// <param name="green">Green component</param>
            /// <param name="blue">Blue component</param>
            /// <param name="alpha">Alpha (transparency) component</param>
            ////////////////////////////////////////////////////////////
            public Color(byte red, byte green, byte blue, byte alpha)
            {
                R = red;
                G = green;
                B = blue;
                A = alpha;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the color from another
            /// </summary>
            /// <param name="color">Color to copy</param>
            ////////////////////////////////////////////////////////////
            public Color(Color color) :
                this(color.R, color.G, color.B, color.A)
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Provide a string describing the object
            /// </summary>
            /// <returns>String description of the object</returns>
            ////////////////////////////////////////////////////////////
            public override string ToString()
            {
                return "[Color]" +
                       " R(" + R + ")" +
                       " G(" + G + ")" +
                       " B(" + B + ")" +
                       " A(" + A + ")";
            }

            /// <summary>Red component of the color</summary>
            public byte R;

            /// <summary>Green component of the color</summary>
            public byte G;

            /// <summary>Blue component of the color</summary>
            public byte B;

            /// <summary>Alpha (transparent) component of the color</summary>
            public byte A;


            /// <summary>Predefined black color</summary>
            public static readonly Color Black = new Color(0, 0, 0);

            /// <summary>Predefined white color</summary>
            public static readonly Color White = new Color(255, 255, 255);

            /// <summary>Predefined red color</summary>
            public static readonly Color Red = new Color(255, 0, 0);

            /// <summary>Predefined green color</summary>
            public static readonly Color Green = new Color(0, 255, 0);

            /// <summary>Predefined blue color</summary>
            public static readonly Color Blue = new Color(0, 0, 255);

            /// <summary>Predefined yellow color</summary>
            public static readonly Color Yellow = new Color(255, 255, 0);

            /// <summary>Predefined magenta color</summary>
            public static readonly Color Magenta = new Color(255, 0, 255);

            /// <summary>Predefined cyan color</summary>
            public static readonly Color Cyan = new Color(0, 255, 255);
        }
    }
}

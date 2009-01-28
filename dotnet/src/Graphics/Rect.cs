using System;
using System.Runtime.InteropServices;

namespace SFML
{
    namespace Graphics
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// IntRect is an utility class for manipulating 2D rectangles
        /// with integer coordinates
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Sequential)]
        public struct IntRect
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the rectangle from its coordinates
            /// </summary>
            /// <param name="left">Left coordinate of the rectangle</param>
            /// <param name="top">Top coordinate of the rectangle</param>
            /// <param name="right">Right coordinate of the rectangle</param>
            /// <param name="bottom">Bottom coordinate of the rectangle</param>
            ////////////////////////////////////////////////////////////
            public IntRect(int left, int top, int right, int bottom)
            {
                Left   = left;
                Top    = top;
                Right  = right;
                Bottom = bottom;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Width of the rectangle
            /// </summary>
            ////////////////////////////////////////////////////////////
            public int Width
            {
                get {return Right - Left;}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Height of the rectangle
            /// </summary>
            ////////////////////////////////////////////////////////////
            public int Height
            {
                get {return Bottom - Top;}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Move the whole rectangle by the given offset
            /// </summary>
            /// <param name="offsetX">Horizontal offset</param>
            /// <param name="offsetY">Vertical offset</param>
            ////////////////////////////////////////////////////////////
            public void Offset(int offsetX, int offsetY)
            {
                Left   += offsetX;
                Top    += offsetY;
                Right  += offsetX;
                Bottom += offsetY;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Check if a point is inside the rectangle's area
            /// </summary>
            /// <param name="x">X coordinate of the point to test</param>
            /// <param name="y">Y coordinate of the point to test</param>
            /// <returns>True if the point is inside</returns>
            ////////////////////////////////////////////////////////////
            public bool Contains(int x, int y)
            {
                return (x >= Left) && (x <= Right) && (y >= Top) && (y <= Bottom);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Check intersection between two rectangles
            /// </summary>
            /// <param name="rect"> Rectangle to test</param>
            /// <returns>True if rectangles overlap</returns>
            ////////////////////////////////////////////////////////////
            public bool Intersects(IntRect rect)
            {
                return ((Math.Max(Left, rect.Left) < Math.Min(Right,  rect.Right)) &&
                        (Math.Max(Top,  rect.Top)  < Math.Min(Bottom, rect.Bottom)));
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Check intersection between two rectangles
            /// </summary>
            /// <param name="rect"> Rectangle to test</param>
            /// <param name="overlap">Rectangle to be filled with overlapping rect</param>
            /// <returns>True if rectangles overlap</returns>
            ////////////////////////////////////////////////////////////
            public bool Intersects(IntRect rect, out IntRect overlap)
            {
                IntRect Overlapping = new IntRect(Math.Max(Left,   rect.Left),
                                                  Math.Max(Top,    rect.Top),
                                                  Math.Min(Right,  rect.Right),
                                                  Math.Min(Bottom, rect.Bottom));

                if ((Overlapping.Left < Overlapping.Right) && (Overlapping.Top < Overlapping.Bottom))
                {
                    overlap.Left   = Overlapping.Left;
                    overlap.Top    = Overlapping.Top;
                    overlap.Right  = Overlapping.Right;
                    overlap.Bottom = Overlapping.Bottom;
                    return true;
                }
                else
                {
                    overlap.Left   = 0;
                    overlap.Top    = 0;
                    overlap.Right  = 0;
                    overlap.Bottom = 0;
                    return false;
                }
            }

            /// <summary>Left coordinate of the rectangle</summary>
            public int Left;

            /// <summary>Top coordinate of the rectangle</summary>
            public int Top;

            /// <summary>Right coordinate of the rectangle</summary>
            public int Right;

            /// <summary>Bottom coordinate of the rectangle</summary>
            public int Bottom;
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// FloatRect is an utility class for manipulating 2D rectangles
        /// with float coordinates
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Sequential)]
        public struct FloatRect
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the rectangle from its coordinates
            /// </summary>
            /// <param name="left">Left coordinate of the rectangle</param>
            /// <param name="top">Top coordinate of the rectangle</param>
            /// <param name="right">Right coordinate of the rectangle</param>
            /// <param name="bottom">Bottom coordinate of the rectangle</param>
            ////////////////////////////////////////////////////////////
            public FloatRect(float left, float top, float right, float bottom)
            {
                Left   = left;
                Top    = top;
                Right  = right;
                Bottom = bottom;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Width of the rectangle
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float Width
            {
                get {return Right - Left;}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Height of the rectangle
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float Height
            {
                get {return Bottom - Top;}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Move the whole rectangle by the given offset
            /// </summary>
            /// <param name="offsetX">Horizontal offset</param>
            /// <param name="offsetY">Vertical offset</param>
            ////////////////////////////////////////////////////////////
            public void Offset(float offsetX, float offsetY)
            {
                Left   += offsetX;
                Top    += offsetY;
                Right  += offsetX;
                Bottom += offsetY;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Check if a point is inside the rectangle's area
            /// </summary>
            /// <param name="x">X coordinate of the point to test</param>
            /// <param name="y">Y coordinate of the point to test</param>
            /// <returns>True if the point is inside</returns>
            ////////////////////////////////////////////////////////////
            public bool Contains(float x, float y)
            {
                return (x >= Left) && (x <= Right) && (y >= Top) && (y <= Bottom);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Check intersection between two rectangles
            /// </summary>
            /// <param name="rect"> Rectangle to test</param>
            /// <returns>True if rectangles overlap</returns>
            ////////////////////////////////////////////////////////////
            public bool Intersects(FloatRect rect)
            {
                return ((Math.Max(Left, rect.Left) < Math.Min(Right,  rect.Right)) &&
                        (Math.Max(Top,  rect.Top)  < Math.Min(Bottom, rect.Bottom)));
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Check intersection between two rectangles
            /// </summary>
            /// <param name="rect"> Rectangle to test</param>
            /// <param name="overlap">Rectangle to be filled with overlapping rect</param>
            /// <returns>True if rectangles overlap</returns>
            ////////////////////////////////////////////////////////////
            public bool Intersects(FloatRect rect, out FloatRect overlap)
            {
                FloatRect Overlapping = new FloatRect(Math.Max(Left,   rect.Left),
                                                      Math.Max(Top,    rect.Top),
                                                      Math.Min(Right,  rect.Right),
                                                      Math.Min(Bottom, rect.Bottom));

                if ((Overlapping.Left < Overlapping.Right) && (Overlapping.Top < Overlapping.Bottom))
                {
                    overlap.Left   = Overlapping.Left;
                    overlap.Top    = Overlapping.Top;
                    overlap.Right  = Overlapping.Right;
                    overlap.Bottom = Overlapping.Bottom;
                    return true;
                }
                else
                {
                    overlap.Left   = 0;
                    overlap.Top    = 0;
                    overlap.Right  = 0;
                    overlap.Bottom = 0;
                    return false;
                }
            }

            /// <summary>Left coordinate of the rectangle</summary>
            public float Left;

            /// <summary>Top coordinate of the rectangle</summary>
            public float Top;

            /// <summary>Right coordinate of the rectangle</summary>
            public float Right;

            /// <summary>Bottom coordinate of the rectangle</summary>
            public float Bottom;
        }
    }
}

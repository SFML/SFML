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
            /// <param name="width">Width of the rectangle</param>
            /// <param name="height">Height of the rectangle</param>
            ////////////////////////////////////////////////////////////
            public IntRect(int left, int top, int width, int height)
            {
                Left   = left;
                Top    = top;
                Width  = width;
                Height = height;
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
                return (x >= Left) && (x < Left + Width) && (y >= Top) && (y < Top + Height);
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
                // Compute the intersection boundaries
                int left   = Math.Max(Left,         rect.Left);
                int top    = Math.Max(Top,          rect.Top);
                int right  = Math.Min(Left + Width, rect.Left + rect.Width);
                int bottom = Math.Min(Top + Height, rect.Top + rect.Height);
            
                return (left < right) && (top < bottom);
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
                // Compute the intersection boundaries
                int left   = Math.Max(Left,         rect.Left);
                int top    = Math.Max(Top,          rect.Top);
                int right  = Math.Min(Left + Width, rect.Left + rect.Width);
                int bottom = Math.Min(Top + Height, rect.Top + rect.Height);

                // If the intersection is valid (positive non zero area), then there is an intersection
                if ((left < right) && (top < bottom))
                {
                    overlap.Left   = left;
                    overlap.Top    = top;
                    overlap.Width  = right - left;
                    overlap.Height = bottom - top;
                    return true;
                }
                else
                {
                    overlap.Left   = 0;
                    overlap.Top    = 0;
                    overlap.Width  = 0;
                    overlap.Height = 0;
                    return false;
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
                return "[IntRect]" +
                       " Left(" + Left + ")" +
                       " Top(" + Top + ")" +
                       " Width(" + Width + ")" +
                       " Height(" + Height + ")";
            }

            /// <summary>Left coordinate of the rectangle</summary>
            public int Left;

            /// <summary>Top coordinate of the rectangle</summary>
            public int Top;

            /// <summary>Width of the rectangle</summary>
            public int Width;

            /// <summary>Height of the rectangle</summary>
            public int Height;
        }
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// IntRect is an utility class for manipulating 2D rectangles
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
            /// <param name="width">Width of the rectangle</param>
            /// <param name="height">Height of the rectangle</param>
            ////////////////////////////////////////////////////////////
            public FloatRect(float left, float top, float width, float height)
            {
                Left   = left;
                Top    = top;
                Width  = width;
                Height = height;
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
                return (x >= Left) && (x < Left + Width) && (y >= Top) && (y < Top + Height);
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
                // Compute the intersection boundaries
                float left   = Math.Max(Left,         rect.Left);
                float top    = Math.Max(Top,          rect.Top);
                float right  = Math.Min(Left + Width, rect.Left + rect.Width);
                float bottom = Math.Min(Top + Height, rect.Top + rect.Height);
            
                return (left < right) && (top < bottom);
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
                // Compute the intersection boundaries
                float left   = Math.Max(Left,         rect.Left);
                float top    = Math.Max(Top,          rect.Top);
                float right  = Math.Min(Left + Width, rect.Left + rect.Width);
                float bottom = Math.Min(Top + Height, rect.Top + rect.Height);

                // If the intersection is valid (positive non zero area), then there is an intersection
                if ((left < right) && (top < bottom))
                {
                    overlap.Left   = left;
                    overlap.Top    = top;
                    overlap.Width  = right - left;
                    overlap.Height = bottom - top;
                    return true;
                }
                else
                {
                    overlap.Left   = 0;
                    overlap.Top    = 0;
                    overlap.Width  = 0;
                    overlap.Height = 0;
                    return false;
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
                return "[FloatRect]" +
                       " Left(" + Left + ")" +
                       " Top(" + Top + ")" +
                       " Width(" + Width + ")" +
                       " Height(" + Height + ")";
            }

            /// <summary>Left coordinate of the rectangle</summary>
            public float Left;

            /// <summary>Top coordinate of the rectangle</summary>
            public float Top;

            /// <summary>Width of the rectangle</summary>
            public float Width;

            /// <summary>Height of the rectangle</summary>
            public float Height;
        }
    }
}

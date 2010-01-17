using System;
using System.Runtime.InteropServices;

namespace SFML
{
    namespace Audio
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Vector3 is an utility class for manipulating 3 dimensional
        /// vectors with float components
        /// </summary>
        ////////////////////////////////////////////////////////////
        [StructLayout(LayoutKind.Sequential)]
        public struct Vector3
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the vector from its coordinates
            /// </summary>
            /// <param name="x">X coordinate</param>
            /// <param name="y">Y coordinate</param>
            /// <param name="z">Z coordinate</param>
            ////////////////////////////////////////////////////////////
            public Vector3(float x, float y, float z)
            {
                X = x;
                Y = y;
                Z = z;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Operator - overload ; returns the opposite of a vector
            /// </summary>
            /// <param name="v">Vector to negate</param>
            /// <returns>-v</returns>
            ////////////////////////////////////////////////////////////
            public static Vector3 operator -(Vector3 v)
            {
                return new Vector3(-v.X, -v.Y, -v.Z);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Operator - overload ; subtracts two vectors
            /// </summary>
            /// <param name="v1">First vector</param>
            /// <param name="v2">Second vector</param>
            /// <returns>v1 - v2</returns>
            ////////////////////////////////////////////////////////////
            public static Vector3 operator -(Vector3 v1, Vector3 v2)
            {
                return new Vector3(v1.X - v2.X, v1.Y - v2.X, v1.Z - v2.Z);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Operator + overload ; add two vectors
            /// </summary>
            /// <param name="v1">First vector</param>
            /// <param name="v2">Second vector</param>
            /// <returns>v1 + v2</returns>
            ////////////////////////////////////////////////////////////
            public static Vector3 operator +(Vector3 v1, Vector3 v2)
            {
                return new Vector3(v1.X + v2.X, v1.Y + v2.X, v1.Z + v2.Z);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Operator * overload ; multiply a vector by a scalar value
            /// </summary>
            /// <param name="v">Vector</param>
            /// <param name="x">Scalar value</param>
            /// <returns>v * x</returns>
            ////////////////////////////////////////////////////////////
            public static Vector3 operator *(Vector3 v, float x)
            {
                return new Vector3(v.X * x, v.Y * x, v.Z * x);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Operator * overload ; multiply a scalar value by a vector
            /// </summary>
            /// <param name="x">Scalar value</param>
            /// <param name="v">Vector</param>
            /// <returns>x * v</returns>
            ////////////////////////////////////////////////////////////
            public static Vector3 operator *(float x, Vector3 v)
            {
                return new Vector3(v.X * x, v.Y * x, v.Z * x);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Operator / overload ; divide a vector by a scalar value
            /// </summary>
            /// <param name="v">Vector</param>
            /// <param name="x">Scalar value</param>
            /// <returns>v / x</returns>
            ////////////////////////////////////////////////////////////
            public static Vector3 operator /(Vector3 v, float x)
            {
                return new Vector3(v.X / x, v.Y / x, v.Z / x);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Provide a string describing the object
            /// </summary>
            /// <returns>String description of the object</returns>
            ////////////////////////////////////////////////////////////
            public override string ToString()
            {
                return "[Vector3]" +
                       " X(" + X + ")" +
                       " Y(" + Y + ")" +
                       " Z(" + Z + ")";
            }

            /// <summary>X (horizontal) component of the vector</summary>
            public float X;
 
            /// <summary>Y (vertical) component of the vector</summary>
            public float Y;
 
            /// <summary>Z (depth) component of the vector</summary>
            public float Z;
        }
    }
}

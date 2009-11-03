using System;
using System.Runtime.InteropServices;

namespace SFML
{
    namespace Graphics
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Enumerate the blending modes available for drawable objects
        /// </summary>
        ////////////////////////////////////////////////////////////
        public enum BlendMode
        {
            /// <summary>Pixel = Src * a + Dest * (1 - a)</summary>
            Alpha,

            /// <summary>Pixel = Src + Dest</summary>
            Add,

            /// <summary>Pixel = Src * Dest</summary>
            Multiply,

            /// <summary>No blending</summary>
            None
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Abstract base class for every object that can be drawn
        /// into a render window
        /// </summary>
        ////////////////////////////////////////////////////////////
        public abstract class Drawable : ObjectBase
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Position of the object on screen
            /// </summary>
            ////////////////////////////////////////////////////////////
            public abstract Vector2 Position {get; set;}

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Rotation of the object, defined in degrees
            /// </summary>
            ////////////////////////////////////////////////////////////
            public abstract float Rotation {get; set;}

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Vertical and horizontal scale of the object
            /// </summary>
            ////////////////////////////////////////////////////////////
            public abstract Vector2 Scale {get; set;}

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Origin of the transformation of the object
            /// (center of translation, rotation and scale)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public abstract Vector2 Origin {get; set;}

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Global color of the object
            /// </summary>
            ////////////////////////////////////////////////////////////
            public abstract Color Color {get; set;}

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Blending mode of the object
            /// </summary>
            ////////////////////////////////////////////////////////////
            public abstract BlendMode BlendMode {get; set;}

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Transform a point from global coordinates into local coordinates
            /// (ie it applies the inverse of object's origin, translation, rotation and scale to the point)
            /// </summary>
            /// <param name="point">Point to transform</param>
            /// <returns>Transformed point</returns>
            ////////////////////////////////////////////////////////////
            public abstract Vector2 TransformToLocal(Vector2 point);

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Transform a point from local coordinates into global coordinates
            /// (ie it applies the object's origin, translation, rotation and scale to the point)
            /// </summary>
            /// <param name="point">Point to transform</param>
            /// <returns>Transformed point</returns>
            ////////////////////////////////////////////////////////////
            public abstract Vector2 TransformToGlobal(Vector2 point);

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Render the object into the given render window
            /// </summary>
            /// <param name="target">Target render window</param>
            /// <param name="shader">Shader to apply</param>
            ////////////////////////////////////////////////////////////
            internal abstract void Render(RenderWindow target, Shader shader);

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Render the object into the given render image
            /// </summary>
            /// <param name="target">Target render image</param>
            /// <param name="shader">Shader to apply</param>
            ////////////////////////////////////////////////////////////
            internal abstract void Render(RenderImage target, Shader shader);

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Internal constructor, for derived classes
            /// </summary>
            /// <param name="thisPtr">Pointer to the object in C library</param>
            ////////////////////////////////////////////////////////////
            protected Drawable(IntPtr thisPtr) :
                base(thisPtr)
            {
            }
        }
    }
}

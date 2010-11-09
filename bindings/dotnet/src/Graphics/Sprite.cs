using System;
using System.Security;
using System.Runtime.InteropServices;

namespace SFML
{
    namespace Graphics
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// This class defines a sprite : texture, transformations,
        /// color, and draw on screen
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class Sprite : Drawable
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Default constructor
            /// </summary>
            ////////////////////////////////////////////////////////////
            public Sprite() :
                base(sfSprite_Create())
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the sprite from a source image
            /// </summary>
            /// <param name="image">Source image to assign to the sprite</param>
            ////////////////////////////////////////////////////////////
            public Sprite(Image image) :
                base(sfSprite_Create())
            {
                Image = image;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the sprite from another sprite
            /// </summary>
            /// <param name="copy">Sprite to copy</param>
            ////////////////////////////////////////////////////////////
            public Sprite(Sprite copy) :
                base(sfSprite_Copy(copy.This))
            {
                Image = copy.Image;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Position of the object on screen
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override Vector2 Position
            {
                get { return new Vector2(sfSprite_GetX(This), sfSprite_GetY(This)); }
                set { sfSprite_SetPosition(This, value.X, value.Y); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Rotation of the object, defined in degrees
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override float Rotation
            {
                get { return sfSprite_GetRotation(This); }
                set { sfSprite_SetRotation(This, value); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Vertical and horizontal scale of the object
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override Vector2 Scale
            {
                get { return new Vector2(sfSprite_GetScaleX(This), sfSprite_GetScaleY(This)); }
                set { sfSprite_SetScale(This, value.X, value.Y); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Origin of the transformation of the object
            /// (center of translation, rotation and scale)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override Vector2 Origin
            {
                get { return new Vector2(sfSprite_GetOriginX(This), sfSprite_GetOriginY(This)); }
                set { sfSprite_SetOrigin(This, value.X, value.Y); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Global color of the object
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override Color Color
            {
                get { return sfSprite_GetColor(This); }
                set { sfSprite_SetColor(This, value); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Blending mode of the object
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override BlendMode BlendMode
            {
                get { return sfSprite_GetBlendMode(This); }
                set { sfSprite_SetBlendMode(This, value); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Transform a point from global coordinates into local coordinates
            /// (ie it applies the inverse of object's origin, translation, rotation and scale to the point)
            /// </summary>
            /// <param name="point">Point to transform</param>
            /// <returns>Transformed point</returns>
            ////////////////////////////////////////////////////////////
            public override Vector2 TransformToLocal(Vector2 point)
            {
                Vector2 Transformed;
                sfSprite_TransformToLocal(This, point.X, point.Y, out Transformed.X, out Transformed.Y);

                return Transformed;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Transform a point from local coordinates into global coordinates
            /// (ie it applies the object's origin, translation, rotation and scale to the point)
            /// </summary>
            /// <param name="point">Point to transform</param>
            /// <returns>Transformed point</returns>
            ////////////////////////////////////////////////////////////
            public override Vector2 TransformToGlobal(Vector2 point)
            {
                Vector2 Transformed;
                sfSprite_TransformToGlobal(This, point.X, point.Y, out Transformed.X, out Transformed.Y);

                return Transformed;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Width of the sprite
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float Width
            {
                get { return sfSprite_GetWidth(This); }
                set { sfSprite_Resize(This, value, this.Height); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Height of the sprite
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float Height
            {
                get { return sfSprite_GetHeight(This); }
                set { sfSprite_Resize(This, this.Width, value); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Source images displayed by the sprite
            /// </summary>
            ////////////////////////////////////////////////////////////
            public Image Image
            {
                get { return myImage; }
                set { myImage = value; sfSprite_SetImage(This, value != null ? value.This : IntPtr.Zero, false); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Sub-rectangle of the source image displayed by the sprite
            /// </summary>
            ////////////////////////////////////////////////////////////
            public IntRect SubRect
            {
                get { return sfSprite_GetSubRect(This); }
                set { sfSprite_SetSubRect(This, value); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Flip the sprite horizontically
            /// </summary>
            /// <param name="flipped">True to flip, false to canel flip</param>
            ////////////////////////////////////////////////////////////
            public void FlipX(bool flipped)
            {
                sfSprite_FlipX(This, flipped);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Flip the sprite vertically
            /// </summary>
            /// <param name="flipped">True to flip, false to canel flip</param>
            ////////////////////////////////////////////////////////////
            public void FlipY(bool flipped)
            {
                sfSprite_FlipY(This, flipped);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get the color of a given pixel in the sprite
            /// (point is in local coordinates)
            /// </summary>
            /// <param name="x">X coordinate of the pixel to get</param>
            /// <param name="y">Y coordinate of the pixel to get</param>
            /// <returns>Color of pixel (x, y)</returns>
            ////////////////////////////////////////////////////////////
            public Color GetPixel(uint x, uint y)
            {
                return sfSprite_GetPixel(This, x, y);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Provide a string describing the object
            /// </summary>
            /// <returns>String description of the object</returns>
            ////////////////////////////////////////////////////////////
            public override string ToString()
            {
                return "[Sprite]" +
                       " Position(" + Position + ")" +
                       " Rotation(" + Rotation + ")" +
                       " Scale(" + Scale + ")" +
                       " Origin(" + Origin + ")" +
                       " Color(" + Color + ")" +
                       " BlendMode(" + BlendMode + ")" +
                       " Width(" + Width + ")" +
                       " Height(" + Height + ")" +
                       " SubRect(" + SubRect + ")" +
                       " Image(" + Image + ")";
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Render the object into the given render window
            /// </summary>
            /// <param name="target">Target render window</param>
            /// <param name="shader">Shader to apply</param>
            ////////////////////////////////////////////////////////////
            internal override void Render(RenderWindow target, Shader shader)
            {
                if (shader == null)
                    sfRenderWindow_DrawSprite(target.This, This);
                else
                    sfRenderWindow_DrawSpriteWithShader(target.This, This, shader.This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Render the object into the given render image
            /// </summary>
            /// <param name="target">Target render image</param>
            /// <param name="shader">Shader to apply</param>
            ////////////////////////////////////////////////////////////
            internal override void Render(RenderImage target, Shader shader)
            {
                if (shader == null)
                    sfRenderImage_DrawSprite(target.This, This);
                else
                    sfRenderImage_DrawSpriteWithShader(target.This, This, shader.This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Handle the destruction of the object
            /// </summary>
            /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
            ////////////////////////////////////////////////////////////
            protected override void Destroy(bool disposing)
            {
                sfSprite_Destroy(This);
            }

            private Image myImage = null;

            #region Imports
            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfSprite_Create();

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfSprite_Copy(IntPtr Sprite);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSprite_Destroy(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSprite_SetPosition(IntPtr This, float X, float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSprite_GetX(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSprite_GetY(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSprite_SetRotation(IntPtr This, float Rotation);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSprite_GetRotation(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSprite_SetScale(IntPtr This, float X, float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSprite_GetScaleX(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSprite_GetScaleY(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSprite_SetOrigin(IntPtr This, float X, float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSprite_GetOriginX(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSprite_GetOriginY(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSprite_SetColor(IntPtr This, Color Color);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern Color sfSprite_GetColor(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSprite_SetBlendMode(IntPtr This, BlendMode Mode);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern BlendMode sfSprite_GetBlendMode(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern Vector2 sfSprite_TransformToLocal(IntPtr This, float PointX, float PointY, out float X, out float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern Vector2 sfSprite_TransformToGlobal(IntPtr This, float PointX, float PointY, out float X, out float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_DrawSprite(IntPtr This, IntPtr Sprite);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_DrawSpriteWithShader(IntPtr This, IntPtr Sprite, IntPtr Shader);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderImage_DrawSprite(IntPtr This, IntPtr Sprite);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderImage_DrawSpriteWithShader(IntPtr This, IntPtr Sprite, IntPtr Shader);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSprite_Resize(IntPtr This, float Width, float Height);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSprite_GetWidth(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfSprite_GetHeight(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSprite_SetImage(IntPtr This, IntPtr Image, bool AdjustToNewSize);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSprite_SetSubRect(IntPtr This, IntRect Rect);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntRect sfSprite_GetSubRect(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSprite_FlipX(IntPtr This, bool Flipped);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfSprite_FlipY(IntPtr This, bool Flipped);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern Color sfSprite_GetPixel(IntPtr This, uint X, uint Y);
            #endregion
        }
    }
}

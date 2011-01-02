using System;
using System.Runtime.InteropServices;
using System.Security;

namespace SFML
{
    namespace Graphics
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Shape defines a drawable convex shape ; it also defines
        /// helper functions to draw simple shapes like
        /// lines, rectangles, circles, etc.
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class Shape : Drawable
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Default constructor
            /// </summary>
            ////////////////////////////////////////////////////////////
            public Shape() :
                base(sfShape_Create())
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the shape from another shape
            /// </summary>
            /// <param name="copy">Shape to copy</param>
            ////////////////////////////////////////////////////////////
            public Shape(Shape copy) :
                base(sfShape_Copy(copy.This))
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Position of the object on screen
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override Vector2 Position
            {
                get { return new Vector2(sfShape_GetX(This), sfShape_GetY(This)); }
                set { sfShape_SetPosition(This, value.X, value.Y); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Rotation of the object, defined in degrees
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override float Rotation
            {
                get { return sfShape_GetRotation(This); }
                set { sfShape_SetRotation(This, value); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Vertical and horizontal scale of the object
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override Vector2 Scale
            {
                get { return new Vector2(sfShape_GetScaleX(This), sfShape_GetScaleY(This)); }
                set { sfShape_SetScale(This, value.X, value.Y); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Origin of the transformation of the object
            /// (center of translation, rotation and scale)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override Vector2 Origin
            {
                get { return new Vector2(sfShape_GetOriginX(This), sfShape_GetOriginY(This)); }
                set { sfShape_SetOrigin(This, value.X, value.Y); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Global color of the object
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override Color Color
            {
                get { return sfShape_GetColor(This); }
                set { sfShape_SetColor(This, value); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Blending mode of the object
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override BlendMode BlendMode
            {
                get { return sfShape_GetBlendMode(This); }
                set { sfShape_SetBlendMode(This, value); }
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
                sfShape_TransformToLocal(This, point.X, point.Y, out Transformed.X, out Transformed.Y);

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
                sfShape_TransformToGlobal(This, point.X, point.Y, out Transformed.X, out Transformed.Y);

                return Transformed;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Add a point to the shape
            /// </summary>
            /// <param name="position">Position of the point</param>
            /// <param name="color">Color of the point</param>
            ////////////////////////////////////////////////////////////
            public void AddPoint(Vector2 position, Color color)
            {
                AddPoint(position, color, Color.Black);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Add a point to the shape
            /// </summary>
            /// <param name="position">Position of the point</param>
            /// <param name="color">Color of the point</param>
            /// <param name="outlineColor">Outline color of the point</param>
            ////////////////////////////////////////////////////////////
            public void AddPoint(Vector2 position, Color color, Color outlineColor)
            {
                sfShape_AddPoint(This, position.X, position.Y, color, outlineColor);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Enable or disable filling the shape.
            /// Fill is enabled by default
            /// </summary>
            /// <param name="enable">True to enable, false to disable</param>
            ////////////////////////////////////////////////////////////
            public void EnableFill(bool enable)
            {
                sfShape_EnableFill(This, enable);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Enable or disable drawing the shape outline.
            /// Outline is enabled by default
            /// </summary>
            /// <param name="enable">True to enable, false to disable</param>
            ////////////////////////////////////////////////////////////
            public void EnableOutline(bool enable)
            {
                sfShape_EnableOutline(This, enable);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Thickness of the shape outline
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float OutlineThickness
            {
                get {return sfShape_GetOutlineThickness(This);}
                set {sfShape_SetOutlineThickness(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Total number of points of the shape
            /// </summary>
            ////////////////////////////////////////////////////////////
            public uint PointsCount
            {
                get {return sfShape_GetPointsCount(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Set the position of a point
            /// </summary>
            /// <param name="index">Index of the point, in range [0, NbPoints - 1]</param>
            /// <param name="position">New position of the index-th point</param>
            ////////////////////////////////////////////////////////////
            public void SetPointPosition(uint index, Vector2 position)
            {
                sfShape_SetPointPosition(This, index, position.X, position.Y);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get the position of a point
            /// </summary>
            /// <param name="index">Index of the point, in range [0, NbPoints - 1]</param>
            /// <returns>Position of the index-th point</returns>
            ////////////////////////////////////////////////////////////
            public Vector2 GetPointPosition(uint index)
            {
                Vector2 Pos;
                sfShape_GetPointPosition(This, index, out Pos.X, out Pos.Y);

                return Pos;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Set the color of a point
            /// </summary>
            /// <param name="index">Index of the point, in range [0, NbPoints - 1]</param>
            /// <param name="color">New color of the index-th point</param>
            ////////////////////////////////////////////////////////////
            public void SetPointColor(uint index, Color color)
            {
                sfShape_SetPointColor(This, index, color);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get the color of a point
            /// </summary>
            /// <param name="index">Index of the point, in range [0, NbPoints - 1]</param>
            /// <returns>Color of the index-th point</returns>
            ////////////////////////////////////////////////////////////
            public Color GetPointColor(uint index)
            {
                return sfShape_GetPointColor(This, index);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Set the outline color of a point
            /// </summary>
            /// <param name="index">Index of the point, in range [0, NbPoints - 1]</param>
            /// <param name="color">New outline color of the index-th point</param>
            ////////////////////////////////////////////////////////////
            public void SetPointOutlineColor(uint index, Color color)
            {
                sfShape_SetPointOutlineColor(This, index, color);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get the outline color of a point
            /// </summary>
            /// <param name="index">Index of the point, in range [0, NbPoints - 1]</param>
            /// <returns>Outline color of the index-th point</returns>
            ////////////////////////////////////////////////////////////
            public Color GetPointOutlineColor(uint index)
            {
                return sfShape_GetPointOutlineColor(This, index);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create a shape made of a single line
            /// </summary>
            /// <param name="p1">Position of the first point</param>
            /// <param name="p2">Position of the second point</param>
            /// <param name="thickness">Line thickness</param>
            /// <param name="color">Color used to draw the line</param>
            /// <returns>New line shape built with the given parameters</returns>
            ////////////////////////////////////////////////////////////
            public static Shape Line(Vector2 p1, Vector2 p2, float thickness, Color color)
            {
                return Line(p1, p2, thickness, color, 0, Color.White);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create a shape made of a single line
            /// </summary>
            /// <param name="p1">Position of the first point</param>
            /// <param name="p2">Position of the second point</param>
            /// <param name="thickness">Line thickness</param>
            /// <param name="color">Color used to draw the line</param>
            /// <param name="outline">Outline width</param>
            /// <param name="outlineColor">Color used to draw the outline</param>
            /// <returns>New line shape built with the given parameters</returns>
            ////////////////////////////////////////////////////////////
            public static Shape Line(Vector2 p1, Vector2 p2, float thickness, Color color, float outline, Color outlineColor)
            {
                return new Shape(sfShape_CreateLine(p1.X, p1.Y, p2.X, p2.Y, thickness, color, outline, outlineColor));
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create a shape made of a single rectangle
            /// </summary>
            /// <param name="rectangle">Rectangle to create</param>
            /// <param name="color">Color used to fill the rectangle</param>
            /// <returns>New rectangle shape built with the given parameters</returns>
            ////////////////////////////////////////////////////////////
            public static Shape Rectangle(FloatRect rectangle, Color color)
            {
                return Rectangle(rectangle, color, 0, Color.White);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create a shape made of a single rectangle
            /// </summary>
            /// <param name="rectangle">Rectangle to create</param>
            /// <param name="color">Color used to fill the rectangle</param>
            /// <param name="outline">Outline width</param>
            /// <param name="outlineColor">Color used to draw the outline</param>
            /// <returns>New rectangle shape built with the given parameters</returns>
            ////////////////////////////////////////////////////////////
            public static Shape Rectangle(FloatRect rectangle, Color color, float outline, Color outlineColor)
            {
                return new Shape(sfShape_CreateRectangle(rectangle.Left, rectangle.Top, rectangle.Width, rectangle.Height, color, outline, outlineColor));
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create a shape made of a single circle
            /// </summary>
            /// <param name="center">Position of the center</param>
            /// <param name="radius">Radius of the circle</param>
            /// <param name="color">Color used to fill the circle</param>
            /// <returns>New circle shape built with the given parameters</returns>
            ////////////////////////////////////////////////////////////
            public static Shape Circle(Vector2 center, float radius, Color color)
            {
                return Circle(center, radius, color, 0, Color.White);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create a shape made of a single circle
            /// </summary>
            /// <param name="center">Position of the center</param>
            /// <param name="radius">Radius of the circle</param>
            /// <param name="color">Color used to fill the circle</param>
            /// <param name="outline">Outline width</param>
            /// <param name="outlineColor">Color used to draw the outline</param>
            /// <returns>New circle shape built with the given parameters</returns>
            ////////////////////////////////////////////////////////////
            public static Shape Circle(Vector2 center, float radius, Color color, float outline, Color outlineColor)
            {
                return new Shape(sfShape_CreateCircle(center.X, center.Y, radius, color, outline, outlineColor));
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Provide a string describing the object
            /// </summary>
            /// <returns>String description of the object</returns>
            ////////////////////////////////////////////////////////////
            public override string ToString()
            {
                return "[Shape]" +
                       " Position(" + Position + ")" +
                       " Rotation(" + Rotation + ")" +
                       " Scale(" + Scale + ")" +
                       " Origin(" + Origin + ")" +
                       " Color(" + Color + ")" +
                       " BlendMode(" + BlendMode + ")" +
                       " OutlineThickness(" + OutlineThickness + ")" +
                       " PointsCount(" + PointsCount + ")";
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
                    sfRenderWindow_DrawShape(target.This, This);
                else
                    sfRenderWindow_DrawShapeWithShader(target.This, This, shader.This);
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
                    sfRenderImage_DrawShape(target.This, This);
                else
                    sfRenderImage_DrawShapeWithShader(target.This, This, shader.This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Handle the destruction of the object
            /// </summary>
            /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
            ////////////////////////////////////////////////////////////
            protected override void Destroy(bool disposing)
            {
                sfShape_Destroy(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Internal constructor
            /// </summary>
            /// <param name="thisPtr">Pointer to the internal object in C library</param>
            ////////////////////////////////////////////////////////////
            private Shape(IntPtr thisPtr) :
                base(thisPtr)
            {
            }

            #region Imports
            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfShape_Create();

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfShape_Copy(IntPtr Shape);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShape_Destroy(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShape_SetPosition(IntPtr This, float X, float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfShape_GetX(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfShape_GetY(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShape_SetRotation(IntPtr This, float Rotation);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfShape_GetRotation(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShape_SetScale(IntPtr This, float X, float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfShape_GetScaleX(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfShape_GetScaleY(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShape_SetOrigin(IntPtr This, float X, float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfShape_GetOriginX(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfShape_GetOriginY(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShape_SetColor(IntPtr This, Color Color);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern Color sfShape_GetColor(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShape_SetBlendMode(IntPtr This, BlendMode Mode);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern BlendMode sfShape_GetBlendMode(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern Vector2 sfShape_TransformToLocal(IntPtr This, float PointX, float PointY, out float X, out float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern Vector2 sfShape_TransformToGlobal(IntPtr This, float PointX, float PointY, out float X, out float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_DrawShape(IntPtr This, IntPtr Shape);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_DrawShapeWithShader(IntPtr This, IntPtr Shape, IntPtr Shader);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderImage_DrawShape(IntPtr This, IntPtr Shape);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderImage_DrawShapeWithShader(IntPtr This, IntPtr Shape, IntPtr Shader);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfShape_CreateLine(float P1X, float P1Y, float P2X, float P2Y, float Thickness, Color Col, float Outline, Color OutlineCol);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfShape_CreateRectangle(float P1X, float P1Y, float P2X, float P2Y, Color Col, float Outline, Color OutlineCol);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfShape_CreateCircle(float X, float Y, float Radius, Color Col, float Outline, Color OutlineCol);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShape_AddPoint(IntPtr This, float X, float Y, Color Col, Color OutlineCol);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShape_EnableFill(IntPtr This, bool Enable);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShape_EnableOutline(IntPtr This, bool Enable);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShape_SetOutlineThickness(IntPtr This, float Thickness);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfShape_GetOutlineThickness(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern uint sfShape_GetPointsCount(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShape_SetPointPosition(IntPtr This, uint Index, float X, float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShape_GetPointPosition(IntPtr This, uint Index, out float X, out float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShape_SetPointColor(IntPtr This, uint Index, Color Col);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern Color sfShape_GetPointColor(IntPtr This, uint Index);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShape_SetPointOutlineColor(IntPtr This, uint Index, Color Col);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern Color sfShape_GetPointOutlineColor(IntPtr This, uint Index);
            #endregion
        }
    }
}

using System;
using System.Security;
using System.Runtime.InteropServices;

namespace SFML
{
    namespace Graphics
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// This class defines a graphical 2D text, that can be drawn on screen
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class String2D : Drawable
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Enumerate the string drawing styles
            /// </summary>
            ////////////////////////////////////////////////////////////
            [Flags]
            public enum Styles
            {
                /// <summary>Regular characters, no style</summary>
                Regular = 0,

                /// <summary> Characters are bold</summary>
                Bold = 1 << 0,

                /// <summary>Characters are in italic</summary>
                Italic = 1 << 1,

                /// <summary>Characters are underlined</summary>
                Underlined = 1 << 2
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Default constructor
            /// </summary>
            ////////////////////////////////////////////////////////////
            public String2D() :
                this("")
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the string from a text
            /// </summary>
            /// <param name="text">Text to display</param>
            ////////////////////////////////////////////////////////////
            public String2D(string text) :
                this(text, Font.DefaultFont)
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the string from a text and a font
            /// </summary>
            /// <param name="text">Text to display</param>
            /// <param name="font">Font to use</param>
            ////////////////////////////////////////////////////////////
            public String2D(string text, Font font) :
                this(text, font, 30)
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the string from a text, font and size
            /// </summary>
            /// <param name="text">Text to display</param>
            /// <param name="font">Font to use</param>
            /// <param name="size">Base characters size</param>
            ////////////////////////////////////////////////////////////
            public String2D(string text, Font font, uint size) :
                base(sfString_Create())
            {
                Text = text;
                Font = font;
                Size = size;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Position of the object on screen
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override Vector2 Position
            {
                get { return new Vector2(sfString_GetX(This), sfString_GetY(This)); }
                set { sfString_SetPosition(This, value.X, value.Y); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Rotation of the object, defined in degrees
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override float Rotation
            {
                get { return sfString_GetRotation(This); }
                set { sfString_SetRotation(This, value); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Vertical and horizontal scale of the object
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override Vector2 Scale
            {
                get { return new Vector2(sfString_GetScaleX(This), sfString_GetScaleY(This)); }
                set { sfString_SetScale(This, value.X, value.Y); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Center of the transformation of the object
            /// (center of translation, rotation and scale)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override Vector2 Center
            {
                get { return new Vector2(sfString_GetCenterX(This), sfString_GetCenterY(This)); }
                set { sfString_SetCenter(This, value.X, value.Y); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Global color of the object
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override Color Color
            {
                get { return sfString_GetColor(This); }
                set { sfString_SetColor(This, value); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Blending mode of the object
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override BlendMode BlendMode
            {
                get { return sfString_GetBlendMode(This); }
                set { sfString_SetBlendMode(This, value); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Transform a point from global coordinates into local coordinates
            /// (ie it applies the inverse of object's center, translation, rotation and scale to the point)
            /// </summary>
            /// <param name="point">Point to transform</param>
            /// <returns>Transformed point</returns>
            ////////////////////////////////////////////////////////////
            public override Vector2 TransformToLocal(Vector2 point)
            {
                Vector2 Transformed;
                sfString_TransformToLocal(This, point.X, point.Y, out Transformed.X, out Transformed.Y);

                return Transformed;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Transform a point from local coordinates into global coordinates
            /// (ie it applies the object's center, translation, rotation and scale to the point)
            /// </summary>
            /// <param name="point">Point to transform</param>
            /// <returns>Transformed point</returns>
            ////////////////////////////////////////////////////////////
            public override Vector2 TransformToGlobal(Vector2 point)
            {
                Vector2 Transformed;
                sfString_TransformToGlobal(This, point.X, point.Y, out Transformed.X, out Transformed.Y);

                return Transformed;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Text displayed
            /// </summary>
            ////////////////////////////////////////////////////////////
            public string Text
            {
                // TODO : use unicode functions
                // (convert from UTF-16 to UTF-32, and find how to marshal System.String as sfUint32*...)
                get {return sfString_GetText(This);}
                set {sfString_SetText(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Font used to display the text
            /// </summary>
            ////////////////////////////////////////////////////////////
            public Font Font
            {
                get {return myFont;}
                set {myFont = value; sfString_SetFont(This, value != null ? value.This : IntPtr.Zero);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Base size of characters
            /// </summary>
            ////////////////////////////////////////////////////////////
            public float Size
            {
                get {return sfString_GetSize(This);}
                set {sfString_SetSize(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Style of the text (see Styles enum)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public Styles Style
            {
                get {return sfString_GetStyle(This);}
                set {sfString_SetStyle(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get the string rectangle on screen
            /// </summary>
            /// <returns>String rectangle in global coordinates (doesn't include rotation)</returns>
            ////////////////////////////////////////////////////////////
            public FloatRect GetRect()
            {
                return sfString_GetRect(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Return the visual position of the Index-th character of the string,
            /// in coordinates relative to the string
            /// (note : translation, center, rotation and scale are not applied)
            /// </summary>
            /// <param name="index">Index of the character</param>
            /// <returns>Position of the Index-th character (end of string if Index is out of range)</returns>
            ////////////////////////////////////////////////////////////
            public Vector2 GetCharacterPos(uint index)
            {
                Vector2 Pos;
                sfString_GetCharacterPos(This, index, out Pos.X, out Pos.Y);
                
                return Pos;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Render the object into the given render window
            /// </summary>
            /// <param name="window">Target window</param>
            ////////////////////////////////////////////////////////////
            internal override void Render(RenderWindow window)
            {
                sfRenderWindow_DrawString(window.This, This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Handle the destruction of the object
            /// </summary>
            /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
            ////////////////////////////////////////////////////////////
            protected override void Destroy(bool disposing)
            {
                sfString_Destroy(This);
            }

            private Font myFont = Font.DefaultFont;

            #region Imports
            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfString_Create();

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfString_Destroy(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfString_SetPosition(IntPtr This, float X, float Y);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern float sfString_GetX(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern float sfString_GetY(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfString_SetRotation(IntPtr This, float Rotation);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern float sfString_GetRotation(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfString_SetScale(IntPtr This, float X, float Y);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern float sfString_GetScaleX(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern float sfString_GetScaleY(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfString_SetCenter(IntPtr This, float X, float Y);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern float sfString_GetCenterX(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern float sfString_GetCenterY(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfString_SetColor(IntPtr This, Color Color);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern Color sfString_GetColor(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfString_SetBlendMode(IntPtr This, BlendMode Mode);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern BlendMode sfString_GetBlendMode(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern Vector2 sfString_TransformToLocal(IntPtr This, float PointX, float PointY, out float X, out float Y);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern Vector2 sfString_TransformToGlobal(IntPtr This, float PointX, float PointY, out float X, out float Y);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_DrawString(IntPtr This, IntPtr String);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern float sfString_GetWidth(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern float sfString_GetHeight(IntPtr This);

            [DllImport("csfml-graphics", CharSet = CharSet.Ansi), SuppressUnmanagedCodeSecurity]
            static extern void sfString_SetText(IntPtr This, string Text);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfString_SetFont(IntPtr This, IntPtr Font);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfString_SetSize(IntPtr This, float Size);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfString_SetStyle(IntPtr This, Styles Style);

            [DllImport("csfml-graphics", CharSet = CharSet.Ansi), SuppressUnmanagedCodeSecurity]
            static extern string sfString_GetText(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern float sfString_GetSize(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern Styles sfString_GetStyle(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern FloatRect sfString_GetRect(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfString_GetCharacterPos(IntPtr This, uint Index, out float X, out float Y);

            #endregion
        }
    }
}

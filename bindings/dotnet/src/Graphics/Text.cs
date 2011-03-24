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
        public class Text : Drawable
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
            public Text() :
                this("")
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the text from a string
            /// </summary>
            /// <param name="str">String to display</param>
            ////////////////////////////////////////////////////////////
            public Text(string str) :
                this(str, Font.DefaultFont)
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the text from a string and a font
            /// </summary>
            /// <param name="str">String to display</param>
            /// <param name="font">Font to use</param>
            ////////////////////////////////////////////////////////////
            public Text(string str, Font font) :
                this(str, font, 30)
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the text from a string, font and size
            /// </summary>
            /// <param name="str">String to display</param>
            /// <param name="font">Font to use</param>
            /// <param name="characterSize">Base characters size</param>
            ////////////////////////////////////////////////////////////
            public Text(string str, Font font, uint characterSize) :
                base(sfText_Create())
            {
                DisplayedString = str;
                Font = font;
                CharacterSize = characterSize;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the text from another text
            /// </summary>
            /// <param name="copy">Text to copy</param>
            ////////////////////////////////////////////////////////////
            public Text(Text copy) :
                base(sfText_Copy(copy.This))
            {
                Font = copy.Font;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Position of the object on screen
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override Vector2 Position
            {
                get { return new Vector2(sfText_GetX(This), sfText_GetY(This)); }
                set { sfText_SetPosition(This, value.X, value.Y); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Rotation of the object, defined in degrees
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override float Rotation
            {
                get { return sfText_GetRotation(This); }
                set { sfText_SetRotation(This, value); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Vertical and horizontal scale of the object
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override Vector2 Scale
            {
                get { return new Vector2(sfText_GetScaleX(This), sfText_GetScaleY(This)); }
                set { sfText_SetScale(This, value.X, value.Y); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Origin of the transformation of the object
            /// (center of translation, rotation and scale)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override Vector2 Origin
            {
                get { return new Vector2(sfText_GetOriginX(This), sfText_GetOriginY(This)); }
                set { sfText_SetOrigin(This, value.X, value.Y); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Global color of the object
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override Color Color
            {
                get { return sfText_GetColor(This); }
                set { sfText_SetColor(This, value); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Blending mode of the object
            /// </summary>
            ////////////////////////////////////////////////////////////
            public override BlendMode BlendMode
            {
                get { return sfText_GetBlendMode(This); }
                set { sfText_SetBlendMode(This, value); }
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
                sfText_TransformToLocal(This, point.X, point.Y, out Transformed.X, out Transformed.Y);

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
                sfText_TransformToGlobal(This, point.X, point.Y, out Transformed.X, out Transformed.Y);

                return Transformed;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// String which is displayed
            /// </summary>
            ////////////////////////////////////////////////////////////
            public string DisplayedString
            {
                // TODO : use unicode functions
                // (convert from UTF-16 to UTF-32, and find how to marshal System.String as sfUint32*...)
                get {return sfText_GetString(This);}
                set {sfText_SetString(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Font used to display the text
            /// </summary>
            ////////////////////////////////////////////////////////////
            public Font Font
            {
                get {return myFont;}
                set {myFont = value; sfText_SetFont(This, value != null ? value.This : IntPtr.Zero);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Base size of characters
            /// </summary>
            ////////////////////////////////////////////////////////////
            public uint CharacterSize
            {
                get {return sfText_GetCharacterSize(This);}
                set {sfText_SetCharacterSize(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Style of the text (see Styles enum)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public Styles Style
            {
                get {return sfText_GetStyle(This);}
                set {sfText_SetStyle(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get the text rectangle on screen
            /// </summary>
            /// <returns>Text rectangle in global coordinates (doesn't include rotation)</returns>
            ////////////////////////////////////////////////////////////
            public FloatRect GetRect()
            {
                return sfText_GetRect(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Return the visual position of the Index-th character of the text,
            /// in coordinates relative to the text
            /// (note : translation, origin, rotation and scale are not applied)
            /// </summary>
            /// <param name="index">Index of the character</param>
            /// <returns>Position of the Index-th character (end of text if Index is out of range)</returns>
            ////////////////////////////////////////////////////////////
            public Vector2 GetCharacterPos(uint index)
            {
                Vector2 Pos;
                sfText_GetCharacterPos(This, index, out Pos.X, out Pos.Y);
                
                return Pos;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Provide a string describing the object
            /// </summary>
            /// <returns>String description of the object</returns>
            ////////////////////////////////////////////////////////////
            public override string ToString()
            {
                return "[Text]" +
                       " Position(" + Position + ")" +
                       " Rotation(" + Rotation + ")" +
                       " Scale(" + Scale + ")" +
                       " Origin(" + Origin + ")" +
                       " Color(" + Color + ")" +
                       " BlendMode(" + BlendMode + ")" +
                       " String(" + DisplayedString + ")" +
                       " Font(" + Font + ")" +
                       " CharacterSize(" + CharacterSize + ")" +
                       " Style(" + Style + ")" +
                       " Rectangle(" + GetRect() + ")";
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
                    sfRenderWindow_DrawText(target.This, This);
                else
                    sfRenderWindow_DrawTextWithShader(target.This, This, shader.This);
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
                    sfRenderImage_DrawText(target.This, This);
                else
                    sfRenderImage_DrawTextWithShader(target.This, This, shader.This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Handle the destruction of the object
            /// </summary>
            /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
            ////////////////////////////////////////////////////////////
            protected override void Destroy(bool disposing)
            {
                sfText_Destroy(This);
            }

            private Font myFont = Font.DefaultFont;

            #region Imports
            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfText_Create();

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfText_Copy(IntPtr Text);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfText_Destroy(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfText_SetPosition(IntPtr This, float X, float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfText_GetX(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfText_GetY(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfText_SetRotation(IntPtr This, float Rotation);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfText_GetRotation(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfText_SetScale(IntPtr This, float X, float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfText_GetScaleX(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfText_GetScaleY(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfText_SetOrigin(IntPtr This, float X, float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfText_GetOriginX(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern float sfText_GetOriginY(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfText_SetColor(IntPtr This, Color Color);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern Color sfText_GetColor(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfText_SetBlendMode(IntPtr This, BlendMode Mode);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern BlendMode sfText_GetBlendMode(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern Vector2 sfText_TransformToLocal(IntPtr This, float PointX, float PointY, out float X, out float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern Vector2 sfText_TransformToGlobal(IntPtr This, float PointX, float PointY, out float X, out float Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_DrawText(IntPtr This, IntPtr String);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderWindow_DrawTextWithShader(IntPtr This, IntPtr String, IntPtr Shader);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderImage_DrawText(IntPtr This, IntPtr String);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderImage_DrawTextWithShader(IntPtr This, IntPtr String, IntPtr Shader);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi), SuppressUnmanagedCodeSecurity]
            static extern void sfText_SetString(IntPtr This, string Text);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfText_SetFont(IntPtr This, IntPtr Font);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfText_SetCharacterSize(IntPtr This, uint Size);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfText_SetStyle(IntPtr This, Styles Style);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi), SuppressUnmanagedCodeSecurity]
            static extern string sfText_GetString(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern uint sfText_GetCharacterSize(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern Styles sfText_GetStyle(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern FloatRect sfText_GetRect(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfText_GetCharacterPos(IntPtr This, uint Index, out float X, out float Y);

            #endregion
        }
    }
}

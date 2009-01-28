using System;
using System.Runtime.InteropServices;
using System.Security;
using System.IO;

namespace SFML
{
    namespace Graphics
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Font is the low-level class for loading and
        /// manipulating character fonts. This class is meant to
        /// be used by String2D
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class Font : ObjectBase
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the font from a file
            /// </summary>
            /// <param name="filename">Font file to load</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public Font(string filename) :
                this(filename, 30)
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the font from a file, using custom size
            /// </summary>
            /// <param name="filename">Font file to load</param>
            /// <param name="charSize">Character size</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public Font(string filename, uint charSize) :
                this(filename, charSize, "")
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the font from a file, using custom size and characters set
            /// </summary>
            /// <param name="filename">Font file to load</param>
            /// <param name="charSize">Character size</param>
            /// <param name="charset">Set of characters to generate</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public Font(string filename, uint charSize, string charset) :
                base(IntPtr.Zero)
            {
                unsafe
                {
                    IntPtr ptr;
                    int size;
                    if (Int32.TryParse(charset, out size))
                        ptr = new IntPtr(&size);
                    else
                        ptr = IntPtr.Zero;

                    SetThis(sfFont_CreateFromFile(filename, charSize, ptr));
                }

                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("font", filename);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the font from a file in a stream
            /// </summary>
            /// <param name="stream">Stream containing the file contents</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public Font(Stream stream) :
                this(stream, 30)
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the font from a file in a stream, using custom size
            /// </summary>
            /// <param name="stream">Stream containing the file contents</param>
            /// <param name="charSize">Character size</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public Font(Stream stream, uint charSize) :
                this(stream, charSize, "")
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the font from a file in a stream
            /// </summary>
            /// <param name="stream">Stream containing the file contents</param>
            /// <param name="charSize">Character size</param>
            /// <param name="charset">Set of characters to generate</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public Font(Stream stream, uint charSize, string charset) :
                base(IntPtr.Zero)
            {
                unsafe
                {
                    IntPtr ptr;
                    int size;
                    if (Int32.TryParse(charset, out size))
                        ptr = new IntPtr(&size);
                    else
                        ptr = IntPtr.Zero;

                    stream.Position = 0;
                    byte[] StreamData = new byte[stream.Length];
                    uint Read = (uint)stream.Read(StreamData, 0, StreamData.Length);
                    fixed (byte* dataPtr = StreamData)
                    {
                        SetThis(sfFont_CreateFromMemory((char*)dataPtr, Read, charSize, ptr));
                    }
                }
                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("font");
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Base character size
            /// </summary>
            ////////////////////////////////////////////////////////////
            public uint CharacterSize
            {
                get { return sfFont_GetCharacterSize(This); }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Default built-in font
            /// </summary>
            ////////////////////////////////////////////////////////////
            public static Font DefaultFont
            {
                get
                {
                    if (ourDefaultFont == null)
                        ourDefaultFont = new Font(sfFont_GetDefaultFont());

                    return ourDefaultFont;
                }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Handle the destruction of the object
            /// </summary>
            /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
            ////////////////////////////////////////////////////////////
            protected override void Destroy(bool disposing)
            {
                if (this != ourDefaultFont)
                {
                    if (!disposing)
                        Context.Global.SetActive(true);

                    sfFont_Destroy(This);

                    if (!disposing)
                        Context.Global.SetActive(false);
                }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Internal constructor
            /// </summary>
            /// <param name="thisPtr">Pointer to the object in C library</param>
            ////////////////////////////////////////////////////////////
            private Font(IntPtr thisPtr) :
                base(thisPtr)
            {
            }

            private static Font ourDefaultFont = null;

            #region Imports
            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfFont_Create();

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfFont_CreateFromFile(string Filename, uint CharSize, IntPtr Charset);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            unsafe static extern IntPtr sfFont_CreateFromMemory(char* Data, uint SizeInBytes, uint CharSize, IntPtr Charset);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfFont_Destroy(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern uint sfFont_GetCharacterSize(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfFont_GetDefaultFont();
            #endregion
        }
    }
}

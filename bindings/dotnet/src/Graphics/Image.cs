using System;
using System.Runtime.InteropServices;
using System.Security;
using System.IO;
using System.Runtime.ConstrainedExecution;

namespace SFML
{
    namespace Graphics
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Image is the low-level class for loading and
        /// manipulating images
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class Image : ObjectBase
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the image with black color
            /// </summary>
            /// <param name="width">Image width</param>
            /// <param name="height">Image height</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public Image(uint width, uint height) :
                this(width, height, Color.Black)
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the image from a single color
            /// </summary>
            /// <param name="width">Image width</param>
            /// <param name="height">Image height</param>
            /// <param name="color">Color to fill the image with</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public Image(uint width, uint height, Color color) :
                base(sfImage_CreateFromColor(width, height, color))
            {
                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("image");
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the image from a file
            /// </summary>
            /// <param name="filename">Path of the image file to load</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public Image(string filename) :
                base(sfImage_CreateFromFile(filename))
            {
                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("image", filename);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the image from a file in a stream
            /// </summary>
            /// <param name="stream">Stream containing the file contents</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public Image(Stream stream) :
                base(IntPtr.Zero)
            {
                stream.Position = 0;
                byte[] StreamData = new byte[stream.Length];
                uint Read = (uint)stream.Read(StreamData, 0, StreamData.Length);
                unsafe
                {
                    fixed (byte* dataPtr = StreamData)
                    {
                        SetThis(sfImage_CreateFromMemory((char*)dataPtr, Read));
                    }
                }
                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("image");
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the image directly from an array of pixels
            /// </summary>
            /// <param name="pixels">2 dimensions array containing the pixels</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public Image(Color[,] pixels) :
                base(IntPtr.Zero)
            {
                unsafe
                {
                    fixed (Color* PixelsPtr = pixels)
                    {
                        uint Width  = (uint)pixels.GetLength(0);
                        uint Height = (uint)pixels.GetLength(1);
                        SetThis(sfImage_CreateFromPixels(Width, Height, (byte*)PixelsPtr));
                    }
                }

                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("image");
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the image directly from an array of pixels
            /// </summary>
            /// <param name="width">Image width</param>
            /// <param name="height">Image height</param>
            /// <param name="pixels">array containing the pixels</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public Image(uint width, uint height, byte[] pixels) :
                base(IntPtr.Zero)
            {
                unsafe
                {
                    fixed (byte* PixelsPtr = pixels)
                    {
                        SetThis(sfImage_CreateFromPixels(width, height, PixelsPtr));
                    }
                }

                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("image");
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the image from another image
            /// </summary>
            /// <param name="copy">Image to copy</param>
            ////////////////////////////////////////////////////////////
            public Image(Image copy) :
                base(sfImage_Copy(copy.This))
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Save the contents of the image to a file
            /// </summary>
            /// <param name="filename">Path of the file to save (overwritten if already exist)</param>
            /// <returns>True if saving was successful</returns>
            ////////////////////////////////////////////////////////////
            public bool SaveToFile(string filename)
            {
                return sfImage_SaveToFile(This, filename);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create a transparency mask from a specified colorkey
            /// </summary>
            /// <param name="color">Color to become transparent</param>
            ////////////////////////////////////////////////////////////
            public void CreateMaskFromColor(Color color)
            {
                CreateMaskFromColor(color, 0);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create a transparency mask from a specified colorkey
            /// </summary>
            /// <param name="color">Color to become transparent</param>
            /// <param name="alpha">Alpha value to use for transparent pixels</param>
            ////////////////////////////////////////////////////////////
            public void CreateMaskFromColor(Color color, byte alpha)
            {
                sfImage_CreateMaskFromColor(This, color, alpha);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Copy pixels from another image onto this one.
            /// This function does a slow pixel copy and should only
            /// be used at initialization time
            /// </summary>
            /// <param name="source">Source image to copy</param>
            /// <param name="destX">X coordinate of the destination position</param>
            /// <param name="destY">Y coordinate of the destination position</param>
            ////////////////////////////////////////////////////////////
            public void Copy(Image source, uint destX, uint destY)
            {
                Copy(source, destX, destY, new IntRect(0, 0, 0, 0));
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Copy pixels from another image onto this one.
            /// This function does a slow pixel copy and should only
            /// be used at initialization time
            /// </summary>
            /// <param name="source">Source image to copy</param>
            /// <param name="destX">X coordinate of the destination position</param>
            /// <param name="destY">Y coordinate of the destination position</param>
            /// <param name="sourceRect">Sub-rectangle of the source image to copy</param>
            ////////////////////////////////////////////////////////////
            public void Copy(Image source, uint destX, uint destY, IntRect sourceRect)
            {
                sfImage_CopyImage(This, source.This, destX, destY, sourceRect);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create the image from the current contents of the
            /// given window
            /// </summary>
            /// <param name="window">Window to capture</param>
            /// <returns>True if copy has been successful</returns>
            ////////////////////////////////////////////////////////////
            public bool CopyScreen(RenderWindow window)
            {
                return CopyScreen(window, new IntRect(0, 0, 0, 0));
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create the image from the current contents of the
            /// given window
            /// </summary>
            /// <param name="window">Window to capture</param>
            /// <param name="sourceRect">Sub-rectangle of the screen to copy</param>
            /// <returns>True if copy has been successful</returns>
            ////////////////////////////////////////////////////////////
            public bool CopyScreen(RenderWindow window, IntRect sourceRect)
            {
                return sfImage_CopyScreen(This, window.This, sourceRect);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get a pixel from the image
            /// </summary>
            /// <param name="x">X coordinate of pixel in the image</param>
            /// <param name="y">Y coordinate of pixel in the image</param>
            /// <returns>Color of pixel (x, y)</returns>
            ////////////////////////////////////////////////////////////
            public Color GetPixel(uint x, uint y)
            {
                return sfImage_GetPixel(This, x, y);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change the color of a pixel
            /// </summary>
            /// <param name="x">X coordinate of pixel in the image</param>
            /// <param name="y">Y coordinate of pixel in the image</param>
            /// <param name="color">New color for pixel (x, y)</param>
            ////////////////////////////////////////////////////////////
            public void SetPixel(uint x, uint y, Color color)
            {
                sfImage_SetPixel(This, x, y, color);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get a copy of the array of pixels (RGBA 8 bits integers components)
            /// Array size is Width x Height x 4
            /// </summary>
            /// <returns>Array of pixels</returns>
            ////////////////////////////////////////////////////////////
            public byte[] Pixels
            {
                get
                {
                    byte[] PixelsPtr = new byte[Width * Height * 4];
                    Marshal.Copy(sfImage_GetPixelsPtr(This), PixelsPtr, 0, PixelsPtr.Length);
                    return PixelsPtr;
                }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Update the pixels of the image
            /// </summary>
            /// <param name="pixels">2 dimensions array containing the pixels</param>
            ////////////////////////////////////////////////////////////
            public void UpdatePixels(Color[,] pixels)
            {
                UpdatePixels(pixels, 0, 0);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Update the pixels of the image
            /// </summary>
            /// <param name="pixels">2 dimensions array containing the pixels</param>
            /// <param name="x">X position of the rectangle to update</param>
            /// <param name="y">Y position of the rectangle to update</param>
            ////////////////////////////////////////////////////////////
            public void UpdatePixels(Color[,] pixels, uint x, uint y)
            {
                unsafe
                {
                    fixed (Color* PixelsPtr = pixels)
                    {
                        int Width  = pixels.GetLength(0);
                        int Height = pixels.GetLength(1);
                        sfImage_UpdatePixels(This, PixelsPtr, new IntRect((int)x, (int)y, Width, Height));
                    }
                }
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Bind the image for rendering
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void Bind()
            {
                sfImage_Bind(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Control the smooth filter
            /// </summary>
            ////////////////////////////////////////////////////////////
            public bool Smooth
            {
                get {return sfImage_IsSmooth(This);}
                set {sfImage_SetSmooth(This, value);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Width of the image, in pixels
            /// </summary>
            ////////////////////////////////////////////////////////////
            public uint Width
            {
                get {return sfImage_GetWidth(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Height of the image, in pixels
            /// </summary>
            ////////////////////////////////////////////////////////////
            public uint Height
            {
                get {return sfImage_GetHeight(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Provide a string describing the object
            /// </summary>
            /// <returns>String description of the object</returns>
            ////////////////////////////////////////////////////////////
            public override string ToString()
            {
                return "[Image]" +
                       " Width(" + Width + ")" +
                       " Height(" + Height + ")" +
                       " Smooth(" + Smooth + ")";
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Internal constructor
            /// </summary>
            /// <param name="thisPtr">Pointer to the object in C library</param>
            ////////////////////////////////////////////////////////////
            internal Image(IntPtr thisPtr) :
                base(thisPtr)
            {
                myExternal = true;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Handle the destruction of the object
            /// </summary>
            /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
            ////////////////////////////////////////////////////////////
            protected override void Destroy(bool disposing)
            {
                if (!myExternal)
                {
                    if (!disposing)
                        Context.Global.SetActive(true);

                    sfImage_Destroy(This);

                    if (!disposing)
                        Context.Global.SetActive(false);
                }
            }

            bool myExternal = false;

            #region Imports
            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfImage_CreateFromColor(uint Width, uint Height, Color Col);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            unsafe static extern IntPtr sfImage_CreateFromPixels(uint Width, uint Height, byte* Pixels);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfImage_CreateFromFile(string Filename);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfImage_Copy(IntPtr Image);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            unsafe static extern IntPtr sfImage_CreateFromMemory(char* Data, uint Size);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfImage_Destroy(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfImage_SaveToFile(IntPtr This, string Filename);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfImage_CreateMaskFromColor(IntPtr This, Color Col, byte Alpha);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfImage_CopyScreen(IntPtr This, IntPtr Window, IntRect SourceRect);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfImage_CopyImage(IntPtr This, IntPtr Source, uint DestX, uint DestY, IntRect SourceRect);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfImage_SetPixel(IntPtr This, uint X, uint Y, Color Col);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern Color sfImage_GetPixel(IntPtr This, uint X, uint Y);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfImage_GetPixelsPtr(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            unsafe static extern void sfImage_UpdatePixels(IntPtr This, Color* Pixels, IntRect Rectangle);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfImage_Bind(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfImage_SetSmooth(IntPtr This, bool Smooth);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern uint sfImage_GetWidth(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern uint sfImage_GetHeight(IntPtr This);

            [DllImport("csfml-graphics-2", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfImage_IsSmooth(IntPtr This);
            #endregion
        }
    }
}

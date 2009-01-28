using System;
using System.Runtime.InteropServices;
using System.Security;
using System.Collections.Generic;

namespace SFML
{
    namespace Graphics
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// PostFX is used to apply a post effect to a window
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class PostFx : ObjectBase
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Default constructor (invalid effect)
            /// </summary>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public PostFx() :
                base(sfPostFx_Create())
            {
                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("post-fx");
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Load the effect from a file
            /// </summary>
            /// <param name="filename">Path of the effect file to load</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public PostFx(string filename) :
                base(sfPostFX_CreateFromFile(filename))
            {
                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("post-fx", filename);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Load the effect from a text in memory
            /// </summary>
            /// <param name="effect">String containing the effect code</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            void LoadFromString(string effect)
            {
                SetThis(sfPostFX_CreateFromMemory(effect));

                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("post-fx");
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change a 1-component parameter of the effect
            /// </summary>
            /// <param name="name">Name of the parameter in the effect</param>
            /// <param name="x">Value of the parameter</param>
            ////////////////////////////////////////////////////////////
            public void SetParameter(string name, float x)
            {
                sfPostFX_SetParameter1(This, name, x);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change a 2-component parameter of the effect
            /// </summary>
            /// <param name="name">Name of the parameter in the effect</param>
            /// <param name="x">X component of the value</param>
            /// <param name="y">Y component of the value</param>
            ////////////////////////////////////////////////////////////
            public void SetParameter(string name, float x, float y)
            {
                sfPostFX_SetParameter2(This, name, x, y);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change a 3-component parameter of the effect
            /// </summary>
            /// <param name="name">Name of the parameter in the effect</param>
            /// <param name="x">X component of the value</param>
            /// <param name="y">Y component of the value</param>
            /// <param name="z">Z component of the value</param>
            ////////////////////////////////////////////////////////////
            public void SetParameter(string name, float x, float y, float z)
            {
                sfPostFX_SetParameter3(This, name, x, y, z);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change a 4-component parameter of the effect
            /// </summary>
            /// <param name="name">Name of the parameter in the effect</param>
            /// <param name="x">X component of the value</param>
            /// <param name="y">Y component of the value</param>
            /// <param name="z">Z component of the value</param>
            /// <param name="w">W component of the value</param>
            ////////////////////////////////////////////////////////////
            public void SetParameter(string name, float x, float y, float z, float w)
            {
                sfPostFX_SetParameter4(This, name, x, y, z, w);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Set a texture parameter
            /// </summary>
            /// <param name="name">Name of the texture in the effect</param>
            /// <param name="texture">Image to set (pass null to use the contents of the screen)</param>
            ////////////////////////////////////////////////////////////
            public void SetTexture(string name, Image texture)
            {
                myTextures[name] = texture;
                sfPostFX_SetTexture(This, name, texture != null ? texture.This : IntPtr.Zero);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Tell whether or not the system supports post-effects
            /// </summary>
            ////////////////////////////////////////////////////////////
            public static bool CanUsePostFX
            {
                get {return sfPostFX_CanUsePostFX();}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Handle the destruction of the object
            /// </summary>
            /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
            ////////////////////////////////////////////////////////////
            protected override void Destroy(bool disposing)
            {
                if (!disposing)
                    Context.Global.SetActive(true);

                myTextures.Clear();
                sfPostFX_Destroy(This);

                if (!disposing)
                    Context.Global.SetActive(false);
            }

            Dictionary<string, Image> myTextures = new Dictionary<string, Image>();

            #region Imports
            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfPostFx_Create();

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfPostFx_Destroy(IntPtr This);
            
            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfPostFX_CreateFromFile(string Filename);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfPostFX_CreateFromMemory(string Effect);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfPostFX_Destroy(IntPtr PostFX);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfPostFX_SetParameter1(IntPtr PostFX, string Name, float X);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfPostFX_SetParameter2(IntPtr PostFX, string Name, float X, float Y);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfPostFX_SetParameter3(IntPtr PostFX, string Name, float X, float Y, float Z);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfPostFX_SetParameter4(IntPtr PostFX, string Name, float X, float Y, float Z, float W);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfPostFX_SetTexture(IntPtr PostFX, string Name, IntPtr Texture);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern bool sfPostFX_CanUsePostFX();

            #endregion
        }
    }
}

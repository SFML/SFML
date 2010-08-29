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
        /// Wrapper for pixel shaders
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class Shader : ObjectBase
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Default constructor (invalid shader)
            /// </summary>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public Shader() :
                base(sfShader_Create())
            {
                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("shader");
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Load the shader from a file
            /// </summary>
            /// <param name="filename">Path of the shader file to load</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            public Shader(string filename) :
                base(sfShader_CreateFromFile(filename))
            {
                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("shader", filename);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the shader from another shader
            /// </summary>
            /// <param name="copy">Shader to copy</param>
            ////////////////////////////////////////////////////////////
            public Shader(Shader copy) :
                base(sfShader_Copy(copy.This))
            {
                foreach (KeyValuePair<string, Image> pair in copy.myTextures)
                    myTextures[pair.Key] = copy.myTextures[pair.Key];
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Load the shader from a text in memory
            /// </summary>
            /// <param name="shader">String containing the shader code</param>
            /// <exception cref="LoadingFailedException" />
            ////////////////////////////////////////////////////////////
            void LoadFromString(string shader)
            {
                SetThis(sfShader_CreateFromMemory(shader));

                if (This == IntPtr.Zero)
                    throw new LoadingFailedException("shader");
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change a vector2 parameter of the shader
            /// </summary>
            /// <param name="name">Name of the parameter in the shader</param>
            /// <param name="v">Value of the parameter</param>
            ////////////////////////////////////////////////////////////
            public void SetParameter(string name, Vector2 v)
            {
                SetParameter(name, v.X, v.Y);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change a 1-component parameter of the shader
            /// </summary>
            /// <param name="name">Name of the parameter in the shader</param>
            /// <param name="x">Value of the parameter</param>
            ////////////////////////////////////////////////////////////
            public void SetParameter(string name, float x)
            {
                sfShader_SetParameter1(This, name, x);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change a 2-component parameter of the shader
            /// </summary>
            /// <param name="name">Name of the parameter in the shader</param>
            /// <param name="x">X component of the value</param>
            /// <param name="y">Y component of the value</param>
            ////////////////////////////////////////////////////////////
            public void SetParameter(string name, float x, float y)
            {
                sfShader_SetParameter2(This, name, x, y);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change a 3-component parameter of the shader
            /// </summary>
            /// <param name="name">Name of the parameter in the shader</param>
            /// <param name="x">X component of the value</param>
            /// <param name="y">Y component of the value</param>
            /// <param name="z">Z component of the value</param>
            ////////////////////////////////////////////////////////////
            public void SetParameter(string name, float x, float y, float z)
            {
                sfShader_SetParameter3(This, name, x, y, z);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Change a 4-component parameter of the shader
            /// </summary>
            /// <param name="name">Name of the parameter in the shader</param>
            /// <param name="x">X component of the value</param>
            /// <param name="y">Y component of the value</param>
            /// <param name="z">Z component of the value</param>
            /// <param name="w">W component of the value</param>
            ////////////////////////////////////////////////////////////
            public void SetParameter(string name, float x, float y, float z, float w)
            {
                sfShader_SetParameter4(This, name, x, y, z, w);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Set a texture parameter
            /// </summary>
            /// <param name="name">Name of the texture in the shader</param>
            /// <param name="texture">Image to set (pass null to use the texture of the object being drawn)</param>
            ////////////////////////////////////////////////////////////
            public void SetTexture(string name, Image texture)
            {
                myTextures[name] = texture;
                sfShader_SetTexture(This, name, texture != null ? texture.This : IntPtr.Zero);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Bind the shader for rendering
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void Bind()
            {
                sfShader_Bind(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Unbind the shader
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void Unbind()
            {
                sfShader_Unbind(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Tell whether or not the system supports shaders
            /// </summary>
            ////////////////////////////////////////////////////////////
            public static bool IsAvailable
            {
                get {return sfShader_IsAvailable();}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Special image representing the texture used by the object being drawn
            /// </summary>
            ////////////////////////////////////////////////////////////
            public static Image CurrentTexture
            {
                get {return null;}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Provide a string describing the object
            /// </summary>
            /// <returns>String description of the object</returns>
            ////////////////////////////////////////////////////////////
            public override string ToString()
            {
                return "[Shader]";
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
                sfShader_Destroy(This);

                if (!disposing)
                    Context.Global.SetActive(false);
            }

            Dictionary<string, Image> myTextures = new Dictionary<string, Image>();

            #region Imports
            [DllImport("csfml2-graphics", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfShader_Create();
            
            [DllImport("csfml2-graphics", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfShader_CreateFromFile(string Filename);

            [DllImport("csfml2-graphics", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfShader_CreateFromMemory(string Shader);

            [DllImport("csfml2-graphics", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfShader_Copy(IntPtr Shader);

            [DllImport("csfml2-graphics", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShader_Destroy(IntPtr Shader);

            [DllImport("csfml2-graphics", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShader_SetParameter1(IntPtr Shader, string Name, float X);

            [DllImport("csfml2-graphics", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShader_SetParameter2(IntPtr Shader, string Name, float X, float Y);

            [DllImport("csfml2-graphics", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShader_SetParameter3(IntPtr Shader, string Name, float X, float Y, float Z);

            [DllImport("csfml2-graphics", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShader_SetParameter4(IntPtr Shader, string Name, float X, float Y, float Z, float W);

            [DllImport("csfml2-graphics", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShader_SetTexture(IntPtr Shader, string Name, IntPtr Texture);

            [DllImport("csfml2-graphics", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShader_Bind(IntPtr Shader);

            [DllImport("csfml2-graphics", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern void sfShader_Unbind(IntPtr Shader);

            [DllImport("csfml2-graphics", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
            static extern bool sfShader_IsAvailable();

            #endregion
        }
    }
}

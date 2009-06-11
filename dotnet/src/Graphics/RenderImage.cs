using System;
using System.Runtime.InteropServices;
using System.Collections;
using System.Collections.Generic;
using System.Security;
using SFML.Window;

namespace SFML
{
    namespace Graphics
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Simple wrapper for Window that allows easy
        /// 2D rendering
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class RenderImage : ObjectBase, RenderTarget
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create the render image with the given dimensions
            /// </summary>
            /// <param name="width">Width of the render image</param>
            /// <param name="height">Height of the render image</param>
            ////////////////////////////////////////////////////////////
            public RenderImage(uint width, uint height) :
                this(width, height, false)
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create the render image with the given dimensions and
            /// an optional depth-buffer attached
            /// </summary>
            /// <param name="width">Width of the render image</param>
            /// <param name="height">Height of the render image</param>
            /// <param name="depthBuffer">Do you wxant a depth-buffer attached?</param>
            ////////////////////////////////////////////////////////////
            public RenderImage(uint width, uint height, bool depthBuffer) :
                base(sfRenderImage_Create(width, height, depthBuffer))
            {
                myDefaultView = new View(sfRenderImage_GetDefaultView(This));
                myCurrentView = myDefaultView;
                myImage       = new Image(sfRenderImage_GetImage(This));
                GC.SuppressFinalize(myDefaultView);
                GC.SuppressFinalize(myImage);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Width of the rendering region of the image
            /// </summary>
            ////////////////////////////////////////////////////////////
            public uint Width
            {
                get {return sfRenderImage_GetWidth(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Height of the rendering region of the image
            /// </summary>
            ////////////////////////////////////////////////////////////
            public uint Height
            {
                get {return sfRenderImage_GetHeight(This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Activate of deactivate the render image as the current target
            /// for rendering
            /// </summary>
            /// <param name="active">True to activate, false to deactivate (true by default)</param>
            /// <returns>True if operation was successful, false otherwise</returns>
            ////////////////////////////////////////////////////////////
            public bool SetActive(bool active)
            {
                return sfRenderImage_SetActive(This, active);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Default view of the render image
            /// </summary>
            ////////////////////////////////////////////////////////////
            public View DefaultView
            {
                get {return myDefaultView;}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Current view active in the render image
            /// </summary>
            ////////////////////////////////////////////////////////////
            public View CurrentView
            {
                get {return myCurrentView;}
                set {myCurrentView = value; sfRenderImage_SetView(This, value.This);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Tell SFML to preserve external OpenGL states, at the expense of
            /// more CPU charge. Use this function if you don't want SFML
            /// to mess up your own OpenGL states (if any).
            /// Don't enable state preservation if not needed, as it will allow
            /// SFML to do internal optimizations and improve performances.
            /// This parameter is false by default
            /// </summary>
            /// <param name="preserve">True to preserve OpenGL states, false to let SFML optimize</param>
            ////////////////////////////////////////////////////////////
            public void PreserveOpenGLStates(bool preserve)
            {
                sfRenderImage_PreserveOpenGLStates(This, preserve);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Clear the entire render image with black color
            /// </summary>
            ////////////////////////////////////////////////////////////
            public void Clear()
            {
                sfRenderImage_Clear(This, Color.Black);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Clear the entire render image with a single color
            /// </summary>
            /// <param name="color">Color to use to clear the image</param>
            ////////////////////////////////////////////////////////////
            public void Clear(Color color)
            {
                sfRenderImage_Clear(This, color);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Draw something into the render image
            /// </summary>
            /// <param name="objectToDraw">Object to draw</param>
            ////////////////////////////////////////////////////////////
            public void Draw(Drawable objectToDraw)
            {
                objectToDraw.Render(this);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Apply a post-fx to the render image
            /// </summary>
            /// <param name="postFx">PostFx to apply</param>
            ////////////////////////////////////////////////////////////
            public void Draw(PostFx postFx)
            {
                sfRenderImage_DrawPostFX(This, postFx != null ? postFx.This : IntPtr.Zero);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Target image of the render image
            /// </summary>
            ////////////////////////////////////////////////////////////
            public Image Image
            {
                get {return myImage;}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Tell whether or not the system supports render images
            /// </summary>
            ////////////////////////////////////////////////////////////
            public static bool CanUseRenderImage
            {
                get {return sfRenderImage_CanUseRenderImage();}
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

                sfRenderImage_Destroy(This);

                if (disposing)
                {
                    myDefaultView.Dispose();
                    myImage.Dispose();
                }

                if (!disposing)
                    Context.Global.SetActive(false);
            }

            private View  myCurrentView = null;
            private View  myDefaultView = null;
            private Image myImage       = null;

            #region Imports
            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfRenderImage_Create(uint Width, uint Height, bool DepthBuffer);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderImage_Destroy(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderImage_Clear(IntPtr This, Color ClearColor);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern uint sfRenderImage_GetWidth(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern uint sfRenderImage_GetHeight(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern bool sfRenderImage_SetActive(IntPtr This, bool Active);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderImage_SetView(IntPtr This, IntPtr View);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfRenderImage_GetView(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfRenderImage_GetDefaultView(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderImage_PreserveOpenGLStates(IntPtr This, bool Preserve);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfRenderImage_DrawPostFX(IntPtr This, IntPtr PostFx);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfRenderImage_GetImage(IntPtr This);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern bool sfRenderImage_CanUseRenderImage();

            #endregion
        }
    }
}

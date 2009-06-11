using System;
using System.Runtime.InteropServices;
using System.Security;
using SFML.Window;

namespace SFML
{
    namespace Graphics
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Abstract base class for render targets (renderwindow, renderimage)
        /// </summary>
        ////////////////////////////////////////////////////////////
        public interface RenderTarget
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Width of the rendering region of the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            uint Width {get;}

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Height of the rendering region of the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            uint Height {get;}

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Default view of the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            View DefaultView {get;}

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Current view active in the window
            /// </summary>
            ////////////////////////////////////////////////////////////
            View CurrentView {get;}

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
            void PreserveOpenGLStates(bool preserve);

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Clear the entire window with black color
            /// </summary>
            ////////////////////////////////////////////////////////////
            void Clear();

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Clear the entire window with a single color
            /// </summary>
            /// <param name="color">Color to use to clear the window</param>
            ////////////////////////////////////////////////////////////
            void Clear(Color color);

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Draw something into the window
            /// </summary>
            /// <param name="objectToDraw">Object to draw</param>
            ////////////////////////////////////////////////////////////
            void Draw(Drawable objectToDraw);

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Apply a post-fx to the window
            /// </summary>
            /// <param name="postFx">PostFx to apply</param>
            ////////////////////////////////////////////////////////////
            void Draw(PostFx postFx);
        }
    }
}

using System;
using System.Runtime.InteropServices;
using System.Security;

namespace SFML
{
    namespace Graphics
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// This class defines a view (position, size, etc.) ;
        /// you can consider it as a 2D camera
        /// </summary>
        ////////////////////////////////////////////////////////////
        public class View : ObjectBase
        {
            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Create a default view (1000x1000, centered on origin)
            /// </summary>
            ////////////////////////////////////////////////////////////
            public View() :
                base(sfView_Create())
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the view from a rectangle
            /// </summary>
            /// <param name="viewRect">Rectangle defining the position and size of the view</param>
            ////////////////////////////////////////////////////////////
            public View(FloatRect viewRect) :
                base(sfView_CreateFromRect(viewRect))
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Construct the view from its center and half-size
            /// </summary>
            /// <param name="center">Center of the view</param>
            /// <param name="halfSize">Half-size of the view (from center to corner)</param>
            ////////////////////////////////////////////////////////////
            public View(Vector2 center, Vector2 halfSize) :
                base(sfView_Create())
            {
                this.Center   = center;
                this.HalfSize = halfSize;
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Center of the view
            /// </summary>
            ////////////////////////////////////////////////////////////
            public Vector2 Center
            {
                get {return new Vector2(sfView_GetCenterX(This), sfView_GetCenterY(This));}
                set {sfView_SetCenter(This, value.X, value.Y);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Half-size of the view
            /// </summary>
            ////////////////////////////////////////////////////////////
            public Vector2 HalfSize
            {
                get {return new Vector2(sfView_GetHalfSizeX(This), sfView_GetHalfSizeY(This));}
                set {sfView_SetHalfSize(This, value.X, value.Y);}
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Rebuild the view from a rectangle
            /// </summary>
            /// <param name="viewRect">Rectangle defining the position and size of the view</param>
            ////////////////////////////////////////////////////////////
            public void SetFromRect(FloatRect viewRect)
            {
                sfView_SetFromRect(This, viewRect);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Get the rectangle defining the view
            /// </summary>
            /// <returns>Rectangle of the view</returns>
            ////////////////////////////////////////////////////////////
            public FloatRect GetRect()
            {
                return sfView_GetRect(This);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Move the view
            /// </summary>
            /// <param name="offset">Offset to move the view</param>
            ////////////////////////////////////////////////////////////
            public void Move(Vector2 offset)
            {
                sfView_Move(This, offset.X, offset.Y);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Resize the view rectangle to simulate a zoom / unzoom effect
            /// </summary>
            /// <param name="factor">Zoom factor to apply, relative to the current zoom</param>
            ////////////////////////////////////////////////////////////
            public void Zoom(float factor)
            {
                sfView_Zoom(This, factor);
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Internal constructor for other classes which need to manipulate raw views
            /// </summary>
            /// <param name="thisPtr">Direct pointer to the view object in the C library</param>
            ////////////////////////////////////////////////////////////
            internal View(IntPtr thisPtr) :
                base(thisPtr)
            {
            }

            ////////////////////////////////////////////////////////////
            /// <summary>
            /// Handle the destruction of the object
            /// </summary>
            /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
            ////////////////////////////////////////////////////////////
            protected override void Destroy(bool disposing)
            {
                sfView_Destroy(This);
            }

            #region Imports
            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfView_Create();

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern IntPtr sfView_CreateFromRect(FloatRect Rect);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfView_Destroy(IntPtr View);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfView_SetCenter(IntPtr View, float X, float Y);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfView_SetHalfSize(IntPtr View, float HalfWidth, float HalfHeight);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfView_SetFromRect(IntPtr View, FloatRect ViewRect);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern float sfView_GetCenterX(IntPtr View);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern float sfView_GetCenterY(IntPtr View);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern float sfView_GetHalfSizeX(IntPtr View);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern float sfView_GetHalfSizeY(IntPtr View);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern FloatRect sfView_GetRect(IntPtr View);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfView_Move(IntPtr View, float OffsetX, float OffsetY);

            [DllImport("csfml-graphics"), SuppressUnmanagedCodeSecurity]
            static extern void sfView_Zoom(IntPtr View, float Factor);

            #endregion
        }
    }
}

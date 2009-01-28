using System;
using System.Runtime.InteropServices;

namespace SFML
{
    ////////////////////////////////////////////////////////////
    /// <summary>
    /// The ObjectBase class is an abstract base for every
    /// SFML object. It's meant for internal use only
    /// </summary>
    ////////////////////////////////////////////////////////////
    public abstract class ObjectBase : IDisposable
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Construct the object from a pointer to the C library object
        /// </summary>
        /// <param name="thisPtr">Internal pointer to the object in the C libraries</param>
        ////////////////////////////////////////////////////////////
        public ObjectBase(IntPtr thisPtr)
        {
            myThis = thisPtr;
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Dispose the object
        /// </summary>
        ////////////////////////////////////////////////////////////
        ~ObjectBase()
        {
            Dispose(false);
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Access to the internal pointer of the object.
        /// For internal use only
        /// </summary>
        ////////////////////////////////////////////////////////////
        public IntPtr This
        {
            get {return myThis;}
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Explicitely dispose the object
        /// </summary>
        ////////////////////////////////////////////////////////////
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Destroy the object
        /// </summary>
        /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
        ////////////////////////////////////////////////////////////
        private void Dispose(bool disposing)
        {
            if (myThis != IntPtr.Zero)
            {
                Destroy(disposing);
                myThis = IntPtr.Zero;
            }
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Destroy the object (implementation is left to each derived class)
        /// </summary>
        /// <param name="disposing">Is the GC disposing the object, or is it an explicit call ?</param>
        ////////////////////////////////////////////////////////////
        protected abstract void Destroy(bool disposing);

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Set the pointer to the internal object. For internal use only
        /// </summary>
        /// <param name="thisPtr">Pointer to the internal object in C library</param>
        ////////////////////////////////////////////////////////////
        protected void SetThis(IntPtr thisPtr)
        {
            myThis = thisPtr;
        }

        private IntPtr myThis = IntPtr.Zero;
    }
}

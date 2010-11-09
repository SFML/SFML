using System;
using System.Runtime.Serialization;

namespace SFML
{
    ////////////////////////////////////////////////////////////
    /// <summary>
    /// Exception thrown by SFML whenever loading a resource fails
    /// </summary>
    ////////////////////////////////////////////////////////////
    [Serializable]
    public class LoadingFailedException : Exception
    {
        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Default constructor (unknown error)
        /// </summary>
        ////////////////////////////////////////////////////////////
        public LoadingFailedException() :
            base("Failed to load a resource")
        {
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Failure to load a resource from memory
        /// </summary>
        /// <param name="resourceName">Name of the resource</param>
        ////////////////////////////////////////////////////////////
        public LoadingFailedException(string resourceName) :
            base("Failed to load " + resourceName + " from memory")
        {
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Failure to load a resource from memory
        /// </summary>
        /// <param name="resourceName">Name of the resource</param>
        /// <param name="innerException">Exception which is the cause ofthe current exception</param>
        ////////////////////////////////////////////////////////////
        public LoadingFailedException(string resourceName, Exception innerException) :
            base("Failed to load " + resourceName + " from memory", innerException)
        {
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Failure to load a resource from a file
        /// </summary>
        /// <param name="resourceName">Name of the resource</param>
        /// <param name="filename">Path of the file</param>
        ////////////////////////////////////////////////////////////
        public LoadingFailedException(string resourceName, string filename) :
            base("Failed to load " + resourceName + " from file " + filename)
        {
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Failure to load a resource from a file
        /// </summary>
        /// <param name="resourceName">Name of the resource</param>
        /// <param name="filename">Path of the file</param>
        /// <param name="innerException">Exception which is the cause ofthe current exception</param>
        ////////////////////////////////////////////////////////////
        public LoadingFailedException(string resourceName, string filename, Exception innerException) :
            base("Failed to load " + resourceName + " from file " + filename, innerException)
        {
        }

        ////////////////////////////////////////////////////////////
        /// <summary>
        /// Initialize an instance of the exception with serialized data
        /// </summary>
        /// <param name="info">Serialized data</param>
        /// <param name="context">Contextual informations</param>
        ////////////////////////////////////////////////////////////
        public LoadingFailedException(SerializationInfo info, StreamingContext context) :
            base(info, context)
        {
        }
    }
}

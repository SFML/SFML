////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef SFML_GRAPHICSCONTEXT_HPP
#define SFML_GRAPHICSCONTEXT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/GLEW/glew.h>
#include <iostream>
#include <string>


namespace sf
{
class Context;

namespace priv
{
////////////////////////////////////////////////////////////
/// RAII class ensuring a valid shared OpenGL context is bound
/// to the current thread for all the lifetime of the instance.
/// It also ensures all third party libraries (like GLEW)
/// are properly initialized
/// This class is for internal use only, it is required
/// to solve tricky problems involving multi-threading
////////////////////////////////////////////////////////////
class GraphicsContext : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor, activate the global context
    /// if no other context is bound to the current thread
    ///
    ////////////////////////////////////////////////////////////
    GraphicsContext();

    ////////////////////////////////////////////////////////////
    /// Destructor, deactivate the global context
    /// if no other context was previously bound to the current thread
    ///
    ////////////////////////////////////////////////////////////
    ~GraphicsContext();

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    bool myActivated; ///< Keep track of the activation state of the global context
};

} // namespace priv


////////////////////////////////////////////////////////////
/// Let's define a macro to quickly check every OpenGL
/// API calls
////////////////////////////////////////////////////////////
#ifdef SFML_DEBUG

    // In debug mode, perform a test on every OpenGL call
    #define GLCheck(Func) ((Func), GLCheckError(__FILE__, __LINE__))

#else

    // Else, we don't add any overhead
    #define GLCheck(Func) (Func)

#endif


////////////////////////////////////////////////////////////
/// Check the last OpenGL error
///
////////////////////////////////////////////////////////////
inline void GLCheckError(const std::string& File, unsigned int Line)
{
    // Get the last error
    GLenum ErrorCode = glGetError();

    if (ErrorCode != GL_NO_ERROR)
    {
        std::string Error = "unknown error";
        std::string Desc  = "no description";

        // Decode the error code
        switch (ErrorCode)
        {
            case GL_INVALID_ENUM :
            {
                Error = "GL_INVALID_ENUM";
                Desc  = "an unacceptable value has been specified for an enumerated argument";
                break;
            }

            case GL_INVALID_VALUE :
            {
                Error = "GL_INVALID_VALUE";
                Desc  = "a numeric argument is out of range";
                break;
            }

            case GL_INVALID_OPERATION :
            {
                Error = "GL_INVALID_OPERATION";
                Desc  = "the specified operation is not allowed in the current state";
                break;
            }

            case GL_STACK_OVERFLOW :
            {
                Error = "GL_STACK_OVERFLOW";
                Desc  = "this command would cause a stack overflow";
                break;
            }

            case GL_STACK_UNDERFLOW :
            {
                Error = "GL_STACK_UNDERFLOW";
                Desc  = "this command would cause a stack underflow";
                break;
            }

            case GL_OUT_OF_MEMORY :
            {
                Error = "GL_OUT_OF_MEMORY";
                Desc  = "there is not enough memory left to execute the command";
                break;
            }

            case GL_INVALID_FRAMEBUFFER_OPERATION_EXT :
            {
                Error = "GL_INVALID_FRAMEBUFFER_OPERATION_EXT";
                Desc  = "the object bound to FRAMEBUFFER_BINDING_EXT is not \"framebuffer complete\"";
                break;
            }
        }

        // Log the error
        std::cerr << "An internal OpenGL call failed in "
                  << File.substr(File.find_last_of("\\/") + 1) << " (" << Line << ") : "
                  << Error << ", " << Desc
                  << std::endl;
    }
}

} // namespace sf


#endif // SFML_GRAPHICSCONTEXT_HPP

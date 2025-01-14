////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Graphics/GLExtensions.hpp>

#include <SFML/System/Err.hpp>

#include <filesystem>
#include <ostream>


namespace sf::priv
{
////////////////////////////////////////////////////////////
bool glCheckError(std::string_view file, unsigned int line, std::string_view expression)
{
    const auto logError = [&](const char* error, const char* description)
    {
        err() << "An internal OpenGL call failed in " << std::filesystem::path(file).filename() << "(" << line << ")."
              << "\nExpression:\n   " << expression << "\nError description:\n   " << error << "\n   " << description << '\n'
              << std::endl;

        return false;
    };

    switch (glGetError())
    {
        case GL_NO_ERROR:
            return true;

        case GL_INVALID_ENUM:
            return logError("GL_INVALID_ENUM", "An unacceptable value has been specified for an enumerated argument.");

        case GL_INVALID_VALUE:
            return logError("GL_INVALID_VALUE", "A numeric argument is out of range.");

        case GL_INVALID_OPERATION:
            return logError("GL_INVALID_OPERATION", "The specified operation is not allowed in the current state.");

        case GL_STACK_OVERFLOW:
            return logError("GL_STACK_OVERFLOW", "This command would cause a stack overflow.");

        case GL_STACK_UNDERFLOW:
            return logError("GL_STACK_UNDERFLOW", "This command would cause a stack underflow.");

        case GL_OUT_OF_MEMORY:
            return logError("GL_OUT_OF_MEMORY", "There is not enough memory left to execute the command.");

        case GLEXT_GL_INVALID_FRAMEBUFFER_OPERATION:
            return logError("GL_INVALID_FRAMEBUFFER_OPERATION",
                            "The object bound to FRAMEBUFFER_BINDING is not \"framebuffer complete\".");

        default:
            return logError("Unknown error", "Unknown description");
    }
}

} // namespace sf::priv

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/GLExtensions.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/Window/Context.hpp>

// We check for this definition in order to avoid multiple definitions of GLAD
// entities during unity builds of SFML.
#ifndef SF_GLAD_GL_IMPLEMENTATION_INCLUDED
#define SF_GLAD_GL_IMPLEMENTATION_INCLUDED
#define SF_GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#endif

#include <ostream>

#if !defined(GL_MAJOR_VERSION)
#define GL_MAJOR_VERSION 0x821B
#endif

#if !defined(GL_MINOR_VERSION)
#define GL_MINOR_VERSION 0x821C
#endif


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void ensureExtensionsInit()
{
    static bool initialized = false;
    if (!initialized)
    {
        initialized = true;

#ifdef SFML_OPENGL_ES
        gladLoadGLES1(reinterpret_cast<GLADloadfunc>(sf::Context::getFunction));
#else
        gladLoadGL(reinterpret_cast<GLADloadfunc>(sf::Context::getFunction));
#endif

        // Retrieve the context version number
        int majorVersion = 0;
        int minorVersion = 0;

        // Try the new way first
        glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
        glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

        if (glGetError() == GL_INVALID_ENUM)
        {
            // Try the old way
            const GLubyte* version = glGetString(GL_VERSION);
            if (version)
            {
                // The beginning of the returned string is "major.minor" (this is standard)
                majorVersion = version[0] - '0';
                minorVersion = version[2] - '0';
            }
            else
            {
                // Can't get the version number, assume 1.1
                majorVersion = 1;
                minorVersion = 1;
            }
        }

        if ((majorVersion < 1) || ((majorVersion == 1) && (minorVersion < 1)))
        {
            err() << "sfml-graphics requires support for OpenGL 1.1 or greater" << '\n'
                  << "Ensure that hardware acceleration is enabled if available" << std::endl;
        }
    }
}

} // namespace priv

} // namespace sf

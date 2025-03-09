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
#include <SFML/Graphics/GLExtensions.hpp>

#include <SFML/Window/Context.hpp>

#include <SFML/System/Err.hpp>

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


namespace
{
////////////////////////////////////////////////////////////
void extensionSanityCheck()
{
    static const auto check = [](int& flag, auto... entryPoints)
    {
        // If a required entry point is missing, flag the whole extension as unavailable
        if (!(entryPoints && ...))
            flag = 0;
    };
#ifdef SFML_OPENGL_ES
    check(GLEXT_multitexture_dependencies);
    check(GLEXT_vertex_buffer_object_dependencies);
    check(GLEXT_OES_blend_subtract_dependencies);
    check(GLEXT_blend_func_separate_dependencies);
    check(GLEXT_blend_equation_separate_dependencies);
    check(GLEXT_framebuffer_object_dependencies);
    check(GLEXT_EXT_blend_minmax_dependencies);
#else
    check(GLEXT_blend_minmax_dependencies);
    check(GLEXT_multitexture_dependencies);
    check(GLEXT_blend_func_separate_dependencies);
    check(GLEXT_vertex_buffer_object_dependencies);
    check(GLEXT_shader_objects_dependencies);
    check(GLEXT_blend_equation_separate_dependencies);
    check(GLEXT_framebuffer_object_dependencies);
    check(GLEXT_framebuffer_blit_dependencies);
    check(GLEXT_framebuffer_multisample_dependencies);
    check(GLEXT_copy_buffer_dependencies);
#endif
}
} // namespace

namespace sf::priv
{
////////////////////////////////////////////////////////////
void ensureExtensionsInit()
{
    static bool initialized = false;
    if (!initialized)
    {
        initialized = true;

#ifdef SFML_OPENGL_ES
        gladLoadGLES1(Context::getFunction);
#else
        gladLoadGL(Context::getFunction);
#endif
        // Some GL implementations don't fully follow extension specifications
        // and advertise support for extensions although not providing the
        // entry points specified for the corresponding extension.
        // In order to protect ourselves from such implementations, we perform
        // a sanity check to ensure an extension is _really_ supported, even
        // from an entry point perspective.
        extensionSanityCheck();

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

} // namespace sf::priv

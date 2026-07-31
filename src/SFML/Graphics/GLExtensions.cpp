////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2026 Laurent Gomila (laurent@sfml-dev.org)
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

#include <SFML/System/Exception.hpp>

// We check for this definition in order to avoid multiple definitions of GLAD
// entities during unity builds of SFML.
#ifndef SF_GLAD_GL_IMPLEMENTATION_INCLUDED
#define SF_GLAD_GL_IMPLEMENTATION_INCLUDED
#define SF_GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#endif

#include <mutex>
#include <string>
#include <type_traits>

namespace
{
////////////////////////////////////////////////////////////
void extensionSanityCheck()
{
    static const auto check = [](auto&& flag, auto... entryPoints)
    {
        // If a required entry point is missing, flag the whole extension as unavailable
        if (!(entryPoints && ...))
        {
            if constexpr (std::is_lvalue_reference_v<decltype(flag)> &&
                          !std::is_const_v<std::remove_reference_t<decltype(flag)>>)
                flag = 0;
        }
    };
#ifdef SFML_OPENGL_ES
    check(GLEXT_multitexture_dependencies);
    check(GLEXT_vertex_buffer_object_dependencies);
    check(GLEXT_blend_subtract_dependencies);
    check(GLEXT_blend_func_separate_dependencies);
    check(GLEXT_blend_equation_separate_dependencies);
    check(GLEXT_framebuffer_object_dependencies);
    check(GLEXT_blend_minmax_dependencies);
#else
    check(GLEXT_blend_minmax_dependencies);
    check(GLEXT_multitexture_dependencies);
    check(GLEXT_blend_func_separate_dependencies);
    check(GLEXT_vertex_buffer_object_dependencies);
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
    static std::mutex mutex;
    const std::lock_guard lock(mutex);

    if (Context::getActiveContextId() == 0)
        throw Exception("Cannot initialize OpenGL entry points without an active context");

    const auto getString = reinterpret_cast<PFNGLGETSTRINGPROC>(Context::getFunction("glGetString"));
    if (!getString)
        throw Exception("Cannot query the active OpenGL context version");

    const auto* version = reinterpret_cast<const char*>(getString(GL_VERSION));
    if (!version)
        throw Exception("Cannot query the active OpenGL context version");

    static std::string loadedContextVersion;

    if (loadedContextVersion != version)
    {
        int loadedVersion = 0;
#ifdef SFML_OPENGL_ES
        loadedVersion = gladLoadGLES2(Context::getFunction);
#else
        loadedVersion = gladLoadGL(Context::getFunction);
#endif

        if (!loadedVersion || (GLAD_VERSION_MAJOR(loadedVersion) < 2))
            throw Exception("Failed to load the OpenGL 2.0 shader rendering entry points required by sfml-graphics");

        loadedContextVersion = version;

        // Some GL implementations don't fully follow extension specifications
        // and advertise support for extensions although not providing the
        // entry points specified for the corresponding extension.
        // In order to protect ourselves from such implementations, we perform
        // a sanity check to ensure an extension is _really_ supported, even
        // from an entry point perspective.
        extensionSanityCheck();
    }
}

} // namespace sf::priv

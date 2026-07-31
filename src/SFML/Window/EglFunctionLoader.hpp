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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Context.hpp>

#include <glad/egl.h>

#include <array>
#include <cstdint>

#if defined(SFML_SYSTEM_WINDOWS)
#include <SFML/System/Win32/WindowsHeader.hpp>
#else
#include <dlfcn.h>
#endif


namespace sf::priv
{
////////////////////////////////////////////////////////////
inline GlFunctionPointer getEglGlFunction(const char* name)
{
    if (const auto address = eglGetProcAddress(name))
        return reinterpret_cast<GlFunctionPointer>(address);

    // EGL does not require eglGetProcAddress to return core GL/GLES
    // functions, so fall back to the linked client library.
#if defined(SFML_SYSTEM_WINDOWS)
    static const HMODULE module = []
    {
#ifdef SFML_OPENGL_ES
        static constexpr std::array libraries = {"libGLESv2.dll", "GLESv2.dll"};
#else
        static constexpr std::array libraries = {"opengl32.dll"};
#endif

        for (const char* library : libraries)
        {
            if (const HMODULE handle = GetModuleHandleA(library))
                return handle;

            if (const HMODULE handle = LoadLibraryA(library))
                return handle;
        }

        return static_cast<HMODULE>(nullptr);
    }();

    return module ? reinterpret_cast<GlFunctionPointer>(GetProcAddress(module, name)) : nullptr;
#else
    if (void* address = dlsym(RTLD_DEFAULT, name))
        return reinterpret_cast<GlFunctionPointer>(reinterpret_cast<std::uintptr_t>(address));

    static void* const module = []
    {
#ifdef SFML_OPENGL_ES
        static constexpr std::array libraries = {"libGLESv2.so.2", "libGLESv2.so"};
#else
        static constexpr std::array libraries = {"libOpenGL.so.0", "libGL.so.1", "libGL.so"};
#endif

        for (const char* library : libraries)
        {
            if (void* handle = dlopen(library, RTLD_LAZY | RTLD_LOCAL))
                return handle;
        }

        return static_cast<void*>(nullptr);
    }();

    if (!module)
        return nullptr;

    return reinterpret_cast<GlFunctionPointer>(reinterpret_cast<std::uintptr_t>(dlsym(module, name)));
#endif
}

} // namespace sf::priv


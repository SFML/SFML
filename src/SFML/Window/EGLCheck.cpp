////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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
#include <SFML/Window/EGLCheck.hpp>

#include <SFML/System/Err.hpp>

#include <glad/egl.h>

#include <filesystem>
#include <ostream>

namespace
{
namespace eglCheckImpl
{
////////////////////////////////////////////////////////////
bool checkError(std::string_view file, unsigned int line, std::string_view expression)
{
    const auto logError = [&](const char* error, const char* description)
    {
        sf::err() << "An internal EGL call failed in " << std::filesystem::path(file).filename() << "(" << line << ")."
                  << "\nExpression:\n   " << expression << "\nError description:\n   " << error << "\n   "
                  << description << '\n'
                  << std::endl;

        return false;
    };

    switch (eglGetError())
    {
        case EGL_SUCCESS:
            return true;

        case EGL_NOT_INITIALIZED:
            return logError("EGL_NOT_INITIALIZED",
                            "EGL is not initialized, or could not be initialized, for the specified display");

        case EGL_BAD_ACCESS:
            return logError("EGL_BAD_ACCESS",
                            "EGL cannot access a requested resource (for example, a context is bound in another "
                            "thread)");

        case EGL_BAD_ALLOC:
            return logError("EGL_BAD_ALLOC", "EGL failed to allocate resources for the requested operation");

        case EGL_BAD_ATTRIBUTE:
            return logError("EGL_BAD_ATTRIBUTE",
                            "an unrecognized attribute or attribute value was passed in an attribute list");

        case EGL_BAD_CONTEXT:
            return logError("EGL_BAD_CONTEXT", "an EGLContext argument does not name a valid EGLContext");

        case EGL_BAD_CONFIG:
            return logError("EGL_BAD_CONFIG", "an EGLConfig argument does not name a valid EGLConfig");

        case EGL_BAD_CURRENT_SURFACE:
            return logError("EGL_BAD_CURRENT_SURFACE",
                            "the current surface of the calling thread is a window, pbuffer, or pixmap that is no "
                            "longer valid");

        case EGL_BAD_DISPLAY:
            return logError("EGL_BAD_DISPLAY",
                            "an EGLDisplay argument does not name a valid EGLDisplay; or, EGL is not initialized on "
                            "the specified EGLDisplay");


        case EGL_BAD_SURFACE:
            return logError("EGL_BAD_SURFACE",
                            "an EGLSurface argument does not name a valid surface (window, pbuffer, or pixmap) "
                            "configured for rendering");

        case EGL_BAD_MATCH:
            return logError("EGL_BAD_MATCH",
                            "arguments are inconsistent; for example, an otherwise valid context requires buffers "
                            "(e.g. depth or stencil) not allocated by an otherwise valid surface");

        case EGL_BAD_PARAMETER:
            return logError("EGL_BAD_PARAMETER", "one or more argument values are invalid");

        case EGL_BAD_NATIVE_PIXMAP:
            return logError("EGL_BAD_NATIVE_PIXMAP",
                            "an EGLNativePixmapType argument does not refer to a valid native pixmap");

        case EGL_BAD_NATIVE_WINDOW:
            return logError("EGL_BAD_NATIVE_WINDOW",
                            "an EGLNativeWindowType argument does not refer to a valid native window");

        case EGL_CONTEXT_LOST:
            return logError("EGL_CONTEXT_LOST",
                            "a power management event has occurred. The application must destroy all contexts and "
                            "reinitialize client API state and objects to continue rendering");

        default:
            return logError("Unknown error", "Unknown description");
    }
}
} // namespace eglCheckImpl
} // namespace


namespace sf::priv
{
////////////////////////////////////////////////////////////
EglScopedChecker::EglScopedChecker(const std::string_view file, const std::string_view expression, const unsigned int line) :
    m_file(file),
    m_expression(expression),
    m_line(line)
{
    if (const auto error = eglGetError(); error != EGL_SUCCESS)
        err() << "EGL error (" << error << ") detected during eglCheck call" << std::endl;
}


////////////////////////////////////////////////////////////
EglScopedChecker::~EglScopedChecker()
{
    while (!eglCheckImpl::checkError(m_file, m_line, m_expression))
        /* no-op */;
}

} // namespace sf::priv

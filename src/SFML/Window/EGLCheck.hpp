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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>

#include <SFML/System/Err.hpp>

#include <glad/egl.h>

#include <ostream>
#include <string_view>
#include <type_traits>


namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief Check the last EGL error
///
/// \param file Source file where the call is located
/// \param line Line number of the source file where the call is located
/// \param expression The evaluated expression as a string
///
/// \return `false` if an error occurred, `true` otherwise
///
////////////////////////////////////////////////////////////
bool eglCheckError(std::string_view file, unsigned int line, std::string_view expression);

////////////////////////////////////////////////////////////
/// Macro to quickly check every EGL API call
////////////////////////////////////////////////////////////
#ifdef SFML_DEBUG

// In debug mode, perform a test on every EGL call
// The lamdba allows us to call eglCheck as an expression and acts as a single statement perfect for if/else statements
#define eglCheck(...)                                                                                              \
    [](auto&& eglCheckInternalFunction)                                                                            \
    {                                                                                                              \
        if (const EGLint eglCheckInternalError = eglGetError(); eglCheckInternalError != EGL_SUCCESS)              \
            sf::err() << "EGL error (" << eglCheckInternalError << ") detected during eglCheck call" << std::endl; \
                                                                                                                   \
        if constexpr (!std::is_void_v<decltype(eglCheckInternalFunction())>)                                       \
        {                                                                                                          \
            const auto eglCheckInternalReturnValue = eglCheckInternalFunction();                                   \
                                                                                                                   \
            while (!sf::priv::eglCheckError(__FILE__, static_cast<unsigned int>(__LINE__), #__VA_ARGS__))          \
                /* no-op */;                                                                                       \
                                                                                                                   \
            return eglCheckInternalReturnValue;                                                                    \
        }                                                                                                          \
        else                                                                                                       \
        {                                                                                                          \
            eglCheckInternalFunction();                                                                            \
                                                                                                                   \
            while (!sf::priv::eglCheckError(__FILE__, static_cast<unsigned int>(__LINE__), #__VA_ARGS__))          \
                /* no-op */;                                                                                       \
        }                                                                                                          \
    }([&] { return __VA_ARGS__; })

#else

// Else, we don't add any overhead
#define eglCheck(...) (__VA_ARGS__)

#endif


} // namespace sf::priv

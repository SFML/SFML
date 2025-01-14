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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>

#include <SFML/Graphics/GLExtensions.hpp>

#include <SFML/System/Err.hpp>

#include <ostream>
#include <string_view>
#include <type_traits>


namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief Check the last OpenGL error
///
/// \param file Source file where the call is located
/// \param line Line number of the source file where the call is located
/// \param expression The evaluated expression as a string
///
/// \return `false` if an error occurred, `true` otherwise
///
////////////////////////////////////////////////////////////
bool glCheckError(std::string_view file, unsigned int line, std::string_view expression);

////////////////////////////////////////////////////////////
/// Macro to quickly check every OpenGL API call
////////////////////////////////////////////////////////////
#ifdef SFML_DEBUG
// In debug mode, perform a test on every OpenGL call
// The lamdba allows us to call glCheck as an expression and acts as a single statement perfect for if/else statements
#define glCheck(...)                                                                                                \
    [](auto&& glCheckInternalFunction)                                                                              \
    {                                                                                                               \
        if (const GLenum glCheckInternalError = glGetError(); glCheckInternalError != GL_NO_ERROR)                  \
            sf::err() << "OpenGL error (" << glCheckInternalError << ") detected during glCheck call" << std::endl; \
                                                                                                                    \
        if constexpr (!std::is_void_v<decltype(glCheckInternalFunction())>)                                         \
        {                                                                                                           \
            const auto glCheckInternalReturnValue = glCheckInternalFunction();                                      \
                                                                                                                    \
            while (!sf::priv::glCheckError(__FILE__, static_cast<unsigned int>(__LINE__), #__VA_ARGS__))            \
                /* no-op */;                                                                                        \
                                                                                                                    \
            return glCheckInternalReturnValue;                                                                      \
        }                                                                                                           \
        else                                                                                                        \
        {                                                                                                           \
            glCheckInternalFunction();                                                                              \
                                                                                                                    \
            while (!sf::priv::glCheckError(__FILE__, static_cast<unsigned int>(__LINE__), #__VA_ARGS__))            \
                /* no-op */;                                                                                        \
        }                                                                                                           \
    }([&] { return __VA_ARGS__; })
#else

// Else, we don't add any overhead
#define glCheck(...) (__VA_ARGS__)

#endif

} // namespace sf::priv

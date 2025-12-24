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

#include <string_view>


namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief Helper class to check for OpenGL errors in debug mode
///
/// This RAII style class is used internally to detect and report OpenGL
/// errors during development. It captures the location of an OpenGL call
/// and checks for errors when is object is destroyed (i.e. after the
/// OpenGL call executes).
///
////////////////////////////////////////////////////////////
class GlScopedChecker
{
public:
    GlScopedChecker(std::string_view file, std::string_view expression, unsigned int line);
    ~GlScopedChecker();

private:
    const std::string_view m_file;
    const std::string_view m_expression;
    const unsigned int     m_line;
};

////////////////////////////////////////////////////////////
/// Macro to quickly check every OpenGL API call
////////////////////////////////////////////////////////////
#ifdef SFML_DEBUG
// In debug mode, perform a test on every OpenGL call
// The comma operator returns the second operand while sequencing the first operand
#define glCheck(...) \
    (sf::priv::GlScopedChecker(__FILE__, #__VA_ARGS__, static_cast<unsigned int>(__LINE__)), __VA_ARGS__)

#else

// Else, we don't add any overhead
#define glCheck(...) (__VA_ARGS__)

#endif

} // namespace sf::priv

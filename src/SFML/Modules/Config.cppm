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
// Module preamble
////////////////////////////////////////////////////////////

module;

#include <SFML/Config.hpp>

export module sfml.Config;

export namespace sf {
    constexpr inline int VERSION_MAJOR = SFML_VERSION_MAJOR;
    constexpr inline int VERSION_MINOR = SFML_VERSION_MINOR;
    constexpr inline int VERSION_PATCH = SFML_VERSION_PATCH;
    constexpr inline bool VERSION_IS_RELEASE = SFML_VERSION_IS_RELEASE;
}
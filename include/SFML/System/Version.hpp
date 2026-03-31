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
#include <SFML/System/Export.hpp>

#include <string_view>

#include <cstdint>


namespace sf
{
struct Version
{
    const std::int8_t      major;     //!< SFML major version
    const std::int8_t      minor;     //!< SFML minor version
    const std::int8_t      patch;     //!< SFML patch version
    const bool             isRelease; //!< `true` if this is a release version, `false` if this is a development version
    const std::string_view string;    //!< String representation of the SFML version, e.g. 3.1.0 or 3.1.0-dev
};

////////////////////////////////////////////////////////////
/// \brief Retrieve the runtime version of the SFML library
///
/// The SFML_VERSION_MAJOR, SFML_VERSION_MINOR,
/// SFML_VERSION_PATCH and SFML_VERSION_IS_RELEASE defines
/// only provide a way to determine the SFML library version
/// at compile time. If an application is dynamically linked
/// to SFML, the version of the library that is loaded at
/// runtime might not be the same as the version of the
/// library headers which the application included when it
/// was built. In order for an application to determine
/// which SFML version is currently loaded at runtime, it
/// can use this function. This function relies on version
/// information embedded into the loaded library when it was
/// built. This information can be useful when an application
/// has to determine the SFML version in order to diagnose
/// problems and be able to report them to the library
/// maintainers.
///
/// \return The version of the SFML library
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_SYSTEM_API const Version& version();
} // namespace sf

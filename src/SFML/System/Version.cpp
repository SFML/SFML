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
#include <SFML/System/Version.hpp>


namespace sf
{
const Version& version()
{
#if SFML_VERSION_IS_RELEASE
#define SFML_VERSION_SUFFIX
#else
#define SFML_VERSION_SUFFIX "-dev"
#endif
#define STR_HELPER(x) #x
#define STR(x)        STR_HELPER(x)
    static constexpr Version sfmlVersion{SFML_VERSION_MAJOR,
                                         SFML_VERSION_MINOR,
                                         SFML_VERSION_PATCH,
                                         SFML_VERSION_IS_RELEASE,
                                         STR(SFML_VERSION_MAJOR) "." STR(SFML_VERSION_MINOR) "." STR(SFML_VERSION_PATCH)
                                             SFML_VERSION_SUFFIX};
    return sfmlVersion;
}
} // namespace sf

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
#include <SFML/System/StandardPaths.hpp>

#include <filesystem>
#include <optional>


namespace sf::priv::StandardPaths
{

////////////////////////////////////////////////////////////
/// \copydoc sf::getResourceDirectory
///
////////////////////////////////////////////////////////////
std::filesystem::path getResourceDirectory();

////////////////////////////////////////////////////////////
/// \brief Get the base directory used to derive `sf::getUserDataDirectory`
///
/// The public `sf::getUserDataDirectory` appends the organization and
/// application names to the value returned here.
///
////////////////////////////////////////////////////////////
std::filesystem::path getUserDataBaseDirectory();

////////////////////////////////////////////////////////////
/// \copydoc sf::getUserFolder
///
////////////////////////////////////////////////////////////
std::optional<std::filesystem::path> getUserFolder(UserFolder folder);

} // namespace sf::priv::StandardPaths

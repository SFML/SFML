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

#include <filesystem>
#include <optional>
#include <string_view>


namespace sf
{
////////////////////////////////////////////////////////////
/// \ingroup system
/// \brief Well-known user folders that may be queried via `getUserFolder`
///
////////////////////////////////////////////////////////////
enum class UserFolder
{
    Home,      //!< User's home directory
    Desktop,   //!< User's desktop directory
    Documents, //!< User's documents directory
    Downloads, //!< User's downloads directory
    Pictures,  //!< User's pictures directory
    Music,     //!< User's music directory
    Videos     //!< User's videos directory
};

////////////////////////////////////////////////////////////
/// \ingroup system
/// \brief Get the directory containing the application's bundled, read-only resources
///
/// The returned path is platform-specific:
/// \li Windows / Linux / BSD: directory containing the running executable
/// \li macOS: `Contents/Resources` of the application bundle, or the executable
///     directory when not running from a bundle
/// \li iOS: the application bundle's resource directory
/// \li Android: empty path (resources live inside the APK and must be read via
///     `sf::FileInputStream`, which transparently uses the asset manager)
///
/// Use this when locating game assets that ship alongside the executable, instead
/// of relying on the current working directory, which is unreliable for bundled
/// applications.
///
/// \return Path to the resource directory, or an empty path if it could not be determined
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_SYSTEM_API std::filesystem::path getResourceDirectory();

////////////////////////////////////////////////////////////
/// \ingroup system
/// \brief Get a per-application directory suitable for read/write user data
///
/// Use this for save games, settings, and other persistent application data. The
/// returned directory is namespaced by `organization` and `application` to avoid
/// collisions with other applications. The directory is not guaranteed to exist
/// on disk; create it before writing.
///
/// The base location depends on the platform:
/// \li Windows: `%APPDATA%\<organization>\<application>`
/// \li Linux / BSD: `$XDG_DATA_HOME/<organization>/<application>` or
///     `~/.local/share/<organization>/<application>`
/// \li macOS: `~/Library/Application Support/<organization>/<application>`
/// \li iOS: `<sandbox>/Library/Application Support/<organization>/<application>`
/// \li Android: the activity's internal data path, with org/app subdirectories
///
/// \param organization Name of the organization (used as a parent directory)
/// \param application  Name of the application (used as a leaf directory)
///
/// \return Path to the user data directory, or an empty path on failure
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_SYSTEM_API std::filesystem::path getUserDataDirectory(std::string_view organization,
                                                                        std::string_view application);

////////////////////////////////////////////////////////////
/// \ingroup system
/// \brief Get the path to a well-known user folder
///
/// Returns `std::nullopt` if the folder does not exist or has no meaningful path
/// on the current platform. For example, on iOS only `Home` and `Documents`
/// resolve to a real path; on Android most folders return `std::nullopt` because
/// access is mediated by `MediaStore` rather than a direct filesystem path.
///
/// \param folder The well-known folder to look up
///
/// \return Path to the folder, or `std::nullopt` if unavailable
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_SYSTEM_API std::optional<std::filesystem::path> getUserFolder(UserFolder folder);

} // namespace sf

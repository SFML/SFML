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
#include <SFML/System/Android/Activity.hpp>
#include <SFML/System/StandardPathsImpl.hpp>


namespace sf::priv::StandardPaths
{
////////////////////////////////////////////////////////////
std::filesystem::path getResourceDirectory()
{
    // Android resources live inside the APK and are not addressable as a
    // filesystem path; they must be read via the asset manager (which
    // sf::FileInputStream does automatically). Return an empty path to
    // signal this.
    return {};
}


////////////////////////////////////////////////////////////
std::filesystem::path getUserDataBaseDirectory()
{
    if (ActivityStates* states = getActivityStatesPtr(); states != nullptr && states->activity != nullptr)
    {
        if (const char* path = states->activity->internalDataPath; path != nullptr)
            return path;
    }
    return {};
}


////////////////////////////////////////////////////////////
std::optional<std::filesystem::path> getUserFolder(UserFolder folder)
{
    // Scoped storage on modern Android forbids direct filesystem access to
    // public folders; apps must use MediaStore / SAF instead. The only
    // path-addressable directory is the app's private home (internalDataPath).
    if (folder == UserFolder::Home)
    {
        if (ActivityStates* states = getActivityStatesPtr(); states != nullptr && states->activity != nullptr)
        {
            if (const char* path = states->activity->internalDataPath; path != nullptr)
                return std::filesystem::path(path);
        }
    }

    return std::nullopt;
}

} // namespace sf::priv::StandardPaths

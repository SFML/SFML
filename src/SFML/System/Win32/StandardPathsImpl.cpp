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
#include <SFML/System/StandardPathsImpl.hpp>
#include <SFML/System/Win32/WindowsHeader.hpp>

#include <ShlObj.h>
#include <KnownFolders.h>


namespace
{
std::filesystem::path knownFolderPath(REFKNOWNFOLDERID id)
{
    PWSTR rawPath = nullptr;
    if (FAILED(SHGetKnownFolderPath(id, 0, nullptr, &rawPath)) || rawPath == nullptr)
    {
        if (rawPath != nullptr)
            CoTaskMemFree(rawPath);
        return {};
    }

    std::filesystem::path result(rawPath);
    CoTaskMemFree(rawPath);
    return result;
}
} // namespace


namespace sf::priv::StandardPaths
{
////////////////////////////////////////////////////////////
std::filesystem::path getResourceDirectory()
{
    wchar_t buffer[MAX_PATH] = {};
    const DWORD length = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    if (length == 0 || length == MAX_PATH)
        return {};

    return std::filesystem::path(buffer).parent_path();
}


////////////////////////////////////////////////////////////
std::filesystem::path getUserDataBaseDirectory()
{
    return knownFolderPath(FOLDERID_RoamingAppData);
}


////////////////////////////////////////////////////////////
std::optional<std::filesystem::path> getUserFolder(UserFolder folder)
{
    KNOWNFOLDERID id{};
    switch (folder)
    {
        case UserFolder::Home:
            id = FOLDERID_Profile;
            break;
        case UserFolder::Desktop:
            id = FOLDERID_Desktop;
            break;
        case UserFolder::Documents:
            id = FOLDERID_Documents;
            break;
        case UserFolder::Downloads:
            id = FOLDERID_Downloads;
            break;
        case UserFolder::Pictures:
            id = FOLDERID_Pictures;
            break;
        case UserFolder::Music:
            id = FOLDERID_Music;
            break;
        case UserFolder::Videos:
            id = FOLDERID_Videos;
            break;
    }

    auto path = knownFolderPath(id);
    if (path.empty())
        return std::nullopt;
    return path;
}

} // namespace sf::priv::StandardPaths

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

#include <fstream>
#include <string>
#include <system_error>

#include <cstdlib>
#include <pwd.h>
#include <unistd.h>

#if defined(SFML_SYSTEM_FREEBSD) || defined(SFML_SYSTEM_NETBSD) || defined(SFML_SYSTEM_OPENBSD)
#include <sys/sysctl.h>
#include <sys/types.h>
#endif


namespace
{
////////////////////////////////////////////////////////////
std::filesystem::path getHomeDirectory()
{
    if (const char* env = std::getenv("HOME"); env != nullptr && *env != '\0')
        return env;

    if (const passwd* pw = getpwuid(geteuid()); pw != nullptr && pw->pw_dir != nullptr)
        return pw->pw_dir;

    return {};
}


////////////////////////////////////////////////////////////
// Parse the XDG user-dirs.dirs file to look up an entry like
// `XDG_DOCUMENTS_DIR="$HOME/Documents"`. Returns an empty path if
// the file or key is not present.
////////////////////////////////////////////////////////////
std::filesystem::path lookupXdgUserDir(const std::string& key)
{
    const std::filesystem::path home = getHomeDirectory();

    std::filesystem::path configDir;
    if (const char* env = std::getenv("XDG_CONFIG_HOME"); env != nullptr && *env != '\0')
        configDir = env;
    else if (!home.empty())
        configDir = home / ".config";
    else
        return {};

    std::ifstream file(configDir / "user-dirs.dirs");
    if (!file)
        return {};

    std::string line;
    while (std::getline(file, line))
    {
        // Skip comments and blanks
        const auto first = line.find_first_not_of(" \t");
        if (first == std::string::npos || line[first] == '#')
            continue;

        if (line.compare(first, key.size(), key) != 0)
            continue;
        const auto eq = line.find('=', first + key.size());
        if (eq == std::string::npos)
            continue;

        // Value is quoted: XDG_DOCUMENTS_DIR="$HOME/Documents"
        const auto quoteStart = line.find('"', eq);
        const auto quoteEnd   = (quoteStart == std::string::npos) ? std::string::npos : line.find('"', quoteStart + 1);
        if (quoteStart == std::string::npos || quoteEnd == std::string::npos)
            continue;

        std::string value = line.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
        const std::string prefix = "$HOME";
        if (value.compare(0, prefix.size(), prefix) == 0)
        {
            if (home.empty())
                return {};
            return home / std::filesystem::path(value.substr(prefix.size() + 1)); // skip the trailing '/'
        }
        return value;
    }

    return {};
}


////////////////////////////////////////////////////////////
std::filesystem::path executablePath()
{
#if defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_ANDROID)
    std::error_code ec;
    auto            path = std::filesystem::read_symlink("/proc/self/exe", ec);
    if (!ec)
        return path;
#elif defined(SFML_SYSTEM_FREEBSD) || defined(SFML_SYSTEM_NETBSD)
    // Try the procfs route first (often disabled by default on these)
    std::error_code ec;
#if defined(SFML_SYSTEM_FREEBSD)
    auto path = std::filesystem::read_symlink("/proc/curproc/file", ec);
#else
    auto path = std::filesystem::read_symlink("/proc/curproc/exe", ec);
#endif
    if (!ec)
        return path;

    // Fall back to KERN_PROC_PATHNAME via sysctl
    int    mib[4]   = {CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1};
    char   buffer[4096] = {};
    size_t length   = sizeof(buffer);
    if (sysctl(mib, 4, buffer, &length, nullptr, 0) == 0 && length > 0)
        return std::filesystem::path(std::string(buffer, length - 1));
#elif defined(SFML_SYSTEM_OPENBSD)
    // OpenBSD does not expose the executable path; nothing we can do reliably.
#endif

    return {};
}
} // namespace


namespace sf::priv::StandardPaths
{
////////////////////////////////////////////////////////////
std::filesystem::path getResourceDirectory()
{
    auto path = executablePath();
    if (path.empty())
        return {};
    return path.parent_path();
}


////////////////////////////////////////////////////////////
std::filesystem::path getUserDataBaseDirectory()
{
    if (const char* env = std::getenv("XDG_DATA_HOME"); env != nullptr && *env != '\0')
        return env;

    const auto home = getHomeDirectory();
    if (home.empty())
        return {};
    return home / ".local" / "share";
}


////////////////////////////////////////////////////////////
std::optional<std::filesystem::path> getUserFolder(UserFolder folder)
{
    const auto home = getHomeDirectory();

    if (folder == UserFolder::Home)
    {
        if (home.empty())
            return std::nullopt;
        return home;
    }

    if (home.empty())
        return std::nullopt;

    // Map enum -> XDG key + default folder name
    const char* xdgKey      = nullptr;
    const char* defaultName = nullptr;
    switch (folder)
    {
        case UserFolder::Desktop:
            xdgKey      = "XDG_DESKTOP_DIR";
            defaultName = "Desktop";
            break;
        case UserFolder::Documents:
            xdgKey      = "XDG_DOCUMENTS_DIR";
            defaultName = "Documents";
            break;
        case UserFolder::Downloads:
            xdgKey      = "XDG_DOWNLOAD_DIR";
            defaultName = "Downloads";
            break;
        case UserFolder::Pictures:
            xdgKey      = "XDG_PICTURES_DIR";
            defaultName = "Pictures";
            break;
        case UserFolder::Music:
            xdgKey      = "XDG_MUSIC_DIR";
            defaultName = "Music";
            break;
        case UserFolder::Videos:
            xdgKey      = "XDG_VIDEOS_DIR";
            defaultName = "Videos";
            break;
        case UserFolder::Home:
            // Already handled above
            return home;
    }

    if (auto fromXdg = lookupXdgUserDir(xdgKey); !fromXdg.empty())
        return fromXdg;

    return home / defaultName;
}

} // namespace sf::priv::StandardPaths

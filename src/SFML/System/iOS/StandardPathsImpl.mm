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

#import <Foundation/Foundation.h>


namespace
{
////////////////////////////////////////////////////////////
std::filesystem::path firstSearchPath(NSSearchPathDirectory directory)
{
    NSArray<NSString*>* paths = NSSearchPathForDirectoriesInDomains(directory, NSUserDomainMask, YES);
    if (paths.count == 0)
        return {};
    return std::filesystem::path([paths.firstObject UTF8String]);
}
} // namespace


namespace sf::priv::StandardPaths
{
////////////////////////////////////////////////////////////
std::filesystem::path getResourceDirectory()
{
    NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
    if (resourcePath == nil)
        return {};
    return std::filesystem::path([resourcePath UTF8String]);
}


////////////////////////////////////////////////////////////
std::filesystem::path getUserDataBaseDirectory()
{
    return firstSearchPath(NSApplicationSupportDirectory);
}


////////////////////////////////////////////////////////////
std::optional<std::filesystem::path> getUserFolder(UserFolder folder)
{
    // iOS apps are sandboxed: only Home and Documents map to real filesystem paths.
    // The rest (Desktop, Downloads, Pictures, Music, Videos) require dedicated
    // frameworks (PhotoKit, MediaPlayer, ...) and have no path-based access.
    switch (folder)
    {
        case UserFolder::Home:
            return std::filesystem::path([NSHomeDirectory() UTF8String]);
        case UserFolder::Documents:
        {
            auto path = firstSearchPath(NSDocumentDirectory);
            if (path.empty())
                return std::nullopt;
            return path;
        }
        case UserFolder::Desktop:
        case UserFolder::Downloads:
        case UserFolder::Pictures:
        case UserFolder::Music:
        case UserFolder::Videos:
            return std::nullopt;
    }

    return std::nullopt;
}

} // namespace sf::priv::StandardPaths

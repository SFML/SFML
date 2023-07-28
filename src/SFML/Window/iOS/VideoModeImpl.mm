////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/VideoModeImpl.hpp>
#include <SFML/Window/iOS/SFAppDelegate.hpp>

#include <UIKit/UIKit.h>

namespace sf::priv
{
////////////////////////////////////////////////////////////
std::vector<VideoMode> VideoModeImpl::getFullscreenModes()
{
    const VideoMode desktop = getDesktopMode();

    // Return both portrait and landscape resolutions
    return {desktop, VideoMode(Vector2u(desktop.size.y, desktop.size.x), desktop.bitsPerPixel)};
}


////////////////////////////////////////////////////////////
VideoMode VideoModeImpl::getDesktopMode()
{
    const CGRect bounds       = [[UIScreen mainScreen] bounds];
    const double backingScale = [SFAppDelegate getInstance].backingScaleFactor;
    return VideoMode({static_cast<unsigned int>(bounds.size.width * backingScale),
                      static_cast<unsigned int>(bounds.size.height * backingScale)});
}

} // namespace sf::priv

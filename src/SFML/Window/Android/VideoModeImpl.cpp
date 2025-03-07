////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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

#include <SFML/System/Android/Activity.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Vector2.hpp>

#include <mutex>

namespace sf::priv
{
////////////////////////////////////////////////////////////
std::vector<VideoMode> VideoModeImpl::getFullscreenModes()
{
    // Get the activity states
    priv::ActivityStates& states = priv::getActivity();

    const std::lock_guard lock(states.mutex);
    const auto            desktop = VideoMode(Vector2u(states.fullScreenSize));

    // Return both portrait and landscape resolutions
    return {desktop, VideoMode(Vector2u(desktop.size.y, desktop.size.x), desktop.bitsPerPixel)};
}


////////////////////////////////////////////////////////////
VideoMode VideoModeImpl::getDesktopMode()
{
    // Get the activity states
    priv::ActivityStates& states = priv::getActivity();
    const std::lock_guard lock(states.mutex);

    return VideoMode(Vector2u(states.screenSize));
}

} // namespace sf::priv

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2024-2025 Andrew Mickelson
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
#include <SFML/Window/DRM/DRMContext.hpp>
#include <SFML/Window/VideoModeImpl.hpp>

#include <SFML/System/Err.hpp>


namespace sf::priv
{
////////////////////////////////////////////////////////////
std::vector<VideoMode> VideoModeImpl::getFullscreenModes()
{
    std::vector<VideoMode> modes;

    if (const auto* conn = DRMContext::getDRM().savedConnector)
    {
        for (int i = 0; i < conn->count_modes; i++)
            modes.push_back(VideoMode({conn->modes[i].hdisplay, conn->modes[i].vdisplay}));
    }
    else
        modes.push_back(getDesktopMode());

    return modes;
}


////////////////////////////////////////////////////////////
VideoMode VideoModeImpl::getDesktopMode()
{
    const Drm&               drm = DRMContext::getDRM();
    const drmModeModeInfoPtr ptr = drm.mode ? drm.mode : &drm.originalCrtc->mode;
    return VideoMode({ptr->hdisplay, ptr->vdisplay});
}

} // namespace sf::priv

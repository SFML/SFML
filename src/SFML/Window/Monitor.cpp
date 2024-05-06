////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Monitor.hpp>
#include <SFML/Window/MonitorImpl.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <algorithm>
#include <functional>


namespace sf
{


////////////////////////////////////////////////////////////
Monitor::Monitor(std::unique_ptr<priv::MonitorImpl>&& impl) : m_impl(std::move(impl))
{
}

////////////////////////////////////////////////////////////
Monitor Monitor::getPrimaryMonitor()
{
    // Directly forward to the OS-specific implementation
    return priv::MonitorImpl::getPrimaryMonitor();
}


////////////////////////////////////////////////////////////
VideoMode Monitor::getDesktopMode()
{
    // Directly forward to the OS-specific implementation
    return m_impl->getDesktopMode();
}


////////////////////////////////////////////////////////////
bool Monitor::isValid(const VideoMode& mode)
{
    const std::vector<VideoMode>& modes = getFullscreenModes();

    return std::find(modes.begin(), modes.end(), mode) != modes.end();
}


////////////////////////////////////////////////////////////
const std::vector<VideoMode>& Monitor::getFullscreenModes()
{
    static const auto modes = [&]
    {
        std::vector<VideoMode> result = m_impl->getFullscreenModes();
        std::sort(result.begin(), result.end(), std::greater<>());
        return result;
    }();

    return modes;
}

} // namespace sf

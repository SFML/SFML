////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2026 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter this and redistribute it freely,
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
// 3. This notice may not be removed or altered from this source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Monitor.hpp>
#include <SFML/Window/MonitorImpl.hpp>
#include <SFML/Window/VideoMode.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
Monitor::Monitor(std::shared_ptr<priv::MonitorImpl> impl) : m_impl(impl)
{
}


////////////////////////////////////////////////////////////
String Monitor::getName() const
{
    return m_impl ? m_impl->getName() : String();
}


////////////////////////////////////////////////////////////
String Monitor::getIdentifier() const
{
    return m_impl ? m_impl->getIdentifier() : String();
}


////////////////////////////////////////////////////////////
bool Monitor::isPrimary() const
{
    return m_impl && m_impl->isPrimary();
}


////////////////////////////////////////////////////////////
Vector2i Monitor::getPosition() const
{
    return m_impl ? m_impl->getPosition() : Vector2i();
}


////////////////////////////////////////////////////////////
Vector2u Monitor::getResolution() const
{
    return m_impl ? m_impl->getResolution() : Vector2u();
}


////////////////////////////////////////////////////////////
unsigned int Monitor::getRefreshRate() const
{
    return m_impl ? m_impl->getRefreshRate() : 0;
}


////////////////////////////////////////////////////////////
Vector2u Monitor::getScaledResolution() const
{
    return m_impl ? m_impl->getScaledResolution() : Vector2u();
}


////////////////////////////////////////////////////////////
Vector2i Monitor::getWorkAreaPosition() const
{
    return m_impl ? m_impl->getWorkAreaPosition() : Vector2i();
}

////////////////////////////////////////////////////////////
Vector2u Monitor::getWorkAreaSize() const
{
    return m_impl ? m_impl->getWorkAreaSize() : Vector2u();
}


////////////////////////////////////////////////////////////
VideoMode Monitor::getDesktopVideoMode() const
{
    return m_impl ? VideoMode(m_impl->getResolution()) : VideoMode();
}


////////////////////////////////////////////////////////////
std::vector<VideoMode> Monitor::getAvailableVideoModes() const
{
    return m_impl ? m_impl->getAvailableVideoModes() : std::vector<VideoMode>();
}


////////////////////////////////////////////////////////////
Monitor Monitor::getPrimary()
{
    return Monitor(priv::MonitorImpl::getPrimary());
}


////////////////////////////////////////////////////////////
std::vector<Monitor> Monitor::getAvailableMonitors()
{
    std::vector<Monitor> result;
    for (auto&& impl : priv::MonitorImpl::getAvailableMonitors())
    {
        result.emplace_back(std::move(impl));
    }
    return result;
}


////////////////////////////////////////////////////////////
bool operator==(const Monitor& left, const Monitor& right)
{
    return left.getIdentifier() == right.getIdentifier();
}


////////////////////////////////////////////////////////////
bool operator!=(const Monitor& left, const Monitor& right)
{
    return !(left == right);
}

} // namespace sf

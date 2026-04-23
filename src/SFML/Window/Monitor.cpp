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
Monitor::Monitor(const priv::MonitorImpl& impl) :
m_name(impl.name),
m_identifier(impl.identifier),
m_primary(impl.primary),
m_position(impl.position),
m_resolution(impl.resolution),
m_scaledResolution(impl.scaledResolution),
m_workAreaPosition(impl.workAreaPosition),
m_workAreaSize(impl.workAreaSize),
m_refreshRate(impl.refreshRate),
m_nativeHandle(impl.nativeHandle)
{
}


////////////////////////////////////////////////////////////
String Monitor::getName() const
{
    return m_name;
}


////////////////////////////////////////////////////////////
String Monitor::getIdentifier() const
{
    return m_identifier;
}


////////////////////////////////////////////////////////////
bool Monitor::isPrimary() const
{
    return m_primary;
}


////////////////////////////////////////////////////////////
Vector2i Monitor::getPosition() const
{
    return m_position;
}


////////////////////////////////////////////////////////////
Vector2u Monitor::getResolution() const
{
    return m_resolution;
}


////////////////////////////////////////////////////////////
unsigned int Monitor::getRefreshRate() const
{
    return m_refreshRate;
}


////////////////////////////////////////////////////////////
Vector2u Monitor::getScaledResolution() const
{
    return m_scaledResolution;
}


////////////////////////////////////////////////////////////
Vector2i Monitor::getWorkAreaPosition() const
{
    return m_workAreaPosition;
}

////////////////////////////////////////////////////////////
Vector2u Monitor::getWorkAreaSize() const
{
    return m_workAreaSize;
}


////////////////////////////////////////////////////////////
VideoMode Monitor::getDesktopVideoMode() const
{
    return VideoMode(m_resolution);
}


////////////////////////////////////////////////////////////
std::vector<VideoMode> Monitor::getAvailableVideoModes() const
{
    // Video modes aren't part of the snapshot; query them live from the
    // platform using just the identifier that names this monitor.
    priv::MonitorImpl query;
    query.identifier = m_identifier;
    return query.getAvailableVideoModes();
}


////////////////////////////////////////////////////////////
MonitorHandle Monitor::getNativeHandle() const
{
    return m_nativeHandle;
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
    for (const auto& impl : priv::MonitorImpl::getAvailableMonitors())
        result.emplace_back(impl);
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

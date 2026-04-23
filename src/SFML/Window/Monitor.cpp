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

#include <algorithm>


namespace sf
{
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
IntRect Monitor::getWorkArea() const
{
    return m_workArea;
}


////////////////////////////////////////////////////////////
VideoMode Monitor::getDesktopVideoMode() const
{
    return VideoMode(m_resolution);
}


////////////////////////////////////////////////////////////
std::vector<VideoMode> Monitor::getAvailableVideoModes() const
{
    return priv::MonitorImpl::getAvailableVideoModesForMonitor(m_identifier);
}


////////////////////////////////////////////////////////////
Monitor Monitor::getPrimary()
{
    return priv::MonitorImpl::getPrimary();
}


////////////////////////////////////////////////////////////
std::vector<Monitor> Monitor::getAvailableMonitors()
{
    return priv::MonitorImpl::getAvailableMonitors();
}


////////////////////////////////////////////////////////////
bool operator==(const Monitor& left, const Monitor& right)
{
    return (left.getIdentifier() == right.getIdentifier());
}


////////////////////////////////////////////////////////////
bool operator!=(const Monitor& left, const Monitor& right)
{
    return !(left == right);
}

} // namespace sf

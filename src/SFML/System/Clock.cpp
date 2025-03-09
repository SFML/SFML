////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
Time Clock::getElapsedTime() const
{
    if (isRunning())
        return std::chrono::duration_cast<std::chrono::microseconds>(priv::ClockImpl::now() - m_refPoint);
    return std::chrono::duration_cast<std::chrono::microseconds>(m_stopPoint - m_refPoint);
}


////////////////////////////////////////////////////////////
bool Clock::isRunning() const
{
    return m_stopPoint == priv::ClockImpl::time_point();
}


////////////////////////////////////////////////////////////
void Clock::start()
{
    if (!isRunning())
    {
        m_refPoint += priv::ClockImpl::now() - m_stopPoint;
        m_stopPoint = {};
    }
}


////////////////////////////////////////////////////////////
void Clock::stop()
{
    if (isRunning())
        m_stopPoint = priv::ClockImpl::now();
}


////////////////////////////////////////////////////////////
Time Clock::restart()
{
    const Time elapsed = getElapsedTime();
    m_refPoint         = priv::ClockImpl::now();
    m_stopPoint        = {};
    return elapsed;
}


////////////////////////////////////////////////////////////
Time Clock::reset()
{
    const Time elapsed = getElapsedTime();
    m_refPoint         = priv::ClockImpl::now();
    m_stopPoint        = m_refPoint;
    return elapsed;
}

} // namespace sf

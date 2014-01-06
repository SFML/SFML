////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_SENSORIMPL_HPP
#define SFML_SENSORIMPL_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/Window/Sensor.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <queue>
#include <cstddef>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Structure holding a sensor's capabilities
///
////////////////////////////////////////////////////////////
struct SensorCaps
{
    SensorCaps()
    {
        available = false;
        resolution = 0;
        maximumRange = Vector2f(0, 0);
        minimumDelay = Time::Zero;
    }

    bool available;        ///< Is the sensor available on the underlying platform
    float resolution;      ///< How sensible the sensor is in the sensor's unit
    Vector2f maximumRange; ///< Maximum range of the sensor in the sensor's unit
    Time minimumDelay;     ///< Minimum delay allowed between two events
};


////////////////////////////////////////////////////////////
/// \brief Structure holding a sensor's state
///
////////////////////////////////////////////////////////////
struct SensorState
{
    SensorState()
    {
        pendingData = NULL;
        enabled = false;
        refreshRate = Time::Zero;
    }

    std::queue<Sensor::Data>* pendingData; ///< Pending sensor data
    bool enabled;                          ///< Is the sensor currently enabled?
    Time refreshRate;                      ///< Delay between two events
};

} // namespace priv

} // namespace sf


#if defined(SFML_SYSTEM_WINDOWS)
    #include <SFML/Window/Win32/SensorImpl.hpp>
#elif defined(SFML_SYSTEM_LINUX)
    #include <SFML/Window/Unix/SensorImpl.hpp>
#elif defined(SFML_SYSTEM_MACOS)
    #include <SFML/Window/OSX/SensorImpl.hpp>
#elif defined(SFML_SYSTEM_IOS)
    #include <SFML/Window/iOS/SensorImpl.hpp>
#elif defined(SFML_SYSTEM_ANDROID)
    #include <SFML/Window/Android/SensorImpl.hpp>
#endif


#endif // SFML_SENSORIMPL_HPP

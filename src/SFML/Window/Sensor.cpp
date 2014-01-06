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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Sensor.hpp>
#include <SFML/Window/SensorManager.hpp>


namespace sf
{
    
////////////////////////////////////////////////////////////
bool Sensor::isAvailable(Type sensor)
{
    return priv::SensorManager::getInstance().getCapabilities(sensor).available;
}

////////////////////////////////////////////////////////////
float Sensor::getResolution(Type sensor)
{
    return priv::SensorManager::getInstance().getCapabilities(sensor).resolution;
}

////////////////////////////////////////////////////////////
Vector2f Sensor::getMaximumRange(Type sensor)
{
    return priv::SensorManager::getInstance().getCapabilities(sensor).maximumRange;
}

////////////////////////////////////////////////////////////
Time Sensor::getMinimumDelay(Type sensor)
{
    return priv::SensorManager::getInstance().getCapabilities(sensor).minimumDelay;
}

////////////////////////////////////////////////////////////
bool Sensor::isEnable(Type sensor)
{
    return priv::SensorManager::getInstance().isEnable(sensor);
}

////////////////////////////////////////////////////////////
void Sensor::setEnable(Type sensor, bool enable)
{
    return priv::SensorManager::getInstance().setEnable(sensor, enable);
}

////////////////////////////////////////////////////////////
void Sensor::setRefreshRate(Type sensor, const Time& rate)
{
    priv::SensorManager::getInstance().setRefreshRate(sensor, rate);
}

////////////////////////////////////////////////////////////
Sensor::Data Sensor::getData(Type sensor)
{
    return priv::SensorManager::getInstance().getState(sensor).pendingData->back();
}

} // namespace sf

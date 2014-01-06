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
#include <SFML/Window/SensorManager.hpp>
#include <SFML/System/Err.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
SensorManager& SensorManager::getInstance()
{
    static SensorManager instance;
    return instance;
}


////////////////////////////////////////////////////////////
const SensorCaps& SensorManager::getCapabilities(unsigned int sensor) const
{
    return m_sensors[sensor].capabilities;
}


////////////////////////////////////////////////////////////
const SensorState& SensorManager::getState(unsigned int sensor) const
{
    return m_sensors[sensor].state;
}


////////////////////////////////////////////////////////////
void SensorManager::update()
{
    for (int i = 0; i < Sensor::Count; ++i)
    {
        Item& item = m_sensors[i];

        // Skip unavailable sensors
        if (!item.capabilities.available)
            continue;
        
        // Get the current state of the sensor
        item.state = item.sensor.update();
    }
}

////////////////////////////////////////////////////////////
bool SensorManager::isEnable(unsigned int sensor)
{
    if (!m_sensors[sensor].capabilities.available)
    {
        err() << "This sensor isn't available on your system (call Sensor::isAvailable to check it)" << std::endl;
        return false;
    }
    
    return m_sensors[sensor].sensor.isEnable();
}

////////////////////////////////////////////////////////////
void SensorManager::setEnable(unsigned int sensor, bool enable)
{
    if (!m_sensors[sensor].capabilities.available)
    {
        err() << "This sensor isn't available on your system (call Sensor::isAvailable to check it)" << std::endl;
        return;
    }
    
    m_sensors[sensor].sensor.setEnable(enable);
}

////////////////////////////////////////////////////////////
void SensorManager::setRefreshRate(unsigned int sensor, const Time& rate)
{
    if (!m_sensors[sensor].capabilities.available)
    {
        err() << "This sensor isn't available on your system (call Sensor::isAvailable to check it)" << std::endl;
        return;
    }
    
    m_sensors[sensor].sensor.setRefreshRate(rate);
}
    
////////////////////////////////////////////////////////////
SensorManager::SensorManager()
{
    // Global sensor initialization
    SensorImpl::initialize();
    
    // Per sensor initialization
    for (int i = 0; i < Sensor::Count; ++i)
    {
        // Initialize the sensor and get its capabilities
        m_sensors[i].capabilities = m_sensors[i].sensor.initialize(i);

        // Available sensors are disabled by default
        if (m_sensors[i].capabilities.available)
            m_sensors[i].sensor.setEnable(false);
    }
}

////////////////////////////////////////////////////////////
SensorManager::~SensorManager()
{
    // Per sensor cleanup
    for (int i = 0; i < Sensor::Count; ++i)
    {
        if (m_sensors[i].capabilities.available)
        {
            // Disable the sensor
            m_sensors[i].sensor.setEnable(false);
            
            // Terminate the sensor
            m_sensors[i].sensor.terminate();
        }
    }

    // Global sensor cleanup
    SensorImpl::cleanup();
}

} // namespace priv

} // namespace sf

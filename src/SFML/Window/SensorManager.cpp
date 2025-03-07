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
#include <SFML/Window/SensorManager.hpp>

#include <SFML/System/Err.hpp>

#include <ostream>


namespace sf::priv
{
////////////////////////////////////////////////////////////
SensorManager& SensorManager::getInstance()
{
    static SensorManager instance;
    return instance;
}


////////////////////////////////////////////////////////////
bool SensorManager::isAvailable(Sensor::Type sensor)
{
    return m_sensors[sensor].available;
}


////////////////////////////////////////////////////////////
void SensorManager::setEnabled(Sensor::Type sensor, bool enabled)
{
    if (m_sensors[sensor].available)
    {
        m_sensors[sensor].enabled = enabled;
        m_sensors[sensor].sensor.setEnabled(enabled);
    }
    else
    {
        err() << "Warning: trying to enable a sensor that is not available (call Sensor::isAvailable to check it)"
              << std::endl;
    }
}


////////////////////////////////////////////////////////////
bool SensorManager::isEnabled(Sensor::Type sensor) const
{
    return m_sensors[sensor].enabled;
}


////////////////////////////////////////////////////////////
Vector3f SensorManager::getValue(Sensor::Type sensor) const
{
    return m_sensors[sensor].value;
}


////////////////////////////////////////////////////////////
void SensorManager::update()
{
    for (Item& item : m_sensors)
    {
        // Only process available sensors
        if (item.available)
            item.value = item.sensor.update();
    }
}


////////////////////////////////////////////////////////////
SensorManager::SensorManager()
{
    // Global sensor initialization
    SensorImpl::initialize();

    // Per sensor initialization
    for (unsigned int i = 0; i < Sensor::Count; ++i)
    {
        const auto sensor = static_cast<Sensor::Type>(i);

        // Check which sensors are available
        m_sensors[sensor].available = SensorImpl::isAvailable(sensor);

        // Open the available sensors
        if (m_sensors[sensor].available)
        {
            if (m_sensors[sensor].sensor.open(sensor))
            {
                m_sensors[sensor].sensor.setEnabled(false);
            }
            else
            {
                m_sensors[sensor].available = false;
                err() << "Warning: sensor " << i << " failed to open, will not be available" << std::endl;
            }
        }
    }
}

////////////////////////////////////////////////////////////
SensorManager::~SensorManager()
{
    // Per sensor cleanup
    for (Item& item : m_sensors)
    {
        if (item.available)
            item.sensor.close();
    }

    // Global sensor cleanup
    SensorImpl::cleanup();
}

} // namespace sf::priv

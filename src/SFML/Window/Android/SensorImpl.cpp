////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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
#include <SFML/Window/SensorImpl.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Android/Activity.hpp>
#include <android/looper.h>


namespace
{
    ALooper* looper;
    
    ASensorManager*              sensorManager;
    ASensorEventQueue*           sensorEventQueue;
    std::queue<sf::Sensor::Data> sensorData[sf::Sensor::Count];
}
        
namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void SensorImpl::initialize()
{
    // Get the looper associated with this thread
    looper = ALooper_forThread();
    
    // Get the unique sensor manager
    sensorManager = ASensorManager_getInstance();

    // Create the sensor events queue and attach it to the looper
    sensorEventQueue = ASensorManager_createEventQueue(sensorManager, looper,
        1, &processSensorEvents, NULL);
}

////////////////////////////////////////////////////////////
void SensorImpl::cleanup()
{    
    // Detach the sensor events queue from the looper and destroy it
    ASensorManager_destroyEventQueue(sensorManager, sensorEventQueue);
}


////////////////////////////////////////////////////////////
SensorCaps& SensorImpl::initialize(unsigned int type)
{
    // Get the default sensor matching the type
    m_sensor = getDefaultSensor(type);
    
    static SensorCaps capabilities;
    
    if (!m_sensor)
    {
        // Sensor not available, stop here
        capabilities.available = false;
        return capabilities;
    }
    else
        capabilities.available = true;
    
    // Get the sensor resolution
    capabilities.resolution = ASensor_getResolution(m_sensor);
    
    // Get the minimum delay allowed between events
    capabilities.minimumDelay = microseconds(ASensor_getMinDelay(m_sensor));
    
    // To get the maximum range we'll need to use JNI since it's not available from C (todo)
    capabilities.maximumRange = Vector2f();
    
    // Initialize SensorState attributes
    setRefreshRate(capabilities.minimumDelay); // Set the event rate (not to consume too much battery)
    setEnable(false); // Disable the sensor by default (initialize SensorState on the fly)
    m_state.pendingData = &sensorData[type];
    
    return capabilities;
}

////////////////////////////////////////////////////////////
void SensorImpl::terminate()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
SensorState& SensorImpl::update()
{
    // Update our pending sensor data lists
    ALooper_pollAll(0, NULL, NULL, NULL);
    
    return m_state;
}

////////////////////////////////////////////////////////////
bool SensorImpl::isEnable()
{
    return m_state.enabled;
}

////////////////////////////////////////////////////////////
void SensorImpl::setEnable(bool enable)
{
    if (enable)
        ASensorEventQueue_enableSensor(sensorEventQueue, m_sensor);
    else
        ASensorEventQueue_disableSensor(sensorEventQueue, m_sensor);
        
    m_state.enabled = enable;
}

////////////////////////////////////////////////////////////
void SensorImpl::setRefreshRate(const Time& rate)
{
    ASensorEventQueue_setEventRate(sensorEventQueue, m_sensor, rate.asMicroseconds());
    m_state.refreshRate = rate;
}

////////////////////////////////////////////////////////////
ASensor const* SensorImpl::getDefaultSensor(unsigned int type)
{
    int sensorType = 0;
    
    switch(type)
    {
        case Sensor::Accelerometer:
            sensorType = ASENSOR_TYPE_ACCELEROMETER;
            break;
            
        case Sensor::Magnetometer:
            sensorType = ASENSOR_TYPE_MAGNETIC_FIELD;
            break;
            
        case Sensor::Gyroscope:
            sensorType = ASENSOR_TYPE_GYROSCOPE;
            break;
            
        case Sensor::Light:
            sensorType = ASENSOR_TYPE_LIGHT;
            break;
            
        case Sensor::Proximity:
            sensorType = ASENSOR_TYPE_PROXIMITY;
            break;
            
        default:
            // The other sensors are unavailable on Android from the android C API
            return NULL;
    }
    
    return ASensorManager_getDefaultSensor(sensorManager, sensorType);
}


////////////////////////////////////////////////////////////
int SensorImpl::processSensorEvents(int fd, int events, void* data)
{
    ASensorEvent event;

    while (ASensorEventQueue_getEvents(sensorEventQueue, &event, 1) > 0)
    {
        Sensor::Data data;
        Sensor::Type type;
        
        switch (event.sensor)
        {
            case ASENSOR_TYPE_ACCELEROMETER:
                type = Sensor::Accelerometer;
                data.acceleration.x = event.acceleration.x;
                data.acceleration.y = event.acceleration.y;
                data.acceleration.z = event.acceleration.z;
                break;

            case ASENSOR_TYPE_GYROSCOPE:
                type = Sensor::Gyroscope;
                data.vector.x = event.vector.x;
                data.vector.y = event.vector.y;
                data.vector.z = event.vector.z;
                break;

            case ASENSOR_TYPE_MAGNETIC_FIELD:
                type = Sensor::Magnetometer;
                data.magnetic.x = event.magnetic.x;
                data.magnetic.y = event.magnetic.y;
                data.magnetic.z = event.magnetic.z;
                break;
                
            case ASENSOR_TYPE_LIGHT:
                type = Sensor::Light;
                data.light = event.light;
                break;
                
            case ASENSOR_TYPE_PROXIMITY:
                type = Sensor::Proximity;
                break;
        }
                 
        sensorData[type].push(data);
    }

    return 1;
}

} // namespace priv

} // namespace sf

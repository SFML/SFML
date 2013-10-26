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
#include <SFML/Window/JoystickImpl.hpp>
#include <SFML/System/Android/Activity.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Vector3.hpp>
#include <android/looper.h>
#include <android/sensor.h>


namespace
{
    enum
    {
        Accelerometer,
        Gyroscope,
        Magnetometer,
        UserAcceleration,
        AbsoluteOrientation,
        Count
    };

    static const int32_t updateInterval = (1000L/60)*1000;

    ALooper*           looper;
    ASensorManager*    sensorManager;
    ASensorEventQueue* sensorEventQueue;

    struct
    {
        const ASensor* device;
        sf::Vector3f   data;
    } sensors[Count];

    bool areSensorsEnabled;

    int processSensorEvents(int fd, int events, void* data)
    {
        ASensorEvent event;

        while (ASensorEventQueue_getEvents(sensorEventQueue, &event, 1) > 0)
        {
            // Cache the latest data
            switch (event.sensor)
            {
                case ASENSOR_TYPE_ACCELEROMETER:
                    sensors[Accelerometer].data.x = event.acceleration.x;
                    sensors[Accelerometer].data.y = event.acceleration.y;
                    sensors[Accelerometer].data.z = event.acceleration.z;
                    break;

                case ASENSOR_TYPE_GYROSCOPE:
                    sensors[Gyroscope].data.x = event.vector.x;
                    sensors[Gyroscope].data.y = event.vector.y;
                    sensors[Gyroscope].data.z = event.vector.z;
                    break;

                case ASENSOR_TYPE_MAGNETIC_FIELD:
                    sensors[Magnetometer].data.x = event.magnetic.x;
                    sensors[Magnetometer].data.y = event.magnetic.y;
                    sensors[Magnetometer].data.z = event.magnetic.z;
                    break;
             }
        }

        return 1;
    }

    void enableSensors()
    {
        // (Re)start monitoring sensors with 60 events per second
        for (unsigned int i = 0; i < Count; ++i)
        {
            if (sensors[i].device != NULL)
            {
                ASensorEventQueue_enableSensor(sensorEventQueue, sensors[i].device);
                ASensorEventQueue_setEventRate(sensorEventQueue, sensors[i].device, updateInterval);
            }
        }

        areSensorsEnabled = true;
    }

    void disableSensors()
    {
        // Stop monitoring sensors (it avoids consuming battery)
        for (unsigned int i = 0; i < Count; ++i)
        {
            if (sensors[i].device != NULL)
            {
                ASensorEventQueue_disableSensor(sensorEventQueue, sensors[i].device);
            }
        }

        areSensorsEnabled = false;
    }
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void JoystickImpl::initialize()
{
    // Register callbacks to pause sensors when the device is in standby mode
    ActivityStates* states = getActivity(NULL);

    {
        Lock lock(states->mutex);

        states->enableSensors = enableSensors;
        states->disableSensors = disableSensors;
    }

    // Get the unique sensor manager
    sensorManager = ASensorManager_getInstance();

    // Prepare available sensors
    sensors[Accelerometer].device = ASensorManager_getDefaultSensor(sensorManager,
        ASENSOR_TYPE_ACCELEROMETER);
    sensors[Gyroscope].device = ASensorManager_getDefaultSensor(sensorManager,
        ASENSOR_TYPE_GYROSCOPE);
    sensors[Magnetometer].device = ASensorManager_getDefaultSensor(sensorManager,
        ASENSOR_TYPE_MAGNETIC_FIELD);

    // User acceleration and absolute orientation sensors are unavailable on Android
    // (at least, unavailable from the android C API)
    sensors[UserAcceleration].device = NULL;
    sensors[AbsoluteOrientation].device = NULL;

    // Get the looper associated with this thread
    looper = ALooper_forThread();

    // Create the sensor events queue and attach it to the looper
    sensorEventQueue = ASensorManager_createEventQueue(sensorManager, looper,
        1, &processSensorEvents, NULL);

    // Set the event rate (not to consume too much battery)
    ASensorEventQueue_setEventRate(sensorEventQueue, sensors[Accelerometer].device, updateInterval);
    ASensorEventQueue_setEventRate(sensorEventQueue, sensors[Gyroscope].device, updateInterval);
    ASensorEventQueue_setEventRate(sensorEventQueue, sensors[Magnetometer].device, updateInterval);

    // Start monitoring sensors
    enableSensors();
}



////////////////////////////////////////////////////////////
void JoystickImpl::cleanup()
{
    // Stop monitoring sensors
    disableSensors();

    // Detach the sensor events queue from the looper and destroy it
    ASensorManager_destroyEventQueue(sensorManager, sensorEventQueue);
}


////////////////////////////////////////////////////////////
bool JoystickImpl::isConnected(unsigned int index)
{
    // If sensors aren't paused and the requested sensor available
    return (areSensorsEnabled && sensors[index].device);
}


////////////////////////////////////////////////////////////
bool JoystickImpl::open(unsigned int index)
{
    // Save the index if sensor is available
    if(sensors[index].device)
    {
        m_index = index;

        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
void JoystickImpl::close()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilities() const
{
    JoystickCaps caps;

    // All the connected joysticks have (X, Y, Z) axes
    caps.axes[Joystick::X] = true;
    caps.axes[Joystick::Y] = true;
    caps.axes[Joystick::Z] = true;

    return caps;
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::update()
{
    JoystickState state;

    // Poll latest sensor events
    ALooper_pollAll(0, NULL, NULL, NULL);

    // Always connected
    state.connected = sensors[m_index].device ? true : false;

    // Use our cache
    state.axes[Joystick::X] = sensors[m_index].data.x;
    state.axes[Joystick::Y] = sensors[m_index].data.y;
    state.axes[Joystick::Z] = sensors[m_index].data.z;

    return state;
}

} // namespace priv

} // namespace sf

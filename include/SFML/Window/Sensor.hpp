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

#ifndef SFML_SENSOR_HPP
#define SFML_SENSOR_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Export.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/System/Time.hpp>


namespace sf
{
class Window;

////////////////////////////////////////////////////////////
/// \brief Give access to the real-time state of the sensor
///
////////////////////////////////////////////////////////////
class SFML_WINDOW_API Sensor
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Sensor type
    ///
    ////////////////////////////////////////////////////////////
    enum Type
    {
        Accelerometer,      ///< The acceleration sensor measures the acceleration force
        Gyroscope,          ///< The gyroscope sensor measures the rate of rotation
        Magnetometer,       ///< The magnetic sensor measures the ambient magnetic field
        Temperature,        ///< The temperature sensor measures the ambient room temperature
        Proximity,          ///< The proximity sensor measures how close an object is
        Gravity,            ///< The gravity sensor measures the force of gravity
        Light,              ///< The light sensor measures the amount of ambient light or illumination
        LinearAcceleration, ///< The linear acceleration sensor that measures acceleration
        Orientation,        ///< The orientation sensor that measures the degrees of orientation
        Pressure,           ///< The pressure sensor give air pressure readings
        Altimeter,          ///< The altimeter measures the altitude by using air pressure measurements
        Compass,            ///< The compass sensor give compass heading readings
        
        Count               ///< Keep last -- the total number of sensor type
    };
    
    ////////////////////////////////////////////////////////////
    /// \brief Sensor data
    ///
    ////////////////////////////////////////////////////////////
    union Data
    {
        struct
        {
            float x; ///< X-axis data
            float y; ///< Y-axis data
            float z; ///< Z-axis data
        }
        acceleration,      ///< Acceleration in m/s^2 (Sensor::Accelerometer, Sensor::Gravity, Sensor::LinearAcceleration)
        magnetic,          ///< Magnetic in uT (Sensor::Magnetometer)
        vector,            ///< Vector representing angle in degrees (Sensor::Orientation, Sensor::Compass)
        gyroscope;         ///< Gyroscope data in rad/s (Sensor::Gyroscope)
        float temperature; ///< Temperature in degrees Celsius (Sensor::Temperature)
        float distance;    ///< Distance in meter (Sensor::Proximity, Sensor::Altimeter)
        float light;       ///< Illumination in lx (Sensor::Light)
        float pressure;    ///< Pressure in hPa (Sensor::Pressure)
    };
    
    ////////////////////////////////////////////////////////////
    /// \brief Check if a sensor is available on the underlying platform
    ///
    /// \param sensor Sensor to check
    ///
    /// \return True if the sensor is available, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isAvailable(Type sensor);
    
    ////////////////////////////////////////////////////////////
    /// \brief Get the resolution of a sensor
    ///
    /// The resolution represents the sensitivity of a sensor. The 
    /// resolution of a sensor is the smallest change it can detect in 
    /// the quantity that it is measuring
    /// 
    /// \param sensor Sensor to check
    ///
    /// \return The sensor's resolution in the sensor's unit.
    ///
    ////////////////////////////////////////////////////////////
    static float getResolution(Type sensor);
    
    ////////////////////////////////////////////////////////////
    /// \brief Get the maximum range of a sensor
    ///
    /// The resolution represents the sensitivity of a sensor. The 
    /// value returned is in the sensor's unit.
    /// 
    /// \param sensor Sensor to check
    ///
    /// \return The sensor's resolution in the sensor's unit.
    ///
    ////////////////////////////////////////////////////////////
    static Vector2f getMaximumRange(Type sensor);
        
    ////////////////////////////////////////////////////////////
    /// \brief Get the minimum delay allowed between two events
    ///
    /// When setting the refresh rate of a sensor, you'll need to 
    /// make sure the value is a superior to the minimum delay 
    /// allowable
    ///
    /// \param sensor Sensor to check
    ///
    /// \return The minimum delay allowed between two events
    ///
    ////////////////////////////////////////////////////////////
    Time getMinimumDelay(Type sensor);
    
    ////////////////////////////////////////////////////////////
    /// \brief Check if a sensor is enabled
    ///
    /// \param sensor Sensor to check
    ///
    /// \return True if the sensor is enabled, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isEnable(Type sensor);
    
    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable a sensor
    ///
    /// A sensor is disabled by default otherwise it would consume too 
    /// much battery power. Once a sensor enabled, it will start sending 
    /// sensor events of that type (if available).
    ///
    /// \param sensor Sensor to enable
    /// \param enable True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    static void setEnable(Type sensor, bool enable = true);
   
    ////////////////////////////////////////////////////////////
    /// \brief Set the refresh rate of a sensor
    ///
    /// This is the delay you want between each measure report
    ///
    /// \param sensor Sensor to modify
    /// \param rate Delay between each event
    ///
    ////////////////////////////////////////////////////////////
    static void setRefreshRate(Type sensor, const Time& rate);
   
    ////////////////////////////////////////////////////////////
    /// \brief Get the current sensor data
    ///
    /// If the sensor support real-time retrieval, you'll get 
    /// the current sensor data otherwise this is likely the
    /// latest data of this sensor type.
    ///
    /// \param sensor Sensor to check
    ///
    /// \return The current sensor data
    ///
    ////////////////////////////////////////////////////////////
    static Data getData(Type sensor);
};

} // namespace sf


#endif // SFML_SENSOR_HPP

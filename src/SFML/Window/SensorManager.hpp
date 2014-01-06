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

#ifndef SFML_SENSORMANAGER_HPP
#define SFML_SENSORMANAGER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Sensor.hpp>
#include <SFML/Window/SensorImpl.hpp>
#include <SFML/System/NonCopyable.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Global sensor manager
///
////////////////////////////////////////////////////////////
class SensorManager : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Get the global unique instance of the manager
    ///
    /// \return Unique instance of the sensor manager
    ///
    ////////////////////////////////////////////////////////////
    static SensorManager& getInstance();

    ////////////////////////////////////////////////////////////
    /// \brief Get the capabilities of a sensor
    ///
    /// \param sensor Index of the sensor
    ///
    /// \return Capabilities of the sensor
    ///
    ////////////////////////////////////////////////////////////
    const SensorCaps& getCapabilities(unsigned int sensor) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current state of a sensor
    ///
    /// \param sensor Index of the sensor
    ///
    /// \return Current state of the sensor
    ///
    ////////////////////////////////////////////////////////////
    const SensorState& getState(unsigned int sensor) const;

    ////////////////////////////////////////////////////////////
    /// \brief Update the state of all the sensors
    ///
    ////////////////////////////////////////////////////////////
    void update();

    ////////////////////////////////////////////////////////////
    /// \brief Get the current status of a sensor
    ///
    /// \param sensor Index of the sensor
    ///
    /// \return True if the sensor is enabled, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool isEnable(unsigned int sensor);
    
    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable a sensor
    ///
    /// \param sensor Index of the sensor
    /// \param enable Whether it should be enabled or not
    ///
    ////////////////////////////////////////////////////////////
    void setEnable(unsigned int sensor, bool enable);
    
    ////////////////////////////////////////////////////////////
    /// \brief Set the refresh rate of a sensor
    ///
    /// \param sensor Index of the sensor
    /// \param rate Delay between each event
    ///
    ////////////////////////////////////////////////////////////
    void setRefreshRate(unsigned int sensor, const Time& rate);
    
private:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    SensorManager();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~SensorManager();

    ////////////////////////////////////////////////////////////
    /// \brief Sensor information and state
    ///
    ////////////////////////////////////////////////////////////
    struct Item
    {
        SensorImpl  sensor;       ///< Sensor implementation
        SensorState state;        ///< The current sensor state
        SensorCaps  capabilities; ///< The sensor capabilities
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Item m_sensors[Sensor::Count]; ///< Sensors information and state
};

} // namespace priv

} // namespace sf


#endif // SFML_SENSORMANAGER_HPP

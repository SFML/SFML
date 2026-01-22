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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Vector3.hpp>

#include <android/sensor.h>


namespace sf::priv
{
////////////////////////////////////////////////////////////
class SensorImpl
{
public:
    ////////////////////////////////////////////////////////////
    static void initialize();

    ////////////////////////////////////////////////////////////
    static void cleanup();

    ////////////////////////////////////////////////////////////
    static bool isAvailable(Sensor::Type sensor);

    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool open(Sensor::Type sensor);

    ////////////////////////////////////////////////////////////
    void close();

    ////////////////////////////////////////////////////////////
    [[nodiscard]] Vector3f update() const;

    ////////////////////////////////////////////////////////////
    void setEnabled(bool enabled);

private:
    ////////////////////////////////////////////////////////////
    /// \brief Get the default Android sensor matching the sensor type
    ///
    /// \param type Type of the sensor
    ///
    /// \return The default Android sensor, `nullptr` otherwise
    ///
    ////////////////////////////////////////////////////////////
    static const ASensor* getDefaultSensor(Sensor::Type sensor);

    ////////////////////////////////////////////////////////////
    /// \brief Process the pending sensor data available and add them to our lists
    ///
    /// \param fd     File descriptor
    /// \param events Bitmask of the poll events that were triggered
    /// \param data   Data pointer supplied
    ///
    /// \return Whether it should continue (1) or unregister the callback (0)
    ///
    ////////////////////////////////////////////////////////////
    static int processSensorEvents(int fd, int events, void* data);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    const ASensor* m_sensor; ///< Android sensor structure
    Sensor::Type   m_type;   ///< Type of the sensor
};

} // namespace sf::priv

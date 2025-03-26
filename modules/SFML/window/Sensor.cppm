/**
 * @file Sensor.cppm
 * @brief Module file for the Sensor namespace of the Window module
 */

module;

#include <SFML/Window/Sensor.hpp>

export module sfml.window.Sensor;

/**
 * @namespace sf
 * @brief The SFML sf:: namespace 
 */
export namespace sf {
    /**
     * @namespace Sensor
     * @brief The SFML sf::Sensor:: namespace
     */
    namespace Sensor {
        using sf::Sensor::Type;

        using sf::Sensor::isAvailable;
        using sf::Sensor::setEnabled;
        using sf::Sensor::getValue;
    }
}
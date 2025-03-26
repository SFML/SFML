/**
 * @file Joystick.cppm
 * @brief Module file for the Joystick namespace of the Window module
 */

module;

#include <SFML/Window/Joystick.hpp>

export module sfml.window.Joystick;

/**
 * @namespace sf
 * @brief The SFML sf:: namespace 
 */
export namespace sf {
    /**
     * @namespace Joystick
     * @brief The SFML sf::Joystick:: namespace
     */
    namespace Joystick {
        using sf::Joystick::Identification;

        using sf::Joystick::Axis;

        using sf::Joystick::isConnected;
        using sf::Joystick::getButtonCount;
        using sf::Joystick::hasAxis;
        using sf::Joystick::isButtonPressed;
        using sf::Joystick::getAxisPosition;
        using sf::Joystick::getIdentification;
        using sf::Joystick::update;
    }
}
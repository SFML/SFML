/**
 * @file Mouse.cppm
 * @brief Module file for the Mouse namespace of the Window module
 */

module;

#include <SFML/Window/Mouse.hpp>

export module sfml.window.Mouse;

/**
 * @namespace sf
 * @brief The SFML sf:: namespace 
 */
export namespace sf {
    /**
     * @namespace Mouse
     * @brief The SFML sf::Mouse:: namespace
     */
    namespace Mouse {
        using sf::Mouse::Button;
        using sf::Mouse::Wheel;

        using sf::Mouse::isButtonPressed;
        using sf::Mouse::getPosition;
        using sf::Mouse::setPosition;
    }
}
/**
 * @file Touch.cppm
 * @brief Module file for the Touch namespace of the Window module
 */

module;

#include <SFML/Window/Touch.hpp>

export module sfml.window.Touch;

/**
 * @namespace sf
 * @brief The SFML sf:: namespace 
 */
export namespace sf {
    /**
     * @namespace Touch
     * @brief The SFML sf::Touch:: namespace
     */
    namespace Touch {
        using sf::Touch::isDown;
        using sf::Touch::getPosition;
    }
}
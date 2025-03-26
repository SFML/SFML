/**
 * @file Keyboard.cppm
 * @brief Module file for the Keyboard namespace of the Window module
 */

module;

#include <SFML/Window/Keyboard.hpp>

export module sfml.window.Keyboard;

/**
 * @namespace sf
 * @brief The SFML sf:: namespace 
 */
export namespace sf {
    /**
     * @namespace Keyboard
     * @brief The SFML sf::Keyboard:: namespace
     */
    namespace Keyboard {
        using sf::Keyboard::Scancode;

        using sf::Keyboard::Key;
        using sf::Keyboard::Scan;

        using sf::Keyboard::isKeyPressed;
        using sf::Keyboard::localize;
        using sf::Keyboard::delocalize;
        using sf::Keyboard::getDescription;
        using sf::Keyboard::setVirtualKeyboardVisible;
    }
}
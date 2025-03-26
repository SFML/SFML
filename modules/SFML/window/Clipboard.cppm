/**
 * @file Clipboard.cppm
 * @brief Module file for the Clipboard namespace of the Window module
 */

module;

#include <SFML/Window/Clipboard.hpp>

export module sfml.window.Clipboard;

/**
 * @namespace sf
 * @brief The SFML sf:: namespace 
 */
export namespace sf {
    /**
     * @namespace Clipboard
     * @brief The SFML sf::Clipboard:: namespace
     */
    namespace Clipboard {
        using sf::Clipboard::getString;
        using sf::Clipboard::setString;
    }
}
/**
 * @file WindowEnums.cppm
 * @brief Module file for the Style namespace of the Window module
 */

module;

#include <SFML/Window/WindowEnums.hpp>

export module sfml.window.WindowEnums;

/**
 * @namespace sf
 * @brief The SFML sf:: namespace 
 */
export namespace sf {
    /**
     * @namespace Style
     * @brief The SFML sf::Style:: namespace
     */
    namespace Style {
        using sf::Style::None;
        using sf::Style::Titlebar;
        using sf::Style::Resize;
        using sf::Style::Close;
    }

    using sf::State;
}
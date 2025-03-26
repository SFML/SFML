/**
 * @file StencilMode.cppm
 * @brief Module file for the StencilMode and StencilValue classes of the Graphics module
 */

module;

#include <SFML/Graphics/StencilMode.hpp>

export module sfml.graphics.StencilMode;

/**
 * @namespace sf
 * @brief The SFML sf:: namespace 
 */
export namespace sf {
    using sf::StencilMode;
    using sf::StencilValue;

    using sf::StencilComparison;
    using sf::StencilUpdateOperation;
}
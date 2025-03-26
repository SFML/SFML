/**
 * @file String.cppm
 * @brief Module file for the String class of the System module
 */

module;

#include <SFML/System/String.hpp>

export module sfml.system.String;

/**
 * @namespace sf
 * @brief The SFML sf:: namespace 
 */
export namespace sf {
    using sf::U8StringCharTraits;
    using sf::String;

    using sf::U8String;
}
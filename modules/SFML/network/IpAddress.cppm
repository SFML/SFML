/**
 * @file IpAddress.cppm
 * @brief Module file for the IpAddress class of the Network module
 */

module;

#include <SFML/Network/IpAddress.hpp>

export module sfml.network.IpAddress;

/**
 * @namespace sf
 * @brief The SFML sf:: namespace 
 */
export namespace sf {
    using sf::IpAddress;

    using sf::operator==;
    using sf::operator!=;
    using sf::operator<;
    using sf::operator>;
    using sf::operator<=;
    using sf::operator>=;
    using sf::operator>>;
    using sf::operator<<;
}
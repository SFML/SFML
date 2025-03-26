/**
 * @file Listener.cppm
 * @brief Module file for the Listener namespace of the Audio module
 */

module;

#include <SFML/Audio/Listener.hpp>

export module sfml.audio.Listener;

/**
 * @namespace sf
 * @brief The SFML sf:: namespace 
 */
export namespace sf {
    /**
     * @namespace Listener
     * @brief The SFML sf::Listener:: namespace
     */
    namespace Listener {
        using sf::Listener::Cone;

        using sf::Listener::setGlobalVolume;
        using sf::Listener::getGlobalVolume;
        using sf::Listener::setPosition;
        using sf::Listener::getPosition;
        using sf::Listener::setDirection;
        using sf::Listener::getDirection;
        using sf::Listener::setVelocity;
        using sf::Listener::getVelocity;
        using sf::Listener::setCone;
        using sf::Listener::getCone;
        using sf::Listener::setUpVector;
        using sf::Listener::getUpVector;
    }
}
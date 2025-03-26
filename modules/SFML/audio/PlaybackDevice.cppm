/**
 * @file PlaybackDevice.cppm
 * @brief Module file for the PlaybackDevice namespace of the Audio module
 */

module;

#include <SFML/Audio/PlaybackDevice.hpp>

export module sfml.audio.PlaybackDevice;

/**
 * @namespace sf
 * @brief The SFML sf:: namespace
 */
export namespace sf {
    /**
     * @namespace PlaybackDevice
     * @brief The SFML sf::PlaybackDevice:: namespace
     */
    namespace PlaybackDevice {
        using sf::PlaybackDevice::getAvailableDevices;
        using sf::PlaybackDevice::getDefaultDevice;
        using sf::PlaybackDevice::setDevice;
        using sf::PlaybackDevice::getDevice;
    }
}
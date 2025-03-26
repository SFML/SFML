/**
 * @file Config.cppm
 * @brief Module file for SFML macro constants.
 */

module;

#include <SFML/Config.hpp>

export module sfml.Config;

/**
 * @brief The SFML sf:: namespace 
 */
export namespace sf {
    constexpr inline int VERSION_MAJOR = SFML_VERSION_MAJOR;
    constexpr inline int VERSION_MINOR = SFML_VERSION_MINOR;
    constexpr inline int VERSION_PATCH = SFML_VERSION_PATCH;
    constexpr inline bool VERSION_IS_RELEASE = SFML_VERSION_IS_RELEASE;
}
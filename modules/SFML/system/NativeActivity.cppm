/**
 * @file NativeActivity.cppm
 * @brief Module file for the getNativeActivity function of the System module
 */

module;

#ifdef SFML_SYSTEM_ANDROID
#include <SFML/System/NativeActivity.hpp>
#endif

export module sfml.system.NativeActivity;

/**
 * @namespace sf
 * @brief The SFML sf:: namespace 
 */
export namespace sf {
    #ifdef SFML_SYSTEM_ANDROID
    using ::ANativeActivity;

    using sf::getNativeActivity;
    #endif
}
/**
 * @file GlResource.cppm
 * @brief Module file for the GlResource class of the Window module
 */

module;

#include <SFML/Window/GlResource.hpp>

export module sfml.window.GlResource;

/**
 * @namespace sf
 * @brief The SFML sf:: namespace 
 */
export namespace sf {
    using sf::GlResource;

    using ContextDestroyCallback = void (*)(void*);
}
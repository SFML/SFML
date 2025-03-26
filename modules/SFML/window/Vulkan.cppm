/**
 * @file Vulkan.cppm
 * @brief Module file for the Vulkan namespace of the Window module
 */

module;

#include <SFML/Window/Vulkan.hpp>

export module sfml.window.Vulkan;

/**
 * @namespace sf
 * @brief The SFML sf:: namespace 
 */
export namespace sf {
    /**
     * @namespace Vulkan
     * @brief The SFML sf::Vulkan:: namespace
     */
    namespace Vulkan {
        using sf::Vulkan::isAvailable;
        using sf::Vulkan::getFunction;
        using sf::Vulkan::getGraphicsRequiredInstanceExtensions;
    }

    using ::VkInstance;
    using ::VkSurfaceKHR;
    using sf::VulkanFunctionPointer;
}
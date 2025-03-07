#include <SFML/Window/Vulkan.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("[Window] sf::Vulkan")
{
    SECTION("getFunction()")
    {
        CHECK(sf::Vulkan::getFunction("") == nullptr);
        CHECK(sf::Vulkan::getFunction(" ") == nullptr);
        CHECK(sf::Vulkan::getFunction("a string that will never resolve to a Vulkan function") == nullptr);

        CHECKED_IF(sf::Vulkan::isAvailable())
        {
            CHECK(sf::Vulkan::getFunction("vkCreateInstance") != nullptr);
        }
    }

    SECTION("getGraphicsRequiredInstanceExtensions()")
    {
        // If Vulkan is not available this function may or may not return a non-empty vector
        // If Vulkan is available then it will always return a non-empty vector
        CHECKED_IF(sf::Vulkan::isAvailable())
        {
            CHECK(!sf::Vulkan::getGraphicsRequiredInstanceExtensions().empty());
        }
    }
}

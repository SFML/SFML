#include <SFML/Window/ContextSettings.hpp>

#include <doctest/doctest.h>

#include <type_traits>

static_assert(std::is_copy_constructible_v<sf::ContextSettings>);
static_assert(std::is_copy_assignable_v<sf::ContextSettings>);
static_assert(std::is_nothrow_move_constructible_v<sf::ContextSettings>);
static_assert(std::is_nothrow_move_assignable_v<sf::ContextSettings>);

TEST_CASE("[Window] sf::ContextSettings")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            const sf::ContextSettings contextSettings;
            CHECK(contextSettings.depthBits == 0);
            CHECK(contextSettings.stencilBits == 0);
            CHECK(contextSettings.antialiasingLevel == 0);
            CHECK(contextSettings.majorVersion == 1);
            CHECK(contextSettings.minorVersion == 1);
            CHECK(contextSettings.attributeFlags == sf::ContextSettings::Default);
            CHECK(contextSettings.sRgbCapable == false);
        }

        SUBCASE("Verbose constructor")
        {
            const sf::ContextSettings contextSettings(1, 1, 2, 3, 5, sf::ContextSettings::Core, true);
            CHECK(contextSettings.depthBits == 1);
            CHECK(contextSettings.stencilBits == 1);
            CHECK(contextSettings.antialiasingLevel == 2);
            CHECK(contextSettings.majorVersion == 3);
            CHECK(contextSettings.minorVersion == 5);
            CHECK(contextSettings.attributeFlags == sf::ContextSettings::Core);
            CHECK(contextSettings.sRgbCapable == true);
        }
    }
}

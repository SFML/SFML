#include <SFML/Window/ContextSettings.hpp>

#include <doctest.h>

TEST_CASE("sf::ContextSettings class - [window]")
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

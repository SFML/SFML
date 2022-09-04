#include <SFML/Config.hpp>

#include <doctest/doctest.h>

TEST_CASE("SFML/Config.hpp")
{
    SUBCASE("Version macros")
    {
        CHECK(SFML_VERSION_MAJOR == EXPECTED_SFML_VERSION_MAJOR);
        CHECK(SFML_VERSION_MINOR == EXPECTED_SFML_VERSION_MINOR);
        CHECK(SFML_VERSION_PATCH == EXPECTED_SFML_VERSION_PATCH);
        CHECK(SFML_VERSION_IS_RELEASE == EXPECTED_SFML_VERSION_IS_RELEASE);
    }

    SUBCASE("Fixed width types")
    {
        CHECK(sizeof(sf::Int32) == 4);
        CHECK(sizeof(sf::Uint32) == 4);

        CHECK(sizeof(sf::Int64) == 8);
        CHECK(sizeof(sf::Uint64) == 8);
    }
}

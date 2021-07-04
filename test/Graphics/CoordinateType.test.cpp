#include <SFML/Graphics/CoordinateType.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

TEST_CASE("[Graphics] sf::CoordinateType")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::CoordinateType>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::CoordinateType>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::CoordinateType>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::CoordinateType>);
    }
}

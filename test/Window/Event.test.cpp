#include <SFML/Window/Event.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

TEST_CASE("[Window] sf::Event")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::Event>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Event>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Event>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Event>);
    }

    SECTION("Construction")
    {
        const sf::Event event{};
        CHECK(event.type == sf::Event::Closed);
    }
}

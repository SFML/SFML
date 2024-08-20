#include <SFML/System/Exception.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("[System] sf::Exception")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::Exception>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Exception>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Exception>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Exception>);
        STATIC_CHECK(std::is_base_of_v<std::runtime_error, sf::Exception>);
    }

    SECTION("Construction")
    {
        const sf::Exception exception("Oops");
        CHECK(exception.what() == std::string("Oops"));
    }
}

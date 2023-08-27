#include <SFML/Network/Http.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

TEST_CASE("[Network] sf::Http")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_copy_constructible_v<sf::Http>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::Http>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<sf::Http>);
        STATIC_CHECK(!std::is_nothrow_move_assignable_v<sf::Http>);
    }

    SECTION("Request")
    {
        SECTION("Type traits")
        {
            STATIC_CHECK(std::is_copy_constructible_v<sf::Http::Request>);
            STATIC_CHECK(std::is_copy_assignable_v<sf::Http::Request>);
            STATIC_CHECK(std::is_move_constructible_v<sf::Http::Request>);
            STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Http::Request>);
        }
    }

    SECTION("Response")
    {
        SECTION("Type traits")
        {
            STATIC_CHECK(std::is_copy_constructible_v<sf::Http::Response>);
            STATIC_CHECK(std::is_copy_assignable_v<sf::Http::Response>);
            STATIC_CHECK(std::is_move_constructible_v<sf::Http::Response>);
            STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Http::Response>);
        }

        SECTION("Construction")
        {
            const sf::Http::Response response;
            CHECK(response.getField("").empty());
            CHECK(response.getStatus() == sf::Http::Response::Status::ConnectionFailed);
            CHECK(response.getMajorHttpVersion() == 0);
            CHECK(response.getMinorHttpVersion() == 0);
            CHECK(response.getBody().empty());
        }
    }
}

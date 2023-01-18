#include <SFML/Window/Window.hpp>

// Other 1st party headers
#include <SFML/Window/VideoMode.hpp>

#include <SFML/System/String.hpp>

#include <catch2/catch_test_macros.hpp>

#include <WindowUtil.hpp>
#include <type_traits>

TEST_CASE("[Window] sf::Window", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_copy_constructible_v<sf::Window>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::Window>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<sf::Window>);
        STATIC_CHECK(!std::is_nothrow_move_assignable_v<sf::Window>);
    }

    SECTION("Construction")
    {
        const sf::Window window(sf::VideoMode(sf::Vector2u(256, 256), 32),
                                "Window Title",
                                sf::Style::Default,
                                sf::ContextSettings());
        CHECK(window.getSize() == sf::Vector2u(256, 256));
    }
}

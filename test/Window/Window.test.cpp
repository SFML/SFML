#include <SFML/Window/Window.hpp>

// Other 1st party headers
#include <SFML/Window/VideoMode.hpp>

#include <SFML/System/String.hpp>

#include <doctest/doctest.h>

#include <WindowUtil.hpp>
#include <type_traits>

static_assert(!std::is_copy_constructible_v<sf::Window>);
static_assert(!std::is_copy_assignable_v<sf::Window>);
static_assert(!std::is_nothrow_move_constructible_v<sf::Window>);
static_assert(!std::is_nothrow_move_assignable_v<sf::Window>);

TEST_CASE("[Window] sf::Window" * doctest::skip(skipDisplayTests))
{
    SUBCASE("Construction")
    {
        sf::Window window(sf::VideoMode(sf::Vector2u(256, 256), 32), "Window Title", sf::Style::Default, sf::ContextSettings());
        CHECK(window.getSize() == sf::Vector2u(256, 256));
    }
}

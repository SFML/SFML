#include <SFML/Graphics/RenderWindow.hpp>

// Other 1st party headers
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <SFML/Window/VideoMode.hpp>

#include <SFML/System/String.hpp>

#include <doctest/doctest.h>

#include <GraphicsUtil.hpp>
#include <type_traits>

static_assert(!std::is_copy_constructible_v<sf::RenderWindow>);
static_assert(!std::is_copy_assignable_v<sf::RenderWindow>);
static_assert(!std::is_nothrow_move_constructible_v<sf::RenderWindow>);
static_assert(!std::is_nothrow_move_assignable_v<sf::RenderWindow>);

TEST_CASE("[Graphics] sf::RenderWindow" * doctest::skip(skipDisplayTests))
{
    SUBCASE("Construction")
    {
        const sf::RenderWindow window(sf::VideoMode(sf::Vector2u(256, 256), 24),
                                      "Window Title",
                                      sf::Style::Default,
                                      sf::ContextSettings());
        CHECK(window.getSize() == sf::Vector2u(256, 256));
    }

    SUBCASE("Clear")
    {
        sf::RenderWindow window(sf::VideoMode(sf::Vector2u(256, 256), 24),
                                "Window Title",
                                sf::Style::Default,
                                sf::ContextSettings());
        REQUIRE(window.getSize() == sf::Vector2u(256, 256));

        sf::Texture texture;
        REQUIRE(texture.create(window.getSize()));

        window.clear(sf::Color::Red);
        texture.update(window);
        CHECK(texture.copyToImage().getPixel(sf::Vector2u(64, 64)) == sf::Color::Red);

        window.clear(sf::Color::Green);
        texture.update(window);
        CHECK(texture.copyToImage().getPixel(sf::Vector2u(128, 128)) == sf::Color::Green);

        window.clear(sf::Color::Blue);
        texture.update(window);
        CHECK(texture.copyToImage().getPixel(sf::Vector2u(196, 196)) == sf::Color::Blue);
    }
}

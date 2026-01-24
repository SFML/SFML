#include <SFML/Window/Mouse.hpp>

#include <catch2/catch_test_macros.hpp>

#include <WindowUtil.hpp>

TEST_CASE("[Window] sf::Mouse", runDisplayTests())
{
    SECTION("isButtonPressed()")
    {
        CHECK(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
        CHECK(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Right));
        CHECK(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle));
        CHECK(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Extra1));
        CHECK(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Extra2));
    }

    SECTION("setPosition() and getPosition()")
    {
        sf::Vector2i pos{12, 42};
        sf::Mouse::setPosition(pos);
        CHECK(sf::Mouse::getPosition() == pos);
    }
}

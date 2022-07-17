#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include <doctest/doctest.h>

#include <SystemUtil.hpp>
#include <thread>

TEST_CASE("sf::Clock class - [system]")
{
    SUBCASE("getElapsedTime()")
    {
        const sf::Clock clock;
        CHECK(clock.getElapsedTime() >= sf::microseconds(0));
        const auto elapsed = clock.getElapsedTime();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        CHECK(clock.getElapsedTime() > elapsed);
    }

    SUBCASE("restart()")
    {
        sf::Clock clock;
        CHECK(clock.restart() >= sf::microseconds(0));
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        const auto elapsed = clock.restart();
        CHECK(clock.restart() < elapsed);
    }
}

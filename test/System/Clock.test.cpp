#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include <catch2/catch_test_macros.hpp>

#include <SystemUtil.hpp>
#include <thread>
#include <type_traits>

TEST_CASE("[System] sf::Clock")
{
    using namespace std::chrono_literals;

    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::Clock>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Clock>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Clock>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Clock>);
    }

    SECTION("Construction")
    {
        const sf::Clock clock;
        CHECK(clock.isRunning());
        CHECK(clock.getElapsedTime() >= sf::microseconds(0));
    }

    SECTION("getElapsedTime()")
    {
        const sf::Clock clock;
        CHECK(clock.getElapsedTime() >= sf::microseconds(0));
        const auto elapsed = clock.getElapsedTime();
        std::this_thread::sleep_for(1ms);
        CHECK(clock.getElapsedTime() > elapsed);
    }

    SECTION("start/stop")
    {
        sf::Clock clock;
        clock.stop();
        CHECK(!clock.isRunning());
        const auto elapsed = clock.getElapsedTime();
        std::this_thread::sleep_for(1ms);
        CHECK(elapsed == clock.getElapsedTime());

        clock.start();
        CHECK(clock.isRunning());
        CHECK(clock.getElapsedTime() >= elapsed);
    }

    SECTION("restart()")
    {
        sf::Clock clock;
        CHECK(clock.restart() >= sf::microseconds(0));
        CHECK(clock.isRunning());
        std::this_thread::sleep_for(1ms);
        const auto elapsed = clock.restart();
        CHECK(clock.restart() < elapsed);
    }

    SECTION("reset()")
    {
        sf::Clock clock;
        CHECK(clock.reset() >= sf::microseconds(0));
        CHECK(!clock.isRunning());
    }
}

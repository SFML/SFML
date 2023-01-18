#include <SFML/System/Sleep.hpp>

// Other 1st party headers
#include <SFML/System/Time.hpp>

#include <catch2/catch_test_macros.hpp>

#include <chrono>

using namespace std::chrono_literals;

#define CHECK_SLEEP_DURATION(duration)                                     \
    do                                                                     \
    {                                                                      \
        const auto startTime = std::chrono::steady_clock::now();           \
        sf::sleep((duration));                                             \
        const auto elapsed = std::chrono::steady_clock::now() - startTime; \
        CHECK(elapsed >= (duration));                                      \
    } while (false)

TEST_CASE("[System] sf::sleep")
{
    CHECK_SLEEP_DURATION(1ms);
    CHECK_SLEEP_DURATION(10ms);
    CHECK_SLEEP_DURATION(100ms);
}

#include <SFML/System/Sleep.hpp>

// Other 1st party headers
#include <SFML/System/Time.hpp>

#include <doctest/doctest.h>

#include <chrono>

using namespace std::chrono_literals;

// Specialize StringMaker for std::chrono::duration specializations
// https://github.com/doctest/doctest/blob/master/doc/markdown/stringification.md#docteststringmakert-specialisation
namespace doctest
{
template <typename Rep, typename Period>
struct StringMaker<std::chrono::duration<Rep, Period>>
{
    static String convert(const std::chrono::duration<Rep, Period>& duration)
    {
        return toString(std::chrono::nanoseconds(duration).count()) + "ns";
    }
};
} // namespace doctest

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

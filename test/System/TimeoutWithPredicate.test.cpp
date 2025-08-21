#include <SFML/System/TimeoutWithPredicate.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

TEST_CASE("[System] sf::TimeoutWithPredicate")
{
    SECTION("Type traits")
    {
        static_assert(std::is_copy_constructible_v<sf::TimeoutWithPredicate>);
        static_assert(std::is_copy_assignable_v<sf::TimeoutWithPredicate>);
        static_assert(std::is_nothrow_move_constructible_v<sf::TimeoutWithPredicate>);
        static_assert(std::is_nothrow_move_assignable_v<sf::TimeoutWithPredicate>);
    }

    SECTION("Construction using timeout")
    {
        const sf::TimeoutWithPredicate timeoutPredicate(sf::milliseconds(1234));
        CHECK(timeoutPredicate.getPeriod() == sf::milliseconds(1234));
        REQUIRE(timeoutPredicate.getPredicate());
        CHECK(timeoutPredicate.getPredicate()() == false);
    }

    SECTION("Construction using predicate and period")
    {
        auto                           flag = false;
        const sf::TimeoutWithPredicate timeoutPredicate([&flag] { return flag; }, sf::milliseconds(4321));
        CHECK(timeoutPredicate.getPeriod() == sf::milliseconds(4321));
        REQUIRE(timeoutPredicate.getPredicate());
        CHECK(timeoutPredicate.getPredicate()() == false);
        flag = true;
        CHECK(timeoutPredicate.getPredicate()() == true);
    }

    SECTION("Construction using empty predicate and period")
    {
        const sf::TimeoutWithPredicate timeoutPredicate({}, sf::milliseconds(5678));
        CHECK(timeoutPredicate.getPeriod() == sf::milliseconds(5678));
        REQUIRE(timeoutPredicate.getPredicate());
        CHECK(timeoutPredicate.getPredicate()() == false);
    }
}

#include <SFML/Graphics/Transform.hpp>

#include <SFML/System/Angle.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <sstream>
#include <type_traits>
#include <vector>

#include <cassert>

TEST_CASE("[Graphics] sf::Transform")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::Transform>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Transform>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Transform>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Transform>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            STATIC_CHECK(sf::Transform() == sf::Transform::Identity);
        }

        SECTION("3x3 matrix constructor")
        {
            constexpr sf::Transform transform(10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f);
            const std::vector       matrix(transform.getMatrix(), transform.getMatrix() + 16);
            CHECK(matrix ==
                  std::vector{10.0f, 13.0f, 0.0f, 16.0f, 11.0f, 14.0f, 0.0f, 17.0f, 0.0f, 0.0f, 1.0f, 0.0f, 12.0f, 15.0f, 0.0f, 18.0f});
        }
    }

    SECTION("Identity matrix")
    {
        const std::vector matrix(sf::Transform::Identity.getMatrix(), sf::Transform::Identity.getMatrix() + 16);
        CHECK(matrix ==
              std::vector{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f});
    }

    SECTION("getInverse()")
    {
        STATIC_CHECK(sf::Transform::Identity.getInverse() == sf::Transform::Identity);
        STATIC_CHECK(sf::Transform(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f).getInverse() ==
                     sf::Transform::Identity);
        STATIC_CHECK(sf::Transform(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f).getInverse() ==
                     sf::Transform(0.375f, -0.5f, 0.875f, -1.0f, 1.0f, -1.0f, 0.875f, -0.5f, 0.375f));
    }

    SECTION("transformPoint()")
    {
        STATIC_CHECK(sf::Transform::Identity.transformPoint({-10.0f, -10.0f}) == sf::Vector2f(-10.0f, -10.0f));
        STATIC_CHECK(sf::Transform::Identity.transformPoint({-1.0f, -1.0f}) == sf::Vector2f(-1.0f, -1.0f));
        STATIC_CHECK(sf::Transform::Identity.transformPoint({-1.0f, 0.0f}) == sf::Vector2f(-1.0f, 0.0f));
        STATIC_CHECK(sf::Transform::Identity.transformPoint({0.0f, 0.0f}) == sf::Vector2f(0.0f, 0.0f));
        STATIC_CHECK(sf::Transform::Identity.transformPoint({0.0f, 1.0f}) == sf::Vector2f(0.0f, 1.0f));
        STATIC_CHECK(sf::Transform::Identity.transformPoint({1.0f, 1.0f}) == sf::Vector2f(1.0f, 1.0f));
        STATIC_CHECK(sf::Transform::Identity.transformPoint({10.0f, 10.0f}) == sf::Vector2f(10.0f, 10.0f));

        constexpr sf::Transform transform(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
        STATIC_CHECK(transform.transformPoint({-1.0f, -1.0f}) == sf::Vector2f(0.0f, -5.0f));
        STATIC_CHECK(transform.transformPoint({0.0f, 0.0f}) == sf::Vector2f(3.0f, 4.0f));
        STATIC_CHECK(transform.transformPoint({1.0f, 1.0f}) == sf::Vector2f(6.0f, 13.0f));
    }

    SECTION("transformRect()")
    {
        STATIC_CHECK(sf::Transform::Identity.transformRect({{-200.0f, -200.0f}, {-100.0f, -100.0f}}) ==
                     sf::FloatRect({-300.0f, -300.0f}, {100.0f, 100.0f}));
        STATIC_CHECK(sf::Transform::Identity.transformRect({{0.0f, 0.0f}, {0.0f, 0.0f}}) ==
                     sf::FloatRect({0.0f, 0.0f}, {0.0f, 0.0f}));
        STATIC_CHECK(sf::Transform::Identity.transformRect({{100.0f, 100.0f}, {200.0f, 200.0f}}) ==
                     sf::FloatRect({100.0f, 100.0f}, {200.0f, 200.0f}));

        constexpr sf::Transform transform(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
        STATIC_CHECK(transform.transformRect({{-100.0f, -100.0f}, {200.0f, 200.0f}}) ==
                     sf::FloatRect({-297.0f, -896.0f}, {600.0f, 1800.0f}));
        STATIC_CHECK(transform.transformRect({{0.0f, 0.0f}, {0.0f, 0.0f}}) == sf::FloatRect({3.0f, 4.0f}, {0.0f, 0.0f}));
        STATIC_CHECK(transform.transformRect({{100.0f, 100.0f}, {200.0f, 200.0f}}) ==
                     sf::FloatRect({303.0f, 904.0f}, {600.0f, 1800.0f}));
    }

    SECTION("combine()")
    {
        auto identity = sf::Transform::Identity;
        CHECK(identity.combine(sf::Transform::Identity) == sf::Transform::Identity);
        CHECK(identity.combine(sf::Transform::Identity).combine(sf::Transform::Identity) == sf::Transform::Identity);

        sf::Transform transform(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
        CHECK(identity.combine(transform) == transform);
        CHECK(transform.combine(sf::Transform::Identity) == transform);
        CHECK(transform.combine(transform) == sf::Transform(18.0f, 18.0f, 14.0f, 36.0f, 41.0f, 36.0f, 14.0f, 18.0f, 18.0f));
        CHECK(transform.combine(sf::Transform(10.0f, 2.0f, 3.0f, 4.0f, 50.0f, 40.0f, 30.0f, 20.0f, 10.0f)) ==
              sf::Transform(672.0f, 1216.0f, 914.0f, 1604.0f, 2842.0f, 2108.0f, 752.0f, 1288.0f, 942.0f));
    }

    SECTION("translate()")
    {
        sf::Transform transform(9, 8, 7, 6, 5, 4, 3, 2, 1);
        CHECK(transform.translate({10.0f, 20.0f}) == sf::Transform(9, 8, 257, 6, 5, 164, 3, 2, 71));
        CHECK(transform.translate({10.0f, 20.0f}) == sf::Transform(9, 8, 507, 6, 5, 324, 3, 2, 141));
    }

    SECTION("rotate()")
    {
        SECTION("Around origin")
        {
            sf::Transform transform;
            transform.rotate(sf::degrees(90));
            CHECK(transform == Approx(sf::Transform(0, -1, 0, 1, 0, 0, 0, 0, 1)));
        }

        SECTION("Around custom point")
        {
            sf::Transform transform;
            transform.rotate(sf::degrees(90), {1.0f, 0.0f});
            CHECK(transform == Approx(sf::Transform(0, -1, 1, 1, 0, -1, 0, 0, 1)));
        }
    }

    SECTION("scale()")
    {
        SECTION("About origin")
        {
            sf::Transform transform(1, 2, 3, 4, 5, 4, 3, 2, 1);
            CHECK(transform.scale({2.0f, 4.0f}) == sf::Transform(2, 8, 3, 8, 20, 4, 6, 8, 1));
            CHECK(transform.scale({0.0f, 0.0f}) == sf::Transform(0, 0, 3, 0, 0, 4, 0, 0, 1));
            CHECK(transform.scale({10.0f, 10.0f}) == sf::Transform(0, 0, 3, 0, 0, 4, 0, 0, 1));
        }

        SECTION("About custom point")
        {
            sf::Transform transform(1, 2, 3, 4, 5, 4, 3, 2, 1);
            CHECK(transform.scale({1.0f, 2.0f}, {1.0f, 0.0f}) == sf::Transform(1, 4, 3, 4, 10, 4, 3, 4, 1));
            CHECK(transform.scale({0.0f, 0.0f}, {1.0f, 0.0f}) == sf::Transform(0, 0, 4, 0, 0, 8, 0, 0, 4));
        }
    }

    SECTION("Operators")
    {
        SECTION("operator*")
        {
            STATIC_CHECK(sf::Transform::Identity * sf::Transform::Identity == sf::Transform::Identity);
            STATIC_CHECK(sf::Transform::Identity * sf::Transform::Identity * sf::Transform::Identity ==
                         sf::Transform::Identity);

            constexpr sf::Transform transform(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
            STATIC_CHECK(sf::Transform::Identity * transform == transform);
            STATIC_CHECK(transform * sf::Transform::Identity == transform);
            STATIC_CHECK(transform * transform ==
                         sf::Transform(18.0f, 18.0f, 14.0f, 36.0f, 41.0f, 36.0f, 14.0f, 18.0f, 18.0f));
            STATIC_CHECK(transform * sf::Transform(10.0f, 2.0f, 3.0f, 4.0f, 50.0f, 40.0f, 30.0f, 20.0f, 10.0f) ==
                         sf::Transform(108.0f, 162.0f, 113.0f, 180.0f, 338.0f, 252.0f, 68.0f, 126.0f, 99.0f));
        }

        SECTION("operator*=")
        {
            sf::Transform transform(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
            transform *= sf::Transform::Identity;
            CHECK(transform == sf::Transform(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f));
            transform *= transform;
            CHECK(transform == sf::Transform(18.0f, 18.0f, 14.0f, 36.0f, 41.0f, 36.0f, 14.0f, 18.0f, 18.0f));
            transform *= sf::Transform(10.0f, 2.0f, 3.0f, 4.0f, 50.0f, 40.0f, 30.0f, 20.0f, 10.0f);
            CHECK(transform == sf::Transform(672.0f, 1216.0f, 914.0f, 1604.0f, 2842.0f, 2108.0f, 752.0f, 1288.0f, 942.0f));
        }

        SECTION("operator* with vector")
        {
            STATIC_CHECK(sf::Transform::Identity * sf::Vector2f(-10.0f, -10.0f) == sf::Vector2f(-10.0f, -10.0f));
            STATIC_CHECK(sf::Transform::Identity * sf::Vector2f(-1.0f, -1.0f) == sf::Vector2f(-1.0f, -1.0f));
            STATIC_CHECK(sf::Transform::Identity * sf::Vector2f(-1.0f, 0.0f) == sf::Vector2f(-1.0f, 0.0f));
            STATIC_CHECK(sf::Transform::Identity * sf::Vector2f(0.0f, 0.0f) == sf::Vector2f(0.0f, 0.0f));
            STATIC_CHECK(sf::Transform::Identity * sf::Vector2f(0.0f, 1.0f) == sf::Vector2f(0.0f, 1.0f));
            STATIC_CHECK(sf::Transform::Identity * sf::Vector2f(1.0f, 1.0f) == sf::Vector2f(1.0f, 1.0f));
            STATIC_CHECK(sf::Transform::Identity * sf::Vector2f(10.0f, 10.0f) == sf::Vector2f(10.0f, 10.0f));

            constexpr sf::Transform transform(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
            STATIC_CHECK(transform * sf::Vector2f(-1.0f, -1.0f) == sf::Vector2f(0.0f, -5.0f));
            STATIC_CHECK(transform * sf::Vector2f(0.0f, 0.0f) == sf::Vector2f(3.0f, 4.0f));
            STATIC_CHECK(transform * sf::Vector2f(1.0f, 1.0f) == sf::Vector2f(6.0f, 13.0f));
        }

        SECTION("operator==")
        {
            STATIC_CHECK(sf::Transform::Identity == sf::Transform::Identity);
            STATIC_CHECK(sf::Transform() == sf::Transform());
            STATIC_CHECK(sf::Transform(0, 0, 0, 0, 0, 0, 0, 0, 0) == sf::Transform(0, 0, 0, 0, 0, 0, 0, 0, 0));
            STATIC_CHECK(sf::Transform(0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f) ==
                         sf::Transform(0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f));
            STATIC_CHECK(sf::Transform(1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f) ==
                         sf::Transform(1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f));
        }

        SECTION("operator!=")
        {
            STATIC_CHECK_FALSE(sf::Transform::Identity != sf::Transform::Identity);
            STATIC_CHECK_FALSE(sf::Transform() != sf::Transform());
            STATIC_CHECK_FALSE(sf::Transform(0, 0, 0, 0, 0, 0, 0, 0, 0) != sf::Transform(0, 0, 0, 0, 0, 0, 0, 0, 0));
            STATIC_CHECK_FALSE(
                sf::Transform(0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f) !=
                sf::Transform(0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f));
            STATIC_CHECK_FALSE(
                sf::Transform(1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f) !=
                sf::Transform(1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f));

            STATIC_CHECK(sf::Transform(1, 0, 0, 0, 0, 0, 0, 0, 0) != sf::Transform(0, 0, 0, 0, 0, 0, 0, 0, 0));
            STATIC_CHECK(sf::Transform(0, 1, 0, 0, 0, 0, 0, 0, 0) != sf::Transform(0, 0, 0, 0, 0, 0, 0, 0, 0));
            STATIC_CHECK(sf::Transform(0, 0, 1, 0, 0, 0, 0, 0, 0) != sf::Transform(0, 0, 0, 0, 0, 0, 0, 0, 0));
            STATIC_CHECK(sf::Transform(0, 0, 0, 1, 0, 0, 0, 0, 0) != sf::Transform(0, 0, 0, 0, 0, 0, 0, 0, 0));
            STATIC_CHECK(sf::Transform(0, 0, 0, 0, 1, 0, 0, 0, 0) != sf::Transform(0, 0, 0, 0, 0, 0, 0, 0, 0));
            STATIC_CHECK(sf::Transform(0, 0, 0, 0, 0, 1, 0, 0, 0) != sf::Transform(0, 0, 0, 0, 0, 0, 0, 0, 0));
            STATIC_CHECK(sf::Transform(0, 0, 0, 0, 0, 0, 1, 0, 0) != sf::Transform(0, 0, 0, 0, 0, 0, 0, 0, 0));
            STATIC_CHECK(sf::Transform(0, 0, 0, 0, 0, 0, 0, 1, 0) != sf::Transform(0, 0, 0, 0, 0, 0, 0, 0, 0));
            STATIC_CHECK(sf::Transform(0, 0, 0, 0, 0, 0, 0, 0, 1) != sf::Transform(0, 0, 0, 0, 0, 0, 0, 0, 0));
        }
    }
}

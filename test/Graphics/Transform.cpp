#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Angle.hpp>
#include "GraphicsUtil.hpp"
#include "SystemUtil.hpp"
#include <vector>

#include <doctest.h>

namespace std
{
std::ostream& operator<<(std::ostream& out, const std::vector<float>& vector)
{
    out << "{ ";
    for (size_t i = 0; i + 1 < vector.size(); ++i)
        out << vector[i] << ", ";
    out << vector.back() << " }";
    return out;
}
}

TEST_CASE("sf::Transform class - [graphics]")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            CHECK(sf::Transform() == sf::Transform::Identity);
        }

        SUBCASE("3x3 matrix constructor")
        {
            const sf::Transform transform(10.f, 11.f, 12.f,
                                          13.f, 14.f, 15.f,
                                          16.f, 17.f, 18.f);
            const std::vector<float> matrix(transform.getMatrix(), transform.getMatrix() + 16);
            CHECK(matrix == std::vector<float>{10.f, 13.f, 0.f, 16.f,
                                               11.f, 14.f, 0.f, 17.f,
                                                0.f,  0.f, 1.f,  0.f,
                                               12.f, 15.f, 0.f, 18.f});
        }
    }

    SUBCASE("Identity matrix")
    {
        const std::vector<float> matrix(sf::Transform::Identity.getMatrix(), sf::Transform::Identity.getMatrix() + 16);
        CHECK(matrix == std::vector<float>{1.f, 0.f, 0.f, 0.f,
                                           0.f, 1.f, 0.f, 0.f,
                                           0.f, 0.f, 1.f, 0.f,
                                           0.f, 0.f, 0.f, 1.f});
    }

    SUBCASE("getInverse()")
    {
        CHECK(sf::Transform::Identity.getInverse() == sf::Transform::Identity);
        CHECK(sf::Transform(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f).getInverse() == sf::Transform::Identity);
        CHECK(sf::Transform(1.f, 2.f, 3.f, 4.f, 5.f, 4.f, 3.f, 2.f, 1.f).getInverse() == sf::Transform(0.375f, -0.5f, 0.875f, -1.f, 1.f, -1.f, 0.875f, -0.5f, 0.375f));
    }

    SUBCASE("transformPoint()")
    {
        CHECK(sf::Transform::Identity.transformPoint({-10.f, -10.f}) == sf::Vector2f(-10.f, -10.f));
        CHECK(sf::Transform::Identity.transformPoint({-1.f, -1.f}) == sf::Vector2f(-1.f, -1.f));
        CHECK(sf::Transform::Identity.transformPoint({-1.f, 0.f}) == sf::Vector2f(-1.f, 0.f));
        CHECK(sf::Transform::Identity.transformPoint({0.f, 0.f}) == sf::Vector2f(0.f, 0.f));
        CHECK(sf::Transform::Identity.transformPoint({0.f, 1.f}) == sf::Vector2f(0.f, 1.f));
        CHECK(sf::Transform::Identity.transformPoint({1.f, 1.f}) == sf::Vector2f(1.f, 1.f));
        CHECK(sf::Transform::Identity.transformPoint({10.f, 10.f}) == sf::Vector2f(10.f, 10.f));

        const sf::Transform transform(1.f, 2.f, 3.f,
                                      4.f, 5.f, 4.f,
                                      3.f, 2.f, 1.f);
        CHECK(transform.transformPoint({-1.f, -1.f}) == sf::Vector2f(0.f, -5.f));
        CHECK(transform.transformPoint({0.f, 0.f}) == sf::Vector2f(3.f, 4.f));
        CHECK(transform.transformPoint({1.f, 1.f}) == sf::Vector2f(6.f, 13.f));
    }

    SUBCASE("transformRect()")
    {
        CHECK(sf::Transform::Identity.transformRect({{-200.f, -200.f}, {-100.f, -100.f}}) == sf::FloatRect({-300.f, -300.f}, {100.f, 100.f}));
        CHECK(sf::Transform::Identity.transformRect({{0.f, 0.f}, {0.f, 0.f}}) == sf::FloatRect({0.f, 0.f}, {0.f, 0.f}));
        CHECK(sf::Transform::Identity.transformRect({{100.f, 100.f}, {200.f, 200.f}}) == sf::FloatRect({100.f, 100.f}, {200.f, 200.f}));

        const sf::Transform transform(1.f, 2.f, 3.f,
                                      4.f, 5.f, 4.f,
                                      3.f, 2.f, 1.f);
        CHECK(transform.transformRect({{-100.f, -100.f}, {200.f, 200.f}}) == sf::FloatRect({-297.f, -896.f}, {600.f, 1800.f}));
        CHECK(transform.transformRect({{0.f, 0.f}, {0.f, 0.f}}) == sf::FloatRect({3.f, 4.f}, {0.f, 0.f}));
        CHECK(transform.transformRect({{100.f, 100.f}, {200.f, 200.f}}) == sf::FloatRect({303.f, 904.f}, {600.f, 1800.f}));
    }

    SUBCASE("combine()")
    {
        auto identity = sf::Transform::Identity;
        CHECK(identity.combine(sf::Transform::Identity) == sf::Transform::Identity);
        CHECK(identity.combine(sf::Transform::Identity).combine(sf::Transform::Identity) == sf::Transform::Identity);

        sf::Transform transform(1.f, 2.f, 3.f,
                                4.f, 5.f, 4.f,
                                3.f, 2.f, 1.f);
        CHECK(identity.combine(transform) == transform);
        CHECK(transform.combine(sf::Transform::Identity) == transform);
        CHECK(transform.combine(transform) == sf::Transform(18.f, 18.f, 14.f,
                                                            36.f, 41.f, 36.f,
                                                            14.f, 18.f, 18.f));
        CHECK(transform.combine(sf::Transform(10.f,  2.f,  3.f,
                                               4.f, 50.f, 40.f,
                                              30.f, 20.f, 10.f))
            == sf::Transform( 672.f, 1216.f,  914.f,
                             1604.f, 2842.f, 2108.f,
                              752.f, 1288.f,  942.f));
    }

    SUBCASE("translate()")
    {
        sf::Transform transform(9.f, 8.f, 7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f);
        CHECK(transform.translate({10.f, 20.f}) == sf::Transform(9.f, 8.f, 257.f, 6.f, 5.f, 164.f, 3.f, 2.f, 71.f));
        CHECK(transform.translate({10.f, 20.f}) == sf::Transform(9.f, 8.f, 507.f, 6.f, 5.f, 324.f, 3.f, 2.f, 141.f));
    }

    SUBCASE("rotate()")
    {
        SUBCASE("Around origin")
        {
            sf::Transform transform;
            transform.rotate(sf::degrees(90));
            CHECK(transform == Approx(sf::Transform(0.f, -1.f,  0.f,
                                                    1.f,  0.f,  0.f,
                                                    0.f,  0.f,  1.f)));
        }

        SUBCASE("Around custom point")
        {
            sf::Transform transform;
            transform.rotate(sf::degrees(90), {1.f, 0.f});
            CHECK(transform == Approx(sf::Transform(0.f, -1.f,  1.f,
                                                    1.f,  0.f, -1.f,
                                                    0.f,  0.f,  1.f)));
        }
    }

    SUBCASE("scale()")
    {
        SUBCASE("About origin")
        {
            sf::Transform transform(1.f, 2.f, 3.f, 4.f, 5.f, 4.f, 3.f, 2.f, 1.f);
            CHECK(transform.scale({2.f, 4.f}) == sf::Transform(2.f, 8.f, 3.f, 8.f, 20.f, 4.f, 6.f, 8.f, 1.f));
            CHECK(transform.scale({0.f, 0.f}) == sf::Transform(0.f, 0.f, 3.f, 0.f, 0.f, 4.f, 0.f, 0.f, 1.f));
            CHECK(transform.scale({10.f, 10.f}) == sf::Transform(0.f, 0.f, 3.f, 0.f, 0.f, 4.f, 0.f, 0.f, 1.f));
        }

        SUBCASE("About custom point")
        {
            sf::Transform transform(1.f, 2.f, 3.f, 4.f, 5.f, 4.f, 3.f, 2.f, 1.f);
            CHECK(transform.scale({1.f, 2.f}, {1.f, 0.f}) == sf::Transform(1.f, 4.f, 3.f, 4.f, 10.f, 4.f, 3.f, 4.f, 1.f));
            CHECK(transform.scale({0.f, 0.f}, {1.f, 0.f}) == sf::Transform(0.f, 0.f, 4.f, 0.f, 0.f, 8.f, 0.f, 0.f, 4.f));
        }
    }

    SUBCASE("Operators")
    {
        SUBCASE("operator*")
        {
            CHECK(sf::Transform::Identity * sf::Transform::Identity == sf::Transform::Identity);
            CHECK(sf::Transform::Identity * sf::Transform::Identity * sf::Transform::Identity == sf::Transform::Identity);

            const sf::Transform transform(1.f, 2.f, 3.f,
                                          4.f, 5.f, 4.f,
                                          3.f, 2.f, 1.f);
            CHECK(sf::Transform::Identity * transform == transform);
            CHECK(transform * sf::Transform::Identity == transform);
            CHECK(transform * transform == sf::Transform(18.f, 18.f, 14.f,
                                                         36.f, 41.f, 36.f,
                                                         14.f, 18.f, 18.f));
            CHECK(transform * sf::Transform(10.f,  2.f,  3.f,
                                             4.f, 50.f, 40.f,
                                            30.f, 20.f, 10.f)
                == sf::Transform(108.f, 162.f, 113.f,
                                 180.f, 338.f, 252.f,
                                  68.f, 126.f,  99.f));
        }

        SUBCASE("operator*=")
        {
            sf::Transform transform(1.f, 2.f, 3.f,
                                    4.f, 5.f, 4.f,
                                    3.f, 2.f, 1.f);
            transform *= sf::Transform::Identity;
            CHECK(transform == sf::Transform(1.f, 2.f, 3.f, 4.f, 5.f, 4.f, 3.f, 2.f, 1.f));
            transform *= transform;
            CHECK(transform == sf::Transform(18.f, 18.f, 14.f,
                                             36.f, 41.f, 36.f,
                                             14.f, 18.f, 18.f));
            transform *= sf::Transform(10.f,  2.f,  3.f,
                                        4.f, 50.f, 40.f,
                                       30.f, 20.f, 10.f);
            CHECK(transform == sf::Transform( 672.f, 1216.f,  914.f,
                                             1604.f, 2842.f, 2108.f,
                                              752.f, 1288.f,  942.f));
        }

        SUBCASE("operator* with vector")
        {
            CHECK(sf::Transform::Identity * sf::Vector2f(-10.f, -10.f) == sf::Vector2f(-10.f, -10.f));
            CHECK(sf::Transform::Identity * sf::Vector2f(-1.f, -1.f) == sf::Vector2f(-1.f, -1.f));
            CHECK(sf::Transform::Identity * sf::Vector2f(-1.f, 0.f) == sf::Vector2f(-1.f, 0.f));
            CHECK(sf::Transform::Identity * sf::Vector2f(0.f, 0.f) == sf::Vector2f(0.f, 0.f));
            CHECK(sf::Transform::Identity * sf::Vector2f(0.f, 1.f) == sf::Vector2f(0.f, 1.f));
            CHECK(sf::Transform::Identity * sf::Vector2f(1.f, 1.f) == sf::Vector2f(1.f, 1.f));
            CHECK(sf::Transform::Identity * sf::Vector2f(10.f, 10.f) == sf::Vector2f(10.f, 10.f));

            const sf::Transform transform(1.f, 2.f, 3.f,
                                          4.f, 5.f, 4.f,
                                          3.f, 2.f, 1.f);
            CHECK(transform * sf::Vector2f(-1.f, -1.f) == sf::Vector2f(0.f, -5.f));
            CHECK(transform * sf::Vector2f(0.f, 0.f) == sf::Vector2f(3.f, 4.f));
            CHECK(transform * sf::Vector2f(1.f, 1.f) == sf::Vector2f(6.f, 13.f));
        }

        SUBCASE("operator==")
        {
            CHECK(sf::Transform::Identity == sf::Transform::Identity);
            CHECK(sf::Transform() == sf::Transform());
            CHECK(sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f) == sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f));
            CHECK(sf::Transform(0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f)
               == sf::Transform(0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f));
            CHECK(sf::Transform(1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f)
               == sf::Transform(1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f));
        }

        SUBCASE("operator!=")
        {
            CHECK_FALSE(sf::Transform::Identity != sf::Transform::Identity);
            CHECK_FALSE(sf::Transform() != sf::Transform());
            CHECK_FALSE(sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f) != sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f));
            CHECK_FALSE(sf::Transform(0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f)
                     != sf::Transform(0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f, 0.0001f));
            CHECK_FALSE(sf::Transform(1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f)
                     != sf::Transform(1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f, 1000.f));

            CHECK(sf::Transform(1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f) != sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f));
            CHECK(sf::Transform(0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f) != sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f));
            CHECK(sf::Transform(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f) != sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f));
            CHECK(sf::Transform(0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f) != sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f));
            CHECK(sf::Transform(0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f) != sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f));
            CHECK(sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f) != sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f));
            CHECK(sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f) != sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f));
            CHECK(sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f) != sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f));
            CHECK(sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f) != sf::Transform(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f));
        }
    }
}

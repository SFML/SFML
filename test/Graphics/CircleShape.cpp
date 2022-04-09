#include <SFML/Graphics/CircleShape.hpp>
#include "SystemUtil.hpp"

#include <doctest.h>

TEST_CASE("sf::CircleShape class - [graphics]")
{
    SUBCASE("Default constructor")
    {
        const sf::CircleShape circle;
        CHECK(circle.getRadius() == 0);
        CHECK(circle.getPointCount() == 30);
        for (std::size_t i = 0; i < circle.getPointCount(); ++i)
            CHECK(circle.getPoint(i) == sf::Vector2f(0, 0));
    }

    SUBCASE("Radius constructor")
    {
        const sf::CircleShape circle(15.f);
        CHECK(circle.getRadius() == 15.f);
        CHECK(circle.getPointCount() == 30);
        CHECK(circle.getPoint(0) == ApproxVec2(15, 0));
        CHECK(circle.getPoint(1) == ApproxVec2(18.118675232f, 0.327785492f));
        CHECK(circle.getPoint(2) == ApproxVec2(21.101049423f, 1.296817780f));
        CHECK(circle.getPoint(3) == ApproxVec2(23.816780090f, 2.864745140f));
        CHECK(circle.getPoint(4) == ApproxVec2(26.147172928f, 4.963042259f));
        CHECK(circle.getPoint(5) == ApproxVec2(27.990381241f, 7.500000000f));
        CHECK(circle.getPoint(6) == ApproxVec2(29.265848160f, 10.364745140f));
        CHECK(circle.getPoint(7) == ApproxVec2(29.917827606f, 13.432072639f));
        CHECK(circle.getPoint(8) == ApproxVec2(29.917827606f, 16.567928314f));
        CHECK(circle.getPoint(9) == ApproxVec2(29.265846252f, 19.635257721f));
        CHECK(circle.getPoint(10) == ApproxVec2(27.990381241f, 22.500000000f));
        CHECK(circle.getPoint(11) == ApproxVec2(26.147171021f, 25.036960602f));
        CHECK(circle.getPoint(12) == ApproxVec2(23.816780090f, 27.135253906f));
        CHECK(circle.getPoint(13) == ApproxVec2(21.101049423f, 28.703182220f));
        CHECK(circle.getPoint(14) == ApproxVec2(18.118675232f, 29.672214508f));
        CHECK(circle.getPoint(15) == ApproxVec2(14.999999046f, 30.000000000f));
        CHECK(circle.getPoint(16) == ApproxVec2(11.881320953f, 29.672214508f));
        CHECK(circle.getPoint(17) == ApproxVec2(8.898950577f, 28.703182220f));
        CHECK(circle.getPoint(18) == ApproxVec2(6.183218956f, 27.135253906f));
        CHECK(circle.getPoint(19) == ApproxVec2(3.852828979f, 25.036960602f));
        CHECK(circle.getPoint(20) == ApproxVec2(2.009618759f, 22.500000000f));
        CHECK(circle.getPoint(21) == ApproxVec2(0.734152794f, 19.635255814f));
        CHECK(circle.getPoint(22) == ApproxVec2(0.082171440f, 16.567928314f));
        CHECK(circle.getPoint(23) == ApproxVec2(0.082171440f, 13.432073593f));
        CHECK(circle.getPoint(24) == ApproxVec2(0.734151840f, 10.364746094f));
        CHECK(circle.getPoint(25) == ApproxVec2(2.009618759f, 7.500000000f));
        CHECK(circle.getPoint(26) == ApproxVec2(3.852827072f, 4.963040352f));
        CHECK(circle.getPoint(27) == ApproxVec2(6.183218002f, 2.864748001f));
        CHECK(circle.getPoint(28) == ApproxVec2(8.898950577f, 1.296818733f));
        CHECK(circle.getPoint(29) == ApproxVec2(11.881320953f, 0.327786446f));
    }

    SUBCASE("Radius and point count constructor")
    {
        const sf::CircleShape circle(5.f, 8);
        CHECK(circle.getRadius() == 5.f);
        CHECK(circle.getPointCount() == 8);
        CHECK(circle.getPoint(0) == ApproxVec2(5.000000000f, 0.000000000f));
        CHECK(circle.getPoint(1) == ApproxVec2(8.535533905f, 1.464465857f));
        CHECK(circle.getPoint(2) == ApproxVec2(10.000000000f, 5.000000000f));
        CHECK(circle.getPoint(3) == ApproxVec2(8.535533905f, 8.535533905f));
        CHECK(circle.getPoint(4) == ApproxVec2(5.000000000f, 10.000000000f));
        CHECK(circle.getPoint(5) == ApproxVec2(1.464466095f, 8.535533905f));
        CHECK(circle.getPoint(6) == ApproxVec2(0.000000000f, 4.999999523f));
        CHECK(circle.getPoint(7) == ApproxVec2(1.464465857f, 1.464466572f));
    }

    SUBCASE("Set radius")
    {
        sf::CircleShape circle(1.f, 6);
        circle.setRadius(10.f);
        CHECK(circle.getRadius() == 10.f);
        CHECK(circle.getPointCount() == 6);
        CHECK(circle.getPoint(0) == ApproxVec2(10.000000000f, 0.000000000f));
        CHECK(circle.getPoint(1) == ApproxVec2(18.660253525f, 5.000000000f));
        CHECK(circle.getPoint(2) == ApproxVec2(18.660253525f, 15.000000000f));
        CHECK(circle.getPoint(3) == ApproxVec2(10.000000000f, 20.000000000f));
        CHECK(circle.getPoint(4) == ApproxVec2(1.339746475f, 15.000000000f));
        CHECK(circle.getPoint(5) == ApproxVec2(1.339745522f, 5.000000000f));
    }

    SUBCASE("Set point count")
    {
        sf::CircleShape circle(4.f, 10);
        circle.setPointCount(4);
        CHECK(circle.getRadius() == 4.f);
        CHECK(circle.getPointCount() == 4);
        CHECK(circle.getPoint(0) == ApproxVec2(3.999999762f, 0.000000000f));
        CHECK(circle.getPoint(1) == ApproxVec2(8.000000000f, 4.000000000f));
        CHECK(circle.getPoint(2) == ApproxVec2(3.999999762f, 8.000000000f));
        CHECK(circle.getPoint(3) == ApproxVec2(0.000000000f, 3.999999762f));
    }

    SUBCASE("Equilateral triangle")
    {
        const sf::CircleShape triangle(2.f, 3);
        CHECK(triangle.getRadius() == 2.f);
        CHECK(triangle.getPointCount() == 3);
        CHECK(triangle.getPoint(0) == ApproxVec2(1.999999881f, 0.000000000f));
        CHECK(triangle.getPoint(1) == ApproxVec2(3.732050896f, 3.000000000f));
        CHECK(triangle.getPoint(2) == ApproxVec2(0.267949224f, 3.000000000f));
    }
}

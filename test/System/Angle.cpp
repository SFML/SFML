#include <SFML/System/Angle.hpp>

#include <doctest/doctest.h>

#include <SystemUtil.hpp>

TEST_CASE("sf::Angle class - [system]")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            const sf::Angle angle;
            CHECK(angle.asDegrees() == 0.f);
            CHECK(angle.asRadians() == 0.f);
        }

        SUBCASE("wrapSigned()")
        {
            CHECK(sf::Angle::Zero.wrapSigned() == sf::Angle::Zero);
            CHECK(sf::degrees(0).wrapSigned() == sf::degrees(0));
            CHECK(sf::degrees(1).wrapSigned() == sf::degrees(1));
            CHECK(sf::degrees(-1).wrapSigned() == sf::degrees(-1));
            CHECK(sf::degrees(90).wrapSigned() == sf::degrees(90));
            CHECK(sf::degrees(-90).wrapSigned() == sf::degrees(-90));
            CHECK(sf::degrees(180).wrapSigned() == sf::degrees(-180));
            CHECK(sf::degrees(-180).wrapSigned() == sf::degrees(-180));
            CHECK(sf::degrees(360).wrapSigned() == sf::degrees(0));
            CHECK(sf::degrees(-360).wrapSigned() == sf::degrees(0));
            CHECK(sf::degrees(720).wrapSigned() == sf::degrees(0));
            CHECK(sf::degrees(-720).wrapSigned() == sf::degrees(0));
        }

        SUBCASE("wrapUnsigned()")
        {
            CHECK(sf::Angle::Zero.wrapUnsigned() == sf::Angle::Zero);
            CHECK(sf::degrees(0).wrapUnsigned() == sf::degrees(0));
            CHECK(sf::degrees(1).wrapUnsigned() == sf::degrees(1));
            CHECK(sf::degrees(-1).wrapUnsigned() == sf::degrees(359));
            CHECK(sf::degrees(90).wrapUnsigned() == sf::degrees(90));
            CHECK(sf::degrees(-90).wrapUnsigned() == sf::degrees(270));
            CHECK(sf::degrees(180).wrapUnsigned() == sf::degrees(180));
            CHECK(sf::degrees(-180).wrapUnsigned() == sf::degrees(180));
            CHECK(sf::degrees(360).wrapUnsigned() == sf::degrees(0));
            CHECK(sf::degrees(-360).wrapUnsigned() == sf::degrees(0));
            CHECK(sf::degrees(720).wrapUnsigned() == sf::degrees(0));
            CHECK(sf::degrees(-720).wrapUnsigned() == sf::degrees(0));
        }

        SUBCASE("degrees()")
        {
            const sf::Angle angle = sf::degrees(15);
            CHECK(angle == sf::degrees(15));
            CHECK(angle.asRadians() == Approx(0.26179939f));

            const sf::Angle bigAngle = sf::degrees(1000);
            CHECK(bigAngle == sf::degrees(1000));
            CHECK(bigAngle.asRadians() == Approx(17.453293f));

            const sf::Angle bigNegativeAngle = sf::degrees(-4321);
            CHECK(bigNegativeAngle == sf::degrees(-4321));
            CHECK(bigNegativeAngle.asRadians() == Approx(-75.415677f));
        }

        SUBCASE("radians()")
        {
            const sf::Angle angle = sf::radians(1);
            CHECK(angle.asDegrees() == Approx(57.2957795f));
            CHECK(angle.asRadians() == Approx(1.f));

            const sf::Angle bigAngle = sf::radians(72);
            CHECK(bigAngle.asDegrees() == Approx(4125.29612f));
            CHECK(bigAngle.asRadians() == Approx(72.f));

            const sf::Angle bigNegativeAngle = sf::radians(-200);
            CHECK(bigNegativeAngle.asDegrees() == Approx(-11459.1559f));
            CHECK(bigNegativeAngle.asRadians() == Approx(-200.f));
        }
    }

    SUBCASE("Constants")
    {
        CHECK(sf::Angle::Zero.asDegrees() == 0.f);
        CHECK(sf::Angle::Zero.asRadians() == 0.f);
    }

    SUBCASE("Operators")
    {
        SUBCASE("operator==")
        {
            CHECK(sf::Angle() == sf::Angle());
            CHECK(sf::Angle() == sf::Angle::Zero);
            CHECK(sf::Angle() == sf::degrees(0));
            CHECK(sf::Angle() == sf::radians(0));
            CHECK(sf::degrees(0) == sf::radians(0));
            CHECK(sf::degrees(15) == sf::degrees(15));
            CHECK(sf::radians(1) == sf::radians(1));
            CHECK(sf::degrees(360) == sf::degrees(360));
            CHECK(sf::degrees(720) == sf::degrees(720));
        }

        SUBCASE("operator!=")
        {
            CHECK(sf::Angle() != sf::radians(2));
            CHECK(sf::degrees(1) != sf::radians(1));
            CHECK(sf::radians(0) != sf::radians(0.1f));
        }

        SUBCASE("operator<")
        {
            CHECK(sf::radians(0) < sf::degrees(0.1f));
            CHECK(sf::degrees(0) < sf::radians(0.1f));
            CHECK(sf::radians(-0.1f) < sf::radians(0));
            CHECK(sf::degrees(-0.1f) < sf::degrees(0));
        }

        SUBCASE("operator>")
        {
            CHECK(sf::radians(0.1f) > sf::degrees(0));
            CHECK(sf::degrees(0.1f) > sf::radians(0));
            CHECK(sf::radians(0) > sf::radians(-0.1f));
            CHECK(sf::degrees(0) > sf::degrees(-0.1f));
        }

        SUBCASE("operator<=")
        {
            CHECK(sf::radians(0) <= sf::degrees(0.1f));
            CHECK(sf::degrees(0) <= sf::radians(0.1f));
            CHECK(sf::radians(-0.1f) <= sf::radians(0));
            CHECK(sf::degrees(-0.1f) <= sf::degrees(0));

            CHECK(sf::Angle() <= sf::Angle());
            CHECK(sf::Angle() <= sf::Angle::Zero);
            CHECK(sf::Angle() <= sf::degrees(0));
            CHECK(sf::Angle() <= sf::radians(0));
            CHECK(sf::degrees(0) <= sf::radians(0));
            CHECK(sf::degrees(15) <= sf::degrees(15));
            CHECK(sf::radians(1) <= sf::radians(1));
            CHECK(sf::degrees(360) <= sf::degrees(360));
            CHECK(sf::degrees(720) <= sf::degrees(720));
        }

        SUBCASE("operator>=")
        {
            CHECK(sf::radians(0.1f) >= sf::degrees(0));
            CHECK(sf::degrees(0.1f) >= sf::radians(0));
            CHECK(sf::radians(0) >= sf::radians(-0.1f));
            CHECK(sf::degrees(0) >= sf::degrees(-0.1f));

            CHECK(sf::Angle() >= sf::Angle());
            CHECK(sf::Angle() >= sf::Angle::Zero);
            CHECK(sf::Angle() >= sf::degrees(0));
            CHECK(sf::Angle() >= sf::radians(0));
            CHECK(sf::degrees(0) >= sf::radians(0));
            CHECK(sf::degrees(15) >= sf::degrees(15));
            CHECK(sf::radians(1) >= sf::radians(1));
            CHECK(sf::degrees(360) >= sf::degrees(360));
            CHECK(sf::degrees(720) >= sf::degrees(720));
        }

        SUBCASE("Unary operator-")
        {
            CHECK(-sf::Angle() == sf::Angle());
            CHECK(-sf::radians(-1) == sf::radians(1));
            CHECK(-sf::degrees(15) == sf::degrees(-15));
            CHECK(-sf::radians(1) == sf::radians(-1));
        }

        SUBCASE("operator+")
        {
            CHECK(sf::Angle() + sf::Angle() == sf::Angle());
            CHECK(sf::Angle::Zero + sf::radians(0.5f) == sf::radians(0.5f));
            CHECK(sf::radians(6) + sf::radians(0.5f) == sf::radians(6.5f));
            CHECK(sf::radians(10) + sf::radians(0.5f) == sf::radians(10.5f));
            CHECK(sf::degrees(360) + sf::degrees(360) == sf::degrees(720));
        }

        SUBCASE("operator+=")
        {
            sf::Angle angle = sf::degrees(-15);
            angle += sf::degrees(15);
            CHECK(angle == sf::degrees(0));
            angle += sf::radians(10);
            CHECK(angle == sf::radians(10));
        }

        SUBCASE("operator-")
        {
            CHECK(sf::Angle() - sf::Angle() == sf::Angle());
            CHECK(sf::radians(1) - sf::radians(0.5f) == sf::radians(0.5f));
            CHECK(sf::Angle::Zero - sf::radians(0.5f) == sf::radians(-0.5f));
            CHECK(sf::degrees(900) - sf::degrees(1) == sf::degrees(899));
        }

        SUBCASE("operator-=")
        {
            sf::Angle angle = sf::degrees(15);
            angle -= sf::degrees(15);
            CHECK(angle == sf::degrees(0));
            angle -= sf::radians(10);
            CHECK(angle == sf::radians(-10));
        }

        SUBCASE("operator*")
        {
            CHECK(sf::radians(0) * 10 == sf::Angle::Zero);
            CHECK(sf::degrees(10) * 2.5f == sf::degrees(25));
            CHECK(sf::degrees(100) * 10.0f == sf::degrees(1000));

            CHECK(10 * sf::radians(0) == sf::Angle::Zero);
            CHECK(2.5f * sf::degrees(10) == sf::degrees(25));
            CHECK(10.0f * sf::degrees(100) == sf::degrees(1000));
        }

        SUBCASE("operator*=")
        {
            sf::Angle angle = sf::degrees(1);
            angle *= 10;
            CHECK(angle == sf::degrees(10));
        }

        SUBCASE("operator/")
        {
            CHECK(sf::Angle::Zero / 10 == sf::Angle::Zero);
            CHECK(sf::degrees(10) / 2.5f == sf::degrees(4));
            CHECK(sf::radians(12) / 3 == sf::radians(4));

            CHECK(sf::Angle::Zero / sf::degrees(1) == 0.f);
            CHECK(sf::degrees(10) / sf::degrees(10) == 1.f);
            CHECK(sf::radians(10) / sf::radians(2) == Approx(5.f));
        }

        SUBCASE("operator/=")
        {
            sf::Angle angle = sf::degrees(60);
            angle /= 5;
            CHECK(angle == sf::degrees(12));
        }

        SUBCASE("operator%")
        {
            CHECK(sf::Angle::Zero % sf::radians(0.5f) == sf::Angle::Zero);
            CHECK(sf::radians(10) % sf::radians(1) == sf::radians(0));
            CHECK(sf::degrees(90) % sf::degrees(30) == sf::degrees(0));
            CHECK(sf::degrees(90) % sf::degrees(40) == sf::degrees(10));
            CHECK(sf::degrees(-90) % sf::degrees(30) == sf::degrees(0));
            CHECK(sf::degrees(-90) % sf::degrees(40) == sf::degrees(30));
        }

        SUBCASE("operator%=")
        {
            sf::Angle angle = sf::degrees(59);
            angle %= sf::degrees(10);
            CHECK(angle == sf::degrees(9));
        }

        SUBCASE("operator _deg")
        {
            using namespace sf::Literals;
            CHECK(0.0_deg == sf::Angle::Zero);
            CHECK(1.0_deg == sf::degrees(1));
            CHECK(-1.0_deg == sf::degrees(-1));
            CHECK(3.14_deg == sf::degrees(3.14f));
            CHECK(-3.14_deg == sf::degrees(-3.14f));

            CHECK(0_deg == sf::Angle::Zero);
            CHECK(1_deg == sf::degrees(1));
            CHECK(-1_deg == sf::degrees(-1));
            CHECK(100_deg == sf::degrees(100));
            CHECK(-100_deg == sf::degrees(-100));
        }

        SUBCASE("operator _rad")
        {
            using namespace sf::Literals;
            CHECK(0.0_rad == sf::Angle::Zero);
            CHECK(1.0_rad == sf::radians(1));
            CHECK(-1.0_rad == sf::radians(-1));
            CHECK(3.14_rad == sf::radians(3.14f));
            CHECK(-3.14_rad == sf::radians(-3.14f));

            CHECK(0_rad == sf::Angle::Zero);
            CHECK(1_rad == sf::radians(1));
            CHECK(-1_rad == sf::radians(-1));
            CHECK(100_rad == sf::radians(100));
            CHECK(-100_rad == sf::radians(-100));
        }
    }

    SUBCASE("Constexpr support")
    {
        constexpr auto result = []
        {
            sf::Angle angle = sf::degrees(9);
            angle %= sf::degrees(2);
            return angle;
        }();

        static_assert(result == sf::degrees(1));
    }
}

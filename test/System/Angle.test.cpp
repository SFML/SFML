#include <SFML/System/Angle.hpp>

#include <catch2/catch_test_macros.hpp>

#include <SystemUtil.hpp>
#include <type_traits>

TEST_CASE("[System] sf::Angle")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::Angle>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Angle>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Angle>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Angle>);
    }

    SECTION("Construction")
    {
        constexpr sf::Angle angle;
        STATIC_CHECK(angle.asDegrees() == 0.f);
        STATIC_CHECK(angle.asRadians() == 0.f);
    }

    SECTION("wrapSigned()")
    {
        STATIC_CHECK(sf::Angle::Zero.wrapSigned() == sf::Angle::Zero);
        STATIC_CHECK(sf::degrees(0).wrapSigned() == sf::degrees(0));
        STATIC_CHECK(sf::degrees(1).wrapSigned() == sf::degrees(1));
        STATIC_CHECK(sf::degrees(-1).wrapSigned() == sf::degrees(-1));
        STATIC_CHECK(sf::degrees(90).wrapSigned() == sf::degrees(90));
        STATIC_CHECK(sf::degrees(-90).wrapSigned() == sf::degrees(-90));
        STATIC_CHECK(sf::degrees(180).wrapSigned() == sf::degrees(-180));
        STATIC_CHECK(sf::degrees(-180).wrapSigned() == sf::degrees(-180));
        STATIC_CHECK(sf::degrees(360).wrapSigned() == sf::degrees(0));
        STATIC_CHECK(sf::degrees(-360).wrapSigned() == sf::degrees(0));
        STATIC_CHECK(sf::degrees(720).wrapSigned() == sf::degrees(0));
        STATIC_CHECK(sf::degrees(-720).wrapSigned() == sf::degrees(0));
    }

    SECTION("wrapUnsigned()")
    {
        STATIC_CHECK(sf::Angle::Zero.wrapUnsigned() == sf::Angle::Zero);
        STATIC_CHECK(sf::degrees(0).wrapUnsigned() == sf::degrees(0));
        STATIC_CHECK(sf::degrees(1).wrapUnsigned() == sf::degrees(1));
        STATIC_CHECK(sf::degrees(-1).wrapUnsigned() == sf::degrees(359));
        STATIC_CHECK(sf::degrees(90).wrapUnsigned() == sf::degrees(90));
        STATIC_CHECK(sf::degrees(-90).wrapUnsigned() == sf::degrees(270));
        STATIC_CHECK(sf::degrees(180).wrapUnsigned() == sf::degrees(180));
        STATIC_CHECK(sf::degrees(-180).wrapUnsigned() == sf::degrees(180));
        STATIC_CHECK(sf::degrees(360).wrapUnsigned() == sf::degrees(0));
        STATIC_CHECK(sf::degrees(-360).wrapUnsigned() == sf::degrees(0));
        STATIC_CHECK(sf::degrees(720).wrapUnsigned() == sf::degrees(0));
        STATIC_CHECK(sf::degrees(-720).wrapUnsigned() == sf::degrees(0));
    }

    SECTION("degrees()")
    {
        constexpr sf::Angle angle = sf::degrees(15);
        STATIC_CHECK(angle == sf::degrees(15));
        CHECK(angle.asRadians() == Approx(0.26179939f));

        constexpr sf::Angle bigAngle = sf::degrees(1000);
        STATIC_CHECK(bigAngle == sf::degrees(1000));
        CHECK(bigAngle.asRadians() == Approx(17.453293f));

        constexpr sf::Angle bigNegativeAngle = sf::degrees(-4321);
        STATIC_CHECK(bigNegativeAngle == sf::degrees(-4321));
        CHECK(bigNegativeAngle.asRadians() == Approx(-75.415677f));
    }

    SECTION("radians()")
    {
        constexpr sf::Angle angle = sf::radians(1);
        CHECK(angle.asDegrees() == Approx(57.2957795f));
        CHECK(angle.asRadians() == Approx(1.f));

        constexpr sf::Angle bigAngle = sf::radians(72);
        CHECK(bigAngle.asDegrees() == Approx(4125.29612f));
        CHECK(bigAngle.asRadians() == Approx(72.f));

        constexpr sf::Angle bigNegativeAngle = sf::radians(-200);
        CHECK(bigNegativeAngle.asDegrees() == Approx(-11459.1559f));
        CHECK(bigNegativeAngle.asRadians() == Approx(-200.f));
    }

    SECTION("Constants")
    {
        STATIC_CHECK(sf::Angle::Zero.asDegrees() == 0.f);
        STATIC_CHECK(sf::Angle::Zero.asRadians() == 0.f);
    }

    SECTION("Operators")
    {
        SECTION("operator==")
        {
            STATIC_CHECK(sf::Angle() == sf::Angle());
            STATIC_CHECK(sf::Angle() == sf::Angle::Zero);
            STATIC_CHECK(sf::Angle() == sf::degrees(0));
            STATIC_CHECK(sf::Angle() == sf::radians(0));
            STATIC_CHECK(sf::degrees(0) == sf::radians(0));
            STATIC_CHECK(sf::degrees(15) == sf::degrees(15));
            STATIC_CHECK(sf::radians(1) == sf::radians(1));
            STATIC_CHECK(sf::degrees(360) == sf::degrees(360));
            STATIC_CHECK(sf::degrees(720) == sf::degrees(720));
        }

        SECTION("operator!=")
        {
            STATIC_CHECK(sf::Angle() != sf::radians(2));
            STATIC_CHECK(sf::degrees(1) != sf::radians(1));
            STATIC_CHECK(sf::radians(0) != sf::radians(0.1f));
        }

        SECTION("operator<")
        {
            STATIC_CHECK(sf::radians(0) < sf::degrees(0.1f));
            STATIC_CHECK(sf::degrees(0) < sf::radians(0.1f));
            STATIC_CHECK(sf::radians(-0.1f) < sf::radians(0));
            STATIC_CHECK(sf::degrees(-0.1f) < sf::degrees(0));
        }

        SECTION("operator>")
        {
            STATIC_CHECK(sf::radians(0.1f) > sf::degrees(0));
            STATIC_CHECK(sf::degrees(0.1f) > sf::radians(0));
            STATIC_CHECK(sf::radians(0) > sf::radians(-0.1f));
            STATIC_CHECK(sf::degrees(0) > sf::degrees(-0.1f));
        }

        SECTION("operator<=")
        {
            STATIC_CHECK(sf::radians(0) <= sf::degrees(0.1f));
            STATIC_CHECK(sf::degrees(0) <= sf::radians(0.1f));
            STATIC_CHECK(sf::radians(-0.1f) <= sf::radians(0));
            STATIC_CHECK(sf::degrees(-0.1f) <= sf::degrees(0));

            STATIC_CHECK(sf::Angle() <= sf::Angle());
            STATIC_CHECK(sf::Angle() <= sf::Angle::Zero);
            STATIC_CHECK(sf::Angle() <= sf::degrees(0));
            STATIC_CHECK(sf::Angle() <= sf::radians(0));
            STATIC_CHECK(sf::degrees(0) <= sf::radians(0));
            STATIC_CHECK(sf::degrees(15) <= sf::degrees(15));
            STATIC_CHECK(sf::radians(1) <= sf::radians(1));
            STATIC_CHECK(sf::degrees(360) <= sf::degrees(360));
            STATIC_CHECK(sf::degrees(720) <= sf::degrees(720));
        }

        SECTION("operator>=")
        {
            STATIC_CHECK(sf::radians(0.1f) >= sf::degrees(0));
            STATIC_CHECK(sf::degrees(0.1f) >= sf::radians(0));
            STATIC_CHECK(sf::radians(0) >= sf::radians(-0.1f));
            STATIC_CHECK(sf::degrees(0) >= sf::degrees(-0.1f));

            STATIC_CHECK(sf::Angle() >= sf::Angle());
            STATIC_CHECK(sf::Angle() >= sf::Angle::Zero);
            STATIC_CHECK(sf::Angle() >= sf::degrees(0));
            STATIC_CHECK(sf::Angle() >= sf::radians(0));
            STATIC_CHECK(sf::degrees(0) >= sf::radians(0));
            STATIC_CHECK(sf::degrees(15) >= sf::degrees(15));
            STATIC_CHECK(sf::radians(1) >= sf::radians(1));
            STATIC_CHECK(sf::degrees(360) >= sf::degrees(360));
            STATIC_CHECK(sf::degrees(720) >= sf::degrees(720));
        }

        SECTION("Unary operator-")
        {
            STATIC_CHECK(-sf::Angle() == sf::Angle());
            STATIC_CHECK(-sf::radians(-1) == sf::radians(1));
            STATIC_CHECK(-sf::degrees(15) == sf::degrees(-15));
            STATIC_CHECK(-sf::radians(1) == sf::radians(-1));
        }

        SECTION("operator+")
        {
            STATIC_CHECK(sf::Angle() + sf::Angle() == sf::Angle());
            STATIC_CHECK(sf::Angle::Zero + sf::radians(0.5f) == sf::radians(0.5f));
            STATIC_CHECK(sf::radians(6) + sf::radians(0.5f) == sf::radians(6.5f));
            STATIC_CHECK(sf::radians(10) + sf::radians(0.5f) == sf::radians(10.5f));
            STATIC_CHECK(sf::degrees(360) + sf::degrees(360) == sf::degrees(720));
        }

        SECTION("operator+=")
        {
            sf::Angle angle = sf::degrees(-15);
            angle += sf::degrees(15);
            CHECK(angle == sf::degrees(0));
            angle += sf::radians(10);
            CHECK(angle == sf::radians(10));
        }

        SECTION("operator-")
        {
            STATIC_CHECK(sf::Angle() - sf::Angle() == sf::Angle());
            STATIC_CHECK(sf::radians(1) - sf::radians(0.5f) == sf::radians(0.5f));
            STATIC_CHECK(sf::Angle::Zero - sf::radians(0.5f) == sf::radians(-0.5f));
            STATIC_CHECK(sf::degrees(900) - sf::degrees(1) == sf::degrees(899));
        }

        SECTION("operator-=")
        {
            sf::Angle angle = sf::degrees(15);
            angle -= sf::degrees(15);
            CHECK(angle == sf::degrees(0));
            angle -= sf::radians(10);
            CHECK(angle == sf::radians(-10));
        }

        SECTION("operator*")
        {
            STATIC_CHECK(sf::radians(0) * 10 == sf::Angle::Zero);
            STATIC_CHECK(sf::degrees(10) * 2.5f == sf::degrees(25));
            STATIC_CHECK(sf::degrees(100) * 10.0f == sf::degrees(1000));

            STATIC_CHECK(10 * sf::radians(0) == sf::Angle::Zero);
            STATIC_CHECK(2.5f * sf::degrees(10) == sf::degrees(25));
            STATIC_CHECK(10.0f * sf::degrees(100) == sf::degrees(1000));
        }

        SECTION("operator*=")
        {
            sf::Angle angle = sf::degrees(1);
            angle *= 10;
            CHECK(angle == sf::degrees(10));
        }

        SECTION("operator/")
        {
            STATIC_CHECK(sf::Angle::Zero / 10 == sf::Angle::Zero);
            STATIC_CHECK(sf::degrees(10) / 2.5f == sf::degrees(4));
            STATIC_CHECK(sf::radians(12) / 3 == sf::radians(4));

            STATIC_CHECK(sf::Angle::Zero / sf::degrees(1) == 0.f);
            STATIC_CHECK(sf::degrees(10) / sf::degrees(10) == 1.f);
            CHECK(sf::radians(10) / sf::radians(2) == Approx(5.f));
        }

        SECTION("operator/=")
        {
            sf::Angle angle = sf::degrees(60);
            angle /= 5;
            CHECK(angle == sf::degrees(12));
        }

        SECTION("operator%")
        {
            STATIC_CHECK(sf::Angle::Zero % sf::radians(0.5f) == sf::Angle::Zero);
            STATIC_CHECK(sf::radians(10) % sf::radians(1) == sf::radians(0));
            STATIC_CHECK(sf::degrees(90) % sf::degrees(30) == sf::degrees(0));
            STATIC_CHECK(sf::degrees(90) % sf::degrees(40) == sf::degrees(10));
            STATIC_CHECK(sf::degrees(-90) % sf::degrees(30) == sf::degrees(0));
            STATIC_CHECK(sf::degrees(-90) % sf::degrees(40) == sf::degrees(30));
        }

        SECTION("operator%=")
        {
            sf::Angle angle = sf::degrees(59);
            angle %= sf::degrees(10);
            CHECK(angle == sf::degrees(9));
        }

        SECTION("operator _deg")
        {
            using namespace sf::Literals;
            STATIC_CHECK(0.0_deg == sf::Angle::Zero);
            STATIC_CHECK(1.0_deg == sf::degrees(1));
            STATIC_CHECK(-1.0_deg == sf::degrees(-1));
            STATIC_CHECK(3.14_deg == sf::degrees(3.14f));
            STATIC_CHECK(-3.14_deg == sf::degrees(-3.14f));

            STATIC_CHECK(0_deg == sf::Angle::Zero);
            STATIC_CHECK(1_deg == sf::degrees(1));
            STATIC_CHECK(-1_deg == sf::degrees(-1));
            STATIC_CHECK(100_deg == sf::degrees(100));
            STATIC_CHECK(-100_deg == sf::degrees(-100));
        }

        SECTION("operator _rad")
        {
            using namespace sf::Literals;
            STATIC_CHECK(0.0_rad == sf::Angle::Zero);
            STATIC_CHECK(1.0_rad == sf::radians(1));
            STATIC_CHECK(-1.0_rad == sf::radians(-1));
            STATIC_CHECK(3.14_rad == sf::radians(3.14f));
            STATIC_CHECK(-3.14_rad == sf::radians(-3.14f));

            STATIC_CHECK(0_rad == sf::Angle::Zero);
            STATIC_CHECK(1_rad == sf::radians(1));
            STATIC_CHECK(-1_rad == sf::radians(-1));
            STATIC_CHECK(100_rad == sf::radians(100));
            STATIC_CHECK(-100_rad == sf::radians(-100));
        }
    }
}

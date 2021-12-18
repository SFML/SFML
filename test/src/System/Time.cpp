#include <SFML/System/Time.hpp>

#include <catch2/catch.hpp>

namespace sf
{
std::ostream& operator <<(std::ostream& os, const sf::Time& time)
{
    os << time.asMicroseconds() << "us";
    return os;
}
}

TEST_CASE("sf::Time class", "[system]")
{
    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            const sf::Time time;
            CHECK(time.asSeconds() == 0.0f);
            CHECK(time.asMilliseconds() == 0);
            CHECK(time.asMicroseconds() == 0);
        }

        SECTION("Construct from seconds")
        {
            const sf::Time time = sf::seconds(123);
            CHECK(time.asSeconds() == 123.0f);
            CHECK(time.asMilliseconds() == 123'000);
            CHECK(time.asMicroseconds() == 123'000'000);

            CHECK(sf::seconds(1'000.0f).asMicroseconds() == 1'000'000'000);
            CHECK(sf::seconds(0.000002f).asMicroseconds() == Approx(2).margin(1));
            CHECK(sf::seconds(0.000001f).asMicroseconds() == Approx(1).margin(1));
            CHECK(sf::seconds(0.0000009f).asMicroseconds() == 0);
            CHECK(sf::seconds(0.0000001f).asMicroseconds() == 0);
            CHECK(sf::seconds(0.00000001f).asMicroseconds() == 0);
            CHECK(sf::seconds(0.000000001f).asMicroseconds() == 0);
            CHECK(sf::seconds(-0.000000001f).asMicroseconds() == 0);
            CHECK(sf::seconds(-0.00000001f).asMicroseconds() == 0);
            CHECK(sf::seconds(-0.0000001f).asMicroseconds() == 0);
            CHECK(sf::seconds(-0.0000009f).asMicroseconds() == 0);
            CHECK(sf::seconds(-0.000001f).asMicroseconds() == Approx(-1).margin(1));
            CHECK(sf::seconds(-0.000002f).asMicroseconds() == Approx(-2).margin(1));
            CHECK(sf::seconds(-1'000.0f).asMicroseconds() == -1'000'000'000);
        }

        SECTION("Construct from milliseconds")
        {
            const sf::Time time = sf::milliseconds(42);
            CHECK(time.asSeconds() == 0.042f);
            CHECK(time.asMilliseconds() == 42);
            CHECK(time.asMicroseconds() == 42'000);
        }

        SECTION("Construct from microseconds")
        {
            const sf::Time time = sf::microseconds(987654);
            CHECK(time.asSeconds() == 0.987654f);
            CHECK(time.asMilliseconds() == 987);
            CHECK(time.asMicroseconds() == 987'654);
        }
    }

    SECTION("Zero time")
    {
        CHECK(sf::Time::Zero.asSeconds() == 0.0f);
        CHECK(sf::Time::Zero.asMilliseconds() == 0);
        CHECK(sf::Time::Zero.asMicroseconds() == 0);
    }

    SECTION("Operators")
    {
        SECTION("operator==")
        {
            CHECK(sf::Time() == sf::Time());
            CHECK(sf::seconds(1) == sf::seconds(1));
            CHECK(sf::seconds(10) == sf::milliseconds(10'000));
            CHECK(sf::milliseconds(450'450) == sf::microseconds(450'450'000));
            CHECK(sf::seconds(0.5f) == sf::microseconds(500'000));
        }

        SECTION("operator!=")
        {
            CHECK(sf::seconds(10.12f) != sf::milliseconds(10'121));
            CHECK(sf::microseconds(123'456) != sf::milliseconds(123));
        }

        SECTION("operator<")
        {
            CHECK(sf::seconds(54.999f) < sf::seconds(55));
            CHECK(sf::microseconds(10) < sf::milliseconds(10));
            CHECK(sf::milliseconds(1'000) < sf::microseconds(1'000'001));
        }

        SECTION("operator>")
        {
            CHECK(sf::seconds(55.001f) > sf::seconds(55));
            CHECK(sf::microseconds(1) > sf::seconds(0.0000001f));
            CHECK(sf::microseconds(1'000'001) > sf::milliseconds(1'000));
        }

        SECTION("operator<=")
        {
            CHECK(sf::milliseconds(100) <= sf::milliseconds(100));
            CHECK(sf::seconds(0.0012f) <= sf::microseconds(1'201));
        }

        SECTION("operator>=")
        {
            CHECK(sf::milliseconds(100) >= sf::milliseconds(-100));
            CHECK(sf::microseconds(1'201) >= sf::seconds(0.0012f));
        }

        SECTION("operator-")
        {
            CHECK(sf::seconds(-1) == -sf::seconds(1));
            CHECK(sf::microseconds(1'234) == -sf::microseconds(-1'234));
        }

        SECTION("operator+")
        {
            CHECK(sf::seconds(1) + sf::seconds(1) == sf::seconds(2));
            CHECK(sf::milliseconds(400) + sf::microseconds(400) == sf::microseconds(400400));
        }

        SECTION("operator+=")
        {
            sf::Time time = sf::seconds(1.5f);
            time += sf::seconds(1);
            CHECK(time == sf::seconds(2.5f));
        }

        SECTION("operator-")
        {
            CHECK(sf::seconds(1) - sf::seconds(1) == sf::seconds(0));
            CHECK(sf::milliseconds(400) - sf::microseconds(400) == sf::microseconds(399600));
        }

        SECTION("operator-=")
        {
            sf::Time time = sf::seconds(1.5f);
            time -= sf::seconds(10);
            CHECK(time == sf::seconds(-8.5f));
        }

        SECTION("operator*")
        {
            CHECK(sf::seconds(1) * 2.0f == sf::seconds(2));
            CHECK(sf::seconds(12) * 0.5f == sf::seconds(6));
            CHECK(sf::seconds(1) * static_cast<sf::Int64>(2) == sf::seconds(2));
            CHECK(sf::seconds(42) * static_cast<sf::Int64>(2) == sf::seconds(84));
            CHECK(2.0f * sf::seconds(1) == sf::seconds(2));
            CHECK(0.5f * sf::seconds(12) == sf::seconds(6));
            CHECK(static_cast<sf::Int64>(2) * sf::seconds(1) == sf::seconds(2));
            CHECK(static_cast<sf::Int64>(2) * sf::seconds(42) == sf::seconds(84));
        }

        SECTION("operator*=")
        {
            sf::Time time = sf::milliseconds(420);
            time *= static_cast<sf::Int64>(10);
            CHECK(time == sf::milliseconds(4'200));
            time *= 0.1f;
            CHECK(time.asMicroseconds() == Approx(420'000).margin(1));
        }

        SECTION("operator/")
        {
            CHECK(sf::seconds(1) / 2.0f == sf::seconds(0.5f));
            CHECK(sf::seconds(12) / 0.5f == sf::seconds(24));
            CHECK(sf::seconds(1) / static_cast<sf::Int64>(2) == sf::seconds(0.5f));
            CHECK(sf::seconds(42) / static_cast<sf::Int64>(2) == sf::seconds(21));
            CHECK(sf::seconds(1) / sf::seconds(1) == 1.0f);
            CHECK(sf::milliseconds(10) / sf::microseconds(1) == Approx(10'000.0f).epsilon(1e-6f));
        }

        SECTION("operator/=")
        {
            sf::Time time = sf::milliseconds(420);
            time /= static_cast<sf::Int64>(42);
            CHECK(time == sf::milliseconds(10));
            time /= 10.0f;
            CHECK(time.asMicroseconds() == Approx(1'000).margin(1));
        }

        SECTION("operator%")
        {
            CHECK(sf::seconds(10) % sf::seconds(3) == sf::seconds(1));
            CHECK(sf::milliseconds(100) % sf::microseconds(10) == sf::seconds(0));
        }

        SECTION("operator%=")
        {
            sf::Time time = sf::milliseconds(100);
            time %= sf::milliseconds(99);
            CHECK(time == sf::milliseconds(1));
        }
    }
}

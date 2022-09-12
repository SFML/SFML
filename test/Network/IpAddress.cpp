#include <SFML/Network/IpAddress.hpp>

#include <doctest/doctest.h>

#include <sstream>
#include <string_view>

using namespace std::string_literals;
using namespace std::string_view_literals;

TEST_CASE("sf::IpAddress class - [network]")
{
    SUBCASE("Construction")
    {
        SUBCASE("static 'create' function")
        {
            const auto ipAddress = sf::IpAddress::resolve("192.168.0.1"sv);
            REQUIRE(ipAddress.has_value());
            CHECK(ipAddress->toString() == "192.168.0.1"s);
            CHECK(ipAddress->toInteger() == 0xC0A80001);
            CHECK(*ipAddress != sf::IpAddress::Any);
            CHECK(*ipAddress != sf::IpAddress::Broadcast);
            CHECK(*ipAddress != sf::IpAddress::LocalHost);

            const auto broadcast = sf::IpAddress::resolve("255.255.255.255"sv);
            REQUIRE(broadcast.has_value());
            CHECK(broadcast->toString() == "255.255.255.255"s);
            CHECK(broadcast->toInteger() == 0xFFFFFFFF);
            CHECK(*broadcast == sf::IpAddress::Broadcast);

            const auto any = sf::IpAddress::resolve("0.0.0.0"sv);
            REQUIRE(any.has_value());
            CHECK(any->toString() == "0.0.0.0"s);
            CHECK(any->toInteger() == 0x00000000);
            CHECK(*any == sf::IpAddress::Any);

            const auto localHost = sf::IpAddress::resolve("localhost"s);
            REQUIRE(localHost.has_value());
            CHECK(localHost->toString() == "127.0.0.1"s);
            CHECK(localHost->toInteger() == 0x7F000001);
            CHECK(*localHost == sf::IpAddress::LocalHost);

            CHECK(!sf::IpAddress::resolve("255.255.255.256"s).has_value());
            CHECK(!sf::IpAddress::resolve("").has_value());
        }

        SUBCASE("Byte constructor")
        {
            const sf::IpAddress ipAddress(142, 250, 69, 238);
            CHECK(ipAddress.toString() == "142.250.69.238"s);
            CHECK(ipAddress.toInteger() == 0x8EFA45EE);
        }

        SUBCASE("std::uint32_t constructor")
        {
            const sf::IpAddress ipAddress(0xDEADBEEF);
            CHECK(ipAddress.toString() == "222.173.190.239"s);
            CHECK(ipAddress.toInteger() == 0xDEADBEEF);
        }
    }

    SUBCASE("Static functions")
    {
        SUBCASE("getLocalAddress")
        {
            const std::optional<sf::IpAddress> ipAddress = sf::IpAddress::getLocalAddress();
            REQUIRE(ipAddress.has_value());
            CHECK(ipAddress->toString() != "0.0.0.0");
            CHECK(ipAddress->toInteger() != 0);
        }

        SUBCASE("getPublicAddress")
        {
            const std::optional<sf::IpAddress> ipAddress = sf::IpAddress::getPublicAddress(sf::seconds(1));
            if (ipAddress.has_value())
            {
                CHECK(ipAddress->toString() != "0.0.0.0");
                CHECK(ipAddress->toInteger() != 0);
            }
        }
    }

    SUBCASE("Static constants")
    {
        CHECK(sf::IpAddress::Any.toString() == "0.0.0.0"s);
        CHECK(sf::IpAddress::Any.toInteger() == 0);

        CHECK(sf::IpAddress::LocalHost.toString() == "127.0.0.1"s);
        CHECK(sf::IpAddress::LocalHost.toInteger() == 0x7F000001);

        CHECK(sf::IpAddress::Broadcast.toString() == "255.255.255.255"s);
        CHECK(sf::IpAddress::Broadcast.toInteger() == 0xFFFFFFFF);
    }

    SUBCASE("Operators")
    {
        SUBCASE("operator==")
        {
            CHECK(sf::IpAddress(0x42, 0x69, 0x96, 0x24) == sf::IpAddress(0x42699624));
            CHECK(sf::IpAddress(0xABCDEF01) == sf::IpAddress(171, 205, 239, 1));
        }

        SUBCASE("operator!=")
        {
            CHECK(sf::IpAddress(0x12344321) != sf::IpAddress(1234));
            CHECK(sf::IpAddress(192, 168, 1, 10) != sf::IpAddress(192, 168, 1, 11));
        }

        SUBCASE("operator<")
        {
            CHECK(sf::IpAddress(1) < sf::IpAddress(2));
            CHECK(sf::IpAddress(0, 0, 0, 0) < sf::IpAddress(1, 0, 0, 0));
            CHECK(sf::IpAddress(1, 0, 0, 0) < sf::IpAddress(0, 1, 0, 0));
            CHECK(sf::IpAddress(0, 1, 0, 0) < sf::IpAddress(0, 0, 1, 0));
            CHECK(sf::IpAddress(0, 0, 1, 0) < sf::IpAddress(0, 0, 0, 1));
            CHECK(sf::IpAddress(0, 0, 0, 1) < sf::IpAddress(1, 0, 0, 1));
        }

        SUBCASE("operator>")
        {
            CHECK(sf::IpAddress(2) > sf::IpAddress(1));
            CHECK(sf::IpAddress(1, 0, 0, 0) > sf::IpAddress(0, 0, 0, 0));
            CHECK(sf::IpAddress(0, 1, 0, 0) > sf::IpAddress(1, 0, 0, 0));
            CHECK(sf::IpAddress(0, 0, 1, 0) > sf::IpAddress(0, 1, 0, 0));
            CHECK(sf::IpAddress(0, 0, 0, 1) > sf::IpAddress(0, 0, 1, 0));
            CHECK(sf::IpAddress(1, 0, 0, 1) > sf::IpAddress(0, 0, 0, 1));
        }

        SUBCASE("operator<=")
        {
            CHECK(sf::IpAddress(1) <= sf::IpAddress(2));
            CHECK(sf::IpAddress(0, 0, 0, 0) <= sf::IpAddress(1, 0, 0, 0));
            CHECK(sf::IpAddress(1, 0, 0, 0) <= sf::IpAddress(0, 1, 0, 0));
            CHECK(sf::IpAddress(0, 1, 0, 0) <= sf::IpAddress(0, 0, 1, 0));
            CHECK(sf::IpAddress(0, 0, 1, 0) <= sf::IpAddress(0, 0, 0, 1));
            CHECK(sf::IpAddress(0, 0, 0, 1) <= sf::IpAddress(1, 0, 0, 1));

            CHECK(sf::IpAddress(0x42, 0x69, 0x96, 0x24) <= sf::IpAddress(0x42699624));
            CHECK(sf::IpAddress(0xABCDEF01) <= sf::IpAddress(171, 205, 239, 1));
        }

        SUBCASE("operator>=")
        {
            CHECK(sf::IpAddress(2) >= sf::IpAddress(1));
            CHECK(sf::IpAddress(1, 0, 0, 0) >= sf::IpAddress(0, 0, 0, 0));
            CHECK(sf::IpAddress(0, 1, 0, 0) >= sf::IpAddress(1, 0, 0, 0));
            CHECK(sf::IpAddress(0, 0, 1, 0) >= sf::IpAddress(0, 1, 0, 0));
            CHECK(sf::IpAddress(0, 0, 0, 1) >= sf::IpAddress(0, 0, 1, 0));
            CHECK(sf::IpAddress(1, 0, 0, 1) >= sf::IpAddress(0, 0, 0, 1));

            CHECK(sf::IpAddress(0x42, 0x69, 0x96, 0x24) >= sf::IpAddress(0x42699624));
            CHECK(sf::IpAddress(0xABCDEF01) >= sf::IpAddress(171, 205, 239, 1));
        }

        SUBCASE("operator>>")
        {
            std::optional<sf::IpAddress> ipAddress;
            std::istringstream("4.4.4.4") >> ipAddress;
            REQUIRE(ipAddress.has_value());
            CHECK(ipAddress->toString() == "4.4.4.4"s);
            CHECK(ipAddress->toInteger() == 0x04040404);

            std::istringstream("92.100.0.72") >> ipAddress;
            REQUIRE(ipAddress.has_value());
            CHECK(ipAddress->toString() == "92.100.0.72"s);
            CHECK(ipAddress->toInteger() == 0x5C640048);

            std::istringstream("") >> ipAddress;
            CHECK(!ipAddress.has_value());
        }

        SUBCASE("operator<<")
        {
            std::ostringstream out;
            out << sf::IpAddress(10, 9, 8, 7);
            CHECK(out.str() == "10.9.8.7"s);
        }
    }
}

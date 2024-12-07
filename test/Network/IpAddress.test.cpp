#include <SFML/Network/IpAddress.hpp>

#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <string_view>
#include <type_traits>

using namespace std::string_literals;
using namespace std::string_view_literals;

TEST_CASE("[Network] sf::IpAddress")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::IpAddress>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::IpAddress>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::IpAddress>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::IpAddress>);
        STATIC_CHECK(std::is_trivially_copyable_v<sf::IpAddress>);
    }

    SECTION("Construction")
    {
        SECTION("static 'create' function")
        {
            const auto ipAddress = sf::IpAddress::resolve("203.0.113.2"sv);
            REQUIRE(ipAddress.has_value());
            CHECK(ipAddress->toString() == "203.0.113.2"s);
            CHECK(ipAddress->toInteger() == 0xCB007102);
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

        SECTION("Byte constructor")
        {
            const sf::IpAddress ipAddress(198, 51, 100, 234);
            CHECK(ipAddress.toString() == "198.51.100.234"s);
            CHECK(ipAddress.toInteger() == 0xC63364EA);
        }

        SECTION("std::uint32_t constructor")
        {
            const sf::IpAddress ipAddress(0xCB00719A);
            CHECK(ipAddress.toString() == "203.0.113.154"s);
            CHECK(ipAddress.toInteger() == 0xCB00719A);
        }
    }

    SECTION("Static functions")
    {
        SECTION("getLocalAddress")
        {
            const std::optional<sf::IpAddress> ipAddress = sf::IpAddress::getLocalAddress();
            REQUIRE(ipAddress.has_value());
            CHECK(ipAddress->toString() != "0.0.0.0");
            CHECK(ipAddress->toInteger() != 0);
        }

        SECTION("getPublicAddress")
        {
            const std::optional<sf::IpAddress> ipAddress = sf::IpAddress::getPublicAddress(sf::seconds(1));
            if (ipAddress.has_value())
            {
                CHECK(ipAddress->toString() != "0.0.0.0");
                CHECK(ipAddress->toInteger() != 0);
            }
        }
    }

    SECTION("Static constants")
    {
        CHECK(sf::IpAddress::Any.toString() == "0.0.0.0"s);
        CHECK(sf::IpAddress::Any.toInteger() == 0);

        CHECK(sf::IpAddress::LocalHost.toString() == "127.0.0.1"s);
        CHECK(sf::IpAddress::LocalHost.toInteger() == 0x7F000001);

        CHECK(sf::IpAddress::Broadcast.toString() == "255.255.255.255"s);
        CHECK(sf::IpAddress::Broadcast.toInteger() == 0xFFFFFFFF);
    }

    SECTION("Operators")
    {
        SECTION("operator==")
        {
            CHECK(sf::IpAddress(0xC6, 0x33, 0x64, 0x7B) == sf::IpAddress(0xC633647B));
            CHECK(sf::IpAddress(0xCB0071D2) == sf::IpAddress(203, 0, 113, 210));
        }

        SECTION("operator!=")
        {
            CHECK(sf::IpAddress(0x12344321) != sf::IpAddress(1234));
            CHECK(sf::IpAddress(198, 51, 100, 1) != sf::IpAddress(198, 51, 100, 11));
        }

        SECTION("operator<")
        {
            CHECK(sf::IpAddress(1) < sf::IpAddress(2));
            CHECK(sf::IpAddress(0, 0, 0, 0) < sf::IpAddress(1, 0, 0, 0));
            CHECK(sf::IpAddress(0, 1, 0, 0) < sf::IpAddress(1, 0, 0, 0));
            CHECK(sf::IpAddress(0, 0, 1, 0) < sf::IpAddress(0, 1, 0, 0));
            CHECK(sf::IpAddress(0, 0, 0, 1) < sf::IpAddress(0, 0, 1, 0));
            CHECK(sf::IpAddress(0, 0, 0, 1) < sf::IpAddress(1, 0, 0, 1));
        }

        SECTION("operator>")
        {
            CHECK(sf::IpAddress(2) > sf::IpAddress(1));
            CHECK(sf::IpAddress(1, 0, 0, 0) > sf::IpAddress(0, 0, 0, 0));
            CHECK(sf::IpAddress(1, 0, 0, 0) > sf::IpAddress(0, 1, 0, 0));
            CHECK(sf::IpAddress(0, 1, 0, 0) > sf::IpAddress(0, 0, 1, 0));
            CHECK(sf::IpAddress(0, 0, 1, 0) > sf::IpAddress(0, 0, 0, 1));
            CHECK(sf::IpAddress(1, 0, 0, 1) > sf::IpAddress(0, 0, 0, 1));
        }

        SECTION("operator<=")
        {
            CHECK(sf::IpAddress(1) <= sf::IpAddress(2));
            CHECK(sf::IpAddress(0, 0, 0, 0) <= sf::IpAddress(1, 0, 0, 0));
            CHECK(sf::IpAddress(0, 1, 0, 0) <= sf::IpAddress(1, 0, 0, 0));
            CHECK(sf::IpAddress(0, 0, 1, 0) <= sf::IpAddress(0, 1, 0, 0));
            CHECK(sf::IpAddress(0, 0, 0, 1) <= sf::IpAddress(0, 0, 1, 0));
            CHECK(sf::IpAddress(0, 0, 0, 1) <= sf::IpAddress(1, 0, 0, 1));

            CHECK(sf::IpAddress(0xC6, 0x33, 0x64, 0x7B) <= sf::IpAddress(0xC633647B));
            CHECK(sf::IpAddress(0xCB0071D2) <= sf::IpAddress(203, 0, 113, 210));
        }

        SECTION("operator>=")
        {
            CHECK(sf::IpAddress(2) >= sf::IpAddress(1));
            CHECK(sf::IpAddress(1, 0, 0, 0) >= sf::IpAddress(0, 0, 0, 0));
            CHECK(sf::IpAddress(1, 0, 0, 0) >= sf::IpAddress(0, 1, 0, 0));
            CHECK(sf::IpAddress(0, 1, 0, 0) >= sf::IpAddress(0, 0, 1, 0));
            CHECK(sf::IpAddress(0, 0, 1, 0) >= sf::IpAddress(0, 0, 0, 1));
            CHECK(sf::IpAddress(1, 0, 0, 1) >= sf::IpAddress(0, 0, 0, 1));

            CHECK(sf::IpAddress(0xC6, 0x33, 0x64, 0x7B) >= sf::IpAddress(0xC633647B));
            CHECK(sf::IpAddress(0xCB0071D2) >= sf::IpAddress(203, 0, 113, 210));
        }

        SECTION("operator>>")
        {
            std::optional<sf::IpAddress> ipAddress;
            std::istringstream("198.51.100.4") >> ipAddress;
            REQUIRE(ipAddress.has_value());
            CHECK(ipAddress->toString() == "198.51.100.4"s);
            CHECK(ipAddress->toInteger() == 0xC6336404);

            std::istringstream("203.0.113.72") >> ipAddress;
            REQUIRE(ipAddress.has_value());
            CHECK(ipAddress->toString() == "203.0.113.72"s);
            CHECK(ipAddress->toInteger() == 0xCB007148);

            std::istringstream("") >> ipAddress;
            CHECK(!ipAddress.has_value());
        }

        SECTION("operator<<")
        {
            std::ostringstream out;
            out << sf::IpAddress(192, 0, 2, 10);
            CHECK(out.str() == "192.0.2.10"s);
        }
    }
}

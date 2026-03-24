#include <SFML/Network/IpAddress.hpp>

#include <catch2/catch_test_macros.hpp>

#include <NetworkUtil.hpp>
#include <sstream>
#include <string_view>
#include <type_traits>

// Allow testing deprecated functions
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#else
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

using namespace std::string_literals;
using namespace std::string_view_literals;

TEST_CASE("[Network] sf::IpAddress")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_trivially_copy_constructible_v<sf::IpAddress>);
        STATIC_CHECK(std::is_trivially_copy_assignable_v<sf::IpAddress>);
        STATIC_CHECK(std::is_trivially_move_constructible_v<sf::IpAddress>);
        STATIC_CHECK(std::is_trivially_move_assignable_v<sf::IpAddress>);
        STATIC_CHECK(std::is_trivially_copyable_v<sf::IpAddress>);
    }

    SECTION("Backwards compatibility")
    {
        SECTION("static 'resolve' function")
        {
            const auto ipAddress = sf::IpAddress::resolve("203.0.113.2"sv);
            REQUIRE(ipAddress.has_value());
            CHECK(ipAddress->toString() == "203.0.113.2"s);
            CHECK(ipAddress->toInteger() == 0xCB'00'71'02);
            CHECK(*ipAddress != sf::IpAddress::Any);
            CHECK(*ipAddress != sf::IpAddress::Broadcast);
            CHECK(*ipAddress != sf::IpAddress::LocalHost);

            const auto broadcast = sf::IpAddress::resolve("255.255.255.255"sv);
            REQUIRE(broadcast.has_value());
            CHECK(broadcast->toString() == "255.255.255.255"s);
            CHECK(broadcast->toInteger() == 0xFF'FF'FF'FF);
            CHECK(*broadcast == sf::IpAddress::Broadcast);

            const auto any = sf::IpAddress::resolve("0.0.0.0"sv);
            REQUIRE(any.has_value());
            CHECK(any->toString() == "0.0.0.0"s);
            CHECK(any->toInteger() == 0x00'00'00'00);
            CHECK(*any == sf::IpAddress::Any);

            const auto localHost = sf::IpAddress::resolve("localhost"s);
            REQUIRE(localHost.has_value());
            CHECK(localHost->toString() == "127.0.0.1"s);
            CHECK(localHost->toInteger() == 0x7F'00'00'01);
            CHECK(*localHost == sf::IpAddress::LocalHost);

            CHECK_FALSE(sf::IpAddress::resolve("255.255.255.256"s).has_value());
            CHECK_FALSE(sf::IpAddress::resolve("").has_value());
        }

        SECTION("Constants")
        {
            CHECK(sf::IpAddress::Any == sf::IpAddress::AnyV4);
            CHECK(sf::IpAddress::LocalHost == sf::IpAddress::LocalHostV4);
            CHECK(sf::IpAddress::Broadcast == sf::IpAddress::BroadcastV4);
        }
    }

    SECTION("Construction")
    {
        SECTION("IPv4")
        {
            SECTION("static 'fromString' function")
            {
                const auto ipAddress = sf::IpAddress::fromString("203.0.113.2"sv);
                REQUIRE(ipAddress.has_value());
                CHECK(ipAddress->isV4());
                CHECK_FALSE(ipAddress->isV6());
                CHECK(ipAddress->toString() == "203.0.113.2"s);
                CHECK(ipAddress->toInteger() == 0xCB'00'71'02);
                CHECK(*ipAddress != sf::IpAddress::AnyV4);
                CHECK(*ipAddress != sf::IpAddress::BroadcastV4);
                CHECK(*ipAddress != sf::IpAddress::LocalHostV4);

                const auto broadcast = sf::IpAddress::fromString("255.255.255.255"sv);
                REQUIRE(broadcast.has_value());
                CHECK(broadcast->isV4());
                CHECK_FALSE(broadcast->isV6());
                CHECK(broadcast->toString() == "255.255.255.255"s);
                CHECK(broadcast->toInteger() == 0xFF'FF'FF'FF);
                CHECK(*broadcast == sf::IpAddress::BroadcastV4);

                const auto any = sf::IpAddress::fromString("0.0.0.0"sv);
                REQUIRE(any.has_value());
                CHECK(any->isV4());
                CHECK_FALSE(any->isV6());
                CHECK(any->toString() == "0.0.0.0"s);
                CHECK(any->toInteger() == 0x00'00'00'00);
                CHECK(*any == sf::IpAddress::Any);

                CHECK_FALSE(sf::IpAddress::fromString("255.255.255.256"s).has_value());
                CHECK_FALSE(sf::IpAddress::fromString("").has_value());
            }

            SECTION("Byte constructor")
            {
                const sf::IpAddress ipAddress(198, 51, 100, 234);
                CHECK(ipAddress.isV4());
                CHECK_FALSE(ipAddress.isV6());
                CHECK(ipAddress.toString() == "198.51.100.234"s);
                CHECK(ipAddress.toInteger() == 0xC6'33'64'EA);
            }

            SECTION("std::uint32_t constructor")
            {
                const sf::IpAddress ipAddress(0xCB'00'71'9A);
                CHECK(ipAddress.isV4());
                CHECK_FALSE(ipAddress.isV6());
                CHECK(ipAddress.toString() == "203.0.113.154"s);
                CHECK(ipAddress.toInteger() == 0xCB'00'71'9A);
            }
        }

        SECTION("IPv6")
        {
            SECTION("static 'fromString' function")
            {
                const auto ipAddress = sf::IpAddress::fromString("2600:1901:0:13e0::1"sv);
                REQUIRE(ipAddress.has_value());
                CHECK_FALSE(ipAddress->isV4());
                CHECK(ipAddress->isV6());
                CHECK(ipAddress->toString() == "2600:1901:0:13e0::1"s);
                CHECK(ipAddress->toBytes() ==
                      std::array<std::uint8_t,
                                 16>{0x26, 0x00, 0x19, 0x01, 0x00, 0x00, 0x13, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01});
                CHECK(*ipAddress != sf::IpAddress::AnyV6);
                CHECK(*ipAddress != sf::IpAddress::LocalHostV6);

                const auto any = sf::IpAddress::fromString("::"sv);
                REQUIRE(any.has_value());
                CHECK_FALSE(any->isV4());
                CHECK(any->isV6());
                CHECK(any->toString() == "::"s);
                CHECK(any->toBytes() ==
                      std::array<std::uint8_t,
                                 16>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
                CHECK(*any == sf::IpAddress::AnyV6);

                CHECK_FALSE(sf::IpAddress::fromString("1:::1"s).has_value());
                CHECK_FALSE(sf::IpAddress::fromString("1:1"s).has_value());
                CHECK_FALSE(sf::IpAddress::fromString("1::1::1"s).has_value());
                CHECK_FALSE(sf::IpAddress::fromString("ghij:klmn:opqr:stuv::wxzy"s).has_value());
                CHECK_FALSE(sf::IpAddress::fromString("").has_value());
            }

            SECTION("Byte constructor")
            {
                const sf::IpAddress ipAddress(
                    {0x27, 0x00, 0x19, 0x01, 0x00, 0x00, 0x13, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02});
                CHECK_FALSE(ipAddress.isV4());
                CHECK(ipAddress.isV6());
                CHECK(ipAddress.toString() == "2700:1901:0:13e0::2"s);
                CHECK(ipAddress.toBytes() ==
                      std::array<std::uint8_t,
                                 16>{0x27, 0x00, 0x19, 0x01, 0x00, 0x00, 0x13, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02});
            }
        }
    }

    SECTION("Static constants")
    {
        SECTION("IPv4")
        {
            CHECK(sf::IpAddress::AnyV4.toString() == "0.0.0.0"s);
            CHECK(sf::IpAddress::AnyV4.toInteger() == 0);

            CHECK(sf::IpAddress::LocalHostV4.toString() == "127.0.0.1"s);
            CHECK(sf::IpAddress::LocalHostV4.toInteger() == 0x7F'00'00'01);

            CHECK(sf::IpAddress::BroadcastV4.toString() == "255.255.255.255"s);
            CHECK(sf::IpAddress::BroadcastV4.toInteger() == 0xFF'FF'FF'FF);
        }

        SECTION("IPv6")
        {
            CHECK(sf::IpAddress::AnyV6.toString() == "::"s);
            CHECK(sf::IpAddress::AnyV6.toBytes() ==
                  std::array<std::uint8_t,
                             16>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

            CHECK(sf::IpAddress::LocalHostV6.toString() == "::1"s);
            CHECK(sf::IpAddress::LocalHostV6.toBytes() ==
                  std::array<std::uint8_t,
                             16>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01});
        }
    }

    SECTION("Operators")
    {
        SECTION("IPv4")
        {
            SECTION("operator==")
            {
                CHECK(sf::IpAddress(0xC6, 0x33, 0x64, 0x7B) == sf::IpAddress(0xC6'33'64'7B));
                CHECK(sf::IpAddress(0xCB'00'71'D2) == sf::IpAddress(203, 0, 113, 210));
            }

            SECTION("operator!=")
            {
                CHECK(sf::IpAddress(0x12'34'43'21) != sf::IpAddress(1234));
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

                CHECK(sf::IpAddress(0xC6, 0x33, 0x64, 0x7B) <= sf::IpAddress(0xC6'33'64'7B));
                CHECK(sf::IpAddress(0xCB'00'71'D2) <= sf::IpAddress(203, 0, 113, 210));
            }

            SECTION("operator>=")
            {
                CHECK(sf::IpAddress(2) >= sf::IpAddress(1));
                CHECK(sf::IpAddress(1, 0, 0, 0) >= sf::IpAddress(0, 0, 0, 0));
                CHECK(sf::IpAddress(1, 0, 0, 0) >= sf::IpAddress(0, 1, 0, 0));
                CHECK(sf::IpAddress(0, 1, 0, 0) >= sf::IpAddress(0, 0, 1, 0));
                CHECK(sf::IpAddress(0, 0, 1, 0) >= sf::IpAddress(0, 0, 0, 1));
                CHECK(sf::IpAddress(1, 0, 0, 1) >= sf::IpAddress(0, 0, 0, 1));

                CHECK(sf::IpAddress(0xC6, 0x33, 0x64, 0x7B) >= sf::IpAddress(0xC6'33'64'7B));
                CHECK(sf::IpAddress(0xCB'00'71'D2) >= sf::IpAddress(203, 0, 113, 210));
            }

            SECTION("operator>>")
            {
                std::optional<sf::IpAddress> ipAddress;
                std::istringstream("198.51.100.4") >> ipAddress;
                REQUIRE(ipAddress.has_value());
                CHECK(ipAddress->isV4());
                CHECK_FALSE(ipAddress->isV6());
                CHECK(ipAddress->toString() == "198.51.100.4"s);
                CHECK(ipAddress->toInteger() == 0xC6'33'64'04);

                std::istringstream("203.0.113.72") >> ipAddress;
                REQUIRE(ipAddress.has_value());
                CHECK(ipAddress->isV4());
                CHECK_FALSE(ipAddress->isV6());
                CHECK(ipAddress->toString() == "203.0.113.72"s);
                CHECK(ipAddress->toInteger() == 0xCB'00'71'48);

                std::istringstream("") >> ipAddress;
                CHECK_FALSE(ipAddress.has_value());
            }

            SECTION("operator<<")
            {
                std::ostringstream out;
                out << sf::IpAddress(192, 0, 2, 10);
                CHECK(out.str() == "192.0.2.10"s);
            }
        }

        SECTION("IPv6")
        {
            SECTION("operator==")
            {
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f}) ==
                    sf::IpAddress(
                        {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f}));
            }

            SECTION("operator!=")
            {
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f}) !=
                    sf::IpAddress(
                        {0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00}));
            }

            SECTION("operator<")
            {
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}) <
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}));
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) <
                    sf::IpAddress(
                        {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) <
                    sf::IpAddress(
                        {0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) <
                    sf::IpAddress(
                        {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) <
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}) <
                    sf::IpAddress(
                        {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}));
            }

            SECTION("operator>")
            {
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}) >
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}));
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}) >
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
                CHECK(
                    sf::IpAddress(
                        {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) >
                    sf::IpAddress(
                        {0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) >
                    sf::IpAddress(
                        {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) >
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
                CHECK(
                    sf::IpAddress(
                        {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}) >
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}));
            }

            SECTION("operator<=")
            {
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}) <=
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}));
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) <=
                    sf::IpAddress(
                        {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) <=
                    sf::IpAddress(
                        {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) <=
                    sf::IpAddress(
                        {0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) <=
                    sf::IpAddress(
                        {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) <=
                    sf::IpAddress(
                        {0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));

                CHECK(
                    sf::IpAddress(
                        {0x27, 0x00, 0x19, 0x01, 0x00, 0x00, 0x13, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}) <=
                    sf::IpAddress(
                        {0x27, 0x00, 0x19, 0x01, 0x00, 0x00, 0x13, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}));
            }

            SECTION("operator>=")
            {
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}) >=
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01}));
                CHECK(
                    sf::IpAddress(
                        {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) >=
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
                CHECK(
                    sf::IpAddress(
                        {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) >=
                    sf::IpAddress(
                        {0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) >=
                    sf::IpAddress(
                        {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
                CHECK(
                    sf::IpAddress(
                        {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) >=
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));
                CHECK(
                    sf::IpAddress(
                        {0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}) >=
                    sf::IpAddress(
                        {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}));

                CHECK(
                    sf::IpAddress(
                        {0x27, 0x00, 0x19, 0x01, 0x00, 0x00, 0x13, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}) >=
                    sf::IpAddress(
                        {0x27, 0x00, 0x19, 0x01, 0x00, 0x00, 0x13, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}));
            }

            SECTION("operator>>")
            {
                std::optional<sf::IpAddress> ipAddress;
                std::istringstream("2600:1901:0:13e0::1") >> ipAddress;
                REQUIRE(ipAddress.has_value());
                CHECK_FALSE(ipAddress->isV4());
                CHECK(ipAddress->isV6());
                CHECK(ipAddress->toString() == "2600:1901:0:13e0::1"s);
                CHECK(ipAddress->toBytes() ==
                      std::array<std::uint8_t,
                                 16>{0x26, 0x00, 0x19, 0x01, 0x00, 0x00, 0x13, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01});

                std::istringstream("2a00:1450:4001:831::200e") >> ipAddress;
                REQUIRE(ipAddress.has_value());
                CHECK_FALSE(ipAddress->isV4());
                CHECK(ipAddress->isV6());
                CHECK(ipAddress->toString() == "2a00:1450:4001:831::200e"s);
                CHECK(ipAddress->toBytes() ==
                      std::array<std::uint8_t,
                                 16>{0x2a, 0x00, 0x14, 0x50, 0x40, 0x01, 0x08, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x0e});

                std::istringstream("") >> ipAddress;
                CHECK_FALSE(ipAddress.has_value());
            }

            SECTION("operator<<")
            {
                std::ostringstream out;
                out << sf::IpAddress(
                    {0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc});
                CHECK(out.str() == "fe80::1234:5678:9abc"s);
            }
        }
    }
}

TEST_CASE("[Network] sf::IpAddress Discovery (IPv4)", runIpV4InternetTests())
{
    SECTION("Local address")
    {
        const std::optional<sf::IpAddress> ipAddress = sf::IpAddress::getLocalAddress(sf::IpAddress::Type::IpV4);
        REQUIRE(ipAddress.has_value());
        CHECK(ipAddress->isV4());
        CHECK_FALSE(ipAddress->isV6());
        CHECK(ipAddress->toString() != "0.0.0.0");
        CHECK(ipAddress->toInteger() != 0);
    }

    SECTION("Public address")
    {
        const std::optional<sf::IpAddress> ipAddress = sf::IpAddress::getPublicAddress(sf::seconds(1),
                                                                                       sf::IpAddress::Type::IpV4);
        REQUIRE(ipAddress.has_value());
        CHECK(ipAddress->isV4());
        CHECK_FALSE(ipAddress->isV6());
        CHECK(ipAddress->toString() != "0.0.0.0");
        CHECK(ipAddress->toInteger() != 0);
    }

    SECTION("Public address (secure)")
    {
        const std::optional<sf::IpAddress>
            ipAddress = sf::IpAddress::getPublicAddress(sf::seconds(1), sf::IpAddress::Type::IpV4, true);
        REQUIRE(ipAddress.has_value());
        CHECK(ipAddress->isV4());
        CHECK_FALSE(ipAddress->isV6());
        CHECK(ipAddress->toString() != "0.0.0.0");
        CHECK(ipAddress->toInteger() != 0);
    }
}

TEST_CASE("[Network] sf::IpAddress Discovery (IPv6)", runIpV6InternetTests())
{
    SECTION("Local address")
    {
        const std::optional<sf::IpAddress> ipAddress = sf::IpAddress::getLocalAddress(sf::IpAddress::Type::IpV6);
        REQUIRE(ipAddress.has_value());
        CHECK_FALSE(ipAddress->isV4());
        CHECK(ipAddress->isV6());
        CHECK(ipAddress->toString() != "::");
        CHECK(ipAddress->toBytes() !=
              std::array<std::uint8_t,
                         16>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    }

    SECTION("Public address")
    {
        const std::optional<sf::IpAddress> ipAddress = sf::IpAddress::getPublicAddress(sf::seconds(1),
                                                                                       sf::IpAddress::Type::IpV6);
        REQUIRE(ipAddress.has_value());
        CHECK_FALSE(ipAddress->isV4());
        CHECK(ipAddress->isV6());
        CHECK(ipAddress->toString() != "::");
        CHECK(ipAddress->toBytes() !=
              std::array<std::uint8_t,
                         16>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    }

    SECTION("Public address (secure)")
    {
        const std::optional<sf::IpAddress>
            ipAddress = sf::IpAddress::getPublicAddress(sf::seconds(1), sf::IpAddress::Type::IpV6, true);
        REQUIRE(ipAddress.has_value());
        CHECK_FALSE(ipAddress->isV4());
        CHECK(ipAddress->isV6());
        CHECK(ipAddress->toString() != "::");
        CHECK(ipAddress->toBytes() !=
              std::array<std::uint8_t,
                         16>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    }
}

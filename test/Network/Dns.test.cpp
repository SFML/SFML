#include <SFML/Network/Dns.hpp>

#include <catch2/catch_test_macros.hpp>

#include <NetworkUtil.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;

TEST_CASE("[Network] sf::Dns (IPv4)", runIpV4InternetTests())
{
    SECTION("Well known hosts")
    {
        const auto addresses = resolveV4("localhost");
        REQUIRE_FALSE(addresses.empty());
        CHECK(addresses.front() == sf::IpAddress::LocalHost);
    }

    SECTION("IP address strings")
    {
        const auto addresses = resolveV4("123.123.123.123");
        REQUIRE_FALSE(addresses.empty());
        CHECK(addresses.front() == sf::IpAddress(123, 123, 123, 123));
    }

    SECTION("www.sfml-dev.org")
    {
        const auto addresses = resolveV4("www.sfml-dev.org");
        REQUIRE_FALSE(addresses.empty());
        CHECK(addresses.front() != sf::IpAddress::Any);
        CHECK(addresses.front() != sf::IpAddress::LocalHost);
        CHECK(addresses.front() != sf::IpAddress::Broadcast);
        CHECK_FALSE(addresses.front().toString().empty());
    }

    SECTION("ci.sfml-dev.org")
    {
        const auto addresses = resolveV4("ci.sfml-dev.org");
        REQUIRE_FALSE(addresses.empty());
        CHECK(addresses.front() != sf::IpAddress::Any);
        CHECK(addresses.front() != sf::IpAddress::LocalHost);
        CHECK(addresses.front() != sf::IpAddress::Broadcast);
        CHECK_FALSE(addresses.front().toString().empty());
    }

    SECTION("microsoft.com")
    {
        const auto addresses = resolveV4("microsoft.com");
        REQUIRE_FALSE(addresses.empty());
        CHECK(addresses.front() != sf::IpAddress::Any);
        CHECK(addresses.front() != sf::IpAddress::LocalHost);
        CHECK(addresses.front() != sf::IpAddress::Broadcast);
        CHECK_FALSE(addresses.front().toString().empty());
    }

    SECTION("github.com")
    {
        const auto addresses = resolveV4("github.com");
        REQUIRE_FALSE(addresses.empty());
        CHECK(addresses.front() != sf::IpAddress::Any);
        CHECK(addresses.front() != sf::IpAddress::LocalHost);
        CHECK(addresses.front() != sf::IpAddress::Broadcast);
        CHECK_FALSE(addresses.front().toString().empty());
    }

    SECTION("google.com")
    {
        const auto addresses = resolveV4("google.com");
        REQUIRE_FALSE(addresses.empty());
        CHECK(addresses.front() != sf::IpAddress::Any);
        CHECK(addresses.front() != sf::IpAddress::LocalHost);
        CHECK(addresses.front() != sf::IpAddress::Broadcast);
        CHECK_FALSE(addresses.front().toString().empty());
    }

    SECTION("Public IP address")
    {
        const std::optional<sf::IpAddress> ipAddress = sf::Dns::getPublicAddress(sf::seconds(1), sf::IpAddress::Type::IpV4);
        REQUIRE(ipAddress.has_value());
        CHECK(ipAddress->isV4());
        CHECK_FALSE(ipAddress->isV6());
        CHECK(ipAddress->toString() != "0.0.0.0");
        CHECK(ipAddress->toInteger() != 0);
    }
}

TEST_CASE("[Network] sf::Dns (IPv6)", runIpV6InternetTests())
{
    SECTION("Well known hosts")
    {
        // Some systems only provide an IPv6 address for ip6-localhost
        // Try to resolve both localhost and ip6-localhost and check if one is successful
        const auto addresses0 = resolveV6("localhost");
        const auto addresses1 = resolveV6("ip6-localhost");
        REQUIRE_FALSE((addresses0.empty() && addresses1.empty()));
        if (!addresses0.empty())
            CHECK(addresses0.front() == sf::IpAddress::LocalHostV6);
        if (!addresses1.empty())
            CHECK(addresses1.front() == sf::IpAddress::LocalHostV6);
    }

    SECTION("IP address strings")
    {
        const auto addresses = resolveV6("0001:0203:0405:0607:0809:0A0B:0C0D:0E0F");
        REQUIRE_FALSE(addresses.empty());
        CHECK(addresses.front() ==
              sf::IpAddress(
                  {static_cast<std::uint8_t>(0x0),
                   static_cast<std::uint8_t>(0x1),
                   static_cast<std::uint8_t>(0x2),
                   static_cast<std::uint8_t>(0x3),
                   static_cast<std::uint8_t>(0x4),
                   static_cast<std::uint8_t>(0x5),
                   static_cast<std::uint8_t>(0x6),
                   static_cast<std::uint8_t>(0x7),
                   static_cast<std::uint8_t>(0x8),
                   static_cast<std::uint8_t>(0x9),
                   static_cast<std::uint8_t>(0xA),
                   static_cast<std::uint8_t>(0xB),
                   static_cast<std::uint8_t>(0xC),
                   static_cast<std::uint8_t>(0xD),
                   static_cast<std::uint8_t>(0xE),
                   static_cast<std::uint8_t>(0xF)}));
    }

    SECTION("www.sfml-dev.org")
    {
        // TODO: www.sfml-dev.org currently does not have IPv6 address records
    }

    SECTION("ci.sfml-dev.org")
    {
        const auto addresses = resolveV6("ci.sfml-dev.org");
        REQUIRE_FALSE(addresses.empty());
        CHECK(addresses.front() != sf::IpAddress::AnyV6);
        CHECK(addresses.front() != sf::IpAddress::LocalHostV6);
        CHECK_FALSE(addresses.front().toString().empty());
    }

    SECTION("microsoft.com")
    {
        const auto addresses = resolveV6("microsoft.com");
        REQUIRE_FALSE(addresses.empty());
        CHECK(addresses.front() != sf::IpAddress::AnyV6);
        CHECK(addresses.front() != sf::IpAddress::LocalHostV6);
        CHECK_FALSE(addresses.front().toString().empty());
    }

    SECTION("github.com")
    {
        // GitHub currently does not have IPv6 address records
    }

    SECTION("google.com")
    {
        const auto addresses = resolveV6("google.com");
        REQUIRE_FALSE(addresses.empty());
        CHECK(addresses.front() != sf::IpAddress::AnyV6);
        CHECK(addresses.front() != sf::IpAddress::LocalHostV6);
        CHECK_FALSE(addresses.front().toString().empty());
    }

    SECTION("Public IP address")
    {
        const std::optional<sf::IpAddress> ipAddress = sf::Dns::getPublicAddress(sf::seconds(1), sf::IpAddress::Type::IpV6);
        REQUIRE(ipAddress.has_value());
        CHECK_FALSE(ipAddress->isV4());
        CHECK(ipAddress->isV6());
        CHECK(ipAddress->toString() != "::");
        CHECK(ipAddress->toBytes() !=
              std::array<std::uint8_t,
                         16>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    }
}

TEST_CASE("[Network] sf::Dns (Advanced Queries)", runDnsTestsAdvanced())
{
    SECTION("NS records")
    {
        const auto records = sf::Dns::queryNS("sfml-dev.org");
        REQUIRE_FALSE(records.empty());
        CHECK_FALSE(records.empty());
    }

    SECTION("MX records")
    {
        const auto records = sf::Dns::queryMX("sfml-dev.org");
        REQUIRE_FALSE(records.empty());
        CHECK_FALSE(records.front().exchange.isEmpty());
        CHECK(records.front().preference != 0);
    }

    SECTION("SRV records")
    {
        const auto records = sf::Dns::querySRV("_xmpp-client._tcp.xmpp.org");
        REQUIRE_FALSE(records.empty());
        CHECK_FALSE(records.front().target.isEmpty());
        CHECK(records.front().port != 0);
        CHECK(records.front().weight != 0);
        CHECK(records.front().priority != 0);
    }

    SECTION("TXT records")
    {
        const auto records = sf::Dns::queryTXT("sfml-dev.org");
        REQUIRE_FALSE(records.empty());
        CHECK_FALSE(records.front().isEmpty());
    }
}

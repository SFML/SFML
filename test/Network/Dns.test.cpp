#include <SFML/Network/Dns.hpp>

#include <catch2/catch_test_macros.hpp>

#include <NetworkUtil.hpp>

using namespace std::string_literals;
using namespace std::string_view_literals;

TEST_CASE("[Network] sf::Dns (Invalid hosts)")
{
    // Strings that fail parsing as an IP address will be treated as hostnames
    // and cause a name lookup to be performed if the string satisfies the
    // hostname grammar specified in RFC 952 and RFC 1123
    // According to RFC 1123:
    // Whenever a user inputs the identity of an Internet host, it SHOULD
    // be possible to enter either (1) a host domain name or (2) an IP
    // address in dotted-decimal ("#.#.#.#") form.  The host SHOULD check
    // the string syntactically for a dotted-decimal number before
    // looking it up in the Domain Name System.

    // If we deliberately want the resolution to fail we have to pass strings
    // that are neither valid IP addresses nor valid hostnames

    // In the following a label refers to a sequence of valid non-dot characters between dot characters
    // If no trailing dot character is specified at the end of a hostname one is implied

    SECTION("IPv4")
    {
        // Hostnames are not allowed to be empty
        CHECK(resolveV4(""s).empty());

        // Hostnames are not allowed to contain spaces
        CHECK(resolveV4(" "s).empty());

        // Hostnames must start with a digit or letter
        CHECK(resolveV4("!"s).empty());
        CHECK(resolveV4("@+"s).empty());
        CHECK(resolveV4("-0.0.0.0"s).empty());

        // Labels not directly under the root zone must not be empty
        // A string consisting of a single dot is a valid hostname and refers to the root zone
        CHECK(resolveV4(".."s).empty());
        CHECK(resolveV4(".com"s).empty());
        CHECK(resolveV4(".com."s).empty());
        CHECK(resolveV4(".org"s).empty());
        CHECK(resolveV4(".org."s).empty());

        // The maximum label length must not exceed 63 characters
        CHECK(resolveV4("0123456789012345678901234567890123456789012345678901234567890123"s).empty());

        // The total length of the hostname must not exceed 255 characters
        CHECK(resolveV4("0123456789012345678901234567890123456789012345678901234567890123"
                        "0123456789012345678901234567890123456789012345678901234567890123"
                        "0123456789012345678901234567890123456789012345678901234567890123"
                        "0123456789012345678901234567890123456789012345678901234567890123"s)
                  .empty());
    }

    SECTION("IPv6")
    {
        // Hostnames are not allowed to be empty
        CHECK(resolveV6(""s).empty());

        // Hostnames are not allowed to contain spaces
        CHECK(resolveV6(" "s).empty());

        // Hostnames must start with a digit or letter
        CHECK(resolveV6("!"s).empty());
        CHECK(resolveV6("@+"s).empty());
        CHECK(resolveV6("-0::0"s).empty());

        // Labels not directly under the root zone must not be empty
        // A string consisting of a single dot is a valid hostname and refers to the root zone
        CHECK(resolveV6(".."s).empty());
        CHECK(resolveV6(".com"s).empty());
        CHECK(resolveV6(".com."s).empty());
        CHECK(resolveV6(".org"s).empty());
        CHECK(resolveV6(".org."s).empty());

        // The maximum label length must not exceed 63 characters
        CHECK(resolveV6("0123456789012345678901234567890123456789012345678901234567890123"s).empty());

        // The total length of the hostname must not exceed 255 characters
        CHECK(resolveV6("0123456789012345678901234567890123456789012345678901234567890123"
                        "0123456789012345678901234567890123456789012345678901234567890123"
                        "0123456789012345678901234567890123456789012345678901234567890123"
                        "0123456789012345678901234567890123456789012345678901234567890123"s)
                  .empty());
    }
}

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
        REQUIRE((!addresses0.empty() || !addresses1.empty()));
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
        const auto addresses = resolveV6("www.sfml-dev.org");
        REQUIRE_FALSE(addresses.empty());
        CHECK(addresses.front() != sf::IpAddress::AnyV6);
        CHECK(addresses.front() != sf::IpAddress::LocalHostV6);
        CHECK_FALSE(addresses.front().toString().empty());
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
        const auto records = sf::Dns::queryNs("sfml-dev.org");
        REQUIRE_FALSE(records.empty());
        CHECK_FALSE(records.empty());
    }

    SECTION("MX records")
    {
        const auto records = sf::Dns::queryMx("sfml-dev.org");
        REQUIRE_FALSE(records.empty());
        CHECK_FALSE(records.front().exchange.isEmpty());
        CHECK(records.front().preference != 0);
    }

    SECTION("SRV records")
    {
        SECTION("_irc._tcp.sfml-dev.org")
        {
            const auto records = sf::Dns::querySrv("_irc._tcp.sfml-dev.org");
            REQUIRE(records.size() == 1);
            CHECK(records.front().target == "irc.sfml-dev.org");
            CHECK(records.front().port == 6667);
            CHECK(records.front().weight == 5);
            CHECK(records.front().priority == 10);
        }

        SECTION("_ircs._tcp.sfml-dev.org")
        {
            const auto records = sf::Dns::querySrv("_ircs._tcp.sfml-dev.org");
            REQUIRE(records.size() == 1);
            CHECK(records.front().target == "irc.sfml-dev.org");
            CHECK(records.front().port == 6697);
            CHECK(records.front().weight == 5);
            CHECK(records.front().priority == 5);
        }
    }

    SECTION("TXT records")
    {
        SECTION("sfml-dev.org")
        {
            const auto records = sf::Dns::queryTxt("sfml-dev.org");
            REQUIRE_FALSE(records.empty());
            REQUIRE_FALSE(records.front().empty());
            CHECK_FALSE(records.front().front().isEmpty());
        }

        SECTION("test-txt-short.sfml-dev.org")
        {
            const auto records = sf::Dns::queryTxt("test-txt-short.sfml-dev.org");
            REQUIRE(records.size() == 1);
            REQUIRE(records.front().size() == 1);
            CHECK(records.front().front() == "AAAAAAAAAA");
        }

        SECTION("test-txt-long.sfml-dev.org")
        {
            const auto records = sf::Dns::queryTxt("test-txt-long.sfml-dev.org");
            REQUIRE(records.size() == 1);
            REQUIRE(records.front().size() == 2);
            CHECK(records.front()[0] ==
                  "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
                  "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
                  "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
            CHECK(records.front()[1] == "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
        }
    }
}

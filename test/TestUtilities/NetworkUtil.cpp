#include <SFML/Network/Dns.hpp>

#include <NetworkUtil.hpp>
#include <algorithm>
#include <string>

std::string runIpV4LoopbackTests()
{
#if defined(SFML_RUN_LOOPBACK_TESTS) && defined(SFML_RUN_IPV4_LINK_TESTS)
    return "";
#else
    // https://github.com/catchorg/Catch2/blob/devel/docs/test-cases-and-sections.md#special-tags
    // This tag tells Catch2 to not run a given TEST_CASE
    return "[.loopbackv4]";
#endif
}

std::string runIpV4LinkTests()
{
#if defined(SFML_RUN_IPV4_LINK_TESTS)
    return "";
#else
    // https://github.com/catchorg/Catch2/blob/devel/docs/test-cases-and-sections.md#special-tags
    // This tag tells Catch2 to not run a given TEST_CASE
    return "[.linkv4]";
#endif
}

std::string runIpV4InternetTests()
{
#if defined(SFML_RUN_IPV4_INTERNET_TESTS)
    return "";
#else
    // https://github.com/catchorg/Catch2/blob/devel/docs/test-cases-and-sections.md#special-tags
    // This tag tells Catch2 to not run a given TEST_CASE
    return "[.internetv4]";
#endif
}

std::string runIpV6LoopbackTests()
{
#if defined(SFML_RUN_LOOPBACK_TESTS) && defined(SFML_RUN_IPV6_LINK_TESTS)
    return "";
#else
    // https://github.com/catchorg/Catch2/blob/devel/docs/test-cases-and-sections.md#special-tags
    // This tag tells Catch2 to not run a given TEST_CASE
    return "[.loopbackv6]";
#endif
}

std::string runIpV6LinkTests()
{
#if defined(SFML_RUN_IPV6_LINK_TESTS)
    return "";
#else
    // https://github.com/catchorg/Catch2/blob/devel/docs/test-cases-and-sections.md#special-tags
    // This tag tells Catch2 to not run a given TEST_CASE
    return "[.linkv6]";
#endif
}

std::string runIpV6InternetTests()
{
#if defined(SFML_RUN_IPV6_INTERNET_TESTS)
    return "";
#else
    // https://github.com/catchorg/Catch2/blob/devel/docs/test-cases-and-sections.md#special-tags
    // This tag tells Catch2 to not run a given TEST_CASE
    return "[.internetv6]";
#endif
}

std::string runDnsTestsAdvanced()
{
#if defined(SFML_RUN_IPV4_INTERNET_TESTS) || defined(SFML_RUN_IPV6_INTERNET_TESTS)
    return "";
#else
    // https://github.com/catchorg/Catch2/blob/devel/docs/test-cases-and-sections.md#special-tags
    // This tag tells Catch2 to not run a given TEST_CASE
    return "[.dnsadvanced]";
#endif
}

std::vector<sf::IpAddress> resolveV4(std::string_view hostname)
{
    auto addresses = sf::Dns::resolve(hostname);
    addresses
        .erase(std::remove_if(addresses.begin(), addresses.end(), [](const auto& address) { return !address.isV4(); }),
               addresses.end());
    return addresses;
}

std::vector<sf::IpAddress> resolveV6(std::string_view hostname)
{
    auto addresses = sf::Dns::resolve(hostname);
    addresses
        .erase(std::remove_if(addresses.begin(), addresses.end(), [](const auto& address) { return !address.isV6(); }),
               addresses.end());
    return addresses;
}

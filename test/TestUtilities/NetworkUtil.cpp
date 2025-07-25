#include <NetworkUtil.hpp>
#include <string>

std::string runLoopbackTests()
{
#ifdef SFML_RUN_LOOPBACK_TESTS
    return "";
#else
    // https://github.com/catchorg/Catch2/blob/devel/docs/test-cases-and-sections.md#special-tags
    // This tag tells Catch2 to not run a given TEST_CASE
    return "[.loopback]";
#endif
}

std::string runConnectionTests()
{
#ifdef SFML_RUN_CONNECTION_TESTS
    return "";
#else
    // https://github.com/catchorg/Catch2/blob/devel/docs/test-cases-and-sections.md#special-tags
    // This tag tells Catch2 to not run a given TEST_CASE
    return "[.connection]";
#endif
}

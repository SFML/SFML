#include <SFML/System/Version.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("[System] sf::version")
{
    const auto& version = sf::version();
    CHECK(version.major == EXPECTED_SFML_VERSION_MAJOR);
    CHECK(version.minor == EXPECTED_SFML_VERSION_MINOR);
    CHECK(version.patch == EXPECTED_SFML_VERSION_PATCH);
    CHECK(version.isRelease == EXPECTED_SFML_VERSION_IS_RELEASE);

#if EXPECTED_SFML_VERSION_IS_RELEASE
#define EXPECTED_SFML_VERSION_SUFFIX
#else
#define EXPECTED_SFML_VERSION_SUFFIX "-dev"
#endif
#define STR_HELPER(x) #x
#define STR(x)        STR_HELPER(x)

    CHECK(version.string == STR(EXPECTED_SFML_VERSION_MAJOR) "." STR(EXPECTED_SFML_VERSION_MINOR) "." STR(
                                EXPECTED_SFML_VERSION_PATCH) EXPECTED_SFML_VERSION_SUFFIX);
}

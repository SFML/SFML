#include <SFML/System/FileInputStream.hpp>
#include "SystemUtil.hpp"
#include <string_view>
#include <utility>

#include <doctest.h>

TEST_CASE("sf::FileInputStream class - [system]")
{
    SUBCASE("Empty stream")
    {
        sf::FileInputStream fis;

        CHECK(fis.read(nullptr, 0) == -1);
        CHECK(fis.seek(0) == -1);
        CHECK(fis.tell() == -1);
    }

// Work around GCC 8.x bug with `<filesystem>`.
#if !defined(__GNUC__) || (__GNUC__ >= 9)
    SUBCASE("Temporary file stream")
    {
        const std::string fileContents = "hello world";

        sf::Testing::TemporaryFile tmpFile(fileContents);
        sf::FileInputStream fis;

        REQUIRE(fis.open(tmpFile.getPath()));

        char buffer[32];

        CHECK(fis.read(buffer, 5) == 5);
        CHECK(std::string_view(buffer, 5) == std::string_view(fileContents.c_str(), 5));

        SUBCASE("Move semantics")
        {
            sf::FileInputStream fis2 = std::move(fis);

            CHECK(fis2.read(buffer, 6) == 6);
            CHECK(std::string_view(buffer, 6) == std::string_view(fileContents.c_str() + 5, 6));
        }
    }
#endif // !defined(__GNUC__) || (__GNUC__ >= 9)
}

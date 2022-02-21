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

        CHECK(fis.read(sf::Span<std::byte>()) == -1);
        CHECK(fis.seek(0) == -1);
        CHECK(fis.tell() == -1);
    }

    SUBCASE("Temporary file stream")
    {
        const std::string fileContents = "hello world";

        sf::Testing::TemporaryFile tmpFile(fileContents);
        sf::FileInputStream fis;

        REQUIRE(fis.open(tmpFile.getPath()));

        char buffer[32];

        CHECK(fis.read(sf::asWritableBytes(sf::Span(buffer, 5))) == 5);
        CHECK(std::string_view(buffer, 5) == std::string_view(fileContents.c_str(), 5));

        SUBCASE("Move semantics")
        {
            sf::FileInputStream fis2 = std::move(fis);

            CHECK(fis2.read(sf::asWritableBytes(sf::Span(buffer, 6))) == 6);
            CHECK(std::string_view(buffer, 6) == std::string_view(fileContents.c_str() + 5, 6));
        }
    }
}

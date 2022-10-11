#include <SFML/System/MemoryInputStream.hpp>

#include <doctest/doctest.h>

#include <ostream>
#include <string_view>

TEST_CASE("[System] sf::MemoryInputStream")
{
    SUBCASE("Empty stream")
    {
        sf::MemoryInputStream mis;

        CHECK(mis.read(nullptr, 0) == -1);
        CHECK(mis.seek(0) == -1);
        CHECK(mis.tell() == -1);
        CHECK(mis.getSize() == -1);
    }

    SUBCASE("Open memory stream")
    {
        using namespace std::literals::string_view_literals;
        constexpr auto        memoryContents = "hello world"sv;
        sf::MemoryInputStream mis;
        mis.open(memoryContents.data(), sizeof(char) * memoryContents.size());

        char buffer[32];
        CHECK(mis.read(buffer, 5) == 5);
        CHECK(std::string_view(buffer, 5) == std::string_view(memoryContents.data(), 5));
        CHECK(mis.seek(10) == 10);
        CHECK(mis.tell() == 10);
        CHECK(mis.getSize() == 11);
    }
}

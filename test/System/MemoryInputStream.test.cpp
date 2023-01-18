#include <SFML/System/MemoryInputStream.hpp>

#include <catch2/catch_test_macros.hpp>

#include <ostream>
#include <string_view>

TEST_CASE("[System] sf::MemoryInputStream")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::MemoryInputStream>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::MemoryInputStream>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::MemoryInputStream>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::MemoryInputStream>);
    }

    SECTION("Empty stream")
    {
        sf::MemoryInputStream mis;

        CHECK(mis.read(nullptr, 0) == -1);
        CHECK(mis.seek(0) == -1);
        CHECK(mis.tell() == -1);
        CHECK(mis.getSize() == -1);
    }

    SECTION("Open memory stream")
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

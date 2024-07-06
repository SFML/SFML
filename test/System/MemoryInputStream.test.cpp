#include <SFML/System/MemoryInputStream.hpp>

#include <catch2/catch_test_macros.hpp>

#include <array>
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

    using namespace std::literals::string_view_literals;

    SECTION("Construction")
    {
        SECTION("Null data")
        {
            sf::MemoryInputStream memoryInputStream(nullptr, 0);
            CHECK(memoryInputStream.read(nullptr, 0) == std::nullopt);
            CHECK(memoryInputStream.seek(0) == std::nullopt);
            CHECK(memoryInputStream.tell() == std::nullopt);
            CHECK(memoryInputStream.getSize() == std::nullopt);
        }

        static constexpr auto input = "hello world"sv;

        SECTION("Zero length")
        {
            sf::MemoryInputStream memoryInputStream(input.data(), 0);
            CHECK(memoryInputStream.tell().value() == 0);
            CHECK(memoryInputStream.getSize().value() == 0);
        }

        SECTION("Full length")
        {
            sf::MemoryInputStream memoryInputStream(input.data(), input.size());
            CHECK(memoryInputStream.tell().value() == 0);
            CHECK(memoryInputStream.getSize().value() == input.size());
        }
    }

    SECTION("read()")
    {
        static constexpr auto input = "hello world"sv;
        sf::MemoryInputStream memoryInputStream(input.data(), input.size());
        CHECK(memoryInputStream.tell().value() == 0);
        CHECK(memoryInputStream.getSize().value() == input.size());

        // Read within input
        std::array<char, 32> output{};
        CHECK(memoryInputStream.read(output.data(), 5).value() == 5);
        CHECK(std::string_view(output.data(), 5) == "hello"sv);
        CHECK(memoryInputStream.tell().value() == 5);
        CHECK(memoryInputStream.getSize().value() == input.size());

        // Read beyond input
        CHECK(memoryInputStream.read(output.data(), 100).value() == 6);
        CHECK(std::string_view(output.data(), 6) == " world"sv);
        CHECK(memoryInputStream.tell().value() == 11);
        CHECK(memoryInputStream.getSize().value() == input.size());
    }

    SECTION("seek()")
    {
        static constexpr auto input = "We Love SFML!"sv;
        sf::MemoryInputStream memoryInputStream(input.data(), input.size());
        CHECK(memoryInputStream.tell().value() == 0);
        CHECK(memoryInputStream.getSize().value() == input.size());

        SECTION("Seek within input")
        {
            CHECK(memoryInputStream.seek(0).value() == 0);
            CHECK(memoryInputStream.tell().value() == 0);

            CHECK(memoryInputStream.seek(5).value() == 5);
            CHECK(memoryInputStream.tell().value() == 5);
        }

        SECTION("Seek beyond input")
        {
            CHECK(memoryInputStream.seek(1'000).value() == input.size());
            CHECK(memoryInputStream.tell().value() == input.size());
        }
    }
}

#include <SFML/System/FileInputStream.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <array>
#include <string_view>
#include <type_traits>


TEST_CASE("[System] sf::FileInputStream")
{
    using namespace std::string_view_literals;

    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_copy_constructible_v<sf::FileInputStream>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::FileInputStream>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::FileInputStream>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::FileInputStream>);
    }

    std::array<char, 32> buffer{};

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            sf::FileInputStream fileInputStream;
            CHECK(fileInputStream.read(nullptr, 0) == std::nullopt);
            CHECK(fileInputStream.seek(0) == std::nullopt);
            CHECK(fileInputStream.tell() == std::nullopt);
            CHECK(fileInputStream.getSize() == std::nullopt);
        }

        SECTION("File path constructor")
        {
            sf::FileInputStream fileInputStream("test.txt");
            CHECK(fileInputStream.read(buffer.data(), 5) == 5);
            CHECK(fileInputStream.tell() == 5);
            CHECK(fileInputStream.getSize() == 12);
            CHECK(std::string_view(buffer.data(), 5) == "Hello"sv);
            CHECK(fileInputStream.seek(6) == 6);
            CHECK(fileInputStream.tell() == 6);
        }
    }

    SECTION("Move semantics")
    {
        SECTION("Move constructor")
        {
            sf::FileInputStream movedFileInputStream("test.txt");
            sf::FileInputStream fileInputStream = std::move(movedFileInputStream);
            CHECK(fileInputStream.read(buffer.data(), 6) == 6);
            CHECK(fileInputStream.tell() == 6);
            CHECK(fileInputStream.getSize() == 12);
            CHECK(std::string_view(buffer.data(), 6) == "Hello "sv);
        }

        SECTION("Move assignment")
        {
            sf::FileInputStream movedFileInputStream("test.txt");
            sf::FileInputStream fileInputStream("test2.txt");
            fileInputStream = std::move(movedFileInputStream);
            CHECK(fileInputStream.read(buffer.data(), 6) == 6);
            CHECK(fileInputStream.tell() == 6);
            CHECK(fileInputStream.getSize() == 12);
            CHECK(std::string_view(buffer.data(), 6) == "Hello "sv);
        }
    }

    SECTION("Temporary file stream open")
    {
        sf::FileInputStream fileInputStream;
        REQUIRE(fileInputStream.open("test.txt"));
        CHECK(fileInputStream.read(buffer.data(), 5) == 5);
        CHECK(fileInputStream.tell() == 5);
        CHECK(fileInputStream.getSize() == 12);
        CHECK(std::string_view(buffer.data(), 5) == "Hello"sv);
        CHECK(fileInputStream.seek(6) == 6);
        CHECK(fileInputStream.tell() == 6);
    }

    SECTION("Temporary file stream create")
    {
        sf::FileInputStream fileInputStream("test.txt");
        CHECK(fileInputStream.read(buffer.data(), 5) == 5);
        CHECK(fileInputStream.tell() == 5);
        CHECK(fileInputStream.getSize() == 12);
        CHECK(std::string_view(buffer.data(), 5) == "Hello"sv);
        CHECK(fileInputStream.seek(6) == 6);
        CHECK(fileInputStream.tell() == 6);
    }

    SECTION("open()")
    {
        const std::u32string        filenameSuffix = GENERATE(U"", U"-ń", U"-🐌");
        const std::filesystem::path filename       = U"test" + filenameSuffix + U".txt";
        INFO("Filename: " << reinterpret_cast<const char*>(filename.u8string().c_str()));

        sf::FileInputStream fileInputStream;
        CHECK(fileInputStream.open(filename));

        CHECK(fileInputStream.read(buffer.data(), 5) == 5);
        CHECK(fileInputStream.tell() == 5);
        CHECK(fileInputStream.getSize() == 12);
        CHECK(std::string_view(buffer.data(), 5) == "Hello"sv);
        CHECK(fileInputStream.seek(6) == 6);
        CHECK(fileInputStream.tell() == 6);
    }
}

#include <SFML/System/FileInputStream.hpp>

#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <sstream>
#include <string_view>
#include <type_traits>
#include <utility>

#include <cassert>

namespace
{
std::filesystem::path getTemporaryFilePath()
{
    static int counter = 0;

    std::ostringstream oss;
    oss << "sfmltemp" << counter++ << ".tmp";

    return std::filesystem::temp_directory_path() / oss.str();
}

class TemporaryFile
{
public:
    // Create a temporary file with a randomly generated path, containing 'contents'.
    TemporaryFile(const std::string& contents) : m_path(getTemporaryFilePath())
    {
        std::ofstream ofs(m_path);
        assert(ofs && "Stream encountered an error");

        ofs << contents;
        assert(ofs && "Stream encountered an error");
    }

    // Close and delete the generated file.
    ~TemporaryFile()
    {
        [[maybe_unused]] const bool removed = std::filesystem::remove(m_path);
        assert(removed && "m_path failed to be removed from filesystem");
    }

    // Prevent copies.
    TemporaryFile(const TemporaryFile&) = delete;

    TemporaryFile& operator=(const TemporaryFile&) = delete;

    // Return the randomly generated path.
    const std::filesystem::path& getPath() const
    {
        return m_path;
    }

private:
    std::filesystem::path m_path;
};
} // namespace

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

    SECTION("Default constructor")
    {
        sf::FileInputStream fileInputStream;
        CHECK(fileInputStream.read(nullptr, 0) == -1);
        CHECK(fileInputStream.seek(0) == -1);
        CHECK(fileInputStream.tell() == -1);
        CHECK(fileInputStream.getSize() == -1);
    }

    const TemporaryFile temporaryFile("Hello world");
    char                buffer[32];

    SECTION("Move semantics")
    {
        SECTION("Move constructor")
        {
            sf::FileInputStream movedFileInputStream;
            REQUIRE(movedFileInputStream.open(temporaryFile.getPath()));

            sf::FileInputStream fileInputStream = std::move(movedFileInputStream);
            CHECK(fileInputStream.read(buffer, 6) == 6);
            CHECK(fileInputStream.tell() == 6);
            CHECK(fileInputStream.getSize() == 11);
            CHECK(std::string_view(buffer, 6) == "Hello "sv);
        }

        SECTION("Move assignment")
        {
            sf::FileInputStream movedFileInputStream;
            REQUIRE(movedFileInputStream.open(temporaryFile.getPath()));

            sf::FileInputStream fileInputStream;
            fileInputStream = std::move(movedFileInputStream);
            CHECK(fileInputStream.read(buffer, 6) == 6);
            CHECK(fileInputStream.tell() == 6);
            CHECK(fileInputStream.getSize() == 11);
            CHECK(std::string_view(buffer, 6) == "Hello "sv);
        }
    }

    SECTION("Temporary file stream")
    {
        sf::FileInputStream fileInputStream;
        REQUIRE(fileInputStream.open(temporaryFile.getPath()));
        CHECK(fileInputStream.read(buffer, 5) == 5);
        CHECK(fileInputStream.tell() == 5);
        CHECK(fileInputStream.getSize() == 11);
        CHECK(std::string_view(buffer, 5) == "Hello"sv);
        CHECK(fileInputStream.seek(6) == 6);
        CHECK(fileInputStream.tell() == 6);
    }
}

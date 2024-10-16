#include <SFML/System/FileInputStream.hpp>

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
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
    explicit TemporaryFile(const std::string& contents) : m_path(getTemporaryFilePath())
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
    [[nodiscard]] const std::filesystem::path& getPath() const
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

    const TemporaryFile  temporaryFile("Hello world");
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
            sf::FileInputStream fileInputStream(temporaryFile.getPath());
            CHECK(fileInputStream.read(buffer.data(), 5) == 5);
            CHECK(fileInputStream.tell() == 5);
            CHECK(fileInputStream.getSize() == 11);
            CHECK(std::string_view(buffer.data(), 5) == "Hello"sv);
            CHECK(fileInputStream.seek(6) == 6);
            CHECK(fileInputStream.tell() == 6);
        }
    }

    SECTION("Move semantics")
    {
        SECTION("Move constructor")
        {
            sf::FileInputStream movedFileInputStream(temporaryFile.getPath());
            sf::FileInputStream fileInputStream = std::move(movedFileInputStream);
            CHECK(fileInputStream.read(buffer.data(), 6) == 6);
            CHECK(fileInputStream.tell() == 6);
            CHECK(fileInputStream.getSize() == 11);
            CHECK(std::string_view(buffer.data(), 6) == "Hello "sv);
        }

        SECTION("Move assignment")
        {
            sf::FileInputStream movedFileInputStream(temporaryFile.getPath());
            const TemporaryFile temporaryFile2("Hello world the sequel");
            sf::FileInputStream fileInputStream(temporaryFile2.getPath());
            fileInputStream = std::move(movedFileInputStream);
            CHECK(fileInputStream.read(buffer.data(), 6) == 6);
            CHECK(fileInputStream.tell() == 6);
            CHECK(fileInputStream.getSize() == 11);
            CHECK(std::string_view(buffer.data(), 6) == "Hello "sv);
        }
    }

    SECTION("Temporary file stream open")
    {
        sf::FileInputStream fileInputStream;
        REQUIRE(fileInputStream.open(temporaryFile.getPath()));
        CHECK(fileInputStream.read(buffer.data(), 5) == 5);
        CHECK(fileInputStream.tell() == 5);
        CHECK(fileInputStream.getSize() == 11);
        CHECK(std::string_view(buffer.data(), 5) == "Hello"sv);
        CHECK(fileInputStream.seek(6) == 6);
        CHECK(fileInputStream.tell() == 6);
    }

    SECTION("Temporary file stream create")
    {
        sf::FileInputStream fileInputStream(temporaryFile.getPath());
        CHECK(fileInputStream.read(buffer.data(), 5) == 5);
        CHECK(fileInputStream.tell() == 5);
        CHECK(fileInputStream.getSize() == 11);
        CHECK(std::string_view(buffer.data(), 5) == "Hello"sv);
        CHECK(fileInputStream.seek(6) == 6);
        CHECK(fileInputStream.tell() == 6);
    }
}

#include <SFML/System/FileInputStream.hpp>

#include <doctest/doctest.h>

#include <cassert>
#include <fstream>
#include <sstream>
#include <string_view>
#include <utility>

static std::string getTemporaryFilePath()
{
    static int counter = 0;

    std::ostringstream oss;
    oss << "sfmltemp" << counter << ".tmp";
    ++counter;

    std::filesystem::path result;
    result /= std::filesystem::temp_directory_path();
    result /= oss.str();

    return result.string();
}

class TemporaryFile
{
private:
    std::string m_path;

public:
    // Create a temporary file with a randomly generated path, containing 'contents'.
    TemporaryFile(const std::string& contents) : m_path(getTemporaryFilePath())
    {
        std::ofstream ofs(m_path);
        assert(ofs);

        ofs << contents;
        assert(ofs);
    }

    // Close and delete the generated file.
    ~TemporaryFile()
    {
        [[maybe_unused]] const bool removed = std::filesystem::remove(m_path);
        assert(removed);
    }

    // Prevent copies.
    TemporaryFile(const TemporaryFile&) = delete;

    TemporaryFile& operator=(const TemporaryFile&) = delete;

    // Return the randomly generated path.
    const std::string& getPath() const
    {
        return m_path;
    }
};

TEST_CASE("sf::FileInputStream class - [system]")
{
    SUBCASE("Empty stream")
    {
        sf::FileInputStream fis;

        CHECK(fis.read(nullptr, 0) == -1);
        CHECK(fis.seek(0) == -1);
        CHECK(fis.tell() == -1);
    }

    SUBCASE("Temporary file stream")
    {
        const std::string fileContents = "hello world";

        TemporaryFile       tmpFile(fileContents);
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
}

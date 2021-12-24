#include "SystemUtil.hpp"

#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>

// Work around GCC 8.x bug with `<filesystem>`.
#if !defined(__GNUC__) || (__GNUC__ >= 9)
#include <filesystem>
#endif // !defined(__GNUC__) || (__GNUC__ >= 9)

#include <fstream>
#include <sstream>
#include <cassert>

#include <doctest.h>

namespace sf
{
    doctest::String toString(const sf::String& string)
    {
        return string.toAnsiString().c_str();
    }

    doctest::String toString(sf::Time time)
    {
        std::ostringstream stream;
        stream << time.asMicroseconds() << "us";
        return stream.str().c_str();
    }
}

// Work around GCC 8.x bug with `<filesystem>`.
#if !defined(__GNUC__) || (__GNUC__ >= 9)
namespace sf::Testing
{
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

    TemporaryFile::TemporaryFile(const std::string& contents)
        : m_path(getTemporaryFilePath())
    {
        std::ofstream ofs(m_path);
        assert(ofs);

        ofs << contents;
        assert(ofs);
    }

    TemporaryFile::~TemporaryFile()
    {
        [[maybe_unused]] const bool removed = std::filesystem::remove(m_path);
        assert(removed);
    }

    const std::string& TemporaryFile::getPath() const
    {
        return m_path;
    }
}
#endif // !defined(__GNUC__) || (__GNUC__ >= 9)

#include "SystemUtil.hpp"

#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>

// Work around GCC 8.x bug with `<filesystem>`.
#if !defined(__GNUC__) || (__GNUC__ >= 9)
#include <filesystem>
#endif // !defined(__GNUC__) || (__GNUC__ >= 9)

#include <fstream>
#include <ostream>
#include <sstream>
#include <cassert>

namespace sf
{
    std::ostream& operator <<(std::ostream& os, const sf::String& string)
    {
        os << string.toAnsiString();
        return os;
    }

    std::ostream& operator <<(std::ostream& os, sf::Time time)
    {
        os << time.asMicroseconds() << "us";
        return os;
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

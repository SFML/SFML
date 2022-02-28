#include "SystemUtil.hpp"

#include <SFML/System/Angle.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>

#include <doctest.h> // for Approx
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <limits>
#include <ostream>
#include <sstream>

namespace sf
{
    std::ostream& operator <<(std::ostream& os, const sf::Angle& angle)
    {
        os << std::fixed << std::setprecision(std::numeric_limits<float>::max_digits10);
        os << angle.asDegrees() << " deg";
        return os;
    }

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

bool operator==(const sf::Vector2f& lhs, const ApproxVec& rhs)
{
    return (lhs - rhs.vector).length() == doctest::Approx(0.0);
}

bool operator==(const sf::Angle& lhs, const ApproxDeg& rhs)
{
    return lhs.asDegrees() == doctest::Approx(rhs.degrees);
}

std::ostream& operator <<(std::ostream& os, const ApproxVec& approx)
{
    os << approx.vector;
    return os;
}

std::ostream& operator <<(std::ostream& os, const ApproxDeg& approx)
{
    os << sf::degrees(approx.degrees);
    return os;
}

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

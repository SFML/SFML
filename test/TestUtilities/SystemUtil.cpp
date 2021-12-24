// Note: No need to increase compile time by including TestUtilities/SystemUtil.hpp
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>
#include <sstream>

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

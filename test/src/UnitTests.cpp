// Note: No need to increase compile time by including UnitTests.hpp
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <sstream>


// String conversions for Catch framework
namespace Catch
{
    std::string toString(const sf::String& string)
    {
        return string.toAnsiString();
    }

    std::string toString(sf::Time time)
    {
        std::ostringstream stream;
        stream << time.asMicroseconds() << "us";
        return stream.str();
    }

    std::string toString(const sf::VideoMode& videoMode)
    {
        std::ostringstream stream;
        stream << videoMode.width << "x" << videoMode.height << "x" << videoMode.bitsPerPixel;
        return stream.str();
    }

    std::string toString(const sf::Color& color)
    {
        std::ostringstream stream;
        stream << "0x" << std::hex << color.toInteger() << std::dec
               << " (r=" << static_cast<int>(color.r)
               << ", g=" << static_cast<int>(color.g)
               << ", b=" << static_cast<int>(color.b)
               << ", a=" << static_cast<int>(color.a) << ")";

        return stream.str();
    }
}

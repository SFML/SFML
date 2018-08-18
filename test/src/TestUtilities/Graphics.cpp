// Note: No need to increase compile time by including TestUtilities/Graphics.hpp
#include <SFML/Graphics/Color.hpp>
#include <sstream>

// String conversions for Catch framework
namespace Catch
{
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

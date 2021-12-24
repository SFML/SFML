// Note: No need to increase compile time by including TestUtilities/GraphicsUtil.hpp
#include <SFML/Graphics/Color.hpp>
#include <sstream>
#include <doctest.h>

namespace sf
{
    doctest::String toString(const sf::Color& color)
    {
        std::ostringstream stream;
        stream << "0x" << std::hex << color.toInteger() << std::dec
               << " (r=" << static_cast<int>(color.r)
               << ", g=" << static_cast<int>(color.g)
               << ", b=" << static_cast<int>(color.b)
               << ", a=" << static_cast<int>(color.a) << ")";

        return stream.str().c_str();
    }
}

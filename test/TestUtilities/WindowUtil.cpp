// Note: No need to increase compile time by including TestUtilities/WindowUtil.hpp
#include <SFML/Window/VideoMode.hpp>
#include <sstream>

#include <doctest.h>

namespace sf
{
    doctest::String toString(const sf::VideoMode& videoMode)
    {
        std::ostringstream stream;
        stream << videoMode.width << "x" << videoMode.height << "x" << videoMode.bitsPerPixel;
        return stream.str().c_str();
    }
}

// Note: No need to increase compile time by including TestUtilities/Window.hpp
#include <SFML/Window/VideoMode.hpp>
#include <sstream>

// String conversions for Catch framework
namespace Catch
{
    std::string toString(const sf::VideoMode& videoMode)
    {
        std::ostringstream stream;
        stream << videoMode.width << "x" << videoMode.height << "x" << videoMode.bitsPerPixel;
        return stream.str();
    }
}

// Note: No need to increase compile time by including TestUtilities/Window.hpp
#include <SFML/Window/VideoMode.hpp>

#include <ostream>

namespace sf
{
std::ostream& operator<<(std::ostream& os, const sf::VideoMode& videoMode)
{
    os << videoMode.size.x << "x" << videoMode.size.y << "x" << videoMode.bitsPerPixel;
    return os;
}
} // namespace sf

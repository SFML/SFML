// Header for SFML unit tests.
//
// For a new graphics module test case, include this header.
// This ensures that string conversions are visible and can be used by doctest for debug output.

#ifndef SFML_TESTUTILITIES_GRAPHICS_HPP
#define SFML_TESTUTILITIES_GRAPHICS_HPP

#include <SFML/Graphics/Rect.hpp>

#include <WindowUtil.hpp>
#include <iomanip>
#include <limits>

namespace sf
{
struct BlendMode;
class Color;
class Transform;

std::ostream& operator<<(std::ostream& os, const BlendMode& blendMode);
std::ostream& operator<<(std::ostream& os, const Color& color);
std::ostream& operator<<(std::ostream& os, const Transform& transform);

template <typename T>
std::ostream& operator<<(std::ostream& os, const sf::Rect<T>& rect)
{
    const auto flags = os.flags();
    os << std::fixed << std::setprecision(std::numeric_limits<T>::max_digits10);
    os << "(left=" << rect.left << ", top=" << rect.top << ", width=" << rect.width << ", height=" << rect.height << ")";
    os.flags(flags);
    return os;
}
} // namespace sf

bool operator==(const sf::Transform& lhs, const Approx<sf::Transform>& rhs);

#endif // SFML_TESTUTILITIES_GRAPHICS_HPP

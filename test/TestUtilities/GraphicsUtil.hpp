// Header for SFML unit tests.
//
// For a new graphics module test case, include this header.
// This ensures that string conversions are visible and can be used by doctest for debug output.

#pragma once

#include <SFML/Graphics/Rect.hpp>

#include <WindowUtil.hpp>
#include <iomanip>
#include <limits>

#ifdef SFML_RUN_DISPLAY_TESTS
static constexpr bool skipDisplayTests = false;
#else
static constexpr bool skipDisplayTests = true;
#endif

namespace sf
{
struct BlendMode;
class Color;
class Transform;

std::ostream& operator<<(std::ostream& os, const BlendMode& blendMode);
std::ostream& operator<<(std::ostream& os, const Color& color);
std::ostream& operator<<(std::ostream& os, const Transform& transform);

template <typename T>
std::ostream& operator<<(std::ostream& os, const Rect<T>& rect)
{
    const auto flags = os.flags();
    os << std::fixed << std::setprecision(std::numeric_limits<T>::max_digits10);
    os << "(left=" << rect.left << ", top=" << rect.top << ", width=" << rect.width << ", height=" << rect.height << ")";
    os.flags(flags);
    return os;
}
} // namespace sf

bool operator==(const sf::Transform& lhs, const Approx<sf::Transform>& rhs);

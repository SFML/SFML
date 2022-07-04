// Header for SFML unit tests.
//
// For a new system module test case, include this header.
// This ensures that string conversions are visible and can be used by doctest for debug output.

#ifndef SFML_TESTUTILITIES_SYSTEM_HPP
#define SFML_TESTUTILITIES_SYSTEM_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include <iomanip>
#include <limits>
#include <ostream>

// String conversions for doctest framework
namespace sf
{
class Angle;
class String;
class Time;

std::ostream& operator<<(std::ostream& os, const sf::Angle& angle);
std::ostream& operator<<(std::ostream& os, const sf::String& string);
std::ostream& operator<<(std::ostream& os, sf::Time time);

template <typename T>
std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& vector)
{
    os << std::fixed << std::setprecision(std::numeric_limits<T>::max_digits10);
    os << "(" << vector.x << ", " << vector.y << ")";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const sf::Vector3<T>& vector)
{
    os << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
    return os;
}
} // namespace sf

////////////////////////////////////////////////////////////
/// Class template for creating custom approximate comparisons.
/// To register a new type, simply implement a custom operator==
/// overload for that type.
////////////////////////////////////////////////////////////
template <typename T>
struct Approx
{
    explicit Approx(const T& t) : value(t)
    {
    }
    const T& value;
};

bool operator==(const float& lhs, const Approx<float>& rhs);
bool operator==(const sf::Vector2f& lhs, const Approx<sf::Vector2f>& rhs);
bool operator==(const sf::Vector3f& lhs, const Approx<sf::Vector3f>& rhs);
bool operator==(const sf::Angle& lhs, const Approx<sf::Angle>& rhs);

template <typename T>
std::ostream& operator<<(std::ostream& os, const Approx<T>& approx)
{
    return os << approx.value;
}

#endif // SFML_TESTUTILITIES_SYSTEM_HPP

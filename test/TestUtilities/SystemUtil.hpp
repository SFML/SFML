// Header for SFML unit tests.
//
// For a new system module test case, include this header.
// This ensures that string conversions are visible and can be used by doctest for debug output.

#pragma once

#include <iosfwd>

// String conversions for doctest framework
namespace sf
{
class Angle;
class String;
class Time;

template <typename>
class Vector2;

template <typename>
class Vector3;

void setStreamPrecision(std::ostream& os, int maxDigits10);

std::ostream& operator<<(std::ostream& os, const Angle& angle);
std::ostream& operator<<(std::ostream& os, const String& string);
std::ostream& operator<<(std::ostream& os, Time time);

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector2<T>& vector);

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& vector);
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
bool operator==(const sf::Vector2<float>& lhs, const Approx<sf::Vector2<float>>& rhs);
bool operator==(const sf::Vector3<float>& lhs, const Approx<sf::Vector3<float>>& rhs);
bool operator==(const sf::Angle& lhs, const Approx<sf::Angle>& rhs);

template <typename T>
std::ostream& operator<<(std::ostream& os, const Approx<T>& approx);

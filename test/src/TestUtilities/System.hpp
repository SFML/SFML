// Header for SFML unit tests.
//
// For a new system module test case, include this header and not <catch.hpp> directly.
// This ensures that string conversions are visible and can be used by Catch for debug output.

#ifndef SFML_TESTUTILITIES_SYSTEM_HPP
#define SFML_TESTUTILITIES_SYSTEM_HPP

#include <catch.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <sstream>

// Forward declarations for non-template types
namespace sf
{
    class String;
    class Time;
}

// String conversions for Catch framework
namespace Catch
{
    std::string toString(const sf::String& string);
    std::string toString(sf::Time time);

    template <typename T>
    std::string toString(const sf::Vector2<T>& vector)
    {
        std::ostringstream stream;
        stream << "(" << vector.x << ", " << vector.y << ")";
        return stream.str();
    }

    template <typename T>
    std::string toString(const sf::Vector3<T>& vector)
    {
        std::ostringstream stream;
        stream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
        return stream.str();
    }
}

#endif // SFML_TESTUTILITIES_SYSTEM_HPP

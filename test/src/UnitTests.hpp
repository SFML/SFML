// Header for SFML unit tests.
//
// For a new test case, include this header and not <catch.hpp> directly.
// This ensures that string conversions are visible and can be used by Catch for debug output.

#ifndef SFML_UNITTESTS_HPP
#define SFML_UNITTESTS_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <sstream>
#include <catch.hpp>


// Forward declarations for non-template types
namespace sf
{
    class Color;
    class String;
    class Time;
    class VideoMode;
}

// String conversions for Catch framework
namespace Catch
{
    std::string toString(const sf::String& string);
    std::string toString(sf::Time time);
    std::string toString(const sf::VideoMode& videoMode);
    std::string toString(const sf::Color& color);

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

    template <typename T>
    std::string toString(const sf::Rect<T>& rect)
    {
        std::ostringstream stream;
        stream << "(left=" << rect.left << ", top=" << rect.top << ", width=" << rect.width << ", height=" << rect.height << ")";
        return stream.str();
    }
}

#endif // SFML_UNITTESTS_HPP

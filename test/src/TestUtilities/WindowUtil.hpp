// Header for SFML unit tests.
//
// For a new window module test case, include this header and not <catch.hpp> directly.
// This ensures that string conversions are visible and can be used by Catch for debug output.

#ifndef SFML_TESTUTILITIES_WINDOW_HPP
#define SFML_TESTUTILITIES_WINDOW_HPP

#include "SystemUtil.hpp"

#include <SFML/Graphics/Rect.hpp>

// Forward declarations for non-template types
namespace sf
{
    class VideoMode;
}

// String conversions for Catch framework
namespace Catch
{
    std::string toString(const sf::VideoMode& videoMode);

    template <typename T>
    std::string toString(const sf::Rect<T>& rect)
    {
        std::ostringstream stream;
        stream << "(left=" << rect.left << ", top=" << rect.top << ", width=" << rect.width << ", height=" << rect.height << ")";
        return stream.str();
    }
}

#endif // SFML_TESTUTILITIES_WINDOW_HPP

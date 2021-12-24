// Header for SFML unit tests.
//
// For a new window module test case, include this header and not <doctest.h> directly.
// This ensures that string conversions are visible and can be used by doctest for debug output.

#ifndef SFML_TESTUTILITIES_WINDOW_HPP
#define SFML_TESTUTILITIES_WINDOW_HPP

#include "SystemUtil.hpp"

#include <SFML/Graphics/Rect.hpp>

#include <doctest.h>

// String conversions for doctest framework
namespace sf
{
    class VideoMode;

    doctest::String toString(const sf::VideoMode& videoMode);

    template <typename T>
    doctest::String toString(const sf::Rect<T>& rect)
    {
        std::ostringstream stream;
        stream << "(left=" << rect.left << ", top=" << rect.top << ", width=" << rect.width << ", height=" << rect.height << ")";
        return stream.str().c_str();
    }
}

#endif // SFML_TESTUTILITIES_WINDOW_HPP

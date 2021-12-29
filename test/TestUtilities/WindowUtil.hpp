// Header for SFML unit tests.
//
// For a new window module test case, include this header.
// This ensures that string conversions are visible and can be used by doctest for debug output.

#ifndef SFML_TESTUTILITIES_WINDOW_HPP
#define SFML_TESTUTILITIES_WINDOW_HPP

#include "SystemUtil.hpp"

#include <SFML/Graphics/Rect.hpp>

// String conversions for doctest framework
namespace sf
{
    class VideoMode;

    std::ostream& operator <<(std::ostream& os, const sf::VideoMode& videoMode);

    template <typename T>
    std::ostream& operator <<(std::ostream& os, const sf::Rect<T>& rect)
    {
        os << "(left=" << rect.left << ", top=" << rect.top << ", width=" << rect.width << ", height=" << rect.height << ")";
        return os;
    }
}

#endif // SFML_TESTUTILITIES_WINDOW_HPP

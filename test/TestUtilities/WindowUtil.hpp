// Header for SFML unit tests.
//
// For a new window module test case, include this header.
// This ensures that string conversions are visible and can be used by doctest for debug output.

#ifndef SFML_TESTUTILITIES_WINDOW_HPP
#define SFML_TESTUTILITIES_WINDOW_HPP

#include <SystemUtil.hpp>

// String conversions for doctest framework
namespace sf
{
class VideoMode;

std::ostream& operator<<(std::ostream& os, const sf::VideoMode& videoMode);
} // namespace sf

#endif // SFML_TESTUTILITIES_WINDOW_HPP

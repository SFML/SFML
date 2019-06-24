// Header for SFML unit tests.
//
// For a new graphics module test case, include this header and not <catch.hpp> directly.
// This ensures that string conversions are visible and can be used by Catch for debug output.

#ifndef SFML_TESTUTILITIES_GRAPHICS_HPP
#define SFML_TESTUTILITIES_GRAPHICS_HPP

#include "WindowUtil.hpp"

// Forward declarations for non-template types
namespace sf
{
    class Color;
}

// String conversions for Catch framework
namespace Catch
{
    std::string toString(const sf::Color& color);
}

#endif // SFML_TESTUTILITIES_GRAPHICS_HPP

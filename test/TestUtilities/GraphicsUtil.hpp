// Header for SFML unit tests.
//
// For a new graphics module test case, include this header and not <doctest.h> directly.
// This ensures that string conversions are visible and can be used by doctest for debug output.

#ifndef SFML_TESTUTILITIES_GRAPHICS_HPP
#define SFML_TESTUTILITIES_GRAPHICS_HPP

#include "WindowUtil.hpp"

namespace doctest
{
    class String;
}

namespace sf
{
    class Color;
    doctest::String toString(const Color& color);
}

#endif // SFML_TESTUTILITIES_GRAPHICS_HPP

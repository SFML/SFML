// Header for SFML unit tests.
//
// For a new window module test case, include this header.
// This ensures that string conversions are visible and can be used by doctest for debug output.

#pragma once

#include <SystemUtil.hpp>

// String conversions for doctest framework
namespace sf
{
class VideoMode;

std::ostream& operator<<(std::ostream& os, const VideoMode& videoMode);
} // namespace sf

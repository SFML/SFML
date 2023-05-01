// Header for SFML unit tests.
//
// For a new window module test case, include this header.
// This ensures that string conversions are visible and can be used by doctest for debug output.

#pragma once

#include <SystemUtil.hpp>

#ifdef SFML_RUN_DISPLAY_TESTS
static constexpr bool skipDisplayTests = false;
#else
static constexpr bool skipDisplayTests = true;
#endif

// String conversions for doctest framework
namespace sf
{
class VideoMode;

// Required because WindowUtil.cpp doesn't include WindowUtil.hpp
// NOLINTNEXTLINE(readability-redundant-declaration)
std::ostream& operator<<(std::ostream& os, const VideoMode& videoMode);
} // namespace sf

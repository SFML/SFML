// Header for SFML unit tests.
//
// For a new window module test case, include this header.
// This ensures that string conversions are visible and can be used by Catch2 for debug output.

#pragma once

#include <SystemUtil.hpp>
#include <string>

// Required because WindowUtil.cpp doesn't include WindowUtil.hpp
// NOLINTNEXTLINE(readability-redundant-declaration)
std::string runDisplayTests();

// String conversions for Catch2
namespace sf
{
class VideoMode;

// Required because WindowUtil.cpp doesn't include WindowUtil.hpp
// NOLINTNEXTLINE(readability-redundant-declaration)
std::ostream& operator<<(std::ostream& os, const VideoMode& videoMode);
} // namespace sf

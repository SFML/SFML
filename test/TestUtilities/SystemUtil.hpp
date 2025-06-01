// Header for SFML unit tests.
//
// For a new system module test case, include this header.
// This ensures that string conversions are visible and can be used by Catch2 for debug output.

#pragma once

#include <catch2/catch_tostring.hpp>

#include <filesystem>
#include <ios>
#include <sstream>
#include <vector>

#include <cstddef>

// String conversions for Catch2
template <typename CharT>
[[nodiscard]] std::string toString(std::basic_string_view<CharT> string)
{
    std::ostringstream out;
    out << "{ " << std::hex;
    for (std::size_t i = 0; i + 1 < string.size(); ++i)
        out << "\\x" << std::uint32_t{string[i]} << ", ";
    out << "\\x" << std::uint32_t{string.back()} << " }";
    return out.str();
}

template <typename CharT>
[[nodiscard]] std::string toString(const std::basic_string<CharT>& string)
{
    return toString(std::basic_string_view<CharT>(string));
}

namespace sf
{
class Angle;
class String;
class Time;

template <typename>
class Vector2;

template <typename>
class Vector3;

void setStreamPrecision(std::ostream& os, int maxDigits10);

std::ostream& operator<<(std::ostream& os, const Angle& angle);
std::ostream& operator<<(std::ostream& os, const String& string);
std::ostream& operator<<(std::ostream& os, Time time);

template <typename T>
std::ostream& operator<<(std::ostream& os, Vector2<T> vector);

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& vector);
} // namespace sf

namespace Catch
{
template <typename Enum>
struct StringMaker<Enum, std::enable_if_t<std::is_enum_v<Enum>>>
{
    static std::string convert(Enum e)
    {
        return std::to_string(static_cast<int>(e));
    }
};

template <>
struct StringMaker<sf::String>
{
    static std::string convert(const sf::String& string);
};

#ifdef __cpp_char8_t
template <>
struct StringMaker<char8_t>
{
    static std::string convert(char8_t char8);
};
#endif

template <>
struct StringMaker<char16_t>
{
    static std::string convert(char16_t char16);
};

template <>
struct StringMaker<char32_t>
{
    static std::string convert(char32_t char32);
};
} // namespace Catch

////////////////////////////////////////////////////////////
/// Class template for creating custom approximate comparisons.
/// To register a new type, simply implement a custom operator==
/// overload for that type.
////////////////////////////////////////////////////////////
template <typename T>
struct Approx
{
    explicit Approx(const T& t) : value(t)
    {
    }

    const T& value;
};

bool operator==(const float& lhs, const Approx<float>& rhs);
bool operator==(sf::Vector2<float> lhs, const Approx<sf::Vector2<float>>& rhs);
bool operator==(const sf::Vector3<float>& lhs, const Approx<sf::Vector3<float>>& rhs);
bool operator==(const sf::Angle& lhs, const Approx<sf::Angle>& rhs);

template <typename T>
std::ostream& operator<<(std::ostream& os, const Approx<T>& approx)
{
    return os << approx.value;
}

[[nodiscard]] std::vector<std::byte> loadIntoMemory(const std::filesystem::path& path);

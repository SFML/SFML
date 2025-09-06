// Header for SFML unit tests.
//
// For a new graphics module test case, include this header.
// This ensures that string conversions are visible and can be used by Catch2 for debug output.

#pragma once

#include <catch2/matchers/catch_matchers_templated.hpp>

#include <SystemUtil.hpp>
#include <iosfwd>

#include <cmath>

namespace sf
{
struct BlendMode;
enum class StencilComparison;
enum class StencilUpdateOperation;
struct StencilMode;
class Color;
class Transform;

template <typename>
class Rect;

std::ostream& operator<<(std::ostream& os, const BlendMode& blendMode);
std::ostream& operator<<(std::ostream& os, const StencilComparison& comparison);
std::ostream& operator<<(std::ostream& os, const StencilUpdateOperation& updateOperation);
std::ostream& operator<<(std::ostream& os, const StencilMode& stencilMode);
std::ostream& operator<<(std::ostream& os, Color color);
std::ostream& operator<<(std::ostream& os, const Transform& transform);

template <typename T>
std::ostream& operator<<(std::ostream& os, const Rect<T>& rect);
} // namespace sf

bool operator==(const sf::Transform& lhs, const Approx<sf::Transform>& rhs);

template <typename T>
bool operator==(const sf::Rect<T>& lhs, const Approx<sf::Rect<T>>& rhs)
{
    return lhs.position == Approx(rhs.value.position) && lhs.size == Approx(rhs.value.size);
}

template <typename T>
struct EqualsApproxVector2Matcher : Catch::Matchers::MatcherGenericBase
{
    EqualsApproxVector2Matcher(const sf::Vector2<T>& value, T delta) : m_value{value}, m_delta{delta}
    {
    }

    template <typename U>
    bool match(const sf::Vector2<U>& other) const
    {
        return std::abs(other.x - m_value.x) <= m_delta && std::abs(other.y - m_value.y) <= m_delta;
    }

    std::string describe() const override
    {
        return "EqualsApprox: {" + std::to_string(m_value.x) + ';' + std::to_string(m_value.y) +
               "} += " + std::to_string(m_delta);
    }

private:
    const sf::Vector2<T>& m_value;
    const T               m_delta;
};

template <typename T>
auto equalsApprox(const sf::Vector2<T>& value, T delta) -> EqualsApproxVector2Matcher<T>
{
    return EqualsApproxVector2Matcher<T>{value, delta};
}

template <typename T>
struct EqualsApproxRectMatcher : Catch::Matchers::MatcherGenericBase
{
    EqualsApproxRectMatcher(const sf::Rect<T>& value, T delta) : m_value{value}, m_delta{delta}
    {
    }

    template <typename U>
    bool match(const sf::Rect<U>& other) const
    {
        return std::abs(other.position.x - m_value.position.x) <= m_delta &&
               std::abs(other.position.y - m_value.position.y) <= m_delta &&
               std::abs(other.size.x - m_value.size.x) <= m_delta && std::abs(other.size.y - m_value.size.y) <= m_delta;
    }

    std::string describe() const override
    {
        return "EqualsApprox: {{" + std::to_string(m_value.position.x) + ';' + std::to_string(m_value.position.y) +
               "},{" + std::to_string(m_value.size.x) + ';' + std::to_string(m_value.size.y) +
               "}} += " + std::to_string(m_delta);
    }

private:
    const sf::Rect<T>& m_value;
    const T            m_delta;
};

template <typename T>
auto equalsApprox(const sf::Rect<T>& value, T delta) -> EqualsApproxRectMatcher<T>
{
    return EqualsApproxRectMatcher<T>{value, delta};
}

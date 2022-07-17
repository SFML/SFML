#include <SFML/System/Angle.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>

#include <doctest/doctest.h>

#include <SystemUtil.hpp>

namespace sf
{
std::ostream& operator<<(std::ostream& os, const sf::Angle& angle)
{
    os << std::fixed << std::setprecision(std::numeric_limits<float>::max_digits10);
    os << angle.asDegrees() << " deg";
    return os;
}

std::ostream& operator<<(std::ostream& os, const sf::String& string)
{
    os << string.toAnsiString();
    return os;
}

std::ostream& operator<<(std::ostream& os, sf::Time time)
{
    os << time.asMicroseconds() << "us";
    return os;
}
} // namespace sf

bool operator==(const float& lhs, const Approx<float>& rhs)
{
    return static_cast<double>(lhs) == doctest::Approx(static_cast<double>(rhs.value));
}

bool operator==(const sf::Vector2f& lhs, const Approx<sf::Vector2f>& rhs)
{
    return (lhs - rhs.value).length() == Approx(0.f);
}

bool operator==(const sf::Vector3f& lhs, const Approx<sf::Vector3f>& rhs)
{
    return (lhs - rhs.value).length() == Approx(0.f);
}

bool operator==(const sf::Angle& lhs, const Approx<sf::Angle>& rhs)
{
    return lhs.asDegrees() == Approx(rhs.value.asDegrees());
}

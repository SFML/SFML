#include <SFML/System/Angle.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <SystemUtil.hpp>
#include <iomanip>
#include <limits>

namespace sf
{
void setStreamPrecision(std::ostream& os, int maxDigits10)
{
    os << std::fixed << std::setprecision(maxDigits10);
}

std::ostream& operator<<(std::ostream& os, const Angle& angle)
{
    setStreamPrecision(os, std::numeric_limits<float>::max_digits10);
    return os << angle.asDegrees() << " deg";
}

std::ostream& operator<<(std::ostream& os, const String& string)
{
    return os << string.toAnsiString();
}

std::ostream& operator<<(std::ostream& os, Time time)
{
    return os << time.asMicroseconds() << "us";
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector2<T>& vector)
{
    setStreamPrecision(os, std::numeric_limits<T>::max_digits10);
    return os << "(" << vector.x << ", " << vector.y << ")";
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& vector)
{
    setStreamPrecision(os, std::numeric_limits<T>::max_digits10);
    return os << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
}

template std::ostream& operator<<(std::ostream&, const Vector2<int>&);
template std::ostream& operator<<(std::ostream&, const Vector2<unsigned int>&);
template std::ostream& operator<<(std::ostream&, const Vector2<float>&);

template std::ostream& operator<<(std::ostream&, const Vector3<int>&);
template std::ostream& operator<<(std::ostream&, const Vector3<unsigned int>&);
template std::ostream& operator<<(std::ostream&, const Vector3<float>&);
} // namespace sf

bool operator==(const float& lhs, const Approx<float>& rhs)
{
    return static_cast<double>(lhs) == Catch::Approx(static_cast<double>(rhs.value)).margin(1e-5);
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

template <typename T>
std::ostream& operator<<(std::ostream& os, const Approx<T>& approx)
{
    return os << approx.value;
}

template std::ostream& operator<<(std::ostream&, const Approx<int>&);
template std::ostream& operator<<(std::ostream&, const Approx<float>&);
template std::ostream& operator<<(std::ostream&, const Approx<sf::Vector2<float>>&);
template std::ostream& operator<<(std::ostream&, const Approx<sf::Vector3<float>>&);
template std::ostream& operator<<(std::ostream&, const Approx<sf::Angle>&);

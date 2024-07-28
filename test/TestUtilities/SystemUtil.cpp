#include <SFML/System/Angle.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include <catch2/catch_approx.hpp>

#include <SystemUtil.hpp>
#include <fstream>
#include <iomanip>
#include <limits>

#include <cassert>


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
std::ostream& operator<<(std::ostream& os, Vector2<T> vector)
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

template std::ostream& operator<<(std::ostream&, Vector2<int>);
template std::ostream& operator<<(std::ostream&, Vector2<unsigned int>);
template std::ostream& operator<<(std::ostream&, Vector2<float>);

template std::ostream& operator<<(std::ostream&, const Vector3<int>&);
template std::ostream& operator<<(std::ostream&, const Vector3<unsigned int>&);
template std::ostream& operator<<(std::ostream&, const Vector3<float>&);
} // namespace sf

bool operator==(const float& lhs, const Approx<float>& rhs)
{
    return lhs == Catch::Approx(rhs.value).margin(1e-5);
}

bool operator==(sf::Vector2f lhs, const Approx<sf::Vector2f>& rhs)
{
    return (lhs - rhs.value).length() == Approx(0.f);
}

bool operator==(const sf::Vector3f& lhs, const Approx<sf::Vector3f>& rhs)
{
    return (lhs - rhs.value).length() == Approx(0.f);
}

bool operator==(const sf::Angle& lhs, const Approx<sf::Angle>& rhs)
{
    return lhs.asRadians() == Approx(rhs.value.asRadians());
}

std::vector<std::byte> loadIntoMemory(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    assert(file);
    const auto size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<std::byte>       buffer(static_cast<std::size_t>(size));
    [[maybe_unused]] const auto& result = file.read(reinterpret_cast<char*>(buffer.data()),
                                                    static_cast<std::streamsize>(size));
    assert(result);
    return buffer;
}

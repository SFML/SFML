#ifndef SFML_TESTUTILITIES_APPROX_HPP
#define SFML_TESTUTILITIES_APPROX_HPP

#include <ostream>

namespace sf::Testing
{
    template <typename T>
    struct Approx
    {
        Approx(const T& value)
        : value{value}
        {}

        const T& value;
    };

    template <typename T>
    bool operator ==(const T& value, const Approx<T>& approx)
    {
        return almostEqual(value, approx.value);
    }

    template <typename T>
    std::ostream& operator <<(std::ostream& out, const sf::Testing::Approx<T>& approx)
    {
        return out << approx.value;
    }
}

#endif // SFML_TESTUTILITIES_APPROX_HPP

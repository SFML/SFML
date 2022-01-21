#ifndef SFML_TESTUTILITIES_ARRAY_HPP
#define SFML_TESTUTILITIES_ARRAY_HPP

#include <array>
#include <limits>
#include <ostream>

namespace sf::Testing
{
    template <std::size_t size, typename T> // T is the second parameter so it can be omitted and deduced.
    std::array<T, size> toArray(const T* pointer)
    {
        std::array<T, size> array;
        std::copy(pointer, pointer + size, array.begin());
        return array;
    }
}

namespace std
{
    template <typename T, std::size_t size>
    std::ostream& operator <<(std::ostream& out, const std::array<T, size>& array)
    {
        if (size == 0)
            return out << "{}";

        // Make sure enough digits are printed so that distinct floating-point values are uniquely represented.
        const auto previous_precision = out.precision(std::numeric_limits<T>::max_digits10);

        out << "{ ";
        for (std::size_t i = 0; i + 1 < array.size(); ++i)
            out << array[i] << ", ";
        out << array.back() << " }";

        out.precision(previous_precision);

        return out;
    }
}

#endif // SFML_TESTUTILITIES_ARRAY_HPP

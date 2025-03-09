////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Glsl.hpp> // NOLINT(misc-header-include-cycle)

#include <array>

#include <cstddef>


namespace sf
{
class Transform;
} // namespace sf

namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief Helper functions to copy `sf::Transform` to `sf::Glsl::Mat3/4`
///
////////////////////////////////////////////////////////////
void SFML_GRAPHICS_API copyMatrix(const Transform& source, Matrix<3, 3>& dest);
void SFML_GRAPHICS_API copyMatrix(const Transform& source, Matrix<4, 4>& dest);

////////////////////////////////////////////////////////////
/// \brief Copy array-based matrix with given number of elements
///
/// Indirection to `std::copy()` to avoid inclusion of
/// <algorithm> and MSVC's annoying 4996 warning in header
///
////////////////////////////////////////////////////////////
void SFML_GRAPHICS_API copyMatrix(const float* source, std::size_t elements, float* dest);


////////////////////////////////////////////////////////////
/// \brief Matrix type, used to set uniforms in GLSL
///
////////////////////////////////////////////////////////////
template <std::size_t Columns, std::size_t Rows>
struct Matrix
{
    ////////////////////////////////////////////////////////////
    /// \brief Construct from raw data
    ///
    /// \param pointer Points to the beginning of an array that
    ///                has the size of the matrix. The elements
    ///                are copied to the instance.
    ///
    ////////////////////////////////////////////////////////////
    explicit Matrix(const float* pointer)
    {
        copyMatrix(pointer, Columns * Rows, array.data());
    }

    ////////////////////////////////////////////////////////////
    /// \brief Construct implicitly from SFML transform
    ///
    /// This constructor is only supported for 3x3 and 4x4
    /// matrices.
    ///
    /// \param transform Object containing a transform.
    ///
    ////////////////////////////////////////////////////////////
    Matrix(const Transform& transform)
    {
        copyMatrix(transform, *this);
    }

    std::array<float, Columns * Rows> array{}; //!< Array holding matrix data
};

////////////////////////////////////////////////////////////
/// \brief 4D vector type, used to set uniforms in GLSL
///
////////////////////////////////////////////////////////////
template <typename T>
struct Vector4
{
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor, creates a zero vector
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector4() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Construct from 4 vector components
    ///
    /// \param x Component of the 4D vector
    /// \param y Component of the 4D vector
    /// \param z Component of the 4D vector
    /// \param w Component of the 4D vector
    ///
    ////////////////////////////////////////////////////////////
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#endif
    constexpr Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w)
    {
    }
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

    ////////////////////////////////////////////////////////////
    /// \brief Converts the vector to another type of vector
    ///
    ////////////////////////////////////////////////////////////
    template <typename U>
    constexpr explicit operator Vector4<U>() const
    {
        return Vector4<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z), static_cast<U>(w));
    }

    ////////////////////////////////////////////////////////////
    /// \brief Construct vector implicitly from color
    ///
    /// Vector is normalized to [0, 1] for floats, and left as-is
    /// for ints. Not defined for other template arguments.
    ///
    /// \param color Color instance
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector4(Color color);

    T x{}; //!< 1st component (X) of the 4D vector
    T y{}; //!< 2nd component (Y) of the 4D vector
    T z{}; //!< 3rd component (Z) of the 4D vector
    T w{}; //!< 4th component (W) of the 4D vector
};


////////////////////////////////////////////////////////////
template <>
constexpr Vector4<float>::Vector4(Color color) :
x(color.r / 255.f),
y(color.g / 255.f),
z(color.b / 255.f),
w(color.a / 255.f)
{
}


////////////////////////////////////////////////////////////
template <>
constexpr Vector4<int>::Vector4(Color color) : x(color.r), y(color.g), z(color.b), w(color.a)
{
}

} // namespace sf::priv

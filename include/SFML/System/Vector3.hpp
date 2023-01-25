////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once

#include <SFML/System/Export.hpp>

#include <cassert>

namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Utility template class for manipulating
///        3-dimensional vectors
///
////////////////////////////////////////////////////////////
template <typename T>
class Vector3
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates a Vector3(0, 0, 0).
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector3();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the vector from its coordinates
    ///
    /// \param x X coordinate
    /// \param y Y coordinate
    /// \param z Z coordinate
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector3(T x, T y, T z);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the vector from another type of vector
    ///
    /// This constructor doesn't replace the copy constructor,
    /// it's called only when U != T.
    /// A call to this constructor will fail to compile if U
    /// is not convertible to T.
    ///
    /// \param vector Vector to convert
    ///
    ////////////////////////////////////////////////////////////
    template <typename U>
    constexpr explicit Vector3(const Vector3<U>& vector);

    ////////////////////////////////////////////////////////////
    /// \brief Length of the vector <i><b>(floating-point)</b></i>.
    ///
    /// If you are not interested in the actual length, but only in comparisons, consider using lengthSq().
    ///
    ////////////////////////////////////////////////////////////
    SFML_SYSTEM_API T length() const;

    ////////////////////////////////////////////////////////////
    /// \brief Square of vector's length.
    ///
    /// Suitable for comparisons, more efficient than length().
    ///
    ////////////////////////////////////////////////////////////
    constexpr T lengthSq() const;

    ////////////////////////////////////////////////////////////
    /// \brief Vector with same direction but length 1 <i><b>(floating-point)</b></i>.
    ///
    /// \pre \c *this is no zero vector.
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] SFML_SYSTEM_API Vector3 normalized() const;

    ////////////////////////////////////////////////////////////
    /// \brief Dot product of two 3D vectors.
    ///
    ////////////////////////////////////////////////////////////
    constexpr T dot(const Vector3& rhs) const;

    ////////////////////////////////////////////////////////////
    /// \brief Cross product of two 3D vectors.
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector3 cross(const Vector3& rhs) const;

    ////////////////////////////////////////////////////////////
    /// \brief Component-wise multiplication of \c *this and \c rhs.
    ///
    /// Computes <tt>(lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z)</tt>.
    ///
    /// Scaling is the most common use case for component-wise multiplication/division.
    /// This operation is also known as the Hadamard or Schur product.
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector3 cwiseMul(const Vector3& rhs) const;

    ////////////////////////////////////////////////////////////
    /// \brief Component-wise division of \c *this and \c rhs.
    ///
    /// Computes <tt>(lhs.x/rhs.x, lhs.y/rhs.y, lhs.z/rhs.z)</tt>.
    ///
    /// Scaling is the most common use case for component-wise multiplication/division.
    ///
    /// \pre Neither component of \c rhs is zero.
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector3 cwiseDiv(const Vector3& rhs) const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    T x{}; //!< X coordinate of the vector
    T y{}; //!< Y coordinate of the vector
    T z{}; //!< Z coordinate of the vector
};

////////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of unary operator -
///
/// \param left Vector to negate
///
/// \return Memberwise opposite of the vector
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector3<T> operator-(const Vector3<T>& left);

////////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of binary operator +=
///
/// This operator performs a memberwise addition of both vectors,
/// and assigns the result to \a left.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector3<T>& operator+=(Vector3<T>& left, const Vector3<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of binary operator -=
///
/// This operator performs a memberwise subtraction of both vectors,
/// and assigns the result to \a left.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector3<T>& operator-=(Vector3<T>& left, const Vector3<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of binary operator +
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return Memberwise addition of both vectors
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector3<T> operator+(const Vector3<T>& left, const Vector3<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of binary operator -
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return Memberwise subtraction of both vectors
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector3<T> operator-(const Vector3<T>& left, const Vector3<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of binary operator *
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a scalar value)
///
/// \return Memberwise multiplication by \a right
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector3<T> operator*(const Vector3<T>& left, T right);

////////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of binary operator *
///
/// \param left  Left operand (a scalar value)
/// \param right Right operand (a vector)
///
/// \return Memberwise multiplication by \a left
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector3<T> operator*(T left, const Vector3<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of binary operator *=
///
/// This operator performs a memberwise multiplication by \a right,
/// and assigns the result to \a left.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a scalar value)
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector3<T>& operator*=(Vector3<T>& left, T right);

////////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of binary operator /
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a scalar value)
///
/// \return Memberwise division by \a right
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector3<T> operator/(const Vector3<T>& left, T right);

////////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of binary operator /=
///
/// This operator performs a memberwise division by \a right,
/// and assigns the result to \a left.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a scalar value)
///
/// \return Reference to \a left
///
////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector3<T>& operator/=(Vector3<T>& left, T right);

////////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of binary operator ==
///
/// This operator compares strict equality between two vectors.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return True if \a left is equal to \a right
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr bool operator==(const Vector3<T>& left, const Vector3<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of binary operator !=
///
/// This operator compares strict difference between two vectors.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return True if \a left is not equal to \a right
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr bool operator!=(const Vector3<T>& left, const Vector3<T>& right);

#include <SFML/System/Vector3.inl>

// Define the most common types
using Vector3i = Vector3<int>;
using Vector3f = Vector3<float>;

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::Vector3
/// \ingroup system
///
/// sf::Vector3 is a simple class that defines a mathematical
/// vector with three coordinates (x, y and z). It can be used to
/// represent anything that has three dimensions: a size, a point,
/// a velocity, etc.
///
/// The template parameter T is the type of the coordinates. It
/// can be any type that supports arithmetic operations (+, -, /, *)
/// and comparisons (==, !=), for example int or float.
/// Note that some operations are only meaningful for vectors where T is
/// a floating point type (e.g. float or double), often because
/// results cannot be represented accurately with integers.
/// The method documentation mentions "(floating-point)" in those cases.
///
/// You generally don't have to care about the templated form (sf::Vector3<T>),
/// the most common specializations have special type aliases:
/// \li sf::Vector3<float> is sf::Vector3f
/// \li sf::Vector3<int> is sf::Vector3i
///
/// The sf::Vector3 class has a small and simple interface, its x, y and z members
/// can be accessed directly (there are no accessors like setX(), getX()).
///
/// Usage example:
/// \code
/// sf::Vector3f v(16.5f, 24.f, -3.2f);
/// v.x = 18.2f;
/// float y = v.y;
///
/// sf::Vector3f w = v * 5.f;
/// sf::Vector3f u;
/// u = v + w;
///
/// float s = v.dot(w);
/// sf::Vector3f t = v.cross(w);
///
/// bool different = (v != u);
/// \endcode
///
/// Note: for 2-dimensional vectors, see sf::Vector2.
///
////////////////////////////////////////////////////////////

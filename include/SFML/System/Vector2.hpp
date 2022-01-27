////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2021 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_VECTOR2_HPP
#define SFML_VECTOR2_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "math.h"

namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Utility template class for manipulating
///        2-dimensional vectors
///
////////////////////////////////////////////////////////////
template <typename T>
class Vector2
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates a Vector2(0, 0).
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector2();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the vector from its coordinates
    ///
    /// \param X X coordinate
    /// \param Y Y coordinate
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector2(T X, T Y);

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
    constexpr explicit Vector2(const Vector2<U>& vector);

    ////////////////////////////////////////////////////////////
    /// \relates Vector2
    /// \brief Obtain magnitude of calling vector
    ///
    /// This function determines the linear magnitude of a
    /// vector using Pythagorean's theorem
    ///
    ///
    /// \return Floating point value for vector magnitude
    ///
    ////////////////////////////////////////////////////////////
    constexpr float mag() const;

    ////////////////////////////////////////////////////////////
    /// \relates Vector2
    /// \brief Obtain the heading (direction) of calling vector
    ///
    /// This function determines the direction a vector is 
    /// pointing relative to the positive x-axis.
    ///
    ///
    /// \return Double representation of heading angle in radians
    ///
    ////////////////////////////////////////////////////////////
    constexpr double heading() const;

    ////////////////////////////////////////////////////////////
    /// \relates Vector2
    /// \brief Statically rotates vector by specified amount
    ///
    /// This function rotates a vector by specified radians
    ///
    /// \param vector Vector to rotate
    /// \param rad Number of Radians to rotate vector
    ///
    /// \return Vector2<double> of rotated parameter vector
    ///
    ////////////////////////////////////////////////////////////
    static constexpr Vector2<double> rotate(const Vector2<T>& vector, float rad);

    ////////////////////////////////////////////////////////////
    /// \relates Vector2
    /// \brief Statically determine vector an angle from 
    /// another vector
    ///
    /// This function determines the vector a particular
    /// angle away from a specified vector. If none is provided
    /// it will assume a reverence unit vector at (1, 0), at
    /// 0 radians. 
    ///
    /// \param rad Number of Radians between vectors
    /// \param vector Starting reference vector
    ///
    /// \return Vector2<double> of new vector at specified angle
    /// from reference
    ///
    ////////////////////////////////////////////////////////////
    static constexpr Vector2<double> fromAngle(float rad, Vector2<T> vector = Vector2<T>(1,0));

    ////////////////////////////////////////////////////////////
    /// \relates Vector2
    /// \brief Statically calculates distance between two vectors
    ///
    /// This function determines the Euclidean distance between
    /// two points represented by each vector
    ///
    /// \param vector1 First point
    /// \param vector2 Second point
    ///
    /// \return Double representing linear distance between points
    ///
    ////////////////////////////////////////////////////////////
    static constexpr double dist(const Vector2<T>& vector1, const Vector2<T>& vector2);

    ////////////////////////////////////////////////////////////
    /// \relates Vector2
    /// \brief Statically calculates the dot-product vectors
    ///
    /// This function calculates the dot-product of two vectors
    ///
    /// \param vector1 First vector
    /// \param vector2 Second vector
    ///
    /// \return Type T representing the vector dot-product
    ///
    ////////////////////////////////////////////////////////////
    static constexpr T dot(const Vector2<T>& vector1, const Vector2<T>& vector2);

    ////////////////////////////////////////////////////////////
    /// \relates Vector2
    /// \brief Statically determine the angle between vectors
    ///
    /// This function determines the angle between two vectors
    /// in radians 
    ///
    /// \param vector1 First vector
    /// \param vector2 Second vector
    ///
    /// \return Double representing the angle between in radians
    ///
    ////////////////////////////////////////////////////////////
    static constexpr double angleBetween(const Vector2<T>& vector1, const Vector2<T>& vector2);

    ////////////////////////////////////////////////////////////
    /// \relates Vector2
    /// \brief Statically obtain the unit-vector of a vector
    ///
    /// This function normalizes the coordinates, obtaining the
    /// vector's unit-vector
    ///
    /// \param vector Vector to normalize
    ///
    /// \return Vector2<double> of representing the unit-vector
    ///
    ////////////////////////////////////////////////////////////
    static constexpr Vector2<double> normalize(const Vector2<T>& vector);


    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    T x; //!< X coordinate of the vector
    T y; //!< Y coordinate of the vector
};

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of unary operator -
///
/// \param right Vector to negate
///
/// \return Memberwise opposite of the vector
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector2<T> operator -(const Vector2<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector2
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
constexpr Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector2
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
constexpr Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary operator +
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return Memberwise addition of both vectors
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary operator -
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return Memberwise subtraction of both vectors
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary operator *
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a scalar value)
///
/// \return Memberwise multiplication by \a right
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector2<T> operator *(const Vector2<T>& left, T right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary operator *
///
/// \param left  Left operand (a scalar value)
/// \param right Right operand (a vector)
///
/// \return Memberwise multiplication by \a left
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector2<T> operator *(T left, const Vector2<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector2
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
constexpr Vector2<T>& operator *=(Vector2<T>& left, T right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary operator /
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a scalar value)
///
/// \return Memberwise division by \a right
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector2<T> operator /(const Vector2<T>& left, T right);

////////////////////////////////////////////////////////////
/// \relates Vector2
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
constexpr Vector2<T>& operator /=(Vector2<T>& left, T right);

////////////////////////////////////////////////////////////
/// \relates Vector2
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
[[nodiscard]] constexpr bool operator ==(const Vector2<T>& left, const Vector2<T>& right);

////////////////////////////////////////////////////////////
/// \relates Vector2
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
[[nodiscard]] constexpr bool operator !=(const Vector2<T>& left, const Vector2<T>& right);

#include <SFML/System/Vector2.inl>

// Define the most common types
using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned int>;
using Vector2f = Vector2<float>;

} // namespace sf


#endif // SFML_VECTOR2_HPP


////////////////////////////////////////////////////////////
/// \class sf::Vector2
/// \ingroup system
///
/// sf::Vector2 is a simple class that defines a mathematical
/// vector with two coordinates (x and y). It can be used to
/// represent anything that has two dimensions: a size, a point,
/// a velocity, etc.
///
/// The template parameter T is the type of the coordinates. It
/// can be any type that supports arithmetic operations (+, -, /, *)
/// and comparisons (==, !=), for example int or float.
///
/// You generally don't have to care about the templated form (sf::Vector2<T>),
/// the most common specializations have special type aliases:
/// \li sf::Vector2<float> is sf::Vector2f
/// \li sf::Vector2<int> is sf::Vector2i
/// \li sf::Vector2<unsigned int> is sf::Vector2u
///
/// The sf::Vector2 class has a small and simple interface, its x and y members
/// can be accessed directly (there are no accessors like setX(), getX()) and it
/// contains no mathematical function like dot product, cross product, length, etc.
///
/// Usage example:
/// \code
/// sf::Vector2f v1(16.5f, 24.f);
/// v1.x = 18.2f;
/// float y = v1.y;
///
/// sf::Vector2f v2 = v1 * 5.f;
/// sf::Vector2f v3;
/// v3 = v1 + v2;
///
/// bool different = (v2 != v3);
/// \endcode
///
/// Note: for 3-dimensional vectors, see sf::Vector3.
///
////////////////////////////////////////////////////////////

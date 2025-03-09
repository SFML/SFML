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

#pragma once

#include <SFML/System/Export.hpp>

#include <SFML/System/Angle.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Class template for manipulating
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
    /// Creates a `Vector2(0, 0)`.
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector2() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Construct the vector from cartesian coordinates
    ///
    /// \param x X coordinate
    /// \param y Y coordinate
    ///
    ////////////////////////////////////////////////////////////
    constexpr Vector2(T x, T y);

    ////////////////////////////////////////////////////////////
    /// \brief Converts the vector to another type of vector
    ///
    ////////////////////////////////////////////////////////////
    template <typename U>
    constexpr explicit operator Vector2<U>() const;

    ////////////////////////////////////////////////////////////
    /// \brief Construct the vector from polar coordinates <i><b>(floating-point)</b></i>
    ///
    /// \param r   Length of vector (can be negative)
    /// \param phi Angle from X axis
    ///
    /// Note that this constructor is lossy: calling `length()` and `angle()`
    /// may return values different to those provided in this constructor.
    ///
    /// In particular, these transforms can be applied:
    /// * `Vector2(r, phi) == Vector2(-r, phi + 180_deg)`
    /// * `Vector2(r, phi) == Vector2(r, phi + n * 360_deg)`
    ///
    ////////////////////////////////////////////////////////////
    SFML_SYSTEM_API Vector2(T r, Angle phi);

    ////////////////////////////////////////////////////////////
    /// \brief Length of the vector <i><b>(floating-point)</b></i>.
    ///
    /// If you are not interested in the actual length, but only in comparisons, consider using `lengthSquared()`.
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] SFML_SYSTEM_API T length() const;

    ////////////////////////////////////////////////////////////
    /// \brief Square of vector's length.
    ///
    /// Suitable for comparisons, more efficient than `length()`.
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] constexpr T lengthSquared() const;

    ////////////////////////////////////////////////////////////
    /// \brief Vector with same direction but length 1 <i><b>(floating-point)</b></i>.
    ///
    /// \pre `*this` is no zero vector.
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] SFML_SYSTEM_API Vector2 normalized() const;

    ////////////////////////////////////////////////////////////
    /// \brief Signed angle from `*this` to `rhs` <i><b>(floating-point)</b></i>.
    ///
    /// \return The smallest angle which rotates `*this` in positive
    /// or negative direction, until it has the same direction as `rhs`.
    /// The result has a sign and lies in the range [-180, 180) degrees.
    /// \pre Neither `*this` nor `rhs` is a zero vector.
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] SFML_SYSTEM_API Angle angleTo(Vector2 rhs) const;

    ////////////////////////////////////////////////////////////
    /// \brief Signed angle from +X or (1,0) vector <i><b>(floating-point)</b></i>.
    ///
    /// For example, the vector (1,0) corresponds to 0 degrees, (0,1) corresponds to 90 degrees.
    ///
    /// \return Angle in the range [-180, 180) degrees.
    /// \pre This vector is no zero vector.
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] SFML_SYSTEM_API Angle angle() const;

    ////////////////////////////////////////////////////////////
    /// \brief Rotate by angle \c phi <i><b>(floating-point)</b></i>.
    ///
    /// Returns a vector with same length but different direction.
    ///
    /// In SFML's default coordinate system with +X right and +Y down,
    /// this amounts to a clockwise rotation by `phi`.
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] SFML_SYSTEM_API Vector2 rotatedBy(Angle phi) const;

    ////////////////////////////////////////////////////////////
    /// \brief Projection of this vector onto `axis` <i><b>(floating-point)</b></i>.
    ///
    /// \param axis Vector being projected onto. Need not be normalized.
    /// \pre `axis` must not have length zero.
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] SFML_SYSTEM_API Vector2 projectedOnto(Vector2 axis) const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns a perpendicular vector.
    ///
    /// Returns `*this` rotated by +90 degrees; (x,y) becomes (-y,x).
    /// For example, the vector (1,0) is transformed to (0,1).
    ///
    /// In SFML's default coordinate system with +X right and +Y down,
    /// this amounts to a clockwise rotation.
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] constexpr Vector2 perpendicular() const;

    ////////////////////////////////////////////////////////////
    /// \brief Dot product of two 2D vectors.
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] constexpr T dot(Vector2 rhs) const;

    ////////////////////////////////////////////////////////////
    /// \brief Z component of the cross product of two 2D vectors.
    ///
    /// Treats the operands as 3D vectors, computes their cross product
    /// and returns the result's Z component (X and Y components are always zero).
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] constexpr T cross(Vector2 rhs) const;

    ////////////////////////////////////////////////////////////
    /// \brief Component-wise multiplication of `*this` and `rhs`.
    ///
    /// Computes `(lhs.x*rhs.x, lhs.y*rhs.y)`.
    ///
    /// Scaling is the most common use case for component-wise multiplication/division.
    /// This operation is also known as the Hadamard or Schur product.
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] constexpr Vector2 componentWiseMul(Vector2 rhs) const;

    ////////////////////////////////////////////////////////////
    /// \brief Component-wise division of `*this` and `rhs`.
    ///
    /// Computes `(lhs.x/rhs.x, lhs.y/rhs.y)`.
    ///
    /// Scaling is the most common use case for component-wise multiplication/division.
    ///
    /// \pre Neither component of `rhs` is zero.
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] constexpr Vector2 componentWiseDiv(Vector2 rhs) const;


    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    T x{}; //!< X coordinate of the vector
    T y{}; //!< Y coordinate of the vector
};

// Define the most common types
using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned int>;
using Vector2f = Vector2<float>;

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of unary `operator-`
///
/// \param right Vector to negate
///
/// \return Member-wise opposite of the vector
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector2<T> operator-(Vector2<T> right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary `operator+=`
///
/// This operator performs a member-wise addition of both vectors,
/// and assigns the result to `left`.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return Reference to `left`
///
////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T>& operator+=(Vector2<T>& left, Vector2<T> right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary `operator-=`
///
/// This operator performs a member-wise subtraction of both vectors,
/// and assigns the result to `left`.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return Reference to \c left
///
////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T>& operator-=(Vector2<T>& left, Vector2<T> right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary `operator+`
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return Member-wise addition of both vectors
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector2<T> operator+(Vector2<T> left, Vector2<T> right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary `operator-`
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return Member-wise subtraction of both vectors
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector2<T> operator-(Vector2<T> left, Vector2<T> right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary `operator*`
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a scalar value)
///
/// \return Member-wise multiplication by `right`
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector2<T> operator*(Vector2<T> left, T right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary `operator*`
///
/// \param left  Left operand (a scalar value)
/// \param right Right operand (a vector)
///
/// \return Member-wise multiplication by `left`
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector2<T> operator*(T left, Vector2<T> right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary `operator*=`
///
/// This operator performs a member-wise multiplication by `right`,
/// and assigns the result to `left`.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a scalar value)
///
/// \return Reference to `left`
///
////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T>& operator*=(Vector2<T>& left, T right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary `operator/`
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a scalar value)
///
/// \return Member-wise division by `right`
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr Vector2<T> operator/(Vector2<T> left, T right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary `operator/=`
///
/// This operator performs a member-wise division by `right`,
/// and assigns the result to `left`.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a scalar value)
///
/// \return Reference to `left`
///
////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T>& operator/=(Vector2<T>& left, T right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary `operator==`
///
/// This operator compares strict equality between two vectors.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return `true` if `left` is equal to `right`
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr bool operator==(Vector2<T> left, Vector2<T> right);

////////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Overload of binary `operator!=`
///
/// This operator compares strict difference between two vectors.
///
/// \param left  Left operand (a vector)
/// \param right Right operand (a vector)
///
/// \return `true` if `left` is not equal to `right`
///
////////////////////////////////////////////////////////////
template <typename T>
[[nodiscard]] constexpr bool operator!=(Vector2<T> left, Vector2<T> right);

} // namespace sf

#include <SFML/System/Vector2.inl>


////////////////////////////////////////////////////////////
/// \class sf::Vector2
/// \ingroup system
///
/// `sf::Vector2` is a simple class that defines a mathematical
/// vector with two coordinates (x and y). It can be used to
/// represent anything that has two dimensions: a size, a point,
/// a velocity, a scale, etc.
///
/// The API provides basic arithmetic (addition, subtraction, scale), as
/// well as more advanced geometric operations, such as dot/cross products,
/// length and angle computations, projections, rotations, etc.
///
/// The template parameter T is the type of the coordinates. It
/// can be any type that supports arithmetic operations (+, -, /, *)
/// and comparisons (==, !=), for example int or float.
/// Note that some operations are only meaningful for vectors where T is
/// a floating point type (e.g. float or double), often because
/// results cannot be represented accurately with integers.
/// The method documentation mentions "(floating-point)" in those cases.
///
/// You generally don't have to care about the templated form (`sf::Vector2<T>`),
/// the most common specializations have special type aliases:
/// \li `sf::Vector2<float>` is `sf::Vector2f`
/// \li `sf::Vector2<int>` is `sf::Vector2i`
/// \li `sf::Vector2<unsigned int>` is `sf::Vector2u`
///
/// The `sf::Vector2` class has a simple interface, its x and y members
/// can be accessed directly (there are no accessors like setX(), getX()).
///
/// Usage example:
/// \code
/// sf::Vector2f v(16.5f, 24.f);
/// v.x = 18.2f;
/// float y = v.y;
///
/// sf::Vector2f w = v * 5.f;
/// sf::Vector2f u;
/// u = v + w;
///
/// float s = v.dot(w);
///
/// bool different = (v != u);
/// \endcode
///
/// Note: for 3-dimensional vectors, see `sf::Vector3`.
///
////////////////////////////////////////////////////////////

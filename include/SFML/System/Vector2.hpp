////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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


namespace sf
{
////////////////////////////////////////////////////////////
/// Vector2 is an utility class for manipulating 2 dimensional
/// vectors. Template parameter defines the type of coordinates
/// (integer, float, ...)
////////////////////////////////////////////////////////////
template <typename T>
class Vector2
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Vector2();

    ////////////////////////////////////////////////////////////
    /// Construct the vector from its coordinates
    ///
    /// \param X : X coordinate
    /// \param Y : Y coordinate
    ///
    ////////////////////////////////////////////////////////////
    Vector2(T X, T Y);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    T x; ///< X coordinate of the vector
    T y; ///< Y coordinate of the vector
};

////////////////////////////////////////////////////////////
/// Operator - overload ; returns the opposite of a vector
///
/// \param V : Vector to negate
///
/// \return -V
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator -(const Vector2<T>& V);

////////////////////////////////////////////////////////////
/// Operator += overload ; add two vectors and assign to the first op
///
/// \param V1 : First vector
/// \param V2 : Second vector
///
/// \return V1 + V2
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T>& operator +=(Vector2<T>& V1, const Vector2<T>& V2);

////////////////////////////////////////////////////////////
/// Operator -= overload ; subtract two vectors and assign to the first op
///
/// \param V1 : First vector
/// \param V2 : Second vector
///
/// \return V1 - V2
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T>& operator -=(Vector2<T>& V1, const Vector2<T>& V2);

////////////////////////////////////////////////////////////
/// Operator + overload ; adds two vectors
///
/// \param V1 : First vector
/// \param V2 : Second vector
///
/// \return V1 + V2
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator +(const Vector2<T>& V1, const Vector2<T>& V2);

////////////////////////////////////////////////////////////
/// Operator - overload ; subtracts two vectors
///
/// \param V1 : First vector
/// \param V2 : Second vector
///
/// \return V1 - V2
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator -(const Vector2<T>& V1, const Vector2<T>& V2);

////////////////////////////////////////////////////////////
/// Operator * overload ; multiply a vector by a scalar value
///
/// \param V : Vector
/// \param X : Scalar value
///
/// \return V * X
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator *(const Vector2<T>& V, T X);

////////////////////////////////////////////////////////////
/// Operator * overload ; multiply a scalar value by a vector
///
/// \param X : Scalar value
/// \param V : Vector
///
/// \return X * V
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator *(T X, const Vector2<T>& V);

////////////////////////////////////////////////////////////
/// Operator *= overload ; multiply-assign a vector by a scalar value
///
/// \param V : Vector
/// \param X : Scalar value
///
/// \return V * X
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T>& operator *=(Vector2<T>& V, T X);

////////////////////////////////////////////////////////////
/// Operator / overload ; divide a vector by a scalar value
///
/// \param V : Vector
/// \param X : Scalar value
///
/// \return V / X
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T> operator /(const Vector2<T>& V, T X);

////////////////////////////////////////////////////////////
/// Operator /= overload ; divide-assign a vector by a scalar value
///
/// \param V : Vector
/// \param X : Scalar value
///
/// \return V / X
///
////////////////////////////////////////////////////////////
template <typename T>
Vector2<T>& operator /=(Vector2<T>& V, T X);

////////////////////////////////////////////////////////////
/// Operator == overload ; compares the equality of two vectors
///
/// \param V1 : First vector
/// \param V2 : Second vector
///
/// \return True if V1 is equal to V2
///
////////////////////////////////////////////////////////////
template <typename T>
bool operator ==(const Vector2<T>& V1, const Vector2<T>& V2);

////////////////////////////////////////////////////////////
/// Operator != overload ; compares the difference of two vectors
///
/// \param V1 : First vector
/// \param V2 : Second vector
///
/// \return True if V1 is different than V2
///
////////////////////////////////////////////////////////////
template <typename T>
bool operator !=(const Vector2<T>& V1, const Vector2<T>& V2);

#include <SFML/System/Vector2.inl>

// Define the most common types
typedef Vector2<int>   Vector2i;
typedef Vector2<float> Vector2f;

} // namespace sf


#endif // SFML_VECTOR2_HPP

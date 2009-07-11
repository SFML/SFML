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

#ifndef SFML_MATRIX3_HPP
#define SFML_MATRIX3_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <math.h>


namespace sf
{
////////////////////////////////////////////////////////////
/// Utility class to manipulate 3x3 matrices representing
/// 2D transformations
////////////////////////////////////////////////////////////
class SFML_API Matrix3
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor (builds an identity matrix)
    ///
    ////////////////////////////////////////////////////////////
    Matrix3();

    ////////////////////////////////////////////////////////////
    /// Construct a matrix from its 9 elements
    ///
    ////////////////////////////////////////////////////////////
    Matrix3(float a00, float a01, float a02,
            float a10, float a11, float a12,
            float a20, float a21, float a22);

    ////////////////////////////////////////////////////////////
    /// Build a matrix from a set of transformations
    ///
    /// \param origin :      Origin for the transformations
    /// \param translation : Translation offset
    /// \param rotation :    Rotation angle in degrees
    /// \param scale :       Scaling factors
    ///
    ////////////////////////////////////////////////////////////
    void SetFromTransformations(const Vector2f& origin, const Vector2f& translation, float rotation, const Vector2f& scale);

    ////////////////////////////////////////////////////////////
    /// Build a matrix from a projection
    ///
    /// \param center :   Center of the view
    /// \param size :     Size of the view
    /// \param rotation : Angle of rotation of the view rectangle, in degrees
    ///
    ////////////////////////////////////////////////////////////
    void SetFromProjection(const Vector2f& center, const Vector2f& size, float rotation);

    ////////////////////////////////////////////////////////////
    /// Transform a point by the matrix
    ///
    /// \param point : Point to transform
    ///
    /// \return Transformed point
    ///
    ////////////////////////////////////////////////////////////
    Vector2f Transform(const Vector2f& point) const;

    ////////////////////////////////////////////////////////////
    /// Return the inverse of the matrix
    ///
    /// \return A new matrix which is the inverse of this
    ///
    ////////////////////////////////////////////////////////////
    Matrix3 GetInverse() const;

    ////////////////////////////////////////////////////////////
    /// Return the elements of the matrix as a 4x4,
    /// in an array of 16 floats
    ///
    /// \return Pointer to the 4x4 matrix elements
    ///
    ////////////////////////////////////////////////////////////
    const float* Get4x4Elements() const;

    ////////////////////////////////////////////////////////////
    /// Operator () overloads to access the matrix elements
    ///
    /// \param row :    Element row (0 based)
    /// \param column : Element column (0 based)
    ///
    /// \return Matrix element (Row, Col)
    ///
    ////////////////////////////////////////////////////////////
    float  operator ()(unsigned int row, unsigned int column) const;
    float& operator ()(unsigned int row, unsigned int column);

    ////////////////////////////////////////////////////////////
    /// Operator * overload to multiply two matrices
    ///
    /// \param right : Matrix to multiply
    ///
    /// \return this * right
    ///
    ////////////////////////////////////////////////////////////
    Matrix3 operator *(const Matrix3& right) const;

    ////////////////////////////////////////////////////////////
    /// Operator *= overload to multiply-assign two matrices
    ///
    /// \param right : Matrix to multiply
    ///
    /// \return this * right
    ///
    ////////////////////////////////////////////////////////////
    Matrix3& operator *=(const Matrix3& right);

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    static const Matrix3 Identity; ///< Identity matrix

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    float myData[16]; /// Matrix elements (we directly store it as a 4x4 matrix for optimization purpose)
};

#include <SFML/Graphics/Matrix3.inl>

} // namespace sf


#endif // SFML_MATRIX3_HPP

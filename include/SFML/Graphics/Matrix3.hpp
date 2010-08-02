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
#include <cmath>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Utility class to manipulate 3x3 matrices of floats
///
////////////////////////////////////////////////////////////
class SFML_API Matrix3
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor creates an identity matrix.
    ///
    ////////////////////////////////////////////////////////////
    Matrix3();

    ////////////////////////////////////////////////////////////
    /// \brief Construct a matrix from its 9 elements
    ///
    /// \param a00 Element (0, 0) of the matrix
    /// \param a01 Element (0, 1) of the matrix
    /// \param a02 Element (0, 2) of the matrix
    /// \param a10 Element (1, 0) of the matrix
    /// \param a11 Element (1, 1) of the matrix
    /// \param a12 Element (1, 2) of the matrix
    /// \param a20 Element (2, 0) of the matrix
    /// \param a21 Element (2, 1) of the matrix
    /// \param a22 Element (2, 2) of the matrix
    ///
    ////////////////////////////////////////////////////////////
    Matrix3(float a00, float a01, float a02,
            float a10, float a11, float a12,
            float a20, float a21, float a22);

    ////////////////////////////////////////////////////////////
    /// \brief Transform a point by the matrix
    ///
    /// \param point Point to transform
    ///
    /// \return Transformed point
    ///
    ////////////////////////////////////////////////////////////
    Vector2f Transform(const Vector2f& point) const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the inverse of the matrix
    ///
    /// If the inverse cannot be computed, the identity matrix
    /// is returned.
    ///
    /// \return A new matrix which is the inverse of self
    ///
    ////////////////////////////////////////////////////////////
    Matrix3 GetInverse() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the elements of the matrix
    ///
    /// This function returns an array of 16 floats containing
    /// the corresponding 4x4 matrix, so that it is directly
    /// compatible with OpenGL functions.
    ///
    /// \return Pointer to the 4x4 matrix elements
    ///
    ////////////////////////////////////////////////////////////
    const float* Get4x4Elements() const;

    ////////////////////////////////////////////////////////////
    /// \brief Overload of binary operator *
    ///
    /// \param right Right operand of the multiplication
    ///
    /// \return New matrix which is the result of self * \a right
    ///
    ////////////////////////////////////////////////////////////
    Matrix3 operator *(const Matrix3& right) const;

    ////////////////////////////////////////////////////////////
    /// \brief Build a matrix from a set of transformations
    ///
    /// \param origin      Origin for the transformations
    /// \param translation Translation offset
    /// \param rotation    Rotation angle in degrees
    /// \param scale       Scaling factors
    ///
    /// \return New Matrix3 containing the transformations
    ///
    /// \see Projection
    ///
    ////////////////////////////////////////////////////////////
    static Matrix3 Transformation(const Vector2f& origin, const Vector2f& translation, float rotation, const Vector2f& scale);

    ////////////////////////////////////////////////////////////
    /// \brief Build a 2D project matrix
    ///
    /// \param center    Center of the view
    /// \param size      Size of the view
    /// \param rotation  Angle of rotation of the view, in degrees
    ///
    /// \return New Matrix3 containing the projection
    ///
    /// \see Transformation
    ///
    ////////////////////////////////////////////////////////////
    static Matrix3 Projection(const Vector2f& center, const Vector2f& size, float rotation);

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    static const Matrix3 Identity; ///< The identity matrix

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    float myData[16]; /// Matrix elements (we directly store it as a 4x4 matrix for optimization purpose)
};

#include <SFML/Graphics/Matrix3.inl>

} // namespace sf


#endif // SFML_MATRIX3_HPP


////////////////////////////////////////////////////////////
/// \class sf::Matrix3
/// \ingroup graphics
///
/// Matrix3 is only meant for internal use, its interface is
/// limited and its implementation is optimized for OpenGL
/// rendering.
///
/// This type is not used at all in the public API of SFML.
///
////////////////////////////////////////////////////////////

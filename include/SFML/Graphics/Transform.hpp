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

#ifndef SFML_TRANSFORM_HPP
#define SFML_TRANSFORM_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Define a 3x3 transform matrix
///
////////////////////////////////////////////////////////////
class SFML_API Transform
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an identity transform (a transform that does nothing).
    ///
    ////////////////////////////////////////////////////////////
    Transform();

    ////////////////////////////////////////////////////////////
    /// \brief Construct a transform from a 3x3 matrix
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
    Transform(float a00, float a01, float a02,
              float a10, float a11, float a12,
              float a20, float a21, float a22);

    ////////////////////////////////////////////////////////////
    /// \brief Return the transform as a 4x4 matrix
    ///
    /// This function returns a pointer to an array of 16 floats
    /// containing the transform elements as a 4x4 matrix, which
    /// is directly compatible with OpenGL functions.
    ///
    /// \code
    /// sf::Transform transform = ...;
    /// glLoadMatrixf(transform.GetMatrix());
    /// \endcode
    ///
    /// \return Pointer to a 4x4 matrix
    ///
    ////////////////////////////////////////////////////////////
    const float* GetMatrix() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the inverse of the transform
    ///
    /// If the inverse cannot be computed, an identity transform
    /// is returned.
    ///
    /// \return A new transform which is the inverse of self
    ///
    ////////////////////////////////////////////////////////////
    Transform GetInverse() const;

    ////////////////////////////////////////////////////////////
    /// \brief Transform a 2D point
    ///
    /// \param x X coordinate of the point to transform
    /// \param y Y coordinate of the point to transform
    ///
    /// \return Transformed point
    ///
    ////////////////////////////////////////////////////////////
    Vector2f TransformPoint(float x, float y) const;

    ////////////////////////////////////////////////////////////
    /// \brief Transform a 2D point
    ///
    /// \param point Point to transform
    ///
    /// \return Transformed point
    ///
    ////////////////////////////////////////////////////////////
    Vector2f TransformPoint(const Vector2f& point) const;

    ////////////////////////////////////////////////////////////
    /// \brief Transform a rectangle
    ///
    /// Since SFML doesn't provide support for oriented rectangles,
    /// the result of this function is always an axis-aligned
    /// rectangle. Which means that if the transform contains a
    /// rotation, the bounding rectangle of the transformed rectangle
    /// is returned.
    ///
    /// \param rectangle Rectangle to transform
    ///
    /// \return Transformed rectangle
    ///
    ////////////////////////////////////////////////////////////
    FloatRect TransformRect(const FloatRect& rectangle) const;

    ////////////////////////////////////////////////////////////
    /// \brief Combine two transforms
    ///
    /// The result is a transform that is equivalent to applying
    /// *this followed by \a transform. Mathematically, it is
    /// equivalent to a matrix multiplication.
    ///
    /// \param transform Transform to combine to this transform
    ///
    /// \return New combined transform
    ///
    ////////////////////////////////////////////////////////////
    Transform Combine(const Transform& transform) const;

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a translation
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.Translate(100, 200).Rotate(45);
    /// \endcode
    ///
    /// \param x Offset to apply on X axis
    /// \param y Offset to apply on Y axis
    ///
    /// \return Reference to *this
    ///
    /// \see Rotate, Scale
    ///
    ////////////////////////////////////////////////////////////
    Transform& Translate(float x, float y);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a translation
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.Translate(sf::Vector2f(100, 200)).Rotate(45);
    /// \endcode
    ///
    /// \param offset Translation offset to apply
    ///
    /// \return Reference to *this
    ///
    /// \see Rotate, Scale
    ///
    ////////////////////////////////////////////////////////////
    Transform& Translate(const Vector2f& offset);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a rotation
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.Rotate(90).Translate(50, 20);
    /// \endcode
    ///
    /// \param angle Rotation angle, in degrees
    ///
    /// \return Reference to *this
    ///
    /// \see Translate, Scale
    ///
    ////////////////////////////////////////////////////////////
    Transform& Rotate(float angle);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a rotation
    ///
    /// The center of rotation is provided for convenience as a second
    /// argument, so that you can build rotations around arbitrary points
    /// more easily (and efficiently) than the usual
    /// Translate(-center).Rotate(angle).Translate(center).
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.Rotate(90, 8, 3).Translate(50, 20);
    /// \endcode
    ///
    /// \param angle Rotation angle, in degrees
    /// \param centerX X coordinate of the center of rotation
    /// \param centerY Y coordinate of the center of rotation
    ///
    /// \return Reference to *this
    ///
    /// \see Translate, Scale
    ///
    ////////////////////////////////////////////////////////////
    Transform& Rotate(float angle, float centerX, float centerY);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a rotation
    ///
    /// The center of rotation is provided for convenience as a second
    /// argument, so that you can build rotations around arbitrary points
    /// more easily (and efficiently) than the usual
    /// Translate(-center).Rotate(angle).Translate(center).
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.Rotate(90, sf::Vector2f(8, 3)).Translate(sf::Vector2f(50, 20));
    /// \endcode
    ///
    /// \param angle Rotation angle, in degrees
    /// \param center Center of rotation
    ///
    /// \return Reference to *this
    ///
    /// \see Translate, Scale
    ///
    ////////////////////////////////////////////////////////////
    Transform& Rotate(float angle, const Vector2f& center);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a scaling
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.Scale(2, 1).Rotate(45);
    /// \endcode
    ///
    /// \param scaleX Scaling factor on the X axis
    /// \param scaleY Scaling factor on the Y axis
    ///
    /// \return Reference to *this
    ///
    /// \see Translate, Rotate
    ///
    ////////////////////////////////////////////////////////////
    Transform& Scale(float scaleX, float scaleY);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a scaling
    ///
    /// The center of scaling is provided for convenience as a second
    /// argument, so that you can build scaling around arbitrary points
    /// more easily (and efficiently) than the usual
    /// Translate(-center).Scale(factors).Translate(center).
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.Scale(2, 1, 8, 3).Rotate(45);
    /// \endcode
    ///
    /// \param scaleX Scaling factor on X axis
    /// \param scaleY Scaling factor on Y axis
    /// \param centerX X coordinate of the center of scaling
    /// \param centerY Y coordinate of the center of scaling
    ///
    /// \return Reference to *this
    ///
    /// \see Translate, Rotate
    ///
    ////////////////////////////////////////////////////////////
    Transform& Scale(float scaleX, float scaleY, float centerX, float centerY);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a scaling
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.Scale(sf::Vector2f(2, 1)).Rotate(45);
    /// \endcode
    ///
    /// \param factors Scaling factors
    ///
    /// \return Reference to *this
    ///
    /// \see Translate, Rotate
    ///
    ////////////////////////////////////////////////////////////
    Transform& Scale(const Vector2f& factors);

    ////////////////////////////////////////////////////////////
    /// \brief Combine the current transform with a scaling
    ///
    /// The center of scaling is provided for convenience as a second
    /// argument, so that you can build scaling around arbitrary points
    /// more easily (and efficiently) than the usual
    /// Translate(-center).Scale(factors).Translate(center).
    ///
    /// This function returns a reference to *this, so that calls
    /// can be chained.
    /// \code
    /// sf::Transform transform;
    /// transform.Scale(sf::Vector2f(2, 1), sf::Vector2f(8, 3)).Rotate(45);
    /// \endcode
    ///
    /// \param factors Scaling factors
    /// \param center Center of scaling
    ///
    /// \return Reference to *this
    ///
    /// \see Translate, Rotate
    ///
    ////////////////////////////////////////////////////////////
    Transform& Scale(const Vector2f& factors, const Vector2f& center);

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    static const Transform Identity; ///< The identity transform (does nothing)

private:

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    float myMatrix[16]; ///< 4x4 matrix defining the transformation
};

////////////////////////////////////////////////////////////
/// \relates sf::Transform
/// \brief Overload of binary operator * to combine two transforms
///
/// This call is equivalent to calling left.Combine(right).
///
/// \param left Left operand (the first transform)
/// \param right Right operand (the second transform)
///
/// \return New combined transform
///
////////////////////////////////////////////////////////////
SFML_API Transform operator *(const Transform& left, const Transform& right);

////////////////////////////////////////////////////////////
/// \relates sf::Transform
/// \brief Overload of binary operator *= to combine two transforms
///
/// This call is equivalent to calling left = left.Combine(right).
///
/// \param left Left operand (the first transform)
/// \param right Right operand (the second transform)
///
/// \return The combined transform
///
////////////////////////////////////////////////////////////
SFML_API Transform& operator *=(Transform& left, const Transform& right);

////////////////////////////////////////////////////////////
/// \relates sf::Transform
/// \brief Overload of binary operator * to transform a point
///
/// This call is equivalent to calling left.TransformPoint(right).
///
/// \param left Left operand (the transform)
/// \param right Right operand (the point to transform)
///
/// \return New transformed point
///
////////////////////////////////////////////////////////////
SFML_API Vector2f operator *(const Transform& left, const Vector2f& right);

} // namespace sf


#endif // SFML_TRANSFORM_HPP


////////////////////////////////////////////////////////////
/// \class sf::Transform
/// \ingroup graphics
///
/// A sf::Transform specifies how to translate, rotate, scale,
/// shear, project, whatever things. In mathematical terms, it defines
/// how to transform a coordinate system into another.
///
/// For example, if you apply a rotation transform to a sprite, the
/// result will be a rotated sprite. And anything that is transformed
/// by this rotation transform will be rotated the same way, according
/// to its initial position.
///
/// Transforms are typically used for drawing. But they can also be
/// used for any computation that requires to transform points between
/// the local and global coordinate systems of an entity (like collision
/// detection).
///
/// Example:
/// \code
/// // define a translation transform
/// sf::Transform translation;
/// translation.Translate(20, 50);
///
/// // define a rotation transform
/// sf::Transform rotation;
/// rotation.Rotate(45);
///
/// // combine them
/// sf::Transform transform = translation * rotation;
///
/// // use the result to transform stuff...
/// sf::Vector2f point = transform.TransformPoint(10, 20);
/// sf::FloatRect rect = transform.TransformRect(sf::FloatRect(0, 0, 10, 100));
/// \endcode
///
/// \see sf::Transformable, sf::RenderStates
///
////////////////////////////////////////////////////////////

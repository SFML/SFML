////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_ANGLE_HPP
#define SFML_ANGLE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Export.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Represents an angle value
///
////////////////////////////////////////////////////////////
class SFML_SYSTEM_API Angle
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Sets the angle value to zero.
    ///
    ////////////////////////////////////////////////////////////
    Angle();

    ////////////////////////////////////////////////////////////
    /// \brief Implicit conversion from an angle in degrees to
    /// provide backward compatibility. You should use degrees()
    /// instead.
    ///
    /// \param angle Angle in degrees
    ///
    ////////////////////////////////////////////////////////////
    Angle(float angle);

    ////////////////////////////////////////////////////////////
    /// \brief Implicit conversion to a float number representing
    /// the angle in degrees to provide backward compatibility.
    /// You should use toDegrees() instead.
    ///
    /// \return Angle in degrees
    ///
    ////////////////////////////////////////////////////////////
    operator float () const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the angle's value in degrees
    ///
    /// \return Angle in degrees
    ///
    /// \see asRadians
    ///
    ////////////////////////////////////////////////////////////
    float asDegrees() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the angle's value in radians
    ///
    /// \return Angle in radians
    ///
    /// \see asDegree
    ///
    ////////////////////////////////////////////////////////////
    float asRadians() const;

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    static const Angle Zero; ///< Predefined 0 degree angle value

private:

    friend SFML_SYSTEM_API Angle degrees(float);
    friend SFML_SYSTEM_API Angle radians(float);

private:

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    float m_radians; ///< Angle value stored as radians
};

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Construct an angle value from a number of degrees
///
/// \param amount Number of degrees
///
/// \return Angle value constructed from the amount of degrees
///
/// \see radians
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle degrees(float angle);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Construct an angle value from a number of radians
///
/// \param amount Number of radians
///
/// \return Angle value constructed from the amount of radians
///
/// \see degrees
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle radians(float angle);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of == operator to compare two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return True if both angle values are equal
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API bool operator ==(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of != operator to compare two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return True if both angle values are different
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API bool operator !=(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of < operator to compare two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return True if \a left is lesser than \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API bool operator <(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of > operator to compare two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return True if \a left is greater than \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API bool operator >(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of <= operator to compare two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return True if \a left is lesser or equal than \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API bool operator <=(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of >= operator to compare two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return True if \a left is greater or equal than \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API bool operator >=(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of unary - operator to negate an angle value
///
/// \param right Right operand (an angle)
///
/// \return Opposite of the angle value
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle operator -(Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary + operator to add two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return Sum of the two angle values
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle operator +(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary += operator to add/assign two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return Sum of the two angle values
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle& operator +=(Angle& left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary - operator to subtract two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return Difference of the two angle values
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle operator -(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary -= operator to subtract/assign two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return Difference of the two angle values
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle& operator -=(Angle& left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary * operator to scale an angle value
///
/// \param left  Left operand (an angle)
/// \param right Right operand (a number)
///
/// \return \a left multiplied by \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle operator *(Angle left, float right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary * operator to scale an angle value
///
/// \param left  Left operand (an angle)
/// \param right Right operand (a number)
///
/// \return \a left multiplied by \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle operator *(Angle left, Int64 right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary * operator to scale an angle value
///
/// \param left  Left operand (a number)
/// \param right Right operand (an angle)
///
/// \return \a left multiplied by \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle operator *(float left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary * operator to scale an angle value
///
/// \param left  Left operand (a number)
/// \param right Right operand (an angle)
///
/// \return \a left multiplied by \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle operator *(Int64 left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary *= operator to scale/assign an angle value
///
/// \param left  Left operand (an angle)
/// \param right Right operand (a number)
///
/// \return \a left multiplied by \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle& operator *=(Angle& left, float right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary *= operator to scale/assign an angle value
///
/// \param left  Left operand (an angle)
/// \param right Right operand (a number)
///
/// \return \a left multiplied by \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle& operator *=(Angle& left, Int64 right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary / operator to scale an angle value
///
/// \param left  Left operand (an angle)
/// \param right Right operand (a number)
///
/// \return \a left divided by \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle operator /(Angle left, float right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary / operator to scale an angle value
///
/// \param left  Left operand (an angle)
/// \param right Right operand (a number)
///
/// \return \a left divided by \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle operator /(Angle left, Int64 right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary /= operator to scale/assign an angle value
///
/// \param left  Left operand (an angle)
/// \param right Right operand (a number)
///
/// \return \a left divided by \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle& operator /=(Angle& left, float right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary /= operator to scale/assign an angle value
///
/// \param left  Left operand (an angle)
/// \param right Right operand (a number)
///
/// \return \a left divided by \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle& operator /=(Angle& left, Int64 right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary / operator to compute the ratio of two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return \a left divided by \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API float operator /(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary % operator to compute remainder of an angle value
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return \a left modulo \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle operator %(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary %= operator to compute/assign remainder of an angle value
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return \a left modulo \a right
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API Angle& operator %=(Angle& left, Angle right);

} // namespace sf


#endif // SFML_ANGLE_HPP


////////////////////////////////////////////////////////////
/// \class sf::Angle
/// \ingroup system
///
/// sf::Angle encapsulates an angle value in a flexible way.
/// It allows to define an angle value either as a number of
/// degrees or radians. It also works the other way round:
/// you can read an angle value as either a number of
/// degrees or radians.
///
/// All angles are always normalized to the interval
/// <em>[0 - 360[</em> (in degree).
///
/// By using such a flexible interface, the API doesn't
/// impose any fixed type or resolution for angle values,
/// and let the user choose its own favorite representation.
///
/// Angle values support the usual mathematical operations:
/// you can add or subtract two angles, multiply or divide
/// an angle by a number, compare two angles, etc.
///
/// Usage example:
/// \code
/// sf::Angle a1  = sf::degree(90.f);
/// float radians = a1.asRadians(); // 1.5708
///
/// sf::Angle a2 = sf::radians(PI);
/// float degrees = a2.asDegree(); // 180
/// \endcode
///
////////////////////////////////////////////////////////////

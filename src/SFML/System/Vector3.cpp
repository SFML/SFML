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

#include <SFML/System/Vector3.hpp>

#include <cassert>
#include <cmath>
#include <type_traits>


namespace sf
{
////////////////////////////////////////////////////////////
template <typename T>
Vector3<T> Vector3<T>::normalized() const
{
    static_assert(std::is_floating_point_v<T>, "Vector3::normalized() is only supported for floating point types");

    assert(*this != Vector3<T>());
    return (*this) / length();
}


////////////////////////////////////////////////////////////
template <typename T>
T Vector3<T>::length() const
{
    static_assert(std::is_floating_point_v<T>, "Vector3::length() is only supported for floating point types");

    // don't use std::hypot because of slow performance
    return std::sqrt(x * x + y * y + z * z);
}

} // namespace sf


////////////////////////////////////////////////////////////
// Explicit instantiation definitions
////////////////////////////////////////////////////////////

template class sf::Vector3<float>;
template class sf::Vector3<double>;
template class sf::Vector3<long double>;

#define SFML_INSTANTIATE_VECTOR3_BASIC_MEMBER_FUNCTIONS(type) \
    template /*             */ sf::Vector3<type>::Vector3();  \
    template /*             */ sf::Vector3<type>::Vector3(type, type, type);

#define SFML_INSTANTIATE_VECTOR3_INTEGRAL_MEMBER_FUNCTIONS(type)                      \
    template type              sf::Vector3<type>::lengthSq() const;                   \
    template type              sf::Vector3<type>::dot(const Vector3& rhs) const;      \
    template sf::Vector3<type> sf::Vector3<type>::cross(const Vector3& rhs) const;    \
    template sf::Vector3<type> sf::Vector3<type>::cwiseMul(const Vector3& rhs) const; \
    template sf::Vector3<type> sf::Vector3<type>::cwiseDiv(const Vector3& rhs) const;

SFML_INSTANTIATE_VECTOR3_BASIC_MEMBER_FUNCTIONS(bool)
SFML_INSTANTIATE_VECTOR3_BASIC_MEMBER_FUNCTIONS(int)
SFML_INSTANTIATE_VECTOR3_BASIC_MEMBER_FUNCTIONS(unsigned int)

SFML_INSTANTIATE_VECTOR3_INTEGRAL_MEMBER_FUNCTIONS(int)
SFML_INSTANTIATE_VECTOR3_INTEGRAL_MEMBER_FUNCTIONS(unsigned int)

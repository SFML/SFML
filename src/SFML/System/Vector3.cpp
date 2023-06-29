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

#include <type_traits>

#include <cassert>
#include <cmath>


namespace sf
{
////////////////////////////////////////////////////////////
template <typename T>
Vector3<T> Vector3<T>::normalized() const
{
    static_assert(std::is_floating_point_v<T>, "Vector3::normalized() is only supported for floating point types");

    assert(*this != Vector3<T>() && "Vector3::normalized() cannot normalize a zero vector");
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
// Explicit template instantiations
////////////////////////////////////////////////////////////

template class sf::Vector3<float>;
template class sf::Vector3<double>;
template class sf::Vector3<long double>;

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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include <cstddef>


namespace sf
{
namespace priv
{
// Forward declarations
template <std::size_t Columns, std::size_t Rows>
struct Matrix;

template <typename T>
struct Vector4;

} // namespace priv


////////////////////////////////////////////////////////////
/// \brief Namespace with GLSL types
///
////////////////////////////////////////////////////////////
namespace Glsl
{

////////////////////////////////////////////////////////////
/// \brief 2D float vector (\p vec2 in GLSL)
///
////////////////////////////////////////////////////////////
using Vec2 = Vector2<float>;

////////////////////////////////////////////////////////////
/// \brief 2D int vector (\p ivec2 in GLSL)
///
////////////////////////////////////////////////////////////
using Ivec2 = Vector2<int>;

////////////////////////////////////////////////////////////
/// \brief 2D bool vector (\p bvec2 in GLSL)
///
////////////////////////////////////////////////////////////
using Bvec2 = Vector2<bool>;

////////////////////////////////////////////////////////////
/// \brief 3D float vector (\p vec3 in GLSL)
///
////////////////////////////////////////////////////////////
using Vec3 = Vector3<float>;

////////////////////////////////////////////////////////////
/// \brief 3D int vector (\p ivec3 in GLSL)
///
////////////////////////////////////////////////////////////
using Ivec3 = Vector3<int>;

////////////////////////////////////////////////////////////
/// \brief 3D bool vector (\p bvec3 in GLSL)
///
////////////////////////////////////////////////////////////
using Bvec3 = Vector3<bool>;

#ifdef SFML_DOXYGEN

////////////////////////////////////////////////////////////
/// \brief 4D float vector (\p vec4 in GLSL)
///
/// 4D float vectors can be implicitly converted from `sf::Color`
/// instances. Each color channel is normalized from integers
/// in [0, 255] to floating point values in [0, 1].
/// \code
/// sf::Glsl::Vec4 zeroVector;
/// sf::Glsl::Vec4 vector(1.f, 2.f, 3.f, 4.f);
/// sf::Glsl::Vec4 color = sf::Color::Cyan;
/// \endcode
////////////////////////////////////////////////////////////
using Vec4 = ImplementationDefined;

////////////////////////////////////////////////////////////
/// \brief 4D int vector (\p ivec4 in GLSL)
///
/// 4D int vectors can be implicitly converted from `sf::Color`
/// instances. Each color channel remains unchanged inside
/// the integer interval [0, 255].
/// \code
/// sf::Glsl::Ivec4 zeroVector;
/// sf::Glsl::Ivec4 vector(1, 2, 3, 4);
/// sf::Glsl::Ivec4 color = sf::Color::Cyan;
/// \endcode
////////////////////////////////////////////////////////////
using Ivec4 = ImplementationDefined;

////////////////////////////////////////////////////////////
/// \brief 4D bool vector (\p bvec4 in GLSL)
///
////////////////////////////////////////////////////////////
using Bvec4 = ImplementationDefined;

////////////////////////////////////////////////////////////
/// \brief 3x3 float matrix (\p mat3 in GLSL)
///
/// The matrix can be constructed from an array with 3x3
/// elements, aligned in column-major order. For example,
/// a translation by (x, y) looks as follows:
/// \code
/// float array[9] =
/// {
///     1, 0, 0,
///     0, 1, 0,
///     x, y, 1
/// };
///
/// sf::Glsl::Mat3 matrix(array);
/// \endcode
///
/// Mat3 can also be implicitly converted from `sf::Transform`:
/// \code
/// sf::Transform transform;
/// sf::Glsl::Mat3 matrix = transform;
/// \endcode
////////////////////////////////////////////////////////////
using Mat3 = ImplementationDefined;

////////////////////////////////////////////////////////////
/// \brief 4x4 float matrix (\p mat4 in GLSL)
///
/// The matrix can be constructed from an array with 4x4
/// elements, aligned in column-major order. For example,
/// a translation by (x, y, z) looks as follows:
/// \code
/// float array[16] =
/// {
///     1, 0, 0, 0,
///     0, 1, 0, 0,
///     0, 0, 1, 0,
///     x, y, z, 1
/// };
///
/// sf::Glsl::Mat4 matrix(array);
/// \endcode
///
/// Mat4 can also be implicitly converted from `sf::Transform`:
/// \code
/// sf::Transform transform;
/// sf::Glsl::Mat4 matrix = transform;
/// \endcode
////////////////////////////////////////////////////////////
using Mat4 = ImplementationDefined;

#else // SFML_DOXYGEN

using Vec4  = priv::Vector4<float>;
using Ivec4 = priv::Vector4<int>;
using Bvec4 = priv::Vector4<bool>;
using Mat3  = priv::Matrix<3, 3>;
using Mat4  = priv::Matrix<4, 4>;

#endif // SFML_DOXYGEN

} // namespace Glsl
} // namespace sf

#include <SFML/Graphics/Glsl.inl>


////////////////////////////////////////////////////////////
/// \namespace sf::Glsl
/// \ingroup graphics
///
/// \details The `sf::Glsl` namespace contains types that match
/// their equivalents in GLSL, the OpenGL shading language.
/// These types are exclusively used by the `sf::Shader` class.
///
/// Types that already exist in SFML, such as `sf::Vector2<T>`
/// and `sf::Vector3<T>`, are reused as type aliases, so you
/// can use the types in this namespace as well as the original ones.
/// Others are newly defined, such as `Glsl::Vec4` or `Glsl::Mat3`.
/// Their actual type is an implementation detail and should not be used.
///
/// All vector types support a default constructor that
/// initializes every component to zero, in addition to a
/// constructor with one parameter for each component.
/// The components are stored in member variables called
/// x, y, z, and w.
///
/// All matrix types support a constructor with a `float*`
/// parameter that points to a float array of the appropriate
/// size (that is, 9 in a 3x3 matrix, 16 in a 4x4 matrix).
/// Furthermore, they can be converted from `sf::Transform`
/// objects.
///
/// \see `sf::Shader`
///
////////////////////////////////////////////////////////////

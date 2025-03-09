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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Transform.hpp> // NOLINT(misc-header-include-cycle)

#include <array>

#include <cstddef>


namespace sf
{
////////////////////////////////////////////////////////////
// clang-format off
constexpr Transform::Transform(float a00, float a01, float a02,
                               float a10, float a11, float a12,
                               float a20, float a21, float a22)
    : m_matrix{a00, a10, 0.f, a20,
               a01, a11, 0.f, a21,
               0.f, 0.f, 1.f, 0.f,
               a02, a12, 0.f, a22}
{
}
// clang-format on


////////////////////////////////////////////////////////////
constexpr const float* Transform::getMatrix() const
{
    return m_matrix.data();
}


////////////////////////////////////////////////////////////
constexpr Transform Transform::getInverse() const
{
    // clang-format off
    // Compute the determinant
    const float det = m_matrix[0] * (m_matrix[15] * m_matrix[5] - m_matrix[7] * m_matrix[13]) -
                      m_matrix[1] * (m_matrix[15] * m_matrix[4] - m_matrix[7] * m_matrix[12]) +
                      m_matrix[3] * (m_matrix[13] * m_matrix[4] - m_matrix[5] * m_matrix[12]);
    // clang-format on

    // Compute the inverse if the determinant is not zero
    // (don't use an epsilon because the determinant may *really* be tiny)
    if (det != 0.f)
    {
        // clang-format off
        return {(m_matrix[15] * m_matrix[5] - m_matrix[7] * m_matrix[13]) / det,
               -(m_matrix[15] * m_matrix[4] - m_matrix[7] * m_matrix[12]) / det,
                (m_matrix[13] * m_matrix[4] - m_matrix[5] * m_matrix[12]) / det,
               -(m_matrix[15] * m_matrix[1] - m_matrix[3] * m_matrix[13]) / det,
                (m_matrix[15] * m_matrix[0] - m_matrix[3] * m_matrix[12]) / det,
               -(m_matrix[13] * m_matrix[0] - m_matrix[1] * m_matrix[12]) / det,
                (m_matrix[7]  * m_matrix[1] - m_matrix[3] * m_matrix[5])  / det,
               -(m_matrix[7]  * m_matrix[0] - m_matrix[3] * m_matrix[4])  / det,
                (m_matrix[5]  * m_matrix[0] - m_matrix[1] * m_matrix[4])  / det};
        // clang-format on
    }

    return Identity;
}


////////////////////////////////////////////////////////////
constexpr Vector2f Transform::transformPoint(Vector2f point) const
{
    return {m_matrix[0] * point.x + m_matrix[4] * point.y + m_matrix[12],
            m_matrix[1] * point.x + m_matrix[5] * point.y + m_matrix[13]};
}


////////////////////////////////////////////////////////////
constexpr FloatRect Transform::transformRect(const FloatRect& rectangle) const
{
    // Transform the 4 corners of the rectangle
    const std::array points = {transformPoint(rectangle.position),
                               transformPoint(rectangle.position + Vector2f(0.f, rectangle.size.y)),
                               transformPoint(rectangle.position + Vector2f(rectangle.size.x, 0.f)),
                               transformPoint(rectangle.position + rectangle.size)};

    // Compute the bounding rectangle of the transformed points
    Vector2f pmin = points[0];
    Vector2f pmax = points[0];

    for (std::size_t i = 1; i < points.size(); ++i)
    {
        // clang-format off
        if      (points[i].x < pmin.x) pmin.x = points[i].x;
        else if (points[i].x > pmax.x) pmax.x = points[i].x;

        if      (points[i].y < pmin.y) pmin.y = points[i].y;
        else if (points[i].y > pmax.y) pmax.y = points[i].y;
        // clang-format on
    }

    return {pmin, pmax - pmin};
}


////////////////////////////////////////////////////////////
constexpr Transform& Transform::combine(const Transform& transform)
{
    const auto& a = m_matrix;
    const auto& b = transform.m_matrix;

    // clang-format off
    *this = Transform(a[0] * b[0]  + a[4] * b[1]  + a[12] * b[3],
                      a[0] * b[4]  + a[4] * b[5]  + a[12] * b[7],
                      a[0] * b[12] + a[4] * b[13] + a[12] * b[15],
                      a[1] * b[0]  + a[5] * b[1]  + a[13] * b[3],
                      a[1] * b[4]  + a[5] * b[5]  + a[13] * b[7],
                      a[1] * b[12] + a[5] * b[13] + a[13] * b[15],
                      a[3] * b[0]  + a[7] * b[1]  + a[15] * b[3],
                      a[3] * b[4]  + a[7] * b[5]  + a[15] * b[7],
                      a[3] * b[12] + a[7] * b[13] + a[15] * b[15]);
    // clang-format on

    return *this;
}


////////////////////////////////////////////////////////////
constexpr Transform& Transform::translate(Vector2f offset)
{
    // clang-format off
    const Transform translation(1, 0, offset.x,
                                0, 1, offset.y,
                                0, 0, 1);
    // clang-format on

    return combine(translation);
}


////////////////////////////////////////////////////////////
constexpr Transform& Transform::scale(Vector2f factors)
{
    // clang-format off
    const Transform scaling(factors.x, 0,         0,
                            0,         factors.y, 0,
                            0,         0,         1);
    // clang-format on

    return combine(scaling);
}


////////////////////////////////////////////////////////////
constexpr Transform& Transform::scale(Vector2f factors, Vector2f center)
{
    // clang-format off
    const Transform scaling(factors.x, 0,         center.x * (1 - factors.x),
                            0,         factors.y, center.y * (1 - factors.y),
                            0,         0,         1);
    // clang-format on

    return combine(scaling);
}


////////////////////////////////////////////////////////////
constexpr Transform operator*(const Transform& left, const Transform& right)
{
    return Transform(left).combine(right);
}


////////////////////////////////////////////////////////////
constexpr Transform& operator*=(Transform& left, const Transform& right)
{
    return left.combine(right);
}


////////////////////////////////////////////////////////////
constexpr Vector2f operator*(const Transform& left, Vector2f right)
{
    return left.transformPoint(right);
}


////////////////////////////////////////////////////////////
constexpr bool operator==(const Transform& left, const Transform& right)
{
    const float* a = left.getMatrix();
    const float* b = right.getMatrix();

    // clang-format off
    return ((a[0]  == b[0])  && (a[1]  == b[1])  && (a[3]  == b[3]) &&
            (a[4]  == b[4])  && (a[5]  == b[5])  && (a[7]  == b[7]) &&
            (a[12] == b[12]) && (a[13] == b[13]) && (a[15] == b[15]));
    // clang-format on
}


////////////////////////////////////////////////////////////
constexpr bool operator!=(const Transform& left, const Transform& right)
{
    return !(left == right);
}


////////////////////////////////////////////////////////////
// Static member data
////////////////////////////////////////////////////////////

// Note: the 'inline' keyword here is technically not required, but VS2019 fails
// to compile with a bogus "multiple definition" error if not explicitly used.
inline constexpr Transform Transform::Identity;

} // namespace sf

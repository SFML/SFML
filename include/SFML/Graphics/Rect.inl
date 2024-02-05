////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/Rect.hpp> // NOLINT(misc-header-include-cycle)


namespace sf
{
////////////////////////////////////////////////////////////
template <typename T>
constexpr Rect<T>::Rect() = default;


////////////////////////////////////////////////////////////
template <typename T>
constexpr Rect<T>::Rect(const Vector2<T>& position, const Vector2<T>& size) :
left(position.x),
top(position.y),
width(size.x),
height(size.y)
{
}


////////////////////////////////////////////////////////////
template <typename T>
template <typename U>
constexpr Rect<T>::Rect(const Rect<U>& rectangle) :
left(static_cast<T>(rectangle.left)),
top(static_cast<T>(rectangle.top)),
width(static_cast<T>(rectangle.width)),
height(static_cast<T>(rectangle.height))
{
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr bool Rect<T>::contains(const Vector2<T>& point) const
{
    // Not using 'std::min' and 'std::max' to avoid depending on '<algorithm>'
    const auto min = [](T a, T b) { return (a < b) ? a : b; };
    const auto max = [](T a, T b) { return (a < b) ? b : a; };

    // Rectangles with negative dimensions are allowed, so we must handle them correctly

    // Compute the real min and max of the rectangle on both axes
    const T minX = min(left, static_cast<T>(left + width));
    const T maxX = max(left, static_cast<T>(left + width));
    const T minY = min(top, static_cast<T>(top + height));
    const T maxY = max(top, static_cast<T>(top + height));

    return (point.x >= minX) && (point.x < maxX) && (point.y >= minY) && (point.y < maxY);
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr std::optional<Rect<T>> Rect<T>::findIntersection(const Rect<T>& rectangle) const
{
    // Not using 'std::min' and 'std::max' to avoid depending on '<algorithm>'
    const auto min = [](T a, T b) { return (a < b) ? a : b; };
    const auto max = [](T a, T b) { return (a < b) ? b : a; };

    // Rectangles with negative dimensions are allowed, so we must handle them correctly

    // Compute the min and max of the first rectangle on both axes
    const T r1MinX = min(left, static_cast<T>(left + width));
    const T r1MaxX = max(left, static_cast<T>(left + width));
    const T r1MinY = min(top, static_cast<T>(top + height));
    const T r1MaxY = max(top, static_cast<T>(top + height));

    // Compute the min and max of the second rectangle on both axes
    const T r2MinX = min(rectangle.left, static_cast<T>(rectangle.left + rectangle.width));
    const T r2MaxX = max(rectangle.left, static_cast<T>(rectangle.left + rectangle.width));
    const T r2MinY = min(rectangle.top, static_cast<T>(rectangle.top + rectangle.height));
    const T r2MaxY = max(rectangle.top, static_cast<T>(rectangle.top + rectangle.height));

    // Compute the intersection boundaries
    const T interLeft   = max(r1MinX, r2MinX);
    const T interTop    = max(r1MinY, r2MinY);
    const T interRight  = min(r1MaxX, r2MaxX);
    const T interBottom = min(r1MaxY, r2MaxY);

    // If the intersection is valid (positive non zero area), then there is an intersection
    if ((interLeft < interRight) && (interTop < interBottom))
    {
        return Rect<T>({interLeft, interTop}, {interRight - interLeft, interBottom - interTop});
    }
    else
    {
        return std::nullopt;
    }
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T> Rect<T>::getPosition() const
{
    return Vector2<T>(left, top);
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T> Rect<T>::getSize() const
{
    return Vector2<T>(width, height);
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr Vector2<T> Rect<T>::getCenter() const
{
    return getPosition() + getSize() / T{2};
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr bool operator==(const Rect<T>& left, const Rect<T>& right)
{
    return (left.left == right.left) && (left.width == right.width) && (left.top == right.top) &&
           (left.height == right.height);
}


////////////////////////////////////////////////////////////
template <typename T>
constexpr bool operator!=(const Rect<T>& left, const Rect<T>& right)
{
    return !(left == right);
}

} // namespace sf

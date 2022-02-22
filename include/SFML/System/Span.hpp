////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_SYSTEM_SPAN_HPP
#define SFML_SYSTEM_SPAN_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <cassert>


namespace sf
{
////////////////////////////////////////////////////////////
/// \relates Span
/// \brief Constant to differentiate Span of static or dynamic extent
///
/// \see https://en.cppreference.com/w/cpp/container/span/dynamic_extent
///
////////////////////////////////////////////////////////////
inline constexpr std::size_t DynamicExtent = SIZE_MAX;

////////////////////////////////////////////////////////////
/// \brief Utility class to refer to a contiguous sequence of objects
///
////////////////////////////////////////////////////////////
template <typename ElementType, std::size_t Extent = DynamicExtent>
class Span;

////////////////////////////////////////////////////////////
/// \relates Span
/// \brief Convert a span into a read-only view of its underlying bytes
///
/// \see asWritableBytes
/// \see https://en.cppreference.com/w/cpp/container/span/as_bytes
///
////////////////////////////////////////////////////////////
template <typename ElementType, std::size_t Extent>
Span<const std::byte, ((Extent == DynamicExtent) ? DynamicExtent
                                                 : sizeof(ElementType) * Extent)>
asBytes(Span<ElementType, Extent> s) noexcept;

////////////////////////////////////////////////////////////
/// \relates Span
/// \brief Convert a span into a writable view of its underlying bytes
///
/// \see asBytes
/// \see https://en.cppreference.com/w/cpp/container/span/as_bytes
///
////////////////////////////////////////////////////////////
template <
    class ElementType, std::size_t Extent,
    typename std::enable_if<!std::is_const<ElementType>::value, int>::type = 0>
Span<std::byte, ((Extent == DynamicExtent) ? DynamicExtent
                                           : sizeof(ElementType) * Extent)>
asWritableBytes(Span<ElementType, Extent> s) noexcept;

#include <SFML/System/Span.inl>

} // namespace sf


#endif // SFML_SYSTEM_SPAN_HPP


////////////////////////////////////////////////////////////
/// \class sf::Span
/// \ingroup system
///
/// A span is a non-owning view over a contiguous sequence of objects.
/// It can have a \em static extent, defined at compile-time, or a \em dynamic extent.
///
/// \c std::span is part of C++20 standard library, but SFML supports C++17,
/// so this alternative implementation is used instead.
/// It is based on [Tristan Brindle's span library](https://github.com/tcbrindle/span).
///
/// \see https://en.cppreference.com/w/cpp/container/span
/// \see https://github.com/tcbrindle/span
///
////////////////////////////////////////////////////////////

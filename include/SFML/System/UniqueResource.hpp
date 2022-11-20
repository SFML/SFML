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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <optional>
#include <type_traits>
#include <utility>

#include <cassert>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Class template for storing non-copyable resource handles
///
////////////////////////////////////////////////////////////
template <typename Handle, typename Deleter>
class UniqueResource
{
    static_assert(!std::is_pointer_v<Handle>, "Handle cannot be a pointer");
    static_assert(!std::is_const_v<Handle>, "Handle cannot be const");
    static_assert(std::is_trivially_constructible_v<Handle>, "Handle must be trivially constructible");
    static_assert(std::is_trivially_destructible_v<Handle>, "Handle must be trivially destructible");
    static_assert(std::is_invocable_v<Deleter, Handle>, "Deleter must define operator()(Handle)");

public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    UniqueResource();

    ////////////////////////////////////////////////////////////
    /// \brief Construct from handle
    ///
    /// \param handle Handle to the resource to store
    ///
    ////////////////////////////////////////////////////////////
    UniqueResource(Handle handle);

    ////////////////////////////////////////////////////////////
    /// \brief Delete the resource
    ///
    ////////////////////////////////////////////////////////////
    ~UniqueResource();

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy constructor
    ///
    ////////////////////////////////////////////////////////////
    UniqueResource(const UniqueResource&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy assignment
    ///
    ////////////////////////////////////////////////////////////
    UniqueResource& operator=(const UniqueResource&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Move constructor
    ///
    ////////////////////////////////////////////////////////////
    UniqueResource(UniqueResource&& other) noexcept;

    ////////////////////////////////////////////////////////////
    /// \brief Move assignment
    ///
    ////////////////////////////////////////////////////////////
    UniqueResource& operator=(UniqueResource&& other) noexcept;

    ////////////////////////////////////////////////////////////
    /// \brief Release ownership of the managed resource
    ///
    ////////////////////////////////////////////////////////////
    void release();

    ////////////////////////////////////////////////////////////
    /// \brief Dispose of the resource by calling the deleter
    ///
    ////////////////////////////////////////////////////////////
    void reset();

    ////////////////////////////////////////////////////////////
    /// \brief Dispose of the resource by calling the deleter
    /// then assign to the resource
    ///
    ////////////////////////////////////////////////////////////
    void reset(Handle handle);

    ////////////////////////////////////////////////////////////
    /// \brief Access the underlying handle
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Handle get() const;

private:
    std::optional<Handle> m_handle;    //!< Handle to the managed resource
    Deleter               m_deleter{}; //!< Functor to clean up resource
};

#include <SFML/System/UniqueResource.inl>

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::UniqueResource
/// \ingroup system
///
/// This class provides a wrapper around a handle to a resource
/// with a custom deleter that cleans up that resource at the
/// end of its lifetime.
///
/// Usage example:
/// \code
/// struct Deleter
/// {
///     void operator()(int) const
///     {
///         std::cout << "Cleanup\n";
///     }
/// };
///
/// sf::UniqueResource<int, Deleter> uniqueResource(42);
/// useResource(uniqueResource.get());
/// ...
/// // When destructing, prints "Cleanup"
/// \endcode
///
/// This type is not copyeable but is moveable. If you move
/// from this type, the deleter will not be called on the
/// moved-from instance. This ensures that the resource it
/// owns will never be deleted twice.
///
////////////////////////////////////////////////////////////

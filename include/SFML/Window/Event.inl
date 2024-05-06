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

// Repeating isEventType<T> allows for cleaner compiler errors.
// It is not strictly necessary but it's useful nonetheless.
// It works by ensuring that the code within the conditional is
// only compiled when the condition is met. Otherwise you get
// a static_assert failure in addition to the compiler failing
// to compile the code within the compiletime conditional when
// an incorrect template parameter is provided.

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Event.hpp> // NOLINT(misc-header-include-cycle)


namespace sf
{
////////////////////////////////////////////////////////////
template <typename T>
Event::Event(const T& t)
{
    static_assert(isEventType<T>, "T must be a subtype of sf::Event");
    if constexpr (isEventType<T>)
        m_data = t;
}


////////////////////////////////////////////////////////////
template <typename T>
bool Event::is() const
{
    static_assert(isEventType<T>, "T must be a subtype of sf::Event");
    if constexpr (isEventType<T>)
        return std::holds_alternative<T>(m_data);
}


////////////////////////////////////////////////////////////
template <typename T>
const T* Event::getIf() const
{
    static_assert(isEventType<T>, "T must be a subtype of sf::Event");
    if constexpr (isEventType<T>)
        return std::get_if<T>(&m_data);
}

} // namespace sf

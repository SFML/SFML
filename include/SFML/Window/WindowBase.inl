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
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowBase.hpp> // NOLINT(misc-header-include-cycle)

#include <utility>


namespace sf
{
namespace priv
{
struct DelayOverloadResolution
{
    template <typename T>
    DelayOverloadResolution(const T&)
    {
    }
};
} // namespace priv

////////////////////////////////////////////////////////////
template <typename... Handlers>
void WindowBase::handleEvents(Handlers&&... handlers)
{
    static_assert(sizeof...(Handlers) > 0, "Must provide at least one handler");

    while (std::optional event = pollEvent())
        event->visit(std::forward<Handlers>(handlers)..., [](priv::DelayOverloadResolution) { /* ignore */ });
}

} // namespace sf

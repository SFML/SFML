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
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowBase.hpp> // NOLINT(misc-header-include-cycle)

#include <utility>


namespace sf
{
namespace priv
{
template <typename... Ts>
struct OverloadSet : Ts...
{
    using Ts::operator()...;
#if defined(_MSC_VER) && !defined(__clang__)
    unsigned char dummy; // Dummy variable to ensure that this struct is not empty thus avoiding a crash due to an MSVC bug
#endif
};
template <typename... Ts>
OverloadSet(Ts...) -> OverloadSet<Ts...>;

struct DelayOverloadResolution
{
    template <typename T>
    DelayOverloadResolution(const T&)
    {
    }
};
} // namespace priv


////////////////////////////////////////////////////////////
template <typename... Ts>
void WindowBase::handleEvents(Ts&&... handlers) // NOLINT(cppcoreguidelines-missing-std-forward)
{
    static_assert(sizeof...(Ts) > 0, "Must provide at least one handler");

    // Disable misc-const-correctness for this line since clang-tidy
    // complains about it even though the code would become uncompilable

    // NOLINTNEXTLINE(misc-const-correctness)
    priv::OverloadSet overloadSet{std::forward<Ts>(handlers)..., [](const priv::DelayOverloadResolution&) { /* ignore */ }};

    while (const std::optional event = pollEvent())
        event->visit(overloadSet);
}

} // namespace sf

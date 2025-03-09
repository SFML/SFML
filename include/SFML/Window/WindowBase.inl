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

#include <type_traits>
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

// Wrapper providing a callable type suitable as OverloadSet base type for different kinds of handlers.
// By default, we derive from the handler type and inherit its call operators.
template <typename Handler>
struct Caller : Handler
{
    using Handler::operator();
};

// Inheritance is not possible with reference types.
// In this case, we capture the reference and forward arguments to it.
template <typename Handler>
struct Caller<Handler&>
{
    // Use SFINAE so that the call operator exists only for arguments the captured handler accepts
    template <typename Argument, std::enable_if_t<std::is_invocable_v<Handler&, Argument>, int> = 0>
    decltype(auto) operator()(Argument&& argument)
    {
        return handler(std::forward<Argument>(argument));
    }
    Handler& handler;
};

// Inheritance is not possible with function pointers either.
// In this case, we capture the function pointer to call it.
template <typename Return, typename Argument>
struct Caller<Return (*)(Argument)>
{
    Return operator()(Argument&& argument)
    {
        return function(std::forward<Argument>(argument));
    }
    Return (*function)(Argument);
};
} // namespace priv


////////////////////////////////////////////////////////////
template <typename... Handlers>
void WindowBase::handleEvents(Handlers&&... handlers)
{
    static_assert(sizeof...(Handlers) > 0, "Must provide at least one handler");
    static_assert((Event::isEventHandler<Handlers> && ...), "Handlers must accept at least one subtype of sf::Event");

    // Disable misc-const-correctness for this line since clang-tidy
    // complains about it even though the code would become incorrect

    // NOLINTNEXTLINE(misc-const-correctness)
    priv::OverloadSet overloadSet{priv::Caller<Handlers>{std::forward<Handlers>(handlers)}...,
                                  [](const priv::DelayOverloadResolution&) { /* ignore */ }};

    while (const std::optional event = pollEvent())
        event->visit(overloadSet);
}

} // namespace sf

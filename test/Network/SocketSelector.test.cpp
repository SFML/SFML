#include <SFML/Network/SocketSelector.hpp>

#include <type_traits>

static_assert(std::is_copy_constructible_v<sf::SocketSelector>);
static_assert(std::is_copy_assignable_v<sf::SocketSelector>);
static_assert(std::is_move_constructible_v<sf::SocketSelector>);
static_assert(!std::is_nothrow_move_constructible_v<sf::SocketSelector>);
static_assert(std::is_move_assignable_v<sf::SocketSelector>);
static_assert(!std::is_nothrow_move_assignable_v<sf::SocketSelector>);

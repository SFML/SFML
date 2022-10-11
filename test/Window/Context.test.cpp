#include <SFML/Window/Context.hpp>

#include <type_traits>

static_assert(!std::is_copy_constructible_v<sf::Context>);
static_assert(!std::is_copy_assignable_v<sf::Context>);
static_assert(!std::is_nothrow_move_constructible_v<sf::Context>);
static_assert(!std::is_nothrow_move_assignable_v<sf::Context>);

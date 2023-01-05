#include <SFML/Graphics/Text.hpp>

#include <type_traits>

static_assert(!std::is_constructible_v<sf::Text, sf::String, sf::Font&&, unsigned int>);
static_assert(std::is_copy_constructible_v<sf::Text>);
static_assert(std::is_copy_assignable_v<sf::Text>);
static_assert(std::is_nothrow_move_constructible_v<sf::Text>);
static_assert(std::is_nothrow_move_assignable_v<sf::Text>);

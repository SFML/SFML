#include <SFML/Audio/Sound.hpp>

#include <type_traits>

static_assert(!std::is_constructible_v<sf::Sound, sf::SoundBuffer&&>);
static_assert(std::is_copy_constructible_v<sf::Sound>);
static_assert(std::is_copy_assignable_v<sf::Sound>);
static_assert(std::is_move_constructible_v<sf::Sound>);
static_assert(!std::is_nothrow_move_constructible_v<sf::Sound>);
static_assert(std::is_move_assignable_v<sf::Sound>);
static_assert(!std::is_nothrow_move_assignable_v<sf::Sound>);

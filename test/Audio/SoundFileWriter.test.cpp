#include <SFML/Audio/SoundFileWriter.hpp>

#include <type_traits>

static_assert(!std::is_constructible_v<sf::SoundFileWriter>);
static_assert(!std::is_copy_constructible_v<sf::SoundFileWriter>);
static_assert(std::is_copy_assignable_v<sf::SoundFileWriter>);
static_assert(!std::is_nothrow_move_constructible_v<sf::SoundFileWriter>);
static_assert(std::is_nothrow_move_assignable_v<sf::SoundFileWriter>);
static_assert(std::has_virtual_destructor_v<sf::SoundFileWriter>);

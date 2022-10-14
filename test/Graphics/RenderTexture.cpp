#include <SFML/Graphics/RenderTexture.hpp>

#include <type_traits>

static_assert(!std::is_copy_constructible_v<sf::RenderTexture>);
static_assert(!std::is_copy_assignable_v<sf::RenderTexture>);
static_assert(!std::is_nothrow_move_constructible_v<sf::RenderTexture>);
static_assert(!std::is_nothrow_move_assignable_v<sf::RenderTexture>);

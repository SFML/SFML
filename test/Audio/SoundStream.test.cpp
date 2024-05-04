#include <SFML/Audio/SoundStream.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

TEST_CASE("[Audio] sf::SoundStream")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_constructible_v<sf::SoundStream>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::SoundStream>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::SoundStream>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<sf::SoundStream>);
        STATIC_CHECK(!std::is_nothrow_move_assignable_v<sf::SoundStream>);
    }

    SECTION("Chunk")
    {
        const sf::SoundStream::Chunk chunk;
        CHECK(chunk.samples == nullptr);
        CHECK(chunk.sampleCount == 0);
    }
}

#include <SFML/Audio/SoundFileReader.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("[Audio] sf::SoundFileReader")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_constructible_v<sf::SoundFileReader>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::SoundFileReader>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::SoundFileReader>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<sf::SoundFileReader>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::SoundFileReader>);
        STATIC_CHECK(std::has_virtual_destructor_v<sf::SoundFileReader>);
    }

    SECTION("Info")
    {
        const sf::SoundFileReader::Info info;
        CHECK(info.sampleCount == 0);
        CHECK(info.channelCount == 0);
        CHECK(info.sampleRate == 0);
    }
}

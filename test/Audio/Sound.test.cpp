#include <SFML/Audio/Sound.hpp>

// Other 1st party headers
#include <SFML/Audio/SoundBuffer.hpp>

#include <SFML/System/Time.hpp>

#include <catch2/catch_test_macros.hpp>

#include <AudioUtil.hpp>
#include <SystemUtil.hpp>
#include <type_traits>

TEST_CASE("[Audio] sf::Sound", runAudioDeviceTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_constructible_v<sf::Sound, sf::SoundBuffer&&>);
        STATIC_CHECK(std::is_copy_constructible_v<sf::Sound>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Sound>);
        STATIC_CHECK(std::is_move_constructible_v<sf::Sound>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<sf::Sound>);
        STATIC_CHECK(std::is_move_assignable_v<sf::Sound>);
        STATIC_CHECK(!std::is_nothrow_move_assignable_v<sf::Sound>);
        STATIC_CHECK(std::has_virtual_destructor_v<sf::Sound>);
    }

    const auto soundBuffer = sf::SoundBuffer::loadFromFile("Audio/ding.flac").value();

    SECTION("Construction")
    {
        const sf::Sound sound(soundBuffer);
        CHECK(&sound.getBuffer() == &soundBuffer);
        CHECK(!sound.getLoop());
        CHECK(sound.getPlayingOffset() == sf::Time::Zero);
        CHECK(sound.getStatus() == sf::Sound::Status::Stopped);
    }

    SECTION("Copy semantics")
    {
        const sf::Sound sound(soundBuffer);

        SECTION("Construction")
        {
            const sf::Sound soundCopy(sound); // NOLINT(performance-unnecessary-copy-initialization)
            CHECK(&soundCopy.getBuffer() == &soundBuffer);
            CHECK(!soundCopy.getLoop());
            CHECK(soundCopy.getPlayingOffset() == sf::Time::Zero);
            CHECK(soundCopy.getStatus() == sf::Sound::Status::Stopped);
        }

        SECTION("Assignment")
        {
            const sf::SoundBuffer otherSoundBuffer = sf::SoundBuffer::loadFromFile("Audio/ding.flac").value();
            sf::Sound             soundCopy(otherSoundBuffer);
            soundCopy = sound;
            CHECK(&soundCopy.getBuffer() == &soundBuffer);
            CHECK(!soundCopy.getLoop());
            CHECK(soundCopy.getPlayingOffset() == sf::Time::Zero);
            CHECK(soundCopy.getStatus() == sf::Sound::Status::Stopped);
        }
    }

    SECTION("Set/get buffer")
    {
        const sf::SoundBuffer otherSoundBuffer = sf::SoundBuffer::loadFromFile("Audio/ding.flac").value();
        sf::Sound             sound(soundBuffer);
        sound.setBuffer(otherSoundBuffer);
        CHECK(&sound.getBuffer() == &otherSoundBuffer);
    }

    SECTION("Set/get loop")
    {
        sf::Sound sound(soundBuffer);
        sound.setLoop(true);
        CHECK(sound.getLoop());
    }

    SECTION("Set/get playing offset")
    {
        sf::Sound sound(soundBuffer);
        sound.setPlayingOffset(sf::seconds(10));
        CHECK(sound.getPlayingOffset() == sf::seconds(10));
    }
}

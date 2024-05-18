#include <SFML/Audio/SoundBuffer.hpp>

// Other 1st party headers
#include <SFML/System/FileInputStream.hpp>

#include <catch2/catch_test_macros.hpp>

#include <AudioUtil.hpp>
#include <SystemUtil.hpp>
#include <array>
#include <type_traits>

TEST_CASE("[Audio] sf::SoundBuffer", runAudioDeviceTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_default_constructible_v<sf::SoundBuffer>);
        STATIC_CHECK(std::is_copy_constructible_v<sf::SoundBuffer>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::SoundBuffer>);
        STATIC_CHECK(std::is_move_constructible_v<sf::SoundBuffer>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<sf::SoundBuffer>);
        STATIC_CHECK(std::is_move_assignable_v<sf::SoundBuffer>);
        STATIC_CHECK(!std::is_nothrow_move_assignable_v<sf::SoundBuffer>);
    }

    SECTION("Copy semantics")
    {
        const auto soundBuffer = sf::SoundBuffer::loadFromFile("Audio/ding.flac").value();

        SECTION("Construction")
        {
            const sf::SoundBuffer soundBufferCopy(soundBuffer); // NOLINT(performance-unnecessary-copy-initialization)
            CHECK(soundBufferCopy.getSamples() != nullptr);
            CHECK(soundBufferCopy.getSampleCount() == 87798);
            CHECK(soundBufferCopy.getSampleRate() == 44100);
            CHECK(soundBufferCopy.getChannelCount() == 1);
            CHECK(soundBufferCopy.getDuration() == sf::microseconds(1990884));
        }

        SECTION("Assignment")
        {
            sf::SoundBuffer soundBufferCopy = sf::SoundBuffer::loadFromFile("Audio/doodle_pop.ogg").value();
            soundBufferCopy                 = soundBuffer;
            CHECK(soundBufferCopy.getSamples() != nullptr);
            CHECK(soundBufferCopy.getSampleCount() == 87798);
            CHECK(soundBufferCopy.getSampleRate() == 44100);
            CHECK(soundBufferCopy.getChannelCount() == 1);
            CHECK(soundBufferCopy.getDuration() == sf::microseconds(1990884));
        }
    }

    SECTION("loadFromFile()")
    {
        SECTION("Invalid filename")
        {
            CHECK(!sf::SoundBuffer::loadFromFile("does/not/exist.wav"));
        }

        SECTION("Valid file")
        {
            const auto soundBuffer = sf::SoundBuffer::loadFromFile("Audio/ding.flac").value();
            CHECK(soundBuffer.getSamples() != nullptr);
            CHECK(soundBuffer.getSampleCount() == 87798);
            CHECK(soundBuffer.getSampleRate() == 44100);
            CHECK(soundBuffer.getChannelCount() == 1);
            CHECK(soundBuffer.getDuration() == sf::microseconds(1990884));
        }
    }

    SECTION("loadFromMemory()")
    {
        SECTION("Invalid memory")
        {
            CHECK(!sf::SoundBuffer::loadFromMemory(nullptr, 0));
            constexpr std::array<std::byte, 5> memory{};
            CHECK(!sf::SoundBuffer::loadFromMemory(memory.data(), memory.size()));
        }

        SECTION("Valid memory")
        {
            const auto memory      = loadIntoMemory("Audio/ding.flac");
            const auto soundBuffer = sf::SoundBuffer::loadFromMemory(memory.data(), memory.size()).value();
            CHECK(soundBuffer.getSamples() != nullptr);
            CHECK(soundBuffer.getSampleCount() == 87798);
            CHECK(soundBuffer.getSampleRate() == 44100);
            CHECK(soundBuffer.getChannelCount() == 1);
            CHECK(soundBuffer.getDuration() == sf::microseconds(1990884));
        }
    }

    SECTION("loadFromStream()")
    {
        sf::FileInputStream stream;

        SECTION("Invalid stream")
        {
            CHECK(!sf::SoundBuffer::loadFromStream(stream));
        }

        SECTION("Valid stream")
        {
            REQUIRE(stream.open("Audio/ding.flac"));
            const auto soundBuffer = sf::SoundBuffer::loadFromStream(stream).value();
            CHECK(soundBuffer.getSamples() != nullptr);
            CHECK(soundBuffer.getSampleCount() == 87798);
            CHECK(soundBuffer.getSampleRate() == 44100);
            CHECK(soundBuffer.getChannelCount() == 1);
            CHECK(soundBuffer.getDuration() == sf::microseconds(1990884));
        }
    }

    SECTION("saveToFile()")
    {
        const auto filename = std::filesystem::temp_directory_path() / "ding.flac";

        {
            const auto soundBuffer = sf::SoundBuffer::loadFromFile("Audio/ding.flac").value();
            REQUIRE(soundBuffer.saveToFile(filename));
        }

        const auto soundBuffer = sf::SoundBuffer::loadFromFile(filename).value();
        CHECK(soundBuffer.getSamples() != nullptr);
        CHECK(soundBuffer.getSampleCount() == 87798);
        CHECK(soundBuffer.getSampleRate() == 44100);
        CHECK(soundBuffer.getChannelCount() == 1);
        CHECK(soundBuffer.getDuration() == sf::microseconds(1990884));

        CHECK(std::filesystem::remove(filename));
    }
}

#include <SFML/Audio/SoundStream.hpp>

#include <catch2/catch_test_macros.hpp>

#include <AudioUtil.hpp>
#include <SystemUtil.hpp>
#include <type_traits>

namespace
{
class SoundStream : public sf::SoundStream
{
    [[nodiscard]] bool onGetData(Chunk& /* data */) override
    {
        return true;
    }

    void onSeek(sf::Time /* timeOffset */) override
    {
    }
};
} // namespace

TEST_CASE("[Audio] sf::SoundStream", runAudioDeviceTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_constructible_v<sf::SoundStream>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::SoundStream>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::SoundStream>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<sf::SoundStream>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::SoundStream>);
        STATIC_CHECK(std::has_virtual_destructor_v<sf::SoundStream>);
    }

    SECTION("Chunk")
    {
        const sf::SoundStream::Chunk chunk;
        CHECK(chunk.samples == nullptr);
        CHECK(chunk.sampleCount == 0);
    }

    SECTION("Construction")
    {
        const SoundStream soundStream;
        CHECK(soundStream.getChannelCount() == 0);
        CHECK(soundStream.getSampleRate() == 0);
        CHECK(soundStream.getStatus() == sf::SoundStream::Status::Stopped);
        CHECK(soundStream.getPlayingOffset() == sf::Time::Zero);
        CHECK(!soundStream.isLooping());
    }

    SECTION("Set/get playing offset")
    {
        SoundStream soundStream;
        soundStream.setPlayingOffset(sf::milliseconds(100));
        CHECK(soundStream.getPlayingOffset() == sf::milliseconds(0));
    }

    SECTION("Set/get loop")
    {
        SoundStream soundStream;
        soundStream.setLooping(true);
        CHECK(soundStream.isLooping());
    }
}

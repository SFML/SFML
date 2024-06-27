#include <SFML/Audio/SoundSource.hpp>

#include <catch2/catch_test_macros.hpp>

#include <AudioUtil.hpp>
#include <SystemUtil.hpp>
#include <type_traits>

namespace
{
class SoundSource : public sf::SoundSource
{
    void play() override
    {
    }

    void pause() override
    {
    }

    void stop() override
    {
    }

    [[nodiscard]] void* getSound() const override
    {
        return {};
    }

public:
    [[nodiscard]] Status getStatus() const override
    {
        return {};
    }
};
} // namespace

TEST_CASE("[Audio] sf::SoundSource", runAudioDeviceTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_constructible_v<sf::SoundSource>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::SoundSource>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::SoundSource>);
        STATIC_CHECK(!std::is_move_constructible_v<sf::SoundSource>);
        STATIC_CHECK(std::is_move_assignable_v<sf::SoundSource>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::SoundSource>);
        STATIC_CHECK(std::has_virtual_destructor_v<sf::SoundSource>);
    }

    SECTION("Construction")
    {
        const SoundSource soundSource;
        CHECK(soundSource.getPitch() == 0);
        CHECK(soundSource.getVolume() == 0);
        CHECK(soundSource.getPosition() == sf::Vector3f());
        CHECK(!soundSource.isRelativeToListener());
        CHECK(soundSource.getMinDistance() == 0);
        CHECK(soundSource.getAttenuation() == 0);
        CHECK(soundSource.getStatus() == sf::SoundSource::Status::Stopped);
    }

    SECTION("Copy semantics")
    {
        const SoundSource soundSource;

        SECTION("Construction")
        {
            const SoundSource soundSourceCopy(soundSource); // NOLINT(performance-unnecessary-copy-initialization)
            CHECK(soundSourceCopy.getPitch() == 0);
            CHECK(soundSourceCopy.getVolume() == 0);
            CHECK(soundSourceCopy.getPosition() == sf::Vector3f());
            CHECK(!soundSourceCopy.isRelativeToListener());
            CHECK(soundSourceCopy.getMinDistance() == 0);
            CHECK(soundSourceCopy.getAttenuation() == 0);
            CHECK(soundSourceCopy.getStatus() == sf::SoundSource::Status::Stopped);
        }

        SECTION("Assignment")
        {
            SoundSource soundSourceCopy;
            soundSourceCopy = soundSource;
            CHECK(soundSourceCopy.getPitch() == 0);
            CHECK(soundSourceCopy.getVolume() == 0);
            CHECK(soundSourceCopy.getPosition() == sf::Vector3f());
            CHECK(!soundSourceCopy.isRelativeToListener());
            CHECK(soundSourceCopy.getMinDistance() == 0);
            CHECK(soundSourceCopy.getAttenuation() == 0);
            CHECK(soundSourceCopy.getStatus() == sf::SoundSource::Status::Stopped);
        }
    }

    SECTION("Set/get pitch")
    {
        SoundSource soundSource;
        soundSource.setPitch(42);
        CHECK(soundSource.getPitch() == 0);
    }

    SECTION("Set/get volume")
    {
        SoundSource soundSource;
        soundSource.setVolume(0.5f);
        CHECK(soundSource.getVolume() == 0);
    }

    SECTION("Set/get position")
    {
        SoundSource soundSource;
        soundSource.setPosition({1, 2, 3});
        CHECK(soundSource.getPosition() == sf::Vector3f());
    }

    SECTION("Set/get relative to listener")
    {
        SoundSource soundSource;
        soundSource.setRelativeToListener(true);
        CHECK(!soundSource.isRelativeToListener());
    }

    SECTION("Set/get min distance")
    {
        SoundSource soundSource;
        soundSource.setMinDistance(12.34f);
        CHECK(soundSource.getMinDistance() == 0);
    }

    SECTION("Set/get attenuation")
    {
        SoundSource soundSource;
        soundSource.setAttenuation(10);
        CHECK(soundSource.getAttenuation() == 0);
    }
}

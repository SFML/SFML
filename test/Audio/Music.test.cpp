#include <SFML/Audio/Music.hpp>

// Other 1st party headers
#include <SFML/System/FileInputStream.hpp>

#include <catch2/catch_test_macros.hpp>

#include <AudioUtil.hpp>
#include <SystemUtil.hpp>
#include <array>
#include <fstream>
#include <thread>
#include <type_traits>

TEST_CASE("[Audio] sf::Music", runAudioDeviceTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_copy_constructible_v<sf::Music>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::Music>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<sf::Music>);
        STATIC_CHECK(!std::is_nothrow_move_assignable_v<sf::Music>);
        STATIC_CHECK(std::has_virtual_destructor_v<sf::Music>);
    }

    SECTION("Span")
    {
        const sf::Music::Span<float> span;
        CHECK(span.offset == 0);
        CHECK(span.length == 0);

        const sf::Music::TimeSpan timeSpan;
        CHECK(timeSpan.offset == sf::Time::Zero);
        CHECK(timeSpan.length == sf::Time::Zero);
    }

    SECTION("Construction")
    {
        const sf::Music music;
        const auto [offset, length] = music.getLoopPoints();
        CHECK(offset == sf::Time::Zero);
        CHECK(length == sf::Time::Zero);
        CHECK(music.getChannelCount() == 0);
        CHECK(music.getSampleRate() == 0);
        CHECK(music.getStatus() == sf::Music::Status::Stopped);
        CHECK(music.getPlayingOffset() == sf::Time::Zero);
        CHECK(!music.getLoop());
    }

    SECTION("openFromFile()")
    {
        sf::Music music;

        SECTION("Invalid file")
        {
            REQUIRE(!music.openFromFile("does/not/exist.wav"));
            const auto [offset, length] = music.getLoopPoints();
            CHECK(offset == sf::Time::Zero);
            CHECK(length == sf::Time::Zero);
            CHECK(music.getChannelCount() == 0);
            CHECK(music.getSampleRate() == 0);
            CHECK(music.getStatus() == sf::Music::Status::Stopped);
            CHECK(music.getPlayingOffset() == sf::Time::Zero);
            CHECK(!music.getLoop());
        }

        SECTION("Valid file")
        {
            REQUIRE(music.openFromFile("Audio/ding.mp3"));
            CHECK(music.getDuration() == sf::microseconds(1990884));
            const auto [offset, length] = music.getLoopPoints();
            CHECK(offset == sf::Time::Zero);
            CHECK(length == sf::microseconds(1990884));
            CHECK(music.getChannelCount() == 1);
            CHECK(music.getSampleRate() == 44100);
            CHECK(music.getStatus() == sf::Music::Status::Stopped);
            CHECK(music.getPlayingOffset() == sf::Time::Zero);
            CHECK(!music.getLoop());
        }
    }

    SECTION("openFromMemory()")
    {
        std::vector<std::byte> memory;
        sf::Music              music;

        SECTION("Invalid buffer")
        {
            REQUIRE(!music.openFromMemory(memory.data(), memory.size()));
            const auto [offset, length] = music.getLoopPoints();
            CHECK(offset == sf::Time::Zero);
            CHECK(length == sf::Time::Zero);
            CHECK(music.getChannelCount() == 0);
            CHECK(music.getSampleRate() == 0);
            CHECK(music.getStatus() == sf::Music::Status::Stopped);
            CHECK(music.getPlayingOffset() == sf::Time::Zero);
            CHECK(!music.getLoop());
        }

        SECTION("Valid buffer")
        {
            memory = loadIntoMemory("Audio/ding.flac");
            REQUIRE(music.openFromMemory(memory.data(), memory.size()));
            CHECK(music.getDuration() == sf::microseconds(1990884));
            const auto [offset, length] = music.getLoopPoints();
            CHECK(offset == sf::Time::Zero);
            CHECK(length == sf::microseconds(1990884));
            CHECK(music.getChannelCount() == 1);
            CHECK(music.getSampleRate() == 44100);
            CHECK(music.getStatus() == sf::Music::Status::Stopped);
            CHECK(music.getPlayingOffset() == sf::Time::Zero);
            CHECK(!music.getLoop());
        }
    }

    SECTION("openFromStream()")
    {
        sf::FileInputStream stream;
        sf::Music           music;

        SECTION("Invalid stream")
        {
            CHECK(!music.openFromStream(stream));
            const auto [offset, length] = music.getLoopPoints();
            CHECK(offset == sf::Time::Zero);
            CHECK(length == sf::Time::Zero);
            CHECK(music.getChannelCount() == 0);
            CHECK(music.getSampleRate() == 0);
            CHECK(music.getStatus() == sf::Music::Status::Stopped);
            CHECK(music.getPlayingOffset() == sf::Time::Zero);
            CHECK(!music.getLoop());
        }

        SECTION("Valid stream")
        {
            REQUIRE(stream.open("Audio/doodle_pop.ogg"));
            REQUIRE(music.openFromStream(stream));
            CHECK(music.getDuration() == sf::microseconds(24002176));
            const auto [offset, length] = music.getLoopPoints();
            CHECK(offset == sf::Time::Zero);
            CHECK(length == sf::microseconds(24002176));
            CHECK(music.getChannelCount() == 2);
            CHECK(music.getSampleRate() == 44100);
            CHECK(music.getStatus() == sf::Music::Status::Stopped);
            CHECK(music.getPlayingOffset() == sf::Time::Zero);
            CHECK(!music.getLoop());
        }
    }

    SECTION("play/pause/stop")
    {
        sf::Music music;
        REQUIRE(music.openFromFile("Audio/ding.mp3"));

        // Wait for background thread to start
        music.play();
        while (music.getStatus() == sf::Music::Status::Stopped)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        CHECK(music.getStatus() == sf::Music::Status::Playing);

        // Wait for background thread to pause
        music.pause();
        while (music.getStatus() == sf::Music::Status::Playing)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        CHECK(music.getStatus() == sf::Music::Status::Paused);

        // Wait for background thread to stop
        music.stop();
        while (music.getStatus() == sf::Music::Status::Paused)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        CHECK(music.getStatus() == sf::Music::Status::Stopped);
    }

    SECTION("setLoopPoints()")
    {
        sf::Music music;
        REQUIRE(music.openFromFile("Audio/killdeer.wav"));
        music.setLoopPoints({sf::seconds(1), sf::seconds(2)});
        const auto [offset, length] = music.getLoopPoints();
        CHECK(offset == sf::seconds(1));
        CHECK(length == sf::seconds(2));
        CHECK(music.getChannelCount() == 1);
        CHECK(music.getSampleRate() == 22050);
        CHECK(music.getStatus() == sf::Music::Status::Stopped);
        CHECK(music.getPlayingOffset() == sf::Time::Zero);
        CHECK(!music.getLoop());
    }
}

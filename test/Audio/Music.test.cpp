#include <SFML/Audio/Music.hpp>

// Other 1st party headers
#include <SFML/System/FileInputStream.hpp>

#include <catch2/catch_test_macros.hpp>

#include <AudioUtil.hpp>
#include <SystemUtil.hpp>
#include <thread>
#include <type_traits>

TEST_CASE("[Audio] sf::Music", runAudioDeviceTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_copy_constructible_v<sf::Music>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::Music>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Music>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Music>);
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

    SECTION("openFromFile()")
    {
        SECTION("Invalid file")
        {
            CHECK(!sf::Music::openFromFile("does/not/exist.wav"));
        }

        SECTION("Valid file")
        {
            const auto music = sf::Music::openFromFile("Audio/ding.mp3").value();
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
        std::vector<std::byte> memory(10, std::byte{0xCA});

        SECTION("Invalid buffer")
        {
            CHECK(!sf::Music::openFromMemory(memory.data(), memory.size()));
        }

        SECTION("Valid buffer")
        {
            memory = loadIntoMemory("Audio/ding.flac");

            const auto music = sf::Music::openFromMemory(memory.data(), memory.size()).value();
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

        SECTION("Invalid stream")
        {
            CHECK(!sf::Music::openFromStream(stream));
        }

        SECTION("Valid stream")
        {
            REQUIRE(stream.open("Audio/doodle_pop.ogg"));

            const auto music = sf::Music::openFromStream(stream).value();
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
        auto music = sf::Music::openFromFile("Audio/ding.mp3").value();

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
        auto music = sf::Music::openFromFile("Audio/killdeer.wav").value();
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

#include <SFML/Audio/InputSoundFile.hpp>

// Other 1st party headers
#include <SFML/System/FileInputStream.hpp>
#include <SFML/System/Time.hpp>

#include <catch2/catch_test_macros.hpp>

#include <SystemUtil.hpp>
#include <array>
#include <fstream>
#include <type_traits>

TEST_CASE("[Audio] sf::InputSoundFile")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_default_constructible_v<sf::InputSoundFile>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::InputSoundFile>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::InputSoundFile>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::InputSoundFile>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::InputSoundFile>);
    }

    SECTION("openFromFile()")
    {
        SECTION("Invalid file")
        {
            CHECK(!sf::InputSoundFile::openFromFile("does/not/exist.wav"));
        }

        SECTION("Valid file")
        {
            SECTION("flac")
            {
                const auto inputSoundFile = sf::InputSoundFile::openFromFile("Audio/ding.flac").value();
                CHECK(inputSoundFile.getSampleCount() == 87'798);
                CHECK(inputSoundFile.getChannelCount() == 1);
                CHECK(inputSoundFile.getSampleRate() == 44'100);
                CHECK(inputSoundFile.getDuration() == sf::microseconds(1'990'884));
                CHECK(inputSoundFile.getTimeOffset() == sf::Time::Zero);
                CHECK(inputSoundFile.getSampleOffset() == 0);
            }

            SECTION("mp3")
            {
                const auto inputSoundFile = sf::InputSoundFile::openFromFile("Audio/ding.mp3").value();
                CHECK(inputSoundFile.getSampleCount() == 87'798);
                CHECK(inputSoundFile.getChannelCount() == 1);
                CHECK(inputSoundFile.getSampleRate() == 44'100);
                CHECK(inputSoundFile.getDuration() == sf::microseconds(1'990'884));
                CHECK(inputSoundFile.getTimeOffset() == sf::Time::Zero);
                CHECK(inputSoundFile.getSampleOffset() == 0);
            }

            SECTION("ogg")
            {
                const auto inputSoundFile = sf::InputSoundFile::openFromFile("Audio/doodle_pop.ogg").value();
                CHECK(inputSoundFile.getSampleCount() == 2'116'992);
                CHECK(inputSoundFile.getChannelCount() == 2);
                CHECK(inputSoundFile.getSampleRate() == 44'100);
                CHECK(inputSoundFile.getDuration() == sf::microseconds(24'002'176));
                CHECK(inputSoundFile.getTimeOffset() == sf::Time::Zero);
                CHECK(inputSoundFile.getSampleOffset() == 0);
            }

            SECTION("wav")
            {
                const auto inputSoundFile = sf::InputSoundFile::openFromFile("Audio/killdeer.wav").value();
                CHECK(inputSoundFile.getSampleCount() == 112'941);
                CHECK(inputSoundFile.getChannelCount() == 1);
                CHECK(inputSoundFile.getSampleRate() == 22'050);
                CHECK(inputSoundFile.getDuration() == sf::microseconds(5'122'040));
                CHECK(inputSoundFile.getTimeOffset() == sf::Time::Zero);
                CHECK(inputSoundFile.getSampleOffset() == 0);
            }
        }
    }

    SECTION("openFromMemory()")
    {
        const auto memory         = loadIntoMemory("Audio/killdeer.wav");
        const auto inputSoundFile = sf::InputSoundFile::openFromMemory(memory.data(), memory.size()).value();
        CHECK(inputSoundFile.getSampleCount() == 112'941);
        CHECK(inputSoundFile.getChannelCount() == 1);
        CHECK(inputSoundFile.getSampleRate() == 22'050);
        CHECK(inputSoundFile.getDuration() == sf::microseconds(5'122'040));
        CHECK(inputSoundFile.getTimeOffset() == sf::Time::Zero);
        CHECK(inputSoundFile.getSampleOffset() == 0);
    }

    SECTION("openFromStream()")
    {
        sf::FileInputStream stream;

        SECTION("Invalid stream")
        {
            CHECK(!sf::InputSoundFile::openFromStream(stream));
        }

        SECTION("Valid stream")
        {
            SECTION("flac")
            {
                REQUIRE(stream.open("Audio/ding.flac"));
                const auto inputSoundFile = sf::InputSoundFile::openFromStream(stream).value();
                CHECK(inputSoundFile.getSampleCount() == 87'798);
                CHECK(inputSoundFile.getChannelCount() == 1);
                CHECK(inputSoundFile.getSampleRate() == 44'100);
                CHECK(inputSoundFile.getDuration() == sf::microseconds(1'990'884));
                CHECK(inputSoundFile.getTimeOffset() == sf::Time::Zero);
                CHECK(inputSoundFile.getSampleOffset() == 0);
            }

            SECTION("mp3")
            {
                REQUIRE(stream.open("Audio/ding.mp3"));
                const auto inputSoundFile = sf::InputSoundFile::openFromStream(stream).value();
                CHECK(inputSoundFile.getSampleCount() == 87'798);
                CHECK(inputSoundFile.getChannelCount() == 1);
                CHECK(inputSoundFile.getSampleRate() == 44'100);
                CHECK(inputSoundFile.getDuration() == sf::microseconds(1'990'884));
                CHECK(inputSoundFile.getTimeOffset() == sf::Time::Zero);
                CHECK(inputSoundFile.getSampleOffset() == 0);
            }

            SECTION("ogg")
            {
                REQUIRE(stream.open("Audio/doodle_pop.ogg"));
                const auto inputSoundFile = sf::InputSoundFile::openFromStream(stream).value();
                CHECK(inputSoundFile.getSampleCount() == 2'116'992);
                CHECK(inputSoundFile.getChannelCount() == 2);
                CHECK(inputSoundFile.getSampleRate() == 44'100);
                CHECK(inputSoundFile.getDuration() == sf::microseconds(24'002'176));
                CHECK(inputSoundFile.getTimeOffset() == sf::Time::Zero);
                CHECK(inputSoundFile.getSampleOffset() == 0);
            }

            SECTION("wav")
            {
                REQUIRE(stream.open("Audio/killdeer.wav"));
                const auto inputSoundFile = sf::InputSoundFile::openFromStream(stream).value();
                CHECK(inputSoundFile.getSampleCount() == 112'941);
                CHECK(inputSoundFile.getChannelCount() == 1);
                CHECK(inputSoundFile.getSampleRate() == 22'050);
                CHECK(inputSoundFile.getDuration() == sf::microseconds(5'122'040));
                CHECK(inputSoundFile.getTimeOffset() == sf::Time::Zero);
                CHECK(inputSoundFile.getSampleOffset() == 0);
            }
        }
    }

    SECTION("seek(std::uint64_t)")
    {
        SECTION("flac")
        {
            auto inputSoundFile = sf::InputSoundFile::openFromFile("Audio/ding.flac").value();
            inputSoundFile.seek(1'000);
            CHECK(inputSoundFile.getTimeOffset() == sf::microseconds(22'675));
            CHECK(inputSoundFile.getSampleOffset() == 1'000);
        }

        SECTION("mp3")
        {
            auto inputSoundFile = sf::InputSoundFile::openFromFile("Audio/ding.mp3").value();
            inputSoundFile.seek(1'000);
            CHECK(inputSoundFile.getTimeOffset() == sf::microseconds(22'675));
            CHECK(inputSoundFile.getSampleOffset() == 1'000);
        }

        SECTION("ogg")
        {
            auto inputSoundFile = sf::InputSoundFile::openFromFile("Audio/doodle_pop.ogg").value();
            inputSoundFile.seek(1'000);
            CHECK(inputSoundFile.getTimeOffset() == sf::microseconds(11'337));
            CHECK(inputSoundFile.getSampleOffset() == 1'000);
        }

        SECTION("wav")
        {
            auto inputSoundFile = sf::InputSoundFile::openFromFile("Audio/killdeer.wav").value();
            inputSoundFile.seek(1'000);
            CHECK(inputSoundFile.getTimeOffset() == sf::microseconds(45'351));
            CHECK(inputSoundFile.getSampleOffset() == 1'000);
        }
    }

    SECTION("seek(Time)")
    {
        auto inputSoundFile = sf::InputSoundFile::openFromFile("Audio/ding.flac").value();
        inputSoundFile.seek(sf::milliseconds(100));
        CHECK(inputSoundFile.getSampleCount() == 87'798);
        CHECK(inputSoundFile.getChannelCount() == 1);
        CHECK(inputSoundFile.getSampleRate() == 44'100);
        CHECK(inputSoundFile.getDuration() == sf::microseconds(1'990'884));
        CHECK(inputSoundFile.getTimeOffset() == sf::milliseconds(100));
        CHECK(inputSoundFile.getSampleOffset() == 4'410);
    }

    SECTION("read()")
    {
        auto inputSoundFile = sf::InputSoundFile::openFromFile("Audio/ding.flac").value();

        SECTION("Null address")
        {
            CHECK(inputSoundFile.read(nullptr, 10) == 0);
        }

        std::array<std::int16_t, 4> samples{};

        SECTION("Zero count")
        {
            CHECK(inputSoundFile.read(samples.data(), 0) == 0);
        }

        SECTION("Successful read")
        {
            SECTION("flac")
            {
                inputSoundFile = sf::InputSoundFile::openFromFile("Audio/ding.flac").value();
                CHECK(inputSoundFile.read(samples.data(), samples.size()) == 4);
                CHECK(samples == std::array<std::int16_t, 4>{0, 1, -1, 4});
                CHECK(inputSoundFile.read(samples.data(), samples.size()) == 4);
                CHECK(samples == std::array<std::int16_t, 4>{1, 4, 9, 6});
            }

            SECTION("mp3")
            {
                inputSoundFile = sf::InputSoundFile::openFromFile("Audio/ding.mp3").value();
                CHECK(inputSoundFile.read(samples.data(), samples.size()) == 4);
                CHECK(samples == std::array<std::int16_t, 4>{0, -2, 0, 2});
                CHECK(inputSoundFile.read(samples.data(), samples.size()) == 4);
                CHECK(samples == std::array<std::int16_t, 4>{1, 4, 6, 8});
            }

            SECTION("ogg")
            {
                inputSoundFile = sf::InputSoundFile::openFromFile("Audio/doodle_pop.ogg").value();
                CHECK(inputSoundFile.read(samples.data(), samples.size()) == 4);
                CHECK(samples == std::array<std::int16_t, 4>{-827, -985, -1168, -1319});
                CHECK(inputSoundFile.read(samples.data(), samples.size()) == 4);
                CHECK(samples == std::array<std::int16_t, 4>{-1738, -1883, -2358, -2497});
            }

            SECTION("wav")
            {
                // Cannot be tested since reading from a .wav file triggers UB
            }
        }
    }

    SECTION("close()")
    {
        auto inputSoundFile = sf::InputSoundFile::openFromFile("Audio/ding.flac").value();
        inputSoundFile.close();
        CHECK(inputSoundFile.getSampleCount() == 0);
        CHECK(inputSoundFile.getChannelCount() == 0);
        CHECK(inputSoundFile.getSampleRate() == 0);
        CHECK(inputSoundFile.getDuration() == sf::Time::Zero);
        CHECK(inputSoundFile.getTimeOffset() == sf::Time::Zero);
        CHECK(inputSoundFile.getSampleOffset() == 0);
    }
}

#include <SFML/Audio/SoundFileFactory.hpp>

// Other 1st party headers
#include <SFML/Audio/SoundFileReader.hpp>
#include <SFML/Audio/SoundFileWriter.hpp>

#include <SFML/System/FileInputStream.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

TEST_CASE("[Audio] sf::SoundFileFactory")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::SoundFileFactory>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::SoundFileFactory>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::SoundFileFactory>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::SoundFileFactory>);
    }

    SECTION("createReaderFromFilename()")
    {
        SECTION("Missing file")
        {
            CHECK(!sf::SoundFileFactory::createReaderFromFilename("does/not/exist.wav"));
        }

        SECTION("Valid file")
        {
            CHECK(sf::SoundFileFactory::createReaderFromFilename("Audio/ding.flac"));
            CHECK(sf::SoundFileFactory::createReaderFromFilename("Audio/ding.mp3"));
            CHECK(sf::SoundFileFactory::createReaderFromFilename("Audio/doodle_pop.ogg"));
            CHECK(sf::SoundFileFactory::createReaderFromFilename("Audio/killdeer.wav"));
        }
    }

    SECTION("createReaderFromStream()")
    {
        sf::FileInputStream stream;

        SECTION("Invalid stream")
        {
            CHECK(!sf::SoundFileFactory::createReaderFromStream(stream));
        }

        SECTION("Valid file")
        {
            SECTION("flac")
            {
                REQUIRE(stream.open("Audio/ding.flac"));
            }

            SECTION("mp3")
            {
                REQUIRE(stream.open("Audio/ding.mp3"));
            }

            SECTION("ogg")
            {
                REQUIRE(stream.open("Audio/doodle_pop.ogg"));
            }

            SECTION("wav")
            {
                REQUIRE(stream.open("Audio/killdeer.wav"));
            }

            CHECK(sf::SoundFileFactory::createReaderFromStream(stream));
        }
    }

    SECTION("createWriterFromFilename()")
    {
        SECTION("Invalid extension")
        {
            CHECK(!sf::SoundFileFactory::createWriterFromFilename("cannot/write/to.txt"));
        }

        SECTION("Valid extension")
        {
            CHECK(sf::SoundFileFactory::createWriterFromFilename("file.flac"));
            CHECK(!sf::SoundFileFactory::createWriterFromFilename("file.mp3")); // Mp3 writing not yet implemented
            CHECK(sf::SoundFileFactory::createWriterFromFilename("file.ogg"));
            CHECK(sf::SoundFileFactory::createWriterFromFilename("file.wav"));
        }
    }
}

#include <SFML/Audio/SoundFileFactory.hpp>

// Other 1st party headers
#include <SFML/Audio/SoundChannel.hpp>
#include <SFML/Audio/SoundFileReader.hpp>
#include <SFML/Audio/SoundFileWriter.hpp>

#include <SFML/System/FileInputStream.hpp>
#include <SFML/System/InputStream.hpp>

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <type_traits>

#include <cstdint>

namespace
{

struct NoopSoundFileReader : sf::SoundFileReader
{
    static bool check(sf::InputStream&)
    {
        return false;
    }

    std::optional<Info> open(sf::InputStream&) override
    {
        return {};
    }

    void seek(std::uint64_t) override
    {
    }

    std::uint64_t read(std::int16_t*, std::uint64_t) override
    {
        return 0;
    }
};

struct NoopSoundFileWriter : sf::SoundFileWriter
{
    static bool check(const std::filesystem::path&)
    {
        return false;
    }

    bool open(const std::filesystem::path&, unsigned int, unsigned int, const std::vector<sf::SoundChannel>&) override
    {
        return false;
    }

    void write(const std::int16_t*, std::uint64_t) override
    {
    }
};

} // namespace

TEST_CASE("[Audio] sf::SoundFileFactory")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::SoundFileFactory>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::SoundFileFactory>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::SoundFileFactory>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::SoundFileFactory>);
    }

    SECTION("isReaderRegistered()")
    {
        CHECK(!sf::SoundFileFactory::isReaderRegistered<NoopSoundFileReader>());

        sf::SoundFileFactory::registerReader<NoopSoundFileReader>();
        CHECK(sf::SoundFileFactory::isReaderRegistered<NoopSoundFileReader>());

        sf::SoundFileFactory::unregisterReader<NoopSoundFileReader>();
        CHECK(!sf::SoundFileFactory::isReaderRegistered<NoopSoundFileReader>());
    }

    SECTION("isWriterRegistered()")
    {
        CHECK(!sf::SoundFileFactory::isWriterRegistered<NoopSoundFileWriter>());

        sf::SoundFileFactory::registerWriter<NoopSoundFileWriter>();
        CHECK(sf::SoundFileFactory::isWriterRegistered<NoopSoundFileWriter>());

        sf::SoundFileFactory::unregisterWriter<NoopSoundFileWriter>();
        CHECK(!sf::SoundFileFactory::isWriterRegistered<NoopSoundFileWriter>());
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

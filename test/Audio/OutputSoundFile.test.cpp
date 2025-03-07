#include <SFML/Audio/OutputSoundFile.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <type_traits>

TEST_CASE("[Audio] sf::OutputSoundFile")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_default_constructible_v<sf::OutputSoundFile>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::OutputSoundFile>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::OutputSoundFile>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::OutputSoundFile>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::OutputSoundFile>);
    }

    const std::u32string stem      = GENERATE(U"tmp", U"tmp-ń", U"tmp-🐌");
    const std::u32string extension = GENERATE(U".wav", U".ogg", U".flac");
    const auto           filename  = std::filesystem::temp_directory_path() / std::filesystem::path(stem + extension);
    const std::vector<sf::SoundChannel> channelMap{sf::SoundChannel::FrontLeft, sf::SoundChannel::FrontRight};

    INFO("Filename: " << reinterpret_cast<const char*>(filename.u8string().c_str()));

    SECTION("Construction")
    {
        {
            const sf::OutputSoundFile outputSoundFile(filename, 44'100, static_cast<unsigned int>(channelMap.size()), channelMap);
            CHECK(std::filesystem::exists(filename));
        }
        CHECK(std::filesystem::remove(filename));
    }

    SECTION("openFromFile()")
    {
        sf::OutputSoundFile outputSoundFile;
        CHECK(outputSoundFile.openFromFile(filename, 44'100, static_cast<unsigned int>(channelMap.size()), channelMap));
        CHECK(std::filesystem::exists(filename));
        outputSoundFile.close();
        CHECK(std::filesystem::remove(filename));
    }
}

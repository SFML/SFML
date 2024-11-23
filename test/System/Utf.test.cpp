#include <SFML/System/Utf.hpp>

#include <catch2/catch_test_macros.hpp>

#include <string_view>

using namespace std::string_view_literals;

TEST_CASE("[System] sf::Utf8")
{
    static constexpr std::string_view input = "Hello, World!"sv;

    SECTION("decode")
    {
        std::u32string output;
        char32_t       character = 0;
        for (std::string_view::const_iterator begin = input.begin(); begin < input.end();)
        {
            begin = sf::Utf8::decode(begin, input.end(), character);
            output.push_back(character);
        }
        CHECK(output == U"Hello, World!"sv);
    }

    SECTION("encode")
    {
    }

    SECTION("next")
    {
    }

    SECTION("count")
    {
    }

    SECTION("fromAnsi")
    {
    }

    SECTION("fromWide")
    {
    }

    SECTION("fromLatin1")
    {
    }

    SECTION("toAnsi")
    {
    }

    SECTION("toWide")
    {
    }

    SECTION("toLatin1")
    {
    }

    SECTION("toUtf8")
    {
        std::string output;
        sf::Utf8::toUtf8(input.begin(), input.end(), std::back_inserter(output));
        CHECK(output == input);
    }

    SECTION("toUtf16")
    {
    }

    SECTION("toUtf32")
    {
    }
}

TEST_CASE("[System] sf::Utf16")
{
    static constexpr std::u16string_view input = u"Hello, World!"sv;

    SECTION("decode")
    {
    }

    SECTION("encode")
    {
    }

    SECTION("next")
    {
    }

    SECTION("count")
    {
    }

    SECTION("fromAnsi")
    {
    }

    SECTION("fromWide")
    {
    }

    SECTION("fromLatin1")
    {
        std::u16string output;
        sf::Utf16::fromLatin1(input.begin(), input.end(), std::back_inserter(output));
        CHECK(output == input);
    }

    SECTION("toAnsi")
    {
    }

    SECTION("toWide")
    {
    }

    SECTION("toLatin1")
    {
        std::string output;
        sf::Utf16::toLatin1(input.begin(), input.end(), std::back_inserter(output));
        CHECK(output == "Hello, World!"sv);
    }

    SECTION("toUtf8")
    {
    }

    SECTION("toUtf16")
    {
        std::u16string output;
        sf::Utf16::toUtf16(input.begin(), input.end(), std::back_inserter(output));
        CHECK(output == input);
    }

    SECTION("toUtf32")
    {
    }
}

TEST_CASE("[System] sf::Utf32")
{
    static constexpr std::u32string_view input = U"Hello, World!"sv;

    SECTION("decode")
    {
        std::u32string output;
        char32_t       character = 0;
        for (std::u32string_view::const_iterator begin = input.begin(); begin < input.end();)
        {
            begin = sf::Utf32::decode(begin, {}, character);
            output.push_back(character);
        }
        CHECK(output == input);
    }

    SECTION("encode")
    {
        std::u32string output;
        for (const auto character : input)
            sf::Utf32::encode(character, std::back_inserter(output));
        CHECK(output == input);
    }

    SECTION("next")
    {
        CHECK(sf::Utf32::next(input.begin(), {}) == std::next(input.begin()));
    }

    SECTION("count")
    {
        CHECK(sf::Utf32::count(input.begin(), input.end()) == input.size());
    }

    SECTION("fromAnsi")
    {
    }

    SECTION("fromWide")
    {
    }

    SECTION("fromLatin1")
    {
        std::u32string output;
        sf::Utf32::fromLatin1(input.begin(), input.end(), std::back_inserter(output));
        CHECK(output == input);
    }

    SECTION("toAnsi")
    {
    }

    SECTION("toWide")
    {
    }

    SECTION("toLatin1")
    {
        std::string output;
        sf::Utf32::toLatin1(input.begin(), input.end(), std::back_inserter(output));
        CHECK(output == "Hello, World!");
    }

    SECTION("toUtf8")
    {
    }

    SECTION("toUtf16")
    {
    }

    SECTION("toUtf32")
    {
        std::u32string output;
        sf::Utf32::toUtf32(input.begin(), input.end(), std::back_inserter(output));
        CHECK(output == input);
    }

    SECTION("decodeAnsi")
    {
    }

    SECTION("decodeWide")
    {
        CHECK(sf::Utf32::decodeWide(0) == 0);
        CHECK(sf::Utf32::decodeWide(1) == 1);
        CHECK(sf::Utf32::decodeWide(-1) == std::numeric_limits<std::uint32_t>::max());
    }

    SECTION("encodeAnsi")
    {
    }

    SECTION("encodeWide")
    {
    }
}

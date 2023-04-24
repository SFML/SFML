#include <SFML/System/String.hpp>

#include <doctest/doctest.h>

#include <GraphicsUtil.hpp>
#include <array>
#include <iomanip>
#include <sstream>
#include <type_traits>

#include <cassert>

static_assert(std::is_copy_constructible_v<sf::String>);
static_assert(std::is_copy_assignable_v<sf::String>);
static_assert(std::is_nothrow_move_constructible_v<sf::String>);
static_assert(std::is_nothrow_move_assignable_v<sf::String>);

namespace
{
// Return either argument depending on whether wchar_t is 16 or 32 bits
// Lets us write tests that work on both Windows where wchar_t is 16 bits
// and elsewhere where it is 32. Otherwise the tests would only work on
// one OS or the other.
template <typename T>
auto select(const std::basic_string<T>& string16, const std::basic_string<T>& string32)
{
    assert(string16 != string32);
    if constexpr (sizeof(wchar_t) == 2)
        return string16;
    else
        return string32;
}

template <typename CharT>
auto toHex(const CharT character)
{
    std::ostringstream stream;
    stream << "[\\x" << std::uppercase << std::hex << static_cast<std::uint32_t>(character) << ']';
    return stream.str();
}
} // namespace

// Specialize StringMaker for alternative std::basic_string<T> specializations
// std::string's string conversion cannot be specialized but all other string types get special treatment
// https://github.com/doctest/doctest/blob/master/doc/markdown/stringification.md#docteststringmakert-specialisation
namespace doctest
{
template <typename CharT>
struct StringMaker<std::basic_string<CharT>>
{
    static String convert(const std::basic_string<CharT>& string)
    {
        doctest::String output;
        for (const auto character : string)
        {
            if (character >= 32 && character < 127)
                output += std::string(1, static_cast<char>(character)).c_str();
            else
                output += toHex(character).c_str();
        }
        return output;
    }
};

template <>
struct StringMaker<char32_t>
{
    static String convert(const char32_t character)
    {
        return toHex(character).c_str();
    }
};
} // namespace doctest

TEST_CASE("[System] sf::String")
{
    using namespace std::string_literals;

    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            const sf::String string;
            CHECK(std::string(string) == ""s);
            CHECK(std::wstring(string) == L""s);
            CHECK(string.toAnsiString() == ""s);
            CHECK(string.toWideString() == L""s);
            CHECK(string.toUtf8() == std::basic_string<std::uint8_t>());
            CHECK(string.toUtf16() == u""s);
            CHECK(string.toUtf32() == U""s);
            CHECK(string.getSize() == 0);
            CHECK(string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SUBCASE("ANSI character constructor")
        {
            const sf::String string = 'a';
            CHECK(std::string(string) == "a"s);
            CHECK(std::wstring(string) == L"a"s);
            CHECK(string.toAnsiString() == "a"s);
            CHECK(string.toWideString() == L"a"s);
            CHECK(string.toUtf8() == std::basic_string<std::uint8_t>{'a'});
            CHECK(string.toUtf16() == u"a"s);
            CHECK(string.toUtf32() == U"a"s);
            CHECK(string.getSize() == 1);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SUBCASE("ANSI C string constructor")
        {
            const sf::String string = "def";
            CHECK(std::string(string) == "def"s);
            CHECK(std::wstring(string) == L"def"s);
            CHECK(string.toAnsiString() == "def"s);
            CHECK(string.toWideString() == L"def"s);
            CHECK(string.toUtf8() == std::basic_string<std::uint8_t>{'d', 'e', 'f'});
            CHECK(string.toUtf16() == u"def"s);
            CHECK(string.toUtf32() == U"def"s);
            CHECK(string.getSize() == 3);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SUBCASE("ANSI string constructor")
        {
            const sf::String string = "ghi"s;
            CHECK(std::string(string) == "ghi"s);
            CHECK(std::wstring(string) == L"ghi"s);
            CHECK(string.toAnsiString() == "ghi"s);
            CHECK(string.toWideString() == L"ghi"s);
            CHECK(string.toUtf8() == std::basic_string<std::uint8_t>{'g', 'h', 'i'});
            CHECK(string.toUtf16() == u"ghi"s);
            CHECK(string.toUtf32() == U"ghi"s);
            CHECK(string.getSize() == 3);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SUBCASE("Wide character constructor")
        {
            const sf::String string = L'\xFA';
            CHECK(std::string(string) == select("\xFA"s, "\0"s));
            CHECK(std::wstring(string) == L"\xFA"s);
            CHECK(string.toAnsiString() == select("\xFA"s, "\0"s));
            CHECK(string.toWideString() == L"\xFA"s);
            CHECK(string.toUtf8() == std::basic_string<std::uint8_t>{0xC3, 0xBA});
            CHECK(string.toUtf16() == u"\xFA"s);
            CHECK(string.toUtf32() == U"\xFA"s);
            CHECK(string.getSize() == 1);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SUBCASE("Wide C string constructor")
        {
            const sf::String string = L"j\xFAl";
            CHECK(std::string(string) == select("j\xFAl"s, "j\0l"s));
            CHECK(std::wstring(string) == L"j\xFAl"s);
            CHECK(string.toAnsiString() == select("j\xFAl"s, "j\0l"s));
            CHECK(string.toWideString() == L"j\xFAl"s);
            CHECK(string.toUtf8() == std::basic_string<std::uint8_t>{'j', 0xC3, 0xBA, 'l'});
            CHECK(string.toUtf16() == u"j\xFAl"s);
            CHECK(string.toUtf32() == U"j\xFAl"s);
            CHECK(string.getSize() == 3);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SUBCASE("Wide string constructor")
        {
            const sf::String string = L"mno\xFA"s;
            CHECK(std::string(string) == select("mno\xFA"s, "mno\0"s));
            CHECK(std::wstring(string) == L"mno\xFA"s);
            CHECK(string.toAnsiString() == select("mno\xFA"s, "mno\0"s));
            CHECK(string.toWideString() == L"mno\xFA"s);
            CHECK(string.toUtf8() == std::basic_string<std::uint8_t>{'m', 'n', 'o', 0xC3, 0XBA});
            CHECK(string.toUtf16() == u"mno\xFA"s);
            CHECK(string.toUtf32() == U"mno\xFA"s);
            CHECK(string.getSize() == 4);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SUBCASE("UTF-32 character constructor")
        {
            const sf::String string = U'\U0010AFAF';
            CHECK(std::string(string) == "\0"s);
            CHECK(std::wstring(string) == select(L""s, L"\U0010AFAF"s));
            CHECK(string.toAnsiString() == "\0"s);
            CHECK(string.toWideString() == select(L""s, L"\U0010AFAF"s));
            CHECK(string.toUtf8() == std::basic_string<std::uint8_t>{0xF4, 0x8A, 0xBE, 0xAF});
            CHECK(string.toUtf16() == u"\U0010AFAF"s);
            CHECK(string.toUtf32() == U"\U0010AFAF"s);
            CHECK(string.getSize() == 1);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SUBCASE("UTF-32 C string constructor")
        {
            const sf::String string = U"\U0010ABCDrs";
            CHECK(std::string(string) == "\0rs"s);
            CHECK(std::wstring(string) == select(L"rs"s, L"\U0010ABCDrs"s));
            CHECK(string.toAnsiString() == "\0rs"s);
            CHECK(string.toWideString() == select(L"rs"s, L"\U0010ABCDrs"s));
            CHECK(string.toUtf8() == std::basic_string<std::uint8_t>{0xF4, 0x8A, 0xAF, 0x8D, 'r', 's'});
            CHECK(string.toUtf16() == u"\U0010ABCDrs"s);
            CHECK(string.toUtf32() == U"\U0010ABCDrs"s);
            CHECK(string.getSize() == 3);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SUBCASE("UTF-32 string constructor")
        {
            const sf::String string = U"tuv\U00104321"s;
            CHECK(std::string(string) == "tuv\0"s);
            CHECK(std::wstring(string) == select(L"tuv"s, L"tuv\U00104321"s));
            CHECK(string.toAnsiString() == "tuv\0"s);
            CHECK(string.toWideString() == select(L"tuv"s, L"tuv\U00104321"s));
            CHECK(string.toUtf8() == std::basic_string<std::uint8_t>{'t', 'u', 'v', 0xF4, 0x84, 0x8C, 0xA1});
            CHECK(string.toUtf16() == u"tuv\U00104321"s);
            CHECK(string.toUtf32() == U"tuv\U00104321"s);
            CHECK(string.getSize() == 4);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }
    }

    SUBCASE("fromUtf8()")
    {
        SUBCASE("Nominal")
        {
            constexpr std::array<std::uint8_t, 4> characters{'w', 'x', 'y', 'z'};
            const sf::String                      string = sf::String::fromUtf8(characters.begin(), characters.end());
            CHECK(std::string(string) == "wxyz"s);
            CHECK(std::wstring(string) == L"wxyz"s);
            CHECK(string.toAnsiString() == "wxyz"s);
            CHECK(string.toWideString() == L"wxyz"s);
            CHECK(string.toUtf8() == std::basic_string<std::uint8_t>{'w', 'x', 'y', 'z'});
            CHECK(string.toUtf16() == u"wxyz"s);
            CHECK(string.toUtf32() == U"wxyz"s);
            CHECK(string.getSize() == 4);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SUBCASE("Insufficient input")
        {
            constexpr std::array<std::uint8_t, 1> characters{251};
            const sf::String                      string = sf::String::fromUtf8(characters.begin(), characters.end());
            constexpr char32_t                    defaultReplacementCharacter = 0;
            CHECK(string.getSize() == 1);
            CHECK(string[0] == defaultReplacementCharacter);
        }
    }

    SUBCASE("fromUtf16()")
    {
        constexpr std::array<std::uint16_t, 4> characters{0xF1, 'x', 'y', 'z'};
        const sf::String                       string = sf::String::fromUtf16(characters.begin(), characters.end());
        CHECK(std::string(string) == select("\xF1xyz"s, "\0xyz"s));
        CHECK(std::wstring(string) == L"\xF1xyz"s);
        CHECK(string.toAnsiString() == select("\xF1xyz"s, "\0xyz"s));
        CHECK(string.toWideString() == L"\xF1xyz"s);
        CHECK(string.toUtf8() == std::basic_string<std::uint8_t>{0xC3, 0xB1, 'x', 'y', 'z'});
        CHECK(string.toUtf16() == u"\xF1xyz"s);
        CHECK(string.toUtf32() == U"\xF1xyz"s);
        CHECK(string.getSize() == 4);
        CHECK(!string.isEmpty());
        CHECK(string.getData() != nullptr);
    }

    SUBCASE("fromUtf32()")
    {
        constexpr std::array<std::uint32_t, 4> characters{'w', 0x104321, 'y', 'z'};
        const sf::String                       string = sf::String::fromUtf32(characters.begin(), characters.end());
        CHECK(std::string(string) == "w\0yz"s);
        CHECK(std::wstring(string) == select(L"wyz"s, L"w\U00104321yz"s));
        CHECK(string.toAnsiString() == "w\0yz"s);
        CHECK(string.toWideString() == select(L"wyz"s, L"w\U00104321yz"s));
        CHECK(string.toUtf8() == std::basic_string<std::uint8_t>{'w', 0xF4, 0x84, 0x8C, 0xA1, 'y', 'z'});
        CHECK(string.toUtf16() == u"w\U00104321yz"s);
        CHECK(string.toUtf32() == U"w\U00104321yz"s);
        CHECK(string.getSize() == 4);
        CHECK(!string.isEmpty());
        CHECK(string.getData() != nullptr);
    }

    SUBCASE("clear()")
    {
        sf::String string("you'll never guess what happens when you call clear()");
        string.clear();
        CHECK(string == sf::String());
        CHECK(string.getSize() == 0);
    }

    SUBCASE("erase()")
    {
        sf::String string("what if i want a shorter string?");
        string.erase(0, 8);
        string.erase(string.getSize() - 1, 1);
        CHECK(string == "i want a shorter string");
        CHECK(string.getSize() == 23);
    }

    SUBCASE("insert()")
    {
        sf::String string("please insert text");
        string.insert(7, "don't ");
        CHECK(string == "please don't insert text");
        CHECK(string.getSize() == 24);
    }

    SUBCASE("find()")
    {
        const sf::String string("a little bit of this and a little bit of that");
        CHECK(string.find("a little bit") == 0);
        CHECK(string.find("a little bit", 15) == 25);
        CHECK(string.find("a little bit", 1'000) == sf::String::InvalidPos);
        CHECK(string.find("no way you find this") == sf::String::InvalidPos);
    }

    SUBCASE("replace()")
    {
        sf::String string("sfml is the worst");
        string.replace(12, 5, "best!");
        CHECK(string == "sfml is the best!");
        string.replace("the", "THE");
        CHECK(string == "sfml is THE best!");
    }

    SUBCASE("substring()")
    {
        const sf::String string("let's get some substrings");
        CHECK(string.substring(0) == "let's get some substrings");
        CHECK(string.substring(10) == "some substrings");
        CHECK(string.substring(10, 4) == "some");
        CHECK_THROWS_AS((void)string.substring(1'000), std::out_of_range);
        CHECK_THROWS_AS((void)string.substring(420, 69), std::out_of_range);
    }

    SUBCASE("begin() and end() const")
    {
        const sf::String string("let's test the const iterators");
        CHECK(*string.begin() == 'l');
        CHECK(*(string.end() - 1) == 's');
        for (const auto character : string)
            CHECK(character != 0);
    }

    SUBCASE("begin() and end()")
    {
        sf::String string("let's test the iterators");
        CHECK(*string.begin() == 'l');
        CHECK(*(string.end() - 1) == 's');
        for (auto& character : string)
            character = 'x';
        CHECK(string == "xxxxxxxxxxxxxxxxxxxxxxxx");
    }

    SUBCASE("Operators")
    {
        SUBCASE("operator+=")
        {
            sf::String string;
            string += sf::String("xyz");
            CHECK(string.toAnsiString() == "xyz"s);
        }

        SUBCASE("operator[] const")
        {
            const sf::String string("the quick brown fox");
            CHECK(string[0] == 't');
            CHECK(string[10] == 'b');
        }

        SUBCASE("operator[]")
        {
            sf::String string("the quick brown fox");
            CHECK(string[0] == 't');
            string[1] = 'x';
            CHECK(string[1] == 'x');
        }

        SUBCASE("operator==")
        {
            CHECK(sf::String() == sf::String());
            CHECK_FALSE(sf::String() == sf::String(' '));
        }

        SUBCASE("operator!=")
        {
            CHECK(sf::String() != sf::String(' '));
            CHECK_FALSE(sf::String() != sf::String());
        }

        SUBCASE("operator<")
        {
            CHECK(sf::String('a') < sf::String('b'));
            CHECK_FALSE(sf::String() < sf::String());
        }

        SUBCASE("operator>")
        {
            CHECK(sf::String('b') > sf::String('a'));
            CHECK_FALSE(sf::String() > sf::String());
        }

        SUBCASE("operator<=")
        {
            CHECK(sf::String() <= sf::String());
            CHECK(sf::String('a') <= sf::String('b'));
        }

        SUBCASE("operator>=")
        {
            CHECK(sf::String() >= sf::String());
            CHECK(sf::String('b') >= sf::String('a'));
        }

        SUBCASE("operator+")
        {
            CHECK(sf::String() + sf::String() == sf::String());
            CHECK(sf::String("abc") + sf::String("def") == sf::String("abcdef"));
        }
    }
}

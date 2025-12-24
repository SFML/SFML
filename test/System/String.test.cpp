#include <SFML/System/String.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <array>
#include <iomanip>
#include <sstream>
#include <type_traits>

#include <cassert>

namespace
{
// Return either argument depending on whether wchar_t is 16 or 32 bits
// Lets us write tests that work on both Windows where wchar_t is 16 bits
// and elsewhere where it is 32. Otherwise the tests would only work on
// one OS or the other.
template <typename T>
auto select(const std::basic_string<T>& string16, const std::basic_string<T>& string32)
{
    assert(string16 != string32 && "Invalid to select between identical inputs");
    if constexpr (sizeof(wchar_t) == 2)
        return string16;
    else
        return string32;
}

auto toHex(const char32_t character)
{
    std::ostringstream stream;
    stream << "[\\x" << std::uppercase << std::hex << std::uint32_t{character} << ']';
    return stream.str();
}
} // namespace

// Specialize StringMaker for alternative std::basic_string<T> specializations
// std::string's string conversion cannot be specialized but all other string types get special treatment
// https://github.com/catchorg/Catch2/blob/devel/docs/tostring.md#catchstringmaker-specialisation
namespace Catch
{
template <>
struct StringMaker<sf::U8String>
{
    static std::string convert(const sf::U8String& string)
    {
        std::ostringstream output;
        for (const auto character : string)
        {
            if (character >= 32 && character < 127)
                output << std::string(1, static_cast<char>(character));
            else
                output << toHex(character);
        }
        return output.str();
    }
};
} // namespace Catch

TEST_CASE("[System] sf::U8StringCharTraits")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_trivially_copy_constructible_v<sf::U8StringCharTraits>);
        STATIC_CHECK(std::is_trivially_copy_assignable_v<sf::U8StringCharTraits>);
        STATIC_CHECK(std::is_trivially_move_constructible_v<sf::U8StringCharTraits>);
        STATIC_CHECK(std::is_trivially_move_assignable_v<sf::U8StringCharTraits>);
    }

    SECTION("assign(char_type&, const char_type&)")
    {
        sf::U8StringCharTraits::char_type       c1 = 'a';
        const sf::U8StringCharTraits::char_type c2 = 'b';
        sf::U8StringCharTraits::assign(c1, c2);
        CHECK(c1 == 'b');
        CHECK(c2 == 'b');
    }

    SECTION("assign(char_type*, std::size_t, char_type)")
    {
        std::array<sf::U8StringCharTraits::char_type, 4> s = {'a', 'b', 'c', '\0'};
        CHECK(sf::U8StringCharTraits::assign(s.data(), 2, 'd') == s.data());
        CHECK(s[0] == 'd');
        CHECK(s[1] == 'd');
        CHECK(s[2] == 'c');
    }

    SECTION("eq()")
    {
        CHECK(sf::U8StringCharTraits::eq(0, 0));
        CHECK(!sf::U8StringCharTraits::eq(1, 0));
        CHECK(!sf::U8StringCharTraits::eq(0, 1));
        CHECK(sf::U8StringCharTraits::eq(1, 1));
    }

    SECTION("lt()")
    {
        CHECK(!sf::U8StringCharTraits::lt(0, 0));
        CHECK(!sf::U8StringCharTraits::lt(1, 0));
        CHECK(sf::U8StringCharTraits::lt(0, 1));
        CHECK(!sf::U8StringCharTraits::lt(1, 1));
    }

    SECTION("move()")
    {
        std::array<sf::U8StringCharTraits::char_type, 4>       s1 = {'a', 'b', 'c', '\0'};
        const std::array<sf::U8StringCharTraits::char_type, 4> s2 = {'d', 'e', 'f', '\0'};
        CHECK(sf::U8StringCharTraits::move(s1.data(), s2.data(), s2.size()) == s1.data());
        CHECK(s1[0] == 'd');
        CHECK(s1[1] == 'e');
        CHECK(s1[2] == 'f');
        CHECK(s2[0] == 'd');
        CHECK(s2[1] == 'e');
        CHECK(s2[2] == 'f');
    }

    SECTION("copy()")
    {
        std::array<sf::U8StringCharTraits::char_type, 4>       s1 = {'a', 'b', 'c', '\0'};
        const std::array<sf::U8StringCharTraits::char_type, 4> s2 = {'d', 'e', 'f', '\0'};
        CHECK(sf::U8StringCharTraits::copy(s1.data(), s2.data(), s2.size()) == s1.data());
        CHECK(s1[0] == 'd');
        CHECK(s1[1] == 'e');
        CHECK(s1[2] == 'f');
        CHECK(s2[0] == 'd');
        CHECK(s2[1] == 'e');
        CHECK(s2[2] == 'f');
    }

    SECTION("compare()")
    {
        const std::array<sf::U8StringCharTraits::char_type, 4> s1 = {'a', 'b', 'c', '\0'};
        const std::array<sf::U8StringCharTraits::char_type, 4> s2 = {'a', 'b', 'c', '\0'};
        const std::array<sf::U8StringCharTraits::char_type, 4> s3 = {'d', 'e', 'f', '\0'};
        CHECK(sf::U8StringCharTraits::compare(s1.data(), s2.data(), s1.size()) == 0);
        CHECK(sf::U8StringCharTraits::compare(s1.data(), s3.data(), s1.size()) < 0);
        CHECK(sf::U8StringCharTraits::compare(s3.data(), s1.data(), s3.size()) > 0);
    }

    SECTION("length()")
    {
        const std::array<sf::U8StringCharTraits::char_type, 2> s1 = {'a', '\0'};
        const std::array<sf::U8StringCharTraits::char_type, 6> s2 = {'a', 'b', 'c', 'd', 'e', '\0'};
        CHECK(sf::U8StringCharTraits::length(s1.data()) == 1);
        CHECK(sf::U8StringCharTraits::length(s2.data()) == 5);
    }

    SECTION("find()")
    {
        const std::array<sf::U8StringCharTraits::char_type, 5> s = {'a', 'b', 'c', 'd', 'e'};
        CHECK(*sf::U8StringCharTraits::find(s.data(), s.size(), 'a') == 'a');
        CHECK(sf::U8StringCharTraits::find(s.data(), s.size(), 'f') == nullptr);
    }

    SECTION("to_char_type()")
    {
        CHECK(sf::U8StringCharTraits::to_char_type(sf::U8StringCharTraits::int_type{0}) ==
              sf::U8StringCharTraits::char_type{0});
        CHECK(sf::U8StringCharTraits::to_char_type(sf::U8StringCharTraits::int_type{1}) ==
              sf::U8StringCharTraits::char_type{1});
        CHECK(sf::U8StringCharTraits::to_char_type(sf::U8StringCharTraits::int_type{10}) ==
              sf::U8StringCharTraits::char_type{10});
    }

    SECTION("to_int_type()")
    {
        CHECK(sf::U8StringCharTraits::to_int_type(sf::U8StringCharTraits::char_type{0}) ==
              sf::U8StringCharTraits::int_type{0});
        CHECK(sf::U8StringCharTraits::to_int_type(sf::U8StringCharTraits::char_type{1}) ==
              sf::U8StringCharTraits::int_type{1});
        CHECK(sf::U8StringCharTraits::to_int_type(sf::U8StringCharTraits::char_type{10}) ==
              sf::U8StringCharTraits::int_type{10});
    }

    SECTION("eq_int_type()")
    {
        CHECK(sf::U8StringCharTraits::eq_int_type(sf::U8StringCharTraits::int_type{0}, sf::U8StringCharTraits::int_type{0}));
        CHECK(sf::U8StringCharTraits::eq_int_type(sf::U8StringCharTraits::int_type{1}, sf::U8StringCharTraits::int_type{1}));
        CHECK(sf::U8StringCharTraits::eq_int_type(sf::U8StringCharTraits::int_type{10},
                                                  sf::U8StringCharTraits::int_type{10}));
    }

    SECTION("eof()")
    {
        CHECK(sf::U8StringCharTraits::eof() == ~sf::U8StringCharTraits::int_type{0});
    }

    SECTION("not_eof()")
    {
        CHECK(sf::U8StringCharTraits::not_eof(sf::U8StringCharTraits::int_type{0}) == sf::U8StringCharTraits::int_type{0});
        CHECK(sf::U8StringCharTraits::not_eof(sf::U8StringCharTraits::int_type{1}) == sf::U8StringCharTraits::int_type{1});
        CHECK(sf::U8StringCharTraits::not_eof(sf::U8StringCharTraits::int_type{10}) == sf::U8StringCharTraits::int_type{10});
        CHECK(sf::U8StringCharTraits::not_eof(sf::U8StringCharTraits::eof()) != sf::U8StringCharTraits::eof());
    }
}

TEST_CASE("[System] sf::String")
{
    using namespace std::string_literals;
    using namespace std::string_view_literals;

    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_constructible_v<sf::String, std::nullptr_t>);
        STATIC_CHECK(!std::is_constructible_v<sf::String, std::nullptr_t, const std::locale&>);
        STATIC_CHECK(std::is_copy_constructible_v<sf::String>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::String>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::String>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::String>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            const sf::String string;
            CHECK(std::string(string).empty());
            CHECK(std::wstring(string).empty());
            CHECK(string.toAnsiString().empty());
            CHECK(string.toWideString().empty());
            CHECK(string.toUtf8().empty());
            CHECK(string.toUtf16().empty());
            CHECK(string.toUtf32().empty());
            CHECK(string.getSize() == 0);
            CHECK(string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SECTION("ANSI character constructor")
        {
            const sf::String string = 'a';
            CHECK(std::string(string) == "a"s);
            CHECK(std::wstring(string) == L"a"s);
            CHECK(string.toAnsiString() == "a"s);
            CHECK(string.toWideString() == L"a"s);
            CHECK(string.toUtf8() == sf::U8String{'a'});
            CHECK(string.toUtf16() == u"a"s);
            CHECK(string.toUtf32() == U"a"s);
            CHECK(string.getSize() == 1);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SECTION("ANSI C string constructor")
        {
            {
                const sf::String string = static_cast<char*>(nullptr);
                CHECK(std::string(string).empty());
                CHECK(std::wstring(string).empty());
                CHECK(string.toAnsiString().empty());
                CHECK(string.toWideString().empty());
                CHECK(string.toUtf8().empty());
                CHECK(string.toUtf16().empty());
                CHECK(string.toUtf32().empty());
                CHECK(string.getSize() == 0);
                CHECK(string.isEmpty());
                CHECK(string.getData() != nullptr);
            }
            {
                const sf::String string = "Escargot";
                CHECK(std::string(string) == "Escargot"s);
                CHECK(std::wstring(string) == L"Escargot"s);
                CHECK(string.toAnsiString() == "Escargot"s);
                CHECK(string.toWideString() == L"Escargot"s);
                CHECK(string.toUtf8() == sf::U8String{'E', 's', 'c', 'a', 'r', 'g', 'o', 't'});
                CHECK(string.toUtf16() == u"Escargot"s);
                CHECK(string.toUtf32() == U"Escargot"s);
                CHECK(string.getSize() == 8);
                CHECK(!string.isEmpty());
                CHECK(string.getData() != nullptr);
            }
        }

        SECTION("ANSI string constructor")
        {
            const sf::String string = "Csiga"s;
            CHECK(std::string(string) == "Csiga"s);
            CHECK(std::wstring(string) == L"Csiga"s);
            CHECK(string.toAnsiString() == "Csiga"s);
            CHECK(string.toWideString() == L"Csiga"s);
            CHECK(string.toUtf8() == sf::U8String{'C', 's', 'i', 'g', 'a'});
            CHECK(string.toUtf16() == u"Csiga"s);
            CHECK(string.toUtf32() == U"Csiga"s);
            CHECK(string.getSize() == 5);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SECTION("ANSI string view constructor")
        {
            const sf::String string = "Csiga"sv;
            CHECK(std::string(string) == "Csiga"s);
            CHECK(std::wstring(string) == L"Csiga"s);
            CHECK(string.toAnsiString() == "Csiga"s);
            CHECK(string.toWideString() == L"Csiga"s);
            CHECK(string.toUtf8() == sf::U8String{'C', 's', 'i', 'g', 'a'});
            CHECK(string.toUtf16() == u"Csiga"s);
            CHECK(string.toUtf32() == U"Csiga"s);
            CHECK(string.getSize() == 5);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SECTION("Wide character constructor")
        {
            {
                const sf::String string = L'ú';
                CHECK(std::string(string) == select("\xFA"s, "\0"s));
                CHECK(std::wstring(string) == L"ú"s);
                CHECK(string.toAnsiString() == select("\xFA"s, "\0"s));
                CHECK(string.toWideString() == L"ú"s);
                CHECK(string.toUtf8() == sf::U8String{0xC3, 0xBA});
                CHECK(string.toUtf16() == u"ú"s);
                CHECK(string.toUtf32() == U"ú"s);
                CHECK(string.getSize() == 1);
                CHECK(!string.isEmpty());
                CHECK(string.getData() != nullptr);
            }
            {
                const sf::String string = L'Ǻ';
                CHECK(std::string(string) == "\0"s);
                CHECK(std::wstring(string) == L"Ǻ"s);
                CHECK(string.toAnsiString() == "\0"s);
                CHECK(string.toWideString() == L"Ǻ"s);
                CHECK(string.toUtf8() == sf::U8String{0xC7, 0xBA});
                CHECK(string.toUtf16() == u"Ǻ"s);
                CHECK(string.toUtf32() == U"Ǻ"s);
                CHECK(string.getSize() == 1);
                CHECK(!string.isEmpty());
                CHECK(string.getData() != nullptr);
            }
        }

        SECTION("Wide C string constructor")
        {
            {
                const sf::String string = static_cast<wchar_t*>(nullptr);
                CHECK(std::string(string).empty());
                CHECK(std::wstring(string).empty());
                CHECK(string.toAnsiString().empty());
                CHECK(string.toWideString().empty());
                CHECK(string.toUtf8().empty());
                CHECK(string.toUtf16().empty());
                CHECK(string.toUtf32().empty());
                CHECK(string.getSize() == 0);
                CHECK(string.isEmpty());
                CHECK(string.getData() != nullptr);
            }
            {
                const sf::String string = L"Улитка";
                CHECK(std::string(string) == "\0\0\0\0\0\0"s);
                CHECK(std::wstring(string) == L"Улитка"s);
                CHECK(string.toAnsiString() == "\0\0\0\0\0\0"s);
                CHECK(string.toWideString() == L"Улитка"s);
                CHECK(string.toUtf8() ==
                      sf::U8String{0xD0, 0xA3, 0xD0, 0xBB, 0xD0, 0xB8, 0xD1, 0x82, 0xD0, 0xBA, 0xD0, 0xB0});
                CHECK(string.toUtf16() == u"Улитка"s);
                CHECK(string.toUtf32() == U"Улитка"s);
                CHECK(string.getSize() == 6);
                CHECK(!string.isEmpty());
                CHECK(string.getData() != nullptr);
            }
        }

        SECTION("Wide string constructor")
        {
            const sf::String string = L"Полжав"s;
            CHECK(std::string(string) == "\0\0\0\0\0\0"s);
            CHECK(std::wstring(string) == L"Полжав"s);
            CHECK(string.toAnsiString() == "\0\0\0\0\0\0"s);
            CHECK(string.toWideString() == L"Полжав"s);
            CHECK(string.toUtf8() == sf::U8String{0xD0, 0x9F, 0xD0, 0xBE, 0xD0, 0xBB, 0xD0, 0xB6, 0xD0, 0xB0, 0xD0, 0xB2});
            CHECK(string.toUtf16() == u"Полжав"s);
            CHECK(string.toUtf32() == U"Полжав"s);
            CHECK(string.getSize() == 6);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SECTION("Wide string view constructor")
        {
            const sf::String string = L"Полжав"sv;
            CHECK(std::string(string) == "\0\0\0\0\0\0"s);
            CHECK(std::wstring(string) == L"Полжав"s);
            CHECK(string.toAnsiString() == "\0\0\0\0\0\0"s);
            CHECK(string.toWideString() == L"Полжав"s);
            CHECK(string.toUtf8() == sf::U8String{0xD0, 0x9F, 0xD0, 0xBE, 0xD0, 0xBB, 0xD0, 0xB6, 0xD0, 0xB0, 0xD0, 0xB2});
            CHECK(string.toUtf16() == u"Полжав"s);
            CHECK(string.toUtf32() == U"Полжав"s);
            CHECK(string.getSize() == 6);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SECTION("UTF-32 character constructor")
        {
            const sf::String string = U'🐌';
            CHECK(std::string(string) == "\0"s);
            CHECK(std::wstring(string) == select(L""s, L"🐌"s));
            CHECK(string.toAnsiString() == "\0"s);
            CHECK(string.toWideString() == select(L""s, L"🐌"s));
            CHECK(string.toUtf8() == sf::U8String{0xF0, 0x9F, 0x90, 0x8C});
            CHECK(string.toUtf16() == u"🐌"s);
            CHECK(string.toUtf32() == U"🐌"s);
            CHECK(string.getSize() == 1);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SECTION("UTF-32 C string constructor")
        {
            {
                const sf::String string = static_cast<char32_t*>(nullptr);
                CHECK(std::string(string).empty());
                CHECK(std::wstring(string).empty());
                CHECK(string.toAnsiString().empty());
                CHECK(string.toWideString().empty());
                CHECK(string.toUtf8().empty());
                CHECK(string.toUtf16().empty());
                CHECK(string.toUtf32().empty());
                CHECK(string.getSize() == 0);
                CHECK(string.isEmpty());
                CHECK(string.getData() != nullptr);
            }
            {
                const sf::String string = U"カタツムリ";
                CHECK(std::string(string) == "\0\0\0\0\0"s);
                CHECK(std::wstring(string) == L"カタツムリ"s);
                CHECK(string.toAnsiString() == "\0\0\0\0\0"s);
                CHECK(string.toWideString() == L"カタツムリ"s);
                CHECK(string.toUtf8() ==
                      sf::U8String{0xE3, 0x82, 0xAB, 0xE3, 0x82, 0xBF, 0xE3, 0x83, 0x84, 0xE3, 0x83, 0xA0, 0xE3, 0x83, 0xAA});
                CHECK(string.toUtf16() == u"カタツムリ"s);
                CHECK(string.toUtf32() == U"カタツムリ"s);
                CHECK(string.getSize() == 5);
                CHECK(!string.isEmpty());
                CHECK(string.getData() != nullptr);
            }
            {
                const sf::String string = U"🐌🐚";
                CHECK(std::string(string) == "\0\0"s);
                CHECK(std::wstring(string) == select(L""s, L"🐌🐚"s));
                CHECK(string.toAnsiString() == "\0\0"s);
                CHECK(string.toWideString() == select(L""s, L"🐌🐚"s));
                CHECK(string.toUtf8() == sf::U8String{0xF0, 0x9F, 0x90, 0x8C, 0xF0, 0x9F, 0x90, 0x9A});
                CHECK(string.toUtf16() == u"🐌🐚"s);
                CHECK(string.toUtf32() == U"🐌🐚"s);
                CHECK(string.getSize() == 2);
                CHECK(!string.isEmpty());
                CHECK(string.getData() != nullptr);
            }
        }

        SECTION("UTF-32 string constructor")
        {
            const sf::String string = U"گھونگا"s;
            CHECK(std::string(string) == "\0\0\0\0\0\0"s);
            CHECK(std::wstring(string) == L"گھونگا"s);
            CHECK(string.toAnsiString() == "\0\0\0\0\0\0"s);
            CHECK(string.toWideString() == L"گھونگا"s);
            CHECK(string.toUtf8() == sf::U8String{0xDA, 0xAF, 0xDA, 0xBE, 0xD9, 0x88, 0xD9, 0x86, 0xDA, 0xAF, 0xD8, 0xA7});
            CHECK(string.toUtf16() == u"گھونگا"s);
            CHECK(string.toUtf32() == U"گھونگا"s);
            CHECK(string.getSize() == 6);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }

        SECTION("UTF-32 string view constructor")
        {
            const sf::String string = U"گھونگا"sv;
            CHECK(std::string(string) == "\0\0\0\0\0\0"s);
            CHECK(std::wstring(string) == L"گھونگا"s);
            CHECK(string.toAnsiString() == "\0\0\0\0\0\0"s);
            CHECK(string.toWideString() == L"گھونگا"s);
            CHECK(string.toUtf8() == sf::U8String{0xDA, 0xAF, 0xDA, 0xBE, 0xD9, 0x88, 0xD9, 0x86, 0xDA, 0xAF, 0xD8, 0xA7});
            CHECK(string.toUtf16() == u"گھونگا"s);
            CHECK(string.toUtf32() == U"گھونگا"s);
            CHECK(string.getSize() == 6);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }
    }

    SECTION("fromUtf8()")
    {
        {
            constexpr std::array<std::uint8_t, 1> characters{251};
            const sf::String                      string = sf::String::fromUtf8(characters.begin(), characters.end());
            CHECK(string.getSize() == 1);
            CHECK(string[0] == 0);
        }
        {
            constexpr std::array<std::uint8_t, 4> characters{'w', 'x', 'y', 'z'};
            const sf::String                      string = sf::String::fromUtf8(characters.begin(), characters.end());
            CHECK(std::string(string) == "wxyz"s);
            CHECK(std::wstring(string) == L"wxyz"s);
            CHECK(string.toAnsiString() == "wxyz"s);
            CHECK(string.toWideString() == L"wxyz"s);
            CHECK(string.toUtf8() == sf::U8String{'w', 'x', 'y', 'z'});
            CHECK(string.toUtf16() == u"wxyz"s);
            CHECK(string.toUtf32() == U"wxyz"s);
            CHECK(string.getSize() == 4);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }
        {
            constexpr std::array<std::uint8_t, 4> characters{0xF0, 0x9F, 0x90, 0x8C};
            const sf::String                      string = sf::String::fromUtf8(characters.begin(), characters.end());
            CHECK(std::string(string) == "\0"s);
            CHECK(std::wstring(string) == select(L""s, L"🐌"s));
            CHECK(string.toAnsiString() == "\0"s);
            CHECK(string.toWideString() == select(L""s, L"🐌"s));
            CHECK(string.toUtf8() == sf::U8String{0xF0, 0x9F, 0x90, 0x8C});
            CHECK(string.toUtf16() == u"🐌"s);
            CHECK(string.toUtf32() == U"🐌"s);
            CHECK(string.getSize() == 1);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }
    }

    SECTION("fromUtf16()")
    {
        {
            constexpr std::u16string_view characters = u"SFML!"sv;
            const sf::String              string     = sf::String::fromUtf16(characters.begin(), characters.end());
            CHECK(std::string(string) == "SFML!"s);
            CHECK(std::wstring(string) == L"SFML!"s);
            CHECK(string.toAnsiString() == "SFML!"s);
            CHECK(string.toWideString() == L"SFML!"s);
            CHECK(string.toUtf8() == sf::U8String{'S', 'F', 'M', 'L', '!'});
            CHECK(string.toUtf16() == u"SFML!"s);
            CHECK(string.toUtf32() == U"SFML!"s);
            CHECK(string.getSize() == 5);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }
        {
            constexpr std::u16string_view characters = u"piñata"sv;
            const sf::String              string     = sf::String::fromUtf16(characters.begin(), characters.end());
            CHECK(std::string(string) == select("pi\xF1"
                                                "ata"s,
                                                "pi\0ata"s));
            CHECK(std::wstring(string) == L"piñata"s);
            CHECK(string.toAnsiString() == select("pi\xF1"
                                                  "ata"s,
                                                  "pi\0ata"s));
            CHECK(string.toWideString() == L"piñata"s);
            CHECK(string.toUtf8() == sf::U8String{'p', 'i', 0xC3, 0xB1, 'a', 't', 'a'});
            CHECK(string.toUtf16() == u"piñata"s);
            CHECK(string.toUtf32() == U"piñata"s);
            CHECK(string.getSize() == 6);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }
        {
            constexpr std::u16string_view characters = u"달팽이"sv;
            const sf::String              string     = sf::String::fromUtf16(characters.begin(), characters.end());
            CHECK(std::string(string) == "\0\0\0"s);
            CHECK(std::wstring(string) == L"달팽이"s);
            CHECK(string.toAnsiString() == "\0\0\0"s);
            CHECK(string.toWideString() == L"달팽이"s);
            CHECK(string.toUtf8() == sf::U8String{0xEB, 0x8B, 0xAC, 0xED, 0x8C, 0xBD, 0xEC, 0x9D, 0xB4});
            CHECK(string.toUtf16() == u"달팽이"s);
            CHECK(string.toUtf32() == U"달팽이"s);
            CHECK(string.getSize() == 3);
            CHECK(!string.isEmpty());
            CHECK(string.getData() != nullptr);
        }
    }

    SECTION("fromUtf32()")
    {
        constexpr std::u32string_view characters = U"👍+👎=🤷"sv;
        const sf::String              string     = sf::String::fromUtf32(characters.begin(), characters.end());
        CHECK(std::string(string) == "\0+\0=\0"s);
        CHECK(std::wstring(string) == select(L"+="s, L"👍+👎=🤷"s));
        CHECK(string.toAnsiString() == "\0+\0=\0"s);
        CHECK(string.toWideString() == select(L"+="s, L"👍+👎=🤷"s));
        CHECK(string.toUtf8() ==
              sf::U8String{0xF0, 0x9F, 0x91, 0x8D, '+', 0xF0, 0x9F, 0x91, 0x8E, '=', 0xF0, 0x9F, 0xA4, 0xB7});
        CHECK(string.toUtf16() == u"👍+👎=🤷"s);
        CHECK(string.toUtf32() == U"👍+👎=🤷"s);
        CHECK(string.getSize() == 5);
        CHECK(!string.isEmpty());
        CHECK(string.getData() != nullptr);
    }

    SECTION("clear()")
    {
        sf::String string("you'll never guess what happens when you call clear()");
        string.clear();
        CHECK(string.isEmpty());
        CHECK(string.getSize() == 0);
    }

    SECTION("erase()")
    {
        sf::String string("what if i want a shorter string?");
        string.erase(0, 8);
        string.erase(string.getSize() - 1, 1);
        CHECK(string == "i want a shorter string");
        CHECK(string.getSize() == 23);
    }

    SECTION("insert()")
    {
        sf::String string("please insert text");
        string.insert(7, "don't ");
        CHECK(string == "please don't insert text");
        CHECK(string.getSize() == 24);
    }

    SECTION("find()")
    {
        const sf::String string("a little bit of this and a little bit of that");
        CHECK(string.find("a little bit") == 0);
        CHECK(string.find("a little bit", 15) == 25);
        CHECK(string.find("a little bit", 1'000) == sf::String::InvalidPos);
        CHECK(string.find("no way you find this") == sf::String::InvalidPos);
    }

    SECTION("replace()")
    {
        sf::String string("sfml is the worst");
        string.replace(12, 5, "best!");
        CHECK(string == "sfml is the best!");
        string.replace("the", "THE");
        CHECK(string == "sfml is THE best!");
    }

    SECTION("substring()")
    {
        const sf::String string("let's get some substrings");
        CHECK(string.substring(0) == "let's get some substrings");
        CHECK(string.substring(10) == "some substrings");
        CHECK(string.substring(10, 4) == "some");
        CHECK_THROWS_AS((void)string.substring(1'000), std::out_of_range);
        CHECK_THROWS_AS((void)string.substring(420, 69), std::out_of_range);
    }

    SECTION("begin() and end() const")
    {
        const sf::String string("let's test the const iterators");
        CHECK(*string.begin() == 'l');
        CHECK(*(string.end() - 1) == 's');
        for (const auto character : string)
            CHECK(character != 0);
    }

    SECTION("begin() and end()")
    {
        sf::String string("let's test the iterators");
        CHECK(*string.begin() == 'l');
        CHECK(*(string.end() - 1) == 's');
        for (auto& character : string)
            character = 'x';
        CHECK(string == "xxxxxxxxxxxxxxxxxxxxxxxx");
    }

    SECTION("Operators")
    {
        SECTION("operator+=")
        {
            sf::String string;
            string += sf::String("xyz");
            CHECK(string.toAnsiString() == "xyz"s);
        }

        SECTION("operator[] const")
        {
            const sf::String string("the quick brown fox");
            CHECK(string[0] == 't');
            CHECK(string[10] == 'b');
        }

        SECTION("operator[]")
        {
            sf::String string("the quick brown fox");
            CHECK(string[0] == 't');
            string[1] = 'x';
            CHECK(string[1] == 'x');
        }

        SECTION("operator==")
        {
            CHECK(sf::String() == sf::String());
            CHECK_FALSE(sf::String() == sf::String(' '));
        }

        SECTION("operator!=")
        {
            CHECK(sf::String() != sf::String(' '));
            CHECK_FALSE(sf::String() != sf::String());
        }

        SECTION("operator<")
        {
            CHECK(sf::String('a') < sf::String('b'));
            CHECK_FALSE(sf::String() < sf::String());
        }

        SECTION("operator>")
        {
            CHECK(sf::String('b') > sf::String('a'));
            CHECK_FALSE(sf::String() > sf::String());
        }

        SECTION("operator<=")
        {
            CHECK(sf::String() <= sf::String());
            CHECK(sf::String('a') <= sf::String('b'));
        }

        SECTION("operator>=")
        {
            CHECK(sf::String() >= sf::String());
            CHECK(sf::String('b') >= sf::String('a'));
        }

        SECTION("operator+")
        {
            CHECK(sf::String() + sf::String() == sf::String());
            CHECK(sf::String("abc") + sf::String("def") == sf::String("abcdef"));
        }
    }
}

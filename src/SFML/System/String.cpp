////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/String.hpp>
#include <SFML/System/Utf.hpp>

#include <iterator>
#include <utility>

#include <cassert>
#include <cstring>
#include <cwchar>


namespace sf
{
////////////////////////////////////////////////////////////
void U8StringCharTraits::assign(char_type& c1, char_type c2) noexcept
{
    c1 = c2;
}


////////////////////////////////////////////////////////////
U8StringCharTraits::char_type* U8StringCharTraits::assign(char_type* s, std::size_t n, char_type c)
{
    return reinterpret_cast<char_type*>(std::char_traits<char>::assign(reinterpret_cast<char*>(s), n, static_cast<char>(c)));
}


////////////////////////////////////////////////////////////
bool U8StringCharTraits::eq(char_type c1, char_type c2) noexcept
{
    return c1 == c2;
}


////////////////////////////////////////////////////////////
bool U8StringCharTraits::lt(char_type c1, char_type c2) noexcept
{
    return c1 < c2;
}


////////////////////////////////////////////////////////////
U8StringCharTraits::char_type* U8StringCharTraits::move(char_type* s1, const char_type* s2, std::size_t n)
{
    std::memmove(s1, s2, n);
    return s1;
}


////////////////////////////////////////////////////////////
U8StringCharTraits::char_type* U8StringCharTraits::copy(char_type* s1, const char_type* s2, std::size_t n)
{
    std::memcpy(s1, s2, n);
    return s1;
}


////////////////////////////////////////////////////////////
int U8StringCharTraits::compare(const char_type* s1, const char_type* s2, std::size_t n)
{
    return std::memcmp(s1, s2, n);
}


////////////////////////////////////////////////////////////
std::size_t U8StringCharTraits::length(const char_type* s)
{
    return std::strlen(reinterpret_cast<const char*>(s));
}


////////////////////////////////////////////////////////////
const U8StringCharTraits::char_type* U8StringCharTraits::find(const char_type* s, std::size_t n, const char_type& c)
{
    return reinterpret_cast<const char_type*>(
        std::char_traits<char>::find(reinterpret_cast<const char*>(s), n, static_cast<char>(c)));
}


////////////////////////////////////////////////////////////
U8StringCharTraits::char_type U8StringCharTraits::to_char_type(int_type i) noexcept
{
    return static_cast<U8StringCharTraits::char_type>(std::char_traits<char>::to_char_type(i));
}


////////////////////////////////////////////////////////////
U8StringCharTraits::int_type U8StringCharTraits::to_int_type(char_type c) noexcept
{
    return std::char_traits<char>::to_int_type(static_cast<char>(c));
}


////////////////////////////////////////////////////////////
bool U8StringCharTraits::eq_int_type(int_type i1, int_type i2) noexcept
{
    return i1 == i2;
}


////////////////////////////////////////////////////////////
U8StringCharTraits::int_type U8StringCharTraits::eof() noexcept
{
    return std::char_traits<char>::eof();
}


////////////////////////////////////////////////////////////
U8StringCharTraits::int_type U8StringCharTraits::not_eof(int_type i) noexcept
{
    return std::char_traits<char>::not_eof(i);
}


////////////////////////////////////////////////////////////
String::String(char ansiChar, const std::locale& locale) : m_string(1, Utf32::decodeAnsi(ansiChar, locale))
{
}


////////////////////////////////////////////////////////////
String::String(wchar_t wideChar) : m_string(1, Utf32::decodeWide(wideChar))
{
}


////////////////////////////////////////////////////////////
String::String(char32_t utf32Char) : m_string(1, utf32Char)
{
}


////////////////////////////////////////////////////////////
String::String(const char* ansiString, const std::locale& locale)
{
    if (ansiString)
    {
        const std::size_t length = std::strlen(ansiString);
        if (length > 0)
        {
            m_string.reserve(length + 1);
            Utf32::fromAnsi(ansiString, ansiString + length, std::back_inserter(m_string), locale);
        }
    }
}


////////////////////////////////////////////////////////////
String::String(const std::string& ansiString, const std::locale& locale)
{
    m_string.reserve(ansiString.length() + 1);
    Utf32::fromAnsi(ansiString.begin(), ansiString.end(), std::back_inserter(m_string), locale);
}


////////////////////////////////////////////////////////////
String::String(const wchar_t* wideString)
{
    if (wideString)
    {
        const std::size_t length = std::wcslen(wideString);
        if (length > 0)
        {
            m_string.reserve(length + 1);
            Utf32::fromWide(wideString, wideString + length, std::back_inserter(m_string));
        }
    }
}


////////////////////////////////////////////////////////////
String::String(const std::wstring& wideString)
{
    m_string.reserve(wideString.length() + 1);
    Utf32::fromWide(wideString.begin(), wideString.end(), std::back_inserter(m_string));
}


////////////////////////////////////////////////////////////
String::String(const char32_t* utf32String) : m_string(utf32String ? utf32String : U"")
{
}


////////////////////////////////////////////////////////////
String::String(std::u32string utf32String) : m_string(std::move(utf32String))
{
}


////////////////////////////////////////////////////////////
String::operator std::string() const
{
    return toAnsiString();
}


////////////////////////////////////////////////////////////
String::operator std::wstring() const
{
    return toWideString();
}


////////////////////////////////////////////////////////////
std::string String::toAnsiString(const std::locale& locale) const
{
    // Prepare the output string
    std::string output;
    output.reserve(m_string.length() + 1);

    // Convert
    Utf32::toAnsi(m_string.begin(), m_string.end(), std::back_inserter(output), 0, locale);

    return output;
}


////////////////////////////////////////////////////////////
std::wstring String::toWideString() const
{
    // Prepare the output string
    std::wstring output;
    output.reserve(m_string.length() + 1);

    // Convert
    Utf32::toWide(m_string.begin(), m_string.end(), std::back_inserter(output), 0);

    return output;
}


////////////////////////////////////////////////////////////
U8String String::toUtf8() const
{
    // Prepare the output string
    U8String output;
    output.reserve(m_string.length());

    // Convert
    Utf32::toUtf8(m_string.begin(), m_string.end(), std::back_inserter(output));

    return output;
}


////////////////////////////////////////////////////////////
std::u16string String::toUtf16() const
{
    // Prepare the output string
    std::u16string output;
    output.reserve(m_string.length());

    // Convert
    Utf32::toUtf16(m_string.begin(), m_string.end(), std::back_inserter(output));

    return output;
}


////////////////////////////////////////////////////////////
std::u32string String::toUtf32() const
{
    return m_string;
}


////////////////////////////////////////////////////////////
String& String::operator+=(const String& right)
{
    m_string += right.m_string;
    return *this;
}


////////////////////////////////////////////////////////////
char32_t String::operator[](std::size_t index) const
{
    assert(index < m_string.size() && "Index is out of bounds");
    return m_string[index];
}


////////////////////////////////////////////////////////////
char32_t& String::operator[](std::size_t index)
{
    assert(index < m_string.size() && "Index is out of bounds");
    return m_string[index];
}


////////////////////////////////////////////////////////////
void String::clear()
{
    m_string.clear();
}


////////////////////////////////////////////////////////////
std::size_t String::getSize() const
{
    return m_string.size();
}


////////////////////////////////////////////////////////////
bool String::isEmpty() const
{
    return m_string.empty();
}


////////////////////////////////////////////////////////////
void String::erase(std::size_t position, std::size_t count)
{
    m_string.erase(position, count);
}


////////////////////////////////////////////////////////////
void String::insert(std::size_t position, const String& str)
{
    m_string.insert(position, str.m_string);
}


////////////////////////////////////////////////////////////
std::size_t String::find(const String& str, std::size_t start) const
{
    return m_string.find(str.m_string, start);
}


////////////////////////////////////////////////////////////
void String::replace(std::size_t position, std::size_t length, const String& replaceWith)
{
    m_string.replace(position, length, replaceWith.m_string);
}


////////////////////////////////////////////////////////////
void String::replace(const String& searchFor, const String& replaceWith)
{
    const std::size_t step = replaceWith.getSize();
    const std::size_t len  = searchFor.getSize();
    std::size_t       pos  = find(searchFor);

    // Replace each occurrence of search
    while (pos != InvalidPos)
    {
        replace(pos, len, replaceWith);
        pos = find(searchFor, pos + step);
    }
}


////////////////////////////////////////////////////////////
String String::substring(std::size_t position, std::size_t length) const
{
    return m_string.substr(position, length);
}


////////////////////////////////////////////////////////////
const char32_t* String::getData() const
{
    return m_string.c_str();
}


////////////////////////////////////////////////////////////
String::Iterator String::begin()
{
    return m_string.begin();
}


////////////////////////////////////////////////////////////
String::ConstIterator String::begin() const
{
    return m_string.begin();
}


////////////////////////////////////////////////////////////
String::Iterator String::end()
{
    return m_string.end();
}


////////////////////////////////////////////////////////////
String::ConstIterator String::end() const
{
    return m_string.end();
}


////////////////////////////////////////////////////////////
bool operator==(const String& left, const String& right)
{
    return left.m_string == right.m_string;
}


////////////////////////////////////////////////////////////
bool operator!=(const String& left, const String& right)
{
    return !(left == right);
}


////////////////////////////////////////////////////////////
bool operator<(const String& left, const String& right)
{
    return left.m_string < right.m_string;
}


////////////////////////////////////////////////////////////
bool operator>(const String& left, const String& right)
{
    return right < left;
}


////////////////////////////////////////////////////////////
bool operator<=(const String& left, const String& right)
{
    return !(right < left);
}


////////////////////////////////////////////////////////////
bool operator>=(const String& left, const String& right)
{
    return !(left < right);
}


////////////////////////////////////////////////////////////
String operator+(const String& left, const String& right)
{
    String string = left;
    string += right;

    return string;
}

} // namespace sf

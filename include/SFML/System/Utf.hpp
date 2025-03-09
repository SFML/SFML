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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>

#include <array>
#include <locale>

#include <cstdint>
#include <cstdlib>


namespace sf
{
namespace priv
{
template <class InputIt, class OutputIt>
OutputIt copy(InputIt first, InputIt last, OutputIt dFirst);
}

template <unsigned int N>
class Utf;

////////////////////////////////////////////////////////////
/// \brief Specialization of the Utf template for UTF-8
///
////////////////////////////////////////////////////////////
template <>
class Utf<8>
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Decode a single UTF-8 character
    ///
    /// Decoding a character means finding its unique 32-bits
    /// code (called the codepoint) in the Unicode standard.
    ///
    /// \param begin       Iterator pointing to the beginning of the input sequence
    /// \param end         Iterator pointing to the end of the input sequence
    /// \param output      Codepoint of the decoded UTF-8 character
    /// \param replacement Replacement character to use in case the UTF-8 sequence is invalid
    ///
    /// \return Iterator pointing to one past the last read element of the input sequence
    ///
    ////////////////////////////////////////////////////////////
    template <typename In>
    static In decode(In begin, In end, char32_t& output, char32_t replacement = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Encode a single UTF-8 character
    ///
    /// Encoding a character means converting a unique 32-bits
    /// code (called the codepoint) in the target encoding, UTF-8.
    ///
    /// \param input       Codepoint to encode as UTF-8
    /// \param output      Iterator pointing to the beginning of the output sequence
    /// \param replacement Replacement for characters not convertible to UTF-8 (use 0 to skip them)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename Out>
    static Out encode(char32_t input, Out output, std::uint8_t replacement = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Advance to the next UTF-8 character
    ///
    /// This function is necessary for multi-elements encodings, as
    /// a single character may use more than 1 storage element.
    ///
    /// \param begin Iterator pointing to the beginning of the input sequence
    /// \param end   Iterator pointing to the end of the input sequence
    ///
    /// \return Iterator pointing to one past the last read element of the input sequence
    ///
    ////////////////////////////////////////////////////////////
    template <typename In>
    static In next(In begin, In end);

    ////////////////////////////////////////////////////////////
    /// \brief Count the number of characters of a UTF-8 sequence
    ///
    /// This function is necessary for multi-elements encodings, as
    /// a single character may use more than 1 storage element, thus the
    /// total size can be different from (begin - end).
    ///
    /// \param begin Iterator pointing to the beginning of the input sequence
    /// \param end   Iterator pointing to the end of the input sequence
    ///
    /// \return Iterator pointing to one past the last read element of the input sequence
    ///
    ////////////////////////////////////////////////////////////
    template <typename In>
    static std::size_t count(In begin, In end);

    ////////////////////////////////////////////////////////////
    /// \brief Convert an ANSI characters range to UTF-8
    ///
    /// The current global locale will be used by default, unless you
    /// pass a custom one in the `locale` parameter.
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    /// \param locale Locale to use for conversion
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out fromAnsi(In begin, In end, Out output, const std::locale& locale = {});

    ////////////////////////////////////////////////////////////
    /// \brief Convert a wide characters range to UTF-8
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out fromWide(In begin, In end, Out output);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a latin-1 (ISO-5589-1) characters range to UTF-8
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out fromLatin1(In begin, In end, Out output);

    ////////////////////////////////////////////////////////////
    /// \brief Convert an UTF-8 characters range to ANSI characters
    ///
    /// The current global locale will be used by default, unless you
    /// pass a custom one in the `locale` parameter.
    ///
    /// \param begin       Iterator pointing to the beginning of the input sequence
    /// \param end         Iterator pointing to the end of the input sequence
    /// \param output      Iterator pointing to the beginning of the output sequence
    /// \param replacement Replacement for characters not convertible to ANSI (use 0 to skip them)
    /// \param locale      Locale to use for conversion
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toAnsi(In begin, In end, Out output, char replacement = 0, const std::locale& locale = {});

    ////////////////////////////////////////////////////////////
    /// \brief Convert an UTF-8 characters range to wide characters
    ///
    /// \param begin       Iterator pointing to the beginning of the input sequence
    /// \param end         Iterator pointing to the end of the input sequence
    /// \param output      Iterator pointing to the beginning of the output sequence
    /// \param replacement Replacement for characters not convertible to wide (use 0 to skip them)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toWide(In begin, In end, Out output, wchar_t replacement = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Convert an UTF-8 characters range to latin-1 (ISO-5589-1) characters
    ///
    /// \param begin       Iterator pointing to the beginning of the input sequence
    /// \param end         Iterator pointing to the end of the input sequence
    /// \param output      Iterator pointing to the beginning of the output sequence
    /// \param replacement Replacement for characters not convertible to wide (use 0 to skip them)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toLatin1(In begin, In end, Out output, char replacement = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a UTF-8 characters range to UTF-8
    ///
    /// This functions does nothing more than a direct copy;
    /// it is defined only to provide the same interface as other
    /// specializations of the `sf::Utf<>` template, and allow
    /// generic code to be written on top of it.
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toUtf8(In begin, In end, Out output);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a UTF-8 characters range to UTF-16
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toUtf16(In begin, In end, Out output);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a UTF-8 characters range to UTF-32
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toUtf32(In begin, In end, Out output);
};

////////////////////////////////////////////////////////////
/// \brief Specialization of the Utf template for UTF-16
///
////////////////////////////////////////////////////////////
template <>
class Utf<16>
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Decode a single UTF-16 character
    ///
    /// Decoding a character means finding its unique 32-bits
    /// code (called the codepoint) in the Unicode standard.
    ///
    /// \param begin       Iterator pointing to the beginning of the input sequence
    /// \param end         Iterator pointing to the end of the input sequence
    /// \param output      Codepoint of the decoded UTF-16 character
    /// \param replacement Replacement character to use in case the UTF-8 sequence is invalid
    ///
    /// \return Iterator pointing to one past the last read element of the input sequence
    ///
    ////////////////////////////////////////////////////////////
    template <typename In>
    static In decode(In begin, In end, char32_t& output, char32_t replacement = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Encode a single UTF-16 character
    ///
    /// Encoding a character means converting a unique 32-bits
    /// code (called the codepoint) in the target encoding, UTF-16.
    ///
    /// \param input       Codepoint to encode as UTF-16
    /// \param output      Iterator pointing to the beginning of the output sequence
    /// \param replacement Replacement for characters not convertible to UTF-16 (use 0 to skip them)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename Out>
    static Out encode(char32_t input, Out output, char16_t replacement = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Advance to the next UTF-16 character
    ///
    /// This function is necessary for multi-elements encodings, as
    /// a single character may use more than 1 storage element.
    ///
    /// \param begin Iterator pointing to the beginning of the input sequence
    /// \param end   Iterator pointing to the end of the input sequence
    ///
    /// \return Iterator pointing to one past the last read element of the input sequence
    ///
    ////////////////////////////////////////////////////////////
    template <typename In>
    static In next(In begin, In end);

    ////////////////////////////////////////////////////////////
    /// \brief Count the number of characters of a UTF-16 sequence
    ///
    /// This function is necessary for multi-elements encodings, as
    /// a single character may use more than 1 storage element, thus the
    /// total size can be different from (begin - end).
    ///
    /// \param begin Iterator pointing to the beginning of the input sequence
    /// \param end   Iterator pointing to the end of the input sequence
    ///
    /// \return Iterator pointing to one past the last read element of the input sequence
    ///
    ////////////////////////////////////////////////////////////
    template <typename In>
    static std::size_t count(In begin, In end);

    ////////////////////////////////////////////////////////////
    /// \brief Convert an ANSI characters range to UTF-16
    ///
    /// The current global locale will be used by default, unless you
    /// pass a custom one in the `locale` parameter.
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    /// \param locale Locale to use for conversion
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out fromAnsi(In begin, In end, Out output, const std::locale& locale = {});

    ////////////////////////////////////////////////////////////
    /// \brief Convert a wide characters range to UTF-16
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out fromWide(In begin, In end, Out output);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a latin-1 (ISO-5589-1) characters range to UTF-16
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out fromLatin1(In begin, In end, Out output);

    ////////////////////////////////////////////////////////////
    /// \brief Convert an UTF-16 characters range to ANSI characters
    ///
    /// The current global locale will be used by default, unless you
    /// pass a custom one in the `locale` parameter.
    ///
    /// \param begin       Iterator pointing to the beginning of the input sequence
    /// \param end         Iterator pointing to the end of the input sequence
    /// \param output      Iterator pointing to the beginning of the output sequence
    /// \param replacement Replacement for characters not convertible to ANSI (use 0 to skip them)
    /// \param locale      Locale to use for conversion
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toAnsi(In begin, In end, Out output, char replacement = 0, const std::locale& locale = {});

    ////////////////////////////////////////////////////////////
    /// \brief Convert an UTF-16 characters range to wide characters
    ///
    /// \param begin       Iterator pointing to the beginning of the input sequence
    /// \param end         Iterator pointing to the end of the input sequence
    /// \param output      Iterator pointing to the beginning of the output sequence
    /// \param replacement Replacement for characters not convertible to wide (use 0 to skip them)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toWide(In begin, In end, Out output, wchar_t replacement = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Convert an UTF-16 characters range to latin-1 (ISO-5589-1) characters
    ///
    /// \param begin       Iterator pointing to the beginning of the input sequence
    /// \param end         Iterator pointing to the end of the input sequence
    /// \param output      Iterator pointing to the beginning of the output sequence
    /// \param replacement Replacement for characters not convertible to wide (use 0 to skip them)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toLatin1(In begin, In end, Out output, char replacement = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a UTF-16 characters range to UTF-8
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toUtf8(In begin, In end, Out output);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a UTF-16 characters range to UTF-16
    ///
    /// This functions does nothing more than a direct copy;
    /// it is defined only to provide the same interface as other
    /// specializations of the `sf::Utf<>` template, and allow
    /// generic code to be written on top of it.
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toUtf16(In begin, In end, Out output);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a UTF-16 characters range to UTF-32
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toUtf32(In begin, In end, Out output);
};

////////////////////////////////////////////////////////////
/// \brief Specialization of the Utf template for UTF-32
///
////////////////////////////////////////////////////////////
template <>
class Utf<32>
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Decode a single UTF-32 character
    ///
    /// Decoding a character means finding its unique 32-bits
    /// code (called the codepoint) in the Unicode standard.
    /// For UTF-32, the character value is the same as the codepoint.
    ///
    /// \param begin       Iterator pointing to the beginning of the input sequence
    /// \param end         Iterator pointing to the end of the input sequence
    /// \param output      Codepoint of the decoded UTF-32 character
    /// \param replacement Replacement character to use in case the UTF-8 sequence is invalid
    ///
    /// \return Iterator pointing to one past the last read element of the input sequence
    ///
    ////////////////////////////////////////////////////////////
    template <typename In>
    static In decode(In begin, In end, char32_t& output, char32_t replacement = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Encode a single UTF-32 character
    ///
    /// Encoding a character means converting a unique 32-bits
    /// code (called the codepoint) in the target encoding, UTF-32.
    /// For UTF-32, the codepoint is the same as the character value.
    ///
    /// \param input       Codepoint to encode as UTF-32
    /// \param output      Iterator pointing to the beginning of the output sequence
    /// \param replacement Replacement for characters not convertible to UTF-32 (use 0 to skip them)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename Out>
    static Out encode(char32_t input, Out output, char32_t replacement = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Advance to the next UTF-32 character
    ///
    /// This function is trivial for UTF-32, which can store
    /// every character in a single storage element.
    ///
    /// \param begin Iterator pointing to the beginning of the input sequence
    /// \param end   Iterator pointing to the end of the input sequence
    ///
    /// \return Iterator pointing to one past the last read element of the input sequence
    ///
    ////////////////////////////////////////////////////////////
    template <typename In>
    static In next(In begin, In end);

    ////////////////////////////////////////////////////////////
    /// \brief Count the number of characters of a UTF-32 sequence
    ///
    /// This function is trivial for UTF-32, which can store
    /// every character in a single storage element.
    ///
    /// \param begin Iterator pointing to the beginning of the input sequence
    /// \param end   Iterator pointing to the end of the input sequence
    ///
    /// \return Iterator pointing to one past the last read element of the input sequence
    ///
    ////////////////////////////////////////////////////////////
    template <typename In>
    static std::size_t count(In begin, In end);

    ////////////////////////////////////////////////////////////
    /// \brief Convert an ANSI characters range to UTF-32
    ///
    /// The current global locale will be used by default, unless you
    /// pass a custom one in the `locale` parameter.
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    /// \param locale Locale to use for conversion
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out fromAnsi(In begin, In end, Out output, const std::locale& locale = {});

    ////////////////////////////////////////////////////////////
    /// \brief Convert a wide characters range to UTF-32
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out fromWide(In begin, In end, Out output);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a latin-1 (ISO-5589-1) characters range to UTF-32
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out fromLatin1(In begin, In end, Out output);

    ////////////////////////////////////////////////////////////
    /// \brief Convert an UTF-32 characters range to ANSI characters
    ///
    /// The current global locale will be used by default, unless you
    /// pass a custom one in the `locale` parameter.
    ///
    /// \param begin       Iterator pointing to the beginning of the input sequence
    /// \param end         Iterator pointing to the end of the input sequence
    /// \param output      Iterator pointing to the beginning of the output sequence
    /// \param replacement Replacement for characters not convertible to ANSI (use 0 to skip them)
    /// \param locale      Locale to use for conversion
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toAnsi(In begin, In end, Out output, char replacement = 0, const std::locale& locale = {});

    ////////////////////////////////////////////////////////////
    /// \brief Convert an UTF-32 characters range to wide characters
    ///
    /// \param begin       Iterator pointing to the beginning of the input sequence
    /// \param end         Iterator pointing to the end of the input sequence
    /// \param output      Iterator pointing to the beginning of the output sequence
    /// \param replacement Replacement for characters not convertible to wide (use 0 to skip them)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toWide(In begin, In end, Out output, wchar_t replacement = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Convert an UTF-16 characters range to latin-1 (ISO-5589-1) characters
    ///
    /// \param begin       Iterator pointing to the beginning of the input sequence
    /// \param end         Iterator pointing to the end of the input sequence
    /// \param output      Iterator pointing to the beginning of the output sequence
    /// \param replacement Replacement for characters not convertible to wide (use 0 to skip them)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toLatin1(In begin, In end, Out output, char replacement = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a UTF-32 characters range to UTF-8
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toUtf8(In begin, In end, Out output);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a UTF-32 characters range to UTF-16
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toUtf16(In begin, In end, Out output);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a UTF-32 characters range to UTF-32
    ///
    /// This functions does nothing more than a direct copy;
    /// it is defined only to provide the same interface as other
    /// specializations of the `sf::Utf<>` template, and allow
    /// generic code to be written on top of it.
    ///
    /// \param begin  Iterator pointing to the beginning of the input sequence
    /// \param end    Iterator pointing to the end of the input sequence
    /// \param output Iterator pointing to the beginning of the output sequence
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename In, typename Out>
    static Out toUtf32(In begin, In end, Out output);

    ////////////////////////////////////////////////////////////
    /// \brief Decode a single ANSI character to UTF-32
    ///
    /// This function does not exist in other specializations
    /// of `sf::Utf<>`, it is defined for convenience (it is used by
    /// several other conversion functions).
    ///
    /// \param input  Input ANSI character
    /// \param locale Locale to use for conversion
    ///
    /// \return Converted character
    ///
    ////////////////////////////////////////////////////////////
    template <typename In>
    static char32_t decodeAnsi(In input, const std::locale& locale = {});

    ////////////////////////////////////////////////////////////
    /// \brief Decode a single wide character to UTF-32
    ///
    /// This function does not exist in other specializations
    /// of `sf::Utf<>`, it is defined for convenience (it is used by
    /// several other conversion functions).
    ///
    /// \param input Input wide character
    ///
    /// \return Converted character
    ///
    ////////////////////////////////////////////////////////////
    template <typename In>
    static char32_t decodeWide(In input);

    ////////////////////////////////////////////////////////////
    /// \brief Encode a single UTF-32 character to ANSI
    ///
    /// This function does not exist in other specializations
    /// of `sf::Utf<>`, it is defined for convenience (it is used by
    /// several other conversion functions).
    ///
    /// \param codepoint   Iterator pointing to the beginning of the input sequence
    /// \param output      Iterator pointing to the beginning of the output sequence
    /// \param replacement Replacement if the input character is not convertible to ANSI (use 0 to skip it)
    /// \param locale      Locale to use for conversion
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename Out>
    static Out encodeAnsi(char32_t codepoint, Out output, char replacement = 0, const std::locale& locale = {});

    ////////////////////////////////////////////////////////////
    /// \brief Encode a single UTF-32 character to wide
    ///
    /// This function does not exist in other specializations
    /// of `sf::Utf<>`, it is defined for convenience (it is used by
    /// several other conversion functions).
    ///
    /// \param codepoint   Iterator pointing to the beginning of the input sequence
    /// \param output      Iterator pointing to the beginning of the output sequence
    /// \param replacement Replacement if the input character is not convertible to wide (use 0 to skip it)
    ///
    /// \return Iterator to the end of the output sequence which has been written
    ///
    ////////////////////////////////////////////////////////////
    template <typename Out>
    static Out encodeWide(char32_t codepoint, Out output, wchar_t replacement = 0);
};

// Make type aliases to get rid of the template syntax
using Utf8  = Utf<8>;
using Utf16 = Utf<16>;
using Utf32 = Utf<32>;

} // namespace sf

#include <SFML/System/Utf.inl>


////////////////////////////////////////////////////////////
/// \class sf::Utf
/// \ingroup system
///
/// Utility class providing generic functions for UTF conversions.
///
/// `sf::Utf` is a low-level, generic interface for counting, iterating,
/// encoding and decoding Unicode characters and strings. It is able
/// to handle ANSI, wide, latin-1, UTF-8, UTF-16 and UTF-32 encodings.
///
/// `sf::Utf<X>` functions are all static, these classes are not meant to
/// be instantiated. All the functions are template, so that you
/// can use any character / string type for a given encoding.
///
/// It has 3 specializations:
/// \li `sf::Utf<8>` (with `sf::Utf8` type alias)
/// \li `sf::Utf<16>` (with `sf::Utf16` type alias)
/// \li `sf::Utf<32>` (with `sf::Utf32` type alias)
///
////////////////////////////////////////////////////////////

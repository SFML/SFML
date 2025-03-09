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
#include <SFML/System/Export.hpp>

#include <SFML/System/Utf.hpp>

#include <locale>
#include <string>

#include <cstddef>
#include <cstdint>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Character traits for `std::uint8_t`
///
////////////////////////////////////////////////////////////
struct SFML_SYSTEM_API U8StringCharTraits
{
    // NOLINTBEGIN(readability-identifier-naming)
    using char_type  = std::uint8_t;
    using int_type   = std::char_traits<char>::int_type;
    using off_type   = std::char_traits<char>::off_type;
    using pos_type   = std::char_traits<char>::pos_type;
    using state_type = std::char_traits<char>::state_type;

    static void             assign(char_type& c1, char_type c2) noexcept;
    static char_type*       assign(char_type* s, std::size_t n, char_type c);
    static bool             eq(char_type c1, char_type c2) noexcept;
    static bool             lt(char_type c1, char_type c2) noexcept;
    static char_type*       move(char_type* s1, const char_type* s2, std::size_t n);
    static char_type*       copy(char_type* s1, const char_type* s2, std::size_t n);
    static int              compare(const char_type* s1, const char_type* s2, std::size_t n);
    static std::size_t      length(const char_type* s);
    static const char_type* find(const char_type* s, std::size_t n, const char_type& c);
    static char_type        to_char_type(int_type i) noexcept;
    static int_type         to_int_type(char_type c) noexcept;
    static bool             eq_int_type(int_type i1, int_type i2) noexcept;
    static int_type         eof() noexcept;
    static int_type         not_eof(int_type i) noexcept;
    // NOLINTEND(readability-identifier-naming)
};

////////////////////////////////////////////////////////////
/// \brief Portable replacement for `std::basic_string<std::uint8_t>`
///
/// While all major C++ implementations happen to define this
/// as of early 2024, this specialization is not strictly speaking
/// standard C++. Thus we can't depend on its continued existence.
///
////////////////////////////////////////////////////////////
using U8String = std::basic_string<std::uint8_t, U8StringCharTraits>;

////////////////////////////////////////////////////////////
/// \brief Utility string class that automatically handles
///        conversions between types and encodings
///
////////////////////////////////////////////////////////////
class SFML_SYSTEM_API String
{
public:
    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    using Iterator      = std::u32string::iterator;       //!< Iterator type
    using ConstIterator = std::u32string::const_iterator; //!< Read-only iterator type

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    // NOLINTBEGIN(readability-identifier-naming)
    /// Represents an invalid position in the string
    static inline const std::size_t InvalidPos{std::u32string::npos};
    // NOLINTEND(readability-identifier-naming)

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor creates an empty string.
    ///
    ////////////////////////////////////////////////////////////
    String() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Deleted `std::nullptr_t` constructor
    ///
    /// Disallow construction from `nullptr` literal
    ///
    ////////////////////////////////////////////////////////////
    String(std::nullptr_t, const std::locale& = {}) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Construct from a single ANSI character and a locale
    ///
    /// The source character is converted to UTF-32 according
    /// to the given locale.
    ///
    /// \param ansiChar ANSI character to convert
    /// \param locale   Locale to use for conversion
    ///
    ////////////////////////////////////////////////////////////
    String(char ansiChar, const std::locale& locale = {});

    ////////////////////////////////////////////////////////////
    /// \brief Construct from single wide character
    ///
    /// \param wideChar Wide character to convert
    ///
    ////////////////////////////////////////////////////////////
    String(wchar_t wideChar);

    ////////////////////////////////////////////////////////////
    /// \brief Construct from single UTF-32 character
    ///
    /// \param utf32Char UTF-32 character to convert
    ///
    ////////////////////////////////////////////////////////////
    String(char32_t utf32Char);

    ////////////////////////////////////////////////////////////
    /// \brief Construct from a null-terminated C-style ANSI string and a locale
    ///
    /// The source string is converted to UTF-32 according
    /// to the given locale.
    ///
    /// \param ansiString ANSI string to convert
    /// \param locale     Locale to use for conversion
    ///
    ////////////////////////////////////////////////////////////
    String(const char* ansiString, const std::locale& locale = {});

    ////////////////////////////////////////////////////////////
    /// \brief Construct from an ANSI string and a locale
    ///
    /// The source string is converted to UTF-32 according
    /// to the given locale.
    ///
    /// \param ansiString ANSI string to convert
    /// \param locale     Locale to use for conversion
    ///
    ////////////////////////////////////////////////////////////
    String(const std::string& ansiString, const std::locale& locale = {});

    ////////////////////////////////////////////////////////////
    /// \brief Construct from null-terminated C-style wide string
    ///
    /// \param wideString Wide string to convert
    ///
    ////////////////////////////////////////////////////////////
    String(const wchar_t* wideString);

    ////////////////////////////////////////////////////////////
    /// \brief Construct from a wide string
    ///
    /// \param wideString Wide string to convert
    ///
    ////////////////////////////////////////////////////////////
    String(const std::wstring& wideString);

    ////////////////////////////////////////////////////////////
    /// \brief Construct from a null-terminated C-style UTF-32 string
    ///
    /// \param utf32String UTF-32 string to assign
    ///
    ////////////////////////////////////////////////////////////
    String(const char32_t* utf32String);

    ////////////////////////////////////////////////////////////
    /// \brief Construct from an UTF-32 string
    ///
    /// \param utf32String UTF-32 string to assign
    ///
    ////////////////////////////////////////////////////////////
    String(std::u32string utf32String);

    ////////////////////////////////////////////////////////////
    /// \brief Create a new `sf::String` from a UTF-8 encoded string
    ///
    /// \param begin Forward iterator to the beginning of the UTF-8 sequence
    /// \param end   Forward iterator to the end of the UTF-8 sequence
    ///
    /// \return A `sf::String` containing the source string
    ///
    /// \see `fromUtf16`, `fromUtf32`
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    [[nodiscard]] static String fromUtf8(T begin, T end);

    ////////////////////////////////////////////////////////////
    /// \brief Create a new `sf::String` from a UTF-16 encoded string
    ///
    /// \param begin Forward iterator to the beginning of the UTF-16 sequence
    /// \param end   Forward iterator to the end of the UTF-16 sequence
    ///
    /// \return A `sf::String` containing the source string
    ///
    /// \see `fromUtf8`, `fromUtf32`
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    [[nodiscard]] static String fromUtf16(T begin, T end);

    ////////////////////////////////////////////////////////////
    /// \brief Create a new `sf::String` from a UTF-32 encoded string
    ///
    /// This function is provided for consistency, it is equivalent to
    /// using the constructors that takes a `const char32_t*` or
    /// a `std::u32string`.
    ///
    /// \param begin Forward iterator to the beginning of the UTF-32 sequence
    /// \param end   Forward iterator to the end of the UTF-32 sequence
    ///
    /// \return A `sf::String` containing the source string
    ///
    /// \see `fromUtf8`, `fromUtf16`
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    [[nodiscard]] static String fromUtf32(T begin, T end);

    ////////////////////////////////////////////////////////////
    /// \brief Implicit conversion operator to `std::string` (ANSI string)
    ///
    /// The current global locale is used for conversion. If you
    /// want to explicitly specify a locale, see toAnsiString.
    /// Characters that do not fit in the target encoding are
    /// discarded from the returned string.
    /// This operator is defined for convenience, and is equivalent
    /// to calling `toAnsiString()`.
    ///
    /// \return Converted ANSI string
    ///
    /// \see `toAnsiString`, `operator std::wstring`
    ///
    ////////////////////////////////////////////////////////////
    operator std::string() const;

    ////////////////////////////////////////////////////////////
    /// \brief Implicit conversion operator to `std::wstring` (wide string)
    ///
    /// Characters that do not fit in the target encoding are
    /// discarded from the returned string.
    /// This operator is defined for convenience, and is equivalent
    /// to calling `toWideString()`.
    ///
    /// \return Converted wide string
    ///
    /// \see `toWideString`, `operator std::string`
    ///
    ////////////////////////////////////////////////////////////
    operator std::wstring() const;

    ////////////////////////////////////////////////////////////
    /// \brief Convert the Unicode string to an ANSI string
    ///
    /// The UTF-32 string is converted to an ANSI string in
    /// the encoding defined by `locale`.
    /// Characters that do not fit in the target encoding are
    /// discarded from the returned string.
    ///
    /// \param locale Locale to use for conversion
    ///
    /// \return Converted ANSI string
    ///
    /// \see `toWideString`, `operator std::string`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::string toAnsiString(const std::locale& locale = {}) const;

    ////////////////////////////////////////////////////////////
    /// \brief Convert the Unicode string to a wide string
    ///
    /// Characters that do not fit in the target encoding are
    /// discarded from the returned string.
    ///
    /// \return Converted wide string
    ///
    /// \see `toAnsiString`, `operator std::wstring`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::wstring toWideString() const;

    ////////////////////////////////////////////////////////////
    /// \brief Convert the Unicode string to a UTF-8 string
    ///
    /// \return Converted UTF-8 string
    ///
    /// \see `toUtf16`, `toUtf32`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] sf::U8String toUtf8() const;

    ////////////////////////////////////////////////////////////
    /// \brief Convert the Unicode string to a UTF-16 string
    ///
    /// \return Converted UTF-16 string
    ///
    /// \see `toUtf8`, `toUtf32`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::u16string toUtf16() const;

    ////////////////////////////////////////////////////////////
    /// \brief Convert the Unicode string to a UTF-32 string
    ///
    /// This function doesn't perform any conversion, since the
    /// string is already stored as UTF-32 internally.
    ///
    /// \return Converted UTF-32 string
    ///
    /// \see `toUtf8`, `toUtf16`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::u32string toUtf32() const;

    ////////////////////////////////////////////////////////////
    /// \brief Overload of `operator+=` to append an UTF-32 string
    ///
    /// \param right String to append
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    String& operator+=(const String& right);

    ////////////////////////////////////////////////////////////
    /// \brief Overload of `operator[]` to access a character by its position
    ///
    /// This function provides read-only access to characters.
    /// Note: the behavior is undefined if `index` is out of range.
    ///
    /// \param index Index of the character to get
    ///
    /// \return Character at position `index`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] char32_t operator[](std::size_t index) const;

    ////////////////////////////////////////////////////////////
    /// \brief Overload of `operator[]` to access a character by its position
    ///
    /// This function provides read and write access to characters.
    /// Note: the behavior is undefined if `index` is out of range.
    ///
    /// \param index Index of the character to get
    ///
    /// \return Reference to the character at position `index`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] char32_t& operator[](std::size_t index);

    ////////////////////////////////////////////////////////////
    /// \brief Clear the string
    ///
    /// This function removes all the characters from the string.
    ///
    /// \see `isEmpty`, `erase`
    ///
    ////////////////////////////////////////////////////////////
    void clear();

    ////////////////////////////////////////////////////////////
    /// \brief Get the size of the string
    ///
    /// \return Number of characters in the string
    ///
    /// \see `isEmpty`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::size_t getSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Check whether the string is empty or not
    ///
    /// \return `true` if the string is empty (i.e. contains no character)
    ///
    /// \see `clear`, `getSize`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool isEmpty() const;

    ////////////////////////////////////////////////////////////
    /// \brief Erase one or more characters from the string
    ///
    /// This function removes a sequence of `count` characters
    /// starting from `position`.
    ///
    /// \param position Position of the first character to erase
    /// \param count    Number of characters to erase
    ///
    ////////////////////////////////////////////////////////////
    void erase(std::size_t position, std::size_t count = 1);

    ////////////////////////////////////////////////////////////
    /// \brief Insert one or more characters into the string
    ///
    /// This function inserts the characters of `str`
    /// into the string, starting from `position`.
    ///
    /// \param position Position of insertion
    /// \param str      Characters to insert
    ///
    ////////////////////////////////////////////////////////////
    void insert(std::size_t position, const String& str);

    ////////////////////////////////////////////////////////////
    /// \brief Find a sequence of one or more characters in the string
    ///
    /// This function searches for the characters of `str`
    /// in the string, starting from `start`.
    ///
    /// \param str   Characters to find
    /// \param start Where to begin searching
    ///
    /// \return Position of `str` in the string, or `String::InvalidPos` if not found
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::size_t find(const String& str, std::size_t start = 0) const;

    ////////////////////////////////////////////////////////////
    /// \brief Replace a substring with another string
    ///
    /// This function replaces the substring that starts at index `position`
    /// and spans `length` characters with the string `replaceWith`.
    ///
    /// \param position    Index of the first character to be replaced
    /// \param length      Number of characters to replace. You can pass InvalidPos to
    ///                    replace all characters until the end of the string.
    /// \param replaceWith String that replaces the given substring.
    ///
    ////////////////////////////////////////////////////////////
    void replace(std::size_t position, std::size_t length, const String& replaceWith);

    ////////////////////////////////////////////////////////////
    /// \brief Replace all occurrences of a substring with a replacement string
    ///
    /// This function replaces all occurrences of `searchFor` in this string
    /// with the string `replaceWith`.
    ///
    /// \param searchFor   The value being searched for
    /// \param replaceWith The value that replaces found `searchFor` values
    ///
    ////////////////////////////////////////////////////////////
    void replace(const String& searchFor, const String& replaceWith);

    ////////////////////////////////////////////////////////////
    /// \brief Return a part of the string
    ///
    /// This function returns the substring that starts at index `position`
    /// and spans `length` characters.
    ///
    /// \param position Index of the first character
    /// \param length   Number of characters to include in the substring (if
    ///                 the string is shorter, as many characters as possible
    ///                 are included). `InvalidPos` can be used to include all
    ///                 characters until the end of the string.
    ///
    /// \return String object containing a substring of this object
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] String substring(std::size_t position, std::size_t length = InvalidPos) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a pointer to the C-style array of characters
    ///
    /// This functions provides a read-only access to a
    /// null-terminated C-style representation of the string.
    /// The returned pointer is temporary and is meant only for
    /// immediate use, thus it is not recommended to store it.
    ///
    /// \return Read-only pointer to the array of characters
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] const char32_t* getData() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return an iterator to the beginning of the string
    ///
    /// \return Read-write iterator to the beginning of the string characters
    ///
    /// \see `end`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Iterator begin();

    ////////////////////////////////////////////////////////////
    /// \brief Return an iterator to the beginning of the string
    ///
    /// \return Read-only iterator to the beginning of the string characters
    ///
    /// \see `end`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] ConstIterator begin() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return an iterator to the end of the string
    ///
    /// The end iterator refers to 1 position past the last character;
    /// thus it represents an invalid character and should never be
    /// accessed.
    ///
    /// \return Read-write iterator to the end of the string characters
    ///
    /// \see `begin`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Iterator end();

    ////////////////////////////////////////////////////////////
    /// \brief Return an iterator to the end of the string
    ///
    /// The end iterator refers to 1 position past the last character;
    /// thus it represents an invalid character and should never be
    /// accessed.
    ///
    /// \return Read-only iterator to the end of the string characters
    ///
    /// \see `begin`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] ConstIterator end() const;

private:
    friend SFML_SYSTEM_API bool operator==(const String& left, const String& right);
    friend SFML_SYSTEM_API bool operator<(const String& left, const String& right);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::u32string m_string; //!< Internal string of UTF-32 characters
};

////////////////////////////////////////////////////////////
/// \relates String
/// \brief Overload of `operator==` to compare two UTF-32 strings
///
/// \param left  Left operand (a string)
/// \param right Right operand (a string)
///
/// \return `true` if both strings are equal
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_SYSTEM_API bool operator==(const String& left, const String& right);

////////////////////////////////////////////////////////////
/// \relates String
/// \brief Overload of `operator!=` to compare two UTF-32 strings
///
/// \param left  Left operand (a string)
/// \param right Right operand (a string)
///
/// \return `true` if both strings are different
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_SYSTEM_API bool operator!=(const String& left, const String& right);

////////////////////////////////////////////////////////////
/// \relates String
/// \brief Overload of `operator<` to compare two UTF-32 strings
///
/// \param left  Left operand (a string)
/// \param right Right operand (a string)
///
/// \return `true` if `left` is lexicographically before `right`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_SYSTEM_API bool operator<(const String& left, const String& right);

////////////////////////////////////////////////////////////
/// \relates String
/// \brief Overload of `operator>` to compare two UTF-32 strings
///
/// \param left  Left operand (a string)
/// \param right Right operand (a string)
///
/// \return `true` if `left` is lexicographically after `right`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_SYSTEM_API bool operator>(const String& left, const String& right);

////////////////////////////////////////////////////////////
/// \relates String
/// \brief Overload of `operator<=` to compare two UTF-32 strings
///
/// \param left  Left operand (a string)
/// \param right Right operand (a string)
///
/// \return `true` if `left` is lexicographically before or equivalent to `right`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_SYSTEM_API bool operator<=(const String& left, const String& right);

////////////////////////////////////////////////////////////
/// \relates String
/// \brief Overload of `operator>=` to compare two UTF-32 strings
///
/// \param left  Left operand (a string)
/// \param right Right operand (a string)
///
/// \return `true` if `left` is lexicographically after or equivalent to `right`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_SYSTEM_API bool operator>=(const String& left, const String& right);

////////////////////////////////////////////////////////////
/// \relates String
/// \brief Overload of binary `operator+` to concatenate two strings
///
/// \param left  Left operand (a string)
/// \param right Right operand (a string)
///
/// \return Concatenated string
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_SYSTEM_API String operator+(const String& left, const String& right);

} // namespace sf

#include <SFML/System/String.inl>


////////////////////////////////////////////////////////////
/// \class sf::String
/// \ingroup system
///
/// `sf::String` is a utility string class defined mainly for
/// convenience. It is a Unicode string (implemented using
/// UTF-32), thus it can store any character in the world
/// (European, Chinese, Arabic, Hebrew, etc.).
///
/// It automatically handles conversions from/to ANSI and
/// wide strings, so that you can work with standard string
/// classes and still be compatible with functions taking a
/// `sf::String`.
///
/// \code
/// sf::String s;
///
/// std::string s1 = s;  // automatically converted to ANSI string
/// std::wstring s2 = s; // automatically converted to wide string
/// s = "hello";         // automatically converted from ANSI string
/// s = L"hello";        // automatically converted from wide string
/// s += 'a';            // automatically converted from ANSI string
/// s += L'a';           // automatically converted from wide string
/// \endcode
///
/// Conversions involving ANSI strings use the default user locale. However
/// it is possible to use a custom locale if necessary:
/// \code
/// std::locale locale;
/// sf::String s;
/// ...
/// std::string s1 = s.toAnsiString(locale);
/// s = sf::String("hello", locale);
/// \endcode
///
/// `sf::String` defines the most important functions of the
/// standard `std::string` class: removing, random access, iterating,
/// appending, comparing, etc. However it is a simple class
/// provided for convenience, and you may have to consider using
/// a more optimized class if your program requires complex string
/// handling. The automatic conversion functions will then take
/// care of converting your string to `sf::String` whenever SFML
/// requires it.
///
/// Please note that SFML also defines a low-level, generic
/// interface for Unicode handling, see the `sf::Utf` classes.
///
////////////////////////////////////////////////////////////

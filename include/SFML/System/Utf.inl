////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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
// References:
//
// https://www.unicode.org/
// https://www.unicode.org/Public/PROGRAMS/CVTUTF/ConvertUTF.c
// https://www.unicode.org/Public/PROGRAMS/CVTUTF/ConvertUTF.h
// https://people.w3.org/rishida/scripts/uniview/conversion
//
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
template <typename InputIt, typename OutputIt>
OutputIt priv::copy(InputIt first, InputIt last, OutputIt d_first)
{
    while (first != last)
        *d_first++ = static_cast<typename OutputIt::container_type::value_type>(*first++);

    return d_first;
}

template <typename In>
In Utf<8>::decode(In begin, In end, std::uint32_t& output, std::uint32_t replacement)
{
    // clang-format off
    // Some useful precomputed data
    static constexpr int trailing[256] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5
    };

    static constexpr std::uint32_t offsets[6] =
    {
        0x00000000, 0x00003080, 0x000E2080, 0x03C82080, 0xFA082080, 0x82082080
    };
    // clang-format on

    // decode the character
    int trailingBytes = trailing[static_cast<std::uint8_t>(*begin)];
    if (begin + trailingBytes < end)
    {
        output = 0;

        // clang-format off
        switch (trailingBytes)
        {
            case 5: output += static_cast<std::uint8_t>(*begin++); output <<= 6; [[fallthrough]];
            case 4: output += static_cast<std::uint8_t>(*begin++); output <<= 6; [[fallthrough]];
            case 3: output += static_cast<std::uint8_t>(*begin++); output <<= 6; [[fallthrough]];
            case 2: output += static_cast<std::uint8_t>(*begin++); output <<= 6; [[fallthrough]];
            case 1: output += static_cast<std::uint8_t>(*begin++); output <<= 6; [[fallthrough]];
            case 0: output += static_cast<std::uint8_t>(*begin++);
        }
        // clang-format on

        output -= offsets[trailingBytes];
    }
    else
    {
        // Incomplete character
        begin  = end;
        output = replacement;
    }

    return begin;
}


////////////////////////////////////////////////////////////
template <typename Out>
Out Utf<8>::encode(std::uint32_t input, Out output, std::uint8_t replacement)
{
    // Some useful precomputed data
    static constexpr std::uint8_t firstBytes[7] = {0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};

    // encode the character
    if ((input > 0x0010FFFF) || ((input >= 0xD800) && (input <= 0xDBFF)))
    {
        // Invalid character
        if (replacement)
            *output++ = static_cast<typename Out::container_type::value_type>(replacement);
    }
    else
    {
        // Valid character

        // Get the number of bytes to write
        std::size_t bytestoWrite = 1;

        // clang-format off
        if      (input <  0x80)       bytestoWrite = 1;
        else if (input <  0x800)      bytestoWrite = 2;
        else if (input <  0x10000)    bytestoWrite = 3;
        else if (input <= 0x0010FFFF) bytestoWrite = 4;
        // clang-format on

        // Extract the bytes to write
        std::uint8_t bytes[4];

        // clang-format off
        switch (bytestoWrite)
        {
            case 4: bytes[3] = static_cast<std::uint8_t>((input | 0x80) & 0xBF); input >>= 6; [[fallthrough]];
            case 3: bytes[2] = static_cast<std::uint8_t>((input | 0x80) & 0xBF); input >>= 6; [[fallthrough]];
            case 2: bytes[1] = static_cast<std::uint8_t>((input | 0x80) & 0xBF); input >>= 6; [[fallthrough]];
            case 1: bytes[0] = static_cast<std::uint8_t> (input | firstBytes[bytestoWrite]);
        }
        // clang-format on

        // Add them to the output
        output = priv::copy(bytes, bytes + bytestoWrite, output);
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In>
In Utf<8>::next(In begin, In end)
{
    std::uint32_t codepoint;
    return decode(begin, end, codepoint);
}


////////////////////////////////////////////////////////////
template <typename In>
std::size_t Utf<8>::count(In begin, In end)
{
    std::size_t length = 0;
    while (begin < end)
    {
        begin = next(begin, end);
        ++length;
    }

    return length;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<8>::fromAnsi(In begin, In end, Out output, const std::locale& locale)
{
    while (begin < end)
    {
        std::uint32_t codepoint = Utf<32>::decodeAnsi(*begin++, locale);
        output                  = encode(codepoint, output);
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<8>::fromWide(In begin, In end, Out output)
{
    while (begin < end)
    {
        std::uint32_t codepoint = Utf<32>::decodeWide(*begin++);
        output                  = encode(codepoint, output);
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<8>::fromLatin1(In begin, In end, Out output)
{
    // Latin-1 is directly compatible with Unicode encodings,
    // and can thus be treated as (a sub-range of) UTF-32
    while (begin < end)
        output = encode(*begin++, output);

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<8>::toAnsi(In begin, In end, Out output, char replacement, const std::locale& locale)
{
    while (begin < end)
    {
        std::uint32_t codepoint;
        begin  = decode(begin, end, codepoint);
        output = Utf<32>::encodeAnsi(codepoint, output, replacement, locale);
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<8>::toWide(In begin, In end, Out output, wchar_t replacement)
{
    while (begin < end)
    {
        std::uint32_t codepoint;
        begin  = decode(begin, end, codepoint);
        output = Utf<32>::encodeWide(codepoint, output, replacement);
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<8>::toLatin1(In begin, In end, Out output, char replacement)
{
    // Latin-1 is directly compatible with Unicode encodings,
    // and can thus be treated as (a sub-range of) UTF-32
    while (begin < end)
    {
        std::uint32_t codepoint;
        begin     = decode(begin, end, codepoint);
        *output++ = codepoint < 256 ? static_cast<char>(codepoint) : replacement;
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<8>::toUtf8(In begin, In end, Out output)
{
    return priv::copy(begin, end, output);
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<8>::toUtf16(In begin, In end, Out output)
{
    while (begin < end)
    {
        std::uint32_t codepoint;
        begin  = decode(begin, end, codepoint);
        output = Utf<16>::encode(codepoint, output);
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<8>::toUtf32(In begin, In end, Out output)
{
    while (begin < end)
    {
        std::uint32_t codepoint;
        begin     = decode(begin, end, codepoint);
        *output++ = codepoint;
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In>
In Utf<16>::decode(In begin, In end, std::uint32_t& output, std::uint32_t replacement)
{
    std::uint16_t first = *begin++;

    // If it's a surrogate pair, first convert to a single UTF-32 character
    if ((first >= 0xD800) && (first <= 0xDBFF))
    {
        if (begin < end)
        {
            std::uint32_t second = *begin++;
            if ((second >= 0xDC00) && (second <= 0xDFFF))
            {
                // The second element is valid: convert the two elements to a UTF-32 character
                output = ((first - 0xD800u) << 10) + (second - 0xDC00) + 0x0010000;
            }
            else
            {
                // Invalid character
                output = replacement;
            }
        }
        else
        {
            // Invalid character
            begin  = end;
            output = replacement;
        }
    }
    else
    {
        // We can make a direct copy
        output = first;
    }

    return begin;
}


////////////////////////////////////////////////////////////
template <typename Out>
Out Utf<16>::encode(std::uint32_t input, Out output, std::uint16_t replacement)
{
    if (input <= 0xFFFF)
    {
        // The character can be copied directly, we just need to check if it's in the valid range
        if ((input >= 0xD800) && (input <= 0xDFFF))
        {
            // Invalid character (this range is reserved)
            if (replacement)
                *output++ = replacement;
        }
        else
        {
            // Valid character directly convertible to a single UTF-16 character
            *output++ = static_cast<std::uint16_t>(input);
        }
    }
    else if (input > 0x0010FFFF)
    {
        // Invalid character (greater than the maximum Unicode value)
        if (replacement)
            *output++ = replacement;
    }
    else
    {
        // The input character will be converted to two UTF-16 elements
        input -= 0x0010000;
        *output++ = static_cast<std::uint16_t>((input >> 10) + 0xD800);
        *output++ = static_cast<std::uint16_t>((input & 0x3FFUL) + 0xDC00);
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In>
In Utf<16>::next(In begin, In end)
{
    std::uint32_t codepoint;
    return decode(begin, end, codepoint);
}


////////////////////////////////////////////////////////////
template <typename In>
std::size_t Utf<16>::count(In begin, In end)
{
    std::size_t length = 0;
    while (begin < end)
    {
        begin = next(begin, end);
        ++length;
    }

    return length;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<16>::fromAnsi(In begin, In end, Out output, const std::locale& locale)
{
    while (begin < end)
    {
        std::uint32_t codepoint = Utf<32>::decodeAnsi(*begin++, locale);
        output                  = encode(codepoint, output);
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<16>::fromWide(In begin, In end, Out output)
{
    while (begin < end)
    {
        std::uint32_t codepoint = Utf<32>::decodeWide(*begin++);
        output                  = encode(codepoint, output);
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<16>::fromLatin1(In begin, In end, Out output)
{
    // Latin-1 is directly compatible with Unicode encodings,
    // and can thus be treated as (a sub-range of) UTF-32
    return priv::copy(begin, end, output);
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<16>::toAnsi(In begin, In end, Out output, char replacement, const std::locale& locale)
{
    while (begin < end)
    {
        std::uint32_t codepoint;
        begin  = decode(begin, end, codepoint);
        output = Utf<32>::encodeAnsi(codepoint, output, replacement, locale);
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<16>::toWide(In begin, In end, Out output, wchar_t replacement)
{
    while (begin < end)
    {
        std::uint32_t codepoint;
        begin  = decode(begin, end, codepoint);
        output = Utf<32>::encodeWide(codepoint, output, replacement);
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<16>::toLatin1(In begin, In end, Out output, char replacement)
{
    // Latin-1 is directly compatible with Unicode encodings,
    // and can thus be treated as (a sub-range of) UTF-32
    while (begin < end)
    {
        *output++ = *begin < 256 ? static_cast<char>(*begin) : replacement;
        ++begin;
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<16>::toUtf8(In begin, In end, Out output)
{
    while (begin < end)
    {
        std::uint32_t codepoint;
        begin  = decode(begin, end, codepoint);
        output = Utf<8>::encode(codepoint, output);
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<16>::toUtf16(In begin, In end, Out output)
{
    return priv::copy(begin, end, output);
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<16>::toUtf32(In begin, In end, Out output)
{
    while (begin < end)
    {
        std::uint32_t codepoint;
        begin     = decode(begin, end, codepoint);
        *output++ = codepoint;
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In>
In Utf<32>::decode(In begin, In /*end*/, std::uint32_t& output, std::uint32_t /*replacement*/)
{
    output = *begin++;
    return begin;
}


////////////////////////////////////////////////////////////
template <typename Out>
Out Utf<32>::encode(std::uint32_t input, Out output, std::uint32_t /*replacement*/)
{
    *output++ = input;
    return output;
}


////////////////////////////////////////////////////////////
template <typename In>
In Utf<32>::next(In begin, In /*end*/)
{
    return ++begin;
}


////////////////////////////////////////////////////////////
template <typename In>
std::size_t Utf<32>::count(In begin, In end)
{
    return begin - end;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<32>::fromAnsi(In begin, In end, Out output, const std::locale& locale)
{
    while (begin < end)
        *output++ = decodeAnsi(*begin++, locale);

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<32>::fromWide(In begin, In end, Out output)
{
    while (begin < end)
        *output++ = decodeWide(*begin++);

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<32>::fromLatin1(In begin, In end, Out output)
{
    // Latin-1 is directly compatible with Unicode encodings,
    // and can thus be treated as (a sub-range of) UTF-32
    return priv::copy(begin, end, output);
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<32>::toAnsi(In begin, In end, Out output, char replacement, const std::locale& locale)
{
    while (begin < end)
        output = encodeAnsi(*begin++, output, replacement, locale);

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<32>::toWide(In begin, In end, Out output, wchar_t replacement)
{
    while (begin < end)
        output = encodeWide(*begin++, output, replacement);

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<32>::toLatin1(In begin, In end, Out output, char replacement)
{
    // Latin-1 is directly compatible with Unicode encodings,
    // and can thus be treated as (a sub-range of) UTF-32
    while (begin < end)
    {
        *output++ = *begin < 256 ? static_cast<char>(*begin) : replacement;
        ++begin;
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<32>::toUtf8(In begin, In end, Out output)
{
    while (begin < end)
        output = Utf<8>::encode(*begin++, output);

    return output;
}

////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<32>::toUtf16(In begin, In end, Out output)
{
    while (begin < end)
        output = Utf<16>::encode(*begin++, output);

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<32>::toUtf32(In begin, In end, Out output)
{
    return priv::copy(begin, end, output);
}


////////////////////////////////////////////////////////////
template <typename In>
std::uint32_t Utf<32>::decodeAnsi(In input, [[maybe_unused]] const std::locale& locale)
{
    // On Windows, GCC's standard library (glibc++) has almost
    // no support for Unicode stuff. As a consequence, in this
    // context we can only use the default locale and ignore
    // the one passed as parameter.

#if defined(SFML_SYSTEM_WINDOWS) &&                         /* if Windows ... */                          \
    (defined(__GLIBCPP__) || defined(__GLIBCXX__)) &&       /* ... and standard library is glibc++ ... */ \
    !(defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION)) /* ... and STLPort is not used on top of it */

    wchar_t character = 0;
    mbtowc(&character, &input, 1);
    return static_cast<std::uint32_t>(character);

#else

    // Get the facet of the locale which deals with character conversion
    const auto& facet = std::use_facet<std::ctype<wchar_t>>(locale);

    // Use the facet to convert each character of the input string
    return static_cast<std::uint32_t>(facet.widen(input));

#endif
}


////////////////////////////////////////////////////////////
template <typename In>
std::uint32_t Utf<32>::decodeWide(In input)
{
    // The encoding of wide characters is not well defined and is left to the system;
    // however we can safely assume that it is UCS-2 on Windows and
    // UCS-4 on Unix systems.
    // In both cases, a simple copy is enough (UCS-2 is a subset of UCS-4,
    // and UCS-4 *is* UTF-32).

    return static_cast<std::uint32_t>(input);
}


////////////////////////////////////////////////////////////
template <typename Out>
Out Utf<32>::encodeAnsi(std::uint32_t codepoint, Out output, char replacement, [[maybe_unused]] const std::locale& locale)
{
    // On Windows, gcc's standard library (glibc++) has almost
    // no support for Unicode stuff. As a consequence, in this
    // context we can only use the default locale and ignore
    // the one passed as parameter.

#if defined(SFML_SYSTEM_WINDOWS) &&                         /* if Windows ... */                          \
    (defined(__GLIBCPP__) || defined(__GLIBCXX__)) &&       /* ... and standard library is glibc++ ... */ \
    !(defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION)) /* ... and STLPort is not used on top of it */

    char character = 0;
    if (wctomb(&character, static_cast<wchar_t>(codepoint)) >= 0)
        *output++ = character;
    else if (replacement)
        *output++ = replacement;

    return output;

#else

    // Get the facet of the locale which deals with character conversion
    const auto& facet = std::use_facet<std::ctype<wchar_t>>(locale);

    // Use the facet to convert each character of the input string
    *output++ = facet.narrow(static_cast<wchar_t>(codepoint), replacement);

    return output;

#endif
}


////////////////////////////////////////////////////////////
template <typename Out>
Out Utf<32>::encodeWide(std::uint32_t codepoint, Out output, wchar_t replacement)
{
    // The encoding of wide characters is not well defined and is left to the system;
    // however we can safely assume that it is UCS-2 on Windows and
    // UCS-4 on Unix systems.
    // For UCS-2 we need to check if the source characters fits in (UCS-2 is a subset of UCS-4).
    // For UCS-4 we can do a direct copy (UCS-4 *is* UTF-32).

    switch (sizeof(wchar_t))
    {
        case 4:
        {
            *output++ = static_cast<wchar_t>(codepoint);
            break;
        }

        default:
        {
            if ((codepoint <= 0xFFFF) && ((codepoint < 0xD800) || (codepoint > 0xDFFF)))
            {
                *output++ = static_cast<wchar_t>(codepoint);
            }
            else if (replacement)
            {
                *output++ = replacement;
            }
            break;
        }
    }

    return output;
}

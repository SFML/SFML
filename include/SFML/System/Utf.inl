////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2012 Laurent Gomila (laurent.gom@gmail.com)
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
// References :
//
// http://www.unicode.org/
// http://www.unicode.org/Public/PROGRAMS/CVTUTF/ConvertUTF.c
// http://www.unicode.org/Public/PROGRAMS/CVTUTF/ConvertUTF.h
// http://people.w3.org/rishida/scripts/uniview/conversion
//
////////////////////////////////////////////////////////////


namespace priv
{
typedef std::codecvt<wchar_t, char, std::mbstate_t> cvt_t;

////////////////////////////////////////////////////////////
inline std::size_t cvtMaxOutLen(const cvt_t& cvt, char)
{
    return static_cast<std::size_t>(std::max(1, cvt.max_length() + 1));
}


////////////////////////////////////////////////////////////
inline std::size_t cvtMaxOutLen(const cvt_t&, wchar_t)
{
    return sizeof(sf::Uint32) / sizeof(wchar_t) + 1;
}


////////////////////////////////////////////////////////////
inline std::codecvt_base::result cvtConvert(
    std::mbstate_t& mb,
    const cvt_t& codecvt,
    const std::vector<wchar_t>& in, const wchar_t*& inConvertedEnd,
    std::vector<char>& out, char*& outConvertedEnd)
{
    return codecvt.out(
        mb,
        &in[0], &in[0] + in.size(), inConvertedEnd,
        &out[0], &out[0] + out.size(), outConvertedEnd);
}


////////////////////////////////////////////////////////////
inline std::codecvt_base::result cvtConvert(
    std::mbstate_t& mb,
    const cvt_t& codecvt,
    const std::vector<char>& in, const char*& inConvertedEnd,
    std::vector<wchar_t>& out, wchar_t*& outConvertedEnd)
{
    return codecvt.in(
        mb,
        &in[0], &in[0] + in.size(), inConvertedEnd,
        &out[0], &out[0] + out.size(), outConvertedEnd);
}


////////////////////////////////////////////////////////////
template<typename Out>
Out cvtFinalize(
    Out output, std::vector<char>& out,
    std::mbstate_t mb, const cvt_t& codecvt,
    char replacement)
{
    char* outConvertedEnd = &out[0];
    const std::codecvt_base::result r = codecvt.unshift(
        mb, &out[0], &out[0] + out.size(), outConvertedEnd);
    output = std::copy(&out[0], outConvertedEnd, output);
    if (r != std::codecvt_base::ok)
        *output++ = replacement;
    return output;
}


////////////////////////////////////////////////////////////
template<typename Out>
Out cvtFinalize(
    Out output, std::vector<wchar_t>&,
    std::mbstate_t, const cvt_t&,
    wchar_t)
{
    return output;
}


////////////////////////////////////////////////////////////
template<typename To> struct CvtFromType;
template<> struct CvtFromType<char> { typedef wchar_t type; };
template<> struct CvtFromType<wchar_t> { typedef char type; };


////////////////////////////////////////////////////////////
template <typename To, typename In, typename Out>
Out cvtWideAnsi(
    In begin, In end, Out output,
    To replacement = To(), const std::locale& locale = std::locale())
{
    typedef CvtFromType<To>::type From;
    const cvt_t& codecvt = std::use_facet<cvt_t>(locale);

    std::mbstate_t mb = std::mbstate_t();
    std::vector<From> in;
    std::vector<To> out(cvtMaxOutLen(codecvt, To()));
    while (begin != end)
    {
        in.push_back(*begin++);
        const From* inConvertedEnd = &in[0];
        To* outConvertedEnd = &out[0];
        const std::codecvt_base::result r = cvtConvert(
            mb, codecvt, in, inConvertedEnd, out, outConvertedEnd);
        switch (r)
        {
            case std::codecvt_base::error:
            {
                *output++ = replacement;
                mb = std::mbstate_t();
                break;
            }
            case std::codecvt_base::noconv:
            case std::codecvt_base::partial:
            case std::codecvt_base::ok:
            {
                const std::size_t nConverted = inConvertedEnd - &in[0];
                in.erase(in.begin(), in.begin() + nConverted);
                output = std::copy(&out[0], outConvertedEnd, output);
                break;
            }
            default:
                assert(!"Unexpected error from priv::convert()");
        }
    }
    return cvtFinalize(output, out, mb, codecvt, replacement);
}


////////////////////////////////////////////////////////////
template <unsigned int N, typename In, typename Out>
Out toAnsi(In begin, In end, Out output, char replacement, const std::locale& locale)
{
    std::vector<char> ansi;
    switch (sizeof(wchar_t))
    {
        case N / 8:
        {
            return priv::cvtWideAnsi<char>(
                begin, end, output, replacement, locale);
        }
        default:
        {
            std::vector<wchar_t> wide;
            wide.reserve(end - begin);
            Utf<N>::toWide(
                begin, end, std::back_inserter(wide), replacement);
            return priv::cvtWideAnsi<char>(
                wide.begin(), wide.end(), output, replacement, locale);
        }
    }
}


////////////////////////////////////////////////////////////
template <unsigned int NFrom, unsigned int NTo,
          typename In, typename Out>
Out utfToUtf(In begin, In end, Out output)
{
    switch (NTo)
    {
        case 8:
            return Utf<NFrom>::toUtf8(begin, end, output);
        case 16:
            return Utf<NFrom>::toUtf16(begin, end, output);
        case 32:
            return Utf<NFrom>::toUtf32(begin, end, output);
    }
    assert(!"Invalid NTo (only 8, 16 and 32 are valid).");
    return output;
}


////////////////////////////////////////////////////////////
template <typename In>
std::size_t wideReserveSize(In, In, std::input_iterator_tag)
{
    return 0;
}


////////////////////////////////////////////////////////////
template <typename In>
std::size_t wideReserveSize(In begin, In end, std::forward_iterator_tag)
{
    return std::distance(begin, end);
}


////////////////////////////////////////////////////////////
template <unsigned int N, typename In, typename Out>
Out fromAnsi(In begin, In end, Out output, const std::locale& locale)
{
    std::vector<wchar_t> wide;
    wide.reserve(wideReserveSize(
        begin, end, std::iterator_traits<In>::iterator_category()));
    priv::cvtWideAnsi<wchar_t>(begin, end, std::back_inserter(wide));
    return Utf<N>::fromWide(wide.begin(), wide.end(), output);
}

} // namespace priv



////////////////////////////////////////////////////////////
template <typename In>
In Utf<8>::decode(In begin, In end, Uint32& output, Uint32 replacement)
{
    // Some useful precomputed data
    static const int trailing[256] =
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
    static const Uint32 offsets[6] =
    {
        0x00000000, 0x00003080, 0x000E2080, 0x03C82080, 0xFA082080, 0x82082080
    };

    // decode the character
    int trailingBytes = trailing[static_cast<Uint8>(*begin)];
    if (begin + trailingBytes < end)
    {
        output = 0;
        switch (trailingBytes)
        {
            case 5 : output += static_cast<Uint8>(*begin++); output <<= 6;
            case 4 : output += static_cast<Uint8>(*begin++); output <<= 6;
            case 3 : output += static_cast<Uint8>(*begin++); output <<= 6;
            case 2 : output += static_cast<Uint8>(*begin++); output <<= 6;
            case 1 : output += static_cast<Uint8>(*begin++); output <<= 6;
            case 0 : output += static_cast<Uint8>(*begin++);
        }
        output -= offsets[trailingBytes];
    }
    else
    {
        // Incomplete character
        begin = end;
        output = replacement;
    }

    return begin;
}


////////////////////////////////////////////////////////////
template <typename Out>
Out Utf<8>::encode(Uint32 input, Out output, Uint8 replacement)
{
    // Some useful precomputed data
    static const Uint8 firstBytes[7] =
    {
        0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC
    };

    // encode the character
    if ((input > 0x0010FFFF) || ((input >= 0xD800) && (input <= 0xDBFF)))
    {
        // Invalid character
        if (replacement)
            *output++ = replacement;
    }
    else
    {
        // Valid character

        // Get the number of bytes to write
        int bytestoWrite = 1;
        if      (input <  0x80)       bytestoWrite = 1;
        else if (input <  0x800)      bytestoWrite = 2;
        else if (input <  0x10000)    bytestoWrite = 3;
        else if (input <= 0x0010FFFF) bytestoWrite = 4;

        // Extract the bytes to write
        Uint8 bytes[4];
        switch (bytestoWrite)
        {
            case 4 : bytes[3] = static_cast<Uint8>((input | 0x80) & 0xBF); input >>= 6;
            case 3 : bytes[2] = static_cast<Uint8>((input | 0x80) & 0xBF); input >>= 6;
            case 2 : bytes[1] = static_cast<Uint8>((input | 0x80) & 0xBF); input >>= 6;
            case 1 : bytes[0] = static_cast<Uint8> (input | firstBytes[bytestoWrite]);
        }

        // Add them to the output
        const Uint8* currentByte = bytes;
        switch (bytestoWrite)
        {
            case 4 : *output++ = *currentByte++;
            case 3 : *output++ = *currentByte++;
            case 2 : *output++ = *currentByte++;
            case 1 : *output++ = *currentByte++;
        }
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In>
In Utf<8>::next(In begin, In end)
{
    Uint32 codepoint;
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
    return priv::fromAnsi<8>(begin, end, output, locale);
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<8>::fromWide(In begin, In end, Out output)
{
    return priv::utfToUtf<8, sizeof(wchar_t) * 8>(begin, end, output);
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
    return priv::toAnsi<8>(begin, end, output, replacement, locale);
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<8>::toWide(In begin, In end, Out output, wchar_t replacement)
{
    return priv::utfToUtf<8 * sizeof(wchar_t), 8>(begin, end, output);
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<8>::toLatin1(In begin, In end, Out output, char replacement)
{
    // Latin-1 is directly compatible with Unicode encodings,
    // and can thus be treated as (a sub-range of) UTF-32
    while (begin < end)
    {
        Uint32 codepoint;
        begin = decode(begin, end, codepoint);
        *output++ = codepoint < 256 ? static_cast<char>(codepoint) : replacement;
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<8>::toUtf8(In begin, In end, Out output)
{
    while (begin < end)
        *output++ = *begin++;

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<8>::toUtf16(In begin, In end, Out output)
{
    while (begin < end)
    {
        Uint32 codepoint;
        begin = decode(begin, end, codepoint);
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
        Uint32 codepoint;
        begin = decode(begin, end, codepoint);
        *output++ = codepoint;
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In>
In Utf<16>::decode(In begin, In end, Uint32& output, Uint32 replacement)
{
    Uint16 first = *begin++;

    // If it's a surrogate pair, first convert to a single UTF-32 character
    if ((first >= 0xD800) && (first <= 0xDBFF))
    {
        if (begin < end)
        {
            Uint32 second = *begin++;
            if ((second >= 0xDC00) && (second <= 0xDFFF))
            {
                // The second element is valid: convert the two elements to a UTF-32 character
                output = static_cast<Uint32>(((first - 0xD800) << 10) + (second - 0xDC00) + 0x0010000);
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
            begin = end;
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
Out Utf<16>::encode(Uint32 input, Out output, Uint16 replacement)
{
    if (input < 0xFFFF)
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
            *output++ = static_cast<Uint16>(input);
        }
    }
    else if (input > 0x0010FFFF)
    {
        // Invalid character (greater than the maximum unicode value)
        if (replacement)
            *output++ = replacement;
    }
    else
    {
        // The input character will be converted to two UTF-16 elements
        input -= 0x0010000;
        *output++ = static_cast<Uint16>((input >> 10)     + 0xD800);
        *output++ = static_cast<Uint16>((input & 0x3FFUL) + 0xDC00);
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In>
In Utf<16>::next(In begin, In end)
{
    Uint32 codepoint;
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
    return priv::fromAnsi<16>(begin, end, output, locale);
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<16>::fromWide(In begin, In end, Out output)
{
    return priv::utfToUtf<16, sizeof(wchar_t) * 8>(begin, end, output);
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<16>::fromLatin1(In begin, In end, Out output)
{
    // Latin-1 is directly compatible with Unicode encodings,
    // and can thus be treated as (a sub-range of) UTF-32
    while (begin < end)
        *output++ = *begin++;

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<16>::toAnsi(In begin, In end, Out output, char replacement, const std::locale& locale)
{
    return priv::toAnsi<16>(begin, end, output, replacement, locale);
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<16>::toWide(In begin, In end, Out output, wchar_t replacement)
{
    return priv::utfToUtf<8 * sizeof(wchar_t), 16>(begin, end, output);
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
        begin++;
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<16>::toUtf8(In begin, In end, Out output)
{
    while (begin < end)
    {
        Uint32 codepoint;
        begin = decode(begin, end, codepoint);
        output = Utf<8>::encode(codepoint, output);
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<16>::toUtf16(In begin, In end, Out output)
{
    while (begin < end)
        *output++ = *begin++;

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<16>::toUtf32(In begin, In end, Out output)
{
    while (begin < end)
    {
        Uint32 codepoint;
        begin = decode(begin, end, codepoint);
        *output++ = codepoint;
    }

    return output;
}


////////////////////////////////////////////////////////////
template <typename In>
In Utf<32>::decode(In begin, In /*end*/, Uint32& output, Uint32 /*replacement*/)
{
    output = *begin++;
    return begin;
}


////////////////////////////////////////////////////////////
template <typename Out>
Out Utf<32>::encode(Uint32 input, Out output, Uint32 /*replacement*/)
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
    return priv::fromAnsi<32>(begin, end, output, locale);
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<32>::fromWide(In begin, In end, Out output)
{
    return priv::utfToUtf<32, sizeof(wchar_t) * 8>(begin, end, output);
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<32>::fromLatin1(In begin, In end, Out output)
{
    // Latin-1 is directly compatible with Unicode encodings,
    // and can thus be treated as (a sub-range of) UTF-32
    while (begin < end)
        *output++ = *begin++;

    return output;
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<32>::toAnsi(In begin, In end, Out output, char replacement, const std::locale& locale)
{
    return priv::toAnsi<32>(begin, end, output, replacement, locale);
}


////////////////////////////////////////////////////////////
template <typename In, typename Out>
Out Utf<32>::toWide(In begin, In end, Out output, wchar_t replacement)
{
    return priv::utfToUtf<8 * sizeof(wchar_t), 32>(begin, end, output);
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
        begin++;
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
    while (begin < end)
        *output++ = *begin++;

    return output;
}


////////////////////////////////////////////////////////////
template <typename In>
Uint32 Utf<32>::decodeAnsi(In input, const std::locale& locale)
{
    // On Windows, gcc's standard library (glibc++) has almost
    // no support for Unicode stuff. As a consequence, in this
    // context we can only use the default locale and ignore
    // the one passed as parameter.

    #if defined(SFML_SYSTEM_WINDOWS) &&                       /* if Windows ... */                          \
       (defined(__GLIBCPP__) || defined (__GLIBCXX__)) &&     /* ... and standard library is glibc++ ... */ \
      !(defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION)) /* ... and STLPort is not used on top of it */

        (void)locale; // to avoid warnings

        wchar_t character = 0;
        mbtowc(&character, &input, 1);
        return static_cast<Uint32>(character);

    #else

        // Get the facet of the locale which deals with character conversion
        const std::ctype<wchar_t>& facet = std::use_facet< std::ctype<wchar_t> >(locale);

        // Use the facet to convert each character of the input string
        return static_cast<Uint32>(facet.widen(input));

    #endif
}


////////////////////////////////////////////////////////////
template <typename In>
Uint32 Utf<32>::decodeWide(In input)
{
    // The encoding of wide characters is not well defined and is left to the system;
    // however we can safely assume that it is UCS-2 on Windows and
    // UCS-4 on Unix systems.
    // In both cases, a simple copy is enough (UCS-2 is a subset of UCS-4,
    // and UCS-4 *is* UTF-32).

    return input;
}


////////////////////////////////////////////////////////////
template <typename Out>
Out Utf<32>::encodeAnsi(Uint32 codepoint, Out output, char replacement, const std::locale& locale)
{
    // On Windows, gcc's standard library (glibc++) has almost
    // no support for Unicode stuff. As a consequence, in this
    // context we can only use the default locale and ignore
    // the one passed as parameter.

    #if defined(SFML_SYSTEM_WINDOWS) &&                       /* if Windows ... */                          \
       (defined(__GLIBCPP__) || defined (__GLIBCXX__)) &&     /* ... and standard library is glibc++ ... */ \
      !(defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION)) /* ... and STLPort is not used on top of it */

        (void)locale; // to avoid warnings

        char character = 0;
        if (wctomb(&character, static_cast<wchar_t>(codepoint)) >= 0)
            *output++ = character;
        else if (replacement)
            *output++ = replacement;

        return output;

    #else

        // Get the facet of the locale which deals with character conversion
        const std::ctype<wchar_t>& facet = std::use_facet< std::ctype<wchar_t> >(locale);

        // Use the facet to convert each character of the input string
        *output++ = facet.narrow(static_cast<wchar_t>(codepoint), replacement);

        return output;

    #endif
}


////////////////////////////////////////////////////////////
template <typename Out>
Out Utf<32>::encodeWide(Uint32 codepoint, Out output, wchar_t replacement)
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

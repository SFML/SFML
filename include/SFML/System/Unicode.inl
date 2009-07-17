////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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
/// Generic function to convert an UTF-32 characters range
/// to an ANSI characters range, using the given locale
////////////////////////////////////////////////////////////
template <typename In, typename Out>
inline Out Unicode::UTF32ToANSI(In begin, In end, Out output, char replacement, const std::locale& locale)
{
    #ifdef __MINGW32__

        // MinGW has a almost no support for unicode stuff
        // As a consequence, the MinGW version of this function can only use the default locale
        // and ignores the one passed as parameter
        while (begin < end)
        {
            char character = 0;
            if (wctomb(&character, static_cast<wchar_t>(*begin++)) >= 0)
                *output++ = character;
            else if (replacement)
                *output++ = replacement;
        }

    #else

        // Get the facet of the locale which deals with character conversion
        const std::ctype<wchar_t>& facet = std::use_facet< std::ctype<wchar_t> >(locale);

        // Use the facet to convert each character of the input string
        while (begin < end)
            *output++ = facet.narrow(static_cast<wchar_t>(*begin++), replacement);

    #endif

    return output;
}


////////////////////////////////////////////////////////////
/// Generic function to convert an ANSI characters range
/// to an UTF-32 characters range, using the given locale
////////////////////////////////////////////////////////////
template <typename In, typename Out>
inline Out Unicode::ANSIToUTF32(In begin, In end, Out output, const std::locale& locale)
{
    #ifdef __MINGW32__

        // MinGW has a almost no support for unicode stuff
        // As a consequence, the MinGW version of this function can only use the default locale
        // and ignores the one passed as parameter
        while (begin < end)
        {
            wchar_t character = 0;
            mbtowc(&character, &*begin, 1);
            begin++;
            *output++ = static_cast<Uint32>(character);
        }

    #else

        // Get the facet of the locale which deals with character conversion
        const std::ctype<wchar_t>& facet = std::use_facet< std::ctype<wchar_t> >(locale);

        // Use the facet to convert each character of the input string
        while (begin < end)
            *output++ = static_cast<Uint32>(facet.widen(*begin++));

    #endif

    return output;
}


////////////////////////////////////////////////////////////
/// Generic function to convert an UTF-8 characters range
/// to an UTF-16 characters range, using the given locale
////////////////////////////////////////////////////////////
template <typename In, typename Out>
inline Out Unicode::UTF8ToUTF16(In begin, In end, Out output, Uint16 replacement)
{
    while (begin < end)
    {
        Uint32 character = 0;
        int trailingBytes = UTF8TrailingBytes[static_cast<int>(*begin)];
        if (begin + trailingBytes < end)
        {
            // First decode the UTF-8 character
            switch (trailingBytes)
            {
                case 5 : character += *begin++; character <<= 6;
                case 4 : character += *begin++; character <<= 6;
                case 3 : character += *begin++; character <<= 6;
                case 2 : character += *begin++; character <<= 6;
                case 1 : character += *begin++; character <<= 6;
                case 0 : character += *begin++;
            }
	        character -= UTF8Offsets[trailingBytes];

            // Then encode it in UTF-16
            if (character < 0xFFFF)
            {
                // Character can be converted directly to 16 bits, just need to check it's in the valid range
                if ((character >= 0xD800) && (character <= 0xDFFF))
                {
                    // Invalid character (this range is reserved)
                    if (replacement)
                        *output++ = replacement;
                }
                else
                {
                    // Valid character directly convertible to 16 bits
                    *output++ = static_cast<Uint16>(character);
                }
            }
            else if (character > 0x0010FFFF)
            {
                // Invalid character (greater than the maximum unicode value)
                if (replacement)
                    *output++ = replacement;
            }
            else
            {
                // Character will be converted to 2 UTF-16 elements
                character -= 0x0010000;
                *output++ = static_cast<Uint16>((character >> 10)     + 0xD800);
                *output++ = static_cast<Uint16>((character & 0x3FFUL) + 0xDC00);
            }
        }
    }

    return output;
}


////////////////////////////////////////////////////////////
/// Generic function to convert an UTF-8 characters range
/// to an UTF-32 characters range, using the given locale
////////////////////////////////////////////////////////////
template <typename In, typename Out>
inline Out Unicode::UTF8ToUTF32(In begin, In end, Out output, Uint32 replacement)
{
    while (begin < end)
    {
        Uint32 character = 0;
        int trailingBytes = UTF8TrailingBytes[static_cast<int>(*begin)];
        if (begin + trailingBytes < end)
        {
            // First decode the UTF-8 character
            switch (trailingBytes)
            {
                case 5 : character += *begin++; character <<= 6;
                case 4 : character += *begin++; character <<= 6;
                case 3 : character += *begin++; character <<= 6;
                case 2 : character += *begin++; character <<= 6;
                case 1 : character += *begin++; character <<= 6;
                case 0 : character += *begin++;
            }
	        character -= UTF8Offsets[trailingBytes];

            // Then write it if valid
            if ((character < 0xD800) || (character > 0xDFFF))
            {
                // Valid UTF-32 character
                *output++ = character;
            }
            else
            {
                // Invalid UTF-32 character
                if (replacement)
                    *output++ = replacement;
            }
        }
    }

    return output;
}


////////////////////////////////////////////////////////////
/// Generic function to convert an UTF-16 characters range
/// to an UTF-8 characters range, using the given locale
////////////////////////////////////////////////////////////
template <typename In, typename Out>
inline Out Unicode::UTF16ToUTF8(In begin, In end, Out output, Uint8 replacement)
{
    while (begin < end)
    {
        Uint32 character = *begin++;

        // If it's a surrogate pair, first convert to a single UTF-32 character
        if ((character >= 0xD800) && (character <= 0xDBFF))
        {
            if (begin < end)
            {
                // The second element is valid : convert the two elements to a UTF-32 character
                Uint32 d = *begin++;
                if ((d >= 0xDC00) && (d <= 0xDFFF))
                    character = static_cast<Uint32>(((character - 0xD800) << 10) + (d - 0xDC00) + 0x0010000);
            }
            else
            {
                // Invalid second element
                if (replacement)
                    *output++ = replacement;
            }
        }

        // Then convert to UTF-8
        if (character > 0x0010FFFF)
        {
            // Invalid character (greater than the maximum unicode value)
            if (replacement)
                *output++ = replacement;
        }
        else
        {
            // Valid character

            // Get number of bytes to write
            int bytesToWrite = 1;
            if      (character <  0x80)       bytesToWrite = 1;
            else if (character <  0x800)      bytesToWrite = 2;
            else if (character <  0x10000)    bytesToWrite = 3;
            else if (character <= 0x0010FFFF) bytesToWrite = 4;

            // Extract bytes to write
            Uint8 bytes[4];
            switch (bytesToWrite)
            {
                case 4 : bytes[3] = static_cast<Uint8>((character | 0x80) & 0xBF); character >>= 6;
                case 3 : bytes[2] = static_cast<Uint8>((character | 0x80) & 0xBF); character >>= 6;
                case 2 : bytes[1] = static_cast<Uint8>((character | 0x80) & 0xBF); character >>= 6;
                case 1 : bytes[0] = static_cast<Uint8> (character | UTF8FirstBytes[bytesToWrite]);
            }

            // Add them to the output
            const Uint8* currentByte = bytes;
            switch (bytesToWrite)
            {
                case 4 : *output++ = *currentByte++;
                case 3 : *output++ = *currentByte++;
                case 2 : *output++ = *currentByte++;
                case 1 : *output++ = *currentByte++;
            }
        }
    }

    return output;
}


////////////////////////////////////////////////////////////
/// Generic function to convert an UTF-16 characters range
/// to an UTF-32 characters range, using the given locale
////////////////////////////////////////////////////////////
template <typename In, typename Out>
inline Out Unicode::UTF16ToUTF32(In begin, In end, Out output, Uint32 replacement)
{
    while (begin < end)
    {
        Uint16 character = *begin++;
        if ((character >= 0xD800) && (character <= 0xDBFF))
        {
            // We have a surrogate pair, ie. a character composed of two elements
            if (begin < end)
            {
                Uint16 d = *begin++;
                if ((d >= 0xDC00) && (d <= 0xDFFF))
                {
                    // The second element is valid : convert the two elements to a UTF-32 character
                    *output++ = static_cast<Uint32>(((character - 0xD800) << 10) + (d - 0xDC00) + 0x0010000);
                }
                else
                {
                    // Invalid second element
                    if (replacement)
                        *output++ = replacement;
                }
            }
        }
        else if ((character >= 0xDC00) && (character <= 0xDFFF))
        {
            // Invalid character
            if (replacement)
                *output++ = replacement;
        }
        else
        {
            // Valid character directly convertible to UTF-32
            *output++ = static_cast<Uint32>(character);
        }
    }

    return output;
}


////////////////////////////////////////////////////////////
/// Generic function to convert an UTF-32 characters range
/// to an UTF-8 characters range, using the given locale
////////////////////////////////////////////////////////////
template <typename In, typename Out>
inline Out Unicode::UTF32ToUTF8(In begin, In end, Out output, Uint8 replacement)
{
    while (begin < end)
    {
        Uint32 character = *begin++;
        if (character > 0x0010FFFF)
        {
            // Invalid character (greater than the maximum unicode value)
            if (replacement)
                *output++ = replacement;
        }
        else
        {
            // Valid character

            // Get number of bytes to write
            int bytesToWrite = 1;
            if      (character <  0x80)       bytesToWrite = 1;
            else if (character <  0x800)      bytesToWrite = 2;
            else if (character <  0x10000)    bytesToWrite = 3;
            else if (character <= 0x0010FFFF) bytesToWrite = 4;

            // Extract bytes to write
            Uint8 bytes[4];
            switch (bytesToWrite)
            {
                case 4 : bytes[3] = static_cast<Uint8>((character | 0x80) & 0xBF); character >>= 6;
                case 3 : bytes[2] = static_cast<Uint8>((character | 0x80) & 0xBF); character >>= 6;
                case 2 : bytes[1] = static_cast<Uint8>((character | 0x80) & 0xBF); character >>= 6;
                case 1 : bytes[0] = static_cast<Uint8> (character | UTF8FirstBytes[bytesToWrite]);
            }

            // Add them to the output
            const Uint8* currentByte = bytes;
            switch (bytesToWrite)
            {
                case 4 : *output++ = *currentByte++;
                case 3 : *output++ = *currentByte++;
                case 2 : *output++ = *currentByte++;
                case 1 : *output++ = *currentByte++;
            }
        }
    }

    return output;
}


////////////////////////////////////////////////////////////
/// Generic function to convert an UTF-32 characters range
/// to an UTF-16 characters range, using the given locale
////////////////////////////////////////////////////////////
template <typename In, typename Out>
inline Out Unicode::UTF32ToUTF16(In begin, In end, Out output, Uint16 replacement)
{
    while (begin < end)
    {
        Uint32 character = *begin++;
        if (character < 0xFFFF)
        {
            // Character can be converted directly to 16 bits, just need to check it's in the valid range
            if ((character >= 0xD800) && (character <= 0xDFFF))
            {
                // Invalid character (this range is reserved)
                if (replacement)
                    *output++ = replacement;
            }
            else
            {
                // Valid character directly convertible to 16 bits
                *output++ = static_cast<Uint16>(character);
            }
        }
        else if (character > 0x0010FFFF)
        {
            // Invalid character (greater than the maximum unicode value)
            if (replacement)
                *output++ = replacement;
        }
        else
        {
            // Character will be converted to 2 UTF-16 elements
            character -= 0x0010000;
            *output++ = static_cast<Uint16>((character >> 10)     + 0xD800);
            *output++ = static_cast<Uint16>((character & 0x3FFUL) + 0xDC00);
        }
    }

    return output;
}


////////////////////////////////////////////////////////////
/// Get the number of characters composing an UTF-8 string
////////////////////////////////////////////////////////////
template <typename In>
inline std::size_t Unicode::GetUTF8Length(In begin, In end)
{
    std::size_t length = 0;
    while (begin < end)
    {
        int nbBytes = UTF8TrailingBytes[static_cast<int>(*begin)];
        if (begin + nbBytes < end)
            ++length;

        begin += nbBytes + 1;
    }

    return length;
}


////////////////////////////////////////////////////////////
/// Get the number of characters composing an UTF-16 string
////////////////////////////////////////////////////////////
template <typename In>
inline std::size_t Unicode::GetUTF16Length(In begin, In end)
{
    std::size_t length = 0;
    while (begin < end)
    {
        if ((*begin >= 0xD800) && (*begin <= 0xDBFF))
        {
            ++begin;
            if ((begin < end) && ((*begin >= 0xDC00) && (*begin <= 0xDFFF)))
            {
                ++length;
            }
        }
        else
        {
            ++length;
        }

        ++begin;
    }

    return length;
}


////////////////////////////////////////////////////////////
/// Get the number of characters composing an UTF-32 string
////////////////////////////////////////////////////////////
template <typename In>
inline std::size_t Unicode::GetUTF32Length(In begin, In end)
{
    return end - begin;
}

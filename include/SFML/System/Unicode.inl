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
inline Out Unicode::UTF32ToANSI(In Begin, In End, Out Output, char Replacement, const std::locale& Locale)
{
    #ifdef __MINGW32__

        // MinGW has a almost no support for unicode stuff
        // As a consequence, the MinGW version of this function can only use the default locale
        // and ignores the one passed as parameter
        while (Begin < End)
        {
            char Char = 0;
            if (wctomb(&Char, static_cast<wchar_t>(*Begin++)) >= 0)
                *Output++ = Char;
            else if (Replacement)
                *Output++ = Replacement;
        }

    #else

        // Get the facet of the locale which deals with character conversion
        const std::ctype<wchar_t>& Facet = std::use_facet< std::ctype<wchar_t> >(Locale);

        // Use the facet to convert each character of the input string
        while (Begin < End)
            *Output++ = Facet.narrow(static_cast<wchar_t>(*Begin++), Replacement);

    #endif

    return Output;
}


////////////////////////////////////////////////////////////
/// Generic function to convert an ANSI characters range
/// to an UTF-32 characters range, using the given locale
////////////////////////////////////////////////////////////
template <typename In, typename Out>
inline Out Unicode::ANSIToUTF32(In Begin, In End, Out Output, const std::locale& Locale)
{
    #ifdef __MINGW32__

        // MinGW has a almost no support for unicode stuff
        // As a consequence, the MinGW version of this function can only use the default locale
        // and ignores the one passed as parameter
        while (Begin < End)
        {
            wchar_t Char = 0;
            mbtowc(&Char, &*Begin, 1);
            Begin++;
            *Output++ = static_cast<Uint32>(Char);
        }

    #else

        // Get the facet of the locale which deals with character conversion
        const std::ctype<wchar_t>& Facet = std::use_facet< std::ctype<wchar_t> >(Locale);

        // Use the facet to convert each character of the input string
        while (Begin < End)
            *Output++ = static_cast<Uint32>(Facet.widen(*Begin++));

    #endif

    return Output;
}


////////////////////////////////////////////////////////////
/// Generic function to convert an UTF-8 characters range
/// to an UTF-16 characters range, using the given locale
////////////////////////////////////////////////////////////
template <typename In, typename Out>
inline Out Unicode::UTF8ToUTF16(In Begin, In End, Out Output, Uint16 Replacement)
{
    while (Begin < End)
    {
        Uint32 c = 0;
        int TrailingBytes = UTF8TrailingBytes[static_cast<int>(*Begin)];
        if (Begin + TrailingBytes < End)
        {
            // First decode the UTF-8 character
            switch (TrailingBytes)
            {
                case 5 : c += *Begin++; c <<= 6;
                case 4 : c += *Begin++; c <<= 6;
                case 3 : c += *Begin++; c <<= 6;
                case 2 : c += *Begin++; c <<= 6;
                case 1 : c += *Begin++; c <<= 6;
                case 0 : c += *Begin++;
            }
	        c -= UTF8Offsets[TrailingBytes];

            // Then encode it in UTF-16
            if (c < 0xFFFF)
            {
                // Character can be converted directly to 16 bits, just need to check it's in the valid range
                if ((c >= 0xD800) && (c <= 0xDFFF))
                {
                    // Invalid character (this range is reserved)
                    if (Replacement)
                        *Output++ = Replacement;
                }
                else
                {
                    // Valid character directly convertible to 16 bits
                    *Output++ = static_cast<Uint16>(c);
                }
            }
            else if (c > 0x0010FFFF)
            {
                // Invalid character (greater than the maximum unicode value)
                if (Replacement)
                    *Output++ = Replacement;
            }
            else
            {
                // Character will be converted to 2 UTF-16 elements
                c -= 0x0010000;
                *Output++ = static_cast<Uint16>((c >> 10)     + 0xD800);
                *Output++ = static_cast<Uint16>((c & 0x3FFUL) + 0xDC00);
            }
        }
    }

    return Output;
}


////////////////////////////////////////////////////////////
/// Generic function to convert an UTF-8 characters range
/// to an UTF-32 characters range, using the given locale
////////////////////////////////////////////////////////////
template <typename In, typename Out>
inline Out Unicode::UTF8ToUTF32(In Begin, In End, Out Output, Uint32 Replacement)
{
    while (Begin < End)
    {
        Uint32 c = 0;
        int TrailingBytes = UTF8TrailingBytes[static_cast<int>(*Begin)];
        if (Begin + TrailingBytes < End)
        {
            // First decode the UTF-8 character
            switch (TrailingBytes)
            {
                case 5 : c += *Begin++; c <<= 6;
                case 4 : c += *Begin++; c <<= 6;
                case 3 : c += *Begin++; c <<= 6;
                case 2 : c += *Begin++; c <<= 6;
                case 1 : c += *Begin++; c <<= 6;
                case 0 : c += *Begin++;
            }
	        c -= UTF8Offsets[TrailingBytes];

            // Then write it if valid
            if ((c < 0xD800) || (c > 0xDFFF))
            {
                // Valid UTF-32 character
                *Output++ = c;
            }
            else
            {
                // Invalid UTF-32 character
                if (Replacement)
                    *Output++ = Replacement;
            }
        }
    }

    return Output;
}


////////////////////////////////////////////////////////////
/// Generic function to convert an UTF-16 characters range
/// to an UTF-8 characters range, using the given locale
////////////////////////////////////////////////////////////
template <typename In, typename Out>
inline Out Unicode::UTF16ToUTF8(In Begin, In End, Out Output, Uint8 Replacement)
{
    while (Begin < End)
    {
        Uint32 c = *Begin++;

        // If it's a surrogate pair, first convert to a single UTF-32 character
        if ((c >= 0xD800) && (c <= 0xDBFF))
        {
            if (Begin < End)
            {
                // The second element is valid : convert the two elements to a UTF-32 character
                Uint32 d = *Begin++;
                if ((d >= 0xDC00) && (d <= 0xDFFF))
                    c = static_cast<Uint32>(((c - 0xD800) << 10) + (d - 0xDC00) + 0x0010000);
            }
            else
            {
                // Invalid second element
                if (Replacement)
                    *Output++ = Replacement;
            }
        }

        // Then convert to UTF-8
        if (c > 0x0010FFFF)
        {
            // Invalid character (greater than the maximum unicode value)
            if (Replacement)
                *Output++ = Replacement;
        }
        else
        {
            // Valid character

            // Get number of bytes to write
            int BytesToWrite = 1;
            if      (c <  0x80)       BytesToWrite = 1;
            else if (c <  0x800)      BytesToWrite = 2;
            else if (c <  0x10000)    BytesToWrite = 3;
            else if (c <= 0x0010FFFF) BytesToWrite = 4;

            // Extract bytes to write
            Uint8 Bytes[4];
            switch (BytesToWrite)
            {
                case 4 : Bytes[3] = static_cast<Uint8>((c | 0x80) & 0xBF); c >>= 6;
                case 3 : Bytes[2] = static_cast<Uint8>((c | 0x80) & 0xBF); c >>= 6;
                case 2 : Bytes[1] = static_cast<Uint8>((c | 0x80) & 0xBF); c >>= 6;
                case 1 : Bytes[0] = static_cast<Uint8> (c | UTF8FirstBytes[BytesToWrite]);
            }

            // Add them to the output
            const Uint8* CurByte = Bytes;
            switch (BytesToWrite)
            {
                case 4 : *Output++ = *CurByte++;
                case 3 : *Output++ = *CurByte++;
                case 2 : *Output++ = *CurByte++;
                case 1 : *Output++ = *CurByte++;
            }
        }
    }

    return Output;
}


////////////////////////////////////////////////////////////
/// Generic function to convert an UTF-16 characters range
/// to an UTF-32 characters range, using the given locale
////////////////////////////////////////////////////////////
template <typename In, typename Out>
inline Out Unicode::UTF16ToUTF32(In Begin, In End, Out Output, Uint32 Replacement)
{
    while (Begin < End)
    {
        Uint16 c = *Begin++;
        if ((c >= 0xD800) && (c <= 0xDBFF))
        {
            // We have a surrogate pair, ie. a character composed of two elements
            if (Begin < End)
            {
                Uint16 d = *Begin++;
                if ((d >= 0xDC00) && (d <= 0xDFFF))
                {
                    // The second element is valid : convert the two elements to a UTF-32 character
                    *Output++ = static_cast<Uint32>(((c - 0xD800) << 10) + (d - 0xDC00) + 0x0010000);
                }
                else
                {
                    // Invalid second element
                    if (Replacement)
                        *Output++ = Replacement;
                }
            }
        }
        else if ((c >= 0xDC00) && (c <= 0xDFFF))
        {
            // Invalid character
            if (Replacement)
                *Output++ = Replacement;
        }
        else
        {
            // Valid character directly convertible to UTF-32
            *Output++ = static_cast<Uint32>(c);
        }
    }

    return Output;
}


////////////////////////////////////////////////////////////
/// Generic function to convert an UTF-32 characters range
/// to an UTF-8 characters range, using the given locale
////////////////////////////////////////////////////////////
template <typename In, typename Out>
inline Out Unicode::UTF32ToUTF8(In Begin, In End, Out Output, Uint8 Replacement)
{
    while (Begin < End)
    {
        Uint32 c = *Begin++;
        if (c > 0x0010FFFF)
        {
            // Invalid character (greater than the maximum unicode value)
            if (Replacement)
                *Output++ = Replacement;
        }
        else
        {
            // Valid character

            // Get number of bytes to write
            int BytesToWrite = 1;
            if      (c <  0x80)       BytesToWrite = 1;
            else if (c <  0x800)      BytesToWrite = 2;
            else if (c <  0x10000)    BytesToWrite = 3;
            else if (c <= 0x0010FFFF) BytesToWrite = 4;

            // Extract bytes to write
            Uint8 Bytes[4];
            switch (BytesToWrite)
            {
                case 4 : Bytes[3] = static_cast<Uint8>((c | 0x80) & 0xBF); c >>= 6;
                case 3 : Bytes[2] = static_cast<Uint8>((c | 0x80) & 0xBF); c >>= 6;
                case 2 : Bytes[1] = static_cast<Uint8>((c | 0x80) & 0xBF); c >>= 6;
                case 1 : Bytes[0] = static_cast<Uint8> (c | UTF8FirstBytes[BytesToWrite]);
            }

            // Add them to the output
            const Uint8* CurByte = Bytes;
            switch (BytesToWrite)
            {
                case 4 : *Output++ = *CurByte++;
                case 3 : *Output++ = *CurByte++;
                case 2 : *Output++ = *CurByte++;
                case 1 : *Output++ = *CurByte++;
            }
        }
    }

    return Output;
}


////////////////////////////////////////////////////////////
/// Generic function to convert an UTF-32 characters range
/// to an UTF-16 characters range, using the given locale
////////////////////////////////////////////////////////////
template <typename In, typename Out>
inline Out Unicode::UTF32ToUTF16(In Begin, In End, Out Output, Uint16 Replacement)
{
    while (Begin < End)
    {
        Uint32 c = *Begin++;
        if (c < 0xFFFF)
        {
            // Character can be converted directly to 16 bits, just need to check it's in the valid range
            if ((c >= 0xD800) && (c <= 0xDFFF))
            {
                // Invalid character (this range is reserved)
                if (Replacement)
                    *Output++ = Replacement;
            }
            else
            {
                // Valid character directly convertible to 16 bits
                *Output++ = static_cast<Uint16>(c);
            }
        }
        else if (c > 0x0010FFFF)
        {
            // Invalid character (greater than the maximum unicode value)
            if (Replacement)
                *Output++ = Replacement;
        }
        else
        {
            // Character will be converted to 2 UTF-16 elements
            c -= 0x0010000;
            *Output++ = static_cast<Uint16>((c >> 10)     + 0xD800);
            *Output++ = static_cast<Uint16>((c & 0x3FFUL) + 0xDC00);
        }
    }

    return Output;
}


////////////////////////////////////////////////////////////
/// Get the number of characters composing an UTF-8 string
////////////////////////////////////////////////////////////
template <typename In>
inline std::size_t Unicode::GetUTF8Length(In Begin, In End)
{
    std::size_t Length = 0;
    while (Begin < End)
    {
        int NbBytes = UTF8TrailingBytes[static_cast<int>(*Begin)];
        if (Begin + NbBytes < End)
            ++Length;

        Begin += NbBytes + 1;
    }

    return Length;
}


////////////////////////////////////////////////////////////
/// Get the number of characters composing an UTF-16 string
////////////////////////////////////////////////////////////
template <typename In>
inline std::size_t Unicode::GetUTF16Length(In Begin, In End)
{
    std::size_t Length = 0;
    while (Begin < End)
    {
        if ((*Begin >= 0xD800) && (*Begin <= 0xDBFF))
        {
            ++Begin;
            if ((Begin < End) && ((*Begin >= 0xDC00) && (*Begin <= 0xDFFF)))
            {
                ++Length;
            }
        }
        else
        {
            ++Length;
        }

        ++Begin;
    }

    return Length;
}


////////////////////////////////////////////////////////////
/// Get the number of characters composing an UTF-32 string
////////////////////////////////////////////////////////////
template <typename In>
inline std::size_t Unicode::GetUTF32Length(In Begin, In End)
{
    return End - Begin;
}

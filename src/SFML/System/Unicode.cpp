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
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Unicode.hpp>
#include <stdexcept>
#include <string.h>


////////////////////////////////////////////////////////////
// References :
//
// http://www.unicode.org/
// http://www.unicode.org/Public/PROGRAMS/CVTUTF/ConvertUTF.c
// http://www.unicode.org/Public/PROGRAMS/CVTUTF/ConvertUTF.h
// http://people.w3.org/rishida/scripts/uniview/conversion
//
////////////////////////////////////////////////////////////

namespace
{
    ////////////////////////////////////////////////////////////
    // Generic utility function to compute the number
    // of characters in a null-terminated string of any type
    ////////////////////////////////////////////////////////////
    template <typename T>
    std::size_t StrLen(const T* Str)
    {
        std::size_t Length = 0;
        while (*Str++) Length++;
        return Length;
    }

    ////////////////////////////////////////////////////////////
    // Get the current system locale
    ////////////////////////////////////////////////////////////
    std::locale GetCurrentLocale()
    {
        try
        {
            return std::locale("");
        }
        catch (std::runtime_error&)
        {
            // It seems some implementations don't know the "" locale
            // (Mac OS, MinGW)

            return std::locale();
        }
    }
}

namespace sf
{
////////////////////////////////////////////////////////////
// Static member data
////////////////////////////////////////////////////////////
const int Unicode::UTF8TrailingBytes[256] =
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
const Uint32 Unicode::UTF8Offsets[6] =
{
    0x00000000, 0x00003080, 0x000E2080, 0x03C82080, 0xFA082080, 0x82082080
};
const Uint8 Unicode::UTF8FirstBytes[7] =
{
    0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC
};


////////////////////////////////////////////////////////////
/// Default constructor (empty text)
////////////////////////////////////////////////////////////
Unicode::Text::Text()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
/// Construct the unicode text from any type of string
////////////////////////////////////////////////////////////
Unicode::Text::Text(const char* Str)
{
    if (Str)
    {
        std::size_t Length = StrLen(Str);
        if (Length > 0)
        {
            myUTF32String.reserve(Length + 1);
            Unicode::ANSIToUTF32(Str, Str + Length, std::back_inserter(myUTF32String));
        }
    }
}
Unicode::Text::Text(const wchar_t* Str)
{
    if (Str)
    {
        std::size_t Length = StrLen(Str);
        if (Length > 0)
        {
            // See comments below, in Unicode::Text::Text(const std::wstring&)
            myUTF32String.reserve(Length + 1);
            switch (sizeof(wchar_t))
            {
                case 2 : Unicode::UTF16ToUTF32(Str, Str + Length, std::back_inserter(myUTF32String), 0); break;
                case 4 : std::copy(Str, Str + Length, std::back_inserter(myUTF32String));                break;
                default : break;
            }
        }
    }
}
Unicode::Text::Text(const Uint8* Str)
{
    if (Str)
    {
        std::size_t Length = StrLen(Str);
        if (Length > 0)
        {
            myUTF32String.reserve(Length + 1);
            Unicode::UTF8ToUTF32(Str, Str + Length, std::back_inserter(myUTF32String), 0);
        }
    }
}
Unicode::Text::Text(const Uint16* Str)
{
    if (Str)
    {
        std::size_t Length = StrLen(Str);
        if (Length > 0)
        {
            myUTF32String.reserve(Length+ 1);
            Unicode::UTF16ToUTF32(Str, Str + Length, std::back_inserter(myUTF32String), 0);
        }
    }
}
Unicode::Text::Text(const Uint32* Str)
{
    if (Str)
        myUTF32String = Str;
}
Unicode::Text::Text(const std::string& Str)
{
    myUTF32String.reserve(Str.length() + 1);
    Unicode::ANSIToUTF32(Str.begin(), Str.end(), std::back_inserter(myUTF32String));
}
Unicode::Text::Text(const std::wstring& Str)
{
    // This function assumes that 2-byte large wchar_t are encoded in UTF-16 (Windows), and
    // 4-byte large wchar_t are encoded using UTF-32 (Unix)
    // Is that always true ? (some platforms may use JIS Japanese encoding)
    // The macro __STDC_ISO_10646__ should help identifying UTF-32 compliant implementations

    myUTF32String.reserve(Str.length() + 1);

    // Select the proper function according to the (supposed) wchar_t system encoding
    switch (sizeof(wchar_t))
    {
        // wchar_t uses UTF-16 -- need a conversion
        case 2 :
        {
            Unicode::UTF16ToUTF32(Str.begin(), Str.end(), std::back_inserter(myUTF32String), 0);
            break;
        }

        // wchar_t uses UTF-32 -- direct copy
        case 4 :
        {
            std::copy(Str.begin(), Str.end(), std::back_inserter(myUTF32String));
            break;
        }

        // This should never happen
        default : break;
    }
}
Unicode::Text::Text(const Unicode::UTF8String& Str)
{
    myUTF32String.reserve(Str.length() + 1);
    Unicode::UTF8ToUTF32(Str.begin(), Str.end(), std::back_inserter(myUTF32String), 0);
}
Unicode::Text::Text(const Unicode::UTF16String& Str)
{
    myUTF32String.reserve(Str.length() + 1);
    Unicode::UTF16ToUTF32(Str.begin(), Str.end(), std::back_inserter(myUTF32String), 0);
}
Unicode::Text::Text(const Unicode::UTF32String& Str)
{
    myUTF32String = Str;
}


////////////////////////////////////////////////////////////
/// Operator to cast the text to any type of string
////////////////////////////////////////////////////////////
Unicode::Text::operator std::string() const
{
    std::string Output;
    Output.reserve(myUTF32String.length() + 1);
    Unicode::UTF32ToANSI(myUTF32String.begin(), myUTF32String.end(), std::back_inserter(Output), 0, Unicode::GetDefaultLocale());
    return Output;
}
Unicode::Text::operator std::wstring() const
{
    // This function assumes that 2-byte large wchar_t are encoded in UTF-16 (Windows), and
    // 4-byte large wchar_t are encoded using UTF-32 (Unix)
    // Is that always true ? (some platforms may use JIS Japanese encoding)
    // The macro __STDC_ISO_10646__ should help identifying UTF-32 compliant implementations

    std::wstring Output;
    Output.reserve(myUTF32String.length() + 1);

    // Select the proper function according to the (supposed) wchar_t system encoding
    switch (sizeof(wchar_t))
    {
        // wchar_t uses UTF-16 -- need a conversion
        case 2 :
        {
            UTF32ToUTF16(myUTF32String.begin(), myUTF32String.end(), std::back_inserter(Output), 0);
            break;
        }

        // wchar_t uses UTF-32 -- direct copy
        case 4 :
        {
            std::copy(myUTF32String.begin(), myUTF32String.end(), std::back_inserter(Output));
            break;
        }

        // This should never happen
        default : break;
    }
    return Output;
}
Unicode::Text::operator sf::Unicode::UTF8String() const
{
    Unicode::UTF8String Output;
    Output.reserve(myUTF32String.length() * 4 + 1);
    Unicode::UTF32ToUTF8(myUTF32String.begin(), myUTF32String.end(), std::back_inserter(Output), 0);
    return Output;
}
Unicode::Text::operator sf::Unicode::UTF16String() const
{
    Unicode::UTF16String Output;
    Output.reserve(myUTF32String.length() * 2 + 1);
    Unicode::UTF32ToUTF16(myUTF32String.begin(), myUTF32String.end(), std::back_inserter(Output), 0);
    return Output;
}
Unicode::Text::operator const sf::Unicode::UTF32String&() const
{
    return myUTF32String;
}


////////////////////////////////////////////////////////////
/// Get the default system locale
////////////////////////////////////////////////////////////
const std::locale& Unicode::GetDefaultLocale()
{
    // It seems getting the default locale is a very expensive operation,
    // so we only do it once and then store the locale for reuse.
    // Warning : this code won't be aware of any change of the default locale during runtime

    static std::locale DefaultLocale = GetCurrentLocale();

    return DefaultLocale;
}

} // namespace sf

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
#include <SFML/System/String.hpp>
#include <SFML/System/Utf.hpp>
#include <iterator>
#include <cstring>


namespace sf
{
////////////////////////////////////////////////////////////
const std::size_t String::InvalidPos = std::basic_string<Uint32>::npos;


////////////////////////////////////////////////////////////
String::String()
{
}


////////////////////////////////////////////////////////////
String::String(char ansiChar, const std::locale& locale)
{
    myString += Utf32::DecodeAnsi(ansiChar, locale);
}


////////////////////////////////////////////////////////////
String::String(wchar_t wideChar)
{
    myString += Utf32::DecodeWide(wideChar);
}


////////////////////////////////////////////////////////////
String::String(Uint32 utf32Char)
{
    myString += utf32Char;
}


////////////////////////////////////////////////////////////
String::String(const char* ansiString, const std::locale& locale)
{
    if (ansiString)
    {
        std::size_t length = strlen(ansiString);
        if (length > 0)
        {
            myString.reserve(length + 1);
            Utf32::FromAnsi(ansiString, ansiString + length, std::back_inserter(myString), locale);
        }
    }
}


////////////////////////////////////////////////////////////
String::String(const std::string& ansiString, const std::locale& locale)
{
    myString.reserve(ansiString.length() + 1);
    Utf32::FromAnsi(ansiString.begin(), ansiString.end(), std::back_inserter(myString), locale);
}


////////////////////////////////////////////////////////////
String::String(const wchar_t* wideString)
{
    if (wideString)
    {
        std::size_t length = std::wcslen(wideString);
        if (length > 0)
        {
            myString.reserve(length + 1);
            Utf32::FromWide(wideString, wideString + length, std::back_inserter(myString));
        }
    }
}


////////////////////////////////////////////////////////////
String::String(const std::wstring& wideString)
{
    myString.reserve(wideString.length() + 1);
    Utf32::FromWide(wideString.begin(), wideString.end(), std::back_inserter(myString));
}


////////////////////////////////////////////////////////////
String::String(const Uint32* utf32String)
{
    if (utf32String)
        myString = utf32String;
}


////////////////////////////////////////////////////////////
String::String(const std::basic_string<Uint32>& utf32String) :
myString(utf32String)
{
}


////////////////////////////////////////////////////////////
String::String(const String& copy) :
myString(copy.myString)
{
}


////////////////////////////////////////////////////////////
String::operator std::string() const
{
    return ToAnsiString();
}


////////////////////////////////////////////////////////////
String::operator std::wstring() const
{
    return ToWideString();
}


////////////////////////////////////////////////////////////
std::string String::ToAnsiString(const std::locale& locale) const
{
    // Prepare the output string
    std::string output;
    output.reserve(myString.length() + 1);

    // Convert
    Utf32::ToAnsi(myString.begin(), myString.end(), std::back_inserter(output), 0, locale);

    return output;
}


////////////////////////////////////////////////////////////
std::wstring String::ToWideString() const
{
    // Prepare the output string
    std::wstring output;
    output.reserve(myString.length() + 1);

    // Convert
    Utf32::ToWide(myString.begin(), myString.end(), std::back_inserter(output), 0);

    return output;
}


////////////////////////////////////////////////////////////
String& String::operator =(const String& right)
{
    myString = right.myString;
    return *this;
}


////////////////////////////////////////////////////////////
String& String::operator +=(const String& right)
{
    myString += right.myString;
    return *this;
}


////////////////////////////////////////////////////////////
Uint32 String::operator [](std::size_t index) const
{
    return myString[index];
}


////////////////////////////////////////////////////////////
Uint32& String::operator [](std::size_t index)
{
    return myString[index];
}


////////////////////////////////////////////////////////////
void String::Clear()
{
    myString.clear();
}


////////////////////////////////////////////////////////////
std::size_t String::GetSize() const
{
    return myString.size();
}


////////////////////////////////////////////////////////////
bool String::IsEmpty() const
{
    return myString.empty();
}


////////////////////////////////////////////////////////////
void String::Erase(std::size_t position, std::size_t count)
{
    myString.erase(position, count);
}


////////////////////////////////////////////////////////////
void String::Insert(std::size_t position, const String& str)
{
    myString.insert(position, str.myString);
}


////////////////////////////////////////////////////////////
std::size_t String::Find(const String& str, std::size_t start) const
{
    return myString.find(str.myString, start);
}


////////////////////////////////////////////////////////////
const Uint32* String::GetData() const
{
    return myString.c_str();
}


////////////////////////////////////////////////////////////
String::Iterator String::Begin()
{
    return myString.begin();
}


////////////////////////////////////////////////////////////
String::ConstIterator String::Begin() const
{
    return myString.begin();
}


////////////////////////////////////////////////////////////
String::Iterator String::End()
{
    return myString.end();
}


////////////////////////////////////////////////////////////
String::ConstIterator String::End() const
{
    return myString.end();
}


////////////////////////////////////////////////////////////
bool operator ==(const String& left, const String& right)
{
    return left.myString == right.myString;
}


////////////////////////////////////////////////////////////
bool operator !=(const String& left, const String& right)
{
    return !(left == right);
}


////////////////////////////////////////////////////////////
bool operator <(const String& left, const String& right)
{
    return left.myString < right.myString;
}


////////////////////////////////////////////////////////////
bool operator >(const String& left, const String& right)
{
    return right < left;
}


////////////////////////////////////////////////////////////
bool operator <=(const String& left, const String& right)
{
    return !(right < left);
}


////////////////////////////////////////////////////////////
bool operator >=(const String& left, const String& right)
{
    return !(left < right);
}


////////////////////////////////////////////////////////////
String operator +(const String& left, const String& right)
{
    String string = left;
    string += right;

    return string;
}

} // namespace sf

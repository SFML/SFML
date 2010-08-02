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
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/SocketImpl.hpp>
#include <SFML/System/String.hpp>
#include <cstring>


namespace sf
{
////////////////////////////////////////////////////////////
Packet::Packet() :
myReadPos(0),
myIsValid(true)
{

}


////////////////////////////////////////////////////////////
Packet::~Packet()
{

}


////////////////////////////////////////////////////////////
void Packet::Append(const void* data, std::size_t sizeInBytes)
{
    if (data && (sizeInBytes > 0))
    {
        std::size_t start = myData.size();
        myData.resize(start + sizeInBytes);
        std::memcpy(&myData[start], data, sizeInBytes);
    }
}


////////////////////////////////////////////////////////////
void Packet::Clear()
{
    myData.clear();
    myReadPos = 0;
    myIsValid = true;
}


////////////////////////////////////////////////////////////
const char* Packet::GetData() const
{
    return !myData.empty() ? &myData[0] : NULL;
}


////////////////////////////////////////////////////////////
std::size_t Packet::GetDataSize() const
{
    return myData.size();
}


////////////////////////////////////////////////////////////
bool Packet::EndOfPacket() const
{
    return myReadPos >= myData.size();
}


////////////////////////////////////////////////////////////
Packet::operator bool() const
{
    return myIsValid;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(bool& data)
{
    Uint8 value;
    if (*this >> value)
        data = (value != 0);

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(Int8& data)
{
    if (CheckSize(sizeof(data)))
    {
        data = *reinterpret_cast<const Int8*>(GetData() + myReadPos);
        myReadPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(Uint8& data)
{
    if (CheckSize(sizeof(data)))
    {
        data = *reinterpret_cast<const Uint8*>(GetData() + myReadPos);
        myReadPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(Int16& data)
{
    if (CheckSize(sizeof(data)))
    {
        data = ntohs(*reinterpret_cast<const Int16*>(GetData() + myReadPos));
        myReadPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(Uint16& data)
{
    if (CheckSize(sizeof(data)))
    {
        data = ntohs(*reinterpret_cast<const Uint16*>(GetData() + myReadPos));
        myReadPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(Int32& data)
{
    if (CheckSize(sizeof(data)))
    {
        data = ntohl(*reinterpret_cast<const Int32*>(GetData() + myReadPos));
        myReadPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(Uint32& data)
{
    if (CheckSize(sizeof(data)))
    {
        data = ntohl(*reinterpret_cast<const Uint32*>(GetData() + myReadPos));
        myReadPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(float& data)
{
    if (CheckSize(sizeof(data)))
    {
        data = *reinterpret_cast<const float*>(GetData() + myReadPos);
        myReadPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(double& data)
{
    if (CheckSize(sizeof(data)))
    {
        data = *reinterpret_cast<const double*>(GetData() + myReadPos);
        myReadPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(char* data)
{
    // First extract string length
    Uint32 length = 0;
    *this >> length;

    if ((length > 0) && CheckSize(length))
    {
        // Then extract characters
        std::memcpy(data, GetData() + myReadPos, length);
        data[length] = '\0';

        // Update reading position
        myReadPos += length;
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(std::string& data)
{
    // First extract string length
    Uint32 length = 0;
    *this >> length;

    data.clear();
    if ((length > 0) && CheckSize(length))
    {
        // Then extract characters
        data.assign(GetData() + myReadPos, length);

        // Update reading position
        myReadPos += length;
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(wchar_t* data)
{
    // First extract string length
    Uint32 length = 0;
    *this >> length;

    if ((length > 0) && CheckSize(length * sizeof(Uint32)))
    {
        // Then extract characters
        for (Uint32 i = 0; i < length; ++i)
        {
            Uint32 character = 0;
            *this >> character;
            data[i] = static_cast<wchar_t>(character);
        }
        data[length] = L'\0';
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(std::wstring& data)
{
    // First extract string length
    Uint32 length = 0;
    *this >> length;

    data.clear();
    if ((length > 0) && CheckSize(length * sizeof(Uint32)))
    {
        // Then extract characters
        for (Uint32 i = 0; i < length; ++i)
        {
            Uint32 character = 0;
            *this >> character;
            data += static_cast<wchar_t>(character);
        }
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator >>(String& data)
{
    // First extract the string length
    Uint32 length = 0;
    *this >> length;

    data.Clear();
    if ((length > 0) && CheckSize(length * sizeof(Uint32)))
    {
        // Then extract characters
        for (Uint32 i = 0; i < length; ++i)
        {
            Uint32 character = 0;
            *this >> character;
            data += character;
        }
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(bool data)
{
    *this << static_cast<Uint8>(data);
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(Int8 data)
{
    Append(&data, sizeof(data));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(Uint8 data)
{
    Append(&data, sizeof(data));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(Int16 data)
{
    Int16 toWrite = htons(data);
    Append(&toWrite, sizeof(toWrite));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(Uint16 data)
{
    Uint16 toWrite = htons(data);
    Append(&toWrite, sizeof(toWrite));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(Int32 data)
{
    Int32 toWrite = htonl(data);
    Append(&toWrite, sizeof(toWrite));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(Uint32 data)
{
    Uint32 toWrite = htonl(data);
    Append(&toWrite, sizeof(toWrite));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(float data)
{
    Append(&data, sizeof(data));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(double data)
{
    Append(&data, sizeof(data));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(const char* data)
{
    // First insert string length
    Uint32 length = 0;
    for (const char* c = data; *c != '\0'; ++c)
        ++length;
    *this << length;

    // Then insert characters
    Append(data, length * sizeof(char));

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(const std::string& data)
{
    // First insert string length
    Uint32 length = static_cast<Uint32>(data.size());
    *this << length;

    // Then insert characters
    if (length > 0)
    {
        Append(data.c_str(), length * sizeof(std::string::value_type));
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(const wchar_t* data)
{
    // First insert string length
    Uint32 length = 0;
    for (const wchar_t* c = data; *c != L'\0'; ++c)
        ++length;
    *this << length;

    // Then insert characters
    for (const wchar_t* c = data; *c != L'\0'; ++c)
        *this << static_cast<Uint32>(*c);

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(const std::wstring& data)
{
    // First insert string length
    Uint32 length = static_cast<Uint32>(data.size());
    *this << length;

    // Then insert characters
    if (length > 0)
    {
        for (std::wstring::const_iterator c = data.begin(); c != data.end(); ++c)
            *this << static_cast<Uint32>(*c);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator <<(const String& data)
{
    // First insert the string length
    Uint32 length = static_cast<Uint32>(data.GetSize());
    *this << length;

    // Then insert characters
    if (length > 0)
    {
        for (String::ConstIterator c = data.Begin(); c != data.End(); ++c)
            *this << *c;
    }

    return *this;
}


////////////////////////////////////////////////////////////
bool Packet::CheckSize(std::size_t size)
{
    myIsValid = myIsValid && (myReadPos + size <= myData.size());

    return myIsValid;
}


////////////////////////////////////////////////////////////
const char* Packet::OnSend(std::size_t& size)
{
    size = GetDataSize();
    return GetData();
}


////////////////////////////////////////////////////////////
void Packet::OnReceive(const char* data, std::size_t size)
{
    Append(data, size);
}

} // namespace sf

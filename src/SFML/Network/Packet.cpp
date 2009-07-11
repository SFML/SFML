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
#include <SFML/Network/SocketHelper.hpp>
#include <string.h>


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Packet::Packet() :
myReadPos(0),
myIsValid(true)
{

}


////////////////////////////////////////////////////////////
/// Virtual destructor
////////////////////////////////////////////////////////////
Packet::~Packet()
{

}


////////////////////////////////////////////////////////////
/// Append data to the end of the packet
////////////////////////////////////////////////////////////
void Packet::Append(const void* data, std::size_t sizeInBytes)
{
    if (data && (sizeInBytes > 0))
    {
        std::size_t start = myData.size();
        myData.resize(start + sizeInBytes);
        memcpy(&myData[start], data, sizeInBytes);
    }
}


////////////////////////////////////////////////////////////
/// Clear the packet data
////////////////////////////////////////////////////////////
void Packet::Clear()
{
    myData.clear();
    myReadPos = 0;
    myIsValid = true;
}


////////////////////////////////////////////////////////////
/// Get a pointer to the data contained in the packet
/// Warning : the returned pointer may be invalid after you
/// append data to the packet
////////////////////////////////////////////////////////////
const char* Packet::GetData() const
{
    return !myData.empty() ? &myData[0] : NULL;
}


////////////////////////////////////////////////////////////
/// Get the size of the data contained in the packet
////////////////////////////////////////////////////////////
std::size_t Packet::GetDataSize() const
{
    return myData.size();
}


////////////////////////////////////////////////////////////
/// Tell if the reading position has reached the end of the packet
////////////////////////////////////////////////////////////
bool Packet::EndOfPacket() const
{
    return myReadPos >= myData.size();
}


////////////////////////////////////////////////////////////
/// Tell if the packet is valid for reading
////////////////////////////////////////////////////////////
Packet::operator bool() const
{
    return myIsValid;
}


////////////////////////////////////////////////////////////
/// Operator >> overloads to extract data from the packet
////////////////////////////////////////////////////////////
Packet& Packet::operator >>(bool& data)
{
    Uint8 value;
    if (*this >> value)
        data = (value != 0);

    return *this;
}
Packet& Packet::operator >>(Int8& data)
{
    if (CheckSize(sizeof(data)))
    {
        data = *reinterpret_cast<const Int8*>(GetData() + myReadPos);
        myReadPos += sizeof(data);
    }

    return *this;
}
Packet& Packet::operator >>(Uint8& data)
{
    if (CheckSize(sizeof(data)))
    {
        data = *reinterpret_cast<const Uint8*>(GetData() + myReadPos);
        myReadPos += sizeof(data);
    }

    return *this;
}
Packet& Packet::operator >>(Int16& data)
{
    if (CheckSize(sizeof(data)))
    {
        data = ntohs(*reinterpret_cast<const Int16*>(GetData() + myReadPos));
        myReadPos += sizeof(data);
    }

    return *this;
}
Packet& Packet::operator >>(Uint16& data)
{
    if (CheckSize(sizeof(data)))
    {
        data = ntohs(*reinterpret_cast<const Uint16*>(GetData() + myReadPos));
        myReadPos += sizeof(data);
    }

    return *this;
}
Packet& Packet::operator >>(Int32& data)
{
    if (CheckSize(sizeof(data)))
    {
        data = ntohl(*reinterpret_cast<const Int32*>(GetData() + myReadPos));
        myReadPos += sizeof(data);
    }

    return *this;
}
Packet& Packet::operator >>(Uint32& data)
{
    if (CheckSize(sizeof(data)))
    {
        data = ntohl(*reinterpret_cast<const Uint32*>(GetData() + myReadPos));
        myReadPos += sizeof(data);
    }

    return *this;
}
Packet& Packet::operator >>(float& data)
{
    if (CheckSize(sizeof(data)))
    {
        data = *reinterpret_cast<const float*>(GetData() + myReadPos);
        myReadPos += sizeof(data);
    }

    return *this;
}
Packet& Packet::operator >>(double& data)
{
    if (CheckSize(sizeof(data)))
    {
        data = *reinterpret_cast<const double*>(GetData() + myReadPos);
        myReadPos += sizeof(data);
    }

    return *this;
}
Packet& Packet::operator >>(char* data)
{
    // First extract string length
    Uint32 length;
    *this >> length;

    if ((length > 0) && CheckSize(length))
    {
        // Then extract characters
        memcpy(data, GetData() + myReadPos, length);
        data[length] = '\0';

        // Update reading position
        myReadPos += length;
    }

    return *this;
}
Packet& Packet::operator >>(std::string& data)
{
    // First extract string length
    Uint32 length;
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
Packet& Packet::operator >>(wchar_t* data)
{
    // First extract string length
    Uint32 length;
    *this >> length;

    if ((length > 0) && CheckSize(length * sizeof(Int32)))
    {
        // Then extract characters
        for (Uint32 i = 0; i < length; ++i)
        {
            Uint32 character;
            *this >> character;
            data[i] = static_cast<wchar_t>(character);
        }
        data[length] = L'\0';
    }

    return *this;
}
Packet& Packet::operator >>(std::wstring& data)
{
    // First extract string length
    Uint32 length;
    *this >> length;

    data.clear();
    if ((length > 0) && CheckSize(length * sizeof(Int32)))
    {
        // Then extract characters
        for (Uint32 i = 0; i < length; ++i)
        {
            Uint32 character;
            *this >> character;
            data += static_cast<wchar_t>(character);
        }
    }

    return *this;
}


////////////////////////////////////////////////////////////
/// Operator << overloads to put data into the packet
////////////////////////////////////////////////////////////
Packet& Packet::operator <<(bool data)
{
    *this << static_cast<Uint8>(data);
    return *this;
}
Packet& Packet::operator <<(Int8 data)
{
    Append(&data, sizeof(data));
    return *this;
}
Packet& Packet::operator <<(Uint8 data)
{
    Append(&data, sizeof(data));
    return *this;
}
Packet& Packet::operator <<(Int16 data)
{
    Int16 toWrite = htons(data);
    Append(&toWrite, sizeof(toWrite));
    return *this;
}
Packet& Packet::operator <<(Uint16 data)
{
    Uint16 toWrite = htons(data);
    Append(&toWrite, sizeof(toWrite));
    return *this;
}
Packet& Packet::operator <<(Int32 data)
{
    Int32 toWrite = htonl(data);
    Append(&toWrite, sizeof(toWrite));
    return *this;
}
Packet& Packet::operator <<(Uint32 data)
{
    Uint32 toWrite = htonl(data);
    Append(&toWrite, sizeof(toWrite));
    return *this;
}
Packet& Packet::operator <<(float data)
{
    Append(&data, sizeof(data));
    return *this;
}
Packet& Packet::operator <<(double data)
{
    Append(&data, sizeof(data));
    return *this;
}
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
Packet& Packet::operator <<(const wchar_t* data)
{
    // First insert string length
    Uint32 length = 0;
    for (const wchar_t* c = data; *c != L'\0'; ++c)
        ++length;
    *this << length;

    // Then insert characters
    for (const wchar_t* c = data; *c != L'\0'; ++c)
        *this << static_cast<Int32>(*c);

    return *this;
}
Packet& Packet::operator <<(const std::wstring& data)
{
    // First insert string length
    Uint32 length = static_cast<Uint32>(data.size());
    *this << length;

    // Then insert characters
    if (length > 0)
    {
        for (std::wstring::const_iterator c = data.begin(); c != data.end(); ++c)
            *this << static_cast<Int32>(*c);
    }

    return *this;
}


////////////////////////////////////////////////////////////
/// Check if the packet can extract a given size of bytes
////////////////////////////////////////////////////////////
bool Packet::CheckSize(std::size_t size)
{
    myIsValid = myIsValid && (myReadPos + size <= myData.size());

    return myIsValid;
}


////////////////////////////////////////////////////////////
/// Called before the packet is sent to the network
////////////////////////////////////////////////////////////
const char* Packet::OnSend(std::size_t& dataSize)
{
    dataSize = GetDataSize();
    return GetData();
}


////////////////////////////////////////////////////////////
/// Called after the packet has been received from the network
////////////////////////////////////////////////////////////
void Packet::OnReceive(const char* data, std::size_t dataSize)
{
    Append(data, dataSize);
}

} // namespace sf

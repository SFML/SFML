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
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/SocketImpl.hpp>
#include <SFML/System/String.hpp>

#include <cstring>
#include <cwchar>


namespace sf
{
////////////////////////////////////////////////////////////
Packet::Packet() : m_readPos(0), m_sendPos(0), m_isValid(true)
{
}


////////////////////////////////////////////////////////////
Packet::~Packet() = default;


////////////////////////////////////////////////////////////
Packet::Packet(const Packet&) = default;


////////////////////////////////////////////////////////////
Packet& Packet::operator=(const Packet&) = default;


////////////////////////////////////////////////////////////
Packet::Packet(Packet&&) noexcept = default;


////////////////////////////////////////////////////////////
Packet& Packet::operator=(Packet&&) noexcept = default;


////////////////////////////////////////////////////////////
void Packet::append(const void* data, std::size_t sizeInBytes)
{
    if (data && (sizeInBytes > 0))
    {
        std::size_t start = m_data.size();
        m_data.resize(start + sizeInBytes);
        std::memcpy(&m_data[start], data, sizeInBytes);
    }
}


////////////////////////////////////////////////////////////
std::size_t Packet::getReadPosition() const
{
    return m_readPos;
}


////////////////////////////////////////////////////////////
void Packet::clear()
{
    m_data.clear();
    m_readPos = 0;
    m_isValid = true;
}


////////////////////////////////////////////////////////////
const void* Packet::getData() const
{
    return !m_data.empty() ? m_data.data() : nullptr;
}


////////////////////////////////////////////////////////////
std::size_t Packet::getDataSize() const
{
    return m_data.size();
}


////////////////////////////////////////////////////////////
bool Packet::endOfPacket() const
{
    return m_readPos >= m_data.size();
}


////////////////////////////////////////////////////////////
Packet::operator bool() const
{
    return m_isValid;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator>>(bool& data)
{
    std::uint8_t value;
    if (*this >> value)
        data = (value != 0);

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator>>(std::int8_t& data)
{
    if (checkSize(sizeof(data)))
    {
        std::memcpy(&data, &m_data[m_readPos], sizeof(data));
        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator>>(std::uint8_t& data)
{
    if (checkSize(sizeof(data)))
    {
        std::memcpy(&data, &m_data[m_readPos], sizeof(data));
        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator>>(std::int16_t& data)
{
    if (checkSize(sizeof(data)))
    {
        std::memcpy(&data, &m_data[m_readPos], sizeof(data));
        data = static_cast<std::int16_t>(ntohs(static_cast<std::uint16_t>(data)));
        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator>>(std::uint16_t& data)
{
    if (checkSize(sizeof(data)))
    {
        std::memcpy(&data, &m_data[m_readPos], sizeof(data));
        data = ntohs(data);
        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator>>(std::int32_t& data)
{
    if (checkSize(sizeof(data)))
    {
        std::memcpy(&data, &m_data[m_readPos], sizeof(data));
        data = static_cast<std::int32_t>(ntohl(static_cast<std::uint32_t>(data)));
        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator>>(std::uint32_t& data)
{
    if (checkSize(sizeof(data)))
    {
        std::memcpy(&data, &m_data[m_readPos], sizeof(data));
        data = ntohl(data);
        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator>>(std::int64_t& data)
{
    if (checkSize(sizeof(data)))
    {
        // Since ntohll is not available everywhere, we have to convert
        // to network byte order (big endian) manually
        std::uint8_t bytes[sizeof(data)];
        std::memcpy(bytes, &m_data[m_readPos], sizeof(data));

        data = (static_cast<std::int64_t>(bytes[0]) << 56) | (static_cast<std::int64_t>(bytes[1]) << 48) |
               (static_cast<std::int64_t>(bytes[2]) << 40) | (static_cast<std::int64_t>(bytes[3]) << 32) |
               (static_cast<std::int64_t>(bytes[4]) << 24) | (static_cast<std::int64_t>(bytes[5]) << 16) |
               (static_cast<std::int64_t>(bytes[6]) << 8) | (static_cast<std::int64_t>(bytes[7]));

        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator>>(std::uint64_t& data)
{
    if (checkSize(sizeof(data)))
    {
        // Since ntohll is not available everywhere, we have to convert
        // to network byte order (big endian) manually
        std::uint8_t bytes[sizeof(data)];
        std::memcpy(bytes, &m_data[m_readPos], sizeof(data));

        data = (static_cast<std::uint64_t>(bytes[0]) << 56) | (static_cast<std::uint64_t>(bytes[1]) << 48) |
               (static_cast<std::uint64_t>(bytes[2]) << 40) | (static_cast<std::uint64_t>(bytes[3]) << 32) |
               (static_cast<std::uint64_t>(bytes[4]) << 24) | (static_cast<std::uint64_t>(bytes[5]) << 16) |
               (static_cast<std::uint64_t>(bytes[6]) << 8) | (static_cast<std::uint64_t>(bytes[7]));

        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator>>(float& data)
{
    if (checkSize(sizeof(data)))
    {
        std::memcpy(&data, &m_data[m_readPos], sizeof(data));
        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator>>(double& data)
{
    if (checkSize(sizeof(data)))
    {
        std::memcpy(&data, &m_data[m_readPos], sizeof(data));
        m_readPos += sizeof(data);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator>>(char* data)
{
    // First extract string length
    std::uint32_t length = 0;
    *this >> length;

    if ((length > 0) && checkSize(length))
    {
        // Then extract characters
        std::memcpy(data, &m_data[m_readPos], length);
        data[length] = '\0';

        // Update reading position
        m_readPos += length;
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator>>(std::string& data)
{
    // First extract string length
    std::uint32_t length = 0;
    *this >> length;

    data.clear();
    if ((length > 0) && checkSize(length))
    {
        // Then extract characters
        data.assign(&m_data[m_readPos], length);

        // Update reading position
        m_readPos += length;
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator>>(wchar_t* data)
{
    // First extract string length
    std::uint32_t length = 0;
    *this >> length;

    if ((length > 0) && checkSize(length * sizeof(std::uint32_t)))
    {
        // Then extract characters
        for (std::uint32_t i = 0; i < length; ++i)
        {
            std::uint32_t character = 0;
            *this >> character;
            data[i] = static_cast<wchar_t>(character);
        }
        data[length] = L'\0';
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator>>(std::wstring& data)
{
    // First extract string length
    std::uint32_t length = 0;
    *this >> length;

    data.clear();
    if ((length > 0) && checkSize(length * sizeof(std::uint32_t)))
    {
        // Then extract characters
        for (std::uint32_t i = 0; i < length; ++i)
        {
            std::uint32_t character = 0;
            *this >> character;
            data += static_cast<wchar_t>(character);
        }
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator>>(String& data)
{
    // First extract the string length
    std::uint32_t length = 0;
    *this >> length;

    data.clear();
    if ((length > 0) && checkSize(length * sizeof(std::uint32_t)))
    {
        // Then extract characters
        for (std::uint32_t i = 0; i < length; ++i)
        {
            std::uint32_t character = 0;
            *this >> character;
            data += character;
        }
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator<<(bool data)
{
    *this << static_cast<std::uint8_t>(data);
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator<<(std::int8_t data)
{
    append(&data, sizeof(data));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator<<(std::uint8_t data)
{
    append(&data, sizeof(data));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator<<(std::int16_t data)
{
    auto toWrite = static_cast<std::int16_t>(htons(static_cast<std::uint16_t>(data)));
    append(&toWrite, sizeof(toWrite));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator<<(std::uint16_t data)
{
    std::uint16_t toWrite = htons(data);
    append(&toWrite, sizeof(toWrite));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator<<(std::int32_t data)
{
    std::int32_t toWrite = static_cast<std::int32_t>(htonl(static_cast<std::uint32_t>(data)));
    append(&toWrite, sizeof(toWrite));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator<<(std::uint32_t data)
{
    std::uint32_t toWrite = htonl(data);
    append(&toWrite, sizeof(toWrite));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator<<(std::int64_t data)
{
    // Since htonll is not available everywhere, we have to convert
    // to network byte order (big endian) manually

    std::uint8_t toWrite[] = {static_cast<std::uint8_t>((data >> 56) & 0xFF),
                              static_cast<std::uint8_t>((data >> 48) & 0xFF),
                              static_cast<std::uint8_t>((data >> 40) & 0xFF),
                              static_cast<std::uint8_t>((data >> 32) & 0xFF),
                              static_cast<std::uint8_t>((data >> 24) & 0xFF),
                              static_cast<std::uint8_t>((data >> 16) & 0xFF),
                              static_cast<std::uint8_t>((data >> 8) & 0xFF),
                              static_cast<std::uint8_t>((data)&0xFF)};

    append(&toWrite, sizeof(toWrite));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator<<(std::uint64_t data)
{
    // Since htonll is not available everywhere, we have to convert
    // to network byte order (big endian) manually

    std::uint8_t toWrite[] = {static_cast<std::uint8_t>((data >> 56) & 0xFF),
                              static_cast<std::uint8_t>((data >> 48) & 0xFF),
                              static_cast<std::uint8_t>((data >> 40) & 0xFF),
                              static_cast<std::uint8_t>((data >> 32) & 0xFF),
                              static_cast<std::uint8_t>((data >> 24) & 0xFF),
                              static_cast<std::uint8_t>((data >> 16) & 0xFF),
                              static_cast<std::uint8_t>((data >> 8) & 0xFF),
                              static_cast<std::uint8_t>((data)&0xFF)};

    append(&toWrite, sizeof(toWrite));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator<<(float data)
{
    append(&data, sizeof(data));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator<<(double data)
{
    append(&data, sizeof(data));
    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator<<(const char* data)
{
    // First insert string length
    auto length = static_cast<std::uint32_t>(std::strlen(data));
    *this << length;

    // Then insert characters
    append(data, length * sizeof(char));

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator<<(const std::string& data)
{
    // First insert string length
    auto length = static_cast<std::uint32_t>(data.size());
    *this << length;

    // Then insert characters
    if (length > 0)
        append(data.c_str(), length * sizeof(std::string::value_type));

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator<<(const wchar_t* data)
{
    // First insert string length
    auto length = static_cast<std::uint32_t>(std::wcslen(data));
    *this << length;

    // Then insert characters
    for (const wchar_t* c = data; *c != L'\0'; ++c)
        *this << static_cast<std::uint32_t>(*c);

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator<<(const std::wstring& data)
{
    // First insert string length
    auto length = static_cast<std::uint32_t>(data.size());
    *this << length;

    // Then insert characters
    if (length > 0)
    {
        for (wchar_t c : data)
            *this << static_cast<std::uint32_t>(c);
    }

    return *this;
}


////////////////////////////////////////////////////////////
Packet& Packet::operator<<(const String& data)
{
    // First insert the string length
    auto length = static_cast<std::uint32_t>(data.getSize());
    *this << length;

    // Then insert characters
    if (length > 0)
    {
        for (unsigned int datum : data)
            *this << datum;
    }

    return *this;
}


////////////////////////////////////////////////////////////
bool Packet::checkSize(std::size_t size)
{
    m_isValid = m_isValid && (m_readPos + size <= m_data.size());

    return m_isValid;
}


////////////////////////////////////////////////////////////
const void* Packet::onSend(std::size_t& size)
{
    size = getDataSize();
    return getData();
}


////////////////////////////////////////////////////////////
void Packet::onReceive(const void* data, std::size_t size)
{
    append(data, size);
}

} // namespace sf

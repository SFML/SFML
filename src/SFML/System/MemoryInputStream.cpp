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
#include <SFML/System/MemoryInputStream.hpp>
#include <cstring>


namespace sf
{
////////////////////////////////////////////////////////////
MemoryInputStream::MemoryInputStream() :
m_data(),
m_offset(0)
{
}


////////////////////////////////////////////////////////////
void MemoryInputStream::open(Span<const std::byte> data)
{
    m_data = data;
    m_offset = 0;
}


////////////////////////////////////////////////////////////
Int64 MemoryInputStream::read(Span<std::byte> data)
{
    if (!m_data.data())
        return -1;

    auto endPosition = m_offset + data.size();
    auto count = endPosition <= m_data.size() ? data.size() : m_data.size() - m_offset;

    if (count > 0)
    {
        std::memcpy(data.data(), m_data.data() + m_offset, count);
        m_offset += count;
    }

    return static_cast<Int64>(count);
}


////////////////////////////////////////////////////////////
Int64 MemoryInputStream::seek(Int64 position)
{
    if (!m_data.data())
        return -1;

    m_offset = static_cast<std::size_t>(position) < m_data.size() ? static_cast<std::size_t>(position) : m_data.size();
    return static_cast<Int64>(m_offset);
}


////////////////////////////////////////////////////////////
Int64 MemoryInputStream::tell()
{
    if (!m_data.data())
        return -1;

    return static_cast<Int64>(m_offset);
}


////////////////////////////////////////////////////////////
Int64 MemoryInputStream::getSize()
{
    if (!m_data.data())
        return -1;

    return static_cast<Int64>(m_data.size());
}

} // namespace sf

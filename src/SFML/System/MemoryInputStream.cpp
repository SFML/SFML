////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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

#include <algorithm>

#include <cassert>
#include <cstring>


namespace sf
{
////////////////////////////////////////////////////////////
MemoryInputStream::MemoryInputStream(const void* data, std::size_t sizeInBytes) :
m_data(static_cast<const std::byte*>(data)),
m_size(sizeInBytes)
{
    assert(m_data && "MemoryInputStream must be initialized with non-null data");
}


////////////////////////////////////////////////////////////
std::optional<std::size_t> MemoryInputStream::read(void* data, std::size_t size)
{
    const std::size_t count = std::min(size, m_size - m_offset);
    if (count > 0)
    {
        std::memcpy(data, m_data + m_offset, static_cast<std::size_t>(count));
        m_offset += count;
    }

    return count;
}


////////////////////////////////////////////////////////////
std::optional<std::size_t> MemoryInputStream::seek(std::size_t position)
{
    m_offset = position < m_size ? position : m_size;
    return m_offset;
}


////////////////////////////////////////////////////////////
std::optional<std::size_t> MemoryInputStream::tell()
{
    return m_offset;
}


////////////////////////////////////////////////////////////
std::optional<std::size_t> MemoryInputStream::getSize()
{
    return m_size;
}

} // namespace sf

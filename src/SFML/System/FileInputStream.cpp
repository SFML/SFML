////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/System/FileInputStream.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
bool FileInputStream::open(const std::string& filename)
{
    m_file.open(filename.c_str(), std::ios::binary);
    return m_file.good();
}


////////////////////////////////////////////////////////////
Int64 FileInputStream::read(void* data, Int64 size)
{
    m_file.read(static_cast<char*>(data), size);
    return m_file.gcount();
}


////////////////////////////////////////////////////////////
Int64 FileInputStream::seek(Int64 position)
{
    if (m_file.eof())
        m_file.clear();
    m_file.seekg(position);
    return tell();
}


////////////////////////////////////////////////////////////
Int64 FileInputStream::tell()
{
    return m_file.tellg();
}


////////////////////////////////////////////////////////////
Int64 FileInputStream::getSize()
{
    std::ifstream::pos_type pos = m_file.tellg();
    m_file.seekg(0, std::ios::end);
    std::ifstream::pos_type size = m_file.tellg();
    m_file.seekg(pos);
    return size;
}

} // namespace sf

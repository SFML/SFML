////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2021 Laurent Gomila (laurent@sfml-dev.org)
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
#ifdef SFML_SYSTEM_ANDROID
#include <SFML/System/Android/ResourceStream.hpp>
#endif
#include <memory>
#include <cstddef>

namespace sf
{
////////////////////////////////////////////////////////////
#ifndef SFML_SYSTEM_ANDROID
void FileInputStream::FileCloser::operator()(std::FILE* file)
{
    std::fclose(file);
}
#endif


////////////////////////////////////////////////////////////
FileInputStream::FileInputStream() = default;


////////////////////////////////////////////////////////////
FileInputStream::~FileInputStream() = default;


////////////////////////////////////////////////////////////
FileInputStream::FileInputStream(FileInputStream&&) = default;


////////////////////////////////////////////////////////////
FileInputStream& FileInputStream::operator=(FileInputStream&&) = default;


////////////////////////////////////////////////////////////
bool FileInputStream::open(const std::string& filename)
{
#ifdef SFML_SYSTEM_ANDROID
    m_file = std::make_unique<priv::ResourceStream>(filename);
    return m_file->tell() != -1;
#else
    m_file.reset(std::fopen(filename.c_str(), "rb"));
    return m_file != nullptr;
#endif
}


////////////////////////////////////////////////////////////
Int64 FileInputStream::read(void* data, Int64 size)
{
#ifdef SFML_SYSTEM_ANDROID
    return m_file->read(data, size);
#else
    if (m_file)
        return static_cast<Int64>(std::fread(data, 1, static_cast<std::size_t>(size), m_file.get()));
    else
        return -1;
#endif
}


////////////////////////////////////////////////////////////
Int64 FileInputStream::seek(Int64 position)
{
#ifdef SFML_SYSTEM_ANDROID
    return m_file->seek(position);
#else
    if (m_file)
    {
        if (std::fseek(m_file.get(), static_cast<long>(position), SEEK_SET))
            return -1;

        return tell();
    }
    else
    {
        return -1;
    }
#endif
}


////////////////////////////////////////////////////////////
Int64 FileInputStream::tell()
{
#ifdef SFML_SYSTEM_ANDROID
    return m_file->tell();
#else
    if (m_file)
        return std::ftell(m_file.get());
    else
        return -1;
#endif
}


////////////////////////////////////////////////////////////
Int64 FileInputStream::getSize()
{
#ifdef SFML_SYSTEM_ANDROID
    return m_file->getSize();
#else
    if (m_file)
    {
        Int64 position = tell();
        std::fseek(m_file.get(), 0, SEEK_END);
        Int64 size = tell();

        if (seek(position) == -1)
            return -1;

        return size;
    }
    else
    {
        return -1;
    }
#endif
}

} // namespace sf

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
#include <SFML/System/FileInputStream.hpp>
#ifdef SFML_SYSTEM_ANDROID
#include <SFML/System/Android/ResourceStream.hpp>
#endif
#include <cstddef>
#include <memory>

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
bool FileInputStream::open(const std::filesystem::path& filename)
{
#ifdef SFML_SYSTEM_ANDROID
    m_file = std::make_unique<priv::ResourceStream>(filename);
    return m_file->tell() != -1;
#else
#ifdef SFML_SYSTEM_WINDOWS
    m_file.reset(_wfopen(filename.c_str(), L"rb"));
#else
    m_file.reset(std::fopen(filename.c_str(), "rb"));
#endif
    return m_file != nullptr;
#endif
}


////////////////////////////////////////////////////////////
std::int64_t FileInputStream::read(void* data, std::int64_t size)
{
#ifdef SFML_SYSTEM_ANDROID
    return m_file->read(data, size);
#else
    if (m_file)
        return static_cast<std::int64_t>(std::fread(data, 1, static_cast<std::size_t>(size), m_file.get()));
    else
        return -1;
#endif
}


////////////////////////////////////////////////////////////
std::int64_t FileInputStream::seek(std::int64_t position)
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
std::int64_t FileInputStream::tell()
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
std::int64_t FileInputStream::getSize()
{
#ifdef SFML_SYSTEM_ANDROID
    return m_file->getSize();
#else
    if (m_file)
    {
        std::int64_t position = tell();
        std::fseek(m_file.get(), 0, SEEK_END);
        std::int64_t size = tell();

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

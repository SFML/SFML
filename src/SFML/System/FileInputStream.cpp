////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/System/Exception.hpp>
#include <SFML/System/FileInputStream.hpp>
#include <SFML/System/Utils.hpp>
#ifdef SFML_SYSTEM_ANDROID
#include <SFML/System/Android/Activity.hpp>
#include <SFML/System/Android/ResourceStream.hpp>
#endif
#include <memory>

#include <cstddef>

namespace sf
{
////////////////////////////////////////////////////////////
void FileInputStream::FileCloser::operator()(std::FILE* file)
{
    std::fclose(file);
}


////////////////////////////////////////////////////////////
FileInputStream::FileInputStream() = default;


////////////////////////////////////////////////////////////
FileInputStream::FileInputStream(const std::filesystem::path& filename)
{
    if (!open(filename))
        throw sf::Exception("Failed to open file input stream");
}


////////////////////////////////////////////////////////////
FileInputStream::~FileInputStream() = default;


////////////////////////////////////////////////////////////
FileInputStream::FileInputStream(FileInputStream&&) noexcept = default;


////////////////////////////////////////////////////////////
FileInputStream& FileInputStream::operator=(FileInputStream&&) noexcept = default;


////////////////////////////////////////////////////////////
bool FileInputStream::open(const std::filesystem::path& filename)
{
#ifdef SFML_SYSTEM_ANDROID
    if (priv::getActivityStatesPtr() != nullptr)
    {
        m_androidFile = std::make_unique<priv::ResourceStream>();
        if (!m_androidFile->open(filename))
            return false;
        return m_androidFile->tell().has_value();
    }
#endif
    m_file.reset(openFile(filename, "rb"));
    return m_file != nullptr;
}


////////////////////////////////////////////////////////////
std::optional<std::size_t> FileInputStream::read(void* data, std::size_t size)
{
#ifdef SFML_SYSTEM_ANDROID
    if (priv::getActivityStatesPtr() != nullptr)
    {
        if (!m_androidFile)
            return std::nullopt;
        return m_androidFile->read(data, size);
    }
#endif
    if (!m_file)
        return std::nullopt;
    return std::fread(data, 1, size, m_file.get());
}


////////////////////////////////////////////////////////////
std::optional<std::size_t> FileInputStream::seek(std::size_t position)
{
#ifdef SFML_SYSTEM_ANDROID
    if (priv::getActivityStatesPtr() != nullptr)
    {
        if (!m_androidFile)
            return std::nullopt;
        return m_androidFile->seek(position);
    }
#endif
    if (!m_file)
        return std::nullopt;
    if (std::fseek(m_file.get(), static_cast<long>(position), SEEK_SET))
        return std::nullopt;

    return tell();
}


////////////////////////////////////////////////////////////
std::optional<std::size_t> FileInputStream::tell()
{
#ifdef SFML_SYSTEM_ANDROID
    if (priv::getActivityStatesPtr() != nullptr)
    {
        if (!m_androidFile)
            return std::nullopt;
        return m_androidFile->tell();
    }
#endif
    if (!m_file)
        return std::nullopt;
    const auto position = std::ftell(m_file.get());
    return position < 0 ? std::nullopt : std::optional<std::size_t>(position);
}


////////////////////////////////////////////////////////////
std::optional<std::size_t> FileInputStream::getSize()
{
#ifdef SFML_SYSTEM_ANDROID
    if (priv::getActivityStatesPtr() != nullptr)
    {
        if (!m_androidFile)
            return std::nullopt;
        return m_androidFile->getSize();
    }
#endif
    if (!m_file)
        return std::nullopt;
    const auto position = tell().value();
    std::fseek(m_file.get(), 0, SEEK_END);
    const std::optional size = tell();

    if (!seek(position).has_value())
        return std::nullopt;

    return size;
}

} // namespace sf

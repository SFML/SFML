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
#include <SFML/System/FileInputStream.hpp>
#ifdef SFML_SYSTEM_ANDROID
#include <SFML/System/Android/Activity.hpp>
#include <SFML/System/Android/ResourceStream.hpp>
#endif
#include <memory>

#include <cassert>
#include <cstddef>

namespace sf
{
////////////////////////////////////////////////////////////
void FileInputStream::FileCloser::operator()(std::FILE* file)
{
    std::fclose(file);
}


////////////////////////////////////////////////////////////
FileInputStream::~FileInputStream() = default;


////////////////////////////////////////////////////////////
FileInputStream::FileInputStream(FileInputStream&&) noexcept = default;


////////////////////////////////////////////////////////////
FileInputStream& FileInputStream::operator=(FileInputStream&&) noexcept = default;


////////////////////////////////////////////////////////////
std::optional<FileInputStream> FileInputStream::open(const std::filesystem::path& filename)
{
#ifdef SFML_SYSTEM_ANDROID
    if (priv::getActivityStatesPtr() != nullptr)
    {
        auto androidFile = std::make_unique<priv::ResourceStream>(filename);
        if (androidFile->tell().has_value())
            return FileInputStream(std::move(androidFile));
        return std::nullopt;
    }
#endif
#ifdef SFML_SYSTEM_WINDOWS
    if (auto file = std::unique_ptr<std::FILE, FileCloser>(_wfopen(filename.c_str(), L"rb")))
#else
    if (auto file = std::unique_ptr<std::FILE, FileCloser>(std::fopen(filename.c_str(), "rb")))
#endif
        return FileInputStream(std::move(file));
    return std::nullopt;
}


////////////////////////////////////////////////////////////
std::optional<std::size_t> FileInputStream::read(void* data, std::size_t size)
{
#ifdef SFML_SYSTEM_ANDROID
    if (priv::getActivityStatesPtr() != nullptr)
    {
        assert(m_androidFile);
        return m_androidFile->read(data, size);
    }
#endif
    assert(m_file);
    return std::fread(data, 1, size, m_file.get());
}


////////////////////////////////////////////////////////////
std::optional<std::size_t> FileInputStream::seek(std::size_t position)
{
#ifdef SFML_SYSTEM_ANDROID
    if (priv::getActivityStatesPtr() != nullptr)
    {
        assert(m_androidFile);
        return m_androidFile->seek(position);
    }
#endif
    assert(m_file);
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
        assert(m_androidFile);
        return m_androidFile->tell();
    }
#endif
    assert(m_file);
    const auto position = std::ftell(m_file.get());
    return position < 0 ? std::nullopt : std::optional<std::size_t>(position);
}


////////////////////////////////////////////////////////////
std::optional<std::size_t> FileInputStream::getSize()
{
#ifdef SFML_SYSTEM_ANDROID
    if (priv::getActivityStatesPtr() != nullptr)
    {
        assert(m_androidFile);
        return m_androidFile->getSize();
    }
#endif
    assert(m_file);
    const auto position = tell().value();
    std::fseek(m_file.get(), 0, SEEK_END);
    const std::optional size = tell();

    if (!seek(position).has_value())
        return std::nullopt;

    return size;
}


////////////////////////////////////////////////////////////
FileInputStream::FileInputStream(std::unique_ptr<std::FILE, FileCloser>&& file) : m_file(std::move(file))
{
}


////////////////////////////////////////////////////////////
#ifdef SFML_SYSTEM_ANDROID
FileInputStream::FileInputStream(std::unique_ptr<priv::ResourceStream>&& androidFile) :
m_androidFile(std::move(androidFile))
{
}
#endif

} // namespace sf

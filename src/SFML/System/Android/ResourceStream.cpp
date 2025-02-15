////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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
#include <SFML/System/Android/Activity.hpp>
#include <SFML/System/Android/ResourceStream.hpp>

#include <mutex>


namespace sf::priv
{
////////////////////////////////////////////////////////////
bool ResourceStream::open(const std::filesystem::path& filename)
{
    ActivityStates&       states = getActivity();
    const std::lock_guard lock(states.mutex);
    m_file.reset(AAssetManager_open(states.activity->assetManager, filename.c_str(), AASSET_MODE_UNKNOWN));
    return m_file != nullptr;
}


////////////////////////////////////////////////////////////
std::optional<std::size_t> ResourceStream::read(void* data, std::size_t size)
{
    assert(m_file && "ResourceStream::read() cannot be called when file is not initialized");
    const auto numBytesRead = AAsset_read(m_file.get(), data, size);
    if (numBytesRead < 0)
        return std::nullopt;
    return numBytesRead;
}


////////////////////////////////////////////////////////////
std::optional<std::size_t> ResourceStream::seek(std::size_t position)
{
    assert(m_file && "ResourceStream::seek() cannot be called when file is not initialized");
    const auto newPosition = AAsset_seek(m_file.get(), static_cast<off_t>(position), SEEK_SET);
    if (newPosition < 0)
        return std::nullopt;
    return newPosition;
}


////////////////////////////////////////////////////////////
std::optional<std::size_t> ResourceStream::tell()
{
    assert(m_file && "ResourceStream::tell() cannot be called when file is not initialized");
    return getSize().value() - static_cast<std::size_t>(AAsset_getRemainingLength(m_file.get()));
}


////////////////////////////////////////////////////////////
std::optional<std::size_t> ResourceStream::getSize()
{
    assert(m_file && "ResourceStream::getSize() cannot be called when file is not initialized");
    return AAsset_getLength(m_file.get());
}


////////////////////////////////////////////////////////////
void ResourceStream::AAssetDeleter::operator()(AAsset* file)
{
    AAsset_close(file);
}

} // namespace sf::priv

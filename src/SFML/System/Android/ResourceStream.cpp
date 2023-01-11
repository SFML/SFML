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
ResourceStream::ResourceStream(const std::filesystem::path& filename) : m_file(nullptr)
{
    ActivityStates& states = getActivity();
    std::lock_guard lock(states.mutex);
    m_file = AAssetManager_open(states.activity->assetManager, filename.c_str(), AASSET_MODE_UNKNOWN);
}


////////////////////////////////////////////////////////////
ResourceStream::~ResourceStream()
{
    if (m_file)
    {
        AAsset_close(m_file);
    }
}


////////////////////////////////////////////////////////////
std::int64_t ResourceStream::read(void* data, std::int64_t size)
{
    if (m_file)
    {
        return AAsset_read(m_file, data, static_cast<std::size_t>(size));
    }
    else
    {
        return -1;
    }
}


////////////////////////////////////////////////////////////
std::int64_t ResourceStream::seek(std::int64_t position)
{
    if (m_file)
    {
        return AAsset_seek(m_file, static_cast<off_t>(position), SEEK_SET);
    }
    else
    {
        return -1;
    }
}


////////////////////////////////////////////////////////////
std::int64_t ResourceStream::tell()
{
    if (m_file)
    {
        return getSize() - AAsset_getRemainingLength(m_file);
    }
    else
    {
        return -1;
    }
}


////////////////////////////////////////////////////////////
std::int64_t ResourceStream::getSize()
{
    if (m_file)
    {
        return AAsset_getLength(m_file);
    }
    else
    {
        return -1;
    }
}


} // namespace sf::priv

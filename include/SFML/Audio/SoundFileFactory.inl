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
#include <SFML/Audio/SoundFileFactory.hpp> // NOLINT(misc-header-include-cycle)

#include <memory>

namespace sf
{
namespace priv
{
template <typename T>
std::unique_ptr<SoundFileReader> createReader()
{
    return std::make_unique<T>();
}
template <typename T>
std::unique_ptr<SoundFileWriter> createWriter()
{
    return std::make_unique<T>();
}
} // namespace priv


////////////////////////////////////////////////////////////
template <typename T>
void SoundFileFactory::registerReader()
{
    getReaderFactoryMap()[&priv::createReader<T>] = &T::check;
}


////////////////////////////////////////////////////////////
template <typename T>
void SoundFileFactory::unregisterReader()
{
    getReaderFactoryMap().erase(&priv::createReader<T>);
}


////////////////////////////////////////////////////////////
template <typename T>
bool SoundFileFactory::isReaderRegistered()
{
    return getReaderFactoryMap().count(&priv::createReader<T>) == 1;
}


////////////////////////////////////////////////////////////
template <typename T>
void SoundFileFactory::registerWriter()
{
    getWriterFactoryMap()[&priv::createWriter<T>] = &T::check;
}


////////////////////////////////////////////////////////////
template <typename T>
void SoundFileFactory::unregisterWriter()
{
    getWriterFactoryMap().erase(&priv::createWriter<T>);
}


////////////////////////////////////////////////////////////
template <typename T>
bool SoundFileFactory::isWriterRegistered()
{
    return getWriterFactoryMap().count(&priv::createWriter<T>) == 1;
}

} // namespace sf

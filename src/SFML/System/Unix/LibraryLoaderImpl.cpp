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
#include <SFML/System/Unix/LibraryLoaderImpl.hpp>
#include <SFML/System/Err.hpp>
#include <dlfcn.h>
#include <iostream>


namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
LibraryLoaderImpl::LibraryLoaderImpl(const sf::String& libraryName) :
m_libraryName(libraryName), m_moduleHandle(dlopen(m_libraryName.toAnsiString().c_str(), RTLD_LAZY))
{
}


////////////////////////////////////////////////////////////
LibraryLoaderImpl::~LibraryLoaderImpl()
{
    freeLibrary();
}


////////////////////////////////////////////////////////////
bool LibraryLoaderImpl::isLoaded() const
{
    return m_moduleHandle != nullptr;
}


////////////////////////////////////////////////////////////
void LibraryLoaderImpl::freeLibrary()
{
    if (m_moduleHandle != nullptr)
    {
        if (dlclose(m_moduleHandle) != 0)
        {
            sf::String errorMessage(dlerror());
            sf::err() << "Failed to free library " << m_libraryName.toAnsiString() << " with error: " << errorMessage.toAnsiString() << std::endl;
        }

        m_moduleHandle = nullptr;
    }
}


////////////////////////////////////////////////////////////
bool LibraryLoaderImpl::getProcedureAddress(void*& procedureAddress, const sf::String& procedureName) const
{
    if (m_moduleHandle == nullptr)
    {
        procedureAddress = nullptr;
        return false;
    }

    procedureAddress = dlsym(m_moduleHandle, procedureName.toAnsiString().c_str());

    return procedureAddress != nullptr;
}


} // namespace priv

} // namespace sf

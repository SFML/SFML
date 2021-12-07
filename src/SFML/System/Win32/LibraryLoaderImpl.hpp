
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

#ifndef SFML_LIBRARYLOADERIMPL_HPP
#define SFML_LIBRARYLOADERIMPL_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Export.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/String.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <cstring>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief todo
///
////////////////////////////////////////////////////////////
class SFML_SYSTEM_API LibraryLoaderImpl final
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// todo
    ///
    ////////////////////////////////////////////////////////////
    LibraryLoaderImpl() = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Copy and assignment are not allowed
    ///
    ////////////////////////////////////////////////////////////
    LibraryLoaderImpl(const LibraryLoaderImpl&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Copy and assignment are not allowed
    ///
    ////////////////////////////////////////////////////////////
    LibraryLoaderImpl& operator=(const LibraryLoaderImpl&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief todo
    ///
    /// todo
    ///
    /// \param libraryName todo
    ///
    ////////////////////////////////////////////////////////////
    LibraryLoaderImpl(const sf::String& libraryName);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// todo
    ///
    ////////////////////////////////////////////////////////////
    ~LibraryLoaderImpl();

    ////////////////////////////////////////////////////////////
    /// \brief todo
    ///
    /// \return True if the TODO
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool isLoaded() const;

    ////////////////////////////////////////////////////////////
    /// \brief todo
    ///
    /// todo
    ///
    /// \param procedureAddress todo
    /// \param procedureName todo
    ///
    /// \return True if the TODO
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool getProcedureAddress(void*& procedureAddress, const sf::String& procedureName) const;

private:

    ////////////////////////////////////////////////////////////
    /// \brief todo
    ///
    /// todo
    ///
    ////////////////////////////////////////////////////////////
    void freeLibrary();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    sf::String m_libraryName;  //!< todo
    HMODULE    m_moduleHandle; //!< todo
};

} // namespace priv

} // namespace sf


#endif // SFML_LIBRARYLOADERIMPL_HPP

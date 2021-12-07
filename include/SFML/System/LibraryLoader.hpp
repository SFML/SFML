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

#ifndef SFML_LIBRARYLOADER_HPP
#define SFML_LIBRARYLOADER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Export.hpp>
#include <SFML/System/String.hpp>
#include <cstring>
#include <memory>


namespace sf
{
namespace priv
{
    class LibraryLoaderImpl;
}

////////////////////////////////////////////////////////////
/// \brief todo
///
////////////////////////////////////////////////////////////
class SFML_SYSTEM_API LibraryLoader final
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// todo
    ///
    ////////////////////////////////////////////////////////////
    LibraryLoader() = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Copy and assignment are not allowed
    ///
    ////////////////////////////////////////////////////////////
    LibraryLoader(const LibraryLoader&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Copy and assignment are not allowed
    ///
    ////////////////////////////////////////////////////////////
    LibraryLoader& operator=(const LibraryLoader&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief todo
    ///
    /// todo
    ///
    /// \param libraryName todo
    ///
    ////////////////////////////////////////////////////////////
    LibraryLoader(const sf::String& libraryName);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// todo
    ///
    ////////////////////////////////////////////////////////////
    ~LibraryLoader();

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
    template<typename TFuncType>
    [[nodiscard]] bool getProcedureAddress(TFuncType& procedureAddress, const sf::String& procedureName) const;

private:

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
    [[nodiscard]] bool getProcedureAddressInternal(void*& procedureAddress, const sf::String& procedureName) const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////

    std::unique_ptr<priv::LibraryLoaderImpl> m_impl; //!< OS specific implementation
};

#include <SFML/System/LibraryLoader.inl>

} // namespace sf


#endif // SFML_LIBRARYLOADER_HPP

////////////////////////////////////////////////////////////
/// \class sf::LibraryLoader
/// \ingroup system
///
/// TOFO
/// 
////////////////////////////////////////////////////////////

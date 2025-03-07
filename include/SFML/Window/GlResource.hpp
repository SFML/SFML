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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Export.hpp>

#include <memory>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Base class for classes that require an OpenGL context
///
////////////////////////////////////////////////////////////
class SFML_WINDOW_API GlResource
{
protected:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    GlResource();

    ////////////////////////////////////////////////////////////
    /// \brief Register an OpenGL object to be destroyed when its containing context is destroyed
    ///
    /// This is used for internal purposes in order to properly
    /// clean up OpenGL resources that cannot be shared between
    /// contexts.
    ///
    /// \param object Object to be destroyed when its containing context is destroyed
    ///
    ////////////////////////////////////////////////////////////
    static void registerUnsharedGlObject(std::shared_ptr<void> object);

    ////////////////////////////////////////////////////////////
    /// \brief Unregister an OpenGL object from its containing context
    ///
    /// \param object Object to be unregistered
    ///
    ////////////////////////////////////////////////////////////
    static void unregisterUnsharedGlObject(std::shared_ptr<void> object);

    ////////////////////////////////////////////////////////////
    /// \brief RAII helper class to temporarily lock an available context for use
    ///
    ////////////////////////////////////////////////////////////
    class SFML_WINDOW_API TransientContextLock
    {
    public:
        ////////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        ////////////////////////////////////////////////////////////
        TransientContextLock();

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ///
        ////////////////////////////////////////////////////////////
        ~TransientContextLock();

        ////////////////////////////////////////////////////////////
        /// \brief Deleted copy constructor
        ///
        ////////////////////////////////////////////////////////////
        TransientContextLock(const TransientContextLock&) = delete;

        ////////////////////////////////////////////////////////////
        /// \brief Deleted copy assignment
        ///
        ////////////////////////////////////////////////////////////
        TransientContextLock& operator=(const TransientContextLock&) = delete;
    };

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::shared_ptr<void> m_sharedContext; //!< Shared context used to link all contexts together for resource sharing
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::GlResource
/// \ingroup window
///
/// This class is for internal use only, it must be the base
/// of every class that requires a valid OpenGL context in
/// order to work.
///
////////////////////////////////////////////////////////////

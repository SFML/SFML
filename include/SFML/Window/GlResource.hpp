////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2017 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_GLRESOURCE_HPP
#define SFML_GLRESOURCE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Export.hpp>
#include <SFML/System/NonCopyable.hpp>


namespace sf
{

class Context;

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
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~GlResource();

    ////////////////////////////////////////////////////////////
    /// \brief Empty function for ABI compatibility, use acquireTransientContext instead
    ///
    ////////////////////////////////////////////////////////////
    static void ensureGlContext();

    ////////////////////////////////////////////////////////////
    /// \brief RAII helper class to temporarily lock an available context for use
    ///
    ////////////////////////////////////////////////////////////
    class SFML_WINDOW_API TransientContextLock : NonCopyable
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

    private:
        Context* m_context; ///< Temporary context, in case we needed to create one
    };
};

} // namespace sf


#endif // SFML_GLRESOURCE_HPP

////////////////////////////////////////////////////////////
/// \class sf::GlResource
/// \ingroup window
///
/// This class is for internal use only, it must be the base
/// of every class that requires a valid OpenGL context in
/// order to work.
///
////////////////////////////////////////////////////////////

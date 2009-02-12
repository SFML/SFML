////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_CONTEXT_HPP
#define SFML_CONTEXT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/System/NonCopyable.hpp>


namespace sf
{
namespace priv
{
    class WindowImpl;
}

////////////////////////////////////////////////////////////
/// Class wrapping an OpenGL context.
/// All SFML windows already have their own context, so
/// this class is more a helper for specific issues involving
/// OpenGL and multi-threading.
/// It's meant to be used internally.
////////////////////////////////////////////////////////////
class SFML_API Context : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor, create the context
    ///
    ////////////////////////////////////////////////////////////
    Context();

    ////////////////////////////////////////////////////////////
    /// Destructor, destroy the context
    ///
    ////////////////////////////////////////////////////////////
    ~Context();

    ////////////////////////////////////////////////////////////
    /// Activate or deactivate the context
    ///
    /// \param Active : True to activate the context, false to deactivate it
    ///
    ////////////////////////////////////////////////////////////
    void SetActive(bool Active);

    ////////////////////////////////////////////////////////////
    /// Check if there's a context bound to the current thread
    ///
    /// \return True if there's a context bound to the current thread
    ///
    ////////////////////////////////////////////////////////////
    static bool IsContextActive();

    ////////////////////////////////////////////////////////////
    /// Get the global context
    ///
    /// \return Reference to the global context
    ///
    ////////////////////////////////////////////////////////////
    static Context& GetGlobal();

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    priv::WindowImpl* myDummyWindow; ///< Dummy window holding the context
};

} // namespace sf


#endif // SFML_CONTEXT_HPP

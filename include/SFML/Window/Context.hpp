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
    class ContextGL;
}

////////////////////////////////////////////////////////////
/// Class holding a valid drawing context
///
/// If you need to make OpenGL / graphics calls without
/// having an active window (like in a thread), you can use
/// an instance of this class to get a valid context.
///
/// Having a valid context is necessary for *every* OpenGL call,
/// and for most of the classes from the Graphics package.
///
/// Note that a context is only active in its current thread,
/// if you create a new thread it will have no valid context
/// by default.
///
/// \code
/// void ThreadFunction(void*)
/// {
///    sf::Context context;
///    // from now on, you have a valid context
///
///    // you can make OpenGL calls
///    glClear(GL_DEPTH_BUFFER_BIT);
///
///    // as well as using objects from the graphics package
///    sf::Image Img;
///    Img.LoadFromFile("image.png");
/// }
/// // the context is automatically deactivated and destroyed
/// // by the sf::Context destructor
/// \endcode
////////////////////////////////////////////////////////////
class SFML_API Context : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor -- creates and activates the context
    ///
    ////////////////////////////////////////////////////////////
    Context();

    ////////////////////////////////////////////////////////////
    /// Destructor -- deactivates and destroys the context
    ///
    ////////////////////////////////////////////////////////////
    ~Context();

    ////////////////////////////////////////////////////////////
    /// Activate or deactivate explicitely the context
    ///
    /// \param active : True to activate, false to deactivate
    ///
    ////////////////////////////////////////////////////////////
    void SetActive(bool active);

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    priv::ContextGL* myContext; ///< Internal OpenGL context
};

} // namespace sf


#endif // SFML_CONTEXT_HPP

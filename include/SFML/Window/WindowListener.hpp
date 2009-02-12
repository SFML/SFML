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

#ifndef SFML_WINDOWLISTENER_HPP
#define SFML_WINDOWLISTENER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>


namespace sf
{
class Event;

////////////////////////////////////////////////////////////
/// Base class for classes that want to receive events
/// from a window (for internal use only)
////////////////////////////////////////////////////////////
class SFML_API WindowListener
{
public :

    ////////////////////////////////////////////////////////////
    /// Called each time an event is received from attached window
    ///
    /// \param EventReceived : Event received
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnEvent(const Event& EventReceived) = 0;

protected :

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~WindowListener() {}
};

} // namespace sf


#endif // SFML_WINDOWLISTENER_HPP

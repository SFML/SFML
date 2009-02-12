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

#ifndef SFML_CLOCK_HPP
#define SFML_CLOCK_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// Clock is an utility class for manipulating time
////////////////////////////////////////////////////////////
class SFML_API Clock
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Clock();

    ////////////////////////////////////////////////////////////
    /// Get the time elapsed since last reset
    ///
    /// \return Time elapsed, in seconds
    ///
    ////////////////////////////////////////////////////////////
    float GetElapsedTime() const;

    ////////////////////////////////////////////////////////////
    /// Restart the timer
    ///
    ////////////////////////////////////////////////////////////
    void Reset();

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    double myStartTime; ///< Time of last reset
};

} // namespace sf


#endif // SFML_CLOCK_HPP

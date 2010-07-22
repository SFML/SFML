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
/// \brief Utility class for manipulating time
///
////////////////////////////////////////////////////////////
class SFML_API Clock
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// The clock starts automatically after being constructed.
    ///
    ////////////////////////////////////////////////////////////
    Clock();

    ////////////////////////////////////////////////////////////
    /// \brief Get the time elapsed
    ///
    /// This function returns the time elapsed since the last call
    /// to Reset() (or the construction of the instance if Reset()
    /// has not been called) in seconds.
    ///
    /// \return Time elapsed, in seconds
    ///
    ////////////////////////////////////////////////////////////
    float GetElapsedTime() const;

    ////////////////////////////////////////////////////////////
    /// \brief Restart the timer
    ///
    /// This function puts the time counter back to zero.
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


////////////////////////////////////////////////////////////
/// \class sf::Clock
/// \ingroup system
///
/// sf::Clock is a lightweight class for measuring time.
/// Its resolution depends on the underlying OS, but you can generally
/// expect a 1 ms resolution.
///
/// Usage example:
/// \code
/// sf::Clock clock;
/// ...
/// float time1 = clock.GetElapsedTime();
/// clock.Reset();
/// ...
/// float time2 = clock.GetElapsedTime();
/// \endcode
///
////////////////////////////////////////////////////////////

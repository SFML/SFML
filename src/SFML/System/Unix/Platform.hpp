////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_PLATFORMUNIX_HPP
#define SFML_PLATFORMUNIX_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <vector>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Give access to some system-specific low-level functions
////////////////////////////////////////////////////////////
class Platform
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Get the current system time
    ///
    /// \return System time, in seconds
    ///
    ////////////////////////////////////////////////////////////
    static double GetSystemTime();

    ////////////////////////////////////////////////////////////
    /// \brief Suspend the execution of the current thread for a specified time
    ///
    /// \param time Time to sleep, in seconds
    ///
    ////////////////////////////////////////////////////////////
    static void Sleep(float time);
};

} // namespace priv

} // namespace sf


#endif // SFML_PLATFORMUNIX_HPP

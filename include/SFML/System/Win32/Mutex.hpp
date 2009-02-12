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

#ifndef SFML_MUTEXWIN32_HPP
#define SFML_MUTEXWIN32_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/NonCopyable.hpp>
#include <windows.h>


namespace sf
{
////////////////////////////////////////////////////////////
/// Mutex defines a mutex (MUTual EXclusion) object,
/// that allows a thread to lock critical instructions
/// to avoid simultaneous access with other threads.
/// The Win32 version uses critical sections, as it is
/// faster than mutexes.<br/>
/// See Lock for an efficient way of using it.
////////////////////////////////////////////////////////////
class SFML_API Mutex : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Mutex();

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~Mutex();

    ////////////////////////////////////////////////////////////
    /// Lock the mutex
    ///
    ////////////////////////////////////////////////////////////
    void Lock();

    ////////////////////////////////////////////////////////////
    /// Unlock the mutex
    ///
    ////////////////////////////////////////////////////////////
    void Unlock();

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    CRITICAL_SECTION myHandle; ///< Win32 handle of the mutex
};

} // namespace sf


#endif // SFML_MUTEXWIN32_HPP

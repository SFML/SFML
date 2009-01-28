////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2008 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_MUTEX_H
#define SFML_MUTEX_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>


////////////////////////////////////////////////////////////
/// sfMutex defines a mutex (MUTual EXclusion) object,
/// that allows a thread to lock critical instructions
/// to avoid simultaneous access with other threads.
////////////////////////////////////////////////////////////
typedef struct sfMutex sfMutex;

////////////////////////////////////////////////////////////
/// Create a new mutex
///
/// \return A new sfMutex object
///
////////////////////////////////////////////////////////////
CSFML_API sfMutex* sfMutex_Create();

////////////////////////////////////////////////////////////
/// Destroy an existing mutex
///
/// \param Mutex : Mutex to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfMutex_Destroy(sfMutex* Mutex);

////////////////////////////////////////////////////////////
/// Lock a mutex
///
/// \param Mutex : Mutex to lock
///
////////////////////////////////////////////////////////////
CSFML_API void sfMutex_Lock(sfMutex* Mutex);

////////////////////////////////////////////////////////////
/// Unlock a mutex
///
/// \param Mutex : Mutex to unlock
///
////////////////////////////////////////////////////////////
CSFML_API void sfMutex_Unlock(sfMutex* Mutex);


#endif // SFML_MUTEX_H

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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Mutex.h>
#include <SFML/System/MutexStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Create a new mutex
////////////////////////////////////////////////////////////
sfMutex* sfMutex_Create(void)
{
    return new sfMutex;
}


////////////////////////////////////////////////////////////
/// Destroy an existing mutex
////////////////////////////////////////////////////////////
void sfMutex_Destroy(sfMutex* mutex)
{
    delete mutex;
}


////////////////////////////////////////////////////////////
/// Lock a mutex
////////////////////////////////////////////////////////////
void sfMutex_Lock(sfMutex* mutex)
{
    CSFML_CALL(mutex, Lock())
}


////////////////////////////////////////////////////////////
/// Unlock a mutex
////////////////////////////////////////////////////////////
void sfMutex_Unlock(sfMutex* mutex)
{
    CSFML_CALL(mutex, Unlock())
}

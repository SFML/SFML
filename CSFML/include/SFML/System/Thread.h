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

#ifndef SFML_THREAD_H
#define SFML_THREAD_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/System/Types.h>


////////////////////////////////////////////////////////////
/// Construct a new thread from a function pointer
///
/// \param function : Entry point of the thread
/// \param userData : Data to pass to the thread function
///
////////////////////////////////////////////////////////////
CSFML_API sfThread* sfThread_Create(void (*function)(void*), void* userData);

////////////////////////////////////////////////////////////
/// Destroy an existing thread
///
/// \param thread : Thread to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfThread_Destroy(sfThread* thread);

////////////////////////////////////////////////////////////
/// Run a thread
///
/// \param thread : Thread to launch
///
////////////////////////////////////////////////////////////
CSFML_API void sfThread_Launch(sfThread* thread);

////////////////////////////////////////////////////////////
/// Wait until a thread finishes
///
/// \param thread : Thread to wait for
///
////////////////////////////////////////////////////////////
CSFML_API void sfThread_Wait(sfThread* thread);

////////////////////////////////////////////////////////////
/// Terminate a thread
/// Terminating a thread with this function is not safe,
/// you should rather try to make the thread function
/// terminate by itself
///
/// \param thread : Thread to terminate
///
////////////////////////////////////////////////////////////
CSFML_API void sfThread_Terminate(sfThread* thread);


#endif // SFML_THREAD_H

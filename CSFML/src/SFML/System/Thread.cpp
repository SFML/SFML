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
#include <SFML/System/Thread.h>
#include <SFML/System/ThreadStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Construct a new thread from a function pointer
////////////////////////////////////////////////////////////
sfThread* sfThread_Create(void (*function)(void*), void* userData)
{
    return new sfThread(function, userData);
}


////////////////////////////////////////////////////////////
/// Destroy an existing thread
////////////////////////////////////////////////////////////
void sfThread_Destroy(sfThread* thread)
{
    delete thread;
}


////////////////////////////////////////////////////////////
/// Run a thread
////////////////////////////////////////////////////////////
void sfThread_Launch(sfThread* thread)
{
    CSFML_CALL(thread, Launch());
}


////////////////////////////////////////////////////////////
/// Wait until a thread finishes
////////////////////////////////////////////////////////////
void sfThread_Wait(sfThread* thread)
{
    CSFML_CALL(thread, Wait());
}


////////////////////////////////////////////////////////////
/// Terminate a thread
/// Terminating a thread with this function is not safe,
/// you should rather try to make the thread function
/// terminate by itself
////////////////////////////////////////////////////////////
void sfThread_Terminate(sfThread* thread)
{
    CSFML_CALL(thread, Terminate());
}

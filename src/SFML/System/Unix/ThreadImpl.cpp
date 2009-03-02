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
#include <SFML/System/Unix/ThreadImpl.hpp>
#include <SFML/System/Thread.hpp>
#include <iostream>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
ThreadImpl::ThreadImpl(Thread* Owner) :
myIsActive(true)
{
    myIsActive = pthread_create(&myThread, NULL, &ThreadImpl::EntryPoint, Owner) == 0;

    if (!myIsActive)
        std::cerr << "Failed to create thread" << std::endl;
}


////////////////////////////////////////////////////////////
/// Wait until the thread finishes
////////////////////////////////////////////////////////////
void ThreadImpl::Wait()
{
    if (myIsActive)
        pthread_join(myThread, NULL);
}


////////////////////////////////////////////////////////////
/// Terminate the thread
/// Terminating a thread with this function is not safe,
/// you should rather try to make the thread function
/// terminate by itself
////////////////////////////////////////////////////////////
void ThreadImpl::Terminate()
{
    if (myIsActive)
        pthread_cancel(myThread);
}


////////////////////////////////////////////////////////////
/// Global entry point for all threads
////////////////////////////////////////////////////////////
void* ThreadImpl::EntryPoint(void* UserData)
{
    // The Thread instance is stored in the user data
    Thread* Owner = static_cast<Thread*>(UserData);

    // Tell the thread to handle cancel requests immediatly
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    // Forward to the owner
    Owner->Run();

    return NULL;
}

} // namespace priv

} // namespace sf

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/System/Lock.hpp>
#include <iostream>
#include <cassert>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
ThreadImpl::ThreadImpl(Thread* owner) :
m_isCreated(false),
m_isRunning(false)
{
    // We must take the lock early, or we could set m_isRunning to true, after the function has died instantly.
    Lock theLock(m_mutex);

    m_isCreated = pthread_create(&m_thread, NULL, &ThreadImpl::entryPoint, owner) == 0;

    if (!m_isCreated)
        std::cerr << "Failed to create thread" << std::endl;
    else
        m_isRunning = true;
}


////////////////////////////////////////////////////////////
void ThreadImpl::wait()
{
    if (m_isCreated)
    {
        assert(pthread_equal(pthread_self(), m_thread) == 0); // A thread cannot wait for itself!
        pthread_join(m_thread, NULL);
    }
}

////////////////////////////////////////////////////////////
bool ThreadImpl::poll()
{
    bool ret;
    {
        Lock theLock(m_mutex);
        ret = m_isRunning;
    }

    if (!ret)
        wait();

    return ret;        
}


////////////////////////////////////////////////////////////
void ThreadImpl::terminate()
{
    if (m_isCreated)
    {
        pthread_cancel(m_thread);
        // No lock needed here, the thread is dead.
        m_isRunning = false;
    }
}


////////////////////////////////////////////////////////////
void* ThreadImpl::entryPoint(void* userData)
{
    // The Thread instance is stored in the user data
    Thread* owner = static_cast<Thread*>(userData);

    // Tell the thread to handle cancel requests immediatly
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    // Forward to the owner
    owner->run();

    // Note that we have finished.
    Lock theLock(owner->m_impl->m_mutex);
    owner->m_impl->m_isRunning = false;

    return NULL;
}

} // namespace priv

} // namespace sf

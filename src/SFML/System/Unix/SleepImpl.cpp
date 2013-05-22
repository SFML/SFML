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
#include <SFML/System/Unix/SleepImpl.hpp>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void sleepImpl(Time time)
{
    // usleep is not reliable enough (it might block the
    // whole process instead of just the current thread)
    // so we must use pthread_cond_timedwait instead

    // this implementation is inspired from Qt

    Uint64 usecs = time.asMicroseconds();

    // get the current time
    timeval tv;
    gettimeofday(&tv, NULL);

    // construct the time limit (current time + time to wait)
    timespec ti;
    ti.tv_nsec = (tv.tv_usec + (usecs % 1000000)) * 1000;
    ti.tv_sec = tv.tv_sec + (usecs / 1000000) + (ti.tv_nsec / 1000000000);
    ti.tv_nsec %= 1000000000;

    // create a mutex and thread condition
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, 0);
    pthread_cond_t condition;
    pthread_cond_init(&condition, 0);

    // wait...
    pthread_mutex_lock(&mutex);
    pthread_cond_timedwait(&condition, &mutex, &ti);
    pthread_mutex_unlock(&mutex);

    // destroy the mutex and condition
    pthread_cond_destroy(&condition);
    pthread_mutex_destroy(&mutex);
}

} // namespace priv

} // namespace sf

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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Unix/Thread.hpp>
#include <iostream>


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Thread::Thread() :
myIsActive(false),
myFunction(NULL),
myUserData(NULL)
{

}


////////////////////////////////////////////////////////////
/// Construct the thread from a function pointer
////////////////////////////////////////////////////////////
Thread::Thread(Thread::FuncType Function, void* UserData) :
myIsActive(false),
myFunction(Function),
myUserData(UserData)
{

}


////////////////////////////////////////////////////////////
/// Virtual destructor
////////////////////////////////////////////////////////////
Thread::~Thread()
{
    // Wait for the thread to finish before destroying the instance
    if (myIsActive)
        Wait();
}


////////////////////////////////////////////////////////////
/// Create and run the thread
////////////////////////////////////////////////////////////
void Thread::Launch()
{
    // Create the thread
    myIsActive = true;
    int Error = pthread_create(&myThread, NULL, &Thread::ThreadFunc, this);

    // Error ?
    if (Error != 0)
    {
        std::cerr << "Failed to create thread" << std::endl;
        myIsActive = false;
    }
}


////////////////////////////////////////////////////////////
/// Wait until the thread finishes
////////////////////////////////////////////////////////////
void Thread::Wait()
{
    if (myIsActive)
    {
        // Wait for the thread to finish, no timeout
        pthread_join(myThread, NULL);

        // Reset the thread state
        myIsActive = false;
    }
}


////////////////////////////////////////////////////////////
/// Terminate the thread
/// Terminating a thread with this function is not safe,
/// you should rather try to make the thread function
/// terminate by itself
////////////////////////////////////////////////////////////
void Thread::Terminate()
{
    if (myIsActive)
    {
        pthread_cancel(myThread);
        myIsActive = false;
    }
}


////////////////////////////////////////////////////////////
/// Function called as the thread entry point
////////////////////////////////////////////////////////////
void Thread::Run()
{
    if (myFunction)
        myFunction(myUserData);
}


////////////////////////////////////////////////////////////
/// Actual thread entry point, dispatches to instances
////////////////////////////////////////////////////////////
void* Thread::ThreadFunc(void* UserData)
{
    // The sfThread instance is stored in the user data
    Thread* ThreadToRun = reinterpret_cast<Thread*>(UserData);

    // Tell the thread to handle cancel requests immediatly
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    // Forward to the instance
    ThreadToRun->Run();

    return NULL;
}

} // namespace sf

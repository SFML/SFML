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
#include <SFML/System/Win32/Thread.hpp>
#include <process.h>
#include <iostream>


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Thread::Thread() :
myHandle  (NULL),
myFunction(NULL),
myUserData(NULL)
{

}


////////////////////////////////////////////////////////////
/// Construct the thread from a function pointer
////////////////////////////////////////////////////////////
Thread::Thread(Thread::FuncType Function, void* UserData) :
myHandle  (NULL),
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
    if (myHandle)
        Wait();
}


////////////////////////////////////////////////////////////
/// Create and run the thread
////////////////////////////////////////////////////////////
void Thread::Launch()
{
    // Create the thread
    myHandle = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &Thread::ThreadFunc, this, 0, NULL));

    // Error ?
    if (myHandle == NULL)
        std::cerr << "Failed to create thread" << std::endl;
}


////////////////////////////////////////////////////////////
/// Wait until the thread finishes
////////////////////////////////////////////////////////////
void Thread::Wait()
{
    if (myHandle)
    {
        // Wait for the thread to finish, no timeout
        WaitForSingleObject(myHandle, INFINITE);

        // Don't forget to close the thread handle (__endthreadex doesn't do it)
        CloseHandle(myHandle);
        myHandle = NULL;
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
    if (myHandle)
    {
        TerminateThread(myHandle, 0);
        myHandle = NULL;
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
unsigned int __stdcall Thread::ThreadFunc(void* UserData)
{
    // The Thread instance is stored in the user data
    Thread* ThreadInstance = reinterpret_cast<Thread*>(UserData);

    // Forward to the instance
    ThreadInstance->Run();

    // Optional, but it is cleaner
    _endthreadex(0);

    return 0;
}

} // namespace sf

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
#include <SFML/System/Thread.hpp>


#if defined(SFML_SYSTEM_WINDOWS)

    #include <SFML/System/Win32/ThreadImpl.hpp>

#else

    #include <SFML/System/Unix/ThreadImpl.hpp>

#endif


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Thread::Thread() :
myThreadImpl(NULL),
myFunction  (NULL),
myUserData  (NULL)
{

}


////////////////////////////////////////////////////////////
/// Construct the thread from a function pointer
////////////////////////////////////////////////////////////
Thread::Thread(Thread::FuncType Function, void* UserData) :
myThreadImpl(NULL),
myFunction  (Function),
myUserData  (UserData)
{

}


////////////////////////////////////////////////////////////
/// Virtual destructor
////////////////////////////////////////////////////////////
Thread::~Thread()
{
    // Wait for the thread to finish before destroying the instance
    Wait();

    // Destroy the implementation
    delete myThreadImpl;
}


////////////////////////////////////////////////////////////
/// Create and run the thread
////////////////////////////////////////////////////////////
void Thread::Launch()
{
    delete myThreadImpl;
    myThreadImpl = new priv::ThreadImpl(this);
}


////////////////////////////////////////////////////////////
/// Wait until the thread finishes
////////////////////////////////////////////////////////////
void Thread::Wait()
{
    if (myThreadImpl)
        myThreadImpl->Wait();
}


////////////////////////////////////////////////////////////
/// Terminate the thread
/// Terminating a thread with this function is not safe,
/// you should rather try to make the thread function
/// terminate by itself
////////////////////////////////////////////////////////////
void Thread::Terminate()
{
    if (myThreadImpl)
        myThreadImpl->Terminate();
}


////////////////////////////////////////////////////////////
/// Function called as the thread entry point
////////////////////////////////////////////////////////////
void Thread::Run()
{
    if (myFunction)
        myFunction(myUserData);
}

} // namespace sf

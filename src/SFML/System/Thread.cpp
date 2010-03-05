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
Thread::Thread() :
myThreadImpl(NULL),
myFunction  (NULL),
myUserData  (NULL)
{

}


////////////////////////////////////////////////////////////
Thread::Thread(Thread::FuncType function, void* userData) :
myThreadImpl(NULL),
myFunction  (function),
myUserData  (userData)
{

}


////////////////////////////////////////////////////////////
Thread::~Thread()
{
    Wait();
}


////////////////////////////////////////////////////////////
void Thread::Launch()
{
    Wait();
    myThreadImpl = new priv::ThreadImpl(this);
}


////////////////////////////////////////////////////////////
void Thread::Wait()
{
    if (myThreadImpl)
    {
        myThreadImpl->Wait();
        delete myThreadImpl;
        myThreadImpl = NULL;
    }
}


////////////////////////////////////////////////////////////
void Thread::Terminate()
{
    if (myThreadImpl)
    {
        myThreadImpl->Terminate();
        delete myThreadImpl;
        myThreadImpl = NULL;
    }
}


////////////////////////////////////////////////////////////
void Thread::Run()
{
    if (myFunction)
        myFunction(myUserData);
}

} // namespace sf

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Window/Win32/GlContextImpl.hpp>
#include <SFML/System/Err.hpp>
#include <windows.h>

namespace sf
{
namespace priv
{

struct UserData
{
    HANDLE waitThread;
    threadExitFunction func;
    void* funcArg;
};

unsigned int __stdcall entryPoint(void* data)
{
    UserData* userData = static_cast<UserData*>(data);

    // The thread we are originally waiting for
    HANDLE waitThread = userData->waitThread;

    // The function to call when that thread ends
    threadExitFunction func = userData->func;

    // The argument to pass to the function to call
    void* funcArg = userData->funcArg;

    delete userData;

    // Wait for the thread to end
    DWORD result = WaitForSingleObject(waitThread, INFINITE);

    CloseHandle(waitThread);

    // If the wait was successful, we call the function with the argument
    if (result == WAIT_OBJECT_0)
    {
        func(funcArg);
    }
    else
    {
        err() << "Failed to wait for thread to end" << std::endl;
    }

    // Optional, but it is cleaner
    _endthreadex(0);

    return 0;
}

void runAtThreadExit(threadExitFunction func, void* arg)
{
    HANDLE currentThreadHandle;

    // Get a real (non-pseudo) handle to the current thread
    BOOL result = DuplicateHandle(GetCurrentProcess(),
                                  GetCurrentThread(),
                                  GetCurrentProcess(),
                                  &currentThreadHandle,
                                  0,
                                  FALSE,
                                  DUPLICATE_SAME_ACCESS);

    if (!result)
    {
        err() << "Failed to duplicate current thread handle" << std::endl;
        return;
    }

    UserData* userData = new UserData;

    userData->waitThread = currentThreadHandle;
    userData->func = func;
    userData->funcArg = arg;

    HANDLE thread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &entryPoint, userData, 0, NULL));

    if (!thread)
        err() << "Failed to create thread" << std::endl;
}

} // namespace priv

} // namespace sf

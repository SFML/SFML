////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/System/Win32/ThreadLocalImpl.hpp>
#include <SFML/System/Err.hpp>


namespace
{
    // Since we need to convert from WINAPI to the user's calling convention
    // and Windows doesn't support specifying custom user data, we need
    // to store the destructor in FLS as well
    struct FlsControlBlock
    {
        sf::ThreadLocalDestructorPointer destructor;
        void* data;
    };

    // FLS entry points
    typedef void (WINAPI* FlsCallback)(void*);

    typedef DWORD (WINAPI* FlsAllocFunc)(FlsCallback);
    typedef BOOL (WINAPI* FlsFreeFunc)(DWORD);
    typedef BOOL (WINAPI* FlsSetValueFunc)(DWORD, PVOID);
    typedef PVOID (WINAPI* FlsGetValueFunc)(DWORD);

    FlsAllocFunc flsAlloc = NULL;
    FlsFreeFunc flsFree = NULL;
    FlsSetValueFunc flsSetValue = NULL;
    FlsGetValueFunc flsGetValue = NULL;

    bool flsInitialized = false;
    bool hasFls = false;

    // Try to load the FLS entry points
    void initializeFls()
    {
        HINSTANCE kernel32Dll = LoadLibraryA("kernel32.dll");

        if (kernel32Dll)
        {
            flsAlloc = reinterpret_cast<FlsAllocFunc>(GetProcAddress(kernel32Dll, "FlsAlloc"));
            flsFree = reinterpret_cast<FlsFreeFunc>(GetProcAddress(kernel32Dll, "FlsFree"));
            flsSetValue = reinterpret_cast<FlsSetValueFunc>(GetProcAddress(kernel32Dll, "FlsSetValue"));
            flsGetValue = reinterpret_cast<FlsGetValueFunc>(GetProcAddress(kernel32Dll, "FlsGetValue"));

            hasFls = (flsAlloc != NULL) && (flsFree != NULL) && (flsSetValue != NULL) && (flsGetValue != NULL);

            FreeLibrary(kernel32Dll);
        }

        flsInitialized = true;
    }

    // This function is called by the operating system when a thread terminates
    void WINAPI flsCallback(void* arg)
    {
        // We mustn't forget to delete the control block if one was allocated
        if (arg)
        {
            FlsControlBlock* controlBlock = static_cast<FlsControlBlock*>(arg);

            if (controlBlock->destructor)
                controlBlock->destructor(controlBlock->data);

            delete controlBlock;
        }
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
ThreadLocalImpl::ThreadLocalImpl(ThreadLocalDestructorPointer destructor) :
m_destructor(destructor)
{
    if (!flsInitialized)
        initializeFls();

    if (hasFls)
    {
        m_index = flsAlloc(flsCallback);
    }
    else
    {
        if (destructor)
            sf::err() << "Warning: Thread local destructors are not supported on this system" << std::endl;

        m_index = TlsAlloc();
    }
}


////////////////////////////////////////////////////////////
ThreadLocalImpl::~ThreadLocalImpl()
{
    if (hasFls)
    {
        flsFree(m_index);
    }
    else
    {
        TlsFree(m_index);
    }
}


////////////////////////////////////////////////////////////
void ThreadLocalImpl::setValue(void* value)
{
    if (hasFls)
    {
        // We are forced to read-modify-write here since there is no other way in Windows
        FlsControlBlock* controlBlock = static_cast<FlsControlBlock*>(flsGetValue(m_index));

        // Initialize the control block if it doesn't already exist
        if (!controlBlock)
        {
            controlBlock = new FlsControlBlock;
            controlBlock->destructor = m_destructor;
        }

        controlBlock->data = value;

        flsSetValue(m_index, controlBlock);
    }
    else
    {
        TlsSetValue(m_index, value);
    }
}


////////////////////////////////////////////////////////////
void* ThreadLocalImpl::getValue() const
{
    if (hasFls)
    {
        FlsControlBlock* controlBlock = static_cast<FlsControlBlock*>(flsGetValue(m_index));

        if (controlBlock)
            return controlBlock->data;

        return NULL;
    }
    else
    {
        return TlsGetValue(m_index);
    }
}


////////////////////////////////////////////////////////////
bool ThreadLocalImpl::isDestructorSupported()
{
    if (!flsInitialized)
        initializeFls();

    return hasFls;
}

} // namespace priv

} // namespace sf

/*
*   DSFML - SFML Library binding in D language.
*   Copyright (C) 2008 Julien Dagorn (sirjulio13@gmail.com)
*
*   This software is provided 'as-is', without any express or
*   implied warranty. In no event will the authors be held
*   liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose,
*   including commercial applications, and to alter it and redistribute
*   it freely, subject to the following restrictions:
*
*   1.  The origin of this software must not be misrepresented;
*       you must not claim that you wrote the original software.
*       If you use this software in a product, an acknowledgment
*       in the product documentation would be appreciated but
*       is not required.
*
*   2.  Altered source versions must be plainly marked as such,
*       and must not be misrepresented as being the original software.
*
*   3.  This notice may not be removed or altered from any
*       source distribution.
*/

module dsfml.system.mutex;

import dsfml.system.common;

/**
*   Provide portable implementation of Mutual Exclusion object.
*   
*   Uses CriticalSection on Windows and pthread mutexes on linux.
*/
final class Mutex : DSFMLObject
{
    /**
    *   Constructor
    */   
    this()
    {
        super(sfMutex_Create());
    }

    override void dispose()
    {
        sfMutex_Destroy(m_ptr);
    }

    /**
    *   Lock the mutex
    *   
    *   Can be called multiples times, but each lock must be unlocked with unlock()        
    */        
    void lock()
    {
        sfMutex_Lock(m_ptr);
    }
    
    /**
    *   Unlock the mutex
    */        
    void unlock()
    {
        sfMutex_Unlock(m_ptr);
    }

private:

// External ====================================================================

    extern (C)
    {
        typedef void* function() pf_sfMutex_Create;
    	typedef void function(void*) pf_sfMutex_Destroy;
    	typedef void function(void*) pf_sfMutex_Lock;
    	typedef void function(void*) pf_sfMutex_Unlock;
    	
    	static pf_sfMutex_Create sfMutex_Create; 
    	static pf_sfMutex_Destroy sfMutex_Destroy;
    	static pf_sfMutex_Lock sfMutex_Lock;   
    	static pf_sfMutex_Unlock sfMutex_Unlock; 
    }

    static this()
    {
        DllLoader dll = DllLoader.load("csfml-system");
        
        sfMutex_Create = cast(pf_sfMutex_Create)dll.getSymbol("sfMutex_Create"); 
        sfMutex_Destroy = cast(pf_sfMutex_Destroy)dll.getSymbol("sfMutex_Destroy");
        sfMutex_Lock = cast(pf_sfMutex_Lock)dll.getSymbol("sfMutex_Lock");   
        sfMutex_Unlock = cast(pf_sfMutex_Unlock)dll.getSymbol("sfMutex_Unlock"); 
    }
}

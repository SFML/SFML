/*
*   DSFML - SFML Library wrapper for the D programming language.
*   Copyright (C) 2008 Julien Dagorn (sirjulio13@gmail.com)
*   Copyright (C) 2010 Andreas Hollandt
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

module dsfml.system.clock;

import dsfml.system.common;

/**
 *  Utility class for manipulating time
 */ 
class Clock : DSFMLObject
{
    /**
     *  Default constructor
     */        
    this()
    {
        super(sfClock_Create());
    }

    /**
     *  Destructor        
     */        
    override void dispose()
    {
        sfClock_Destroy(m_ptr);
    }

    /**
     *  Return the time elapsed since the last reset
     *  Returns: 
     *      Elapsed Time in seconds                          
     */        
    float getElapsedTime()
    {
        return sfClock_GetTime(m_ptr);
    }

    /**
     *  Restart the timer        
     */        
    void reset()
    {
        sfClock_Reset(m_ptr);
    }

private:
    
// External ====================================================================
    
    
    extern (C)
    {
        typedef void* function() pf_sfClock_Create;
    	typedef void function(void*) pf_sfClock_Destroy;
    	typedef float function(void*) pf_sfClock_GetTime;
    	typedef void function(void*) pf_sfClock_Reset;
    
    	static pf_sfClock_Create sfClock_Create;
    	static pf_sfClock_Destroy sfClock_Destroy;
    	static pf_sfClock_GetTime sfClock_GetTime;
    	static pf_sfClock_Reset sfClock_Reset;
    	
    	static this()
        {
		debug
			DllLoader dll = DllLoader.load("csfml-system-d");
		else
			DllLoader dll = DllLoader.load("csfml-system");
            
            sfClock_Create = cast(pf_sfClock_Create)dll.getSymbol("sfClock_Create");
            sfClock_Destroy = cast(pf_sfClock_Destroy)dll.getSymbol("sfClock_Destroy");
            sfClock_GetTime = cast(pf_sfClock_GetTime)dll.getSymbol("sfClock_GetTime");
            sfClock_Reset = cast(pf_sfClock_Reset)dll.getSymbol("sfClock_Reset");
        }
    }
}


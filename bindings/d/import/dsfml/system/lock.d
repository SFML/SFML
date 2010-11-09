/*
*	DSFML - SFML Library wrapper for the D programming language.
*	Copyright (C) 2008 Julien Dagorn (sirjulio13@gmail.com)
*	Copyright (C) 2010 Andreas Hollandt
*
*	This software is provided 'as-is', without any express or
*	implied warranty. In no event will the authors be held
*	liable for any damages arising from the use of this software.
*
*	Permission is granted to anyone to use this software for any purpose,
*	including commercial applications, and to alter it and redistribute
*	it freely, subject to the following restrictions:
*
*	1.  The origin of this software must not be misrepresented;
*		you must not claim that you wrote the original software.
*		If you use this software in a product, an acknowledgment
*		in the product documentation would be appreciated but
*		is not required.
*
*	2.  Altered source versions must be plainly marked as such,
*		and must not be misrepresented as being the original software.
*
*	3.  This notice may not be removed or altered from any
*		source distribution.
*/

module dsfml.system.lock;

import core.sync.mutex;

/**
*	Encapsulation of an critical section. Unlocking is guaranteed when the Lock goes out of scope, even on exception.
*	
*	Remarks:
*		Lock is a scope class, you need to mark Lock object as scope :
*		
*	-----------------
*	Mutex m = new Mutex;
*	//..
*	{
*		scope Lock l = new Lock(m);
*		// Critical section 
*	} // End of critical (Destructor called and mutex unlocked)
*	//..	
*
*	-----------------
*/
scope class Lock
{
	/**
	*	Construct the lock and lock the mutex
	*/		
	this(Mutex m)
	{
		m_mutex = m;
		m_mutex.lock();
	}
	
	~this()
	{
		m_mutex.unlock();
	}

private:
	Mutex m_mutex;
}

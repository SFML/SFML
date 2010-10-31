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

module dsfml.system.alloc;

version (Tango)
{
	public import tango.core.Memory;
}
else
{
	public import core.memory;
}

/*
	struct GC
	{
		static void* malloc(uint size)
		{
			return std.c.stdlib.malloc(size);
		}
	
		static void free(void* ptr)
		{
			std.c.stdlib.free(ptr);
		}
	
		static void addRange(void* ptr, uint size)
		{
			std.gc.addRange(ptr, ptr + size);
		}
	
		static void removeRange(void* ptr)
		{
			std.gc.removeRange(ptr);
		}
	}

*/

/*
*	Template for native non-GCed allocation for interaction between C and D threads.
*/
template Alloc()
{
	new (size_t size)
	{
		void* p = GC.malloc(size);
		if (!p)
			assert(0, "Memory allocation failed");
		
		GC.addRange(p, size);
		return p;
	}

	delete(void* p)
	{
		GC.removeRange(p);
		GC.free(p);
	}
}
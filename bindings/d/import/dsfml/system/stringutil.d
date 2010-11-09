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

module dsfml.system.stringutil;

import std.traits; // for Unqual

/*
version (Tango)
{
	 public import tango.stdc.stringz;
}
else
{
	public import std.string;
}
*/


	T* toStringz(T)(T[] str)
	{
		if (str is null)
			return null;
		else if (str.length && str[$ - 1] is T.init)
			return str.ptr;

		auto ret = new Unqual!(T)[str.length + 1];
		
		ret[0 .. str.length] = str[0 .. $];
		ret[str.length] = 0;
		
		return cast(T*) ret.ptr;
	}


	size_t stringLength(T)(T* p)
	{
		if (p is null || *p == T.init)
			return 0;
		
		size_t length;
		
		while (*(p + length))
		{
			length++;
		}
	
		return length;
	}

	T[] fromStringz(T)(T* ptr)
	{	
		auto ret = new Unqual!(T)[stringLength(ptr)];
		ret[0..$] = ptr[0..ret.length];
		
		return cast(T[]) ret;
	}

  
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

module dsfml.system.common;

public import dsfml.system.dllloader;

// type aliases for D2
package
{
	alias const(char) cchar;
	alias const(wchar) cwchar;
	alias const(dchar) cdchar;
	alias immutable(char) ichar;
	alias immutable(wchar) iwchar;
	alias immutable(dchar) idchar;
	alias const(char)[] cstring;
	
//	alias immutable(void)	ivoid;
	alias const(void)		cvoid;
	typedef immutable(void)*	SFMLClass;
}

// used to mixin code function
string loadFromSharedLib(string fname)
{
	return fname ~ " = " ~ "cast(typeof(" ~ fname ~ ")) dll.getSymbol(\"" ~ fname ~ "\");";
}

//used to mixin code function
string loadFromSharedLib2(S...)(string lib, string className, S fnames)
{
	string res = `static this()
{
	debug
		DllLoader dll = DllLoader.load("` ~ lib ~ `-d-2");
	else
		DllLoader dll = DllLoader.load("` ~ lib ~ `-2");

`;

	foreach(fname; fnames)
	{
		res ~= "\t" ~ className ~ "_" ~ fname ~ " = " ~ "cast(typeof(" ~ className ~ "_" ~ fname ~ ")) dll.getSymbol(\"" ~ className ~ "_" ~ fname ~ "\");\n";
	}
	return res ~ "}\n";
}

string loadDerivedFromSharedLib(S...)(string lib, string baseClass, string derivedClass, S fnames)
{
	string res = `static this()
{
	debug
		DllLoader dll = DllLoader.load("` ~ lib ~ `-d-2");
	else
		DllLoader dll = DllLoader.load("` ~ lib ~ `-2");

`;

	foreach(fname; fnames)
	{
		res ~= "\t" ~ baseClass ~ "_" ~ fname ~ " = " ~ "cast(typeof(" ~ baseClass ~ "_" ~ fname ~ ")) dll.getSymbol(\"" ~ derivedClass ~ "_" ~ fname ~ "\");\n";
	}
	return res ~ "}\n";
}

/**
 *	Base class for all DSFML classes.
 */
class DSFMLObject
{
private:
	bool m_preventDelete;

protected:
	SFMLClass m_ptr;

	abstract void dispose();

public:
	/// get the underlying C pointer
	@property final SFMLClass nativePointer()
	{
		return m_ptr;
	}

public:

	this(SFMLClass ptr, bool preventDelete = false)
	{
		m_ptr = ptr;
		m_preventDelete = preventDelete;
	}
	
	~this()
	{
		if (!m_preventDelete)
			dispose();
			
		m_ptr = m_ptr.init;
	}
	

	final void setHandled(bool handled)
	{
		m_preventDelete = handled;
	}
	
	override bool opEquals(Object o)
	{
		return (m_ptr == (cast(DSFMLObject)o).nativePointer);
	}
	
	protected invariant()
	{
		assert(m_ptr !is null, "Problem occurs with a null pointer in " ~ this.toString);
	}
}
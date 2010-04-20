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

module dsfml.system.vector2;

/**
 *	Vector2 is an utility class for manipulating 2 dimensional
 *	vectors. Template parameter defines the type of coordinates
 *	(integer, float, ...)
 */
struct Vector2(T)
{
	T x;
	T y;

	/// negate the vector
	Vector2 opUnary(string op : "-")()
	{
		return Vector2!(T)(-x, -y);
	}

	/// dot product
	T opBinary(string op : "*", U:Vector2)(U v)
	{
		return x*v.x + y*v.y;
	}

	/// element-wise operations, +, -, 
	Vector2 opBinary(string op, U:Vector2)(U v)
	if (op != "*")
	{
//		pragma(msg, "opBinary!"~op);
		mixin("return Vector2!(T)( cast(T)(x " ~ op ~ " v.x), cast(T)(y " ~ op ~ " v.y) );");
	}
	
	/// operations with a scalar
	Vector2 opBinary(string op)(int i)
	{
		mixin("return Vector2!(T) ( cast(T)(x " ~ op ~ " i), cast(T)(y " ~ op ~ " i) );");
	}
	
	/// element-wise assign operations, +=, -=, ...
	Vector2 opOpAssign(string op, U:Vector2)(U v)
	{
		mixin("x " ~ op ~ " v.x;");
		mixin("y " ~ op ~ " v.y;");
		return this;
	}
	


	/// (*=) overload
	Vector2 opOpAssign(string op)(int i)
	{
		mixin("x "~op~" i;");
		mixin("y "~op~" i;");
		return this;
	}

	///
	const bool opEquals(ref const(Vector2) other)
	{
		return (x == other.x) && (y == other.y);
	}
	
	T max()
	{
		return (x>y) ? x : y;
	}
}

unittest
{
	Vector2f main = Vector2f(10f, 10f);
	Vector2f other = Vector2f(10f, 10f);
	Vector2f result;
	
	result = -main;
	assert (result == Vector2f(-10.f, -10.f) );
	
	result = main;
	result += other;
	assert (result == Vector2f(20.f, 20.f));
	
	result = main;
	result -= other;
	assert (result == Vector2f(0.f, 0.f));
	
	result = main + other;
	assert (result == Vector2f(20.f, 20.f));
	
	result = main - other;
	assert (result == Vector2f(0.f, 0.f));
	
	result = main * 10;
	assert (result == Vector2f(100.f, 100.f));
	
	result *= 2;
	assert (result == Vector2f(200.f, 200.f));
	
	result = main / 2;
	assert (result == Vector2f(5.f, 5.f));
	
	result = main;
	result /= 2;
	assert (result == Vector2f(5.f, 5.f));
}

/// Aliases
alias Vector2!(float) Vector2f;
/// ditto
alias Vector2!(int) Vector2i;
/// ditto
alias Vector2!(uint) Vector2ui;
/// ditto
alias Vector2!(ubyte) Vector2ub;
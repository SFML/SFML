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

	/// unary (-) overload
	Vector2 opNeg()
	{
		return Vector2!(T)(-x, -y);
	}

	/// (+=) overload
	Vector2 opAddAssign(Vector2 other)
	{
		x += other.x;
		y += other.y;
		return this;
	}

	/// (-=) overload
	Vector2 opSubAssign(Vector2 other)
	{
		x -= other.x;
		y -= other.y;
		return this;
	}

	/// (+) overload
	Vector2 opAdd(Vector2 other)
	{
		return Vector2!(T)( cast(T)(x + other.x), cast(T)(y + other.y) );
	}

	/// (-) overload
	Vector2 opSub(Vector2 other)
	{
		return Vector2!(T) ( cast(T)(x - other.x), cast(T)(y - other.y) );
	}
	
	/// (*) overload
	Vector2 opMul(int i)
	{
		return Vector2!(T) ( cast(T)(x * i), cast(T)(y * i) );
	}

	/// (*=) overload
	Vector2 opMulAssign(int i)
	{
		x *= i;
		y *= i;
		return this;
	}

	/// (/) overload
	Vector2 opDiv(int i)
	{
		return Vector2!(T) ( cast(T)(x / i), cast(T)(y / i));
	}

	/// (/=) overload
	Vector2 opDivAssign(int i)
	{
		x /= i;
		y /= i;
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

version (UnitTest)
{
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
}

/// Aliases
alias Vector2!(float) Vector2f;
/// ditto
alias Vector2!(int) Vector2i;

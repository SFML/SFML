/*
*	DSFML - SFML Library wrapper for the D programming language.
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
module dsfml.system.vector;

import std.conv;
import std.math;
import std.traits : isFloatingPoint;

import std.typetuple;

/**
 *	generic fixed-size Vector struct
 *
 *	Params:
 *		T	= element type
 *		dim	= vector dimension
 */
struct Vector(T, uint dim)
{
	static assert (dim >= 2 && dim <= 4);
	
	// vectors of 3 floats are extended to 4 to make it possible to use SSE optimizations
	private const realdim = (is(T == float) && dim == 3 && sseAvailable) ? 4 : dim;
	// vectors of (3)4 floats or 2 doubles will use SSE
	private const bool useSSE = (is(T == float) && realdim == 4 /* || is(T == double) && dim == 2 */ ) && sseAvailable;
	
	private alias LengthReturnType!(T) LengthType; // the type returned by length
	union
	{
		/// normal struct element access
		struct
		{
			static if (dim >= 1)	T x;
			static if (dim >= 2)	T y;
			static if (dim >= 3)	T z;
			static if (dim >= 4)	T w;
		}

		struct
		{
			static if (dim >= 1)	T r;
			static if (dim >= 2)	T g;
			static if (dim >= 3)	T b;
			static if (dim >= 4)	T a;
		}
		
		// only the array has the hidden 4th value in case of vec3f
		// this is to be able to foreach over tuple without computing w unnecessarily
		T[realdim] cell;		/// array access
		Repeat!(T, dim) tuple;	/// for tuple access
	}

	// zero vectors
	static if (2 == dim) const static Vector zero = Vector(0, 0);
	static if (3 == dim) const static Vector zero = Vector(0, 0, 0);
	static if (4 == dim) const static Vector zero = Vector(0, 0, 0, 0);

	static if (2 == dim) const static Vector one = Vector(1, 1);
	static if (3 == dim) const static Vector one = Vector(1, 1, 1);
	static if (4 == dim) const static Vector one = Vector(1, 1, 1, 1);

	static if (2 == dim) const static Vector unitX = Vector(1, 0);
	static if (3 == dim) const static Vector unitX = Vector(1, 0, 0);
	static if (4 == dim) const static Vector unitX = Vector(1, 0, 0, 0);

	static if (2 == dim) const static Vector unitY = Vector(0, 1);
	static if (3 == dim) const static Vector unitY = Vector(0, 1, 0);
	static if (4 == dim) const static Vector unitY = Vector(0, 1, 0, 0);

	static if (3 == dim) const static Vector unitZ = Vector(0, 0, 1);
	static if (4 == dim) const static Vector unitZ = Vector(0, 0, 1, 0);

	static if (4 == dim) const static Vector unitW = Vector(0, 0, 0, 1);

	
	/// ensure that no component is a NaN
	invariant()
	{
		assert(isValid());
	}

	// checks if the elements aren't NaNs
	private bool isValid() const
	{
		static if (dim >= 1) if (isNaN(x)) return false;
		static if (dim >= 2) if (isNaN(y)) return false;
		static if (dim >= 3) if (isNaN(z)) return false;
		static if (dim >= 4) if (isNaN(w)) return false;
		return true;
	}

	/************************************************************************************
	 * Operator overloading
	 ***********************************************************************************/
	
	/// negate the vector
	Vector opUnary(string op : "-")() const
	{
				static if (dim == 2) return Vector(-x, -y);
		else	static if (dim == 3) return Vector(-x, -y, -z);
		else	static if (dim == 4) return Vector(-x, -y, -z, -w);
	}
	
	/// dot product
	T opBinary(string op : "*")(typeof(this) v) const
	if (is(typeof(T+T)) && is(typeof(T*T)))
	{
				static if (dim == 2) return x*v.x + y*v.y;
		else	static if (dim == 3) return x*v.x + y*v.y + z*v.z;
		else	static if (dim == 4) return x*v.x + y*v.y + z*v.z + w*v.w;
	}

	/// element-wise operations, +, -, 
	Vector opBinary(string op, U:typeof(this))(U v) const
	// check if the operation is supported on the type T
	if (op != "*" && (op == "+" && is(typeof(T+T)) || op == "-" && is(typeof(T-T)) || op == "*" && is(typeof(T*T))
					|| op == "/" && is(typeof(T/T))	|| op == "%" && is(typeof(T%T))))
	{
		Vector res = void;
		foreach (i, x; tuple)
			mixin("res.tuple[i] = tuple[i] " ~ op ~ " v.tuple[i];");
		return res;
	}

	/// operations with a scalar
	Vector opBinary(string op, U)(U s) const
	{
		Vector res = void;
		foreach(i, x; tuple)
			mixin("res.tuple[i] = tuple[i] " ~ op ~ " s;");
		return res;
	}
	
	/// element-wise assign operations, +=, -=, ...
	Vector opOpAssign(string op, U:Vector)(U v)
	{
		foreach (i, _; tuple)
			mixin("tuple[i] " ~ op ~ "= v.tuple[i];");

		return this;
	}

	/// (*=) overload
	Vector opOpAssign(string op, U)(U s)
	if (!is(U:typeof(this))) // TODO: there's some dmd bug about this
	{
		foreach (i, _; tuple)
			mixin("tuple[i] " ~ op ~ "= s;");

		return this;
	}

	/// return length*length
	@property LengthType sqLength()
	{
				static if (2 == dim) return (x * x + y * y);
		else	static if (3 == dim) return (x * x + y * y + z * z);
		else	static if (4 == dim) return (x * x + y * y + z * z + w * w);
		else	static assert (false);
	}

	/// return the vector length
	@property LengthType length()
	{
		static if (useSSE)
		{
				static if (is(t == float) && dim == 3) // make sure that w is 0 
					assert(w == 0);
			
			float res;
			auto p = cell.ptr;
			asm
			{
//				movups XMM0, &cell;
				mov EAX, p;
				movups XMM0, [EAX];
				mulps XMM0, XMM0;			// v0 = vec(x*x, y*y, z*z, w*w)
				movaps XMM1, XMM0;			// v1 = v0
				shufps XMM0, XMM1, 0x4e;	// v0 = vec(z*z, w*w, x*x, y*y)
				addps XMM0, XMM1;			// v0 = vec(x*x + z*z, y*y + w*w, z*z + x*x, w*w + y*y)
				movaps XMM1, XMM0;			// v1 = v0
				shufps XMM1, XMM1, 0x11;	// v1 = vec(w*w + y*y, z*z + x*x, w*w + y*y, z*z + x*x)
				addps XMM0, XMM1;			// v0 = |vec|^2 at all 4 positions
				rsqrtss XMM0, XMM0;			// v0 = 1/sqrt(v0)
				rcpss XMM0, XMM0;			// v= = 1/v0
				movss res, XMM0;
			}
			return res;
		}
		else
		{
			// compute squared length
			auto ret = sqLength();
			
			// compute sqrt
			version(useFastSqrt)
			{
				static if (is(T == float))
					return fastSqrt(ret);
			}
			return sqrt(ret);
		}
	}

	void normalize()
	{
		static if (useSSE)
		{
			static if (is(t == float) && dim == 3) // make sure that w is 0 
				assert (w == 0, "vector component w isn't 0!");
		
			auto p = cell.ptr;
			asm
			{
				mov EAX, p;
				movups XMM0, [EAX];
				movaps XMM2, XMM0;			// save it for later

				mulps XMM0, XMM0;			// v0 = vec(x*x, y*y, z*z, w*w)
				movaps XMM1, XMM0;			// v1 = v0
				shufps XMM0, XMM1, 0x4e;	// v0 = vec(z*z, w*w, x*x, y*y)
				addps XMM0, XMM1;			// v0 = vec(x*x + z*z, y*y + w*w, z*z + x*x, w*w + y*y)
				movaps XMM1, XMM0;			// v1 = v0
				shufps XMM1, XMM1, 0x11;	// v1 = vec(w*w + y*y, z*z + x*x, w*w + y*y, z*z + x*x)
				addps XMM0, XMM1;			// v0 = |vec|^2 at all 4 positions
				rsqrtps XMM0, XMM0;			// v0 = 1/sqrt(v0)
				mulps XMM2, XMM0;			// v2 = vec * v0

				movups [EAX], XMM0;
			}
		}
		else
		{
			auto len = length();
			foreach(i, _; tuple) // bug 2411 workaround, foreach ref on tuples doesn't work
				tuple[i] /= len;
		}
	}

	/// return normalized version of this vector
	Vector normalized()
	{
		Vector res = this;
		res.normalize();
		return res;
	}

	///
	string toString()
	{
		string res = "[";

		res ~= to!(string)(x);
		static if (dim >= 2) res ~= ", " ~ to!(string)(y);
		static if (dim >= 3) res ~= ", " ~ to!(string)(z);
		static if (dim >= 4) res ~= ", " ~ to!(string)(w);

		return res ~ "]";
	}
	
	static if (is(T == float))
	{
		/// do a quick normalize using fast approximate inverse sqrt
		void quickNormalize()
		{
			T inv = invSqrt(sqLength);
			this *= inv;
		}

		/// return a normalized version of this vector
		Vector quickNormalized()
		{
			auto res = this;
			res.quickNormalize();
			return res;
		}
	}
	
	/// return a pointer to the vector data
	@property T* ptr()
	{
		return &x;
	}
	
	/// calculate distance to other vector
	LengthType distance(Vector other)
	{
		assert (isValid);
		assert (other.isValid);
		other -= this; // doable cause other is a struct not ref
		return other.length;
	}
	
	///
	bool opEquals(ref const Vector v) const
	{
		assert (isValid);
		assert (v.isValid);
		
		static if (dim >= 1) if (x != v.x) return false;
		static if (dim >= 2) if (y != v.y) return false;
		static if (dim >= 3) if (z != v.z) return false;
		static if (dim >= 4) if (w != v.w) return false;
		return true;
	}
	
	/// swizzling
	@property Vector!(T,n.length) opDispatch(string n)() const
	if (allCharsValid(n,"xyzw"[0..dim]))
	{
		static if (n.length == 2) return  
			Vector!(T,n.length)(cell[n[0]-'x'], cell[n[1]-'x']);
		static if (n.length == 3) return  
			Vector!(T,n.length)(cell[n[0]-'x'], cell[n[1]-'x'], cell[n[2]-'x']);
		static if (n.length == 4) return  
			Vector!(T,n.length)(cell[n[0]-'x'], cell[n[1]-'x'], cell[n[2]-'x'], cell[n[3]-'x']);
	}
	
	// helper function
	static private bool allCharsValid( string s, string valid )
	{
		foreach ( e1; s )
		{
			bool b = false;
			foreach (e2; valid)
				b |= e1 == e2;
			if (!b)
				return false;
		}
		return true;
	}
	
	///
	bool isUnit()
	{
		real sql = cast(real)sqLength();
		return abs(sql - 1.0) < 0.001;
	}

}

/******* useful alias declarations *******/

alias Vector!(float, 2)	Vector2f; ///
alias Vector!(float, 3)	Vector3f; ///
alias Vector!(float, 4)	Vector4f; ///

alias Vector!(double, 2) Vector2d; ///
alias Vector!(double, 3) Vector3d; ///
alias Vector!(double, 4) Vector4d; ///

alias Vector!(int, 2)	Vector2i; ///
alias Vector!(int, 3)	Vector3i; ///
alias Vector!(int, 4)	Vector4i; ///

alias Vector!(uint, 2)	Vector2ui; ///
alias Vector!(uint, 3)	Vector3ui; ///
alias Vector!(uint, 4)	Vector4ui; ///

alias Vector!(ushort, 2) Vector2us; ///
alias Vector!(ushort, 3) Vector3us; ///
alias Vector!(ushort, 4) Vector4us; ///

alias Vector!(ubyte, 2) Vector2ub; ///
alias Vector!(ubyte, 3) Vector3ub; ///
alias Vector!(ubyte, 4) Vector4ub; ///


// TODO: do all kinds of unittesting
import std.stdio;
unittest
{
	writeln("unittests running");
	Vector3f v = {1.5f, 1.f, 0.5f};
	Vector3f w = {-1.f, 2.f, -0.5f};

	writefln("v: %f w: %f", v.length - sqrt(3.5f), w.length - sqrt(5.25f));
	// strangely calculating w.length is much less accurate
	assert(v.length - sqrt(3.5f) < 0.001, sseAvailable ? "SSE length calculation failed" : "normal length calculation failed");
	assert(w.length - sqrt(5.25f) < 0.001, sseAvailable ? "SSE length calculation failed" : "normal length calculation failed");

	assert(v+w == Vector3f(0.5f, 3.f, 0.f));
	assert(v-w == Vector3f(2.5f, -1.f, 1.f));
	
	auto r = v.xy;
	writeln(r);
}

/**
 *	compute 1/sqrt(x)
 *	assumes x > 0
 *
 *	Copyright (C) 2002-2006 Chris Lomont
 *	explanation on www.lomont.org
 */
float invSqrt(float x)
{
	assert(x > 0);

	float xhalf = 0.5f * x;
	int i = *cast(int*)&x;			// get bits for floating value
	i = 0x5f375a86 - (i >> 1);		// gives initial guess y0 with magic number
	x = *cast(float*)&i;			// convert bits back to float
	x = x*(1.5f - xhalf * x * x);	// Newton step, repeating increases accuracy
	return x;
}


/**
 *	compute sqrt(x)
 *	assumes x >= 0
 */
float fastSqrt(float x)
{
	assert(x >= 0);

	int i = *cast(int*) &x;
	if (0 == ((i >> 23)&255))
		return 0; // close
	return x * invSqrt(x);
}

// get the correct return type for the length function
private template LengthReturnType(T)
{
	static if (is(T == float) || is(T == double) || is(T == real))
		alias T LengthReturnType;
	else
		alias float LengthReturnType;
}

/// repeat a type count times
template Repeat(T, int count)
{
	static if (!count)
		alias TypeTuple!() Repeat;
	else
		alias TypeTuple!(T, Repeat!(T, count-1)) Repeat;
}

// determine SSE usability
// TODO: make more sophisticated
version(X86)
	version(D_InlineAsm_X86)
		const bool sseAvailable = is(typeof({void* foo; asm { mov EAX, foo; movups XMM0, [EAX]; }  }));
version(X86_64)
	version(D_InlineAsm_X86_64)
		const bool sseAvailable = false; // TODO: add this
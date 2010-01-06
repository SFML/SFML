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

module dsfml.system.vector3;

/**
*   Vector3 is an utility class for manipulating 3 dimensional
*   vectors. Template parameter defines the type of coordinates
*   (integer, float, ...)
*/
struct Vector3(T)
{
    T x;
    T y;
    T z;

    static Vector3 opCall(T x, T y, T z)
    {
        Vector3!(T) ret;
        
        ret.x = x;
        ret.y = y;
        ret.z = z;
        
        return ret;
    }

    /// unary (-) overload
    Vector3 opNeg()
    {
        return Vector3!(T)(-x, -y, -z);
    }

    /// (+=) overload
    Vector3 opAddAssign(Vector3 other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return this;
    }

    /// (-=) overload
    Vector3 opSubAssign(Vector3 other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return this;
    }

    /// (+) overload
    Vector3 opAdd(Vector3 other)
    {
        return Vector3!(T)( (x + other.x), (y + other.y), (z + other.z) );
    }

    /// (-) overload
    Vector3 opSub(Vector3 other)
    {
        return Vector3!(T) ( (x - other.x), (y - other.y), (z - other.z) );
    }

    /// (*) overload
    Vector3 opMul(int i)
    {
        return Vector3!(T) ( (x * i), (y * i), (z * i) );
    }

    /// (*=) overload
    Vector3 opMulAssign(int i)
    {
        x *= i;
        y *= i;
        z *= i;
        return this;
    }

    /// (/) overload
    Vector3 opDiv(int i)
    {
        return Vector3!(T) ( (x / i), (y / i), (z / i) );
    }

    /// (/=) overload
    Vector3 opDivAssign(int i)
    {
        x /= i;
        y /= i;
        z /= i;
        return this;
    }

    ///
    const bool opEquals(ref const(Vector3) other)
    {
        return (x == other.x) && (y == other.y) && (z == other.z) ;
    }
}

version (UnitTest)
{
    unittest
    {
        Vector3f main = Vector3f(10f, 10f, 10.f);
        Vector3f other = Vector3f(10f, 10f, 10.f);
        Vector3f result;
        
        result = -main;
        assert (result == Vector3f(-10.f, -10.f, -10.f) );
        
        result = main;
        result += other;
        assert (result == Vector3f(20.f, 20.f, 20.f));
        
        result = main;
        result -= other;
        assert (result == Vector3f(0.f, 0.f, 0.f));
        
        result = main + other;
        assert (result == Vector3f(20.f, 20.f, 20.f));
        
        result = main - other;
        assert (result == Vector3f(0.f, 0.f, 0.f));
        
        result = main * 10;
        assert (result == Vector3f(100.f, 100.f, 100.f));
        
        result *= 2;
        assert (result == Vector3f(200.f, 200.f, 200.f));
        
        result = main / 2;
        assert (result == Vector3f(5.f, 5.f, 5.f));
        
        result = main;
        result /= 2;
        assert (result == Vector3f(5.f, 5.f, 5.f));
    }
}

/// Aliases
alias Vector3!(float) Vector3f;
/// ditto
alias Vector3!(int) Vector3i;

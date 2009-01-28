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

module dsfml.system.stringutil;

// version (Tango)
// {
//     public import tango.stdc.stringz;
// }
// else
// {
    T* toStringz(T)(T[] str)
    {
        if (str is null)
            return null;
        else if (str.length && str[$ - 1] is T.init)
            return str.ptr;

        T[] ret = new T[str.length + 1];
        
        ret[0 .. str.length] = str[0 .. $];
        ret[str.length] = 0;
        
        return ret.ptr;
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
        T[] ret = new T[stringLength(ptr)];
        ret[0..$] = ptr[0..ret.length];
        
        return ret;
    }

    
    
//     /*
//     *   Tango equivalent functions
//     *   
//     *   Author : Keinfarbton    
//     *   Licence : BSD style        
//     */    
//     char* toStringz(char[] s)
//     {
//         if (s.ptr)
//             if (!(s.length && s[$-1] is 0))
//                    s = s ~ '\0';
//         return s.ptr;
//     }
//     
//     char[] fromStringz (char* s)
//     {
//         size_t i;
//     
//         if (s)
//             while (*(s+i))
//                 ++i;
//     
//         return s ? s[0 .. i] : null;
//     }
//     
//     wchar* toString16z(wchar[] s)
//     {
//         if (s.ptr)
//             if (!(s.length && s[$-1] is 0))
//                    s = s ~ '\0';
//         return s.ptr;
//     }
//     
//     wchar[] fromString16z (wchar* s)
//     {
//         size_t i;
//     
//         if (s)
//             while (*(s+i))
//                 ++i;
//     
//         return s ? s[0 .. i] : null;
//     }
// 
//     dchar* toString32z (dchar[] s)
//     {
//         if (s.ptr)
//             if (!(s.length && s[$-1] is 0))
//                    s = s ~ "\0"d;
//         return s.ptr;
//     }
//     
//     
//     dchar[] fromString32z (dchar* s)
//     {
//         size_t i;
//     
//         if (s)
//             while (*(s+i))
//                 ++i;
//     
//         return s ? s[0 .. i] : null;
//     }
// }
version (UnitTest)
{
    void main()
    {
        
    }
    unittest
    {
        char[] str = "Test";
        char[] espaceStr = "string with espace";
        
        dchar[] strW = "Test"d;
        dchar[] espaceStrW = "string with espace"d;
        
        char[] empty = "";
        dchar[] emptyW = ""d;
        
        char[] nullStr = null;
        dchar[] nullStrW = null;
        
        assert(fromStringz(toStringz(str)) == str);
        assert(fromStringz(toStringz(espaceStr)) == espaceStr);
        assert(fromStringz(toStringz(strW)) == strW);
        assert(fromStringz(toStringz(espaceStrW)) == espaceStrW);
        
        assert(fromStringz(toStringz(empty)) == empty);
        assert(fromStringz(toStringz(emptyW)) == emptyW);
        
        assert(fromStringz(toStringz(nullStr)) == nullStr);
        assert(fromStringz(toStringz(nullStrW)) == nullStrW); 
    }
}

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

module dsfml.graphics.color;

import std.string;

alias RGBA Color; // standard Color is RGBA

/**
 *	Color is an utility structure for manipulating colors
 */
struct RGBA
{
align(1):
	ubyte r;		/// Red component
	ubyte g;		/// Green component
	ubyte b;		/// Blue component
	ubyte a = 255;	/// Alpha (transparency) component, 255 = opaque

	/**
	 *  Operator == and != overload to compare two colors	
	 */
	const bool opEquals(ref const(Color) color2)
	{
		return 
			(r == color2.r) 
			&& (g == color2.g) 
			&& (b == color2.b) 
			&& (a == color2.a);
	}
	/**
	 *	Operator + overload to add two colors
	 */
	Color opAdd(Color color2)
	{
		ubyte r = this.r + color2.r > 255 ? 255 : cast(ubyte) (this.r + color2.r);
		ubyte g = this.g + color2.g > 255 ? 255 : cast(ubyte) (this.g + color2.g);
		ubyte b = this.b + color2.b > 255 ? 255 : cast(ubyte) (this.b + color2.b);
		ubyte a = this.a + color2.a > 255 ? 255 : cast(ubyte) (this.a + color2.a);
	
		return Color(r, g, b, a);
	}

	/**
	 *	Operator += overload
	 */
	Color opAddAssign(Color color2)
	{
		this.r = this.r + color2.r > 255 ? 255 : cast(ubyte) (this.r + color2.r);
		this.g = this.g + color2.g > 255 ? 255 : cast(ubyte) (this.g + color2.g);
		this.b = this.b + color2.b > 255 ? 255 : cast(ubyte) (this.b + color2.b);
		this.a = this.a + color2.a > 255 ? 255 : cast(ubyte) (this.a + color2.a);
		
		return this;
	}

	/**
	 *	Operator * overload to modulate colors
	 */
	Color opMul(Color color2)
	{
		ubyte r = cast(ubyte) (this.r * color2.r / 255);
		ubyte g = cast(ubyte) (this.g * color2.g / 255);
		ubyte b = cast(ubyte) (this.b * color2.b / 255);
		ubyte a = cast(ubyte) (this.a * color2.a / 255);
		
		return Color(r, g, b, a);
	}

	/**
	 *	Operator *= overload
	 */
	Color opMulAssign(Color color2)
	{
		this.r = cast(ubyte) (this.r * color2.r / 255);
		this.g = cast(ubyte) (this.g * color2.g / 255);
		this.b = cast(ubyte) (this.b * color2.b / 255);
		this.a = cast(ubyte) (this.a * color2.a / 255);
		
		return this;
	}
	
	string toString()
	{
		return std.string.format("(%d,%d,%d,%d)", r,g,b,a);
	}
	
	static immutable
	{
		Color BLACK		= Color(0, 0, 0);		/// Black predefined color
		Color WHITE		= Color(255, 255, 255);	/// White predefined color
		Color RED		= Color(255, 0, 0);		/// Red predefined color
		Color GREEN		= Color(0, 255, 0);		/// Green predefined color
		Color BLUE		= Color(0, 0, 255);		/// Blue predefined color
		Color YELLOW	= Color(255, 0, 255);	/// Yellow predefined color
		Color MAGENTA	= Color(255, 0, 255);	/// Magenta predefined color
		Color CYAN		= Color(0, 255, 255);	/// Cyan predefined color
	}
}
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

module dsfml.graphics.rect;

version (Tango)
{
	import tango.core.Traits;
}
else
{
	template isIntegerType(T)
	{
		const bool isIntegerType = 
			is (T == byte) ||
			is (T == short) ||
			is (T == int) ||
			is (T == long);
	}

	template isRealType(T)
	{
		const bool isRealType =
			is (T == float) ||
			is (T == double) ||
			is (T == real);
	}
}

/**
*	Rect is an utility class for manipulating rectangles.
*	Template parameter defines the type of coordinates (integer float, ...)
*/
struct Rect(T)
{

private:
	T left;	// Left coordinate of the rectangle
	T top;	// Top coordinate of the rectangle
	T right;  // Right coordinate of the rectangle
	T bottom; // Bottom coordinate of the rectangle

public:
	static if (!isIntegerType!(T) && !isRealType!(T))
	{
		static assert (0, "This type is not supported by Rectangle");
	}
	
	T min(T)(T i, T j)
	{
		return i < j ? i : j;
	}
	
	T max(T)(T i, T j)
	{
		return i > j ? i : j;
	}
	
/+
	/**
	*	Construct the rectangle from its coordinates
	*
	*	Params:
	*		leftCoord = Left coordinate of the rectangle
	*		topCoord = Top coordinate of the rectangle
	*		rightCoord = Right coordinate of the rectangle
	*		bottomCoord = Bottom coordinate of the rectangle
	*/
	this(T leftCoord, T topCoord, T rightCoord, T bottomCoord)
	{
		left = leftCoord;
		top = topCoord;
		right = rightCoord;
		bottom = bottomCoord;
	}
+/

	/**
	*	Get the width of the rectangle
	*
	*	Returns:
	*		Width of rectangle
	*/
	T getWidth() 
	{
		return right - left;
	}

	/**
	*	Get the height of the rectangle
	*
	*	Returns:
	*		Height of rectangle
	*/
	T getHeight()
	{
		return bottom - top;
	}

	/**
	*	Move the whole rectangle by the given offset
	*
	*	Params:
	*		offsetX = Horizontal offset
	*		offsetY = Vertical offset
	*/
	void offset(T offsetX, T offsetY)
	{
		left	+= offsetX;
		right  += offsetX;
		top	+= offsetY;
		bottom += offsetY;
	}

	/**
	*	Check if a point is inside the rectangle's area
	*
	*	Params:
	*		x = X coordinate of the point to test
	*		y = Y coordinate of the point to test
	*
	*	Returns:
	*		True if the point is inside
	*/
	bool contains(T x, T y)
	{
		return (x >= left) && (x <= right) && (y >= top) && (y <= bottom);
	}

	/**
	*	Check intersection between two rectangles
	*
	*	Params:
	*		rectangle = Rectangle to test
	*		overlappingRect = Rectangle to be filled with overlapping rect (NULL by default)
	*
	*	Returns:
	*		True if rectangles overlap
	*/
	bool intersects(Rect!(T) rectangle, out Rect!(T) overlappingRect = Rect!(T)())
	{
		// Compute overlapping rect
		auto overlapping = Rect!(T)(	
										max(left,	rectangle.left),
										max(top,	rectangle.top),
										min(right,  rectangle.right),
										min(bottom, rectangle.bottom)
									);
	
		// If overlapping rect is valid, then there is intersection
		if ((overlapping.left < overlapping.right) && (overlapping.top < overlapping.bottom))
		{
			overlappingRect = overlapping;
			return true;
		}
		else
		{
			overlappingRect = Rect!(T)();
			return false;
		}
	}
	
	//bool opEquals
}

///Alias
alias Rect!(int) IntRect;
///ditto
alias Rect!(float) FloatRect;
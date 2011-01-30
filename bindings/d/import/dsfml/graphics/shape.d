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

module dsfml.graphics.shape;

import dsfml.system.vector;

import dsfml.graphics.blendmode;
import dsfml.graphics.color;
import dsfml.graphics.drawableimpl;

/**
 *	Shape defines a drawable convex shape ; it also defines
 *	helper functions to draw simple shapes like
 *	lines, rectangles, circles, etc.
 */
class Shape : DrawableImpl!("sfShape")
{
private:
	this (SFMLClass ptr)
	{
		super(ptr);
	}
	
public:
	this()
	{
		super();
	}

	/**
	 *	Add a point to the shape
	 *
	 *	Params:
	 *		x = X position of the point
	 *		y = Y position of the point
	 *		col = Color of the point (white by default)
	 *		outlineCol = Outline color of the point (black by default)
	 */		
	void addPoint(float x, float y, Color col = Color.WHITE, Color outlineCol = Color.BLACK)
	{
		sfShape_AddPoint(m_ptr, x, y, col, outlineCol);
	}
 
	/**
	 *	Add a point to the shape
	 *
	 *	Params:
	 *		position = position of the point
	 *		col = Color of the point (white by default)
	 *		outlineCol = Outline color of the point (black by default)
	 */		
	void addPoint(Vector2f position, Color col = Color.WHITE, Color outlineCol = Color.BLACK)
	{
		sfShape_AddPoint(m_ptr, position.x, position.x, col, outlineCol);
	}

@property
{
	/**
	 *	Enable or disable filling the shape.
	 *	Fill is enabled by default.
	 *	
	 *	Params:
	 *		enable = True to enable, false to disable				
	 */		
	void enableFill(bool enable)
	{
		sfShape_EnableFill(m_ptr, enable);
	}

	/**
	 *	Enable or disable drawing a shape outline.
	 *	Outline is enabled by default
	 *
	 *	Params:
	 *		enable = True to enable, false to disable
	 */
	void enableOutline(bool enable)
	{
		sfShape_EnableOutline(m_ptr, enable);
	}

	/**
	 *	Change the thickness of a shape outline
	 *
	 *	Params: 
	 *		thickness = New thickness
	 */
	void outlineThickness(float thickness)
	{
		sfShape_SetOutlineThickness(m_ptr, thickness);
	}

	/**
	 *	Get the thickness of the shape outline
	 *
	 *	Returns:
	 *		Current outline thickness
	 *
	 */
	float outlineThickness() 
	{
		return sfShape_GetOutlineThickness(m_ptr);
	}

	/**
	 *	Get the number of points composing a shape
	 *
	 *	Returns:
	 *		Total number of points
	 */
	uint pointsCount()
	{
		return sfShape_GetPointsCount(m_ptr);
	}
}

	/**
	 *	Get a point of the shape
	 *
	 *	Params:	
	 *		index = Index of the point
	 *		
	 *	Returns:
	 *		position of the point			
	 */
	Vector2f getPointPosition(uint index)
	{
		float x, y;
		sfShape_GetPointPosition(m_ptr, index, &x, &y);
		return Vector2f(x, y);
	}

	/**
	 *	Set the position of a shape point
	 *	
	 *	Params:
	 *		index = Index of the point
	 *		position = New position of the point				
	 */		
	void setPointPosition(uint index, Vector2f position)
	{
		sfShape_SetPointPosition(m_ptr, index, position.x, position.y);
	}

	/**
	 *	Get the color of a shape's point
	 *	
	 *	Params:	
	 *		index = Index of the point
	 *		
	 *	Returns:
	 *		Color of the point			 
	 */
	Color getPointColor(uint index)
	{
		return sfShape_GetPointColor(m_ptr, index);
	}

	/**
	 *	Set the color of a shape's point
	 *	
	 *	Params:	
	 *		index = Index of the point
	 *		color = new color of the point			
	 */		
	void setPointColor(uint index, Color color)
	{
		sfShape_SetPointColor(m_ptr, index, color);
	}

	/**
	 *	Get the outline color of a shape's point
	 *	
	 *	Params:
	 *		index = Index of the point
	 *		
	 *	Returns:
	 *		Color of the outline					  
	 */		
	Color getPointOutlineColor(uint index)
	{
		return sfShape_GetPointOutlineColor(m_ptr, index);
	}

	/**
	 *	Set the outline color of a shape's point
	 *	
	 *	Params:
	 *		index = Index of the point
	 *		color = new color of the point				
	 */		
	void setPointOutlineColor(uint index, Color color)
	{
		sfShape_SetPointOutlineColor(m_ptr, index, color);
	}


 
	/**
	 *	Create a shape made of a single line
	 *
	 *	Params:
	 *		p1X, p1Y = Position of the first point
	 *		p2X, p2Y = Position second point
	 *		thickness = Line thickness
	 *		col = Color used to draw the line
	 *		outline = Outline thickness (0 by default)
	 *		outlineCol = Color used to draw the outline (black by default)
	 *	
	 *	Returns:
	 *		New line shape		
	 */
	static Shape line(float p1X, float p1Y, float p2X, float p2Y, float thickness, Color col, float outline = 0.f, Color outlineCol = Color.BLACK)
	{
		
		return new Shape(sfShape_CreateLine(p1X, p1Y, p2X, p2Y, thickness, col, outline, outlineCol));
	}

	/**
	 *	Create a shape made of a single rectangle
	 *
	 *	Params:	
	 *		left, top = Top-left corner of the rectangle
	 *		width, height = Size of the rectangle
	 *		col = Color used to fill the rectangle
	 *		outline = Outline thickness (0 by default)
	 *		outlineCol = Color used to draw the outline (black by default)
	 *		
	 *	Returns:
	 *		new rectangle shape			
	 */
	static Shape rectangle(float left, float top, float width, float height, Color col, float outline = 0.f, Color outlineCol = Color.BLACK)
	{
		return new Shape(sfShape_CreateRectangle(left, top, width, height, col, outline, outlineCol));
	}

	/**
	 *	Create a shape made of a single circle
	 *
	 *	Params:	
	 *		x = X position of the center
	 *		y = Y position of the center	
	 *		radius = Radius
	 *		col = Color used to fill the circle
	 *		outline = Outline thickness (0 by default)
	 *		outlineCol = Color used to draw the outline (black by default)
	 *
	 *	Returns:
	 *		new circle shape		
	 */
	static Shape circle(float x, float y, float radius, Color col, float outline = 0.f, Color outlineCol = Color.BLACK)
	{
		return new Shape(sfShape_CreateCircle(x, y, radius, col, outline, outlineCol));
	}

private:
	
	static extern(C)
	{
		SFMLClass	function(float, float, float, float, float, Color, float, Color)	sfShape_CreateLine;
		SFMLClass	function(float, float, float, float, Color, float, Color)			sfShape_CreateRectangle;
		SFMLClass	function(float, float, float, Color, float, Color)					sfShape_CreateCircle;
		void	function(SFMLClass, float, float, Color, Color)							sfShape_AddPoint;
		void	function(SFMLClass, int)												sfShape_EnableFill;
		void	function(SFMLClass, int)												sfShape_EnableOutline;
		void	function(SFMLClass, float Width)										sfShape_SetOutlineThickness;
		float	function(SFMLClass)														sfShape_GetOutlineThickness;
		uint	function(SFMLClass)														sfShape_GetPointsCount;
		void	function(SFMLClass, uint Index, float* X, float* Y)						sfShape_GetPointPosition;
		void	function(SFMLClass, uint Index, float X, float Y)						sfShape_SetPointPosition;
		Color	function(SFMLClass, uint index)											sfShape_GetPointColor;
		void	function(SFMLClass, uint index, Color color)							sfShape_SetPointColor;
		Color	function(SFMLClass, uint index)											sfShape_GetPointOutlineColor;
		void	function(SFMLClass, uint index, Color color)							sfShape_SetPointOutlineColor;
	}

	mixin(loadFromSharedLib2("csfml-graphics", "sfShape",
	"CreateLine", "CreateRectangle", "CreateCircle", "AddPoint", "EnableFill", "EnableOutline", "SetOutlineThickness", "GetOutlineThickness",
	"GetPointsCount", "GetPointPosition", "SetPointPosition", "GetPointColor", "SetPointColor", "GetPointOutlineColor",
	"SetPointOutlineColor"));
}
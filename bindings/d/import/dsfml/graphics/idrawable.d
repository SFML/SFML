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

module dsfml.graphics.idrawable;

import dsfml.system.vector;

import dsfml.graphics.irendertarget;
import	dsfml.graphics.color,
		dsfml.graphics.blendmode,
		dsfml.graphics.shader;


/**
 *	Interface for drawable object
 *	
 *	Shape, Text and Sprite implement IDrawable
 */
interface IDrawable
{
	/**
	 *	Set the position of the object
	 *	
	 *	Params: 
	 *		x = New left coordinate
	 *		y = New top coordinate			
	 */
	void setPosition(float x, float y);

	/**
	 *	Set the scale of the object
	 *	
	 *	Params:
	 *		scaleX = New horizontal scale > 0
	 *		scaleY = New vertical scale > 0
	 */
	void setScale(float scalex, float scaley);
	// in {assert(scalex > 0 && scalex > 0);} // TODO: add in again when interface contracts work

	/**
	 *	Set the origin of the object, in coordinates relative to the
	 *	top-left of the object (take 2 values).
	 *	The default origin is (0, 0)
	 *
	 *	Params:
	 *		originX : X coordinate of the origin
	 *		originY : Y coordinate of the origin
	 */
	void setOrigin(float originX, float originY);

@property
{
	/**
	 *	Set the left position of the object
	 *	
	 *	Params:
	 *		x = New left coordinate			
	 */
	void x(float x);

	/**
	 *	Set the top position of the object
	 *	
	 *	Params:
	 *		y = New top coordinate			
	 */
	void y(float y);

	/**
	 *	Set the position of the object
	 *	
	 *	Params:
	 *		vec = new position			 
	 */
	void position(Vector2f vec);
	
	/**
	 *	Set the horizontal scale of the object
	 *	
	 *	Params:
	 *		scale = New horizontal scale (Strictly positive)
	 */
	void scaleX(float scale);
		
	/**
	 *	Set the vertical scale of the object
	 *	
	 *	Params:
	 *		scale = New vertical scale (Strictly positive)			
	 */
	void scaleY(float scale);

	/**
	 *	Set the scale of the object
	 *	
	 *	Params:
	 *		scale = new scale			
	 */
	void scale(Vector2f scale);
//	in {assert(scale.x > 0 && scale.y > 0);} // TODO
	
	/**
	 *	Set the origin of the object, in coordinates relative to the
	 *	top-left of the object (take a 2D vector).
	 *	The default origin is (0, 0)
	 *
	 *	Params:
	 *	  origin : New origin
	 */
	void origin(Vector2f origin);

			 
	/**
	 *	Set the rotation of the object
	 *	
	 *	Params: 
	 *		angle = Angle of rotation, in degree			
	 */
	void rotation(float angle);
		
	/**
	 *	Set the color
	 *	
	 *	Params:
	 *		c = New color			
	 */
	void color(Color c);
			
	/**
	 *	Set the blending mode for the object.
	 *	The default blend mode is Blend.Alpha
	 *
	 *	Params: 
	 *		mode = New blending mode
	 */
	void blendMode(BlendMode mode);

const
{
	/**
	 *	Get the position of the object
	 *
	 *	Returns:
	 *		Current position
	 *
	 */
	Vector2f position();

	/**
	 *	Get the current scale of the object
	 *	
	 *	Returns:
	 *		Current scale			
	 */
	Vector2f scale();
	
	/**
	 *	Get the origin of the object
	 *
	 *	Returns: 
	 *		Current position of the origin
	 *
	 */
	Vector2f origin();
	
	/**
	 *	Get the rotation angle of the object
	 *	
	 *	Returns: 
	 *		Angle of rotation, in degree			
	 */
	float rotation();
	
	/**
	 *	Get the color of the string
	 *	
	 *	Returns:
	 *		Current color				
	 */
	Color color();

	/**
	 *	Get the current blending mode
	 *
	 *	Returns: 
	 *		Current blending mode
	 */
	BlendMode blendMode();
} // const
} // @property

	/**
	 *	Rotate the object
	 *	Angle is added to the current orientation of the objet	
	 *	
	 *	Params:
	 *		angle = Angle of rotation in degree			
	 */
	void rotate(float angle);

	/**
	 *	Move the object
	 *	New offset is added to object current position	
	 *	
	 *	Params:
	 *		offsetX = Offset on the X axis
	 *		offsetY = Offset on the Y axis				
	 */
	void move(float offsetX, float offsetY);
	
	/**
	 *	Move the object
	 *	New offset is added to object current position	
	 *	
	 *	Params:
	 *		offset = Amount of units to move the object of			 
	 */
	void move(Vector2f offset);

	/**
	 *	Transform a point from global coordinates into local coordinates
	 *	(ie it applies the inverse of object's origin, translation, rotation and scale to the point)
	 *
	 *	Params: 
	 *		point = Point to transform
	 *
	 *	Returns: 
	 *		Transformed point
	 */
	Vector2f transformToLocal(Vector2f point) const;

	/**
	 *	Transform a point from local coordinates into global coordinates
	 *	(ie it applies the inverse of object's origin, translation, rotation and scale to the point)
	 *
	 *	Params: 
	 *		point = Point to transform
	 *
	 *	Returns: 
	 *		Transformed point
	 */
	Vector2f transformToGlobal(Vector2f point) const;
	
	/**
	 *	Render the specific geometry of the object
	 *	
	 *	Params:
	 *		window = Target into which render the object		
	 */
	void render(IRenderTarget window);
	
	/**
	 *	Render the specific geometry of the object with a shader
	 *
	 *	Params:
	 *		window = Render target
	 *		shader = Shader to use
	 */
	void renderWithShader(IRenderTarget window, Shader shader);
}
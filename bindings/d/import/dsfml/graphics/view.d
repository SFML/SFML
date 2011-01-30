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

module dsfml.graphics.view;

import	dsfml.graphics.rect;

import	dsfml.system.common,
		dsfml.system.vector;

/**
*	This class defines a view (position, size and zoom) ;
*	you can consider it as a camera
*/
class View : DSFMLObject
{
private:
	FloatRect	_rect; // a view defines a source area of the scene to display, and a destination area into the rendertarget where to map the source area
	FloatRect	_viewport; // the viewport is the destination area in the rendertarget
	bool m_isModified = true;

public:
	/**
	*	Constructor
	*	
	*	Default view (1000 x 1000)
	*/
	this()
	{
		super(sfView_Create());
	}
	 
	/**
	*	Constructor
	*	
	*	Params:
	*		center = center of the view
	*		size = size of the view (width, height)
	*/			  
	this(Vector2f center, Vector2f size)
	{
		super(sfView_CreateFromRect(FloatRect(center.x - size.x / 2, center.y - size.y / 2, size.x, size.y) ));
	}
	
	/**
	*	Constructor
	*	
	*	Params:
	*		rect = Rectangle defining the position and size of the view				 
	*/		
	this(FloatRect rect)
	{
		super(sfView_CreateFromRect(rect));
	}

	override void dispose()
	{
		sfView_Destroy(m_ptr);
	}
	
	/**
	*	Change the center of the view
	*	
	*	Params:
	*		x = X coordinates of the new center
	*		y = Y coordinates of the new center					 
	*/		
	void setCenter(float x, float y)
	{
		sfView_SetCenter(m_ptr, x, y);
		m_isModified = true;
	}

	/**
	*	Change the center of the view
	*	
	*	Params:
	*		center = New center					 
	*/		
	void setCenter(Vector2f center)
	{
		sfView_SetCenter(m_ptr, center.x, center.y);
		m_isModified = true;
	}

	/**
	*	Change the size of the view (take 2 values)
	*
	*	Params:
	*		width = New width
	*		height = New height
	*/
	void setSize(float width, float height)
	{
		sfView_SetSize(m_ptr, width, height);
		m_isModified = true;
	}

	/**
	*	Change the size of the view (take 2 values)
	*
	*	Params:
	*		size = New size
	*/
	void setSize(Vector2f size)
	{
		sfView_SetSize(m_ptr, size.x, size.y);
		m_isModified = true;
	}

	/**
	* Rebuild the view from a rectangle
	*
	* Params:
	*	 viewport : Rectangle defining the position and size of the view
	*/
	void setViewport(FloatRect viewport)
	{
		sfView_SetViewport(m_ptr, viewport);
		_viewport = viewport;
	}

	/**
	*	Get the center of the view
	*
	*	Returns: 
	*		Center of the view
	*/
	Vector2f getCenter()
	{
		return Vector2f(sfView_GetCenterX(m_ptr), sfView_GetCenterY(m_ptr));
	}

	/**
	*	Get the size of the view
	*
	*	Returns: 
	*		size of the view
	*/
	Vector2f getSize()
	{
		return Vector2f(sfView_GetWidth(m_ptr), sfView_GetHeight(m_ptr));
	}
	
	/**
	 *	Get the width of the view
	 *
	 *	Returns:
	 *		width of the view
	 */
	float getWidth()
	{
		return sfView_GetWidth(m_ptr);
	}

	/**
	 *	Get the height of the view
	 *
	 *	Returns:
	 *		height of the view
	 */
	float getHeight()
	{
		return sfView_GetHeight(m_ptr);
	}
	
	/**
	*	Get the bounding retangle of the view
	*/			  
	FloatRect getViewport()
	{
		if (m_isModified)
		{
			m_isModified = false;
			_viewport = sfView_GetViewport(m_ptr);
		}
		return _viewport;
	}
	
	/**
	*	Move the view 
	*
	*	Params:
	*		offsetX = Offset to move the view, on X axis
	*		offsetY = Offset to move the view, on Y axis
	*/
	View move(float offsetX, float offsetY)
	{
		sfView_Move(m_ptr, offsetX, offsetY);
		m_isModified = true;
		
		return this;
	}

	/**
	*	Move the view 
	*
	*	Params:
	*		offset = offsetto move the view
	*/
	View move(Vector2f offset)
	{
		sfView_Move(m_ptr, offset.x, offset.y);
		m_isModified = true;
		
		return this;
	}
	
	/**
	*	Resize the view rectangle to simulate a zoom / unzoom effect
	*
	*	Params:
	*		factor = Zoom factor to apply, relative to the current zoom
	*/
	View zoom(float factor)
	{
		sfView_Zoom(m_ptr, factor);
		m_isModified = true;
		
		return this;
	}

	/**
	 *	Rotate the view relatively to its current orientation.
	 * 
	 *	Params:
	 *		angle = Angle to rotate, in degree
	 */
	View rotate(float angle)
	{
		sfView_Rotate(m_ptr, angle);
		
		return this;
	}
	
	/**
	 *	Set the orientation of the view
	 *	The default rotation of a view is 0 degree
	 *
	 *	Params:
	 *		angle = New angle, in degrees
	 */
	View setRotation(float angle)
	{
		sfView_SetRotation(m_ptr, angle);
		
		return this;
	}

	/**
	 *	Get the current orientation of the view
	 *
	 *	Returns:
	 *		Rotation angle of the view, in degrees
	 */
	float getRotation()
	{
		return sfView_GetRotation(m_ptr);
	}

	void reset(FloatRect rect)
	{
		sfView_Reset(m_ptr, rect);
		_rect = rect;
	}
package:

	this(SFMLClass ptr, bool preventDelete)
	{
		super(ptr, preventDelete);
	}
	
private:
	static extern(C)
	{
		SFMLClass		function()						sfView_Create;
		SFMLClass		function(FloatRect)				sfView_CreateFromRect;
		void		function(SFMLClass)					sfView_Destroy;
		void		function(SFMLClass, float, float)	sfView_SetCenter;
		void		function(SFMLClass, float, float)	sfView_SetSize;
		void		function(SFMLClass, FloatRect)		sfView_SetViewport;
		float		function(SFMLClass)					sfView_GetCenterX;
		float		function(SFMLClass)					sfView_GetCenterY;
		float		function(SFMLClass)					sfView_GetWidth;
		float		function(SFMLClass)					sfView_GetHeight;
		FloatRect	function(SFMLClass)					sfView_GetViewport;
		void		function(SFMLClass, float, float)	sfView_Move;
		void		function(SFMLClass, float)			sfView_Zoom;
		
		// DSFML2
		void		function(SFMLClass, float)			sfView_SetRotation;
		float		function(SFMLClass)					sfView_GetRotation;
		void		function(SFMLClass, float)			sfView_Rotate;
		void		function(SFMLClass, FloatRect)		sfView_Reset;
	}

	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-graphics-d-2");
	else
		DllLoader dll = DllLoader.load("csfml-graphics-2");
		
		mixin(loadFromSharedLib("sfView_Create"));
		mixin(loadFromSharedLib("sfView_CreateFromRect"));
		mixin(loadFromSharedLib("sfView_Destroy"));
		mixin(loadFromSharedLib("sfView_SetCenter"));
		mixin(loadFromSharedLib("sfView_SetSize"));
		mixin(loadFromSharedLib("sfView_SetViewport"));
		mixin(loadFromSharedLib("sfView_GetCenterX"));
		mixin(loadFromSharedLib("sfView_GetCenterY"));
		mixin(loadFromSharedLib("sfView_GetWidth"));
		mixin(loadFromSharedLib("sfView_GetHeight"));
		mixin(loadFromSharedLib("sfView_GetViewport"));
		mixin(loadFromSharedLib("sfView_Move"));
		mixin(loadFromSharedLib("sfView_Zoom"));
		
		// DSFML2
		mixin(loadFromSharedLib("sfView_SetRotation"));
		mixin(loadFromSharedLib("sfView_GetRotation"));
		mixin(loadFromSharedLib("sfView_Rotate"));
		mixin(loadFromSharedLib("sfView_Reset"));
	}
}
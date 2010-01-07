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

module dsfml.graphics.sprite;

import dsfml.graphics.blendmode;
import dsfml.graphics.color;
import dsfml.graphics.drawableimpl;
import dsfml.graphics.image;
import dsfml.graphics.rect;

import dsfml.system.vector2;

/**
*	Sprite defines a sprite : texture, transformations,
*	color, and draw on screen
*	See_Also:
*		IDrawable
*/
class Sprite : Drawableimpl!("sfSprite")
{
private:
	Image m_image;	  	//< Image used to draw the sprite
	IntRect m_subRect;	//< Sub-rectangle of source image to assign to the sprite
	
public:

	/**
	*	Default constructor
	*/
	this()
	{
		super();
	}

	/**
	*	Construct the sprite from a source image
	*
	*	Params: 
	*		img = Image of the sprite
	*		left = Left coordinate of the sprite (0 by default)
	*		top = Top coordinate of the sprite (0 by default)
	*		scaleX = Horizontal scale (1 by default)
	*		scaleY= Vertical scale (1 by default)
	*		rotation = Orientation, in degrees (0 by default)
	*		col = Color of the sprite (white by default)
	*/
	this(Image img, float left = 0.f, float top = 0.f, float scaleX = 1.f, float scaleY = 1.f, float rotation = 0.f, Color col = Color.WHITE)
	{
		super();
		m_image = img;
		sfSprite_SetImage(m_ptr, img.getNativePointer);
		setX(left);
		setY(top);
		setScaleX(scaleX);
		setScaleY(scaleY);
		setRotation(rotation);
		setColor(col);
	}

	/**
	*	Change the image of the sprite
	*
	*	Params:
	*		img = New image
	*/
	void setImage(Image img)
	{
		assert(img !is null, "Trying to set a null image.");
		sfSprite_SetImage(m_ptr, img.getNativePointer);
		m_image = img;
	}

	/**
	*	Set the sub-rectangle of a sprite inside the source image.
	*  
	*	Params:
	*		rect = New sub-rectangle					
	*/	
	void setSubRect(IntRect rect)
	{
		IntRect r = rect;
		sfSprite_SetSubRect(m_ptr, &r);
		m_subRect = rect;
	}

	/**
	*	Resize the sprite (by changing its scale factors).
	*	The default size is defined by the subrect
	*
	*	Params:	
	*		width = New width (must be strictly positive)
	*		height = New height (must be strictly positive)
	*/
	void resize(float width, float height)
	{
		if (width > 0 && height > 0)
			sfSprite_Resize(m_ptr, width, height);
	}

	/**
	*	Resize the sprite (by changing its scale factors).
	*	The default size is defined by the subrect
	*
	*	Params:	
	*		size = New size (both coordinates must be strictly positive)
	*/
	void resize(Vector2f size)
	{
		if (size.x > 0 && size.y > 0)
			sfSprite_Resize(m_ptr, size.x, size.y);
	}
 

	/**
	*	Flip the sprite horizontally
	*
	*	Params: 
	*		flipped = True to flip the sprite
	*/
	void flipX(bool flipped)
	{
		sfSprite_FlipX(m_ptr, flipped);
	}

	/**
	*	Flip the sprite vertically
	*
	*	Params: 
	*		flipped = True to flip the sprite
	*/
	void flipY(bool flipped)
	{
		sfSprite_FlipY(m_ptr, flipped);
	}

	/**
	*	Get the source image of the sprite
	*
	*	Returns: 
	*		Pointer to the image (can be NULL)
	*/
	Image getImage()
	{
		return m_image;
	}

	/**
	*	Get the sub-rectangle of the sprite inside the source image
	*	
	*	Returns:
	*		Sub-rectangle			
	*/		
	IntRect getSubRect()
	{
		if (m_subRect == IntRect())
			m_subRect = IntRect(0, 0, m_image.getWidth(), m_image.getHeight());
			
		return m_subRect;
	}

	/**
	*	Get the sprite size
	*
	*	Returns: 
	*		Size of the sprite
	*/		
	Vector2f getSize()
	{
		return Vector2f(sfSprite_GetWidth(m_ptr), sfSprite_GetHeight(m_ptr));
	}

	/**
	*	Get the color of a given pixel in the sprite
	*	
	*	Params:
	*		x = X coordinate 
	*		y = Y coordinate
	*	
	*	Returns:
	*		Color of pixel			 
	*/
	Color getPixel(uint x, uint y)
	{
		return sfSprite_GetPixel(m_ptr, x, y);
	}

private:
	
	extern (C)
	{
		typedef void function(void*, void*) pf_sfSprite_SetImage;
		typedef void function(void*, IntRect*) pf_sfSprite_SetSubRect;
		typedef void function(void*, float, float) pf_sfSprite_Resize;
		typedef void function(void*, int) pf_sfSprite_FlipX;
		typedef void function(void*, int) pf_sfSprite_FlipY;
		typedef void* function(void*) pf_sfSprite_GetImage;
		typedef void* function(void*) pf_sfSprite_GetSubRect;
		typedef float function(void*) pf_sfSprite_GetWidth;
		typedef float function(void*) pf_sfSprite_GetHeight;
		typedef Color function(void*, uint, uint) pf_sfSprite_GetPixel;
	
		static pf_sfSprite_SetImage sfSprite_SetImage;
		static pf_sfSprite_SetSubRect sfSprite_SetSubRect;
		static pf_sfSprite_Resize sfSprite_Resize;
		static pf_sfSprite_FlipX sfSprite_FlipX;
		static pf_sfSprite_FlipY sfSprite_FlipY;
		static pf_sfSprite_GetImage sfSprite_GetImage;
		static pf_sfSprite_GetSubRect sfSprite_GetSubRect;
		static pf_sfSprite_GetWidth sfSprite_GetWidth;
		static pf_sfSprite_GetHeight sfSprite_GetHeight;
		static pf_sfSprite_GetPixel sfSprite_GetPixel;
	}

	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-graphics-d");
	else
		DllLoader dll = DllLoader.load("csfml-graphics");
		
		sfSprite_SetImage = cast(pf_sfSprite_SetImage)dll.getSymbol("sfSprite_SetImage");
		sfSprite_SetSubRect = cast(pf_sfSprite_SetSubRect)dll.getSymbol("sfSprite_SetSubRect");
		sfSprite_Resize = cast(pf_sfSprite_Resize)dll.getSymbol("sfSprite_Resize");
		sfSprite_FlipX = cast(pf_sfSprite_FlipX)dll.getSymbol("sfSprite_FlipX");
		sfSprite_FlipY = cast(pf_sfSprite_FlipY)dll.getSymbol("sfSprite_FlipY");
		sfSprite_GetImage = cast(pf_sfSprite_GetImage)dll.getSymbol("sfSprite_GetImage");
		sfSprite_GetSubRect = cast(pf_sfSprite_GetSubRect)dll.getSymbol("sfSprite_GetSubRect");
		sfSprite_GetWidth = cast(pf_sfSprite_GetWidth)dll.getSymbol("sfSprite_GetWidth");
		sfSprite_GetHeight = cast(pf_sfSprite_GetHeight)dll.getSymbol("sfSprite_GetHeight");
		sfSprite_GetPixel = cast(pf_sfSprite_GetPixel)dll.getSymbol("sfSprite_GetPixel");
	}	
}
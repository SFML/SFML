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

import dsfml.system.vector;

/**
*	Sprite defines a sprite : texture, transformations,
*	color, and draw on screen
*	See_Also:
*		IDrawable
*/
class Sprite : DrawableImpl!("sfSprite")
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
	*		scalex = Horizontal scale (1 by default)
	*		scaley = Vertical scale (1 by default)
	*		rot = Orientation, in degrees (0 by default)
	*		col = Color of the sprite (white by default)
	*/
	this(Image img, float left = 0.f, float top = 0.f, float scalex = 1.f, float scaley = 1.f, float rot = 0.f, Color col = Color.WHITE)
	{
		super();
		m_image = img;
		sfSprite_SetImage(m_ptr, img.nativePointer, true);
		x = left;
		y = top;
		scaleX = scalex;
		scaleY = scaley;
		rotation = rot;
		color = col;
	}

	/**
	*	Change the image of the sprite
	*
	*	Params:
	*		img = New image
	*		adjustToNewSize = adjust sprite subrect to new image size
	*/
	void setImage(Image img, bool adjustToNewSize = false)
	{
		assert(img !is null, "Trying to set a null image.");
		sfSprite_SetImage(m_ptr, img.nativePointer, adjustToNewSize);
		m_image = img;
	}

	@property void image(Image img)
	{
		setImage(img, false);
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
	 *	Get the color of a given pixel in the sprite
	 *	
	 *	Params:
	 *		x = X coordinate 
	 *		y = Y coordinate
	 *	
	 *	Returns:
	 *		Color of pixel			 
	 */
	Color getPixel(uint x, uint y) const
	{
		return sfSprite_GetPixel(m_ptr, x, y);
	}
	
@property
{
	/**
	 *	Set the sub-rectangle of a sprite inside the source image.
	 *  
	 *	Params:
	 *		rect = New sub-rectangle					
	 */	
	void subRect(IntRect rect)
	{
		sfSprite_SetSubRect(m_ptr, rect);
		m_subRect = rect;
	}

	/**
	 *	Get the source image of the sprite
	 *
	 *	Returns: 
	 *		Pointer to the image (can be NULL)
	 */
	Image image()
	{
		return m_image;
	}

	/**
	 *	Get the sub-rectangle of the sprite inside the source image
	 *	
	 *	Returns:
	 *		Sub-rectangle			
	 */		
	IntRect subRect()
	{
		if (m_subRect == IntRect())
			m_subRect = sfSprite_GetSubRect(m_ptr);
			//m_subRect = IntRect(0, 0, m_image.getWidth(), m_image.getHeight());
			
		return m_subRect;
	}

	/**
	 *	Get the sprite size
	 *
	 *	Returns: 
	 *		Size of the sprite
	 */		
	Vector2f size() const
	{
		return Vector2f(sfSprite_GetWidth(m_ptr), sfSprite_GetHeight(m_ptr));
	}
}

private:
	
	static extern(C)
	{
		void	function(SFMLClass, SFMLClass, bool)	sfSprite_SetImage;
		void	function(SFMLClass, IntRect)		sfSprite_SetSubRect;
		void	function(SFMLClass, float, float)	sfSprite_Resize;
		void	function(SFMLClass, int)			sfSprite_FlipX;
		void	function(SFMLClass, int)			sfSprite_FlipY;
		SFMLClass	function(SFMLClass)				sfSprite_GetImage;
		IntRect	function(SFMLClass)					sfSprite_GetSubRect;
		float	function(SFMLClass)					sfSprite_GetWidth;
		float	function(SFMLClass)					sfSprite_GetHeight;
		Color	function(SFMLClass, uint, uint)		sfSprite_GetPixel;
	}

	mixin(loadFromSharedLib2("csfml-graphics", "sfSprite",
	"SetImage", "SetSubRect", "Resize", "FlipX", "FlipY", "GetImage", "GetSubRect", "GetWidth", "GetHeight", "GetPixel"));
}
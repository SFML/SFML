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


module dsfml.graphics.renderimage;

import	dsfml.system.common,
		dsfml.system.exception,
		dsfml.system.stringutil,
		dsfml.system.vector;

import	dsfml.graphics.idrawable,
		dsfml.graphics.image,
		dsfml.graphics.color,
		dsfml.graphics.rect,
		dsfml.graphics.shader,
		dsfml.graphics.view,
		dsfml.graphics.irendertarget;


/**
 *	Target for 2D rendering into an image that can be reused in a sprite
 */
class RenderImage : DSFMLObject, IRenderTarget
{
private:
	Image _image = null;
	View _view = null;
	View _defaultView = null;

package:
	this(SFMLClass ptr)
	{
		super(ptr, true);
	}
	
	override void dispose()
	{
		sfRenderImage_Destroy(m_ptr);
	}

public:

	/**
	 *	Construct a new renderimage
	 *
	 *	Params:
	 *		width = Width of the renderimage
	 *		height = Height of the renderimage
	 *		depthBuffer = Do you want a depth-buffer attached? (useful only if you're doing 3D OpenGL on the renderimage)
	 */
	this(uint width, uint height, bool depthBuffer = false)
	{
		super(sfRenderImage_Create(width, height, depthBuffer));
	}
	
	/**
	 *	Update the contents of the target image
	 */
	void display()
	{
		sfRenderImage_Display(m_ptr);
	}

	/**
	 *	Draw something on a renderimage
	 *
	 *	Params:
	 *	    drawable = object to draw
	 */
	void draw(IDrawable drawable)
	{
		drawable.render(this);
	}
	
	/**
	 *	
	 *	Params:
	 *	    drawable = Object to draw
	 *	    shader = Shader to use
	 */
	void draw(IDrawable drawable, Shader shader)
	{
		drawable.renderWithShader(this, shader);
	}

	/**
	 *	Clear the renderimage with the given color
	 *
	 *	Params:
	 *	    color = Fill color
	 */
	void clear(Color color)
	{
		sfRenderImage_Clear(m_ptr, color);
	}

	/**
	*	Convert a point in image coordinates into view coordinates
	*
	*	Params: 
	*		imageX = X coordinate of the point to convert, relative to the image
	*		imageY = Y coordinate of the point to convert, relative to the image
	*		targetView = Target view to convert the point to (pass NULL to use the current view)
	*		
	*	Returns:
	*		Converted point			
	*/	 
	Vector2f convertCoords(uint imageX, uint imageY, View targetView = null)
	{
		Vector2f vec;
		sfRenderImage_ConvertCoords(m_ptr, imageX, imageY, &vec.x, &vec.y, targetView is null ? null : targetView.nativePointer);
		return vec;
	}
	


	/**
	 *	Save the current OpenGL render states and matrices
	 */
	void saveGLStates()
	{
		sfRenderImage_SaveGLStates(m_ptr);
	}

	/**
	 *	Restore the previously saved OpenGL render states and matrices
	 */
	void restoreGLStates()
	{
		sfRenderImage_RestoreGLStates(m_ptr);
	}

@property
{
	/**
	*	Return the width of the rendering region of a renderimage
	*
	*	Returns: 
	*		Width in pixels
	*/
	uint width()
	{
		return sfRenderImage_GetWidth(m_ptr);
	}

	/**
	*	Return the height of the rendering region of a renderimage
	*
	*	Returns: 
	*		Height in pixels
	*/
	uint height()
	{
		return sfRenderImage_GetHeight(m_ptr);
	}
	
	/**
	 *	Activate or deactivate a renderimage as the current target for rendering
	 *
	 *	Params:
	 *	    active = true to activate, false to deactivate
	 *	Returns:
	 *		true if operation was successful, false otherwise
	 */
	bool active(bool activ)
	{
		return sfRenderImage_SetActive(m_ptr, activ);
	}

	/**
	 *	Change the current active view of a renderimage
	 *
	 *	Params:
	 *	    view = Pointer to the new view
	 */
	void view(View v)
	{
		if (_view !is null)
		{		
			_view.setHandled(false);
		}
		
		sfRenderImage_SetView(m_ptr, v.nativePointer);
		  
		_view = v;
		_view.setHandled(true);
	}

	/**
	 *	Get the current active view rectangle
	 *
	 *	Returns: 
	 *		current view rectangle, in global coordinates
	 */
	View view()
	{
		if (_view is null)
		{
			SFMLClass cView = sfRenderImage_GetView(m_ptr);
			_view = new View(cView, true);
		}
		return _view;
	}

	/**
	*	Get the default view
	*
	*	Returns:
	*		default view
	*/
	View defaultView()
	{
		if (_defaultView is null)
		{
			SFMLClass cView = sfRenderImage_GetDefaultView(m_ptr);
			_defaultView = new View(cView, true);
		}
		return _defaultView;
	}
	
	
	IntRect viewport(View v = null) // TODO: is there a need to accept other Views than the currently assigned one?
	{
		return sfRenderImage_GetViewport(m_ptr, v is null ? _view.nativePointer : v.nativePointer);
	}
	
	/**
	 *	Get the target image
	 *
	 *	Returns:
	 *		target image
	 */
	Image image()
	{
		if (_image is null)
		{
			SFMLClass cImage = sfRenderImage_GetImage(m_ptr);
			_image = new Image(cImage);
		}
		return _image;
	}
	
	/**
	 *	Check whether the system supports render images or not
	 *
	 *	Returns:
	 *		true if the RenderImage class can be used
	 */
	bool isAvailable()
	{
		return sfRenderImage_IsAvailable();
	}
}

private:
	static extern(C)
	{
		SFMLClass	function(uint, uint, bool)		sfRenderImage_Create;
		void	function(SFMLClass)					sfRenderImage_Destroy;
		uint	function(SFMLClass)					sfRenderImage_GetWidth;
		uint	function(SFMLClass)					sfRenderImage_GetHeight;
		bool	function(SFMLClass, bool)			sfRenderImage_SetActive;
		void	function(SFMLClass)					sfRenderImage_Display;
		
		void	function(SFMLClass, void*)			sfRenderImage_DrawSprite;
		void	function(SFMLClass, void*)			sfRenderImage_DrawShape;
		void	function(SFMLClass, void*)			sfRenderImage_DrawText;

		void	function(SFMLClass, void*, void*)	sfRenderImage_DrawSpriteWithShader;
		void	function(SFMLClass, void*, void*)	sfRenderImage_DrawShapeWithShader;
		void	function(SFMLClass, void*, void*)	sfRenderImage_DrawTextWithShader;

		void	function(SFMLClass, Color)			sfRenderImage_Clear;
		void	function(SFMLClass, SFMLClass)		sfRenderImage_SetView;
		SFMLClass	function(SFMLClass)				sfRenderImage_GetView;
		SFMLClass	function(SFMLClass)				sfRenderImage_GetDefaultView;
		IntRect	function(SFMLClass, SFMLClass)		sfRenderImage_GetViewport;
		void	function(SFMLClass, uint, uint, float*, float*, SFMLClass)	sfRenderImage_ConvertCoords;
		SFMLClass	function(SFMLClass)				sfRenderImage_GetImage;
		bool	function()							sfRenderImage_IsAvailable;
		
		// DSFML2
		void	function(SFMLClass)				sfRenderImage_SaveGLStates;
		void	function(SFMLClass)				sfRenderImage_RestoreGLStates;
	}

	mixin(loadFromSharedLib2("csfml-graphics", "sfRenderImage", "Create", "Destroy", "GetWidth", "GetHeight",
	"SetActive", "Display", "Clear", "SetView", "GetView", "GetDefaultView", "GetViewport", "ConvertCoords",
	"GetImage", "IsAvailable",
	// DSFML2
	"SaveGLStates", "RestoreGLStates"));

}
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

module dsfml.graphics.renderwindow;

import	dsfml.graphics.color,
		dsfml.graphics.rect,
		dsfml.graphics.shader,
		dsfml.graphics.view,
		dsfml.graphics.idrawable,
		dsfml.graphics.irendertarget;

import	dsfml.window.event,
		dsfml.window.input,
		dsfml.window.videomode,
		dsfml.window.window,
		dsfml.window.windowhandle;

import	dsfml.system.common,
		dsfml.system.stringutil,
		dsfml.system.vector;

/**
 *	Simple wrapper for Window that allows easy 2D rendering. 
 */
class RenderWindow : Window, IRenderTarget
{
private:	
	View m_view = null;
	View m_defaultView = null;

public:

	/**
	 *	Construct the window
	 *
	 *	Params:	
	 *		mode = Video mode to use
	 *		title = Title of the window
	 *		windowStyle = Window style (Resize | Close by default)
	 *		settings = Context settings (default is default ContextSettings values)
	 */
	this(VideoMode mode, string title, Style windowStyle = Style.Default, ContextSettings settings = ContextSettings())
	{
		super(sfRenderWindow_Create(mode, toStringz(title), windowStyle, &settings));
		m_input = new Input(sfRenderWindow_GetInput(m_ptr));
	}

	/**
	 *	Construct the window from an existing control
	 *	
	 *	Params:		
	 *		handle = Platform-specific handle of the control
	 *		settings = Context settings (default is default ContextSettings values)
	 */
	this(WindowHandle handle, ContextSettings settings = ContextSettings())
	{
		super(sfRenderWindow_CreateFromHandle(handle, &settings));
		m_input = new Input(sfRenderWindow_GetInput(m_ptr));
	}

	override void dispose()
	{
		sfRenderWindow_Destroy(m_ptr);
	}

	/**
	 *	Create (or recreate) the window
	 *	
	 *	Input created with getInput will become invalid.
	 *					
	 *	Params:		
	 *		mode = Video mode to use
	 *		title = Title of the window
	 *		windowStyle = Window style (Resize | Close by default)
	 *		settings = Context settings (default is default ContextSettings values)
	 *
	 */
	override void create(VideoMode mode, string title, Style windowStyle = Style.Default, ContextSettings settings = ContextSettings())
	{
		if (m_ptr !is null)
			dispose();
			
		m_ptr = sfRenderWindow_Create(mode, toStringz(title), windowStyle, &settings);
		m_input = new Input(sfRenderWindow_GetInput(m_ptr));
	}

	/**
	 *	Create (or recreate) the window from an existing control
	 *	
	 *	Input created with getInput become invalid.
	 *					
	 *	Params:		
	 *		handle = Platform-specific handle of the control
	 *		settings = Context settings (default is default ContextSettings values)
	 *
	 */
	override void create(WindowHandle handle, ContextSettings settings = ContextSettings())
	{
		if (m_ptr !is null)
			dispose();

		m_ptr = sfRenderWindow_CreateFromHandle(handle, &settings);
		m_input = new Input(sfRenderWindow_GetInput(m_ptr));
	}
	
	/**
	 *	Draw a sprite, shape or text on the window with a shader
	 *
	 *	Params:
	 *		drawable = IDrawable to draw
	 *		shader = Shader to use
	 */
	void draw(IDrawable drawable, Shader shader)
	{
		drawable.renderWithShader(this, shader);
	}
	
	/**
	 *	Draw a sprite, shape or text
	 *	
	 *	Params:
	 *		drawable = IDrawable to draw
	 */				
	void draw(IDrawable drawable)
	{
		drawable.render(this);
	}
	
	/**
	 *	Clear the screen with the given color.
	 *
	 *	Params:
	 *		col = Fill color
	 */
	void clear(Color col = Color.BLACK)
	{
		sfRenderWindow_Clear(m_ptr, col);
	}



	/**
	 *	Convert a point in window coordinates into view coordinates
	 *
	 *	Params: 
	 *		windowX = X coordinate of the point to convert, relative to the window
	 *		windowY = Y coordinate of the point to convert, relative to the window
	 *		targetView = Target view to convert the point to (pass NULL to use the current view)
	 *		
	 *	Returns:
	 *		Converted point			
	 */	 
	Vector2f convertCoords(uint windowX, uint windowY, View targetView = null)
	{
		Vector2f vec;
		sfRenderWindow_ConvertCoords(m_ptr, windowX, windowY, &vec.x, &vec.y, targetView is null ? null : targetView.nativePointer);
		return vec;
	}

	/**
	 *	Save the current OpenGL render states and matrices
	 */
	void saveGLStates()
	{
		sfRenderWindow_SaveGLStates(m_ptr);
	}

	/**
	 *	Restore the previously saved OpenGL render states and matrices
	 */
	void restoreGLStates()
	{
		sfRenderWindow_RestoreGLStates(m_ptr);
	}

@property
{
	/**
	 *	Change the current active view.
	 *	The current view is defined with the initial size of the window
	 *
	 *	Params: 
	 *		newView = Pointer to the new view (pass getDefaultView to set the default view)
	 */
	void view(View newView)
	{
		if (m_view !is null)
		{		
			m_view.setHandled(false);
		}
		
		sfRenderWindow_SetView(m_ptr, newView.nativePointer);
		  
		m_view = newView;
		m_view.setHandled(true);
	}

	/**
	 *	Get the current view rectangle
	 *
	 *	Returns: 
	 *		current view rectangle, in global coordinates
	 */
	View view()
	{
		if (m_view is null)
		{
			SFMLClass cView = sfRenderWindow_GetView(m_ptr);
			m_view = new View(cView, true);
		}
		return m_view;
	}

	/**
	 *	Get the default view
	 *
	 *	Returns: 
	 *		default view
	 */
	View defaultView()
	{
		if (m_defaultView is null)
		{
			SFMLClass cView = sfRenderWindow_GetDefaultView(m_ptr);
			m_defaultView = new View(cView, true);
		}
		return m_defaultView;
	}
	
	/**
	 *	Return the width of the rendering region of a renderwindow
	 *
	 *	Returns: 
	 *		Width in pixels
	 */
	override uint width()
	{
		return sfRenderWindow_GetWidth(m_ptr);
	}

	/**
	 *	Return the height of the rendering region of a renderwindow
	 *
	 *	Returns: 
	 *		Height in pixels
	 */
	override uint height()
	{
		return sfRenderWindow_GetHeight(m_ptr);
	}

	/**
	 *	Get the viewport of a view applied to this target
	 *
	 *	Params:
	 *	    view = Target view
	 *	Returns:
	 *		Viewport rectangle, expressed in pixels in the current target
	 */
	IntRect viewport(View v = null) // TODO: is there a need to accept other Views than the currently assigned one?
	{
		return sfRenderWindow_GetViewport(m_ptr, v is null ? m_view.nativePointer : v.nativePointer);
	}
}

private:
	
	static extern(C)
	{
		SFMLClass	function(VideoMode, cchar*, Style, ContextSettings*)sfRenderWindow_Create;
		SFMLClass	function(WindowHandle, ContextSettings*)			sfRenderWindow_CreateFromHandle;
		void		function(SFMLClass)									sfRenderWindow_Destroy;
		SFMLClass	function(SFMLClass)									sfRenderWindow_GetInput;
		bool		function(SFMLClass)									sfRenderWindow_IsOpened;
		uint		function(SFMLClass)									sfRenderWindow_GetWidth;
		uint		function(SFMLClass)									sfRenderWindow_GetHeight;

	/*
		void	function(SFMLClass, SFMLClass)							sfRenderWindow_DrawSprite;
		void	function(SFMLClass, SFMLClass)							sfRenderWindow_DrawShape;
		void	function(SFMLClass, SFMLClass)							sfRenderWindow_DrawText;

		void	function(SFMLClass, SFMLClass, SFMLClass)				sfRenderWindow_DrawSpriteWithShader;
		void	function(SFMLClass, SFMLClass, SFMLClass)				sfRenderWindow_DrawShapeWithShader;
		void	function(SFMLClass, SFMLClass, SFMLClass)				sfRenderWindow_DrawTextWithShader;
	*/
		
		SFMLClass	function(SFMLClass)									sfRenderWindow_Capture;
		void		function(SFMLClass, Color)							sfRenderWindow_Clear;
		void		function(SFMLClass, SFMLClass)						sfRenderWindow_SetView;
		SFMLClass	function(SFMLClass)									sfRenderWindow_GetView;
		SFMLClass	function(SFMLClass)									sfRenderWindow_GetDefaultView;
		void		function(SFMLClass, uint, uint, float*, float*, SFMLClass)	sfRenderWindow_ConvertCoords;
		
		// DSFML2
		void		function(SFMLClass)									sfRenderWindow_SaveGLStates;
		void		function(SFMLClass)									sfRenderWindow_RestoreGLStates;
		IntRect		function(SFMLClass, SFMLClass)						sfRenderWindow_GetViewport;
	}

	mixin(loadFromSharedLib2("csfml-graphics", "sfRenderWindow", "Create", "CreateFromHandle",
	"Destroy", "GetInput", "Clear", "SetView", "GetView", "GetDefaultView", "ConvertCoords",
	"GetWidth", "GetHeight",
	// DSFML2
	"SaveGLStates", "RestoreGLStates", "GetViewport"));

	
	static ~this()
	{
		
	}
}
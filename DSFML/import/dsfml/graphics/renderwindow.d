/*
*   DSFML - SFML Library wrapper for the D programming language.
*   Copyright (C) 2008 Julien Dagorn (sirjulio13@gmail.com)
*   Copyright (C) 2010 Andreas Hollandt
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

module dsfml.graphics.renderwindow;

import	dsfml.graphics.common,
		dsfml.graphics.color,
		dsfml.graphics.sprite,
		dsfml.graphics.shape,
		dsfml.graphics.text,
		dsfml.graphics.image,
		dsfml.graphics.rect,
		dsfml.graphics.shader,
		dsfml.graphics.view,
		dsfml.graphics.idrawable;

import	dsfml.window.event,
		dsfml.window.input,
		dsfml.window.videomode,
		dsfml.window.window,
		dsfml.window.windowhandle;

import	dsfml.system.common,
		dsfml.system.stringutil,
		dsfml.system.vector2;

/**
*   Simple wrapper for Window that allows easy 2D rendering. 
*/
class RenderWindow : Window
{
private:   
	View m_view = null;
	View m_defaultView = null;

public:

    /**
    *   Construct the window
    *
    *   Params:    
    *       mode = Video mode to use
    *       title = Title of the window
    *       windowStyle = Window style (Resize | Close by default)
    *       settings = Context settings (default is default ContextSettings values)
    */
    this(VideoMode mode, string title, uint windowStyle = Style.RESIZE | Style.CLOSE, ContextSettings settings = ContextSettings())
    {
        super(sfRenderWindow_Create(mode, toStringz(title), windowStyle, &settings));
        m_input = new Input(sfRenderWindow_GetInput(m_ptr));
    }

    /**
    *   Construct the window from an existing control
    *   
    *   Params:       
    *       handle = Platform-specific handle of the control
    *       settings = Context settings (default is default ContextSettings values)
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
    *   Create (or recreate) the window
    *   
    *   Input created with getInput will become invalid.
    *                   
    *   Params:        
    *       mode = Video mode to use
    *       title = Title of the window
    *       windowStyle = Window style (Resize | Close by default)
    *       settings = Context settings (default is default ContextSettings values)
    *
    */
    void create(VideoMode mode, string title, uint windowStyle = Style.RESIZE | Style.CLOSE, ContextSettings settings = ContextSettings())
    {
        if (m_ptr !is null)
            dispose();
            
        m_ptr = sfRenderWindow_Create(mode, toStringz(title), windowStyle, &settings);
        m_input = new Input(sfRenderWindow_GetInput(m_ptr));
    }

    /**
    *   Create (or recreate) the window from an existing control
    *   
    *   Input created with getInput become invalid.
    *                   
    *   Params:        
    *       handle = Platform-specific handle of the control
    *       settings = Context settings (default is default ContextSettings values)
    *
    */
    void create(WindowHandle handle, ContextSettings settings = ContextSettings())
    {
        if (m_ptr !is null)
            dispose();

        m_ptr = sfRenderWindow_CreateFromHandle(handle, &settings);
        m_input = new Input(sfRenderWindow_GetInput(m_ptr));
    }
    
	/**
	*   Draw a sprite, shape or text on the window with a shader
	*
	*   Params:
	*   	drawable = IDrawable to draw
	*       shader = Shader to use
	*/
	void draw(IDrawable drawable, Shader shader)
	{
		drawable.renderWithShader(this, shader);
	}
    
	/**
    *   Draw a sprite, shape or text
    *   
    *   Params:
    *       drawable = IDrawable to draw
    */                
    void draw(IDrawable drawable)
    {
    	drawable.render(this);
    }
    
    /**
    *   Clear the screen with the given color.
    *
    *   Params:
    *       col = Fill color
    */
    void clear(Color col = Color.BLACK)
    {
        sfRenderWindow_Clear(m_ptr, col);
    }

    /**
    *   Change the current active view.
    *   The current view is defined with the initial size of the window
    *
    *   Params: 
    *       newView = Pointer to the new view (pass getDefaultView to set the default view)
    */
    void setView(View newView)
    {
        if (m_view !is null)
        {        
            m_view.setHandled(false);
        }
        
        sfRenderWindow_SetView(m_ptr, newView.getNativePointer);
          
        m_view = newView;
        m_view.setHandled(true);
    }

    /**
    *   Get the current view rectangle
    *
    *   Returns: 
    *       current view rectangle, in global coordinates
    */
    View getView()
    {
        if (m_view is null)
        {
            void* cView = sfRenderWindow_GetView(m_ptr);
            m_view = new View(cView, true);
        }
        return m_view;
    }

    /**
    *   Get the default view
    *
    *   Returns: 
    *       default view
    */
    View getDefaultView()
    {
        if (m_defaultView is null)
        {
            void* cView = sfRenderWindow_GetDefaultView(m_ptr);
            m_defaultView = new View(cView, true);
        }
        return m_defaultView;
    }

    /**
    *   Convert a point in window coordinates into view coordinates
    *
    *   Params: 
    *       windowX = X coordinate of the point to convert, relative to the window
    *       windowY = Y coordinate of the point to convert, relative to the window
    *       targetView = Target view to convert the point to (pass NULL to use the current view)
    *       
    *   Returns:
    *       Converted point            
    */     
    Vector2f convertCoords(uint windowX, uint windowY, View targetView = null)
    {
        Vector2f vec;
        sfRenderWindow_ConvertCoords(m_ptr, windowX, windowY, &vec.x, &vec.y, targetView is null ? null : targetView.getNativePointer);
        return vec;
    }
}
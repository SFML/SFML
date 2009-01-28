/*
*   DSFML - SFML Library binding in D language.
*   Copyright (C) 2008 Julien Dagorn (sirjulio13@gmail.com)
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

import dsfml.graphics.color;
import dsfml.graphics.idrawable;
import dsfml.graphics.image;
import dsfml.graphics.rect;
import dsfml.graphics.postfx;
import dsfml.graphics.view;

import dsfml.window.event;
import dsfml.window.input;
import dsfml.window.videomode;
import dsfml.window.window;
import dsfml.window.windowhandle;
import dsfml.window.windowsettings;
import dsfml.window.windowstyle;

import dsfml.system.common;
import dsfml.system.stringutil;
import dsfml.system.vector2;

/**
*   Simple wrapper for Window that allows easy 2D rendering. 
*/
class RenderWindow : Window
{
    /**
    *   Construct the window
    *
    *   Params:    
    *       mode = Video mode to use
    *       title = Title of the window
    *       windowStyle = Window style (Resize | Close by default)
    *       settings = Window settings (default is default WindowSettings values)
    */
    this(VideoMode mode, in char[] title, ulong windowStyle = Style.RESIZE | Style.CLOSE, WindowSettings settings = WindowSettings())
    {
        super(sfRenderWindow_Create(mode, toStringz(title), windowStyle, settings));
        m_input = new Input(sfRenderWindow_GetInput(m_ptr));
    }

    /**
    *   Construct the window from an existing control
    *   
    *   Params:       
    *       handle = Platform-specific handle of the control
    *       settings = Window settings (default is default WindowSettings values)
    */
    this(WindowHandle handle, WindowSettings settings = WindowSettings())
    {
        super(sfRenderWindow_CreateFromHandle(handle, settings));
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
    *       settings = Window settings (default is default WindowSettings values)
    *
    */
    void create(VideoMode mode, char[] title, ulong windowStyle = Style.RESIZE | Style.CLOSE, WindowSettings settings = WindowSettings())
    {
        if (m_ptr !is null)
            dispose();
            
        m_ptr = sfRenderWindow_Create(mode, toStringz(title), windowStyle, settings);
        m_input = new Input(sfRenderWindow_GetInput(m_ptr));
    }

    /**
    *   Create (or recreate) the window from an existing control
    *   
    *   Input created with getInput become invalid.
    *                   
    *   Params:        
    *       handle = Platform-specific handle of the control
    *       settings = Window settings (default is default WindowSettings values)
    *
    */
    void create(WindowHandle handle, WindowSettings settings = WindowSettings())
    {
        if (m_ptr !is null)
            dispose();

        m_ptr = sfRenderWindow_CreateFromHandle(handle, settings);
        m_input = new Input(sfRenderWindow_GetInput(m_ptr));
    }
    
    /**
    *   Draw a PostFX on the window
    *
    *   Params:
    *       postFX = PostFX to draw
    */
    void draw(PostFX postFX)
    {
        sfRenderWindow_DrawPostFX(m_ptr, postFX.getNativePointer);
    }

    /**
    *   Draw a Sprite or a String
    *   
    *   Params:
    *       obj = IDrawable object to draw                    
    */                
    void draw(IDrawable obj)
    {
        obj.render(this);
    }   
    /**
    *   Save the content of the window to an image
    *
    *   Returns:
    *       Image instance containing the contents of the screen
    */
    Image capture()
    {
        return new Image(sfRenderWindow_Capture(m_ptr));
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

    /**
    *   Tell SFML to preserve external OpenGL states, at the expense of
    *   more CPU charge. Use this function if you don't want SFML
    *   to mess up your own OpenGL states (if any).
    *   Don't enable state preservation if not needed, as it will allow
    *   SFML to do internal optimizations and improve performances.
    *   This parameter is false by default
    *
    *   Params: 
    *       preserve = True to preserve OpenGL states, false to let SFML optimize
    *
    */
    void preserveOpenGLStates(bool preserve)
    {
        sfRenderWindow_PreserveOpenGLStates(m_ptr, preserve);
    }

private:   
    View m_view = null;
    View m_defaultView = null;
    
    extern (C)
    {
    	typedef void* function(VideoMode, char*, uint, WindowSettings) pf_sfRenderWindow_Create;
    	typedef void* function(WindowHandle, WindowSettings) pf_sfRenderWindow_CreateFromHandle;
    	typedef void function(void*) pf_sfRenderWindow_Destroy;
    	typedef void* function(void*) pf_sfRenderWindow_GetInput;
    	typedef void function(void*, void*) pf_sfRenderWindow_DrawPostFX;
    	typedef void* function(void*) pf_sfRenderWindow_Capture;
    	typedef void function(void*, Color) pf_sfRenderWindow_Clear;
    	typedef void function(void*, void*) pf_sfRenderWindow_SetView;
    	typedef void* function(void*) pf_sfRenderWindow_GetView;
    	typedef void* function (void*) pf_sfRenderWindow_GetDefaultView;
    	typedef void function(void*, uint, uint, float*, float*, void*) pf_sfRenderWindow_ConvertCoords;
    	typedef void function(void*, int) pf_sfRenderWindow_PreserveOpenGLStates;
        
    	static pf_sfRenderWindow_Create sfRenderWindow_Create;
    	static pf_sfRenderWindow_CreateFromHandle sfRenderWindow_CreateFromHandle;
    	static pf_sfRenderWindow_Destroy sfRenderWindow_Destroy;
    	static pf_sfRenderWindow_GetInput sfRenderWindow_GetInput;
    	static pf_sfRenderWindow_DrawPostFX sfRenderWindow_DrawPostFX;
    	static pf_sfRenderWindow_Capture sfRenderWindow_Capture;
    	static pf_sfRenderWindow_Clear sfRenderWindow_Clear;
    	static pf_sfRenderWindow_SetView sfRenderWindow_SetView;
    	static pf_sfRenderWindow_GetView sfRenderWindow_GetView;
    	static pf_sfRenderWindow_GetDefaultView sfRenderWindow_GetDefaultView;
    	static pf_sfRenderWindow_ConvertCoords sfRenderWindow_ConvertCoords;
    	static pf_sfRenderWindow_PreserveOpenGLStates sfRenderWindow_PreserveOpenGLStates;
    }

    static this()
    {
        DllLoader dll = DllLoader.load("csfml-graphics");
        
        sfRenderWindow_Create = cast(pf_sfRenderWindow_Create)dll.getSymbol("sfRenderWindow_Create");
        sfRenderWindow_CreateFromHandle = cast(pf_sfRenderWindow_CreateFromHandle)dll.getSymbol("sfRenderWindow_CreateFromHandle");
        sfRenderWindow_Destroy = cast(pf_sfRenderWindow_Destroy)dll.getSymbol("sfRenderWindow_Destroy");
        sfRenderWindow_GetInput = cast(pf_sfRenderWindow_GetInput)dll.getSymbol("sfRenderWindow_GetInput");
        sfRenderWindow_DrawPostFX = cast(pf_sfRenderWindow_DrawPostFX)dll.getSymbol("sfRenderWindow_DrawPostFX");
        sfRenderWindow_Capture = cast(pf_sfRenderWindow_Capture)dll.getSymbol("sfRenderWindow_Capture");
        sfRenderWindow_Clear = cast(pf_sfRenderWindow_Clear)dll.getSymbol("sfRenderWindow_Clear");
        sfRenderWindow_SetView = cast(pf_sfRenderWindow_SetView)dll.getSymbol("sfRenderWindow_SetView");
        sfRenderWindow_GetView = cast(pf_sfRenderWindow_GetView)dll.getSymbol("sfRenderWindow_GetView");
        sfRenderWindow_GetDefaultView = cast(pf_sfRenderWindow_GetDefaultView)dll.getSymbol("sfRenderWindow_GetDefaultView");
        sfRenderWindow_ConvertCoords = cast(pf_sfRenderWindow_ConvertCoords)dll.getSymbol("sfRenderWindow_ConvertCoords");
        sfRenderWindow_PreserveOpenGLStates = cast(pf_sfRenderWindow_PreserveOpenGLStates)dll.getSymbol("sfRenderWindow_PreserveOpenGLStates");
    }
}


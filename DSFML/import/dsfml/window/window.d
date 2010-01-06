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

module dsfml.window.window;

import dsfml.window.event;
import dsfml.window.input;
import dsfml.window.videomode;
import dsfml.window.windowhandle;

import dsfml.system.common;
import dsfml.system.stringutil;


/**
*   Window style
*/
enum Style
{
    NONE       = 0,      /// No border / title bar (this flag and all others are mutually exclusive)
    TITLEBAR   = 1 << 0, /// Title bar + fixed border
    RESIZE     = 1 << 1, /// Titlebar + resizable border + maximize button
    CLOSE      = 1 << 2, /// Titlebar + close button
    FULLSCREEN = 1 << 3  /// Fullscreen mode (this flag and all others are mutually exclusive)
}


/**
* Structure defining the creation settings of windows
*/
struct ContextSettings
{
    uint DepthBits = 24;		/// Bits of the depth buffer
    uint StencilBits = 8;		/// Bits of the stencil buffer
    uint AntialiasingLevel = 0;	/// Level of antialiasing
	uint MajorVersion = 3;		/// Major number of the context version to create
	uint MinorVersion = 0;		/// Minor number of the context version to create
}


/**
*   Window is a rendering window ; it can create a new window
*   or connect to an existing one
*/
class Window : DSFMLObject
{    
    /**
    *   Construct a new window
    *
    *   Params:
    *       mode = Video mode to use
    *       title = Title of the window
    *       windowStyle = Window style (Resize | Close by default)
    *       settings = Context settings (default is default ContextSettings values)
    */
    this(VideoMode mode, string title, Style windowStyle = Style.RESIZE | Style.CLOSE, ContextSettings settings = ContextSettings())
    {
        super(sfWindow_Create(mode, toStringz(title), windowStyle, settings));
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
        super(sfWindow_CreateFromHandle(handle, settings));
    }

    override void dispose()
    {
        m_input = null;
        sfWindow_Destroy(m_ptr);       
    }

    /**
    *   Create (or recreate) the window
    *   
    *   Input created with getInput becomes invalid.   
    *                
    *   Params:
    *       mode = Video mode to use
    *       title = Title of the window
    *       windowStyle = Window style (Resize | Close by default)
    *       settings = Context settings (default is default ContextSettings values)
    */
    void create(VideoMode mode, string title, Style windowStyle = Style.RESIZE | Style.CLOSE, ContextSettings settings = ContextSettings())
    {
        if (m_ptr !is null)
            dispose();

        m_ptr = sfWindow_Create(mode, toStringz(title), windowStyle, settings);
    }

    /**
    *   Create (or recreate) the window from an existing control
    *   
    *   Input created with getInput becomes invalid.   
    *                
    *   Params:
    *       handle = Platform-specific handle of the control
    *       settings = Context settings (default is default ContextSettings values)
    */
    void create(WindowHandle handle, ContextSettings settings = ContextSettings())
    {
        if (m_ptr !is null)
            dispose();

        m_ptr = sfWindow_CreateFromHandle(handle, settings);
    }

    /**
    *   Close (destroy) the window.
    *   You can call create to recreate a valid window    
    */        
    void close()
    {
        sfWindow_Close(m_ptr);
    }

    /**
    *   Tell whether or not a window is opened
    *   
    *   Returns:
    *       True if window is currently open.    
    */        
    bool isOpened()
    {
        return cast(bool) sfWindow_IsOpened(m_ptr);
    }
    /**
    *   Get the width of the rendering region of the window
    *
    *   Returns:
    *       Width in pixels
    */
    uint getWidth()
    {
        return sfWindow_GetWidth(m_ptr);
    }

    /**
    *   Get the height of the rendering region of the window
    *
    *   Returns:
    *       Height in pixels
    */
    uint getHeight()
    {
        return sfWindow_GetHeight(m_ptr);
    }

    /**
    *   Get the creation settings of a window
    *   
    *   Returns:
    *       Settings used to create the window            
    */
    ContextSettings getSettings()
    {
        return sfWindow_GetSettings(m_ptr);
    }        


    /**
    *   Get the event on top of events stack, if any, and pop it
    *
    *   Params:
    *       eventReceived = Event to fill, if any
    *
    *   Returns:
    *       True if an event was returned, false if events stack was empty
    */
    bool getEvent(ref Event eventReceived)
    {
        return cast(bool)sfWindow_GetEvent(m_ptr, &eventReceived);
    }

    /**
    *   Enable / disable vertical synchronization
    *
    *   Params: 
    *       enabled : True to enable v-sync, false to deactivate
    */
    void useVerticalSync(bool enabled)
    {
        sfWindow_UseVerticalSync(m_ptr, enabled);
    }

    /**
    *   Show or hide the mouse cursor
    *
    *   Params:
    *       show : True to show, false to hide
    */
    void showMouseCursor(bool show)
    {
        sfWindow_ShowMouseCursor(m_ptr, show);
    }

    /**
    *   Change the position of the mouse cursor
    *
    *   Params:
    *       left = Left coordinate of the cursor, relative to the window
    *       top = Top coordinate of the cursor, relative to the window
    */
    void setCursorPosition(uint left, uint top)
    {
        sfWindow_SetCursorPosition(m_ptr, left, top);
    }

    /**
    *   Change the position of the window on screen.
    *   Only works for top-level windows
    *
    *   Params:
    *       left = Left position
    *       top = Top position
    */
    void setPosition(int left, int top)
    {
        sfWindow_SetPosition(m_ptr, left, top);
    }
    
    /**
    *   change the size of the rendering region of the window
    *   
    *   Params:
    *       width : new width
    *       height : new height                            
    */
    void setSize(uint width, uint height)
    {
        sfWindow_SetSize(m_ptr, width, height);
    }

    /**
    *   Show or hide the window
    *
    *   Params:
    *       state = True to show, false to hide
    *
    */
    void show(bool state)
    {
        sfWindow_Show(m_ptr, state);
    }

    /**
    *   Enable or disable automatic key-repeat for keydown events.
    *   Automatic key-repeat is enabled by default.
    *   
    *   Params:
    *       enabled = true to enable, false to disable            
    */        
    void enableKeyRepeat(bool enabled)
    {
        sfWindow_EnableKeyRepeat(m_ptr, enabled);
    }
    
    /**
    *   Change the window's icon
    *
    *   Params:
    *       width = Icon's width, in pixels
    *       height = Icon's height, in pixels
    *       data = array of pixels in memory, format must be RGBA 32 bits
    *
    */
    void setIcon(size_t width, size_t height, ubyte[] data)
    {
        sfWindow_SetIcon(m_ptr, width, height, data.ptr);
    }

    /**
    *   Set the window as the current target for rendering
    *   
    *   Params:
    *       active = True to activate, false to deactivate        
    *   Returns:
    *       True if operation was successful, false otherwise
    */
    bool setActive(bool active = true)
    {
        return cast(bool)sfWindow_SetActive(m_ptr, active);
    }

    /**
    *   Display the window on screen
    */
    void display()
    {
        sfWindow_Display(m_ptr);
    }

    /**
    *   Get the input manager of the window
    *
    *   Returns: 
    *       An input manager
    *   See_Also :
    *       Input
    */
    Input getInput()
    {
        if (m_input is null)
            m_input = new Input(sfWindow_GetInput(m_ptr));
        return m_input;
    }

    /**
    *   Limit the framerate to a maximum fixed frequency
    *
    *   Params:
    *       limit : Framerate limit, in frames per seconds (use 0 to disable limit)
    */
    void setFramerateLimit(uint limit)
    {
        sfWindow_SetFramerateLimit(m_ptr, limit);
    }

    /**
    *   Get time elapsed since last frame
    *
    *   Returns:
    *       Time elapsed, in seconds
    */
    float getFrameTime()
    {
        return sfWindow_GetFrameTime(m_ptr);
    }

    /**
    *   Change the joystick threshold, ie. the value below which
    *   no move event will be generated
    *
    *   Params:
    *       threshold : New threshold, in range [0, 100]
    */
    void setJoystickThreshold(float threshold)
    {
        sfWindow_SetJoystickThreshold(m_ptr, threshold);
    }

protected:
    this(void* ptr)
    {
        super(ptr);
    }

    Input m_input;
private:
    
// External ====================================================================
    extern (C)
    {
        typedef void* function(VideoMode, cchar*, uint, ContextSettings) pf_sfWindow_Create;
    	typedef void* function(WindowHandle, ContextSettings) pf_sfWindow_CreateFromHandle;
    	typedef void function(void*) pf_sfWindow_Destroy;
    	typedef void function(void*) pf_sfWindow_Close;
    	typedef int function(void*) pf_sfWindow_IsOpened;
    	typedef uint function(void*) pf_sfWindow_GetWidth;
    	typedef uint function(void*) pf_sfWindow_GetHeight;
    	typedef ContextSettings function(void* Window) pf_sfWindow_GetSettings;
    	typedef int function(void*, Event*) pf_sfWindow_GetEvent;
    	typedef void function(void*, int) pf_sfWindow_UseVerticalSync;
    	typedef void function(void*, int) pf_sfWindow_ShowMouseCursor;
    	typedef void function(void*, uint, uint) pf_sfWindow_SetCursorPosition;
    	typedef void function(void*, int, int) pf_sfWindow_SetPosition;
    	typedef void function(void*, uint, uint) pf_sfWindow_SetSize;
    	typedef void function(void*, int) pf_sfWindow_Show;
    	typedef void function(void*, int) pf_sfWindow_EnableKeyRepeat;
    	typedef void function(void*, size_t, size_t, ubyte*) pf_sfWindow_SetIcon;
    	typedef int function(void*, int) pf_sfWindow_SetActive;
    	typedef void function(void*) pf_sfWindow_Display;
    	typedef void* function(void*) pf_sfWindow_GetInput;
    	typedef void function(void*, uint) pf_sfWindow_SetFramerateLimit;
    	typedef float function(void*) pf_sfWindow_GetFrameTime;
    	typedef void function(void*, float) pf_sfWindow_SetJoystickThreshold;
    
    	static pf_sfWindow_Create sfWindow_Create;
    	static pf_sfWindow_CreateFromHandle sfWindow_CreateFromHandle;
    	static pf_sfWindow_Destroy sfWindow_Destroy;
    	static pf_sfWindow_Close sfWindow_Close;
    	static pf_sfWindow_IsOpened sfWindow_IsOpened;
    	static pf_sfWindow_GetWidth sfWindow_GetWidth;
    	static pf_sfWindow_GetHeight sfWindow_GetHeight;
    	static pf_sfWindow_GetSettings sfWindow_GetSettings;
    	static pf_sfWindow_GetEvent sfWindow_GetEvent;
    	static pf_sfWindow_UseVerticalSync sfWindow_UseVerticalSync;
    	static pf_sfWindow_ShowMouseCursor sfWindow_ShowMouseCursor;
    	static pf_sfWindow_SetCursorPosition sfWindow_SetCursorPosition;
    	static pf_sfWindow_SetPosition sfWindow_SetPosition;
    	static pf_sfWindow_SetSize sfWindow_SetSize;
    	static pf_sfWindow_Show sfWindow_Show;
    	static pf_sfWindow_EnableKeyRepeat sfWindow_EnableKeyRepeat;
    	static pf_sfWindow_SetIcon sfWindow_SetIcon;
    	static pf_sfWindow_SetActive sfWindow_SetActive;
    	static pf_sfWindow_Display sfWindow_Display;
    	static pf_sfWindow_GetInput sfWindow_GetInput;
    	static pf_sfWindow_SetFramerateLimit sfWindow_SetFramerateLimit;
    	static pf_sfWindow_GetFrameTime sfWindow_GetFrameTime;
    	static pf_sfWindow_SetJoystickThreshold sfWindow_SetJoystickThreshold;
    }

    static this()
    {
        debug
			DllLoader dll = DllLoader.load("csfml-window-d");
		else
			DllLoader dll = DllLoader.load("csfml-window");
        
        sfWindow_Create = cast(pf_sfWindow_Create)dll.getSymbol("sfWindow_Create");
        sfWindow_CreateFromHandle = cast(pf_sfWindow_CreateFromHandle)dll.getSymbol("sfWindow_CreateFromHandle");
        sfWindow_Destroy = cast(pf_sfWindow_Destroy)dll.getSymbol("sfWindow_Destroy");
        sfWindow_Close = cast(pf_sfWindow_Close)dll.getSymbol("sfWindow_Close");
        sfWindow_IsOpened = cast(pf_sfWindow_IsOpened)dll.getSymbol("sfWindow_IsOpened");
        sfWindow_GetWidth = cast(pf_sfWindow_GetWidth)dll.getSymbol("sfWindow_GetWidth");
        sfWindow_GetHeight = cast(pf_sfWindow_GetHeight)dll.getSymbol("sfWindow_GetHeight");
        sfWindow_GetSettings = cast(pf_sfWindow_GetSettings)dll.getSymbol("sfWindow_GetSettings");
        sfWindow_GetEvent = cast(pf_sfWindow_GetEvent)dll.getSymbol("sfWindow_GetEvent");
        sfWindow_UseVerticalSync = cast(pf_sfWindow_UseVerticalSync)dll.getSymbol("sfWindow_UseVerticalSync");
        sfWindow_ShowMouseCursor = cast(pf_sfWindow_ShowMouseCursor)dll.getSymbol("sfWindow_ShowMouseCursor");
        sfWindow_SetCursorPosition = cast(pf_sfWindow_SetCursorPosition)dll.getSymbol("sfWindow_SetCursorPosition");
        sfWindow_SetPosition = cast(pf_sfWindow_SetPosition)dll.getSymbol("sfWindow_SetPosition");
        sfWindow_SetSize = cast(pf_sfWindow_SetSize)dll.getSymbol("sfWindow_SetSize");
        sfWindow_Show = cast(pf_sfWindow_Show)dll.getSymbol("sfWindow_Show");
        sfWindow_EnableKeyRepeat = cast(pf_sfWindow_EnableKeyRepeat)dll.getSymbol("sfWindow_EnableKeyRepeat");
        sfWindow_SetIcon = cast(pf_sfWindow_SetIcon)dll.getSymbol("sfWindow_SetIcon");
        sfWindow_SetActive = cast(pf_sfWindow_SetActive)dll.getSymbol("sfWindow_SetActive");
        sfWindow_Display = cast(pf_sfWindow_Display)dll.getSymbol("sfWindow_Display");
        sfWindow_GetInput = cast(pf_sfWindow_GetInput)dll.getSymbol("sfWindow_GetInput");
        sfWindow_SetFramerateLimit = cast(pf_sfWindow_SetFramerateLimit)dll.getSymbol("sfWindow_SetFramerateLimit");
        sfWindow_GetFrameTime = cast(pf_sfWindow_GetFrameTime)dll.getSymbol("sfWindow_GetFrameTime");
        sfWindow_SetJoystickThreshold = cast(pf_sfWindow_SetJoystickThreshold)dll.getSymbol("sfWindow_SetJoystickThreshold");
    }
}

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

module dsfml.window.window;

import dsfml.window.event;
import dsfml.window.input;
import dsfml.window.videomode;
import dsfml.window.windowhandle;

import dsfml.system.common;
import dsfml.system.stringutil;


/**
 *	Window style
 */
enum Style : uint
{
	None		= 0,	  /// No border / title bar (this flag and all others are mutually exclusive)
	Titlebar	= 1 << 0, /// Title bar + fixed border
	Resize		= 1 << 1, /// Titlebar + resizable border + maximize button
	Close		= 1 << 2, /// Titlebar + close button
	Fullscreen	= 1 << 3, /// Fullscreen mode (this flag and all others are mutually exclusive)
	
	Default		= Titlebar | Resize | Close /// Default window style
}


/**
 *	Structure defining the creation settings of windows
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
 *	Window is a rendering window ; it can create a new window
 *	or connect to an existing one
 */
class Window : DSFMLObject
{	
protected:
	this(SFMLClass ptr)
	{
		super(ptr);
	}

	Input m_input;

	override void dispose()
	{
		m_input = null;
		sfWindow_Destroy(m_ptr);		
	}

public:
	/**
	 *	Construct a new window
	 *
	 *	Params:
	 *		mode = Video mode to use
	 *		title = Title of the window
	 *		windowStyle = Window style (Resize | Close by default)
	 *		settings = Context settings (default is default ContextSettings values)
	 */
	this(VideoMode mode, string title, Style windowStyle = Style.Default, ContextSettings settings = ContextSettings())
	{
		super(sfWindow_Create(mode, toStringz(title), windowStyle, &settings));
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
		super(sfWindow_CreateFromHandle(handle, &settings));
	}

	/**
	 *	Create (or recreate) the window
	 *	
	 *	Input created with getInput becomes invalid.	
	 *				
	 *	Params:
	 *		mode = Video mode to use
	 *		title = Title of the window
	 *		windowStyle = Window style (Resize | Close by default)
	 *		settings = Context settings (default is default ContextSettings values)
	 */
	void create(VideoMode mode, string title, Style windowStyle = Style.Default, ContextSettings settings = ContextSettings())
	{
		if (m_ptr !is null)
			dispose();

		m_ptr = sfWindow_Create(mode, toStringz(title), windowStyle, &settings);
	}

	/**
	 *	Create (or recreate) the window from an existing control
	 *	
	 *	Input created with getInput becomes invalid.	
	 *				
	 *	Params:
	 *		handle = Platform-specific handle of the control
	 *		settings = Context settings (default is default ContextSettings values)
	 */
	void create(WindowHandle handle, ContextSettings settings = ContextSettings())
	{
		if (m_ptr !is null)
			dispose();

		m_ptr = sfWindow_CreateFromHandle(handle, &settings);
	}

	/**
	 *	Close (destroy) the window.
	 *	You can call create to recreate a valid window	
	 */		
	void close()
	{
		sfWindow_Close(m_ptr);
	}


	/**
	 *	Get the event on top of events stack, if any, and pop it
	 *
	 *	Params:
	 *		eventReceived = Event to fill, if any
	 *
	 *	Returns:
	 *		True if an event was returned, false if events stack was empty
	 */
	bool getEvent(out Event eventReceived)
	{
		return cast(bool) sfWindow_GetEvent(m_ptr, &eventReceived);
	}
	
	/**
	 *	Wait for an event and return it
	 *
	 *	This function is blocking: if there's no pending event then it will wait until an event is received.
	 *	After this function returns (and no error occured), the \a event object is always valid and filled properly.
	 *	This function is typically used when you have a thread that is dedicated to events handling: you want to make this thread
	 *	sleep as long as no new event is received.
	 *
	 *	Params:
	 *		e Event to be returned
	 *
	 *	Returns:
	 *		false if any error occured
	 */
	bool waitEvent(out Event e)
	{
		return sfWindow_WaitEvent(m_ptr, &e);
	}
	
	/**
	 *	Show or hide the window
	 *
	 *	Params:
	 *		state = True to show, false to hide
	 *
	 */
	void show(bool state)
	{
		sfWindow_Show(m_ptr, state);
	}
	
	/**
	 *	Display the window on screen
	 */
	void display()
	{
		sfWindow_Display(m_ptr);
	}

	/**
	 *	Enable / disable vertical synchronization
	 *
	 *	Params: 
	 *		enabled : True to enable v-sync, false to deactivate
	 */
	void enableVerticalSync(bool enabled)
	{
		sfWindow_EnableVerticalSync(m_ptr, enabled);
	}

@property
{
	/**
	 *	Tell whether or not a window is opened
	 *	
	 *	Returns:
	 *		True if window is currently open.	
	 */		
	bool isOpened()
	{
		return cast(bool) sfWindow_IsOpened(m_ptr);
	}
	/**
	 *	Get the width of the rendering region of the window
	 *
	 *	Returns:
	 *		Width in pixels
	 */
	uint width()
	{
		return sfWindow_GetWidth(m_ptr);
	}

	/**
	 *	Get the height of the rendering region of the window
	 *
	 *	Returns:
	 *		Height in pixels
	 */
	uint height()
	{
		return sfWindow_GetHeight(m_ptr);
	}

	/**
	 *	Get the creation settings of a window
	 *	
	 *	Returns:
	 *		Settings used to create the window			
	 */
	ContextSettings settings()
	{
		return sfWindow_GetSettings(m_ptr);
	}		

	/**
	 *	Show or hide the mouse cursor
	 *
	 *	Params:
	 *		show : True to show, false to hide
	 */
	void showMouseCursor(bool show)
	{
		sfWindow_ShowMouseCursor(m_ptr, show);
	}
	
	/**
	 *	Enable or disable automatic key-repeat for keydown events.
	 *	Automatic key-repeat is enabled by default.
	 *	
	 *	Params:
	 *		enabled = true to enable, false to disable			
	 */		
	void enableKeyRepeat(bool enabled)
	{
		sfWindow_EnableKeyRepeat(m_ptr, enabled);
	}
	
	/**
	 *	Set the window as the current target for rendering
	 *	
	 *	Params:
	 *		active = True to activate, false to deactivate		
	 *	Returns:
	 *		True if operation was successful, false otherwise
	 */
	bool active(bool active)
	{
		return cast(bool)sfWindow_SetActive(m_ptr, active);
	}

	/**
	 *	Get the input manager of the window
	 *
	 *	Returns: 
	 *		An input manager
	 *	See_Also :
	 *		Input
	 */
	Input input()
	{
		if (m_input is null)
			m_input = new Input(sfWindow_GetInput(m_ptr));
		return m_input;
	}

	/**
	 *	Limit the framerate to a maximum fixed frequency
	 *
	 *	Params:
	 *		limit : Framerate limit, in frames per seconds (use 0 to disable limit)
	 */
	void framerateLimit(uint limit)
	{
		sfWindow_SetFramerateLimit(m_ptr, limit);
	}

	/**
	 *	Get the time the last frame took
	 *
	 *	Returns:
	 *		time in seconds
	 */
	float frameTime()
	{
		return sfWindow_GetFrameTime(m_ptr);
	}

	/**
	 *	Change the joystick threshold, ie. the value below which
	 *	no move event will be generated
	 *
	 *	Params:
	 *		threshold : New threshold, in range [0, 100]
	 */
	void joystickThreshold(float threshold)
	{
		sfWindow_SetJoystickThreshold(m_ptr, threshold);
	}
	
	/**
	 *	Retrieve the Os-specific handle of a window
	 *
	 *	Params:
	 *		renderWindow = Renderwindow object
	 */
	WindowHandle windowHandle()
	{
		return sfWindow_GetSystemHandle(m_ptr);
	}
}

	/**
	 *	Change the position of the mouse cursor
	 *
	 *	Params:
	 *		left = Left coordinate of the cursor, relative to the window
	 *		top = Top coordinate of the cursor, relative to the window
	 */
	void setCursorPosition(uint left, uint top)
	{
		sfWindow_SetCursorPosition(m_ptr, left, top);
	}

	/**
	 *	Change the position of the window on screen.
	 *	Only works for top-level windows
	 *
	 *	Params:
	 *		left = Left position
	 *		top = Top position
	 */
	void setPosition(int left, int top)
	{
		sfWindow_SetPosition(m_ptr, left, top);
	}
	
	/**
	 *	change the size of the rendering region of the window
	 *	
	 *	Params:
	 *		width : new width
	 *		height : new height							
	 */
	void setSize(uint width, uint height)
	{
		sfWindow_SetSize(m_ptr, width, height);
	}
	
	/**
	 *	Change the window's icon
	 *
	 *	Params:
	 *		width = Icon's width, in pixels
	 *		height = Icon's height, in pixels
	 *		data = array of pixels in memory, format must be RGBA 32 bits
	 *
	 */
	void setIcon(size_t width, size_t height, ubyte[] data)
	{
		sfWindow_SetIcon(m_ptr, width, height, data.ptr);
	}

private:

// External ====================================================================
	static extern(C)
	{
		SFMLClass			function(VideoMode, cchar*, Style, ContextSettings*)sfWindow_Create;
		SFMLClass			function(WindowHandle, ContextSettings*)			sfWindow_CreateFromHandle;
		void			function(SFMLClass)									sfWindow_Destroy;
		void			function(SFMLClass)									sfWindow_Close;
		int				function(SFMLClass)									sfWindow_IsOpened;
		uint			function(SFMLClass)									sfWindow_GetWidth;
		uint			function(SFMLClass)									sfWindow_GetHeight;
		ContextSettings	function(SFMLClass Window)							sfWindow_GetSettings;
		int				function(SFMLClass, Event*)							sfWindow_GetEvent;
		void			function(SFMLClass, int)							sfWindow_EnableVerticalSync;
		void			function(SFMLClass, int)							sfWindow_ShowMouseCursor;
		void			function(SFMLClass, uint, uint)						sfWindow_SetCursorPosition;
		void			function(SFMLClass, int, int)						sfWindow_SetPosition;
		void			function(SFMLClass, uint, uint)						sfWindow_SetSize;
		void			function(SFMLClass, int)							sfWindow_Show;
		void			function(SFMLClass, int)							sfWindow_EnableKeyRepeat;
		void			function(SFMLClass, size_t, size_t, ubyte*)			sfWindow_SetIcon;
		int				function(SFMLClass, int)							sfWindow_SetActive;
		void			function(SFMLClass)									sfWindow_Display;
		SFMLClass		function(SFMLClass)									sfWindow_GetInput;
		void			function(SFMLClass, uint)							sfWindow_SetFramerateLimit;
		float			function(SFMLClass)									sfWindow_GetFrameTime;
		void			function(SFMLClass, float)							sfWindow_SetJoystickThreshold;

		// DSFML2
		bool			function(SFMLClass, void*)							sfWindow_WaitEvent;
		WindowHandle	function(SFMLClass)									sfWindow_GetSystemHandle;
	}

	mixin(loadFromSharedLib2("csfml-window", "sfWindow",
	"Create", "CreateFromHandle", "Destroy", "Close", "IsOpened", "GetWidth", "GetHeight", "GetSettings", "GetEvent", "EnableVerticalSync",
	"ShowMouseCursor", "SetCursorPosition", "SetPosition", "SetSize", "Show", "EnableKeyRepeat", "SetIcon", "SetActive", "Display",
	"GetInput", "SetFramerateLimit", "GetFrameTime", "SetJoystickThreshold", "WaitEvent", "GetSystemHandle"));
}
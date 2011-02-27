/* rbSFML - Copyright (c) 2010 Henrik Valter Vogelius Hansson - groogy@groogy.se
 * This software is provided 'as-is', without any express or
 * implied warranty. In no event will the authors be held
 * liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute
 * it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented;
 *    you must not claim that you wrote the original software.
 *    If you use this software in a product, an acknowledgment
 *    in the product documentation would be appreciated but
 *    is not required.
 *
 * 2. Altered source versions must be plainly marked as such,
 *    and must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any
 *    source distribution.
 */
 
#include <SFML/Window/Window.hpp>
#include "Window.hpp"
#include "VideoMode.hpp"
#include "Vector2.hpp"
#include "main.hpp"

VALUE globalWindowClass;

/* External classes */
extern VALUE globalVideoModeClass;
extern VALUE globalContextSettingsClass;
extern VALUE globalEventClass;
extern VALUE globalInputClass;
extern VALUE globalVector2Class;
extern VALUE globalNonCopyableModule;

/* Free a heap allocated object 
 * Not accessible trough ruby directly!
 */
static void Window_Free( sf::Window *anObject )
{
	delete anObject;
}

/* call-seq:
 *   window.close()
 *
 * Close the window and destroy all the attached resources. 
 *
 * After calling this function, the SFML::Window instance remains valid and you can call SFML::Window#create to recreate
 * the window. All other functions such as getEvent or display will still work (i.e. you don't have to test 
 * isOpened every time), and will have no effect on closed windows. 
 */
static VALUE Window_Close( VALUE self )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->Close();
	return Qnil;
}

/* call-seq:
 *   window.create( mode, title, style = SFML::Style::Default, settings = SFML::ContextSettings.new )
 *
 * Create (or recreate) the window. 
 *
 * If the window was already created, it closes it first. If style contains Style::Fullscreen, 
 * then mode must be a valid video mode.
 */
static VALUE Window_Create( int argc, VALUE *args, VALUE self )
{
	sf::Window *object = NULL;
	sf::VideoMode *mode = NULL;
	sf::ContextSettings *settings = NULL;
	VALUE arg0 = Qnil;
	
	Data_Get_Struct( self, sf::Window, object );
	switch( argc )
	{
		case 2:
			arg0 = VideoMode_ForceType( args[0] );
			VALIDATE_CLASS( arg0, globalVideoModeClass, "first" );
			VALIDATE_CLASS( args[1], rb_cString, "second" );
			Data_Get_Struct( arg0, sf::VideoMode, mode );
			object->Create( *mode ,rb_string_value_cstr( &args[1] ) );
			break;
		case 3:
			arg0 = VideoMode_ForceType( args[0] );
			VALIDATE_CLASS( arg0, globalVideoModeClass, "first" );
			VALIDATE_CLASS( args[1], rb_cString, "second" );
			VALIDATE_CLASS( args[2], rb_cFixnum, "third" );
			Data_Get_Struct( arg0, sf::VideoMode, mode );
			object->Create( *mode, rb_string_value_cstr( &args[1] ), FIX2INT( args[2] ) );
			break;
		case 4:
			arg0 = VideoMode_ForceType( args[0] );
			VALIDATE_CLASS( arg0, globalVideoModeClass, "first" );
			VALIDATE_CLASS( args[1], rb_cString, "second" );
			VALIDATE_CLASS( args[2], rb_cFixnum, "third" );
			VALIDATE_CLASS( args[3], globalContextSettingsClass, "fourth" );
			Data_Get_Struct( arg0, sf::VideoMode, mode );
			Data_Get_Struct( args[3], sf::ContextSettings, settings );
			object->Create( *mode, rb_string_value_cstr( &args[1] ), FIX2INT( args[2] ), *settings );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 2..4 arguments but was given %d", argc );
			break;
	}
	return Qnil;
}

/* call-seq:
 *   window.display()
 *
 * Display on screen what has been rendered to the window so far. 
 *
 * This function is typically called after all OpenGL rendering has been done for the current frame, in order to show 
 * it on screen. 
 */
static VALUE Window_Display( VALUE self )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->Display();
	return Qnil;
}

/* call-seq:
 *   window.enableKeyRepeat( enable )
 *
 * Enable or disable automatic key-repeat. 
 *
 * If key repeat is enabled, you will receive repeated KeyPress events while keeping a key pressed. If it is disabled, 
 * you will only get a single event when the key is pressed.
 *
 * Key repeat is enabled by default.
 */
static VALUE Window_EnableKeyRepeat( VALUE self, VALUE anEnableFlag )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	if( anEnableFlag == Qfalse )
	{
		object->EnableKeyRepeat( false );
	}
	else if( anEnableFlag == Qtrue )
	{
		object->EnableKeyRepeat( true );
	}
	else
	{
		rb_raise( rb_eTypeError, "Expected true or false" );
	}
	return Qnil;
}

/* call-seq:
 *   window.getEvent()	-> event or nil
 *
 * Pop the event on top of events stack, if any, and return it.
 *
 * This function is not blocking: if there's no pending event then it will return nil. Note that more than the returned
 * event may be present in the events stack, thus you should always call this function in a loop to make sure that you
 * process every pending event. 
 */
static VALUE Window_GetEvent( VALUE self )
{
	sf::Event event;
	sf::Window *window = NULL;
	Data_Get_Struct( self, sf::Window, window );
	if( window->GetEvent( event ) == true )
	{
		VALUE rbObject = rb_funcall( globalEventClass, rb_intern( "new" ), 1, INT2FIX( event.Type ) );
		sf::Event *rubyRawEvent = NULL;
		Data_Get_Struct( rbObject, sf::Event, rubyRawEvent );
		*rubyRawEvent = event;
		return rbObject;
	}
	else
	{
		return Qnil;
	}	
}

/* call-seq:
 *   window.getFrameTime()	-> float
 *
 * This function returns the time elapsed during the last frame. This can be useful for calculating the framerate, or 
 * for updating the application's objects.
 */
static VALUE Window_GetFrameTime( VALUE self )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	return rb_float_new( object->GetFrameTime() );
}

/* call-seq:
 *   window.getHeight()	-> fixnum
 *
 * Get the height of the rendering region of the window.
 *
 * The height doesn't include the titlebar and borders of the window.
 */
static VALUE Window_GetHeight( VALUE self )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	return INT2FIX( object->GetHeight() );
}

/* call-seq:
 *   window.getInput()	-> input
 *
 * This input gives access to the real-time state of keyboard, mouse and joysticks for this window
 */
static VALUE Window_GetInput( VALUE self )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	VALUE rbData = Data_Wrap_Struct( globalInputClass, 0, 0, const_cast< sf::Input * >( &object->GetInput() ) );
	rb_obj_call_init( rbData, 0, 0 );
	rb_iv_set( rbData, "@__owner_ref", self );
	return rbData;
}

/* call-seq:
 *   window.getSettings()	-> settings
 *
 * Get the settings of the OpenGL context of the window.
 *
 * Note that these settings may be different from what was passed to the constructor or the Create() function, if one 
 * or more settings were not supported. In this case, SFML chose the closest match.
 */
static VALUE Window_GetSettings( VALUE self )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	VALUE rbData = Data_Wrap_Struct( globalContextSettingsClass, 0, 0, const_cast<sf::ContextSettings *>( &object->GetSettings() ) );
	rb_obj_call_init( rbData, 0, 0 );
	rb_iv_set( rbData, "@__owner_ref", self );
	return rbData;
}

/* call-seq:
 *   window.getWidth()	-> fixnum
 *
 * Get the width of the rendering region of the window.
 *
 * The width doesn't include the titlebar and borders of the window.
 */
static VALUE Window_GetWidth( VALUE self )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	return INT2FIX( object->GetWidth() );
}

/* call-seq:
 *   window.isOpened()	-> true or false
 *
 * This function returns whether or not the window exists. Note that a hidden window (Show(false)) will return true.
 */
static VALUE Window_IsOpened( VALUE self )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	if( object->IsOpened() == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

/* call-seq:
 *   window.setActive( activate )	-> true or false
 *
 * Activate or deactivate the window as the current target for OpenGL rendering.
 *
 * A window is active only on the current thread, if you want to make it active on another thread you have to 
 * deactivate it on the previous thread first if it was active. Only one window can be active on a thread at a time, 
 * thus the window previously active (if any) automatically gets deactivated.
 */
static VALUE Window_SetActive( VALUE self, VALUE anActiveFlag )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	if( anActiveFlag == Qfalse )
	{
		object->SetActive( false );
	}
	else if( anActiveFlag == Qtrue )
	{
		object->SetActive( true );
	}
	else
	{
		rb_raise( rb_eTypeError, "Expected true or false" );
	}
	return Qnil;
}

/* call-seq:
 *   window.setCursorPosition( new_x, new_y )
 *
 * Change the position of the mouse cursor. 
 */
static VALUE Window_SetCursorPosition( VALUE self, VALUE aX, VALUE aY )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->SetCursorPosition( FIX2UINT( aX ), FIX2UINT( aY ) );
	return Qnil;
}

/* call-seq:
 *   window.cursorPosition=( vector2 )
 *
 * Change the position of the mouse cursor. 
 */
static VALUE Window_SetCursorPosition2( VALUE self, VALUE anArgument )
{
	VALUE argument = Vector2_ForceType( anArgument );
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	VALUE argumentX = Vector2_GetX( argument );
	VALUE argumentY = Vector2_GetY( argument );
	object->SetCursorPosition( FIX2UINT( argumentX ), FIX2UINT( argumentY ) );
	return Qnil;
}

/* call-seq:
 *   window.setFramerateLimit( new_limit )
 *
 * Limit the framerate to a maximum fixed frequency.
 *
 * If a limit is set, the window will use a small delay after each call to Display() to ensure that the current frame 
 * lasted long enough to match the framerate limit.
 */
static VALUE Window_SetFramerateLimit( VALUE self, VALUE aLimit )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->SetFramerateLimit( FIX2UINT( aLimit ) );
	return Qnil;
}

/* call-seq:
 *   window.setIcon( width, height, pixels )
 *
 * Change the window's icon.
 *
 * pixels must be an array of width x height pixels in 32-bits RGBA format. In the ruby binding the array will be
 * flattened so you can have array's up to 3 dimensions(or more) to represent each pixel component. The size of the
 * array will be assumed to be width * height * 4.
 *
 * The OS default icon is used by default.
 * 
 * Usage example:
 *   pixels = [
 *     [[255, 0, 0, 255], [0, 0, 255, 255]],
 *     [[0, 255, 0, 255], [0, 0, 0, 255]]
 *   ]
 *   
 *   window.setIcon( 2, 2, pixels )
 */
static VALUE Window_SetIcon( VALUE self, VALUE aWidth, VALUE aHeight, VALUE somePixels )
{
	const unsigned int rawWidth = FIX2UINT( aWidth );
	const unsigned int rawHeight = FIX2UINT( aHeight );
	VALIDATE_CLASS( somePixels, rb_cArray, "pixels" );
	const unsigned long dataSize = rawWidth * rawHeight * 4;
	sf::Uint8 * const tempData = new sf::Uint8[dataSize];
	VALUE pixels = rb_funcall( somePixels, rb_intern("flatten"), 0 );
	for(unsigned long index = 0; index < dataSize; index++)
	{
		sf::Uint8 val = NUM2CHR( rb_ary_entry( pixels, index ) );
		tempData[index] = val;
	}
	
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->SetIcon( rawWidth, rawHeight, tempData );
	delete[] tempData;
	return Qnil;
}

/* call-seq:
 *   window.setJoystickThreshold( new_threshold )
 *
 * Change the joystick threshold.
 *
 * The joystick threshold is the value below which no JoyMoved event will be generated.
 *
 * The threshold value is 0.1 by default. The threshold has to be in the range 0..100
 */
static VALUE Window_SetJoystickThreshold( VALUE self, VALUE aThreshold )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->SetJoystickThreshold( rb_float_new( aThreshold ) );
	return Qnil;
}

/* call-seq:
 *   window.setPosition( new_x, new_y )
 *
 * Change the position of the window on screen.
 *
 * This function only works for top-level windows (i.e. it will be ignored for windows created from the handle of a 
 * child window/control).
 */
static VALUE Window_SetPosition( VALUE self, VALUE aX, VALUE aY )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->SetPosition( FIX2INT( aX ), FIX2INT( aY ) );
	return Qnil;
}

/* call-seq:
 *   window.position=( vector2 )
 *
 * Change the position of the window on screen.
 *
 * This function only works for top-level windows (i.e. it will be ignored for windows created from the handle of a 
 * child window/control).
 */
static VALUE Window_SetPosition2( VALUE self, VALUE anArgument )
{
	VALUE argument = Vector2_ForceType( anArgument );
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	VALUE argumentX = Vector2_GetX( argument );
	VALUE argumentY = Vector2_GetY( argument );
	object->SetPosition( FIX2UINT( argumentX ), FIX2INT( argumentY ) );
	return Qnil;
}

/* call-seq:
 *   window.setSize( new_width, new_height )
 *
 * Change the size of the rendering region of the window. 
 */
static VALUE Window_SetSize( VALUE self, VALUE aWidth, VALUE aHeight )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->SetSize( FIX2UINT( aWidth ), FIX2UINT( aHeight ) );
	return Qnil;
}

/* call-seq:
 *   window.size=( vector2 )
 *
 * Change the size of the rendering region of the window. 
 */
static VALUE Window_SetSize2( VALUE self, VALUE anArgument )
{
	VALUE argument = Vector2_ForceType( anArgument );
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	VALUE argumentX = Vector2_GetX( argument );
	VALUE argumentY = Vector2_GetY( argument );
	object->SetSize( FIX2UINT( argumentX ), FIX2UINT( argumentY ) );
	return Qnil;
}

/* call-seq:
 *   window.setTitle( new_title )
 *
 * Change the title of the window. 
 */
static VALUE Window_SetTitle( VALUE self, VALUE aTitle )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->SetTitle( rb_string_value_cstr( &aTitle ) );
	return Qnil;
}

/* call-seq:
 *   window.show( show )
 *
 * Show or hide the window.
 *
 * The window is shown by default. 
 */
static VALUE Window_Show( VALUE self, VALUE aShowFlag )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	if( aShowFlag == Qfalse )
	{
		object->Show( false );
	}
	else if( aShowFlag == Qtrue )
	{
		object->Show( true );
	}
	else
	{
		rb_raise( rb_eTypeError, "Expected true or false" );
	}
	return Qnil;
}

/* call-seq:
 *   window.showMouseCursor( show )
 *
 * Show or hide the mouse cursor.
 *
 * The mouse cursor is shown by default.
 */
static VALUE Window_ShowMouseCursor( VALUE self, VALUE aShowFlag )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	if( aShowFlag == Qfalse )
	{
		object->ShowMouseCursor( false );
	}
	else if( aShowFlag == Qtrue )
	{
		object->ShowMouseCursor( true );
	}
	else
	{
		rb_raise( rb_eTypeError, "Expected true or false" );
	}
	return Qnil;
}

/* call-seq:
 *   window.enableVerticalSync( enabled )
 *
 * Enable or disable vertical synchronization.
 *
 * Activating vertical synchronization will limit the number of frames displayed to the refresh rate of the monitor. 
 * This can avoid some visual artifacts, and limit the framerate to a good value (but not constant across different 
 * computers).
 *
 * Vertical synchronization is disabled by default.
 */
static VALUE Window_EnableVerticalSync( VALUE self, VALUE aEnableFlag )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	if( aEnableFlag == Qfalse )
	{
		object->EnableVerticalSync( false );
	}
	else if( aEnableFlag == Qtrue )
	{
		object->EnableVerticalSync( true );
	}
	else
	{
		rb_raise( rb_eTypeError, "Expected true or false" );
	}
	return Qnil;
}

/* call-seq:
 *   window.waitEvent()	-> event or nil
 *
 * Wait for an event and return it.
 *
 * This function is blocking: if there's no pending event then it will wait until an event is received. After this 
 * function returns (and no error occured), the event object is always valid and filled properly. This function is 
 * typically used when you have a thread that is dedicated to events handling: you want to make this thread sleep as 
 * long as no new event is received. 
 */
static VALUE Window_WaitEvent( VALUE self )
{
	sf::Event event;
	sf::Window *window = NULL;
	Data_Get_Struct( self, sf::Window, window );
	if( window->WaitEvent( event ) == true )
	{
		VALUE rbObject = rb_funcall( globalEventClass, rb_intern( "new" ), 1, INT2FIX( event.Type ) );
		sf::Event *rubyRawEvent = NULL;
		Data_Get_Struct( rbObject, sf::Event, rubyRawEvent );
		*rubyRawEvent = event;
		return rbObject;
	}
	else
	{
		return Qnil;
	}	
}

/* call-seq:
 *   Window.new()																					-> window
 *   Window.new( mode, title, style = SFML::Style::Default, settings = SFML::ContextSettings.new )	-> window
 *
 * Construct a new window.
 *
 * The first form of new doesn't actually create the visual window, use the other form of new or call 
 * SFML::Window#create to do so.
 *
 * The second form of new creates the window with the size and pixel depth defined in mode. An optional style can be passed 
 * to customize the look and behaviour of the window (borders, title bar, resizable, closable, ...). If style contains 
 * Style::Fullscreen, then mode must be a valid video mode.
 *
 * The fourth parameter is an optional structure specifying advanced OpenGL context settings such as antialiasing, 
 * depth-buffer bits, etc. You shouldn't care about these parameters for a regular usage of the graphics module.
 */
static VALUE Window_Initialize( int argc, VALUE *args, VALUE self )
{
	if( argc > 0 )
	{
		rb_funcall2( self, rb_intern( "create" ), argc, args );
	}
	return self;
}

static VALUE Window_Alloc( VALUE aKlass )
{
	sf::Window *object = new sf::Window();
	return Data_Wrap_Struct( aKlass, 0, Window_Free, object );
}

void Init_Window( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* SFML::Window is the main class of the Window module.
 *
 * It defines an OS window that is able to receive an OpenGL rendering.
 *
 * A SFML::Window can create its own new window, but using an already created window trough a handle is not supported
 * in the ruby bindings yet.
 *
 * The SFML::Window class provides a simple interface for manipulating the window: move, resize, show/hide, control mouse 
 * cursor, etc. It also provides event handling through its getEvent() function, and real-time state handling with its 
 * attached SFML::Input object (see getInput()).
 *
 * Note that OpenGL experts can pass their own parameters (antialiasing level, bits for the depth and stencil buffers, 
 * etc.) to the OpenGL context attached to the window, with the SFML::ContextSettings structure which is passed as an 
 * optional argument when creating the window.
 *
 * Usage example:
 *
 *   # Declare and create a new window
 *   window = SFML::Window.new( SFML::VideoMode.new( 800, 600 ), "SFML window" )
 *
 *   # Limit the framerate to 60 frames per second (this step is optional)
 *   window.setFramerateLimit( 60 );
 *
 *   # The main loop - ends as soon as the window is closed
 *   while window.open?
 *
 *     # Event processing
 *     while event = window.getEvent
 *
 *       # Request for closing the window
 *       if event.type == SFML::Event::Closed
 *         window.close()
 *       end
 *     end
 *
 *     # Activate the window for OpenGL rendering
 *     window.setActive()
 *
 *     # OpenGL drawing commands go here...
 *
 *     # End the current frame and display its contents on screen
 *     window.display()
 *   end
 */
	globalWindowClass = rb_define_class_under( sfml, "Window", rb_cObject );
	rb_include_module( globalWindowClass, globalNonCopyableModule );
	
	// Class methods
	//rb_define_singleton_method( globalWindowClass, "new", Window_New , -1 );
	rb_define_alloc_func( globalWindowClass, Window_Alloc );
	
	// Instance methods
	rb_define_method( globalWindowClass, "initialize", Window_Initialize, -1 );
	rb_define_method( globalWindowClass, "close", Window_Close, 0 );
	rb_define_method( globalWindowClass, "create", Window_Create, -1 );
	rb_define_method( globalWindowClass, "display", Window_Display, 0 );
	rb_define_method( globalWindowClass, "enableKeyRepeat", Window_EnableKeyRepeat, 1 );
	rb_define_method( globalWindowClass, "getEvent", Window_GetEvent, 0 );
	rb_define_method( globalWindowClass, "getFrameTime", Window_GetFrameTime , 0 );
	rb_define_method( globalWindowClass, "getHeight", Window_GetHeight, 0 );
	rb_define_method( globalWindowClass, "getInput", Window_GetInput, 0 );
	rb_define_method( globalWindowClass, "getSettings", Window_GetSettings, 0 );
	rb_define_method( globalWindowClass, "getWidth", Window_GetWidth, 0 );
	rb_define_method( globalWindowClass, "isOpened", Window_IsOpened, 0 );
	rb_define_method( globalWindowClass, "setActive", Window_SetActive, 1 );
	rb_define_method( globalWindowClass, "setCursorPosition", Window_SetCursorPosition, 2 );
	rb_define_method( globalWindowClass, "cursorPosition=", Window_SetCursorPosition2, 1 );
	rb_define_method( globalWindowClass, "setFramerateLimit", Window_SetFramerateLimit, 1 );
	rb_define_method( globalWindowClass, "setIcon", Window_SetIcon, 3 );
	rb_define_method( globalWindowClass, "setJoystickThreshold", Window_SetJoystickThreshold, 1 );
	rb_define_method( globalWindowClass, "setPosition", Window_SetPosition, 2 );
	rb_define_method( globalWindowClass, "position=", Window_SetPosition2, 1 );
	rb_define_method( globalWindowClass, "setSize", Window_SetSize, 2 );
	rb_define_method( globalWindowClass, "size=", Window_SetSize2, 1 );
	rb_define_method( globalWindowClass, "setTitle", Window_SetTitle, 1 );
	rb_define_method( globalWindowClass, "show", Window_Show, 1 );
	rb_define_method( globalWindowClass, "showMouseCursor", Window_ShowMouseCursor, 1 );
	rb_define_method( globalWindowClass, "enableVerticalSync", Window_EnableVerticalSync, 1 );
	rb_define_method( globalWindowClass, "waitEvent", Window_WaitEvent, 0 );
	
	// Aliases
	rb_define_alias( globalWindowClass, "keyRepeat=", "enableKeyRepeat" );
	rb_define_alias( globalWindowClass, "key_repeat=", "enableKeyRepeat" );
	
	rb_define_alias( globalWindowClass, "get_event", "getEvent" );
	
	rb_define_alias( globalWindowClass, "frameTime", "getFrameTime" );
	rb_define_alias( globalWindowClass, "frame_time", "getFrameTime" );
	
	rb_define_alias( globalWindowClass, "height", "getHeight" );
	rb_define_alias( globalWindowClass, "input", "getInput" );
	rb_define_alias( globalWindowClass, "settings", "getSettings" );
	rb_define_alias( globalWindowClass, "width", "getWidth" );
	
	rb_define_alias( globalWindowClass, "opened?", "isOpened" );
	rb_define_alias( globalWindowClass, "open?", "isOpened" );
	
	rb_define_alias( globalWindowClass, "active=", "setActive" );
	
	rb_define_alias( globalWindowClass, "set_cursor_position", "setCursorPosition" );
	rb_define_alias( globalWindowClass, "cursor_position=", "cursorPosition=" );
	
	rb_define_alias( globalWindowClass, "framerateLimit=", "setFramerateLimit" );
	rb_define_alias( globalWindowClass, "framerate_limit=", "setFramerateLimit" );
	rb_define_alias( globalWindowClass, "framerate=", "setFramerateLimit" );
	
	rb_define_alias( globalWindowClass, "set_icon", "setIcon" );
	
	rb_define_alias( globalWindowClass, "joystickThreshold=", "setJoystickThreshold" );
	rb_define_alias( globalWindowClass, "joystick_threshold=", "setJoystickThreshold" );
	
	rb_define_alias( globalWindowClass, "set_position", "setPosition" );
	rb_define_alias( globalWindowClass, "set_size", "setSize" );
	rb_define_alias( globalWindowClass, "title=", "setTitle" );
	
	rb_define_alias( globalWindowClass, "showMouseCursor=", "showMouseCursor" );
	rb_define_alias( globalWindowClass, "show_mouse_cursor", "showMouseCursor" );
	rb_define_alias( globalWindowClass, "show_mouse_cursor=", "showMouseCursor" );
	
	rb_define_alias( globalWindowClass, "enableVerticalSync=", "enableVerticalSync" );
	rb_define_alias( globalWindowClass, "enable_vertical_sync", "enableVerticalSync" );
	rb_define_alias( globalWindowClass, "enable_vertical_sync=", "enableVerticalSync" );
	rb_define_alias( globalWindowClass, "vertical_sync=", "enableVerticalSync" );
	
	rb_define_alias( globalWindowClass, "wait_event", "waitEvent" );
}

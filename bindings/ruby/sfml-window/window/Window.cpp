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
#include "main.hpp"

/* SFML::Window is the main class of the Window module.
 *
 * It defines an OS window that is able to receive an OpenGL rendering.
 *
 * A SFML::Window can create its own new window, or be embedded into an already existing control using the Create(handle)
 * function. This can be useful for embedding an OpenGL rendering area into a view which is part of a bigger GUI with 
 * existing windows, controls, etc. It can also serve as embedding an OpenGL rendering area into a window created by 
 * another (probably richer) GUI library like Qt or wxWidgets.
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
VALUE globalWindowClass;
extern VALUE globalVideoModeClass;
extern VALUE globalContextSettingsClass;
extern VALUE globalEventClass;
extern VALUE globalInputClass;

#define VALIDATE_CLASS( variable, type, name ) \
if( CLASS_OF( variable ) != type ) \
{ \
	rb_raise( rb_eTypeError, "%s argument must be instance of %s", name, rb_string_value_cstr ( &type ) ); \
}

/* Free a heap allocated object 
 * Not accessible trough ruby directly!
 */
static void Window_Free( sf::Window *anObject )
{
	delete anObject;
}

static VALUE Window_Close( VALUE self )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->Close();
	return Qnil;
}

static VALUE Window_Create( int argc, VALUE *args, VALUE self )
{
	sf::Window *object = NULL;
	sf::VideoMode *mode = NULL;
	sf::ContextSettings *settings = NULL;
	
	Data_Get_Struct( self, sf::Window, object );
	switch( argc )
	{
		case 2:
			VALIDATE_CLASS( args[0], globalVideoModeClass, "first" );
			VALIDATE_CLASS( args[1], rb_cString, "second" );
			Data_Get_Struct( args[0], sf::VideoMode, mode );
			object->Create( *mode ,rb_string_value_cstr( &args[1] ) );
			break;
		case 3:
			VALIDATE_CLASS( args[0], globalVideoModeClass, "first" );
			VALIDATE_CLASS( args[1], rb_cString, "second" );
			VALIDATE_CLASS( args[2], rb_cFixnum, "third" );
			Data_Get_Struct( args[0], sf::VideoMode, mode );
			object->Create( *mode, rb_string_value_cstr( &args[1] ), FIX2UINT( args[2] ) );
			break;
		case 4:
			VALIDATE_CLASS( args[0], globalVideoModeClass, "first" );
			VALIDATE_CLASS( args[1], rb_cString, "second" );
			VALIDATE_CLASS( args[2], rb_cFixnum, "third" );
			VALIDATE_CLASS( args[3], globalContextSettingsClass, "fourth" );
			Data_Get_Struct( args[0], sf::VideoMode, mode );
			Data_Get_Struct( args[3], sf::ContextSettings, settings );
			object->Create( *mode, rb_string_value_cstr( &args[1] ), FIX2UINT( args[2] ), *settings );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 2..4 arguments but was given %d", argc );
			break;
	}
	return Qnil;
}


static VALUE Window_Display( VALUE self )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->Display();
	return Qnil;
}

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

static VALUE Window_GetFrameTime( VALUE self )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	return rb_float_new( object->GetFrameTime() );
}

static VALUE Window_GetHeight( VALUE self )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	return INT2FIX( object->GetHeight() );
}

static VALUE Window_GetInput( VALUE self )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	VALUE rbData = Data_Wrap_Struct( globalInputClass, 0, 0, const_cast< sf::Input * >( &object->GetInput() ) );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

static VALUE Window_GetSettings( VALUE self )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	VALUE rbData = Data_Wrap_Struct( globalContextSettingsClass, 0, 0, const_cast<sf::ContextSettings *>( &object->GetSettings() ) );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

static VALUE Window_GetWidth( VALUE self )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	return INT2FIX( object->GetWidth() );
}

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

static VALUE Window_SetCursorPosition( VALUE self, VALUE aX, VALUE aY )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->SetCursorPosition( FIX2UINT( aX ), FIX2UINT( aY ) );
	return Qnil;
}

static VALUE Window_SetCursorPosition2( VALUE self, VALUE anArray )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	VALIDATE_CLASS( anArray, rb_cArray, "first" );
	VALUE arraySize = rb_funcall( anArray, rb_intern( "size" ), 0 );
	if( FIX2UINT( arraySize ) != 2 )
	{
		rb_raise( rb_eArgError, "Expected 2 elements in array" );
	}
	object->SetCursorPosition( FIX2UINT( rb_ary_entry( anArray, 0 ) ), FIX2UINT( rb_ary_entry( anArray, 1 ) ) );
	return Qnil;
}

static VALUE Window_SetFramerateLimit( VALUE self, VALUE aLimit )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->SetFramerateLimit( FIX2UINT( aLimit ) );
	return Qnil;
}

static VALUE Window_SetIcon( VALUE self, VALUE aWidth, VALUE aHeight, VALUE somePixels )
{
	const unsigned int rawWidth = FIX2UINT( aWidth );
	const unsigned int rawHeight = FIX2UINT( aHeight );
	VALIDATE_CLASS( somePixels, rb_cArray, "third" );
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

static VALUE Window_SetJoystickThreshold( VALUE self, VALUE aThreshold )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->SetJoystickThreshold( rb_float_new( aThreshold ) );
	return Qnil;
}

static VALUE Window_SetPosition( VALUE self, VALUE aX, VALUE aY )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->SetPosition( FIX2UINT( aX ), FIX2UINT( aY ) );
	return Qnil;
}

static VALUE Window_SetPosition2( VALUE self, VALUE anArray )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	VALIDATE_CLASS( anArray, rb_cArray, "first" );
	VALUE arraySize = rb_funcall( anArray, rb_intern( "size" ), 0 );
	if( FIX2UINT( arraySize ) != 2 )
	{
		rb_raise( rb_eArgError, "Expected 2 elements in array" );
	}
	object->SetPosition( FIX2UINT( rb_ary_entry( anArray, 0 ) ), FIX2UINT( rb_ary_entry( anArray, 1 ) ) );
	return Qnil;
}

static VALUE Window_SetSize( VALUE self, VALUE aWidth, VALUE aHeight )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->SetSize( FIX2UINT( aWidth ), FIX2UINT( aHeight ) );
	return Qnil;
}

static VALUE Window_SetSize2( VALUE self, VALUE anArray )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	VALIDATE_CLASS( anArray, rb_cArray, "first" );
	VALUE arraySize = rb_funcall( anArray, rb_intern( "size" ), 0 );
	if( FIX2UINT( arraySize ) != 2 )
	{
		rb_raise( rb_eArgError, "Expected 2 elements in array" );
	}
	object->SetSize( FIX2UINT( rb_ary_entry( anArray, 0 ) ), FIX2UINT( rb_ary_entry( anArray, 1 ) ) );
	return Qnil;
}

static VALUE Window_SetTitle( VALUE self, VALUE aTitle )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	object->SetTitle( rb_string_value_cstr( &aTitle ) );
	return Qnil;
}

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

static VALUE Window_UseVerticalSync( VALUE self, VALUE aEnableFlag )
{
	sf::Window *object = NULL;
	Data_Get_Struct( self, sf::Window, object );
	if( aEnableFlag == Qfalse )
	{
		object->UseVerticalSync( false );
	}
	else if( aEnableFlag == Qtrue )
	{
		object->UseVerticalSync( true );
	}
	else
	{
		rb_raise( rb_eTypeError, "Expected true or false" );
	}
	return Qnil;
}

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

static VALUE Window_New( int argc, VALUE *args, VALUE aKlass )
{
	sf::Window *object = NULL;
	sf::VideoMode *mode = NULL;
	sf::ContextSettings *settings = NULL;
	switch( argc )
	{
		case 0:
			object = new sf::Window();
			break;
		case 2:
			VALIDATE_CLASS( args[0], globalVideoModeClass, "first" );
			VALIDATE_CLASS( args[1], rb_cString, "second" );
			Data_Get_Struct( args[0], sf::VideoMode, mode );
			object = new sf::Window( *mode , rb_string_value_cstr( &args[1] ) );
			break;
		case 3:
			VALIDATE_CLASS( args[0], globalVideoModeClass, "first" );
			VALIDATE_CLASS( args[1], rb_cString, "second" );
			VALIDATE_CLASS( args[2], rb_cFixnum, "third" );
			Data_Get_Struct( args[0], sf::VideoMode, mode );
			object = new sf::Window( *mode, rb_string_value_cstr( &args[1] ), FIX2UINT( args[2] ) );
			break;
		case 4:
			VALIDATE_CLASS( args[0], globalVideoModeClass, "first" );
			VALIDATE_CLASS( args[1], rb_cString, "second" );
			VALIDATE_CLASS( args[2], rb_cFixnum, "third" );
			VALIDATE_CLASS( args[3], globalContextSettingsClass, "fourth" );
			Data_Get_Struct( args[0], sf::VideoMode, mode );
			Data_Get_Struct( args[3], sf::ContextSettings, settings );
			object = new sf::Window( *mode, rb_string_value_cstr( &args[1] ), FIX2UINT( args[2] ), *settings );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 2..4 arguments but was given %d", argc );
			break;
	}
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, Window_Free, object );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

void Init_Window( void )
{
	globalWindowClass = rb_define_class_under( GetNamespace(), "Window", rb_cObject );
	
	// Class methods
	rb_define_singleton_method( globalWindowClass, "new", FUNCPTR( Window_New ), -1 );
	
	// Instance methods
	rb_define_method( globalWindowClass, "close", FUNCPTR( Window_Close ), 0 );
	rb_define_method( globalWindowClass, "create", FUNCPTR( Window_Create ), -1 );
	rb_define_method( globalWindowClass, "display", FUNCPTR( Window_Display ), 0 );
	rb_define_method( globalWindowClass, "enableKeyRepeat", FUNCPTR( Window_EnableKeyRepeat ), 1 );
	rb_define_method( globalWindowClass, "getEvent", FUNCPTR( Window_GetEvent ), 0 );
	rb_define_method( globalWindowClass, "getFrameTime", FUNCPTR( Window_GetFrameTime ), 0 );
	rb_define_method( globalWindowClass, "getHeight", FUNCPTR( Window_GetHeight ), 0 );
	rb_define_method( globalWindowClass, "getInput", FUNCPTR( Window_GetInput ), 0 );
	rb_define_method( globalWindowClass, "getSettings", FUNCPTR( Window_GetSettings ), 0 );
	rb_define_method( globalWindowClass, "getWidth", FUNCPTR( Window_GetWidth ), 0 );
	rb_define_method( globalWindowClass, "isOpened", FUNCPTR( Window_IsOpened ), 0 );
	rb_define_method( globalWindowClass, "setActive", FUNCPTR( Window_SetActive ), 1 );
	rb_define_method( globalWindowClass, "setCursorPosition", FUNCPTR( Window_SetCursorPosition ), 2 );
	rb_define_method( globalWindowClass, "cursorPosition=", FUNCPTR( Window_SetCursorPosition2 ), 2 );
	rb_define_method( globalWindowClass, "setFramerateLimit", FUNCPTR( Window_SetFramerateLimit ), 1 );
	rb_define_method( globalWindowClass, "setIcon", FUNCPTR( Window_SetIcon ), 3 );
	rb_define_method( globalWindowClass, "setJoystickThreshold", FUNCPTR( Window_SetJoystickThreshold ), 1 );
	rb_define_method( globalWindowClass, "setPosition", FUNCPTR( Window_SetPosition ), 2 );
	rb_define_method( globalWindowClass, "position=", FUNCPTR( Window_SetPosition2 ), 2 );
	rb_define_method( globalWindowClass, "setSize", FUNCPTR( Window_SetSize ), 2 );
	rb_define_method( globalWindowClass, "size=", FUNCPTR( Window_SetSize2 ), 2 );
	rb_define_method( globalWindowClass, "setTitle", FUNCPTR( Window_SetTitle ), 1 );
	rb_define_method( globalWindowClass, "show", FUNCPTR( Window_Show ), 1 );
	rb_define_method( globalWindowClass, "showMouseCursor", FUNCPTR( Window_ShowMouseCursor ), 1 );
	rb_define_method( globalWindowClass, "useVerticalSync", FUNCPTR( Window_UseVerticalSync ), 1 );
	rb_define_method( globalWindowClass, "waitEvent", FUNCPTR( Window_WaitEvent ), 0 );
	
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
	
	rb_define_alias( globalWindowClass, "useVerticalSync=", "useVerticalSync" );
	rb_define_alias( globalWindowClass, "use_vertical_sync", "useVerticalSync" );
	rb_define_alias( globalWindowClass, "use_vertical_sync=", "useVerticalSync" );
	
	rb_define_alias( globalWindowClass, "wait_event", "waitEvent" );
}

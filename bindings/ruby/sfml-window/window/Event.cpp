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
 
#include "Event.hpp"
#include "main.hpp"
#include <SFML/Window/Event.hpp>

VALUE globalEventClass;

/* Joystick buttons events parameters (JoyButtonPressed, JoyButtonReleased). */
VALUE globalJoyButtonEventClass;
/* Joystick axis move event parameters (JoyMoved). */
VALUE globalJoyMoveEventClass;
/* Keyboard event parameters (KeyPressed, KeyReleased). */
VALUE globalKeyEventClass;
/* Mouse buttons events parameters (MouseButtonPressed, MouseButtonReleased). */
VALUE globalMouseButtonEventClass;
/* Mouse move event parameters (MouseMoved). */
VALUE globalMouseMoveEventClass;
/* Mouse wheel events parameters (MouseWheelMoved). */
VALUE globalMouseWheelEventClass;
/* Size events parameters (Resized). */
VALUE globalSizeEventClass;
/* Text event parameters (TextEntered). */
VALUE globalTextEventClass;

/* Free a heap allocated object 
 * Not accessible trough ruby directly!
 */
static void Event_Free( sf::Event *anObject )
{
	delete anObject;
}

#define AXIS2NUM( x ) INT2NUM( static_cast< int > ( x ) )
#define NUM2AXIS( x ) static_cast< sf::Joy::Axis >( NUM2INT( x ) )

#define KEY2NUM( x ) INT2NUM( static_cast< int > ( x ) )
#define NUM2KEY( x ) static_cast< sf::Key::Code >( NUM2INT( x ) )

#define MOUSE2NUM( x ) INT2NUM( static_cast< int > ( x ) )
#define NUM2MOUSE( x ) static_cast< sf::Mouse::Button >( NUM2INT( x ) )

#define EVENT_TYPE_ACCESSORS( a, b, conv1 )			\
{								\
	sf::Event:: a##Event * object = NULL;			\
	Data_Get_Struct( self, sf::Event:: a##Event, object );	\
	return conv1 ( object-> b );				\
}

#define EVENT_TYPE_BOOL_ACCESSORS( a, b )			\
{								\
	sf::Event:: a##Event * object = NULL;			\
	Data_Get_Struct( self, sf::Event:: a##Event, object );	\
	if( object-> b == true )				\
		return Qtrue;					\
	else							\
		return Qfalse;					\
}

/* Index of the joystick (0 or 1). */
static VALUE JoyButtonEvent_GetJoystickId( VALUE self )
EVENT_TYPE_ACCESSORS( JoyButton, JoystickId, INT2NUM )
/* Index of the button that has been pressed. */
static VALUE JoyButtonEvent_GetButton( VALUE self )
EVENT_TYPE_ACCESSORS( JoyButton, Button, INT2NUM )

/* Index of the joystick (0 or 1). */
static VALUE JoyMoveEvent_GetJoystickId( VALUE self )
EVENT_TYPE_ACCESSORS( JoyMove, JoystickId, INT2NUM )
/* Axis on which the joystick moved. */
static VALUE JoyMoveEvent_GetAxis( VALUE self )
EVENT_TYPE_ACCESSORS( JoyMove, Axis, AXIS2NUM )
/* New position on the axis (in range [-100, 100]). */
static VALUE JoyMoveEvent_GetPosition( VALUE self )
EVENT_TYPE_ACCESSORS( JoyMove, Position, rb_float_new )

/* Code of the key that has been pressed. */
static VALUE KeyEvent_GetCode( VALUE self )
EVENT_TYPE_ACCESSORS( Key, Code, KEY2NUM )
/* Is the Alt key pressed? */
static VALUE KeyEvent_GetAlt( VALUE self )
EVENT_TYPE_BOOL_ACCESSORS( Key, Alt )
/* Is the Control key pressed? */
static VALUE KeyEvent_GetControl( VALUE self )
EVENT_TYPE_BOOL_ACCESSORS( Key, Control )
/* Is the Shift key pressed? */
static VALUE KeyEvent_GetShift( VALUE self )
EVENT_TYPE_BOOL_ACCESSORS( Key, Shift )

/* Code of the button that has been pressed. */
static VALUE MouseButtonEvent_GetButton( VALUE self )
EVENT_TYPE_ACCESSORS( MouseButton, Button, MOUSE2NUM )
/* X position of the mouse pointer, relative to the left of the owner window */
static VALUE MouseButtonEvent_GetX( VALUE self )
EVENT_TYPE_ACCESSORS( MouseButton, X, INT2NUM )
/* Y position of the mouse pointer, relative to the top of the owner window */
static VALUE MouseButtonEvent_GetY( VALUE self )
EVENT_TYPE_ACCESSORS( MouseButton, Y, INT2NUM )

/* X position of the mouse pointer, relative to the left of the owner window */
static VALUE MouseMoveEvent_GetX( VALUE self )
EVENT_TYPE_ACCESSORS( MouseMove, X, INT2NUM )
/* Y position of the mouse pointer, relative to the top of the owner window */
static VALUE MouseMoveEvent_GetY( VALUE self )
EVENT_TYPE_ACCESSORS( MouseMove, Y, INT2NUM )

/* Number of ticks the wheel has moved (positive is up, negative is down). */
static VALUE MouseWheelEvent_GetDelta( VALUE self )
EVENT_TYPE_ACCESSORS( MouseWheel, Delta, INT2NUM )
/* X position of the mouse pointer, relative to the left of the owner window */
static VALUE MouseWheelEvent_GetX( VALUE self )
EVENT_TYPE_ACCESSORS( MouseWheel, X, INT2NUM )
/* Y position of the mouse pointer, relative to the top of the owner window */
static VALUE MouseWheelEvent_GetY( VALUE self )
EVENT_TYPE_ACCESSORS( MouseWheel, Y, INT2NUM )

/* New width, in pixels. */
static VALUE SizeEvent_GetWidth( VALUE self )
EVENT_TYPE_ACCESSORS( Size, Width, INT2NUM )
/* New height, in pixels. */
static VALUE SizeEvent_GetHeight( VALUE self )
EVENT_TYPE_ACCESSORS( Size, Height, INT2NUM )

/* UTF-32 unicode value of the character. */
static VALUE TextEvent_GetUnicode( VALUE self )
EVENT_TYPE_ACCESSORS( Text, Unicode, INT2NUM )

/* call-seq:
 *   Event.new(type)	-> event
 *
 * You should never call this function directly. You should only aquire event's trough
 * SFML::Window#getEvent or SFML::Window#waitEvent, if you need to pass data to a method
 * that takes an event instance then make a proxy instance to simulate an event.
 * NOTE: Using this method works but it will act constant as you can't access any values.
 */
static VALUE Event_Initialize( VALUE self, VALUE aType )
{
	sf::Event * object = NULL;
	Data_Get_Struct( self, sf::Event, object );
	
	int typeNum = FIX2INT( aType );
	if(typeNum >= 0 && typeNum < sf::Event::Count)
	{
		rb_iv_set( self, "@type", aType );
		object->Type = static_cast< sf::Event::EventType >( typeNum );
	}
	else
	{
		rb_raise( rb_eTypeError, "expected Fixnum in range of 0...SFML::Event::Count" );
	}
	
	bool noSpecialType = false;
	VALUE eventType;
	const char * name = NULL;
	switch( object->Type )
	{
		case sf::Event::JoyButtonPressed:
		case sf::Event::JoyButtonReleased:
			eventType = Data_Wrap_Struct( globalJoyButtonEventClass, 0, 0, &object->JoyButton );
			name = "@joyButton";
			break;
		case sf::Event::JoyMoved:
			eventType = Data_Wrap_Struct( globalJoyMoveEventClass, 0, 0, &object->JoyMove );
			name = "@joyMove";
			break;
		case sf::Event::KeyPressed:
		case sf::Event::KeyReleased:
			eventType = Data_Wrap_Struct( globalKeyEventClass, 0, 0, &object->Key );
			name = "@key";
			break;
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
			eventType = Data_Wrap_Struct( globalMouseButtonEventClass, 0, 0, &object->MouseButton );
			name = "@mouseButton";
			break;
		case sf::Event::MouseMoved:
			eventType = Data_Wrap_Struct( globalMouseMoveEventClass, 0, 0, &object->MouseMove );
			name = "@mouseMove";
			break;
		case sf::Event::MouseWheelMoved:
			eventType = Data_Wrap_Struct( globalMouseWheelEventClass, 0, 0, &object->MouseWheel );
			name = "@mouseWheel";
			break;
		case sf::Event::Resized:
			eventType = Data_Wrap_Struct( globalSizeEventClass, 0, 0, &object->Size );
			name = "@resized";
			break;
		case sf::Event::TextEntered:
			eventType = Data_Wrap_Struct( globalTextEventClass, 0, 0, &object->Text );
			name = "@text";
			break;
		default:
			noSpecialType = true;
	};
	
	if( noSpecialType == false )
	{
		rb_obj_call_init( eventType, 0, 0 );
		rb_iv_set( eventType, "@internal__parent_ref", self );
		rb_iv_set( self, name, eventType );
	}
	return self;
}

static VALUE Event_InitializeCopy( VALUE self, VALUE aSource )
{
	sf::Event *object = NULL;
	Data_Get_Struct( self, sf::Event, object );
	sf::Event *source = NULL;
	Data_Get_Struct( aSource, sf::Event, source );
	*object = *source;
	return Event_Initialize( self, INT2FIX( object->Type ) );
}

static VALUE Event_Alloc( VALUE aKlass )
{
	sf::Event *object = new sf::Event();
	return Data_Wrap_Struct( aKlass, 0, Event_Free, object );
}

void Init_Event( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* SFML::Event holds all the informations about a system event that just happened.
 *
 * Events are retrieved using the SFML::Window#GetEvent function.
 *
 * A SFML::Event instance contains the type of the event (mouse moved, key pressed, window closed, ...)
 * as well as the details about this particular event. Please note that the event parameters are 
 * defined in a union, which means that only the member matching the type of the event will be properly
 * filled; all other members will have undefined values and must not be read if the type of the event 
 * doesn't match. For example, if you received a KeyPressed event, then you must read the event.Key
 *  member, all other members such as event.MouseMove or event.Text will have undefined values.
 *
 * The ruby version differs from C++ in that the parameters are still stored in a union but that
 * the values can be directly accessed from the event object. If you try to access any data which
 * would be considered undefined then SFML::SomeKindOfException will be thrown.
 *
 * Usage example:
 *
 *  while event = window.getEvent()
 *
 *     # Request for closing the window
 *     if event.type == SFML::Event::Closed
 *       window.close
 *
 *     # The escape key was pressed
 *     if ( event.type == sf::Event::KeyPressed ) && ( event.code == SFML::Key::Escape )
 *       window.close
 *
 *     # The window was resized
 *     if event.type == SFML::Event::Resized
 *       DoSomethingWithTheNewSize(event.size);
 *
 *     # etc ...
 *   end
 */
	globalEventClass 			= rb_define_class_under( sfml, "Event", rb_cObject );
	globalJoyButtonEventClass 	= rb_define_class_under( globalEventClass, "JoyButton", rb_cObject );
	globalJoyMoveEventClass		= rb_define_class_under( globalEventClass, "JoyMove", rb_cObject );
	globalKeyEventClass 		= rb_define_class_under( globalEventClass, "Key", rb_cObject );
	globalMouseButtonEventClass = rb_define_class_under( globalEventClass, "MouseButton", rb_cObject );
	globalMouseMoveEventClass 	= rb_define_class_under( globalEventClass, "MouseMove", rb_cObject );
	globalMouseWheelEventClass 	= rb_define_class_under( globalEventClass, "MouseWheel", rb_cObject );
	globalSizeEventClass 		= rb_define_class_under( globalEventClass, "Size", rb_cObject );
	globalTextEventClass 		= rb_define_class_under( globalEventClass, "Text", rb_cObject );
	
	rb_define_const( globalEventClass, "Closed", INT2NUM( sf::Event::Closed ) );
	rb_define_const( globalEventClass, "Resized", INT2NUM( sf::Event::Resized ) );
	rb_define_const( globalEventClass, "LostFocus", INT2NUM( sf::Event::LostFocus ) );
	rb_define_const( globalEventClass, "GainedFocus", INT2NUM( sf::Event::GainedFocus ) );
	rb_define_const( globalEventClass, "TextEntered", INT2NUM( sf::Event::TextEntered ) );
	rb_define_const( globalEventClass, "KeyPressed", INT2NUM( sf::Event::KeyPressed ) );
	rb_define_const( globalEventClass, "KeyReleased", INT2NUM( sf::Event::KeyReleased ) );
	rb_define_const( globalEventClass, "MouseWheelMoved", INT2NUM( sf::Event::MouseWheelMoved ) );
	rb_define_const( globalEventClass, "MouseButtonPressed", INT2NUM( sf::Event::MouseButtonPressed ) );
	rb_define_const( globalEventClass, "MouseButtonReleased", INT2NUM( sf::Event::MouseButtonReleased ) );
	rb_define_const( globalEventClass, "MouseMoved", INT2NUM( sf::Event::MouseMoved ) );
	rb_define_const( globalEventClass, "MouseEntered", INT2NUM( sf::Event::MouseEntered ) );
	rb_define_const( globalEventClass, "MouseLeft", INT2NUM( sf::Event::MouseLeft ) );
	rb_define_const( globalEventClass, "JoyButtonPressed", INT2NUM( sf::Event::JoyButtonPressed ) );
	rb_define_const( globalEventClass, "JoyButtonReleased", INT2NUM( sf::Event::JoyButtonReleased ) );
	rb_define_const( globalEventClass, "JoyMoved", INT2NUM( sf::Event::JoyMoved ) );
	rb_define_const( globalEventClass, "Count", INT2NUM( sf::Event::Count ) );
	
	// Class methods
	//rb_define_singleton_method( globalEventClass, "new", Event_New, -1 );
	rb_define_alloc_func( globalEventClass, Event_Alloc );
	
	// Instance methods
	rb_define_method( globalEventClass, "initialize", Event_Initialize, 1 );
	rb_define_method( globalEventClass, "initialize_copy", Event_InitializeCopy, 1 );
	
	rb_define_attr( globalEventClass, "type", 1, 0 );
	rb_define_attr( globalEventClass, "joyButton", 1, 0 );
	rb_define_attr( globalEventClass, "joyMove", 1, 0 );
	rb_define_attr( globalEventClass, "key", 1, 0 );
	rb_define_attr( globalEventClass, "mouseButton", 1, 0 );
	rb_define_attr( globalEventClass, "mouseMove", 1, 0 );
	rb_define_attr( globalEventClass, "mouseWheel", 1, 0 );
	rb_define_attr( globalEventClass, "size", 1, 0 );
	rb_define_attr( globalEventClass, "text", 1, 0 );
	
	// JoyButton methods
	rb_define_method( globalJoyButtonEventClass, "joystickId", JoyButtonEvent_GetJoystickId, 0 );
	rb_define_method( globalJoyButtonEventClass, "button", JoyButtonEvent_GetButton, 0 );
	
	// JoyMove methods
	rb_define_method( globalJoyMoveEventClass, "joystickId", JoyMoveEvent_GetJoystickId, 0 );
	rb_define_method( globalJoyMoveEventClass, "axis", JoyMoveEvent_GetAxis, 0 );
	rb_define_method( globalJoyMoveEventClass, "position", JoyMoveEvent_GetPosition, 0 );
	
	// Key methods
	rb_define_method( globalKeyEventClass, "code", KeyEvent_GetCode, 0 );
	rb_define_method( globalKeyEventClass, "alt", KeyEvent_GetAlt, 0 );
	rb_define_method( globalKeyEventClass, "control", KeyEvent_GetControl, 0 );
	rb_define_method( globalKeyEventClass, "shift", KeyEvent_GetShift, 0 );
	
	// MouseButton methods
	rb_define_method( globalMouseButtonEventClass, "button", MouseButtonEvent_GetButton, 0 );
	rb_define_method( globalMouseButtonEventClass, "x", MouseButtonEvent_GetX, 0 );
	rb_define_method( globalMouseButtonEventClass, "y", MouseButtonEvent_GetY, 0 );
	
	// MouseMove methods
	rb_define_method( globalMouseMoveEventClass, "x", MouseMoveEvent_GetX, 0 );
	rb_define_method( globalMouseMoveEventClass, "y", MouseMoveEvent_GetY, 0 );
	
	// MouseWheel methods
	rb_define_method( globalMouseWheelEventClass, "delta", MouseWheelEvent_GetDelta, 0 );
	rb_define_method( globalMouseWheelEventClass, "x", MouseWheelEvent_GetX, 0 );
	rb_define_method( globalMouseWheelEventClass, "y", MouseWheelEvent_GetY, 0 );
	
	// Size methods
	rb_define_method( globalSizeEventClass, "width", SizeEvent_GetWidth, 0 );
	rb_define_method( globalSizeEventClass, "height", SizeEvent_GetWidth, 0 );
	
	// Text methods
	rb_define_method( globalTextEventClass, "unicode", TextEvent_GetUnicode, 0 );
}

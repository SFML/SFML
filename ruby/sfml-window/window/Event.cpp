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

static VALUE Event_Initialize( VALUE self )
{
	sf::Event * object = NULL;
	Data_Get_Struct( self, sf::Event, object );
	
	VALUE joyButton 	= Data_Wrap_Struct( globalJoyButtonEventClass, 0, 0, &object->JoyButton );
	VALUE joyMove 		= Data_Wrap_Struct( globalJoyMoveEventClass, 0, 0, &object->JoyMove );
	VALUE key		= Data_Wrap_Struct( globalKeyEventClass, 0, 0, &object->Key );
	VALUE mouseButton 	= Data_Wrap_Struct( globalMouseButtonEventClass, 0, 0, &object->MouseButton );
	VALUE mouseMove 	= Data_Wrap_Struct( globalMouseMoveEventClass, 0, 0, &object->MouseMove );
	VALUE mouseWheel 	= Data_Wrap_Struct( globalMouseWheelEventClass, 0, 0, &object->MouseWheel );
	VALUE size		= Data_Wrap_Struct( globalSizeEventClass, 0, 0, &object->Size );
	VALUE text		= Data_Wrap_Struct( globalTextEventClass, 0, 0, &object->Text );
	
	rb_obj_call_init( joyButton, 0, 0 );
	rb_obj_call_init( joyMove, 0, 0 );
	rb_obj_call_init( key, 0, 0 );
	rb_obj_call_init( mouseButton, 0, 0 );
	rb_obj_call_init( mouseMove, 0, 0 );
	rb_obj_call_init( mouseWheel, 0, 0 );
	rb_obj_call_init( size, 0, 0 );
	rb_obj_call_init( text, 0, 0 );
	
	rb_iv_set( joyButton, "@internal__parent_ref", self );
	rb_iv_set( joyMove, "@internal__parent_ref", self );
	rb_iv_set( key, "@internal__parent_ref", self );
	rb_iv_set( mouseButton, "@internal__parent_ref", self );
	rb_iv_set( mouseMove, "@internal__parent_ref", self );
	rb_iv_set( mouseWheel, "@internal__parent_ref", self );
	rb_iv_set( size, "@internal__parent_ref", self );
	rb_iv_set( text, "@internal__parent_ref", self );
}

/* call-seq:
 *   Event.new()	-> event
 *
 * The constructor creates a new event.
 */
static VALUE Event_New( VALUE aKlass )
{
	sf::Event *object = new sf::Event();
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, Event_Free, object );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

void Init_Event( void )
{
	globalEventClass = rb_define_class_under( GetNamespace(), "Event", rb_cObject );
	globalJoyButtonEventClass 	= rb_define_class_under( globalEventClass, "JoyButton", rb_cObject );
	globalJoyMoveEventClass		= rb_define_class_under( globalEventClass, "JoyMove", rb_cObject );
	globalKeyEventClass 		= rb_define_class_under( globalEventClass, "Key", rb_cObject );
	globalMouseButtonEventClass 	= rb_define_class_under( globalEventClass, "MouseButton", rb_cObject );
	globalMouseMoveEventClass 	= rb_define_class_under( globalEventClass, "MouseMove", rb_cObject );
	globalMouseWheelEventClass 	= rb_define_class_under( globalEventClass, "MouseWheel", rb_cObject );
	globalSizeEventClass 	= rb_define_class_under( globalEventClass, "Size", rb_cObject );
	globalTextEventClass 	= rb_define_class_under( globalEventClass, "Text", rb_cObject );
	
	// Class methods
	rb_define_singleton_method( globalEventClass, "new", FUNCPTR( Event_New ), 0 );
}

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
 
#include "Window.hpp"
#include "main.hpp"
#include <SFML/Window/Window.hpp>
#include <iostream>

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

#define VALIDATE_CLASS( variable, type, name ) \
if( CLASS_OF( variable ) != type ) \
{ \
	rb_raise( rb_eTypeError, name#" argument must be instance of %s", STR2CSTR( rb_funcall( type, rb_intern( "to_s" ), 0 ) ) ); \
}

/* Free a heap allocated object 
 * Not accessible trough ruby directly!
 */
static void Window_Free( sf::Window *anObject )
{
	delete anObject;
}

static VALUE Window_New( int argc, VALUE *args, VALUE aKlass )
{
	sf::Window *object = NULL;
	sf::VideoMode *mode = NULL;
	sf::ContextSettings *settings = NULL;
	switch( argc )
	{
		case 2:
			VALIDATE_CLASS( args[0], globalVideoModeClass, first );
			VALIDATE_CLASS( args[1], rb_cString, second );
			Data_Get_Struct( args[0], sf::VideoMode, mode );
			object = new sf::Window( mode , STR2CSTR( args[1] ) );
			break;
		case 3:
			VALIDATE_CLASS( args[0], globalVideoModeClass, first );
			VALIDATE_CLASS( args[1], rb_cString, second );
			VALIDATE_CLASS( args[2], rb_cFixnum, second );
			Data_Get_Struct( args[0], sf::VideoMode, mode );
			object = new sf::VideoMode( mode, STR2CSTR( args[1] ), FIX2UINT( args[2] ) );
			break;
		case 4:
			VALIDATE_CLASS( args[0], globalVideoModeClass, first );
			VALIDATE_CLASS( args[1], rb_cString, second );
			VALIDATE_CLASS( args[2], rb_cFixnum, second );
			VALIDATE_CLASS( args[3], globalContextSettingsClass, fourth );
			Data_Get_Struct( args[0], sf::VideoMode, mode );
			Data_Get_Struct( args[3], sf::ContextSettings, settings );
			object = new sf::VideoMode( mode, STR2CSTR( args[1] ), FIX2UINT( args[2] ), *settings );
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
	
	// Aliases
}

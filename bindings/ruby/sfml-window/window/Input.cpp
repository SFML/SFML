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
 
#include "Input.hpp"
#include "main.hpp"
#include <SFML/Window/Input.hpp>

VALUE globalInputClass;

/* External classes */
extern VALUE globalNonCopyableModule;

/* Free a heap allocated object 
 * Not accessible trough ruby directly!
 */
static void Input_Free( sf::Event *anObject )
{
	delete anObject;
}

/* call-seq:
 *   input.isKeyDown( keycode )	-> true or false
 *
 * Get the current state of a key (pressed or released).
 */
static VALUE Input_IsKeyDown( VALUE self, VALUE aKeyCode )
{
	sf::Input *object = NULL;
	Data_Get_Struct( self, sf::Input, object );
	sf::Key::Code rawCode = static_cast< sf::Key::Code > ( NUM2INT( aKeyCode ) );
	if( object->IsKeyDown( rawCode ) == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

/* call-seq:
 *   input.isMouseButtonDown( keycode )	-> true or false
 *
 * Get the current state of a mouse button (pressed or released).
 */
static VALUE Input_IsMouseButtonDown( VALUE self, VALUE aMouseButton )
{
	sf::Input *object = NULL;
	Data_Get_Struct( self, sf::Input, object );
	sf::Mouse::Button rawButton = static_cast< sf::Mouse::Button > ( NUM2INT( aMouseButton ) );
	if( object->IsMouseButtonDown( rawButton ) == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

/* call-seq:
 *   input.isJoystickButtonDown( joystick, button )	-> true or false
 *
 * Get the current state of a joystick button (pressed or released).
 */
static VALUE Input_IsJoystickButtonDown( VALUE self, VALUE aJoystick, VALUE aButton )
{
	sf::Input *object = NULL;
	Data_Get_Struct( self, sf::Input, object );
	unsigned int rawJoystick =  NUM2UINT( aJoystick );
	unsigned int rawButton =  NUM2UINT( aButton );
	if( object->IsJoystickButtonDown( aJoystick, rawButton ) == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

/* call-seq:
 *   input.getMouseX()	-> fixnum
 *
 * The returned position is relative to the left border of the owner window.
 */
static VALUE Input_GetMouseX( VALUE self, VALUE aMouseButton )
{
	sf::Input *object = NULL;
	Data_Get_Struct( self, sf::Input, object );
	return INT2FIX( object->GetMouseX() );
}

/* call-seq:
 *   input.getMouseY()	-> fixnum
 *
 * The returned position is relative to the top border of the owner window.
 */
static VALUE Input_GetMouseY( VALUE self, VALUE aMouseButton )
{
	sf::Input *object = NULL;
	Data_Get_Struct( self, sf::Input, object );
	return INT2FIX( object->GetMouseY() );
}

/* call-seq:
 *   input.getJoystickAxis( joystick, axis )	-> true or false
 *
 * The returned position is in the range [-100, 100], except the POV which is an angle and is thus defined in [0, 360].
 */
static VALUE Input_GetJoystickAxis( VALUE self, VALUE aJoystick, VALUE anAxis )
{
	sf::Input *object = NULL;
	Data_Get_Struct( self, sf::Input, object );
	unsigned int rawJoystick =  NUM2UINT( aJoystick );
	sf::Joy::Axis rawAxis =  static_cast< sf::Joy::Axis >( NUM2INT( anAxis ) );
	return rb_float_new( object->GetJoystickAxis( rawJoystick, rawAxis ) );
}

static VALUE Input_Alloc( VALUE aKlass )
{
	sf::Input *object = new sf::Input();
	return Data_Wrap_Struct( aKlass, 0, Input_Free, object );
}

void Init_Input( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* The SFML::Input class provides a way to access the state of keys, mouse buttons, mouse position, joystick 
 * buttons and jostick axis.
 *
 * SFML::Input provides the same informations as the event system, but these informations can be accessed at any time, 
 * which is more convenient in many situations.
 *
 * For example, to move an entity you can decide to catch the SFML::Event::KeyPressed event on arrow keys. But if you 
 * do so, you will only receive one event when the key gets pressed (or repeated events if you activated this feature), 
 * thus the entity will not move smoothly. The best solution here is to use SFML::Input#isKeyDown so that you can 
 * update your entity's position at every iteration of your game loop, not only when you catch a KeyPressed event.
 *
 * Note that instances of SFML::Input cannot be created directly, they must be retrieved from a window (SFML::Window)
 * with the SFML::Window#input method.
 *
 * Usage example:
 * 
 *   # Retrieve the input object attached to our window
 *   input = window.input
 *  
 *   # Move an entity according to the current keys state
 *   offset = 5.0 * window.frameTime # 5 pixels/sec
 *   entity.move( -offset, 0 ) if input.keyDown?( SFML::Key::Left )
 *   entity.move( offset, 0 )  if input.keyDown?( SFML::Key::Right )
 *   entity.move( 0, -offset ) if input.keyDown?( SFML::Key::Up )
 *   entity.move( 0,  offset ) if input.keyDown?( SFML::Key::Down )
 */
	globalInputClass = rb_define_class_under( sfml, "Input", rb_cObject );
	rb_include_module( globalInputClass, globalNonCopyableModule );
	
	// Class methods
	//rb_define_singleton_method( globalInputClass, "new", Input_New, -1 );
	rb_define_alloc_func( globalInputClass, Input_Alloc );
	
	// Instance methods
	rb_define_method( globalInputClass, "isKeyDown", Input_IsKeyDown, 1 );
	rb_define_method( globalInputClass, "isMouseButtonDown", Input_IsMouseButtonDown, 1 );
	rb_define_method( globalInputClass, "isJoystickButtonDown", Input_IsJoystickButtonDown, 2 );
	rb_define_method( globalInputClass, "getMouseX", Input_GetMouseX, 0 );
	rb_define_method( globalInputClass, "getMouseY", Input_GetMouseY, 0 );
	rb_define_method( globalInputClass, "getJoystickAxis", Input_GetJoystickAxis, 2 );
	
	// Aliases
	rb_define_alias( globalInputClass, "key_down?", "isKeyDown");
	rb_define_alias( globalInputClass, "keyDown?", "isKeyDown");
	
	rb_define_alias( globalInputClass, "mouse_button_down?", "isMouseButtonDown");
	rb_define_alias( globalInputClass, "mouseButtonDown?", "isMouseButtonDown");
	
	rb_define_alias( globalInputClass, "joystick_button_down?", "isJoystickButtonDown");
	rb_define_alias( globalInputClass, "joystickButtonDown?", "isJoystickButtonDown");
	
	rb_define_alias( globalInputClass, "mouseX", "getMouseX");
	rb_define_alias( globalInputClass, "mouse_x", "getMouseX");
	
	rb_define_alias( globalInputClass, "mouseY", "getMouseY");
	rb_define_alias( globalInputClass, "mouse_y", "getMouseY");
	
	rb_define_alias( globalInputClass, "joystickAxis", "getJoystickAxis");
	rb_define_alias( globalInputClass, "joystick_axis", "getJoystickAxis");
}

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

/* SFML::Input provides a way to access the state of keys, mouse buttons, 
 * mouse position, joystick buttons and jostick axis.
 *
 * SFML::Input provides the same informations as the event system, but these 
 * informations can be accessed at any time, which is more convenient in many 
 * situations.
 *
 * For example, to move an entity you can decide to catch the 
 * SFML::Event::KeyPressed event on arrow keys. But if you do so, you will only 
 * receive one event when the key gets pressed (or repeated events if you 
 * activated this feature), thus the entity will not move smoothly. The best 
 * solution here is to use sf::Input::IsKeyDown so that you can update your 
 * entity's position at every iteration of your game loop, not only when you 
 * catch a KeyPressed event.
 *
 * Note that instances of sf::Input cannot be created directly, they must be 
 * retrieved from a window (sf::Window) with its GetInput() function.
 *
 * Usage example:
 * 
 *   # Retrieve the input object attached to our window
 *   input = window.getInput();
 *   
 *   # Move an entity according to the current keys state
 *   offset = 5.0 * window.GetFrameTime(); # 5 pixels/sec
 *   entity.move( -offset, 0 ) if input.isKeyDown(SFML::Key::Left) == true
 *   entity.move( offset, 0 )  if input.isKeyDown(SFML::Key::Right) == true 
 *   entity.move( 0, -offset ) if input.isKeyDown(SFML::Key::Up) == true
 *   entity.move( 0,  offset ) if input.isKeyDown(SFML::Key::Down) == true
 */
VALUE globalInputClass;

/* Free a heap allocated object 
 * Not accessible trough ruby directly!
 */
static void Input_Free( sf::Event *anObject )
{
	delete anObject;
}

/* call-seq:
 *   Input.new()	-> input
 *
 * The constructor creates a new input.
 */
static VALUE Input_New( int argc, VALUE * args, VALUE aKlass )
{
	sf::Input *object = new sf::Input();
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, Input_Free, object );
	rb_obj_call_init( rbData, argc, args );
	return rbData;
}

void Init_Input( void )
{
	globalInputClass = rb_define_class_under( GetNamespace(), "Input", rb_cObject );
	
	// Class methods
	rb_define_singleton_method( globalInputClass, "new", FUNCPTR( Input_New ), -1 );
	
	// Instance methods
}

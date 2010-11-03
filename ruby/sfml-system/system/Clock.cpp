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
 
#include "Clock.hpp"
#include "System.hpp"
#include <SFML/System/Clock.hpp>

/* Utility class for manipulating time. */
VALUE globalClockClass;

/* Free a heap allocated object 
 * Not accessible trough ruby directly!
 */
static void Clock_Free( sf::Clock *anObject )
{
	delete anObject;
}

/* call-seq:
 *   clock.getElapsedTime()		-> Float
 *
 * This function returns the time elapsed since the last call to Reset() 
 * (or the construction of the instance if Reset() has not been called) in seconds.
 */
static VALUE Clock_GetElapsedTime( VALUE self )
{
	sf::Clock *object = NULL;
	Data_Get_Struct( self, sf::Clock, object );
	return rb_float_new( object->GetElapsedTime() );
}

/* call-seq:
 *   clock.reset()		-> nil
 * 
 * This function puts the time counter back to zero.
 */
static VALUE Clock_Reset( VALUE self )
{
	sf::Clock *object = NULL;
	Data_Get_Struct( self, sf::Clock, object );
	object->Reset();
	return Qnil;
}

/* call-seq:
 *   Clock.new()		-> clock
 *
 * The clock starts automatically after being constructed.
 */
static VALUE Clock_New( VALUE aKlass )
{
	sf::Clock *object = new sf::Clock();
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, Clock_Free, object );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

void Init_Clock( void )
{
	globalClockClass = rb_define_class_under( GetNamespace(), "Clock", rb_cObject );
	
	// Class methods
	rb_define_singleton_method( globalClockClass, "new", FUNCPTR( Clock_New ), 0 );
	
	// Instance methods
	rb_define_method( globalClockClass, "getElapsedTime", FUNCPTR( Clock_GetElapsedTime ), 0 );
	rb_define_method( globalClockClass, "reset", FUNCPTR( Clock_Reset ), 0 );
	
	// Aliases
	rb_define_alias( globalClockClass, "elapsedTime", "getElapsedTime" );
	rb_define_alias( globalClockClass, "elapsed_time", "getElapsedTime" );
}

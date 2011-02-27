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
 
#include "Context.hpp"
#include "main.hpp"
#include <SFML/Window/Context.hpp>

VALUE globalContextClass;

/* External classes */
extern VALUE globalNonCopyableModule;

/* Free a heap allocated object 
 * Not accessible trough ruby directly!
 */
static void Context_Free( sf::Context *anObject )
{
	delete anObject;
}

/* call-seq:
 *   context.SetActive(bool)	-> nil
 *
 * Activate or deactivate explicitely the context.
 */
static VALUE Context_SetActive( VALUE self, VALUE anArgument )
{
	sf::Context *object = NULL;
	Data_Get_Struct( self, sf::Context, object );
	switch( anArgument )
	{
		case Qtrue:
			object->SetActive( true );
			break;
		case Qfalse:
			object->SetActive( false );
			break;
		default:
			rb_raise( rb_eTypeError, "expected true or false" );
	}
	return Qnil;
}

/* call-seq:
 *   Context.SetReferenceActive()	-> true or false
 *
 * This function is meant to be called internally; it is used to deactivate the 
 * current context by activating another one (so that we still have an active 
 * context on the current thread).
 */
static VALUE Context_SetReferenceActive( VALUE aKlass )
{
	if( sf::Context::SetReferenceActive() == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

static VALUE Context_Alloc( VALUE aKlass )
{
	sf::Context *object = new sf::Context();
	return Data_Wrap_Struct( aKlass, 0, Context_Free, object );
}

void Init_Context( void )
{

/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* If you need to make OpenGL / graphics calls without having an active window 
 * (like in a thread), you can use an instance of this class to get a valid context.
 *
 * Having a valid context is necessary for *every* OpenGL call, and for most of 
 * the classes from the Graphics package.
 *
 * Note that a context is only active in its current thread, if you create a new
 * thread it will have no valid context by default.
 *
 * To use a sf::Context instance, just construct it and let it live as long as 
 * you need a valid context. No explicit activation is needed, all it has to do 
 * is to exist. Its destructor will take care of deactivating and freeing all 
 * the attached resources.
 */
	globalContextClass = rb_define_class_under( sfml, "Context", rb_cObject );
	rb_include_module( globalContextClass, globalNonCopyableModule );
	
	// Class methods
	//rb_define_singleton_method( globalContextClass, "new", Context_New, 0 );
	rb_define_alloc_func( globalContextClass, Context_Alloc );
	rb_define_singleton_method( globalContextClass, "setReferenceActive", Context_SetReferenceActive, 0 );
	
	// Instance methods
	rb_define_method( globalContextClass, "setActive", Context_SetActive, 1 );
	
	// Aliases
	rb_define_alias( globalContextClass, "active=", "setActive" );
	rb_define_alias( globalContextClass, "set_active", "setActive" );
	
	rb_define_alias( CLASS_OF( globalContextClass ), "set_reference_active", "setReferenceActive" );
}

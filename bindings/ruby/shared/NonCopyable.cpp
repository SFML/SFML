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
 
#include "NonCopyable.hpp"
#include "global.hpp"

VALUE globalNonCopyableModule;

/* call-seq:
 *   non_copyable.clone( source ) 	-> copy
 *   non_copyable.dup( source )		-> copy
 * 
 * Override the copy methods so that we can't copy these instances..
 */
static VALUE NonCopyable_InitializeCopy( VALUE self, VALUE aSource)
{
	rb_raise( rb_eRuntimeError, "The object you tried to copy is NonCopyable" );
	return self;
}

void Init_NonCopyable( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Utility mixin-module that makes any derived class non-copyable.
 * This module makes its instances non-copyable, by explicitely disabling its initialize_copy method.
 *
 * To create a non-copyable class, simply include the SFML::NonCopyable module.
 *
 * Usage example:
 *
 *   class MyNonCopyableClass
 *     include SFML::NonCopyable
 *     ...
 *   end
 
 * Deciding whether the instances of a class can be copied or not is a very important design choice. You are strongly 
 * encouraged to think about it before writing a class, and to use SFML::NonCopyable when necessary to prevent many 
 * potential future errors when using it. This is also a very important indication to users of your class.
 */
	globalNonCopyableModule = rb_define_module_under( sfml, "NonCopyable" );
	
	// Instance methods
	rb_define_method( globalNonCopyableModule, "initialize_copy", NonCopyable_InitializeCopy, 1 );
	
}

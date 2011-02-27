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
 
#ifndef SFML_RUBYEXT_GLOBAL_HEADER_
#define SFML_RUBYEXT_GLOBAL_HEADER_

#include "ruby.h"

#define SFML_DYNAMIC

extern VALUE globalSFMLNamespace;

VALUE RetrieveSFMLClass( const char * aName );

typedef VALUE ( *RubyFunctionPtr )( ... );

#define MAX( x, y ) ( ( x ) < ( y ) ? ( y ) : ( x ) )
#define MIN( x, y ) ( ( x ) > ( y ) ? ( x ) : ( y ) )

#define VALIDATE_CLASS( variable, type, name ) \
if( rb_obj_is_kind_of( variable, type ) != Qtrue ) \
{ \
	rb_raise( rb_eTypeError, "%s argument must be instance of %s", name, rb_string_value_cstr ( &type ) ); \
}

#define rb_define_module_function( klass, name, func, argc, ... ) rb_define_module_function( klass, name, reinterpret_cast< RubyFunctionPtr >( func ), argc, ##__VA_ARGS__ )
#define rb_define_singleton_method( klass, name, func, argc, ... ) rb_define_singleton_method( klass, name, reinterpret_cast< RubyFunctionPtr >( func ), argc, ##__VA_ARGS__ )
#define rb_define_method( klass, name, func, argc, ... ) rb_define_method( klass, name, reinterpret_cast< RubyFunctionPtr >( func ), argc, ##__VA_ARGS__ )

#endif // SFML_RUBYEXT_SYSTEM_MAIN_HEADER_

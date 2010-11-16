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
 
#include "Color.hpp"
#include "main.hpp"
#include <SFML/Graphics/Color.hpp>

VALUE globalColorClass;

/* Internal function
 * Will copy the x and y from aSource to self.
 */
static void Color_internal_CopyFrom( VALUE self, VALUE aSource )
{
	VALUE r = rb_funcall( aSource, rb_intern( "r" ), 0 );
	VALUE g = rb_funcall( aSource, rb_intern( "g" ), 0 );
	VALUE b = rb_funcall( aSource, rb_intern( "b" ), 0 );
	VALUE a = rb_funcall( aSource, rb_intern( "a" ), 0 );
	
	rb_funcall( self, rb_intern( "r=" ), 1, r );
	rb_funcall( self, rb_intern( "g=" ), 1, g ); 
	rb_funcall( self, rb_intern( "b=" ), 1, b ); 
	rb_funcall( self, rb_intern( "a=" ), 1, a );  
}


static VALUE Color_Initialize( int argc, VALUE * args, VALUE self )
{
	rb_iv_set( self, "@r", INT2NUM( 0 ) );
	rb_iv_set( self, "@g", INT2NUM( 0 ) );
	rb_iv_set( self, "@b", INT2NUM( 0 ) );
	rb_iv_set( self, "@a", INT2NUM( 255 ) );
	
	switch( argc )
	{
		case 0:
			// Nothing needs to be done
			break;
		case 1:
			Color_internal_CopyFrom( self, args[0] );
			break;
		case 4:
			VALIDATE_CLASS( args[3], rb_cFixnum, "alpha" );
			rb_iv_set( self, "@a", args[3]);
		case 3:
			VALIDATE_CLASS( args[0], rb_cFixnum, "red" );
			VALIDATE_CLASS( args[1], rb_cFixnum, "green" );
			VALIDATE_CLASS( args[2], rb_cFixnum, "blue" );
			rb_iv_set( self, "@r", args[0]);
			rb_iv_set( self, "@g", args[1]);
			rb_iv_set( self, "@b", args[2]);
			break;
		default:
			rb_raise( rb_eArgError, "Expected 0, 3 or 4 arguments but was given %d", argc );
	}
	return self;
}

void Init_Color( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Utility class for manipulating time.
 *
 * sf::Clock is a lightweight class for measuring time.
 *
 * Its resolution depends on the underlying OS, but you can generally expect a 1 ms resolution.
 */
	globalColorClass = rb_define_class_under( sfml, "Color", rb_cObject );
	
	// Instance methods
	rb_define_method( globalColorClass, "initialize", Color_Initialize, -1 );
}

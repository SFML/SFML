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
 * Forces the argument someValue to be a Color. IF it can convert it then it will.
 * So you can always safely asume that this function returns a Color object.
 * If it fails then an exception will be thrown.
 */
VALUE Color_ForceType( VALUE someValue )
{
	if( rb_obj_is_kind_of( someValue, rb_cArray ) == Qtrue )
	{
		VALUE arg1 = rb_ary_entry( someValue, 0 );
		VALUE arg2 = rb_ary_entry( someValue, 1 );
		VALUE arg3 = rb_ary_entry( someValue, 2 );
		if( FIX2INT( rb_funcall( someValue, rb_intern( "size" ), 0 ) ) == 4 )
		{
			VALUE arg4 = rb_ary_entry( someValue, 3 );
			return rb_funcall( globalColorClass, rb_intern( "new" ), 4, arg1, arg2, arg3, arg4 );
		}
		
		return rb_funcall( globalColorClass, rb_intern( "new" ), 3, arg1, arg2, arg3 );
	}
	else if( rb_obj_is_kind_of( someValue, globalColorClass ) == Qtrue )
	{
		return someValue;
	}
	else
	{
		rb_raise( rb_eRuntimeError, "expected Array or Color" );
	}
}

/* Internal function
 * Will copy the x and y from aSource to self.
 */
static void Color_internal_CopyFrom( VALUE self, VALUE aSource )
{
	VALUE sourceVector = Color_ForceType( aSource );
	VALUE r = rb_funcall( sourceVector, rb_intern( "r" ), 0 );
	VALUE g = rb_funcall( sourceVector, rb_intern( "g" ), 0 );
	VALUE b = rb_funcall( sourceVector, rb_intern( "b" ), 0 );
	VALUE a = rb_funcall( sourceVector, rb_intern( "a" ), 0 );
	
	rb_funcall( self, rb_intern( "r=" ), 1, r );
	rb_funcall( self, rb_intern( "g=" ), 1, g ); 
	rb_funcall( self, rb_intern( "b=" ), 1, b ); 
	rb_funcall( self, rb_intern( "a=" ), 1, a );  
}

/* */
static VALUE Color_Add( VALUE self, VALUE aRightOperand )
{
	VALUE right = Color_ForceType( aRightOperand );
	// Get values
	unsigned int leftR  = FIX2INT( rb_funcall( self, rb_intern( "r" ), 0 ) );
	unsigned int leftG  = FIX2INT( rb_funcall( self, rb_intern( "g" ), 0 ) );
	unsigned int leftB  = FIX2INT( rb_funcall( self, rb_intern( "b" ), 0 ) );
	unsigned int leftA  = FIX2INT( rb_funcall( self, rb_intern( "a" ), 0 ) );
	unsigned int rightR = FIX2INT( rb_funcall( right, rb_intern( "r" ), 0 ) );
	unsigned int rightG = FIX2INT( rb_funcall( right, rb_intern( "g" ), 0 ) );
	unsigned int rightB = FIX2INT( rb_funcall( right, rb_intern( "b" ), 0 ) );
	unsigned int rightA = FIX2INT( rb_funcall( right, rb_intern( "a" ), 0 ) );

	// Do calculation	
	unsigned int newR = MIN( leftR + rightR, 255 );
	unsigned int newG = MIN( leftR + rightG, 255 );
	unsigned int newB = MIN( leftR + rightB, 255 );
	unsigned int newA = MIN( leftR + rightA, 255 );
	
	return rb_funcall( globalColorClass, rb_intern( "new" ), 4, newR, newG, newB, newA );
}

/* */
static VALUE Color_Multiply( VALUE self, VALUE aRightOperand )
{
	VALUE right = Color_ForceType( aRightOperand );
	// Get values
	unsigned int leftR  = FIX2INT( rb_funcall( self, rb_intern( "r" ), 0 ) );
	unsigned int leftG  = FIX2INT( rb_funcall( self, rb_intern( "g" ), 0 ) );
	unsigned int leftB  = FIX2INT( rb_funcall( self, rb_intern( "b" ), 0 ) );
	unsigned int leftA  = FIX2INT( rb_funcall( self, rb_intern( "a" ), 0 ) );
	unsigned int rightR = FIX2INT( rb_funcall( right, rb_intern( "r" ), 0 ) );
	unsigned int rightG = FIX2INT( rb_funcall( right, rb_intern( "g" ), 0 ) );
	unsigned int rightB = FIX2INT( rb_funcall( right, rb_intern( "b" ), 0 ) );
	unsigned int rightA = FIX2INT( rb_funcall( right, rb_intern( "a" ), 0 ) );

	// Do calculation	
	unsigned int newR = ( leftR * rightR ) / 255;
	unsigned int newG = ( leftR * rightG ) / 255;
	unsigned int newB = ( leftR * rightB ) / 255;
	unsigned int newA = ( leftR * rightA ) / 255;
	
	return rb_funcall( globalColorClass, rb_intern( "new" ), 4, newR, newG, newB, newA );
}

/* */
static VALUE Color_Equal( VALUE self, VALUE anArgument )
{
	VALUE right = Color_ForceType( anArgument );
	// Get values
	unsigned int leftR  = FIX2INT( rb_funcall( self, rb_intern( "r" ), 0 ) );
	unsigned int leftG  = FIX2INT( rb_funcall( self, rb_intern( "g" ), 0 ) );
	unsigned int leftB  = FIX2INT( rb_funcall( self, rb_intern( "b" ), 0 ) );
	unsigned int leftA  = FIX2INT( rb_funcall( self, rb_intern( "a" ), 0 ) );
	unsigned int rightR = FIX2INT( rb_funcall( right, rb_intern( "r" ), 0 ) );
	unsigned int rightG = FIX2INT( rb_funcall( right, rb_intern( "g" ), 0 ) );
	unsigned int rightB = FIX2INT( rb_funcall( right, rb_intern( "b" ), 0 ) );
	unsigned int rightA = FIX2INT( rb_funcall( right, rb_intern( "a" ), 0 ) );

	// Do calculation	
	if( leftR == rightR && leftG == rightG && leftB == rightB && leftA == rightA )
	{
		return Qtrue;
	}
	return Qfalse;
}

/* call-seq:
 *   Color.new() 		-> color
 *   Color.new([r,g,b,a=255])	-> color
 *   Color.new(vector) 		-> color
 *   Color.new(r,g,b,a=255)	-> color
 * 
 * Create a new color instance.
 */
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
	rb_define_method( globalColorClass, "+", Color_Add, 1 );
	rb_define_method( globalColorClass, "*", Color_Multiply, 1 );
	rb_define_method( globalColorClass, "==", Color_Equal, 1 );
	
	// Attribute accessors
	rb_define_attr( globalColorClass, "r", 1, 1 );
	rb_define_attr( globalColorClass, "g", 1, 1 );
	rb_define_attr( globalColorClass, "b", 1, 1 );
	rb_define_attr( globalColorClass, "a", 1, 1 );
}

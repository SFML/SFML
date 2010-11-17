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
 
#include "Rect.hpp"
#include "Vector2.hpp"
#include "main.hpp"

VALUE globalRectClass;

/* Internal function
 * Forces the argument someValue to be a Vector2. IF it can convert it then it will.
 * So you can always safely asume that this function returns a Vector2 object.
 * If it fails then an exception will be thrown.
 */
VALUE Rect_ForceType( VALUE someValue )
{
	if( rb_obj_is_kind_of( someValue, rb_cArray ) == Qtrue )
	{
		VALUE arg1 = rb_ary_entry( someValue, 0 );
		VALUE arg2 = rb_ary_entry( someValue, 1 );
		VALUE arg3 = rb_ary_entry( someValue, 2 );
		VALUE arg4 = rb_ary_entry( someValue, 3 );
		return rb_funcall( globalRectClass, rb_intern( "new" ), 4, arg1, arg2, arg3, arg4 );
	}
	else if( rb_obj_is_kind_of( someValue, globalRectClass ) == Qtrue )
	{
		return someValue;
	}
	else
	{
		rb_raise( rb_eRuntimeError, "expected Array or Rect" );
	}
}

/* Internal function
 * Will copy the x and y from aSource to self.
 */
static void Rect_internal_CopyFrom( VALUE self, VALUE aSource )
{
	VALUE rect = Rect_ForceType( aSource );
	VALUE left = rb_funcall( rect, rb_intern( "left" ), 0 );
	VALUE top = rb_funcall( rect, rb_intern( "top" ), 0 );
	VALUE width = rb_funcall( rect, rb_intern( "width" ), 0 );
	VALUE height = rb_funcall( rect, rb_intern( "height" ), 0 );
	
	rb_funcall( self, rb_intern( "left=" ), 1, left );
	rb_funcall( self, rb_intern( "top=" ), 1, top );
	rb_funcall( self, rb_intern( "width=" ), 1, width ); 
	rb_funcall( self, rb_intern( "height=" ), 1, height ); 
	rb_iv_set( self, "@dataType", rb_iv_get( rect, "@dataType" ) );
}

/* Internal function
 * Validate that the passed types are the same and numeric.
 */
static void Rect_internal_ValidateTypes( VALUE aFirst, VALUE aSecond, VALUE aThird, VALUE aFourth )
{
	if( CLASS_OF( aFirst ) != CLASS_OF( aSecond ) || CLASS_OF( aFirst ) != CLASS_OF( aThird ) || CLASS_OF( aFirst ) != CLASS_OF( aFourth ) )
	{
		rb_raise( rb_eRuntimeError, "left, top, width and height must be of same type" );
	}
	
	if( rb_obj_is_kind_of( aFirst, rb_cNumeric ) == Qfalse )
	{
		rb_raise( rb_eRuntimeError, "left, top, width and height must be numeric!" );
	}
}


/* call-seq:
 *   Rect.new() 				-> rect
 *   Rect.new( [left, top, width, height] )	-> rect
 *   Rect.new( rect ) 				-> rect
 *   Rect.new( left, top, width, height )	-> rect
 *   Rect.new( position, size )			-> rect
 * 
 * Create a new rect instance.
 */
static VALUE Rect_Initialize( int argc, VALUE *args VALUE self )
{	
	VALUE arg0 = Qnil;
	VALUE arg1 = Qnil;
	switch( argc )
	{
		case 0:
			rb_iv_set( self, "@left", INT2NUM( 0 ) );
			rb_iv_set( self, "@top", INT2NUM( 0 ) );
			rb_iv_set( self, "@width", INT2NUM( 0 ) );
			rb_iv_set( self, "@height", INT2NUM( 0 ) );
			break;
		case 1:
			Color_internal_CopyFrom( self, args[0] );
			break;
		case 2:
			arg0 = Vector2_ForceType( args[0] );
			arg1 = Vector2_ForceType( args[1] );
			rb_iv_set( self, "@left", rb_funcall( arg0, rb_intern( "x" ), 0 ) );
			rb_iv_set( self, "@top", rb_funcall( arg0, rb_intern( "y" ), 0 ) );
			rb_iv_set( self, "@width", rb_funcall( arg1, rb_intern( "x" ), 0 ) );
			rb_iv_set( self, "@height", rb_funcall( arg2, rb_intern( "y" ), 0 ) );
			break;
		case 4:
			Rect_internal_ValidateTypes( args[0], args[1], args[2], args[3] );
			rb_iv_set( self, "@left", args[0]);
			rb_iv_set( self, "@top", args[1]);
			rb_iv_set( self, "@width", args[2]);
			rb_iv_set( self, "@height", args[3]);
			break;
		default:
			rb_raise( rb_eArgError, "Expected 0, 3 or 4 arguments but was given %d", argc );
	}
	return self;
	
	rb_iv_set( self, "@dataType", CLASS_OF( rb_iv_get( self, "@left" ) ) );
	return self;
}

void Init_Rect
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Utility class for manipulating 2D axis aligned rectangles.
 *
 * A rectangle is defined by its top-left corner and its size.
 *
 * It is a very simple class defined for convenience, so its member variables (left, top, width and height) are public 
 * and can be accessed directly, just like the vector classes (SFML::Vector2 and SFML::Vector3).
 *
 * To keep things simple, SFML::Rect doesn't define functions to emulate the properties that are not directly members 
 * (such as right, bottom, center, etc.), it rather only provides intersection functions.
 *
 * SFML::Rect uses the usual rules for its boundaries:
 *
 *   - The left and top edges are included in the rectangle's area
 *   - The right (left + width) and bottom (top + height) edges are excluded from the rectangle's area
 *
 * This means that SFML::Rect.new(0, 0, 1, 1) and SFML::Rect.new(1, 1, 1, 1) don't intersect.
 *
 * SFML::Rect works just like SFML::Vector2 and SFML::Vector3 when it comes to types. It will accept any value that is 
 * Numeric but all values must be of the same class.
 *
 * Usage example:
 *
 *   # Define a rectangle, located at (0, 0) with a size of 20x5
 *   r1 = SFML::Rect.new(0, 0, 20, 5)
 *
 *   # Define another rectangle, located at (4, 2) with a size of 18x10
 *   position = SFML::Vector2.new(4, 2)
 *   size = SFML::Vector2.new(18, 10)
 *   r2 = SFML::Rect.new(position, size)
 *  
 *   # Test intersections with the point (3, 1)
 *   b1 = r1.contains(3, 1); # true
 *   b2 = r2.contains(3, 1); # false
 *
 *   # Test the intersection between r1 and r2
 *   result = SFML::Rect.new
 *   b3 = r1.intersects(r2, result) # true
 *   # result == (4, 2, 16, 3)
 *
 */
	globalRectClass = rb_define_class_under( sfml, "Rect", rb_cObject );
	
	// Instance methods
	rb_define_method( globalRectClass, "initialize", Rect_Initialize, -2 );
	
	// Instance operators
	
	// Attribute accessors
	rb_define_attr( globalRectClass, "left", 1, 1 );
	rb_define_attr( globalRectClass, "top", 1, 1 );
	rb_define_attr( globalRectClass, "width", 1, 1 );
	rb_define_attr( globalRectClass, "height", 1, 1 );
}

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
 
#include "Vector2.hpp"
#include "global.hpp"

VALUE globalVector2Class;

VALUE Vector2_GetX( VALUE self )
{
	static ID idX = rb_intern( "x" );
	return rb_funcall( self, idX, 0 );
}
VALUE Vector2_GetY( VALUE self )
{
	static ID idY = rb_intern( "y" );
	return rb_funcall( self, idY, 0 );
}

VALUE Vector2_SetX( VALUE self, VALUE aVal )
{
	static ID idX = rb_intern( "x=" );
	return rb_funcall( self, idX, 1, aVal );
}
VALUE Vector2_SetY( VALUE self, VALUE aVal )
{
	static ID idY = rb_intern( "y=" );
	return rb_funcall( self, idY, 1, aVal );
}

/* Internal function
 * Forces the argument someValue to be a Vector2. IF it can convert it then it will.
 * So you can always safely asume that this function returns a Vector2 object.
 * If it fails then an exception will be thrown.
 */
VALUE Vector2_ForceType( VALUE someValue )
{
	if( rb_obj_is_kind_of( someValue, rb_cArray ) == Qtrue )
	{
		VALUE arg1 = rb_ary_entry( someValue, 0 );
		VALUE arg2 = rb_ary_entry( someValue, 1 );
		return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, arg1, arg2 );
	}
	else if( rb_obj_is_kind_of( someValue, globalVector2Class ) == Qtrue )
	{
		return someValue;
	}
	else
	{
		rb_raise( rb_eRuntimeError, "expected Array or Vector2" );
	}
}

/* Internal function
 * Will copy the x and y from aSource to self.
 */
static void Vector2_internal_CopyFrom( VALUE self, VALUE aSource )
{
	VALUE vectorSource = Vector2_ForceType( aSource );
	VALUE x = Vector2_GetX( self );
	VALUE y = Vector2_GetY( self );
	
	Vector2_SetX( self, x );
	Vector2_SetY( self, y );
	rb_iv_set( self, "@dataType", rb_iv_get( vectorSource, "@dataType" ) );
}

/* Internal function
 * Validate that the passed types are the same and numeric.
 */
static void Vector2_internal_ValidateTypes( VALUE aFirst, VALUE aSecond )
{
	if( CLASS_OF( aFirst ) != CLASS_OF( aSecond ) )
	{
		rb_raise( rb_eRuntimeError, "x and y must be of same type" );
	}
	
	if( rb_obj_is_kind_of( aFirst, rb_cNumeric ) == Qfalse )
	{
		rb_raise( rb_eRuntimeError, "x and y must be numeric!" );
	}
}

/* */
static VALUE Vector2_Negate( VALUE self )
{
	VALUE x = Vector2_GetX( self );
	VALUE y = Vector2_GetY( self );
	VALUE negatedX = rb_funcall( x, rb_intern( "-@" ), 0 );
	VALUE negatedY = rb_funcall( y, rb_intern( "-@" ), 0 );
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, negatedX, negatedY );
}

/* */
static VALUE Vector2_Add( VALUE self, VALUE aRightOperand )
{
	VALUE rightVector = Vector2_ForceType( aRightOperand );
	// Get values
	VALUE leftX = Vector2_GetX( self );
	VALUE leftY = Vector2_GetY( self );
	VALUE rightX = Vector2_GetX( rightVector );
	VALUE rightY = Vector2_GetY( rightVector );

	// Do calculation	
	VALUE newX = rb_funcall( leftX, rb_intern( "+" ), 1, rightX );
	VALUE newY = rb_funcall( leftY, rb_intern( "+" ), 1, rightY );
	
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, newX, newY );
}

/* */
static VALUE Vector2_Subtract( VALUE self, VALUE aRightOperand )
{
	VALUE rightVector = Vector2_ForceType( aRightOperand );
	// Get values
	VALUE leftX = Vector2_GetX( self );
	VALUE leftY = Vector2_GetY( self );
	VALUE rightX = Vector2_GetX( rightVector );
	VALUE rightY = Vector2_GetY( rightVector );

	// Do calculation
	VALUE newX = rb_funcall( leftX, rb_intern( "-" ), 1, rightX );
	VALUE newY = rb_funcall( leftY, rb_intern( "-" ), 1, rightY );
	
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, newX, newY );
}

/* */
static VALUE Vector2_Multiply( VALUE self, VALUE aRightOperand )
{
	VALUE rightVector = Vector2_ForceType( aRightOperand );
	// Get values
	VALUE leftX = Vector2_GetX( self );
	VALUE leftY = Vector2_GetY( self );
	VALUE rightX = Vector2_GetX( rightVector );
	VALUE rightY = Vector2_GetY( rightVector );

	// Do calculation
	VALUE newX = rb_funcall( leftX, rb_intern( "*" ), 1, rightX );
	VALUE newY = rb_funcall( leftY, rb_intern( "*" ), 1, rightY );
	
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, newX, newY );
}

/* */
static VALUE Vector2_Divide( VALUE self, VALUE aRightOperand )
{
	VALUE rightVector = Vector2_ForceType( aRightOperand );
	// Get values
	VALUE leftX = Vector2_GetX( self );
	VALUE leftY = Vector2_GetY( self );
	VALUE rightX = Vector2_GetX( rightVector );
	VALUE rightY = Vector2_GetY( rightVector );

	// Do calculation	
	VALUE newX = rb_funcall( leftX, rb_intern( "/" ), 1, rightX );
	VALUE newY = rb_funcall( leftY, rb_intern( "/" ), 1, rightY );
	
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, newX, newY );
}

/* */
static VALUE Vector2_Equal( VALUE self, VALUE anArgument )
{
	VALUE aVector = Vector2_ForceType( anArgument );
	VALUE leftX = Vector2_GetX( self );
	VALUE leftY = Vector2_GetY( self );
	VALUE rightX = Vector2_GetX( aVector );
	VALUE rightY = Vector2_GetY( aVector );
	
	if( rb_funcall( leftX, rb_intern( "==" ), 1, rightX ) == Qtrue &&
	    rb_funcall( leftY, rb_intern( "==" ), 1, rightY ) == Qtrue )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

/* */
static VALUE Vector2_StrictEqual( VALUE self, VALUE anArgument )
{
	VALUE aVector = Vector2_ForceType( anArgument );
	VALUE leftX = Vector2_GetX( self );
	VALUE leftY = Vector2_GetY( self );
	VALUE rightX = Vector2_GetX( aVector );
	VALUE rightY = Vector2_GetY( aVector );
	
	if( rb_funcall( leftX, rb_intern( "eql?" ), 1, rightX ) == Qtrue &&
	    rb_funcall( leftY, rb_intern( "eql?" ), 1, rightY ) == Qtrue )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

/* call-seq:
 *   Vector2.new() 		-> vector
 *   Vector2.new([x,y])		-> vector
 *   Vector2.new(vector) 	-> vector
 *   Vector2.new(x,y)		-> vector
 * 
 * Create a new vector instance.
 */
static VALUE Vector2_Initialize( VALUE self, VALUE someArgs )
{
	long arrayLength = RARRAY_LEN( someArgs );
	rb_iv_set( self, "@x", INT2NUM( 0 ) );
	rb_iv_set( self, "@y", INT2NUM( 0 ) );
	
	if( arrayLength == 0 )
	{
		// Nothing needs to be done
	}
	else if( arrayLength == 1 )
	{
		Vector2_internal_CopyFrom( self, rb_ary_entry( someArgs, 0 ) );
	}
	else if( arrayLength == 2 )
	{
		VALUE arg1 = rb_ary_entry( someArgs, 0 );
		VALUE arg2 = rb_ary_entry( someArgs, 1 );
		Vector2_internal_ValidateTypes( arg1, arg2 );
		
		rb_iv_set( self, "@x", arg1 );
		rb_iv_set( self, "@y", arg2 );
	}
	
	rb_iv_set( self, "@dataType", CLASS_OF( rb_iv_get( self, "@x" ) ) );
	return self;
}

void Init_Vector2( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* SFML::Vector2 is a simple class that defines a mathematical vector with two coordinates (x and y).
 *
 * It can be used to represent anything that has two dimensions: a size, a point, a velocity, etc.
 *
 * This class differs from the C++ version. It will accept any value that is Numeric and both x and y must be of the same class.
 *
 *   v1 = SFML::Vector2.new(16.5, 24.0)
 *   v1.x = 18.2
 *   y = v1.y
 *
 *   v2 = v1 * v1;
 *   v3 = SFML::Vector2.new
 *   v3 = v1 + v2
 *
 *   different = (v2 != v3);
 */
	globalVector2Class = rb_define_class_under( sfml, "Vector2", rb_cObject );
	
	// Instance methods
	rb_define_method( globalVector2Class, "initialize", Vector2_Initialize, -2 );
	rb_define_method( globalVector2Class, "eql?", Vector2_StrictEqual, 1 );
	
	// Instance operators
	rb_define_method( globalVector2Class, "-@", Vector2_Negate, 0 );
	rb_define_method( globalVector2Class, "+", Vector2_Add, 1 );
	rb_define_method( globalVector2Class, "-", Vector2_Subtract, 1 );
	rb_define_method( globalVector2Class, "*", Vector2_Multiply, 1 );
	rb_define_method( globalVector2Class, "/", Vector2_Divide, 1 );
	rb_define_method( globalVector2Class, "==", Vector2_Equal, 1 );
	
	// Attribute accessors
	rb_define_attr( globalVector2Class, "x", 1, 1 );
	rb_define_attr( globalVector2Class, "y", 1, 1 );
}

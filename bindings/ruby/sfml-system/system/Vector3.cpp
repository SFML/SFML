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
 
#include "Vector3.hpp"
#include "main.hpp"

VALUE globalVector3Class;

/* Internal function
 * Forces the argument someValue to be a Vector3. IF it can convert it then it will.
 * So you can always safely asume that this function returns a Vector3 object.
 * If it fails then an exception will be thrown.
 */
VALUE Vector3_ForceType( VALUE someValue )
{
	if( rb_obj_is_kind_of( someValue, rb_cArray ) == Qtrue )
	{
		VALUE arg1 = rb_ary_entry( someValue, 0 );
		VALUE arg2 = rb_ary_entry( someValue, 1 );
		VALUE arg3 = rb_ary_entry( someValue, 2 );
		return rb_funcall( globalVector3Class, rb_intern( "new" ), 3, arg1, arg2, arg3 );
	}
	else if( rb_obj_is_kind_of( someValue, globalVector3Class ) == Qtrue )
	{
		return someValue;
	}
	else
	{
		rb_raise( rb_eRuntimeError, "expected Array or Vector3" );
	}
}

/* Internal function
 * Will copy the x, y and z from aSource to self.
 */
static void Vector3_internal_CopyFrom( VALUE self, VALUE aSource )
{
	VALUE vectorSource = Vector3_ForceType( aSource );
	VALUE x = rb_funcall( vectorSource, rb_intern( "x" ), 0 );
	VALUE y = rb_funcall( vectorSource, rb_intern( "y" ), 0 );
	VALUE z = rb_funcall( vectorSource, rb_intern( "z" ), 0 );
	
	rb_funcall( self, rb_intern( "x=" ), 1, x ); 
	rb_funcall( self, rb_intern( "y=" ), 1, y );
	rb_funcall( self, rb_intern( "z=" ), 1, z );
	rb_iv_set( self, "@dataType", rb_iv_get( vectorSource, "@dataType" ) );
}

/* Internal function
 * Validate that the passed values types are the same and numeric.
 */
static void Vector3_internal_ValidateTypes( VALUE aFirst, VALUE aSecond, VALUE aThird )
{
	if( CLASS_OF( aFirst ) != CLASS_OF( aSecond ) && CLASS_OF( aFirst ) != CLASS_OF( aThird ) )
	{
		rb_raise( rb_eRuntimeError, "x, y and z must be of same type" );
	}
	
	if( rb_obj_is_kind_of( aFirst, rb_cNumeric ) == Qfalse )
	{
		rb_raise( rb_eRuntimeError, "x, y and z must be numeric!" );
	}
}

/* */
static VALUE Vector3_Negate( VALUE self )
{
	VALUE x = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE y = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE z = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE negatedX = rb_funcall( x, rb_intern( "-@" ), 0 );
	VALUE negatedY = rb_funcall( y, rb_intern( "-@" ), 0 );
	VALUE negatedZ = rb_funcall( z, rb_intern( "-@" ), 0 );
	return rb_funcall( globalVector3Class, rb_intern( "new" ), 2, negatedX, negatedY, negatedZ );
}

/* */
static VALUE Vector3_Add( VALUE self, VALUE aRightOperand )
{
	VALUE rightVector = Vector3_ForceType( aRightOperand );
	// Get values
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE leftZ  = rb_funcall( self, rb_intern( "z" ), 0 );
	VALUE rightX = rb_funcall( rightVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( rightVector, rb_intern( "y" ), 0 );
	VALUE rightZ = rb_funcall( rightVector, rb_intern( "z" ), 0 );

	// Do calculation	
	VALUE newX = rb_funcall( leftX, rb_intern( "+" ), 1, rightX );
	VALUE newY = rb_funcall( leftY, rb_intern( "+" ), 1, rightY );
	VALUE newZ = rb_funcall( leftZ, rb_intern( "+" ), 1, rightZ );
	
	return rb_funcall( globalVector3Class, rb_intern( "new" ), 2, newX, newY, newZ );
}

/* */
static VALUE Vector3_Subtract( VALUE self, VALUE aRightOperand )
{
	VALUE rightVector = Vector3_ForceType( aRightOperand );
	// Get values
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE leftZ  = rb_funcall( self, rb_intern( "z" ), 0 );
	VALUE rightX = rb_funcall( rightVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( rightVector, rb_intern( "y" ), 0 );
	VALUE rightZ = rb_funcall( rightVector, rb_intern( "z" ), 0 );

	// Do calculation	
	VALUE newX = rb_funcall( leftX, rb_intern( "-" ), 1, rightX );
	VALUE newY = rb_funcall( leftY, rb_intern( "-" ), 1, rightY );
	VALUE newZ = rb_funcall( leftZ, rb_intern( "-" ), 1, rightZ );
	
	return rb_funcall( globalVector3Class, rb_intern( "new" ), 2, newX, newY, newZ );
}

/* */
static VALUE Vector3_Multiply( VALUE self, VALUE aRightOperand )
{
	VALUE rightVector = Vector3_ForceType( aRightOperand );
	// Get values
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE leftZ  = rb_funcall( self, rb_intern( "z" ), 0 );
	VALUE rightX = rb_funcall( rightVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( rightVector, rb_intern( "y" ), 0 );
	VALUE rightZ = rb_funcall( rightVector, rb_intern( "z" ), 0 );

	// Do calculation	
	VALUE newX = rb_funcall( leftX, rb_intern( "*" ), 1, rightX );
	VALUE newY = rb_funcall( leftY, rb_intern( "*" ), 1, rightY );
	VALUE newZ = rb_funcall( leftZ, rb_intern( "*" ), 1, rightZ );
	
	return rb_funcall( globalVector3Class, rb_intern( "new" ), 2, newX, newY, newZ );
}

/* */
static VALUE Vector3_Divide( VALUE self, VALUE aRightOperand )
{
	VALUE rightVector = Vector3_ForceType( aRightOperand );
	// Get values
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE leftZ  = rb_funcall( self, rb_intern( "z" ), 0 );
	VALUE rightX = rb_funcall( rightVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( rightVector, rb_intern( "y" ), 0 );
	VALUE rightZ = rb_funcall( rightVector, rb_intern( "z" ), 0 );

	// Do calculation	
	VALUE newX = rb_funcall( leftX, rb_intern( "/" ), 1, rightX );
	VALUE newY = rb_funcall( leftY, rb_intern( "/" ), 1, rightY );
	VALUE newZ = rb_funcall( leftZ, rb_intern( "/" ), 1, rightZ );
	
	return rb_funcall( globalVector3Class, rb_intern( "new" ), 2, newX, newY, newZ );
}

/* */
static VALUE Vector3_Equal( VALUE self, VALUE anArgument )
{
	VALUE aVector = Vector3_ForceType( anArgument );
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE leftZ  = rb_funcall( self, rb_intern( "z" ), 0 );
	VALUE rightX = rb_funcall( aVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( aVector, rb_intern( "y" ), 0 );
	VALUE rightZ = rb_funcall( aVector, rb_intern( "z" ), 0 );
	
	if( rb_funcall( leftX, rb_intern( "==" ), 1, rightX ) == Qtrue &&
	    rb_funcall( leftY, rb_intern( "==" ), 1, rightY ) == Qtrue &&
	    rb_funcall( leftZ, rb_intern( "==" ), 1, rightZ ) == Qtrue )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

/* */
static VALUE Vector3_StrictEqual( VALUE self, VALUE anArgument )
{
	VALUE aVector = Vector3_ForceType( anArgument );
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE leftZ  = rb_funcall( self, rb_intern( "z" ), 0 );
	VALUE rightX = rb_funcall( aVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( aVector, rb_intern( "y" ), 0 );
	VALUE rightZ = rb_funcall( aVector, rb_intern( "z" ), 0 );
	
	if( rb_funcall( leftX, rb_intern( "eql?" ), 1, rightX ) == Qtrue &&
	    rb_funcall( leftY, rb_intern( "eql?" ), 1, rightY ) == Qtrue &&
	    rb_funcall( leftZ, rb_intern( "eql?" ), 1, rightZ ) == Qtrue )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

/* call-seq:
 *   Vector3.new() 			-> vector
 *   Vector3.new([x,y,z])	-> vector
 *	 Vector3.new(vector) 	-> vector
 *   Vector3.new(x,y,z)		-> vector
 * 
 * Create a new vector instance.
 */
static VALUE Vector3_Initialize( VALUE self, VALUE someArgs )
{
	long arrayLength = RARRAY_LEN( someArgs );
	rb_iv_set( self, "@x", INT2NUM( 0 ) );
	rb_iv_set( self, "@y", INT2NUM( 0 ) );
	rb_iv_set( self, "@z", INT2NUM( 0 ) );
	
	if( arrayLength == 0 )
	{
		// Nothing needs to be done
	}
	else if( arrayLength == 1 )
	{
		Vector3_internal_CopyFrom( self, rb_ary_entry( someArgs, 0 ) );
	}
	else if( arrayLength == 3 )
	{
		VALUE arg1 = rb_ary_entry( someArgs, 0 );
		VALUE arg2 = rb_ary_entry( someArgs, 1 );
		VALUE arg3 = rb_ary_entry( someArgs, 1 );
		Vector3_internal_ValidateTypes( arg1, arg2, arg3 );
		
		rb_iv_set( self, "@x", arg1 );
		rb_iv_set( self, "@y", arg2 );
		rb_iv_set( self, "@z", arg3 );
	}
	
	rb_iv_set( self, "@dataType", CLASS_OF( rb_iv_get( self, "@x" ) ) );
	return self;
}

void Init_Vector3( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* SFML::Vector3 is a simple class that defines a mathematical vector with three coordinates (x, y and z).
 *
 * It can be used to represent anything that has three dimensions: a size, a point, a velocity, etc.
 *
 * This class differs from the C++ version. It will accept any value that is Numeric and both x, y an z must be of the same class.
 *
 *   v1 = SFML::Vector3.new(16.5, 24.0, -8.2)
 *   v1.z = 18.2
 *   y = v1.y
 *
 *   v2 = v1 * v1;
 *   v3 = SFML::Vector3.new
 *   v3 = v1 + v2
 *
 *   different = (v2 != v3);
 */
	globalVector3Class = rb_define_class_under( sfml, "Vector3", rb_cObject );
	
	// Instance methods
	rb_define_method( globalVector3Class, "initialize", Vector3_Initialize, -2 );
	rb_define_method( globalVector3Class, "eql?", Vector3_StrictEqual, 1 );
	
	// Instance operators
	rb_define_method( globalVector3Class, "-@", Vector3_Negate, 0 );
	rb_define_method( globalVector3Class, "+", Vector3_Add, 1 );
	rb_define_method( globalVector3Class, "-", Vector3_Subtract, 1 );
	rb_define_method( globalVector3Class, "*", Vector3_Multiply, 1 );
	rb_define_method( globalVector3Class, "/", Vector3_Divide, 1 );
	rb_define_method( globalVector3Class, "==", Vector3_Equal, 1 );
	
	// Attribute accessors
	rb_define_attr( globalVector3Class, "x", 1, 1 );
	rb_define_attr( globalVector3Class, "y", 1, 1 );
	rb_define_attr( globalVector3Class, "z", 1, 1 );
}

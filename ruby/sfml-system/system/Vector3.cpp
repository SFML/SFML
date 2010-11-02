#include "Vector3.hpp"
#include "System.hpp"

VALUE globalVector3Class;

VALUE Vector3_ForceType( VALUE someValue )
{
	if( rb_obj_is_kind_of( someValue, rb_cArray ) == true )
	{
		VALUE arg1 = rb_ary_entry( someValue, 0 );
		VALUE arg2 = rb_ary_entry( someValue, 1 );
		VALUE arg3 = rb_ary_entry( someValue, 2 );
		return rb_funcall( globalVector3Class, rb_intern( "new" ), 3, arg1, arg2, arg3 );
	}
	else if( rb_obj_is_kind_of( someValue, globalVector3Class ) == true )
	{
		return someValue;
	}
	else
	{
		rb_raise( rb_eRuntimeError, "expected Array or Vector3" );
	}
}

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

VALUE Vector3_New( int anArgCount, VALUE * someArgs, VALUE aKlass )
{
	return rb_call_super( anArgCount, someArgs );
}

void Init_Vector3( void )
{
	globalVector3Class = rb_define_class_under( GetNamespace(), "Vector3", rb_cObject );
	
	// Class methods
	rb_define_singleton_method( globalVector3Class, "new", FUNCPTR( Vector3_New ), -1 );
	
	// Instance methods
	rb_define_method( globalVector3Class, "initialize", FUNCPTR( Vector3_Initialize ), -2 );
	rb_define_method( globalVector3Class, "eql?", FUNCPTR( Vector3_Initialize ), 1 );
	
	// Instance operators
	rb_define_method( globalVector3Class, "-@", FUNCPTR( Vector3_Negate ), 0 );
	rb_define_method( globalVector3Class, "+", FUNCPTR( Vector3_Add ), 1 );
	rb_define_method( globalVector3Class, "-", FUNCPTR( Vector3_Subtract ), 1 );
	rb_define_method( globalVector3Class, "*", FUNCPTR( Vector3_Multiply ), 1 );
	rb_define_method( globalVector3Class, "/", FUNCPTR( Vector3_Divide ), 1 );
	rb_define_method( globalVector3Class, "==", FUNCPTR( Vector3_Divide ), 1 );
	
	// Attribute accessors
	rb_define_attr( globalVector3Class, "x", 1, 1 );
	rb_define_attr( globalVector3Class, "y", 1, 1 );
	rb_define_attr( globalVector3Class, "z", 1, 1 );
}

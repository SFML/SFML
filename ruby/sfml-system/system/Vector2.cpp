#include "Vector2.hpp"
#include "System.hpp"

VALUE globalVector2Class;

VALUE Vector2_ForceType( VALUE someValue )
{
	if( rb_obj_is_kind_of( someValue, rb_cArray ) == true )
	{
		VALUE arg1 = rb_ary_entry( someValue, 0 );
		VALUE arg2 = rb_ary_entry( someValue, 1 );
		return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, arg1, arg2 );
	}
	else if( rb_obj_is_kind_of( someValue, globalVector2Class ) == true )
	{
		return someValue;
	}
	else
	{
		rb_raise( rb_eRuntimeError, "expected Array or Vector2" );
	}
}

static void Vector2_internal_CopyFrom( VALUE self, VALUE aSource )
{
	VALUE vectorSource = Vector2_ForceType( aSource );
	VALUE x = rb_funcall( vectorSource, rb_intern( "x" ), 0 );
	VALUE y = rb_funcall( vectorSource, rb_intern( "y" ), 0 );
	
	rb_funcall( self, rb_intern( "x=" ), 1, x ); 
	rb_funcall( self, rb_intern( "y=" ), 1, y );
	rb_iv_set( self, "@dataType", rb_iv_get( vectorSource, "@dataType" ) );
}

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

static VALUE Vector2_Negate( VALUE self )
{
	VALUE x = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE y = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE negatedX = rb_funcall( x, rb_intern( "-@" ), 0 );
	VALUE negatedY = rb_funcall( y, rb_intern( "-@" ), 0 );
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, negatedX, negatedY );
}

static VALUE Vector2_Add( VALUE self, VALUE aRightOperand )
{
	VALUE rightVector = Vector2_ForceType( aRightOperand );
	// Get values
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE rightX = rb_funcall( rightVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( rightVector, rb_intern( "y" ), 0 );

	// Do calculation	
	VALUE newX = rb_funcall( leftX, rb_intern( "+" ), 1, rightX );
	VALUE newY = rb_funcall( leftY, rb_intern( "+" ), 1, rightY );
	
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, newX, newY );
}

static VALUE Vector2_Subtract( VALUE self, VALUE aRightOperand )
{
	VALUE rightVector = Vector2_ForceType( aRightOperand );
	// Get values
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE rightX = rb_funcall( rightVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( rightVector, rb_intern( "y" ), 0 );

	// Do calculation
	VALUE newX = rb_funcall( leftX, rb_intern( "-" ), 1, rightX );
	VALUE newY = rb_funcall( leftY, rb_intern( "-" ), 1, rightY );
	
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, newX, newY );
}

static VALUE Vector2_Multiply( VALUE self, VALUE aRightOperand )
{
	VALUE rightVector = Vector2_ForceType( aRightOperand );
	// Get values
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE rightX = rb_funcall( rightVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( rightVector, rb_intern( "y" ), 0 );

	// Do calculation
	VALUE newX = rb_funcall( leftX, rb_intern( "*" ), 1, rightX );
	VALUE newY = rb_funcall( leftY, rb_intern( "*" ), 1, rightY );
	
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, newX, newY );
}

static VALUE Vector2_Divide( VALUE self, VALUE aRightOperand )
{
	VALUE rightVector = Vector2_ForceType( aRightOperand );
	// Get values
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE rightX = rb_funcall( rightVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( rightVector, rb_intern( "y" ), 0 );

	// Do calculation	
	VALUE newX = rb_funcall( leftX, rb_intern( "/" ), 1, rightX );
	VALUE newY = rb_funcall( leftY, rb_intern( "/" ), 1, rightY );
	
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, newX, newY );
}

static VALUE Vector2_Equal( VALUE self, VALUE anArgument )
{
	VALUE aVector = Vector2_ForceType( anArgument );
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE rightX = rb_funcall( aVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( aVector, rb_intern( "y" ), 0 );
	
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

static VALUE Vector2_StrictEqual( VALUE self, VALUE anArgument )
{
	VALUE aVector = Vector2_ForceType( anArgument );
	VALUE leftX  = rb_funcall( self, rb_intern( "x" ), 0 );
	VALUE leftY  = rb_funcall( self, rb_intern( "y" ), 0 );
	VALUE rightX = rb_funcall( aVector, rb_intern( "x" ), 0 );
	VALUE rightY = rb_funcall( aVector, rb_intern( "y" ), 0 );
	
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

VALUE Vector2_New( int anArgCount, VALUE * someArgs, VALUE aKlass )
{
	return rb_call_super( anArgCount, someArgs );
}

void Init_Vector2( void )
{
	globalVector2Class = rb_define_class_under( GetNamespace(), "Vector2", rb_cObject );
	
	// Class methods
	rb_define_singleton_method( globalVector2Class, "new", FUNCPTR( Vector2_New ), -1 );
	
	// Instance methods
	rb_define_method( globalVector2Class, "initialize", FUNCPTR( Vector2_Initialize ), -2 );
	rb_define_method( globalVector2Class, "eql?", FUNCPTR( Vector2_Initialize ), 1 );
	
	// Instance operators
	rb_define_method( globalVector2Class, "-@", FUNCPTR( Vector2_Negate ), 0 );
	rb_define_method( globalVector2Class, "+", FUNCPTR( Vector2_Add ), 1 );
	rb_define_method( globalVector2Class, "-", FUNCPTR( Vector2_Subtract ), 1 );
	rb_define_method( globalVector2Class, "*", FUNCPTR( Vector2_Multiply ), 1 );
	rb_define_method( globalVector2Class, "/", FUNCPTR( Vector2_Divide ), 1 );
	rb_define_method( globalVector2Class, "==", FUNCPTR( Vector2_Divide ), 1 );
	
	// Attribute accessors
	rb_define_attr( globalVector2Class, "x", 1, 1 );
	rb_define_attr( globalVector2Class, "y", 1, 1 );
}

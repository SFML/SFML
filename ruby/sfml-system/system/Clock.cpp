#include "Clock.hpp"
#include "System.hpp"
#include <SFML/System/Clock.hpp>

VALUE globalClockClass;

static void Clock_Free( sf::Clock *anObject )
{
	delete anObject;
}

static VALUE Clock_GetElapsedTime( VALUE self )
{
	sf::Clock *object = NULL;
	Data_Get_Struct( self, sf::Clock, object );
	return rb_float_new( object->GetElapsedTime() );
}

static VALUE Clock_Reset( VALUE self )
{
	sf::Clock *object = NULL;
	Data_Get_Struct( self, sf::Clock, object );
	object->Reset();
	return Qnil;
}

VALUE Clock_New( VALUE aKlass )
{
	sf::Clock *object = new sf::Clock();
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, Clock_Free, object );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

void Init_Clock( void )
{
	globalClockClass = rb_define_class_under( GetNamespace(), "Clock", rb_cObject );
	
	// Class methods
	rb_define_singleton_method( globalClockClass, "new", FUNCPTR( Clock_New ), 0 );
	
	// Instance methods
	rb_define_method( globalClockClass, "getElapsedTime", FUNCPTR( Clock_GetElapsedTime ), 0 );
	rb_define_method( globalClockClass, "reset", FUNCPTR( Clock_Reset ), 0 );
	
	// Aliases
	rb_define_alias( globalClockClass, "elapsedTime", "getElapsedTime" );
}

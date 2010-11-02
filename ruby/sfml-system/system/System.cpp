#include "System.hpp"
#include "Clock.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"

VALUE globalSFMLNamespace;

VALUE GetNamespace( void )
{
	return globalSFMLNamespace;
}

void Init_system( void )
{
	globalSFMLNamespace = rb_define_module( "SFML" );
	Init_Clock();
	Init_Vector2();
	Init_Vector3();
}

#include "ruby.h"

extern "C"
{
	void Init_all( void )
	{
		rb_require( "sfml/system" );
		rb_require( "sfml/window" );
		rb_require( "sfml/graphics" );
		rb_require( "sfml/audio" );
	}
}
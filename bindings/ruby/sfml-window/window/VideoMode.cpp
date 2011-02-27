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

#include "VideoMode.hpp"
#include "main.hpp"
#include <SFML/Window/VideoMode.hpp>

VALUE globalVideoModeClass;

/* Internal function
 * Forces the argument someValue to be a VideoMode. If it can convert it then it will.
 * So you can always safely asume that this function returns a VideoMode object.
 * If it fails then an exception will be thrown.
 */
VALUE VideoMode_ForceType( VALUE someValue )
{
	if( rb_obj_is_kind_of( someValue, rb_cArray ) == Qtrue )
	{
		VALUE arg1 = rb_ary_entry( someValue, 0 );
		VALUE arg2 = rb_ary_entry( someValue, 1 );
		if( FIX2INT( rb_funcall( someValue, rb_intern( "size" ), 0 ) ) == 3 )
		{
			VALUE arg3 = rb_ary_entry( someValue, 2 );
			return rb_funcall( globalVideoModeClass, rb_intern( "new" ), 3, arg1, arg2, arg3 );
		}
		else
		{
			return rb_funcall( globalVideoModeClass, rb_intern( "new" ), 2, arg1, arg2 );
		}
	}
	else if( rb_obj_is_kind_of( someValue, globalVideoModeClass ) == Qtrue )
	{
		return someValue;
	}
	else
	{
		rb_raise( rb_eRuntimeError, "expected Array[width, height, bpp] or VideoMode" );
	}
}

/* Free a heap allocated object
 * Not accessible trough ruby directly!
 */
static void VideoMode_Free( sf::VideoMode *anObject )
{
	delete anObject;
}

/* call-seq:
 *   mode.width		-> width
 *
 * Video mode width, in pixels.
 */
static VALUE VideoMode_GetWidth( VALUE self )
{
	sf::VideoMode *object = NULL;
	Data_Get_Struct( self, sf::VideoMode, object );
	return INT2FIX( object->Width );
}

/* call-seq:
 *   mode.width=(new_width)	-> new_width
 *
 * Video mode width, in pixels.
 */
static VALUE VideoMode_SetWidth( VALUE self, VALUE aValue )
{
	sf::VideoMode *object = NULL;
	Data_Get_Struct( self, sf::VideoMode, object );
	return INT2FIX( object->Width = NUM2UINT( aValue ) );
}

/* call-seq:
 *   mode.height	-> height
 *
 * Video mode height, in pixels.
 */
static VALUE VideoMode_GetHeight( VALUE self )
{
	sf::VideoMode *object = NULL;
	Data_Get_Struct( self, sf::VideoMode, object );
	return INT2FIX( object->Height );
}

/* call-seq:
 *   mode.height=(new_height)	-> new_height
 *
 * Video mode height, in pixels.
 */
static VALUE VideoMode_SetHeight( VALUE self, VALUE aValue )
{
	sf::VideoMode *object = NULL;
	Data_Get_Struct( self, sf::VideoMode, object );
	return INT2FIX( object->Height = NUM2UINT( aValue ) );
}

/* call-seq:
 *   mode.bitsPerPixel	-> bpp
 *
 * Video mode pixel depth, in bits per pixels.
 */
static VALUE VideoMode_GetBitsPerPixel( VALUE self )
{
	sf::VideoMode *object = NULL;
	Data_Get_Struct( self, sf::VideoMode, object );
	return INT2FIX( object->BitsPerPixel );
}

/* call-seq:
 *   mode.bitsPerPixel=(new_bpp)	-> new_bpp
 *
 * Video mode pixel depth, in bits per pixels.
 */
static VALUE VideoMode_SetBitsPerPixel( VALUE self, VALUE aValue )
{
	sf::VideoMode *object = NULL;
	Data_Get_Struct( self, sf::VideoMode, object );
	return INT2FIX( object->BitsPerPixel = NUM2UINT( aValue ) );
}

/* call-seq:
 *   mode.isValid()	-> true or false
 *
 * Tell whether or not the video mode is valid.
 *
 * The validity of video modes is only relevant when using fullscreen windows; otherwise any video mode can be used
 * with no restriction.
 */
static VALUE VideoMode_IsValid( VALUE self )
{
	sf::VideoMode *object = NULL;
	Data_Get_Struct( self, sf::VideoMode, object );
	return ( object->IsValid() == true ? Qtrue : Qfalse );
}

static VALUE VideoMode_InitializeCopy( VALUE self, VALUE aSource )
{
	sf::VideoMode *object = NULL;
	Data_Get_Struct( self, sf::VideoMode, object );
	sf::VideoMode *source = NULL;
	Data_Get_Struct( aSource, sf::VideoMode, source );
	*object = *source;
}

/* call-seq:
 *   VideoMode.getDesktopMode	-> desktop_mode
 *
 * Get the current desktop video mode.
 */
static VALUE VideoMode_GetDesktopMode( VALUE aKlass )
{
	sf::VideoMode *object = new sf::VideoMode( sf::VideoMode::GetDesktopMode() );
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, VideoMode_Free, object );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

/* call-seq:
 *   VideoMode.getFullscreenModes	-> [supported_modes]
 *
 * Retrieve all the video modes supported in fullscreen mode.
 *
 * When creating a fullscreen window, the video mode is restricted to be compatible with what the graphics driver and
 * monitor support. This function returns the complete list of all video modes that can be used in fullscreen mode.
 * The returned array is sorted from best to worst, so that the first element will always give the best mode
 * (higher width, height and bits-per-pixel).
 */
static VALUE VideoMode_GetFullscreenModes( VALUE aKlass )
{
	const std::vector< sf::VideoMode >& modes = sf::VideoMode::GetFullscreenModes();
	VALUE array = rb_ary_new();
	for( std::vector< sf::VideoMode >::const_iterator it = modes.begin(), end = modes.end(); it != end; it++ )
	{
		sf::VideoMode *object = new sf::VideoMode( *it );
		VALUE rbData = Data_Wrap_Struct( aKlass, 0, VideoMode_Free, object );
		rb_obj_call_init( rbData, 0, 0 );
		rb_ary_push( array, rbData );
	}
	return array;
}

static VALUE VideoMode_Alloc( VALUE aKlass )
{
	sf::VideoMode *object = new sf::VideoMode();
	return Data_Wrap_Struct( aKlass, 0, VideoMode_Free, object );
}

/* call-seq:
 *   VideoMode.new()							-> mode
 *   VideoMode.new( width, height, bpp = 32 )	-> mode
 *
 * Create a new mode.
 */
static VALUE VideoMode_Initialize( int argc, VALUE *args, VALUE self )
{
	sf::VideoMode *object = NULL;
	Data_Get_Struct( self, sf::VideoMode, object );
	switch( argc )
	{
		case 0:
			break;
		case 3:
			object->BitsPerPixel = NUM2UINT( args[2] );
		case 2:
			object->Height = NUM2UINT( args[1] );
			object->Width = NUM2UINT( args[0] );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 0..3 arguments but was given %d", argc );
			return Qnil;
	}
	return self;
}

void Init_VideoMode( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* A video mode is defined by a width and a height (in pixels) and a depth (in bits per pixel).
 *
 * Video modes are used to setup windows (SFML::Window) at creation time.
 *
 * The main usage of video modes is for fullscreen mode: indeed you must use one of the valid
 * video modes allowed by the OS (which are defined by what the monitor and the graphics card support),
 * otherwise your window creation will just fail.
 *
 * SFML::VideoMode provides a static function for retrieving the list of all the video modes supported by
 * the system: getFullscreenModes().
 *
 * A custom video mode can also be checked directly for fullscreen compatibility with its isValid() function.
 *
 * Additionnally, SFML::VideoMode provides a static function to get the mode currently used by the desktop:
 * getDesktopMode(). This allows to build windows with the same size or pixel depth as the current resolution.
 *
 * Usage example:
 *
 *   # Display the list of all the video modes available for fullscreen
 *   modes = SFML::VideoMode.getFullscreenModes()
 *   i = 0
 *   modes.each do | mode |
 *     puts "Mode #" + i + ": " + mode.width + "x" + mode.height + " - " + mode.bitsPerPixel + " bpp"
 *
 *   end
 *
 *   # Create a window with the same pixel depth as the desktop
 *   desktop = SFML::VideoMode.getDesktopMode()
 *   window.create( SFML::VideoMode.new( 1024, 768, desktop.BitsPerPixel ), "SFML window" )
 */
	globalVideoModeClass = rb_define_class_under( sfml, "VideoMode", rb_cObject );

	// Class methods
	//rb_define_singleton_method( globalVideoModeClass, "new", VideoMode_New, -1 );
	rb_define_alloc_func( globalVideoModeClass, VideoMode_Alloc );
	rb_define_singleton_method( globalVideoModeClass, "getDesktopMode", VideoMode_GetDesktopMode, 0 );
	rb_define_singleton_method( globalVideoModeClass, "getFullscreenModes", VideoMode_GetFullscreenModes, 0 );

	// Instance methods
	rb_define_method( globalVideoModeClass, "initialize", VideoMode_Initialize, -1 );
	rb_define_method( globalVideoModeClass, "initialize_copy", VideoMode_InitializeCopy, 1 );

	rb_define_method( globalVideoModeClass, "width", VideoMode_GetWidth, 0 );
	rb_define_method( globalVideoModeClass, "width=", VideoMode_SetWidth, 1 );

	rb_define_method( globalVideoModeClass, "height", VideoMode_GetHeight, 0 );
	rb_define_method( globalVideoModeClass, "height=", VideoMode_SetHeight, 1 );

	rb_define_method( globalVideoModeClass, "bitsPerPixel", VideoMode_GetBitsPerPixel, 0 );
	rb_define_method( globalVideoModeClass, "bitsPerPixel=", VideoMode_SetBitsPerPixel, 1 );

	rb_define_method( globalVideoModeClass, "isValid", VideoMode_IsValid, 0 );

	// Class aliases
	rb_define_alias( CLASS_OF( globalVideoModeClass ), "desktopMode", "getDesktopMode" );
	rb_define_alias( CLASS_OF( globalVideoModeClass ), "desktop_mode", "getDesktopMode" );
	rb_define_alias( CLASS_OF( globalVideoModeClass ), "fullscreenModes", "getFullscreenModes" );
	rb_define_alias( CLASS_OF( globalVideoModeClass ), "fullscreen_modes", "getFullscreenModes" );

	// Aliases
	rb_define_alias( globalVideoModeClass, "bits_per_pixel", "bitsPerPixel" );
	rb_define_alias( globalVideoModeClass, "bits_per_pixel=", "bitsPerPixel=" );
	rb_define_alias( globalVideoModeClass, "bpp", "bitsPerPixel" );
	rb_define_alias( globalVideoModeClass, "bpp=", "bitsPerPixel=" );

	rb_define_alias( globalVideoModeClass, "is_valid", "isValid" );
	rb_define_alias( globalVideoModeClass, "valid?", "isValid" );
}

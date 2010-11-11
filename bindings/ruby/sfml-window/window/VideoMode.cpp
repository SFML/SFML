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
#include <iostream>

/* A video mode is defined by a width and a height (in pixels) and a depth (in bits per pixel).
 *
 * Video modes are used to setup windows (sf::Window) at creation time.
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
 *   modes = SFMK::VideoMode.getFullscreenModes()
 *   i = 0
 *   modes.each do | mode |
 *     puts "Mode #" + i + ": " + mode.Width + "x" + mode.Height + " - " + mode.BitsPerPixel + " bpp"
 *
 *   end
 *
 *   # Create a window with the same pixel depth as the desktop
 *   desktop = SFML::VideoMode.getDesktopMode()
 *   window.create( SFML::VideoMode.new( 1024, 768, desktop.BitsPerPixel ), "SFML window" )
 */
VALUE globalVideoModeClass;

/* Free a heap allocated object 
 * Not accessible trough ruby directly!
 */
static void VideoMode_Free( sf::VideoMode *anObject )
{
	delete anObject;
}

static VALUE VideoMode_GetWidth( VALUE self )
{
	sf::VideoMode *object = NULL;
	Data_Get_Struct( self, sf::VideoMode, object );
	return INT2FIX( object->Width );
}

static VALUE VideoMode_SetWidth( VALUE self, VALUE aValue )
{
	sf::VideoMode *object = NULL;
	Data_Get_Struct( self, sf::VideoMode, object );
	return INT2FIX( object->Width = NUM2UINT( aValue ) );
}

static VALUE VideoMode_GetHeight( VALUE self )
{
	sf::VideoMode *object = NULL;
	Data_Get_Struct( self, sf::VideoMode, object );
	return INT2FIX( object->Height );
}

static VALUE VideoMode_SetHeight( VALUE self, VALUE aValue )
{
	sf::VideoMode *object = NULL;
	Data_Get_Struct( self, sf::VideoMode, object );
	return INT2FIX( object->Height = NUM2UINT( aValue ) );
}

static VALUE VideoMode_GetBitsPerPixel( VALUE self )
{
	sf::VideoMode *object = NULL;
	Data_Get_Struct( self, sf::VideoMode, object );
	return INT2FIX( object->BitsPerPixel );
}

static VALUE VideoMode_SetBitsPerPixel( VALUE self, VALUE aValue )
{
	sf::VideoMode *object = NULL;
	Data_Get_Struct( self, sf::VideoMode, object );
	return INT2FIX( object->BitsPerPixel = NUM2UINT( aValue ) );
}

static VALUE VideoMode_GetDesktopMode( VALUE aKlass )
{
	sf::VideoMode *object = new sf::VideoMode( sf::VideoMode::GetDesktopMode() );
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, VideoMode_Free, object );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

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

static VALUE VideoMode_New( int argc, VALUE *args, VALUE aKlass )
{
	sf::VideoMode *object = NULL;
	switch( argc )
	{
		case 0:
			object = new sf::VideoMode();
			break;
		case 2:
			object = new sf::VideoMode( INT2FIX( args[0] ), INT2FIX( args[1] ) );
			break;
		case 3:
			object = new sf::VideoMode( INT2FIX( args[0] ), INT2FIX( args[1] ), INT2FIX( args[2] ) );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 0 2 or 3 arguments but was given %d", argc );
			break;
	}
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, VideoMode_Free, object );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

void Init_VideoMode( void )
{
	globalVideoModeClass = rb_define_class_under( GetNamespace(), "VideoMode", rb_cObject );
	
	// Class methods
	rb_define_singleton_method( globalVideoModeClass, "new", FUNCPTR( VideoMode_New ), -1 );
	rb_define_singleton_method( globalVideoModeClass, "getDesktopMode", FUNCPTR( VideoMode_GetDesktopMode ), 0 );
	rb_define_singleton_method( globalVideoModeClass, "getFullscreenModes", FUNCPTR( VideoMode_GetFullscreenModes ), 0 );
	
	// Instance methods
	rb_define_method( globalVideoModeClass, "width", FUNCPTR( VideoMode_GetWidth ), 0 );
	rb_define_method( globalVideoModeClass, "width=", FUNCPTR( VideoMode_SetWidth ), 1 );
	
	rb_define_method( globalVideoModeClass, "height", FUNCPTR( VideoMode_GetWidth ), 0 );
	rb_define_method( globalVideoModeClass, "height=", FUNCPTR( VideoMode_SetWidth ), 1 );

	rb_define_method( globalVideoModeClass, "bitsPerPixel", FUNCPTR( VideoMode_GetBitsPerPixel ), 0 );
	rb_define_method( globalVideoModeClass, "bitsPerPixel=", FUNCPTR( VideoMode_SetBitsPerPixel ), 1 );
	
	// Aliases
	rb_define_alias( globalVideoModeClass, "bits_per_pixel", "bitsPerPixel" );
	rb_define_alias( globalVideoModeClass, "bits_per_pixel=", "bitsPerPixel=" );
	rb_define_alias( globalVideoModeClass, "bpp", "bitsPerPixel" );
	rb_define_alias( globalVideoModeClass, "bpp=", "bitsPerPixel=" );
}

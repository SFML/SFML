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

static VALUE VideoMode_New( VALUE aKlass, VALUE someArgs )
{
	sf::ContextSettings *object = new sf::ContextSettings();
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, VideoMode_Free, object );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

void Init_VideoMode( void )
{
	globalVideoModeClass = rb_define_class_under( GetNamespace(), "VideoMode", rb_cObject );
	
	// Class methods
	rb_define_singleton_method( globalVideoModeClass, "new", FUNCPTR( VideoMode ), 0 );
	
	// Instance methods
	
	// Aliases
}

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
 
#include "ContextSettings.hpp"
#include "main.hpp"
#include <SFML/Window/ContextSettings.hpp>
#include <iostream>

VALUE globalContextSettingsClass;

/* Free a heap allocated object 
 * Not accessible trough ruby directly!
 */
static void ContextSettings_Free( sf::ContextSettings *anObject )
{
	delete anObject;
}

/* call-seq:
 *   settings.depthBits	-> depth
 *
 * Bits of the depth buffer
 */
static VALUE ContextSettings_GetDepth( VALUE self )
{
	sf::ContextSettings *object = NULL;
	Data_Get_Struct( self, sf::ContextSettings, object );
	return INT2FIX( object->DepthBits );
}

/* call-seq:
 *   settings.depthBits=(new_depth)	-> new_depth
 *
 * Bits of the depth buffer
 */
static VALUE ContextSettings_SetDepth( VALUE self, VALUE aValue )
{
	sf::ContextSettings *object = NULL;
	Data_Get_Struct( self, sf::ContextSettings, object );
	return INT2FIX( object->DepthBits = NUM2UINT( aValue ) );
}

/* call-seq:
 *   settings.stencilBits	-> stencil
 *
 * Bits of the stencil buffer
 */
static VALUE ContextSettings_GetStencil( VALUE self )
{
	sf::ContextSettings *object = NULL;
	Data_Get_Struct( self, sf::ContextSettings, object );
	return INT2FIX( object->StencilBits );
}

/* call-seq:
 *   settings.stencilBits=(new_stencil)	-> new_stencil
 *
 * Bits of the stencil buffer
 */
static VALUE ContextSettings_SetStencil( VALUE self, VALUE aValue )
{
	sf::ContextSettings *object = NULL;
	Data_Get_Struct( self, sf::ContextSettings, object );
	return INT2FIX( object->StencilBits = NUM2UINT( aValue ) );
}

/* call-seq:
 *   settings.antialiasingLevel	-> antialiasing
 *
 * Level of antialiasing
 */
static VALUE ContextSettings_GetAntialiasing( VALUE self )
{
	sf::ContextSettings *object = NULL;
	Data_Get_Struct( self, sf::ContextSettings, object );
	return INT2FIX( object->AntialiasingLevel );
}

/* call-seq:
 *   settings.antialiasingLevel=(new_antialiasing)	-> new_antialiasing
 *
 * Level of antialiasing
 */
static VALUE ContextSettings_SetAntialiasing( VALUE self, VALUE aValue )
{
	sf::ContextSettings *object = NULL;
	Data_Get_Struct( self, sf::ContextSettings, object );
	return INT2FIX( object->AntialiasingLevel = NUM2UINT( aValue ) );
}

/* call-seq:
 *   settings.majorVersion	-> major
 *
 * Major number of the context version to create
 */
static VALUE ContextSettings_GetMajorVersion( VALUE self )
{
	sf::ContextSettings *object = NULL;
	Data_Get_Struct( self, sf::ContextSettings, object );
	return INT2FIX( object->MajorVersion );
}

/* call-seq:
 *   settings.majorVersion=(new_major)	-> new_major
 *
 * Major number of the context version to create
 */
static VALUE ContextSettings_SetMajorVersion( VALUE self, VALUE aValue )
{
	sf::ContextSettings *object = NULL;
	Data_Get_Struct( self, sf::ContextSettings, object );
	return INT2FIX( object->MajorVersion = NUM2UINT( aValue ) );
}

/* call-seq:
 *   settings.minorVersion	-> minor
 *
 * Minor number of the context version to create
 */
static VALUE ContextSettings_GetMinorVersion( VALUE self )
{
	sf::ContextSettings *object = NULL;
	Data_Get_Struct( self, sf::ContextSettings, object );
	return INT2FIX( object->MinorVersion );
}

/* call-seq:
 *   settings.minorVersion=(new_minor)	-> new_minor
 *
 * Minor number of the context version to create
 */
static VALUE ContextSettings_SetMinorVersion( VALUE self, VALUE aValue )
{
	sf::ContextSettings *object = NULL;
	Data_Get_Struct( self, sf::ContextSettings, object );
	return INT2FIX( object->MinorVersion = NUM2UINT( aValue ) );
}

static VALUE ContextSettings_InitializeCopy( VALUE self, VALUE aSource )
{
	sf::ContextSettings *object = NULL;
	Data_Get_Struct( self, sf::ContextSettings, object );
	sf::ContextSettings *source = NULL;
	Data_Get_Struct( aSource, sf::ContextSettings, source );
	*object = *source;
	return self;
}

/* call-seq:
 *   ContextSettings.new( depth = 24, stencil = 8, antialiasing = 0, major = 2, minor = 0)	-> settings
 *
 * The constructor creates the settings
 */
static VALUE ContextSettings_Alloc( VALUE aKlass )
{
	sf::ContextSettings *object = new sf::ContextSettings();	 
	return Data_Wrap_Struct( aKlass, 0, ContextSettings_Free, object );;
}

static VALUE ContextSettings_Initialize( int argc, VALUE *args, VALUE self )
{
	sf::ContextSettings *object = NULL;
	Data_Get_Struct( self, sf::ContextSettings, object );
	switch( argc )
	{
		case 0:
			break;
		case 5:
			object->MinorVersion = NUM2UINT( args[4] );
		case 4:
			object->MajorVersion = NUM2UINT( args[3] );
		case 3:
			object->AntialiasingLevel = NUM2UINT( args[2] );
		case 2:
			object->StencilBits = NUM2UINT( args[1] );
		case 1:
			object->DepthBits = NUM2UINT( args[0] );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 0..5 arguments but was given %d", argc );
			return Qnil;
	}
	return self;
}

void Init_ContextSettings( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* ContextSettings allows to define several advanced settings of the OpenGL 
 * context attached to a window.
 *
 * All these settings have no impact on the regular SFML rendering 
 * (graphics module) -- except the anti-aliasing level, so you may need to use 
 * this structure only if you're using SFML as a windowing system for custom 
 * OpenGL rendering.
 *
 * The DepthBits and StencilBits members define the number of bits per pixel 
 * requested for the (respectively) depth and stencil buffers.
 *
 * AntialiasingLevel represents the requested number of multisampling levels 
 * for anti-aliasing.
 *
 * MajorVersion and MinorVersion define the version of the OpenGL context that 
 * you want. Only versions greater or equal to 3.0 are relevant; versions 
 * lesser than 3.0 are all handled the same way (i.e. you can use any version 
 * < 3.0 if you don't want an OpenGL 3 context).
 *
 * Please note that these values are only a hint. No failure will be reported 
 * if one or more of these values are not supported by the system; instead, 
 * SFML will try to find the closest valid match. You can then retrieve the 
 * settings that the window actually used to create its context, with 
 * Window::GetSettings().
 */
	globalContextSettingsClass = rb_define_class_under( sfml, "ContextSettings", rb_cObject );
	
	// Class methods
	//rb_define_singleton_method( globalContextSettingsClass, "new", ContextSettings_New, -1 );
	rb_define_alloc_func( globalContextSettingsClass, ContextSettings_Alloc );
	
	// Instance methods
	rb_define_method( globalContextSettingsClass, "initialize", ContextSettings_Initialize, -1 );
	rb_define_method( globalContextSettingsClass, "initialize_copy", ContextSettings_InitializeCopy, 1 );
	
	rb_define_method( globalContextSettingsClass, "depthBits", ContextSettings_GetDepth, 0 );
	rb_define_method( globalContextSettingsClass, "depthBits=", ContextSettings_SetDepth, 1 );
	
	rb_define_method( globalContextSettingsClass, "stencilBits", ContextSettings_GetStencil, 0 );
	rb_define_method( globalContextSettingsClass, "stencilBits=", ContextSettings_SetStencil, 1 );
	
	rb_define_method( globalContextSettingsClass, "antialiasingLevel", ContextSettings_GetAntialiasing, 0 );
	rb_define_method( globalContextSettingsClass, "antialiasingLevel=", ContextSettings_SetAntialiasing, 1 );
	
	rb_define_method( globalContextSettingsClass, "majorVersion", ContextSettings_GetMajorVersion, 0 );
	rb_define_method( globalContextSettingsClass, "majorVersion=", ContextSettings_SetMajorVersion, 1 );
	
	rb_define_method( globalContextSettingsClass, "minorVersion", ContextSettings_GetMinorVersion, 0 );
	rb_define_method( globalContextSettingsClass, "minorVersion=", ContextSettings_SetMinorVersion, 1 );
	
	// Aliases
	rb_define_alias( globalContextSettingsClass, "depth", "depthBits" );
	rb_define_alias( globalContextSettingsClass, "depth=", "depthBits=" );
	rb_define_alias( globalContextSettingsClass, "depth_bits", "depthBits" );
	rb_define_alias( globalContextSettingsClass, "depth_bits=", "depthBits=" );
	
	rb_define_alias( globalContextSettingsClass, "stencil", "stencilBits" );
	rb_define_alias( globalContextSettingsClass, "stencil=", "stencilBits=" );
	rb_define_alias( globalContextSettingsClass, "stencil_bits", "stencilBits" );
	rb_define_alias( globalContextSettingsClass, "stencil_bits=", "stencilBits=" );
	
	rb_define_alias( globalContextSettingsClass, "antialiasing", "antialiasingLevel" );
	rb_define_alias( globalContextSettingsClass, "antialiasing=", "antialiasingLevel=" );
	rb_define_alias( globalContextSettingsClass, "antialiasing_level", "antialiasingLevel" );
	rb_define_alias( globalContextSettingsClass, "antialiasing_level=", "antialiasingLevel=" );
	
	rb_define_alias( globalContextSettingsClass, "major", "majorVersion" );
	rb_define_alias( globalContextSettingsClass, "major=", "majorVersion=" );
	rb_define_alias( globalContextSettingsClass, "major_version", "majorVersion" );
	rb_define_alias( globalContextSettingsClass, "major_version=", "majorVersion=" );
	
	rb_define_alias( globalContextSettingsClass, "minor", "minorVersion" );
	rb_define_alias( globalContextSettingsClass, "minor=", "minorVersion=" );
	rb_define_alias( globalContextSettingsClass, "minor_version", "minorVersion" );
	rb_define_alias( globalContextSettingsClass, "minor_version=", "minorVersion=" );
	
	
}

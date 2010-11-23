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
 
#include "RenderTarget.hpp"
#include "Color.hpp"
#include "main.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

VALUE globalRenderTargetModule;
VALUE globalRenderTargetInstanceClass;

/* External classes */
extern VALUE globalVector2Class;
extern VALUE globalRectClass;
extern VALUE globalDrawableModule;
extern VALUE globalShaderClass;
extern VALUE globalViewClass;

static VALUE RenderTarget_Clear( int argc, VALUE *args, VALUE self )
{
	sf::Color color = sf::Color::Black;
	switch( argc )
	{
		case 1:
		{
				VALUE temp = Color_ForceType( args[0] );
				color.r = FIX2UINT( Color_GetR( temp ) );
				color.g = FIX2UINT( Color_GetG( temp ) );
				color.b = FIX2UINT( Color_GetB( temp ) );
				color.a = FIX2UINT( Color_GetA( temp ) );
		}
		case 0:
			break;
		default:
			rb_raise( rb_eArgError, "Expected 0 or 1 arguments but was given %d", argc );
	}
	
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	object->Clear( color );
	return Qnil;
}

static VALUE RenderTarget_Draw( int argc, VALUE *args, VALUE self )
{
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	std::cout << object << std::endl;
	switch( argc )
	{
		case 2:
		{
			VALIDATE_CLASS( args[0], globalDrawableModule, "object" );
			VALIDATE_CLASS( args[1], globalShaderClass, "shader" );
			sf::Drawable *drawable = NULL;
			Data_Get_Struct( args[0], sf::Drawable, drawable );
			sf::Shader *shader = NULL;
			Data_Get_Struct( args[1], sf::Shader, shader );
			object->Draw( *drawable, *shader );
			break;
		}
		case 1:
		{
			VALIDATE_CLASS( args[0], globalDrawableModule, "object" );
			sf::Drawable *drawable = NULL;
			Data_Get_Struct( args[0], sf::Drawable, drawable );
			object->Draw( *drawable );
			break;
		}
		default:
			rb_raise( rb_eArgError, "Expected 1 or 2 arguments but was given %d", argc );
	}
	return Qnil;
}

static VALUE RenderTarget_GetWidth( VALUE self )
{
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	return INT2FIX( object->GetWidth() );
}

static VALUE RenderTarget_GetHeight( VALUE self )
{
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	return INT2FIX( object->GetHeight() );
}

static VALUE RenderTarget_SetView( VALUE self, VALUE aView )
{
	VALIDATE_CLASS( aView, globalViewClass, "view" );
	sf::View *view = NULL;
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	Data_Get_Struct( aView, sf::View, view );
	object->SetView( *view );
	return Qnil;
}

static VALUE RenderTarget_GetView( VALUE self )
{
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	const sf::View &view = object->GetView();
	VALUE rbData = Data_Wrap_Struct( globalViewClass, 0, 0, const_cast< sf::View * >( &view ) );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

static VALUE RenderTarget_GetDefaultView( VALUE self )
{
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	const sf::View &view = object->GetDefaultView();
	VALUE rbData = Data_Wrap_Struct( globalViewClass, 0, 0, const_cast< sf::View * >( &view ) );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

static VALUE RenderTarget_GetViewport( VALUE self, VALUE aView )
{
	VALIDATE_CLASS( aView, globalViewClass, "view" );
	sf::View *view = NULL;
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	Data_Get_Struct( aView, sf::View, view );
	sf::IntRect viewport = object->GetViewport( *view );
	return rb_funcall( globalRectClass, rb_intern( "new" ), 4, 
				INT2FIX( viewport.Left ), INT2FIX( viewport.Top ),
				INT2FIX( viewport.Width ), INT2FIX( viewport.Height ) );
}

static VALUE RenderTarget_ConvertCoords( int argc, VALUE *args, VALUE self )
{
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	sf::Vector2f result;
	switch( argc )
	{
		case 2:
		{
			result = object->ConvertCoords( FIX2UINT( args[0] ), FIX2UINT( args[1] ) );
			break;
		}
		case 3:
		{
			VALIDATE_CLASS( args[2], globalViewClass, "view" );
			sf::View *view = NULL;
			Data_Get_Struct( args[2], sf::View, view );
			result = object->ConvertCoords( FIX2UINT( args[0] ), FIX2UINT( args[1] ), *view );
		}
		default:
			rb_raise( rb_eArgError, "Expected 2 or 3 arguments but was given %d", argc );
	}
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, rb_float_new( result.x ), rb_float_new( result.y ) );
}

static VALUE RenderTarget_SaveGLStates( VALUE self )
{
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	object->SaveGLStates();
	return Qnil;
}

static VALUE RenderTarget_RestoreGLStates( VALUE self )
{
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	object->RestoreGLStates();
	return Qnil;
}


void Init_RenderTarget( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* This is not the same class as the one specified in the C++ library! This class acts as a wrapper around the 
 * classes that inherits from sf::RenderTarget(sf::RenderWindow, sf::RenderImage) and you will only use it as the
 * first argument in the SFML::Drawable#render method. 
 */
 	globalRenderTargetModule	= rb_define_module_under( sfml, "RenderTarget" );
	globalRenderTargetInstanceClass = rb_define_class_under( globalRenderTargetModule, "Instance", rb_cObject );
	rb_include_module( globalRenderTargetInstanceClass, globalRenderTargetModule );
		
	// Instance methods
	rb_define_method( globalRenderTargetModule, "clear", RenderTarget_Clear, -1 );
	rb_define_method( globalRenderTargetModule, "draw", RenderTarget_Draw, -1 );
	rb_define_method( globalRenderTargetModule, "getWidth", RenderTarget_GetWidth, 0 );
	rb_define_method( globalRenderTargetModule, "getHeight", RenderTarget_GetHeight, 0 );
	rb_define_method( globalRenderTargetModule, "setView", RenderTarget_SetView, 1 );
	rb_define_method( globalRenderTargetModule, "getView", RenderTarget_GetView, 0 );
	rb_define_method( globalRenderTargetModule, "getDefaultView", RenderTarget_GetDefaultView, 0 );
	rb_define_method( globalRenderTargetModule, "getViewport", RenderTarget_GetViewport, 1 );
	rb_define_method( globalRenderTargetModule, "convertCoords", RenderTarget_ConvertCoords, -1 );
	rb_define_method( globalRenderTargetModule, "saveGLStates", RenderTarget_SaveGLStates, 0 );
	rb_define_method( globalRenderTargetModule, "restoreGLStates", RenderTarget_RestoreGLStates, 0 );
		
	// Instance Aliases
	rb_define_alias( globalRenderTargetModule, "width", "getWidth" );
	rb_define_alias( globalRenderTargetModule, "height", "getHeight" );
	rb_define_alias( globalRenderTargetModule, "view=", "setView" );
	rb_define_alias( globalRenderTargetModule, "view", "getView" );
	rb_define_alias( globalRenderTargetModule, "defaultView", "getDefaultView" );
	rb_define_alias( globalRenderTargetModule, "default_view", "getDefaultView" );
	rb_define_alias( globalRenderTargetModule, "get_viewport", "getViewport" );
	rb_define_alias( globalRenderTargetModule, "convert_cords", "convertCoords" );
	rb_define_alias( globalRenderTargetModule, "save_gl_states", "saveGLStates" );
	rb_define_alias( globalRenderTargetModule, "restore_gl_states", "restoreGLStates" );
}

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
 
#include "RenderImage.hpp"
#include "main.hpp"
#include <SFML/Graphics/RenderImage.hpp>

VALUE globalRenderImageClass;

/* External classes */
extern VALUE globalRenderTargetModule;
extern VALUE globalImageClass;

static void RenderImage_Free( sf::RenderImage *anObject )
{
	delete anObject;
}

static VALUE RenderImage_Create( int argc, VALUE *args, VALUE self )
{
	unsigned int width = 0;
	unsigned int height = 0;
	bool depthBuffer = false;
	switch( argc )
	{
		case 3:
			if( args[2] == Qtrue )
			{
				depthBuffer = true;
			}
			else if( args[2] == Qfalse )
			{
				depthBuffer = false;
			}
			else
			{
				VALIDATE_CLASS( args[2], rb_cTrueClass, "depthBuffer" );
			}
		case 2:
			width  = FIX2UINT( args[0] );
			height = FIX2UINT( args[1] );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 2 or 3 arguments but was given %d", argc );
	}
	
	sf::RenderImage *object = NULL;
	Data_Get_Struct( self, sf::RenderImage, object );
	if( object->Create( width, height, depthBuffer ) == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

static VALUE RenderImage_Draw( int argc, VALUE *args, VALUE self )
{
	sf::RenderImage *object = NULL;
	Data_Get_Struct( self, sf::RenderImage, object );
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

static VALUE RenderImage_Display( VALUE self )
{
	sf::RenderImage *object = NULL;
	Data_Get_Struct( self, sf::RenderImage, object );
	object->Display();
}

static VALUE RenderImage_GetImage( VALUE self )
{
	sf::RenderImage *object = NULL;
	Data_Get_Struct( self, sf::RenderImage, object );
	const sf::Image &image = object->GetImage();
	VALUE rbData = Data_Wrap_Struct( globalImageClass, 0, 0, const_cast< sf::Image * >( &image ) );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

static VALUE RenderImage_IsSmooth( VALUE self )
{
	sf::RenderImage *object = NULL;
	Data_Get_Struct( self, sf::RenderImage, object );
	return ( object->IsSmooth() == true ? Qtrue : Qfalse );
}

static VALUE RenderImage_SetActive( int argc, VALUE *args, VALUE self )
{
	sf::RenderImage *object = NULL;
	Data_Get_Struct( self, sf::RenderImage, object );
	bool flag = true;
	switch( argc )
	{
		case 1:
			if( args[0] == Qtrue )
			{
				flag = true;
			}
			else if( args[0] == Qfalse )
			{
				flag = false;
			}
			else
			{
				VALIDATE_CLASS( args[0], rb_cTrueClass, "active" );
			}
		case 0:
			break;
		default:
			rb_raise( rb_eArgError, "Expected 0 or 1 arguments but was given %d", argc );	
	}
	object->SetActive( flag );
	return Qnil;
}

static VALUE RenderImage_SetSmooth( VALUE self, VALUE aSmoothFlag )
{
	sf::RenderImage *object = NULL;
	Data_Get_Struct( self, sf::RenderImage, object );
	if( aSmoothFlag == Qtrue )
	{
		object->SetSmooth( true );
	}
	else if( aSmoothFlag == Qfalse )
	{
		object->SetSmooth( false );
	}
	else
	{
		VALIDATE_CLASS( aSmoothFlag, rb_cTrueClass, "smooth" );
	}
	return Qnil;
}

static VALUE RenderImage_New( int argc, VALUE *args, VALUE aKlass )
{
	sf::RenderImage *object = new sf::RenderImage();
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, RenderImage_Free, object );
	rb_obj_call_init( rbData, argc, args );
	return rbData;
}

static VALUE RenderImage_IsAvailable( VALUE aKlass )
{
	return ( sf::RenderImage::IsAvailable() == true ? Qtrue : Qfalse );
}

void Init_RenderImage( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Target for off-screen 2D rendering into an image.
 *
 * sf::RenderImage is the little brother of sf::RenderWindow.
 *
 * It implements the same 2D drawing and OpenGL-related functions (see their base class sf::RenderTarget for 
 * more details), the difference is that the result is stored in an off-screen image rather than being show in a window.
 *
 * Rendering to an image can be useful in a variety of situations:
 *
 *   - precomputing a complex static image (like a level's background from multiple tiles)
 *   - applying post-effects to the whole scene with shaders
 *   - creating a sprite from a 3D object rendered with OpenGL
 *   - etc.
 *
 * Usage example:
 *
 *   # First of all: make sure that rendering to image is supported
 *   if SFML::RenderImage.available? == false
 *     # Handle error
 *   end
 *
 *   # Create a new render-window
 *   window = SFML::RenderWindow.new( SFML::VideoMode.new(800, 600), "SFML window" )
 *
 *   # Create a new render-image
 *   image = SFML::RenderImage.new
 *   if image.create( 500, 500 ) == false
 *     # Handle error
 *   end
 *
 *   # The main loop
 *   while window.open?
 *     # Event processing
 *     # ...
 *
 *     # Clear the whole image with red color
 *     image.clear( SFML::Color::Red )
 *
 *     # Draw stuff to the image
 *     image.draw( sprite )  # sprite is a SFML::Sprite
 *     image.draw( shape )   # shape is a SFML::Shape
 *     image.draw( text )    # text is a SFML::Text
 *
 *     # We're done drawing to the image
 *     image.display()
 *
 *     # Now we start rendering to the window, clear it first
 *     window.clear()
 *
 *     # Draw the image
 *     sprite = SFML::Sprite.new( image.getImage() )
 *     window.draw( sprite )
 *
 *     # End the current frame and display its contents on screen
 *     window.display()
 *   end
 *
 * Like SFML::RenderWindow, SFML::RenderImage is still able to render direct OpenGL stuff. It is even possible to mix
 * together OpenGL calls and regular SFML drawing commands. If you need a depth buffer for 3D rendering, don't 
 * forget to request it when calling SFML::RenderImage#create.
 */
 	globalRenderImageClass	= rb_define_class_under( sfml, "RenderImage", rb_cObject );
	rb_include_module( globalRenderImageClass, globalRenderTargetModule );
	
	// Class methods
	rb_define_singleton_method( globalRenderImageClass, "new", RenderImage_New, 0 );
	rb_define_singleton_method( globalRenderImageClass, "isAvailable", RenderImage_IsAvailable, 0 );
	
	// Instance methods
	rb_define_method( globalRenderImageClass, "create", RenderImage_Create, -1 );
	rb_define_method( globalRenderImageClass, "display", RenderImage_Display, 0 );
	rb_define_method( globalRenderImageClass, "getImage", RenderImage_GetImage, 0 );
	rb_define_method( globalRenderImageClass, "isSmooth", RenderImage_IsSmooth, 0 );
	rb_define_method( globalRenderImageClass, "setActive", RenderImage_SetActive, -1 );
	rb_define_method( globalRenderImageClass, "setSmooth", RenderImage_SetSmooth, 1 );
	
	// Class Aliases
	rb_define_alias( CLASS_OF( globalRenderImageClass ), "is_available", "isAvailable" );
	rb_define_alias( CLASS_OF( globalRenderImageClass ), "available?", "isAvailable" );
	
	// Instance Aliases
	rb_define_alias( globalRenderImageClass, "image", "getImage" );
	
	rb_define_alias( globalRenderImageClass, "is_smooth", "isSmooth" );
	rb_define_alias( globalRenderImageClass, "smooth?", "isSmooth" );
	
	rb_define_alias( globalRenderImageClass, "set_active", "setActive" );
	rb_define_alias( globalRenderImageClass, "activate", "setActive" );
	rb_define_alias( globalRenderImageClass, "active=", "setActive" );
	
	rb_define_alias( globalRenderImageClass, "smooth=", "setSmooth" );
}

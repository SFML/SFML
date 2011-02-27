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
extern VALUE globalDrawableModule;
extern VALUE globalShaderClass;
extern VALUE globalViewClass;

static void RenderImage_Free( sf::RenderImage *anObject )
{
	delete anObject;
}

static void View_Free( sf::View *anObject )
{
	delete anObject;
}

/* call-seq:
 *   render_image.create( width, height, depthBuffer = false )	-> true or false
 *
 * Create the render-image.
 *
 * Before calling this function, the render-image is in an invalid state, thus it is mandatory to call it before 
 * doing anything with the render-image. The last parameter, depthBuffer, is useful if you want to use the render-image
 * for 3D OpenGL rendering that requires a depth-buffer. Otherwise it is unnecessary, and you should leave this 
 * parameter to false (which is its default value).
 */
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

/* call-seq:
 *   render_image.display()
 *
 * Update the contents of the target image.
 *
 * This function updates the target image with what has been drawn so far. Like for windows, calling this function is
 * mandatory at the end of rendering. Not calling it may leave the image in an undefined state. 
 */
static VALUE RenderImage_Display( VALUE self )
{
	sf::RenderImage *object = NULL;
	Data_Get_Struct( self, sf::RenderImage, object );
	object->Display();
	return Qnil;
}

/* call-seq:
 *   render_image.getImage()	-> image
 *
 * Get a read-only reference to the target image.
 *
 * After drawing to the render-image and calling display, you can retrieve the updated image using this function, and 
 * draw it using a sprite (for example). The internal SFML::Image of a render-image is always the same instance, so that
 * it is possible to call this function once and keep a reference to the image even after is it modified.
 */
static VALUE RenderImage_GetImage( VALUE self )
{
	sf::RenderImage *object = NULL;
	Data_Get_Struct( self, sf::RenderImage, object );
	const sf::Image &image = object->GetImage();
	VALUE rbData = Data_Wrap_Struct( globalImageClass, 0, 0, const_cast< sf::Image * >( &image ) );
	rb_obj_call_init( rbData, 0, 0 );
	rb_iv_set( rbData, "@__owner_ref", self );
	return rbData;
}

/* call-seq:
 *   render_image.isSmooth()	-> true or false
 *
 * Tell whether the smooth filtering is enabled or not. 
 */
static VALUE RenderImage_IsSmooth( VALUE self )
{
	sf::RenderImage *object = NULL;
	Data_Get_Struct( self, sf::RenderImage, object );
	return ( object->IsSmooth() == true ? Qtrue : Qfalse );
}

/* call-seq:
 *   render_image.setActive( active )
 *
 * Activate of deactivate the render-image for rendering.
 *
 * This function makes the render-image's context current for future OpenGL rendering operations (so you shouldn't 
 * care about it if you're not doing direct OpenGL stuff). Only one context can be current on a thread, so if you want
 * to draw OpenGL geometry to another render target (like a RenderWindow) don't forget to activate it again.
 */
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

/* call-seq:
 *   render_image.setSmooth( smooth )
 *
 * Enable or disable image smoothing.
 *
 * This function is similar to SFML::Image#setSmooth. This parameter is enabled by default.
 */
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

static VALUE RenderImage_SetView( VALUE self, VALUE aView )
{
	VALIDATE_CLASS( aView, globalViewClass, "view" );
	sf::View *view = NULL;
	sf::RenderImage *object = NULL;
	Data_Get_Struct( self, sf::RenderImage, object );
	Data_Get_Struct( aView, sf::View, view );
	object->SetView( *view );
	return Qnil;
}

static VALUE RenderImage_GetView( VALUE self )
{
	sf::RenderImage *object = NULL;
	Data_Get_Struct( self, sf::RenderImage, object );
	const sf::View &original = object->GetView();
	sf::View * view = new sf::View( original );
	VALUE rbData = Data_Wrap_Struct( globalViewClass, 0, View_Free, view );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

static VALUE RenderImage_GetDefaultView( VALUE self )
{
	sf::RenderImage *object = NULL;
	Data_Get_Struct( self, sf::RenderImage, object );
	const sf::View &original = object->GetDefaultView();
	sf::View * view = new sf::View( original );
	VALUE rbData = Data_Wrap_Struct( globalViewClass, 0, View_Free, view );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

static VALUE RenderImage_GetWidth( VALUE self )
{
	sf::RenderImage *object = NULL;
	Data_Get_Struct( self, sf::RenderImage, object );
	return INT2FIX( object->GetWidth() );
}

static VALUE RenderImage_GetHeight( VALUE self )
{
	sf::RenderImage *object = NULL;
	Data_Get_Struct( self, sf::RenderImage, object );
	return INT2FIX( object->GetHeight() );
}

/* call-seq:
 *   RenderImage.new()										-> render_image
 *   RenderImage.new( width, height, depthBuffer = false )	-> render_image
 *
 * Will create a new render image instance.
 * 
 * If any arguments are specified then a call to the #create method will be made passing the arguments to it.
 */
static VALUE RenderImage_Initialize( int argc, VALUE *args, VALUE self )
{
	if( argc > 0 )
	{
		rb_funcall2( self, rb_intern( "create" ), argc, args );
	}
	return self;
}

static VALUE RenderImage_Alloc( VALUE aKlass )
{
	sf::RenderImage *object = new sf::RenderImage();
	return Data_Wrap_Struct( aKlass, 0, RenderImage_Free, object );
}

/* call-seq:
 *   RenderImage.isAvailable()	-> true or false
 *
 * Check whether the system supports render images or not.
 *
 * It is very important to always call this function before trying to use the RenderImage class, as the feature may 
 * not be supported on all platforms (especially very old ones). If this function returns false, then you won't be 
 * able to use the class at all.
 */
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
	//rb_define_singleton_method( globalRenderImageClass, "new", RenderImage_New, 0 );
	rb_define_alloc_func( globalRenderImageClass, RenderImage_Alloc );
	rb_define_singleton_method( globalRenderImageClass, "isAvailable", RenderImage_IsAvailable, 0 );
	
	// Instance methods
	rb_define_method( globalRenderImageClass, "initialize", RenderImage_Initialize, -1 );
	rb_define_method( globalRenderImageClass, "draw", RenderImage_Create, -1 );
	rb_define_method( globalRenderImageClass, "create", RenderImage_Create, -1 );
	rb_define_method( globalRenderImageClass, "display", RenderImage_Display, 0 );
	rb_define_method( globalRenderImageClass, "getImage", RenderImage_GetImage, 0 );
	rb_define_method( globalRenderImageClass, "isSmooth", RenderImage_IsSmooth, 0 );
	rb_define_method( globalRenderImageClass, "setActive", RenderImage_SetActive, -1 );
	rb_define_method( globalRenderImageClass, "setSmooth", RenderImage_SetSmooth, 1 );
	rb_define_method( globalRenderImageClass, "getView", RenderImage_GetView, 0 );
	rb_define_method( globalRenderImageClass, "setView", RenderImage_SetView, 1 );
	rb_define_method( globalRenderImageClass, "getDefaultView", RenderImage_GetDefaultView, 0 );
	rb_define_method( globalRenderImageClass, "getWidth", RenderImage_GetWidth, 0 );
	rb_define_method( globalRenderImageClass, "getHeight", RenderImage_GetHeight, 0 );
	
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
	
	rb_define_alias( globalRenderImageClass, "view=", "setView" );
	rb_define_alias( globalRenderImageClass, "view", "getView" );
	
	rb_define_alias( globalRenderImageClass, "defaultView", "getDefaultView" );
	rb_define_alias( globalRenderImageClass, "default_view", "getDefaultView" );
	
	rb_define_alias( globalRenderImageClass, "width", "getWidth" );
	rb_define_alias( globalRenderImageClass, "height", "getHeight" );
}

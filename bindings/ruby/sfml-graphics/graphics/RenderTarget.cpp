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
#include <SFML/Graphics/RenderImage.hpp>

VALUE globalRenderTargetModule;
VALUE globalRenderTargetInstanceClass;

/* External classes */
extern VALUE globalRenderWindowClass;
extern VALUE globalVector2Class;
extern VALUE globalRectClass;
extern VALUE globalDrawableModule;
extern VALUE globalShaderClass;
extern VALUE globalViewClass;
extern VALUE globalNonCopyableModule;

static VALUE View_Free( sf::View *anObject )
{
	delete anObject;
}

/* call-seq:
 *   render_target.clear( color = SFML::Color::Black )
 *
 * Clear the entire target with a single color.
 *
 * This function is usually called once every frame, to clear the previous contents of the target.
 */
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

/* call-seq:
 *   render_target.draw( drawable )
 *   render_target.draw( drawable, shader )
 *
 * Draw an object into the target with a shader.
 *
 * This function draws anything that inherits from the SFML::Drawable base class (SFML::Sprite, SFML::Shape, SFML::Text, 
 * or even your own derived classes). The shader alters the way that the pixels are processed right before being written 
 * to the render target.
 */
static VALUE RenderTarget_Draw( int argc, VALUE *args, VALUE self )
{
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	
	VALUE targetWrap = Qnil;
	sf::RenderWindow *window = NULL;
	sf::RenderImage *image = NULL;
		
	if( CLASS_OF( self ) == globalRenderWindowClass )
	{
		Data_Get_Struct( self, sf::RenderWindow, window );
	}
	else
	{
		Data_Get_Struct( self, sf::RenderImage, image );
	}
	
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
			( window != NULL ? window->Draw( *drawable, *shader ) : image->Draw( *drawable, *shader ) );
			break;
		}
		case 1:
		{
			VALIDATE_CLASS( args[0], globalDrawableModule, "object" );
			sf::Drawable *drawable = NULL;
			Data_Get_Struct( args[0], sf::Drawable, drawable );
			( window != NULL ? window->Draw( *drawable ) : image->Draw( *drawable ) );
			break;
		}
		default:
			rb_raise( rb_eArgError, "Expected 1 or 2 arguments but was given %d", argc );
	}
	return Qnil;
}

/* call-seq:
 *   render_target.getWidth()	-> fixnum
 *
 * Return the width of the rendering region of the target. 
 */
static VALUE RenderTarget_GetWidth( VALUE self )
{
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	return INT2FIX( object->GetWidth() );
}

/* call-seq:
 *   render_target.getHeight()	-> fixnum
 *
 * Return the height of the rendering region of the target. 
 */
static VALUE RenderTarget_GetHeight( VALUE self )
{
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	return INT2FIX( object->GetHeight() );
}

/* call-seq:
 *   render_target.setView( view )
 *
 * Change the current active view.
 *
 * The new view will affect everything that is drawn, until another view is activated. The render target keeps its own
 * copy of the view object, so it is not necessary to keep the original one alive as long as it is in use. To restore
 * the original view of the target, you can pass the result of getDefaultView() to this function.
 */
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

/* call-seq:
 *   render_target.getView()	-> view
 *
 * Retrieve the view currently in use in the render target. 
 */
static VALUE RenderTarget_GetView( VALUE self )
{
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	const sf::View &original = object->GetView();
	sf::View * view = new sf::View( original );
	VALUE rbData = Data_Wrap_Struct( globalViewClass, 0, View_Free, view );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

/* call-seq:
 *   render_target.getDefaultView()	-> VIEW
 *
 * Get the default view of the render target.
 *
 * The default view has the initial size of the render target, and never changes after the target has been created.
 */
static VALUE RenderTarget_GetDefaultView( VALUE self )
{
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	const sf::View &view = object->GetDefaultView();
	VALUE rbData = Data_Wrap_Struct( globalViewClass, 0, 0, const_cast< sf::View * >( &view ) );
	rb_obj_call_init( rbData, 0, 0 );
	rb_iv_set( rbData, "@__owner_ref", self );
	return rbData;
}

/* call-seq:
 *   render_target.getViewport( view )	-> rectangle
 *
 * Get the viewport of a view, applied to this render target.
 *
 * The viewport is defined in the view as a ratio, this function simply applies this ratio to the current dimensions 
 * of the render target to calculate the pixels rectangle that the viewport actually covers in the target.
 */
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

/* call-seq:
 *   render_target.convertCoords( x, y )	-> vector2
 *   render_target.convertCoords( x, y, view )	-> vector2
 *
 * Convert a point from target coordinates to view coordinates.
 *
 * Initially, a unit of the 2D world matches a pixel of the render target. But if you define a custom view, this 
 * assertion is not true anymore, ie. a point located at (10, 50) in your render target (for example a window) may 
 * map to the point (150, 75) in your 2D world -- for example if the view is translated by (140, 25).
 *
 * For render windows, this function is typically used to find which point (or object) is located below the mouse cursor.
 *
 */
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

/* call-seq:
 *   render_target.saveGLStates()
 *
 * Save the current OpenGL render states and matrices.
 *
 * This function can be used when you mix SFML drawing and direct OpenGL rendering. Combined with RestoreGLStates, 
 * it ensures that:
 *
 *   - SFML's internal states are not messed up by your OpenGL code
 *   - your OpenGL states are not modified by a call to a SFML function
 *
 * More specifically, it must be used around code that calls Draw functions. Example:
 *
 *   # OpenGL code here...
 *   window.saveGLStates()
 *   window.draw(...)
 *   window.draw(...)
 *   window.restoreGLStates()
 *   # OpenGL code here...
 *
 * Note that this function is quite expensive and should be used wisely. It is provided for convenience, and the 
 * best results will be achieved if you handle OpenGL states yourself (because you really know which states have 
 * really changed, and need to be saved / restored).
 */
static VALUE RenderTarget_SaveGLStates( VALUE self )
{
	sf::RenderTarget *object = NULL;
	Data_Get_Struct( self, sf::RenderTarget, object );
	object->SaveGLStates();
	return Qnil;
}

/* call-seq:
 *   render_target.restoreGLStates()
 *
 * Restore the previously saved OpenGL render states and matrices.
 *
 * See the description of saveGLStates to get a detailed description of these functions.
 */
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
 	globalRenderTargetModule = rb_define_module_under( sfml, "RenderTarget" );
	rb_include_module( globalRenderTargetModule, globalNonCopyableModule );
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

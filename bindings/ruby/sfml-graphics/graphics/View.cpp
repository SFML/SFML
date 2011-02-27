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
 
#include "View.hpp"
#include "Vector2.hpp"
#include "Rect.hpp"
#include "main.hpp"
#include <SFML/Graphics/View.hpp>

VALUE globalViewClass;
/* External classes */
extern VALUE globalRectClass;
extern VALUE globalVector2Class;

static void View_Free( sf::View *anObject )
{
	delete anObject;
}

/* call-seq:
 *   View.new()			-> view
 *   View.new( rectangle )	-> view
 *   View.new( center, size )	-> view
 *
 * Construct a view.
 */
static VALUE View_Initialize( int argc, VALUE *args, VALUE self )
{
	VALUE temp = Qnil;
	sf::View *object = NULL;
	Data_Get_Struct( self, sf::View, object );
	
	switch( argc )
	{
		case 2:
		{
			temp = Vector2_ForceType( args[0] );
			sf::Vector2f center;
			center.x = NUM2DBL( Vector2_GetX( temp ) );
			center.y = NUM2DBL( Vector2_GetY( temp ) );
			temp = Vector2_ForceType( args[1] );
			sf::Vector2f size;
			size.x = NUM2DBL( Vector2_GetX( temp ) );
			size.y = NUM2DBL( Vector2_GetY( temp ) );
			object->SetCenter( center );
			object->SetSize( size );
			break;
		}
		case 1:
		{
			temp = Rect_ForceType( args[0] );
			sf::FloatRect rectangle;
			rectangle.Left   = NUM2DBL( Rect_GetLeft( temp ) );
			rectangle.Top    = NUM2DBL( Rect_GetTop( temp ) );
			rectangle.Width  = NUM2DBL( Rect_GetWidth( temp ) );
			rectangle.Height = NUM2DBL( Rect_GetHeight( temp ) );
			object->Reset( rectangle );
			break;
		}
		case 0:
			// Do nothing
			break;
		default:
			rb_raise( rb_eArgError, "Expected 0..2 arguments but was given %d", argc );
	}
	
	return self;
}

static VALUE View_InitializeCopy( VALUE self, VALUE aSource )
{
	sf::View *object = NULL;
	Data_Get_Struct( self, sf::View, object );
	sf::View *source = NULL;
	Data_Get_Struct( aSource, sf::View, source );
	*object = *source;
}

/* call-seq:
 *   view.getCenter()	-> vector2
 *
 * Get the center of the view. 
 */
static VALUE View_GetCenter( VALUE self )
{
	sf::View *object = NULL;
	Data_Get_Struct( self, sf::View, object );
	const sf::Vector2f& center = object->GetCenter();
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, rb_float_new( center.x ), rb_float_new( center.y ) );
}

/* call-seq:
 *   view.getRotation()	-> float
 *
 * Get the current orientation of the view. 
 */
static VALUE View_GetRotation( VALUE self )
{
	sf::View *object = NULL;
	Data_Get_Struct( self, sf::View, object );
	return rb_float_new( object->GetRotation() );
}

/* call-seq:
 *   view.getSize()	-> vector2
 *
 * Get the size of the view.
 */
static VALUE View_GetSize( VALUE self )
{
	sf::View *object = NULL;
	Data_Get_Struct( self, sf::View, object );
	const sf::Vector2f& size = object->GetSize();
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, rb_float_new( size.x ), rb_float_new( size.y ) );
}

/* call-seq:
 *   view.getViewport()	-> rectangle
 *
 * Get the target viewport rectangle of the view. 
 */
static VALUE View_GetViewport( VALUE self )
{
	sf::View *object = NULL;
	Data_Get_Struct( self, sf::View, object );
	const sf::FloatRect& viewport = object->GetViewport();
	return rb_funcall( globalRectClass, rb_intern( "new" ), 4, 
				rb_float_new( viewport.Left ), rb_float_new( viewport.Top ),
				rb_float_new( viewport.Width ), rb_float_new( viewport.Height ) );
}

/* call-seq:
 *   view.move( x, y )
 *   view.move( offset )
 *
 * Move the view relatively to its current position. 
 */
static VALUE View_Move( int argc, VALUE * args, VALUE self )
{
	float offsetX = 0;
	float offsetY = 0;
	
	switch( argc )
	{
		case 1:
		{
			VALUE temp = Vector2_ForceType( args[0] );
			offsetX = NUM2DBL( Vector2_GetX( temp ) );
			offsetY = NUM2DBL( Vector2_GetY( temp ) );
			break;
		}
		case 2:
		{
			offsetX = NUM2DBL( args[0] );
			offsetY = NUM2DBL( args[1] );
			break;
		}
		default:
			rb_raise( rb_eArgError, "Expected 1 or 2 arguments but was given %d", argc );
	}
	
	sf::View *object = NULL;
	Data_Get_Struct( self, sf::View, object );
	object->Move( offsetX, offsetY );
	return Qnil;
}

/* call-seq:
 *   view.reset( rectangle )
 *
 * Reset the view to the given rectangle.
 *
 * Note that this function resets the rotation angle to 0.
 */
static VALUE View_Reset( VALUE self, VALUE aRectangle )
{
	VALUE temp = Rect_ForceType( aRectangle );
	sf::View *object = NULL;
	Data_Get_Struct( self, sf::View, object );
	sf::FloatRect rectangle;
	rectangle.Left   = NUM2DBL( Rect_GetLeft( temp ) );
	rectangle.Top    = NUM2DBL( Rect_GetTop( temp ) );
	rectangle.Width  = NUM2DBL( Rect_GetWidth( temp ) );
	rectangle.Height = NUM2DBL( Rect_GetHeight( temp ) );
	object->Reset( rectangle );
	return Qnil;
}

/* call-seq:
 *   view.rotate( angle )
 *
 * Rotate the view relatively to its current orientation. 
 */
static VALUE View_Rotate( VALUE self, VALUE anAngle )
{
	sf::View *object = NULL;
	Data_Get_Struct( self, sf::View, object );
	object->Rotate( NUM2DBL( anAngle ) );
	return Qnil;
}

/* call-seq:
 *   view.setCenter( center )
 *   view.setCenter( x, y )
 *
 * Set the center of the view. 
 */
static VALUE View_SetCenter( int argc, VALUE * args, VALUE self )
{
	float x = 0;
	float y = 0;
	
	switch( argc )
	{
		case 1:
		{
			VALUE temp = Vector2_ForceType( args[0] );
			x = NUM2DBL( Vector2_GetX( temp ) );
			y = NUM2DBL( Vector2_GetY( temp ) );
			break;
		}
		case 2:
		{
			x = NUM2DBL( args[0] );
			y = NUM2DBL( args[1] );
			break;
		}
		default:
			rb_raise( rb_eArgError, "Expected 1 or 2 arguments but was given %d", argc );
	}
	
	sf::View *object = NULL;
	Data_Get_Struct( self, sf::View, object );
	object->SetCenter( x, y );
	return Qnil;
}

/* call-seq:
 *   view.setRotation( angle )
 *
 * Set the orientation of the view.
 *
 * The default rotation of a view is 0 degree.
 */
static VALUE View_SetRotation( VALUE self, VALUE anAngle )
{
	sf::View *object = NULL;
	Data_Get_Struct( self, sf::View, object );
	object->SetRotation( NUM2DBL( anAngle ) );
	return Qnil;
}

/* call-seq:
 *   view.setSize( size )
 *   view.setSize( width, height )
 *
 * Set the center of the view. 
 */
static VALUE View_SetSize( int argc, VALUE * args, VALUE self )
{
	float x = 0;
	float y = 0;
	
	switch( argc )
	{
		case 1:
		{
			VALUE temp = Vector2_ForceType( args[0] );
			x = NUM2DBL( Vector2_GetX( temp ) );
			y = NUM2DBL( Vector2_GetY( temp ) );
			break;
		}
		case 2:
		{
			x = NUM2DBL( args[0] );
			y = NUM2DBL( args[1] );
			break;
		}
		default:
			rb_raise( rb_eArgError, "Expected 1 or 2 arguments but was given %d", argc );
	}
	
	sf::View *object = NULL;
	Data_Get_Struct( self, sf::View, object );
	object->SetSize( x, y );
	return Qnil;
}

/* call-seq:
 *   view.setViewport( rectangle )
 *
 * Set the target viewport.
 *
 * The viewport is the rectangle into which the contents of the view are displayed, expressed as a factor 
 * (between 0 and 1) of the size of the RenderTarget to which the view is applied. For example, a view which takes the
 * left side of the target would be defined with SFML::View.setViewport( [0.0, 0.0, 0.5, 1.0] ). By default, a view has
 * a viewport which covers the entire target.
 */
static VALUE View_SetViewport( VALUE self, VALUE aRectangle )
{
	VALUE temp = Rect_ForceType( aRectangle );
	sf::View *object = NULL;
	Data_Get_Struct( self, sf::View, object );
	sf::FloatRect viewport;
	viewport.Left   = NUM2DBL( Rect_GetLeft( temp ) );
	viewport.Top    = NUM2DBL( Rect_GetTop( temp ) );
	viewport.Width  = NUM2DBL( Rect_GetWidth( temp ) );
	viewport.Height = NUM2DBL( Rect_GetHeight( temp ) );
	object->SetViewport( viewport );
	return Qnil;
}

/* call-seq:
 *   view.zoom( factor )
 *
 * Resize the view rectangle relatively to its current size.
 *
 * Resizing the view simulates a zoom, as the zone displayed on screen grows or shrinks. factor is a multiplier:
 *
 *   - 1 keeps the size unchanged
 *   - > 1 makes the view bigger (objects appear smaller)
 *   - < 1 makes the view smaller (objects appear bigger)
 *
 */
static VALUE View_Zoom( VALUE self, VALUE aFactor )
{
	sf::View *object = NULL;
	Data_Get_Struct( self, sf::View, object );
	object->Zoom( NUM2DBL( aFactor ) );
	return Qnil;
}

static VALUE View_Alloc( VALUE aKlass )
{
	sf::View *object = new sf::View();
	return Data_Wrap_Struct( aKlass, 0, View_Free, object );
}

void Init_View( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* 2D camera that defines what region is shown on screen
 *
 * sf::View defines a camera in the 2D scene.
 *
 * This is a very powerful concept: you can scroll, rotate or zoom the entire scene without altering the way that your
 * drawable objects are drawn.
 * 
 * A view is composed of a source rectangle, which defines what part of the 2D scene is shown, and a target viewport,
 * which defines where the contents of the source rectangle will be displayed on the render target 
 * (window or render-image).
 *
 * The viewport allows to map the scene to a custom part of the render target, and can be used for split-screen or 
 * for displaying a minimap, for example. If the source rectangle has not the same size as the viewport, its contents
 * will be stretched to fit in.
 *
 * To apply a view, you have to assign it to the render target. Then, every objects drawn in this render target will 
 * be affected by the view until you use another view.
 * 
 * Usage example:
 *
 *   window = SFML::RenderWindow.new
 *   view = SFML::View.new
 * 
 *   # Initialize the view to a rectangle located at (100, 100) and with a size of 400x200
 *   view.reset( [100.0, 100.0, 400.0, 200.0] )
 *
 *   # Rotate it by 45 degrees
 *   view.rotate( 45 )
 *
 *   # Set its target viewport to be half of the window
 *   view.viewport = [0.0, 0.0, 0.5, 1.0 ]
 *
 *   # Apply it
 *   window.view = view
 *
 *   # Render stuff
 *   window.draw( someSprite )
 *
 *   # Set the default view back
 *   window.view = window.defaultView
 *
 *   # Render stuff not affected by the view
 *   window.draw( someText )
 */
	globalViewClass = rb_define_class_under( sfml, "View", rb_cObject );
	
	// Class methods
	//rb_define_singleton_method( globalViewClass, "new", View_New, -1 );
	rb_define_alloc_func( globalViewClass, View_Alloc );
		
	// Instance methods
	rb_define_method( globalViewClass, "initialize", View_Initialize, -1 );
	rb_define_method( globalViewClass, "initialize_copy", View_InitializeCopy, 1 );
	rb_define_method( globalViewClass, "setCenter", View_SetCenter, -1 );
	rb_define_method( globalViewClass, "setSize", View_SetSize, -1 );
	rb_define_method( globalViewClass, "setRotation", View_SetRotation, 1 );
	rb_define_method( globalViewClass, "setViewport", View_SetViewport, 1 );
	rb_define_method( globalViewClass, "reset", View_Reset, 1 );
	rb_define_method( globalViewClass, "getCenter", View_GetCenter, 0 );
	rb_define_method( globalViewClass, "getSize", View_GetSize, 0 );
	rb_define_method( globalViewClass, "getRotation", View_GetRotation, 0 );
	rb_define_method( globalViewClass, "getViewport", View_GetViewport, 0 );
	rb_define_method( globalViewClass, "move", View_Move, -1 );
	rb_define_method( globalViewClass, "rotate", View_Rotate, 1 );
	rb_define_method( globalViewClass, "zoom", View_Zoom, 1 );
	
	// Instance Aliases
	rb_define_alias( globalViewClass, "center=", "setCenter" );
	rb_define_alias( globalViewClass, "center", "getCenter" );
	
	rb_define_alias( globalViewClass, "size=", "setSize" );
	rb_define_alias( globalViewClass, "size", "getSize" );
	
	rb_define_alias( globalViewClass, "rotation=", "setRotation" );
	rb_define_alias( globalViewClass, "rotation", "getRotation" );
	
	rb_define_alias( globalViewClass, "viewport=", "setViewport" );
	rb_define_alias( globalViewClass, "viewport", "getViewport" );
}

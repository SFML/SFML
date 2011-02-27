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
 
#include "Shape.hpp"
#include "Vector2.hpp"
#include "Rect.hpp"
#include "Color.hpp"
#include "main.hpp"
#include <SFML/Graphics/Shape.hpp>

VALUE globalShapeClass;
/* External classes */
extern VALUE globalVector2Class;
extern VALUE globalDrawableModule;
extern VALUE globalColorClass;

static void Shape_Free( sf::Shape *anObject )
{
	delete anObject;
}

/* call-seq:
 *   shape.addPoint( x, y, color, outlineColor )
 *   shape.addPoint( position, color, outlineColor )
 *
 * Add a new point to the shape.
 *
 * The new point is inserted at the end of the shape.
 */
static VALUE Shape_AddPoint( int argc, VALUE *args, VALUE self )
{
	VALUE temp = Qnil;
	float x = 0;
	float y = 0;
	sf::Color color = sf::Color::White;
	sf::Color outlineColor = sf::Color::Black;
	
	if( argc > 0 && rb_obj_is_kind_of( args[0], rb_cFloat ) == Qtrue )
	{
		switch( argc )
		{
			case 4:
				temp = Color_ForceType( args[3] );
				outlineColor.r = INT2FIX( Color_GetR( temp ) );
				outlineColor.g = INT2FIX( Color_GetG( temp ) );
				outlineColor.b = INT2FIX( Color_GetB( temp ) );
				outlineColor.a = INT2FIX( Color_GetA( temp ) );
			case 3:
				temp = Color_ForceType( args[2] );
				color.r = INT2FIX( Color_GetR( temp ) );
				color.g = INT2FIX( Color_GetG( temp ) );
				color.b = INT2FIX( Color_GetB( temp ) );
				color.a = INT2FIX( Color_GetA( temp ) );
			case 2:
				x = NUM2DBL( args[0] );
				y = NUM2DBL( args[1] );
			default:
				rb_raise( rb_eArgError, "Expected 2..4 arguments but was given %d", argc );
				
		}
	}
	else
	{
		switch( argc )
		{
			case 3:
				temp = Color_ForceType( args[2] );
				outlineColor.r = INT2FIX( Color_GetR( temp ) );
				outlineColor.g = INT2FIX( Color_GetG( temp ) );
				outlineColor.b = INT2FIX( Color_GetB( temp ) );
				outlineColor.a = INT2FIX( Color_GetA( temp ) );
			case 2:
				temp = Color_ForceType( args[1] );
				color.r = INT2FIX( Color_GetR( temp ) );
				color.g = INT2FIX( Color_GetG( temp ) );
				color.b = INT2FIX( Color_GetB( temp ) );
				color.a = INT2FIX( Color_GetA( temp ) );
			case 1:
				temp = Vector2_ForceType( args[0] );
				x = NUM2DBL( Vector2_GetX( temp ) );
				y = NUM2DBL( Vector2_GetY( temp ) );
			default:
				rb_raise( rb_eArgError, "Expected 1..3 arguments but was given %d", argc );
				
		}
	}
	
	sf::Shape *object = NULL;
	Data_Get_Struct( self, sf::Shape, object );
	object->AddPoint( x, y, color, outlineColor );
	return Qnil;
}

/* call-seq:
 *   shape.getPointsCount()	-> fixnum
 *
 * Get the number of points composing the shape.
 */
static VALUE Shape_GetPointsCount( VALUE self )
{
	sf::Shape *object = NULL;
	Data_Get_Struct( self, sf::Shape, object );
	return INT2FIX( object->GetPointsCount() );
}

/* call-seq:
 *   shape.enableFill( enable )
 *
 * Enable or disable the shape's filling.
 *
 * This option is enabled by default.
 */
static VALUE Shape_EnableFill( VALUE self, VALUE anEnableFlag )
{
	sf::Shape *object = NULL;
	Data_Get_Struct( self, sf::Shape, object );
	if( anEnableFlag == Qtrue )
	{
		object->EnableFill( true );
	}
	else if( anEnableFlag == Qfalse )
	{
		object->EnableFill( false );
	}
	else
	{
		VALIDATE_CLASS( anEnableFlag, rb_cTrueClass, "enable" );
	}
	return Qnil;
}

/* call-seq:
 *   shape.enableOutline( enable )
 *
 * Enable or disable the shape's outline.
 *
 * This option is enabled by default.
 */
static VALUE Shape_EnableOutline( VALUE self, VALUE anEnableFlag )
{
	sf::Shape *object = NULL;
	Data_Get_Struct( self, sf::Shape, object );
	if( anEnableFlag == Qtrue )
	{
		object->EnableOutline( true );
	}
	else if( anEnableFlag == Qfalse )
	{
		object->EnableOutline( false );
	}
	else
	{
		VALIDATE_CLASS( anEnableFlag, rb_cTrueClass, "enable" );
	}
	return Qnil;
}

/* call-seq:
 *   shape.setPointPosition( index, position )
 *   shape.setPointPosition( index, x, y )
 *
 * Change the position of a point.
 *
 * Warning: this function doesn't check the validity of index, if it is out of bounds (ie. in the range 
 * [0, GetPointscount() - 1]) the behaviour is undefined.
 */
static VALUE Shape_SetPointPosition( int argc, VALUE *args, VALUE self )
{
	VALUE temp = Qnil;
	float x = 0;
	float y = 0;
	
	switch( argc )
	{
		case 2:
			temp = Vector2_ForceType( args[1] );
			x = NUM2DBL( Vector2_GetX( temp ) );
			y = NUM2DBL( Vector2_GetY( temp ) );
			break;
		case 3:
			x = NUM2DBL( args[1] );
			y = NUM2DBL( args[2] );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 2..3 arguments but was given %d", argc );
	}
	sf::Shape *object = NULL;
	Data_Get_Struct( self, sf::Shape, object );
	object->SetPointPosition( FIX2UINT( args[0] ), x, y );
	return Qnil;
}

/* call-seq:
 *   shape.setPointColor( index, color )
 *
 * Change the color of a point.
 *
 * Warning: this function doesn't check the validity of index, if it is out of bounds (ie. in the range 
 * [0, GetPointscount() - 1]) the behaviour is undefined.
 */
static VALUE Shape_SetPointColor( VALUE self, VALUE anIndex, VALUE aColor )
{
	sf::Shape *object = NULL;
	Data_Get_Struct( self, sf::Shape, object );
	VALUE temp = Color_ForceType( aColor );
	sf::Color color;
	color.r = INT2FIX( Color_GetR( temp ) );
	color.g = INT2FIX( Color_GetG( temp ) );
	color.b = INT2FIX( Color_GetB( temp ) );
	color.a = INT2FIX( Color_GetA( temp ) );
	object->SetPointColor( FIX2UINT( anIndex ), color );
	return Qnil;
}

/* call-seq:
 *   shape.setPointOutlineColor( index, color )
 *
 * Change the outline color of a point.
 *
 * Warning: this function doesn't check the validity of index, if it is out of bounds (ie. in the range 
 * [0, GetPointscount() - 1]) the behaviour is undefined.
 */
static VALUE Shape_SetPointOutlineColor( VALUE self, VALUE anIndex, VALUE aColor )
{
	sf::Shape *object = NULL;
	Data_Get_Struct( self, sf::Shape, object );
	VALUE temp = Color_ForceType( aColor );
	sf::Color color;
	color.r = INT2FIX( Color_GetR( temp ) );
	color.g = INT2FIX( Color_GetG( temp ) );
	color.b = INT2FIX( Color_GetB( temp ) );
	color.a = INT2FIX( Color_GetA( temp ) );
	object->SetPointOutlineColor( FIX2UINT( anIndex ), color );
	return Qnil;
}

/* call-seq:
 *   shape.setOutlineThickness( width )
 *
 * Change the thickness of the shape outline. 
 */
static VALUE Shape_SetOutlineThickness( VALUE self, VALUE aWidth )
{
	sf::Shape *object = NULL;
	Data_Get_Struct( self, sf::Shape, object );
	object->SetOutlineThickness( NUM2DBL( aWidth ) );
	return Qnil;
}

/* call-seq:
 *   shape.getPointPosition( index )	-> vector2
 *
 * Get the position of a point.
 *
 * Warning: this function doesn't check the validity of index, if it is out of bounds (ie. in the range 
 * [0, GetPointscount() - 1]) the behaviour is undefined.
 */
static VALUE Shape_GetPointPosition( VALUE self, VALUE anIndex )
{
	sf::Shape *object = NULL;
	Data_Get_Struct( self, sf::Shape, object );
	const sf::Vector2f &vector = object->GetPointPosition( FIX2UINT( anIndex ) );
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, rb_float_new( vector.x ), rb_float_new( vector.y ) );
}

/* call-seq:
 *   shape.getPointColor( index )	-> color
 *
 * Get the color of a point.
 *
 * Warning: this function doesn't check the validity of index, if it is out of bounds (ie. in the range 
 * [0, GetPointscount() - 1]) the behaviour is undefined.
 */
static VALUE Shape_GetPointColor( VALUE self, VALUE anIndex )
{
	sf::Shape *object = NULL;
	Data_Get_Struct( self, sf::Shape, object );
	const sf::Color &color = object->GetPointColor( FIX2UINT( anIndex ) );
	return rb_funcall( globalColorClass, rb_intern( "new" ), 4, 
				INT2FIX( color.r ), INT2FIX( color.g ),
				INT2FIX( color.b ), INT2FIX( color.a ) );
}

/* call-seq:
 *   shape.getPointOutlineColor( index )	-> color
 *
 * Get the outline color of a point.
 *
 * Warning: this function doesn't check the validity of index, if it is out of bounds (ie. in the range 
 * [0, GetPointscount() - 1]) the behaviour is undefined.
 */
static VALUE Shape_GetPointOutlineColor( VALUE self, VALUE anIndex )
{
	sf::Shape *object = NULL;
	Data_Get_Struct( self, sf::Shape, object );
	const sf::Color &color = object->GetPointOutlineColor( FIX2UINT( anIndex ) );
	return rb_funcall( globalColorClass, rb_intern( "new" ), 4, 
				INT2FIX( color.r ), INT2FIX( color.g ),
				INT2FIX( color.b ), INT2FIX( color.a ) );
}

/* call-seq:
 *   shape.getOutlineThickness()	-> float
 *
 * Get the thickness of the shape outline. 
 */
static VALUE Shape_GetOutlineThickness( VALUE self )
{
	sf::Shape *object = NULL;
	Data_Get_Struct( self, sf::Shape, object );
	return rb_float_new( object->GetOutlineThickness() );
}

static VALUE Shape_InitializeCopy( VALUE self, VALUE aSource )
{
	sf::Shape *object = NULL;
	Data_Get_Struct( self, sf::Shape, object );
	sf::Shape *source = NULL;
	Data_Get_Struct( aSource, sf::Shape, source );
	*object = *source;
}

/* call-seq:
 *   Shape.new()	-> shape
 *
 * Create an empty shape.
 */
static VALUE Shape_Alloc( VALUE aKlass )
{
	sf::Shape *object = new sf::Shape();
	return Data_Wrap_Struct( aKlass, 0, Shape_Free, object );
}

/* call-seq:
 *   Shape.line( p1x, p1y, p2x, p2y, thickness, color, outline = 0.0, outlineColor = SFML::Color::Black)	-> shape
 *   Shape.line( start, end, thickness, color, outline = 0.0, outlineColor = SFML::Color::Black)		-> shape
 *
 * Create a new line.
 */
static VALUE Shape_Line( int argc, VALUE *args, VALUE aKlass )
{
	VALUE temp = Qnil;
	float p1x = 0, p1y = 0;
	float p2x = 0, p2y = 0;
	float thickness = 0;
	sf::Color color;
	float outline = 0.0;
	sf::Color outlineColor = sf::Color::Black;
	
	if( argc > 0 && rb_obj_is_kind_of( args[0], rb_cFloat ) == Qtrue )
	{
		switch( argc )
		{
			case 8:
				temp = Color_ForceType( args[7] );
				outlineColor.r = INT2FIX( Color_GetR( temp ) );
				outlineColor.g = INT2FIX( Color_GetG( temp ) );
				outlineColor.b = INT2FIX( Color_GetB( temp ) );
				outlineColor.a = INT2FIX( Color_GetA( temp ) );
			case 7:
				outline = NUM2DBL( args[6] );
			case 6:
				p1x = NUM2DBL( args[0] ); p1y = NUM2DBL( args[1] );
				p2x = NUM2DBL( args[2] ); p2y = NUM2DBL( args[3] );
				thickness = NUM2DBL( args[4] );
				temp = Color_ForceType( args[5] );
				color.r = INT2FIX( Color_GetR( temp ) );
				color.g = INT2FIX( Color_GetG( temp ) );
				color.b = INT2FIX( Color_GetB( temp ) );
				color.a = INT2FIX( Color_GetA( temp ) );
				break;
			default:
				rb_raise( rb_eArgError, "Expected 6..8 arguments but was given %d", argc );
		}
	}
	else
	{
		switch( argc )
		{
			case 6:
				temp = Color_ForceType( args[5] );
				outlineColor.r = INT2FIX( Color_GetR( temp ) );
				outlineColor.g = INT2FIX( Color_GetG( temp ) );
				outlineColor.b = INT2FIX( Color_GetB( temp ) );
				outlineColor.a = INT2FIX( Color_GetA( temp ) );
			case 5:
				outline = NUM2DBL( args[4] );
			case 4:
				temp = Vector2_ForceType( args[0] );
				p1x = NUM2DBL( Vector2_GetX( temp ) ); p1y = NUM2DBL( Vector2_GetY( temp ) );
				temp = Vector2_ForceType( args[1] );
				p2x = NUM2DBL( Vector2_GetX( temp ) ); p2y = NUM2DBL( Vector2_GetY( temp ) );
				thickness = NUM2DBL( args[2] );
				temp = Color_ForceType( args[3] );
				color.r = INT2FIX( Color_GetR( temp ) );
				color.g = INT2FIX( Color_GetG( temp ) );
				color.b = INT2FIX( Color_GetB( temp ) );
				color.a = INT2FIX( Color_GetA( temp ) );
				break;
			default:
				rb_raise( rb_eArgError, "Expected 6..8 arguments but was given %d", argc );
		}
	}
	
	sf::Shape * shape = new sf::Shape( sf::Shape::Line( p1x, p1y, p2x, p2y, thickness, color, outline, outlineColor ) );
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, Shape_Free, shape );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

/* call-seq:
 *   Shape.rectangle( left, top, width, height, color, outline = 0.0, outlineColor = SFML::Color::Black)	-> shape
 *   Shape.rectangle( rectangle, color, outline = 0.0, outlineColor = SFML::Color::Black)			-> shape
 *
 * Create a new rectangular shape. 
 */
static VALUE Shape_Rectangle( int argc, VALUE *args, VALUE aKlass )
{
	VALUE temp = Qnil;
	float p1x = 0, p1y = 0;
	float p2x = 0, p2y = 0;
	sf::Color color;
	float outline = 0.0;
	sf::Color outlineColor = sf::Color::Black;
	
	if( argc > 0 && rb_obj_is_kind_of( args[0], rb_cFloat ) == Qtrue )
	{
		switch( argc )
		{
			case 7:
				temp = Color_ForceType( args[6] );
				outlineColor.r = INT2FIX( Color_GetR( temp ) );
				outlineColor.g = INT2FIX( Color_GetG( temp ) );
				outlineColor.b = INT2FIX( Color_GetB( temp ) );
				outlineColor.a = INT2FIX( Color_GetA( temp ) );
			case 6:
				outline = NUM2DBL( args[5] );
			case 5:
				p1x = NUM2DBL( args[0] ); p1y = NUM2DBL( args[1] );
				p2x = NUM2DBL( args[2] ); p2y = NUM2DBL( args[3] );
				temp = Color_ForceType( args[4] );
				color.r = INT2FIX( Color_GetR( temp ) );
				color.g = INT2FIX( Color_GetG( temp ) );
				color.b = INT2FIX( Color_GetB( temp ) );
				color.a = INT2FIX( Color_GetA( temp ) );
				break;
			default:
				rb_raise( rb_eArgError, "Expected 5..7 arguments but was given %d", argc );
		}
	}
	else
	{
		switch( argc )
		{
			case 4:
				temp = Color_ForceType( args[3] );
				outlineColor.r = INT2FIX( Color_GetR( temp ) );
				outlineColor.g = INT2FIX( Color_GetG( temp ) );
				outlineColor.b = INT2FIX( Color_GetB( temp ) );
				outlineColor.a = INT2FIX( Color_GetA( temp ) );
			case 3:
				outline = NUM2DBL( args[2] );
			case 2:
				temp = Rect_ForceType( args[0] );
				p1x = NUM2DBL( Rect_GetLeft( temp ) ); p1y = NUM2DBL( Rect_GetTop( temp ) );
				p2x = NUM2DBL( Rect_GetWidth( temp ) ); p2y = NUM2DBL( Rect_GetHeight( temp ) );
				temp = Color_ForceType( args[1] );
				color.r = INT2FIX( Color_GetR( temp ) );
				color.g = INT2FIX( Color_GetG( temp ) );
				color.b = INT2FIX( Color_GetB( temp ) );
				color.a = INT2FIX( Color_GetA( temp ) );
				break;
			default:
				rb_raise( rb_eArgError, "Expected 2..4 arguments but was given %d", argc );
		}
	}
	
	sf::Shape * shape = new sf::Shape( sf::Shape::Rectangle( p1x, p1y, p2x, p2y, color, outline, outlineColor ) );
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, Shape_Free, shape );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

/* call-seq:
 *   Shape.circle( x, y, radius, color, outline = 0.0, outlineColor = SFML::Color::Black)	-> shape
 *   Shape.circle( center, radius, color, outline = 0.0, outlineColor = SFML::Color::Black)	-> shape
 *
 * Create a new circular shape.
 */
static VALUE Shape_Circle( int argc, VALUE *args, VALUE aKlass )
{
	VALUE temp = Qnil;
	float x = 0, y = 0;
	float radius = 0;
	sf::Color color;
	float outline = 0.0;
	sf::Color outlineColor = sf::Color::Black;
	
	if( argc > 0 && rb_obj_is_kind_of( args[0], rb_cFloat ) == Qtrue )
	{
		switch( argc )
		{
			case 6:
				temp = Color_ForceType( args[5] );
				outlineColor.r = INT2FIX( Color_GetR( temp ) );
				outlineColor.g = INT2FIX( Color_GetG( temp ) );
				outlineColor.b = INT2FIX( Color_GetB( temp ) );
				outlineColor.a = INT2FIX( Color_GetA( temp ) );
			case 5:
				outline = NUM2DBL( args[4] );
			case 4:
				x = NUM2DBL( args[0] ); y = NUM2DBL( args[1] );
				radius = NUM2DBL( args[2] );
				temp = Color_ForceType( args[3] );
				color.r = INT2FIX( Color_GetR( temp ) );
				color.g = INT2FIX( Color_GetG( temp ) );
				color.b = INT2FIX( Color_GetB( temp ) );
				color.a = INT2FIX( Color_GetA( temp ) );
				break;
			default:
				rb_raise( rb_eArgError, "Expected 4..6 arguments but was given %d", argc );
		}
	}
	else
	{
		switch( argc )
		{
			case 5:
				temp = Color_ForceType( args[4] );
				outlineColor.r = INT2FIX( Color_GetR( temp ) );
				outlineColor.g = INT2FIX( Color_GetG( temp ) );
				outlineColor.b = INT2FIX( Color_GetB( temp ) );
				outlineColor.a = INT2FIX( Color_GetA( temp ) );
			case 4:
				outline = NUM2DBL( args[3] );
			case 3:
				temp = Vector2_ForceType( args[0] );
				x = NUM2DBL( Vector2_GetX( temp ) ); 
				y = NUM2DBL( Vector2_GetY( temp ) );
				radius = NUM2DBL( args[1] );
				temp = Color_ForceType( args[2] );
				color.r = INT2FIX( Color_GetR( temp ) );
				color.g = INT2FIX( Color_GetG( temp ) );
				color.b = INT2FIX( Color_GetB( temp ) );
				color.a = INT2FIX( Color_GetA( temp ) );
				break;
			default:
				rb_raise( rb_eArgError, "Expected 3..5 arguments but was given %d", argc );
		}
	}
	
	sf::Shape * shape = new sf::Shape( sf::Shape::Circle( x, y, radius, color, outline, outlineColor ) );
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, Shape_Free, shape );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}


void Init_Shape( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* A convex, colored polygon with an optional outline.
 *
 * SFML::Shape is a drawable class that allows to define and display a custom convex shape on a render target.
 *
 * It is important to keep in mind that shapes must always be convex, otherwise they may not be drawn correctly. 
 * Moreover, the points must be added in the right order; using a random order would also result in an incorrect shape.
 *
 * A shape is made of points that have their own individual attributes:
 *
 *   - position (relative to the origin of the shape)
 *   - color
 *   - outline color
 *
 * Shapes have an outline that can be enabled or not. You can control the thickness of the outline with the 
 * setOutlineWidth function.
 *
 * They also inherits all the functions from SFML::Drawable: position, rotation, scale, origin, global color 
 * and blend mode.
 * 
 * Some static functions are provided to directly create common shapes such as lines, rectangles and circles:
 *
 *   line = SFML::Shape.line( start, end, thickness, color )
 *   rectangle = SFML::Shape.rectangle( rect, thickness )
 *   circle = SFML::Shape.circle( center, radius, color )
 *
 * A common mistake is to mix the individual points positions / colors and the global position / color of the shape. 
 * They are completely separate attributes that are combined when the shape is drawn (positions are added, colors are 
 * multiplied).
 *
 *   line = SFML::Shape.line( [100, 100], [200, 200], 10, SFML::Color::Red )
 *
 *   # --> line.getPosition() is (0, 0), *not* (100, 100)
 *   # --> line.getColor() is white, *not* red
 *
 * So if you plan to change the position / color of your shape after it is created, you'd better create the points 
 * around the origin and with white color, and use only the global position / color (SetPosition, SetColor).
 *
 * Usage example:
 * 
 *   # Create a shape
 *   shape = SFML::Shape.new
 *   
 *   # Define its points
 *   shape.addPoint( 10, 10, SFML::Color::White, SFML::Color::Red )
 *   shape.addPoint( 50, 10, SFML::Color::White, SFML::Color::Green )
 *   shape.addPoint( 10, 50, SFML::Color::White, SFML::Color::Blue )
 *
 *   # Enable outline only
 *   shape.enableFill( false )
 *   shape.enableOutline( true )
 *   shape.setOutlineWidth( 10 )
 *
 *   # Display it
 *   window.draw( shape ) # window is a SFML::RenderWindow
 *
 *   # Display static shapes
 *   window.draw( SFML::Shape.line( 0, 0, 10, 20, SFML::Color::Red ) )
 *   window.draw( SFML::Shape.rectangle( 100, 1000, 50, 20, SFML::Color::Green ) )
 *   window.draw( SFML::Shape.circle( 500, 500, 20, SFML::Color::Blue, 5, SFML::Color::Black ) )
 *
 */
	globalShapeClass = rb_define_class_under( sfml, "Shape", rb_cObject );
	rb_include_module( globalShapeClass, globalDrawableModule );
	
	// Class methods
	//rb_define_singleton_method( globalShapeClass, "new", Shape_New, -1 );
	rb_define_alloc_func( globalShapeClass, Shape_Alloc );
	rb_define_singleton_method( globalShapeClass, "line", Shape_Line, -1 );
	rb_define_singleton_method( globalShapeClass, "rectangle", Shape_Rectangle, -1 );
	rb_define_singleton_method( globalShapeClass, "circle", Shape_Circle, -1 );
	
	// Instance methods
	rb_define_method( globalShapeClass, "initialize_copy", Shape_InitializeCopy, 1 );
	rb_define_method( globalShapeClass, "addPoint", Shape_AddPoint, -1 );
	rb_define_method( globalShapeClass, "getPointsCount", Shape_GetPointsCount, 0 );
	rb_define_method( globalShapeClass, "enableFill", Shape_EnableFill, 1 );
	rb_define_method( globalShapeClass, "enableOutline", Shape_EnableOutline, 1 );
	rb_define_method( globalShapeClass, "setPointPosition", Shape_SetPointPosition, -1 );
	rb_define_method( globalShapeClass, "setPointColor", Shape_SetPointColor, 2 );
	rb_define_method( globalShapeClass, "setPointOutlineColor", Shape_SetPointOutlineColor, 2 );
	rb_define_method( globalShapeClass, "setOutlineThickness", Shape_SetOutlineThickness, 1 );
	rb_define_method( globalShapeClass, "getPointPosition", Shape_GetPointPosition, 1 );
	rb_define_method( globalShapeClass, "getPointColor", Shape_GetPointColor, 1 );
	rb_define_method( globalShapeClass, "getPointOutlineColor", Shape_GetPointOutlineColor, 1 );
	rb_define_method( globalShapeClass, "getOutlineThickness", Shape_GetOutlineThickness, 0 );
	
	// Instance Aliases
	rb_define_alias( globalShapeClass, "add_point", "addPoint" );
	rb_define_alias( globalShapeClass, "pointsCount", "getPointsCount" );
	rb_define_alias( globalShapeClass, "points_count", "getPointsCount" );
	rb_define_alias( globalShapeClass, "enable_fill", "enableFill" );
	rb_define_alias( globalShapeClass, "fill=", "enableFill" );
	rb_define_alias( globalShapeClass, "enable_outline", "enableOutline" );
	rb_define_alias( globalShapeClass, "outline=", "enableOutline" );
	rb_define_alias( globalShapeClass, "set_point_position", "setPointPosition" );
	rb_define_alias( globalShapeClass, "set_point_color", "setPointColor" );
	rb_define_alias( globalShapeClass, "set_point_outline_color", "setPointOutlineColor" );
	rb_define_alias( globalShapeClass, "outlineThickness=", "setOutlineThickness" );
	rb_define_alias( globalShapeClass, "outline_thickness=", "setOutlineThickness" );
	rb_define_alias( globalShapeClass, "outlineThickness", "getOutlineThickness" );
	rb_define_alias( globalShapeClass, "outline_thickness", "getOutlineThickness" );
	rb_define_alias( globalShapeClass, "get_point_position", "getPointPosition" );
	rb_define_alias( globalShapeClass, "get_point_color", "getPointColor" );
	rb_define_alias( globalShapeClass, "get_point_outline_color", "getPointOutlineColor" );
}

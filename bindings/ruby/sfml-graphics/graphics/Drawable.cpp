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
 
#include "Drawable.hpp"
#include "Vector2.hpp"
#include "Color.hpp"
#include "main.hpp"
#include <SFML/Graphics/Drawable.hpp>

VALUE globalDrawableModule;

/* External classes */
extern VALUE globalVector2Class;
extern VALUE globalColorClass;
extern VALUE globalRenderTargetInstanceClass;
extern VALUE globalRendererClass;

class rbDrawable : public sf::Drawable
{
public:
	void Init( VALUE aSelf )
	{
		myRubySelf = aSelf;
		myRenderID = rb_intern( "render" );
	}
	
protected:
	VALUE myRubySelf;
	ID myRenderID;
	
	virtual void Render( sf::RenderTarget& aTarget, sf::Renderer& aRenderer ) const
	{
		VALUE targetWrap = Data_Wrap_Struct( globalRenderTargetInstanceClass, 0, 0, &aTarget );
		VALUE rendererWrap = Data_Wrap_Struct( globalRendererClass, 0, 0, &aRenderer );
		rb_funcall( myRubySelf, myRenderID, 2, targetWrap, rendererWrap );
	}
};

static void Drawable_Free( rbDrawable *object )
{
	delete object;
}

/* call-seq:
 *   drawable.setPosition( x, y )
 *   drawable.setPosition( vector2 )
 *
 * Set the position of the object.
 *
 * This function completely overwrites the previous position. See Move to apply an offset based on the previous 
 * position instead. The default position of a drawable object is (0, 0).
 */
static VALUE Drawable_SetPosition( int argc, VALUE *args, VALUE self )
{
	rbDrawable *object = NULL;
	VALUE arg0 = Qnil;
	float positionX = 0.0f;
	float positionY = 0.0f;
	switch( argc )
	{
		case 1:
			arg0 = Vector2_ForceType( args[0] );
			positionX = NUM2DBL( Vector2_GetX( arg0 ) );
			positionY = NUM2DBL( Vector2_GetY( arg0 ) );
			break;
		case 2:
			positionX = NUM2DBL( args[0] );
			positionY = NUM2DBL( args[1] );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 1 or 2 arguments but was given %d", argc );
	}
	
	Data_Get_Struct( self, rbDrawable, object );
	object->SetPosition( positionX, positionY );
	
	return Qnil;
}

/* call-seq:
 *   drawable.setX( x )
 *
 * Set the X position of the object.
 */
static VALUE Drawable_SetX( VALUE self, VALUE aX )
{
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	object->SetX( NUM2DBL( aX ) );
	return Qnil;
}

/* call-seq:
 *   drawable.setY( y )
 *
 * Set the Y position of the object.
 */
static VALUE Drawable_SetY( VALUE self, VALUE aY )
{
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	object->SetY( NUM2DBL( aY ) );
	return Qnil;
}

/* call-seq:
 *   drawable.setScale( x, y )
 *   drawable.setScale( vector2 )
 *
 * Set the scale factors of the object.
 *
 * scale.x and scale.y must be strictly positive, otherwise they are ignored. This function completely overwrites 
 * the previous scale. See Scale to add a factor based on the previous scale instead. The default scale of a drawable 
 * object is (1, 1).
 */
static VALUE Drawable_SetScale( int argc, VALUE *args, VALUE self )
{
	rbDrawable *object = NULL;
	VALUE arg0 = Qnil;
	float scaleX = 0.0f;
	float scaleY = 0.0f;
	switch( argc )
	{
		case 1:
			arg0 = Vector2_ForceType( args[0] );
			scaleX = NUM2DBL( Vector2_GetX( arg0 ) );
			scaleY = NUM2DBL( Vector2_GetY( arg0 ) );
			break;
		case 2:
			scaleX = NUM2DBL( args[0] );
			scaleY = NUM2DBL( args[1] );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 1 or 2 arguments but was given %d", argc );
	}
	
	Data_Get_Struct( self, rbDrawable, object );
	object->SetScale( scaleX, scaleY );
	
	return Qnil;
}

/* call-seq:
 *   drawable.setScaleX( factor )
 *
 * Set the X scale factor of the object.
 *
 * factor must be strictly positive, otherwise it is ignored.
 */
static VALUE Drawable_SetScaleX( VALUE self, VALUE aX )
{
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	object->SetScaleX( NUM2DBL( aX ) );
	return Qnil;
}

/* call-seq:
 *   drawable.setScaleY( factor )
 *
 * Set the Y scale factor of the object.
 *
 * factor must be strictly positive, otherwise it is ignored.
 */
static VALUE Drawable_SetScaleY( VALUE self, VALUE aY )
{
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	object->SetScaleY( NUM2DBL( aY ) );
	return Qnil;
}

/* call-seq:
 *   drawable.setOrigin( x, y )
 *   drawable.setOrigin( vector2 )
 *
 * Set the local origin of the object.
 *
 * The origin of an object defines the center point for all transformations (position, scale, rotation). The 
 * coordinates of this point must be relative to the top-left corner of the object, and ignore all transformations 
 * (position, scale, rotation). The default origin of a drawable object is (0, 0).
 */
static VALUE Drawable_SetOrigin( int argc, VALUE *args, VALUE self )
{
	rbDrawable *object = NULL;
	VALUE arg0 = Qnil;
	float originX = 0.0f;
	float originY = 0.0f;
	switch( argc )
	{
		case 1:
			arg0 = Vector2_ForceType( args[0] );
			originX = NUM2DBL( Vector2_GetX( arg0 ) );
			originY = NUM2DBL( Vector2_GetY( arg0 ) );
			break;
		case 2:
			originX = NUM2DBL( args[0] );
			originY = NUM2DBL( args[1] );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 1 or 2 arguments but was given %d", argc );
	}
	
	Data_Get_Struct( self, rbDrawable, object );
	object->SetOrigin( originX, originY );
	
	return Qnil;
}

/* call-seq:
 *   drawable.setRotation( angle )
 *
 * Set the orientation of the object.
 *
 * This function completely overwrites the previous rotation. See Rotate to add an angle based on the previous 
 * rotation instead. The default rotation of a drawable object is 0.
 */
static VALUE Drawable_SetRotation( VALUE self, VALUE aRotation )
{
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	object->SetRotation( NUM2DBL( aRotation ) );
	return Qnil;
}

/* call-seq:
 *   drawable.setColor( color )
 *
 * Set the global color of the object.
 *
 * This global color affects the entire object, and modulates (multiplies) its original pixels. 
 * The default color is white.
 */
static VALUE Drawable_SetColor( VALUE self, VALUE aColor )
{
	VALUE color = Color_ForceType( aColor );
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	object->SetColor( sf::Color( Color_GetR( color ), Color_GetG( color ), Color_GetB( color ), Color_GetA( color ) ) );
	return Qnil;
}

/* call-seq:
 *   drawable.setBlendMode( mode )
 *
 * Set the blending mode of the object.
 *
 * This property defines how the pixels of an object are blended with the pixels of the render target to which it is
 * drawn. To know more about the blending modes available, see the SFML::Blend module. The default blend mode is
 * SFML::Blend::Alpha.
 */
static VALUE Drawable_SetBlendMode( VALUE self, VALUE aMode )
{
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	object->SetBlendMode( static_cast<sf::Blend::Mode>( FIX2INT( aMode ) ) );
	return Qnil;
}

/* call-seq:
 *   drawable.getPosition()	-> vector2
 *
 * Get the position of the object. 
 */
static VALUE Drawable_GetPosition( VALUE self )
{
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	const sf::Vector2f &vector = object->GetPosition();
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, rb_float_new( vector.x ), rb_float_new( vector.y ) );
}

/* call-seq:
 *   drawable.getScale()	-> vector2
 *
 * Get the current scale of the object. 
 */
static VALUE Drawable_GetScale( VALUE self )
{
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	const sf::Vector2f &vector = object->GetScale();
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, rb_float_new( vector.x ), rb_float_new( vector.y ) );
}

/* call-seq:
 *   drawable.getOrigin()	-> vector2
 *
 * Get the local origin of the object.
 */
static VALUE Drawable_GetOrigin( VALUE self )
{
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	const sf::Vector2f &vector = object->GetOrigin();
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, rb_float_new( vector.x ), rb_float_new( vector.y ) );
}

/* call-seq:
 *   drawable.getRotation()	-> float
 *
 * Get the orientation of the object.
 *
 * The rotation is always in the range [0, 360].
 */
static VALUE Drawable_GetRotation( VALUE self )
{
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	return rb_float_new( object->GetRotation() );
}

/* call-seq:
 *   drawable.getColor()	-> color
 *
 * Get the color of the object. 
 */
static VALUE Drawable_GetColor( VALUE self )
{
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	const sf::Color &color = object->GetColor();
	return rb_funcall( globalColorClass, rb_intern( "new" ), 4, 	INT2FIX( color.r ), INT2FIX( color.g ), 
									INT2FIX( color.b ), INT2FIX( color.a ) );
}

/* call-seq:
 *   drawable.getBlendMode()	-> mode
 *
 * Get the blend mode of the object. 
 */
static VALUE Drawable_GetBlendMode( VALUE self )
{
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	return INT2FIX( object->GetBlendMode() );
}

/* call-seq:
 *   drawable.move( x, y )
 *   drawable.move( vector2 )
 *
 * Move the object by a given offset.
 *
 * This function adds to the current position of the object, unlike setPosition which overwrites it.
 */
static VALUE Drawable_Move( int argc, VALUE *args, VALUE self )
{
	rbDrawable *object = NULL;
	VALUE arg0 = Qnil;
	float moveX = 0.0f;
	float moveY = 0.0f;
	switch( argc )
	{
		case 1:
			arg0 = Vector2_ForceType( args[0] );
			moveX = NUM2DBL( Vector2_GetX( arg0 ) );
			moveY = NUM2DBL( Vector2_GetY( arg0 ) );
			break;
		case 2:
			moveX = NUM2DBL( args[0] );
			moveY = NUM2DBL( args[1] );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 1 or 2 arguments but was given %d", argc );
	}
	
	Data_Get_Struct( self, rbDrawable, object );
	object->Move( moveX, moveY );
	
	return Qnil;
}

/* call-seq:
 *   drawable.scale( x, y )
 *   drawable.scale( vector2 )
 *
 * Scale the object.
 *
 * This function multiplies the current scale of the object, unlike setScale which overwrites it.
 */
static VALUE Drawable_Scale( int argc, VALUE *args, VALUE self )
{
	rbDrawable *object = NULL;
	VALUE arg0 = Qnil;
	float scaleX = 0.0f;
	float scaleY = 0.0f;
	switch( argc )
	{
		case 1:
			arg0 = Vector2_ForceType( args[0] );
			scaleX = NUM2DBL( Vector2_GetX( arg0 ) );
			scaleY = NUM2DBL( Vector2_GetY( arg0 ) );
			break;
		case 2:
			scaleX = NUM2DBL( args[0] );
			scaleY = NUM2DBL( args[1] );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 1 or 2 arguments but was given %d", argc );
	}
	
	Data_Get_Struct( self, rbDrawable, object );
	object->Scale( scaleX, scaleY );
	
	return Qnil;
}

/* call-seq:
 *   drawable.rotate( angle )
 *
 * Rotate the object.
 *
 * This function ads to the current rotation of the object, unlike setRotation which overwrites it
 */
static VALUE Drawable_Rotate( VALUE self, VALUE aRotation )
{
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	object->Rotate( NUM2DBL( aRotation ) );
	return Qnil;
}

/* call-seq:
 *   drawable.transformToLocal( vector2 )	-> vector2
 *
 * Transform a point in object local coordinates.
 *
 * This function takes a point in global coordinates, and transforms it in coordinates local to the object. In other
 * words, it applies the inverse of all the transformations applied to the object (origin, translation, rotation 
 * and scale).
 */
static VALUE Drawable_TransformToLocal( VALUE self, VALUE aPoint )
{
	VALUE point = Vector2_ForceType( aPoint );
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	sf::Vector2f newPoint = object->TransformToLocal( sf::Vector2f( NUM2DBL( Vector2_GetX( point ) ), 
									NUM2DBL( Vector2_GetY( point ) ) )
							);
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, rb_float_new( newPoint.x ), rb_float_new( newPoint.y ) );
}

/* call-seq:
 *   drawable.transformToGlobal( vector2 )	-> vector2
 *
 * Transform a local point in global coordinates.
 *
 * This function takes a point in local coordinates, and transforms it in global coordinates. In other words, it 
 * applies the same transformations that are applied to the object (origin, translation, rotation and scale).
 */
static VALUE Drawable_TransformToGlobal( VALUE self, VALUE aPoint )
{
	VALUE point = Vector2_ForceType(aPoint);
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	sf::Vector2f newPoint = object->TransformToGlobal( sf::Vector2f( NUM2DBL( Vector2_GetX( point ) ), 
									NUM2DBL( Vector2_GetY( point ) ) )
							);
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, rb_float_new( newPoint.x ), rb_float_new( newPoint.y ) );
}

/* call-seq:
 *   Rect.new() 				-> rect
 *   Rect.new( [left, top, width, height] )	-> rect
 *   Rect.new( rect ) 				-> rect
 *   Rect.new( left, top, width, height )	-> rect
 *   Rect.new( position, size )			-> rect
 * 
 * Create a new rect instance.
 */
static VALUE Drawable_Initialize( int argc, VALUE *args, VALUE self )
{
	rbDrawable *object = NULL;
	Data_Get_Struct( self, rbDrawable, object );
	VALUE aPosition = Qnil;
	VALUE aScale = Qnil;
	VALUE aColor = Qnil;
	int colorRed = 0;
	int colorGreen = 0;
	int colorBlue = 0;
	int colorAlpha = 0;
	
	switch( argc )
	{
		case 0:
			// Nothing to do
			break;
		case 4: 
			aColor = Color_ForceType( args[3] );
			colorRed   = FIX2INT( rb_funcall( aColor, rb_intern( "r" ), 0 ) );
			colorGreen = FIX2INT( rb_funcall( aColor, rb_intern( "g" ), 0 ) );
			colorBlue  = FIX2INT( rb_funcall( aColor, rb_intern( "b" ), 0 ) );
			colorAlpha = FIX2INT( rb_funcall( aColor, rb_intern( "a" ), 0 ) );
			object->SetColor( sf::Color( colorRed, colorGreen, colorBlue, colorAlpha ) );
		case 3:
			object->SetRotation( NUM2DBL( args[2] ) );
		case 2:
			aScale = Vector2_ForceType( args[1] );
			object->SetScaleX( Vector2_GetX( aScale ) );
			object->SetScaleY( Vector2_GetY( aScale ) );
		case 1:
			aPosition = Vector2_ForceType( args[0] );
			object->SetX( Vector2_GetX( aPosition ) );
			object->SetY( Vector2_GetY( aPosition ) );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 0..4 arguments but was given %d", argc );
	}
	return rb_call_super( argc, args );
}


static VALUE Drawable_InitializeCopy( VALUE self, VALUE aSource )
{
	sf::Drawable *selfDrawable = NULL;
	Data_Get_Struct( self, sf::Drawable, selfDrawable );
	sf::Drawable *sourceDrawable = NULL;
	Data_Get_Struct( aSource, sf::Drawable, sourceDrawable );
	*selfDrawable = *sourceDrawable;
	return self;
}

static VALUE Drawable_Allocate( VALUE aKlass )
{
	rbDrawable *object = new rbDrawable();
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, Drawable_Free, object );
	object->Init( rbData );
	return rbData;
}

static VALUE Drawable_Included( VALUE aModule, VALUE aBase )
{
	//rb_define_singleton_method( aBase, "allocate", Drawable_Allocate, 0 );
	rb_define_alloc_func( aBase, Drawable_Allocate );
	return Qnil;
}

void Init_Drawable( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Abstract base class for objects that can be drawn to a render target.
 *
 * SFML::Drawable defines the attributes and operations that are common to all the drawable classes:
 *
 *   - transformations (position, rotation, scale, local origin)
 *   - global overlay color
 *   - blending mode with background pixels
 *   - the ability to be drawn on either RenderWindow or RenderImage
 *
 * Please note that all these attributes are hardware accelerated, therefore they are extremely cheap to use 
 * (unlike older libraries that perform slow transformations on the CPU, such as rotation or scale).
 *
 * Usage example:
 *
 *   # Here we'll use a SFML::Sprite to demonstrate the features of SFML::Drawable
 *   drawable = SFML::Sprite.new( ...whatever... )
 * 
 *   drawable.SetOrigin(10, 20)               # set its origin to the local point (10, 20)
 *   drawable.SetPosition(100, 100)           # set its position to (100, 100)
 *   drawable.SetRotation(45)                 # set its orientation to 45 degrees
 *   drawable.SetColor(sf::Color::Red)        # set its global color to red
 *   drawable.SetBlendingMode(sf::Blend::Add) # set an additive blend mode
 *
 *   window.draw( drawable ) # finally draw it (window is a SFML::RenderWindow)
 *
 * Deriving your own class from SFML::Drawable is possible, however you have to use the SFML::Renderer class instead of 
 * direct OpenGL calls, which is more limited. To create a derived drawable class, all you have to do is to override the virtual Render function.
 *
 * One of the main benefits of creating your own drawable class is that you can build hierarchies of drawable objects. 
 * Indeed, when you draw a drawable inside the Render function of another drawable, the former inherits the 
 * transformations and color of the latter and combines them with its own attributes. This way, you can apply global 
 * transformations/color to a set of drawables as if it was a single entity.
 *
 * Example:
 *
 *   class MyDrawable
 *     include SFML::Drawable
 *     # ...
 *
 *     def initialize
 *       @myTexture   = SFML::Image.new( ...whatever... )
 *       @mySubSprite = SFML::Sprite.new( ...whatever... )
 *     end
 *   
 *     def render( target, renderer )
 *        # Low-level geometry rendering
 *        renderer.setTexture( @myTexture )
 *        renderer.begin( SFML::Renderer::QuadList )
 *            renderer.addVertex(...)
 *            renderer.addVertex(...)
 *            renderer.addVertex(...)
 *            renderer.addVertex(...)
 *        renderer.end()
 *   
 *        # High-level drawable rendering
 *        target.draw(@mySubSprite);
 *     end
 *   end
 *
 */
	globalDrawableModule = rb_define_module_under( sfml, "Drawable" );
	
	// Class methods
	rb_define_module_function( globalDrawableModule, "included", Drawable_Included, 1 );
	
	// Instance methods
	rb_define_method( globalDrawableModule, "initialize", Drawable_Initialize, -1 );
	rb_define_method( globalDrawableModule, "initialize_copy", Drawable_Initialize, 1 );
	rb_define_method( globalDrawableModule, "setPosition", Drawable_SetPosition, -1 );
	rb_define_method( globalDrawableModule, "setX", Drawable_SetX, 1 );
	rb_define_method( globalDrawableModule, "setY", Drawable_SetY, 1 );
	rb_define_method( globalDrawableModule, "setScale", Drawable_SetScale, -1 );
	rb_define_method( globalDrawableModule, "setScaleX", Drawable_SetScaleX, 1 );
	rb_define_method( globalDrawableModule, "setScaleY", Drawable_SetScaleY, 1 );
	rb_define_method( globalDrawableModule, "setOrigin", Drawable_SetOrigin, -1 );
	rb_define_method( globalDrawableModule, "setRotation", Drawable_SetRotation, 1 );
	rb_define_method( globalDrawableModule, "setColor", Drawable_SetColor, 1 );
	rb_define_method( globalDrawableModule, "setBlendMode", Drawable_SetBlendMode, 1 );
	rb_define_method( globalDrawableModule, "getPosition", Drawable_GetPosition, 0 );
	rb_define_method( globalDrawableModule, "getScale", Drawable_GetScale, 0 );
	rb_define_method( globalDrawableModule, "getOrigin", Drawable_GetOrigin, 0 );
	rb_define_method( globalDrawableModule, "getRotation", Drawable_GetRotation, 0 );
	rb_define_method( globalDrawableModule, "getColor", Drawable_GetColor, 0 );
	rb_define_method( globalDrawableModule, "getBlendMode", Drawable_GetBlendMode, 0 );
	rb_define_method( globalDrawableModule, "move", Drawable_Move, -1 );
	rb_define_method( globalDrawableModule, "scale", Drawable_Scale, -1 );
	rb_define_method( globalDrawableModule, "rotate", Drawable_Rotate, 1 );
	rb_define_method( globalDrawableModule, "transformToLocal", Drawable_TransformToLocal, 1 );
	rb_define_method( globalDrawableModule, "transformToGlobal", Drawable_TransformToGlobal, 1 );
	
	// Aliases
	rb_define_alias( globalDrawableModule, "position=", "setPosition" );
	rb_define_alias( globalDrawableModule, "position", "getPosition" );
	rb_define_alias( globalDrawableModule, "x=", "setX" );
	rb_define_alias( globalDrawableModule, "y=", "setY" );
	
	rb_define_alias( globalDrawableModule, "scale=", "setScale" );
	rb_define_alias( globalDrawableModule, "scale", "getScale" );
	rb_define_alias( globalDrawableModule, "scaleX=", "setScaleX" );
	rb_define_alias( globalDrawableModule, "scale_x=", "setScaleX" );
	rb_define_alias( globalDrawableModule, "scaleY=", "setScaleY" );
	rb_define_alias( globalDrawableModule, "scale_y=", "setScaleY" );
	
	rb_define_alias( globalDrawableModule, "origin=", "setOrigin" );
	rb_define_alias( globalDrawableModule, "origin", "getOrigin" );
	
	rb_define_alias( globalDrawableModule, "rotation=", "setRotation" );
	rb_define_alias( globalDrawableModule, "rotation", "getRotation" );
	
	rb_define_alias( globalDrawableModule, "color=", "setColor" );
	rb_define_alias( globalDrawableModule, "color", "getColor" );
	
	rb_define_alias( globalDrawableModule, "blendMode=", "setBlendMode" );
	rb_define_alias( globalDrawableModule, "blend_mode=", "setBlendMode" );
	rb_define_alias( globalDrawableModule, "blendMode", "getBlendMode" );
	rb_define_alias( globalDrawableModule, "blend_mode", "getBlendMode" );
	
	rb_define_alias( globalDrawableModule, "transform_to_local", "transformToLocal" );
	rb_define_alias( globalDrawableModule, "transform_to_global", "transformToGlobal" );
}

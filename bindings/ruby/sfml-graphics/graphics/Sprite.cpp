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

#include "Sprite.hpp"
#include "Vector2.hpp"
#include "Rect.hpp"
#include "Color.hpp"
#include "main.hpp"
#include <SFML/Graphics.hpp>

VALUE globalSpriteClass;
/* External classes */
extern VALUE globalVector2Class;
extern VALUE globalRectClass;
extern VALUE globalDrawableModule;
extern VALUE globalColorClass;
extern VALUE globalImageClass;

static void Sprite_Free( sf::Sprite *anObject )
{
	delete anObject;
}

/* call-seq:
 *   Sprite.new()											-> sprite
 *   Sprite.new( image, position = [0, 0], scale = [1, 1], rotation = 0.0, color = SFML::Color::White )	-> sprite
 *
 * Construct the sprite from a source image.
 */
static VALUE Sprite_Initialize( int argc, VALUE *args, VALUE self )
{
	VALUE temp		= Qnil;
	sf::Image *image 	= NULL;
	sf::Vector2f position 	= sf::Vector2f( 0, 0 );
	sf::Vector2f scale	= sf::Vector2f( 1, 1 );
	float rotation		= 0;
	sf::Color color 	= sf::Color::White;

	sf::Sprite *object = NULL;
	Data_Get_Struct( self, sf::Sprite, object );
	switch( argc )
	{
		case 5:
			temp = Color_ForceType( args[4] );
			color.r = FIX2INT( Color_GetR( temp ) );
			color.g = FIX2INT( Color_GetG( temp ) );
			color.b = FIX2INT( Color_GetB( temp ) );
			color.a = FIX2INT( Color_GetA( temp ) );
		case 4:
			rotation = NUM2DBL( args[3] );
		case 3:
			temp = Vector2_ForceType( args[2] );
			scale.x = NUM2DBL( Vector2_GetX( temp ) );
			scale.y = NUM2DBL( Vector2_GetY( temp ) );
		case 2:
			temp = Vector2_ForceType( args[1] );
			position.x = NUM2DBL( Vector2_GetX( temp ) );
			position.y = NUM2DBL( Vector2_GetY( temp ) );
		case 1:
			VALIDATE_CLASS( args[0], globalImageClass, "image" );
			Data_Get_Struct( args[0], sf::Image, image );
			*object = sf::Sprite( *image, position, scale, rotation, color );
			rb_iv_set( self, "@__image_ref", args[0] );
		case 0:
			break;
		default:
			rb_raise( rb_eArgError, "Expected 0..5 arguments but was given %d", argc );
	}
	return self;
}

static VALUE Sprite_InitializeCopy( VALUE self, VALUE aSource )
{
	sf::Sprite *object = NULL;
	Data_Get_Struct( self, sf::Sprite, object );
	sf::Sprite *source = NULL;
	Data_Get_Struct( aSource, sf::Sprite, source );
	*object = *source;
}

/* call-seq:
 *   sprite.setImage( image, adjustToNewSize = false)
 *
 * Change the source image of the sprite.
 *
 * The image argument refers to an image that must exist as long as the sprite uses it. Indeed, the sprite doesn't
 * store its own copy of the image, but rather keeps a pointer to the one that you passed to this function. If the
 * source image is destroyed and the sprite tries to use it, it may appear as a white rectangle. If adjustToNewSize is
 * true, the SubRect property of the sprite is adjusted to the size of the new image. If it is false, the SubRect
 * is unchanged.
 */
static VALUE Sprite_SetImage( int argc, VALUE *args, VALUE self )
{
	sf::Image *image	= NULL;
	bool adjustToNewSize 	= false;

	sf::Sprite *object = NULL;
	Data_Get_Struct( self, sf::Sprite, object );
	rb_iv_set( self, "@__image_ref", Qnil );
	switch( argc )
	{
		case 2:
			if( args[1] == Qtrue )
			{
				adjustToNewSize = true;
			}
			else if( args[1] == Qfalse )
			{
				adjustToNewSize = false;
			}
			else
			{
				VALIDATE_CLASS( args[1], rb_cTrueClass, "adjustToNewSize" );
			}
		case 1:
			VALIDATE_CLASS( args[0], globalImageClass, "image" );
			Data_Get_Struct( args[0], sf::Image, image );
			object->SetImage( *image, adjustToNewSize );
			rb_iv_set( self, "@__image_ref", args[0] );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 1 or 2 arguments but was given %d", argc );
	}
	return Qnil;
}

/* call-seq:
 *   sprite.setSubRect( rectangle )
 *
 * Set the part of the image that the sprite will display.
 *
 * The sub-rectangle is useful when you don't want to display the whole image, but rather a part of it. By default,
 * the sub-rectangle covers the entire image.
 */
static VALUE Sprite_SetSubRect( VALUE self, VALUE aRectangle )
{
	VALUE temp = Rect_ForceType( aRectangle );
	sf::IntRect rectangle;
	rectangle.Left   = FIX2INT( Rect_GetLeft( temp ) );
	rectangle.Top    = FIX2INT( Rect_GetTop( temp ) );
	rectangle.Width  = FIX2INT( Rect_GetWidth( temp ) );
	rectangle.Height = FIX2INT( Rect_GetHeight( temp ) );
  sf::Sprite *object = NULL;
  Data_Get_Struct( self, sf::Sprite, object );
	object->SetSubRect( rectangle );
	return Qnil;
}

/* call-seq:
 *   sprite.resize( width, height )
 *   sprite.resize( vector2 )
 *
 * Change the size of the sprite.
 *
 * This function is just a shortcut that calls SetScale with the proper values, calculated from the size of the current
 * subrect. If width or height is not strictly positive, this functions does nothing.
 */
static VALUE Sprite_Resize( int argc, VALUE *args, VALUE self )
{
	VALUE arg0 = Qnil;
	float width = 0.0f;
	float height = 0.0f;
	switch( argc )
	{
		case 1:
			arg0 = Vector2_ForceType( args[0] );
			width = NUM2DBL( Vector2_GetX( arg0 ) );
			height = NUM2DBL( Vector2_GetY( arg0 ) );
			break;
		case 2:
			width = NUM2DBL( args[0] );
			height = NUM2DBL( args[1] );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 1 or 2 arguments but was given %d", argc );
	}
	sf::Sprite *object = NULL;
	Data_Get_Struct( self, sf::Sprite, object );
	object->Resize( width, height );
	return Qnil;
}

/* call-seq:
 *   sprite.flipX( flipped )
 *
 * Flip the sprite horizontally.
 */
static VALUE Sprite_FlipX( VALUE self, VALUE aFlippedFlag )
{
	sf::Sprite *object = NULL;
	Data_Get_Struct( self, sf::Sprite, object );
	if( aFlippedFlag == Qtrue )
	{
		object->FlipX( true );
	}
	else if( aFlippedFlag == Qfalse )
	{
		object->FlipX( false );
	}
	else
	{
		VALIDATE_CLASS( aFlippedFlag, rb_cTrueClass, "flipped" );
	}
	return Qnil;
}

/* call-seq:
 *   sprite.flipY( flipped )
 *
 * Flip the sprite vertically.
 */
static VALUE Sprite_FlipY( VALUE self, VALUE aFlippedFlag )
{
	sf::Sprite *object = NULL;
	Data_Get_Struct( self, sf::Sprite, object );
	if( aFlippedFlag == Qtrue )
	{
		object->FlipY( true );
	}
	else if( aFlippedFlag == Qfalse )
	{
		object->FlipY( false );
	}
	else
	{
		VALIDATE_CLASS( aFlippedFlag, rb_cTrueClass, "flipped" );
	}
	return Qnil;
}

/* call-seq:
 *   sprite.getImage()	-> image or nil
 *
 * Get the source image of the sprite.
 *
 * If the sprite has no source image, or if the image doesn't exist anymore, nil is returned.
 */
static VALUE Sprite_GetImage( VALUE self )
{
	return rb_iv_get( self, "@__image_ref" );
}

/* call-seq:
 *   sprite.getSubRect()	-> rectangle
 *
 * Get the region of the image displayed by the sprite.
 */
static VALUE Sprite_GetSubRect( VALUE self )
{
	sf::Sprite *object = NULL;
	Data_Get_Struct( self, sf::Sprite, object );
	const sf::IntRect &rect = object->GetSubRect();
	return rb_funcall( globalRectClass, rb_intern( "new" ), 4,
				INT2FIX( rect.Left ), INT2FIX( rect.Top ),
				INT2FIX( rect.Width ), INT2FIX( rect.Height ) );
}

/* call-seq:
 *   sprite.getSize()	-> vector2
 *
 * Get the global size of the sprite.
 *
 * This function is a shortcut that multiplies the size of the subrect by the scale factors.
 */
static VALUE Sprite_GetSize( VALUE self )
{
	sf::Sprite *object = NULL;
	Data_Get_Struct( self, sf::Sprite, object );
	const sf::Vector2f size = object->GetSize();
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, rb_float_new( size.x ), rb_float_new( size.y ) );
}

/* call-seq:
 *   sprite.getPixel( x, y )	-> color
 *
 * Get the color of a given pixel in the sprite.
 *
 * This function returns the source image pixel, multiplied by the global color of the sprite. The input point must
 * be in local coordinates. If you have a global point, you can use the TransformToLocal function to make it local.
 * This function doesn't perform any check, you must ensure that the x and y coordinates are not out of bounds.
 */
static VALUE Sprite_GetPixel( VALUE self, VALUE aX, VALUE aY )
{
	sf::Sprite *object = NULL;
	Data_Get_Struct( self, sf::Sprite, object );
	const sf::Color color = object->GetPixel( FIX2UINT( aX ), FIX2UINT( aY ) );
	return rb_funcall( globalColorClass, rb_intern( "new" ), 4,
				INT2FIX( color.r ), INT2FIX( color.g ),
				INT2FIX( color.b ), INT2FIX( color.a ) );
}

static VALUE Sprite_Alloc( VALUE aKlass )
{
	sf::Sprite *object = new sf::Sprite();
	return Data_Wrap_Struct( aKlass, 0, Sprite_Free, object );
}

void Init_Sprite( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Drawable representation of an image, with its own transformations, color, blend mode, etc.
 *
 * SFML::Sprite is a drawable class that allows to easily display an image (or a part of it) on a render target.
 *
 * It inherits all the functions from SFML::Drawable: position, rotation, scale, origin, global color and blend mode.
 * It also adds sprite-specific properties such as the image to use, the part of it to display, and some convenience
 * functions to flip or resize the sprite.
 *
 * SFML::Sprite works in combination with the SFML::Image class, which loads and provides the pixel data of a
 * given image.
 *
 * The separation of SFML::Sprite and SFML::Image allows more flexibility and better performances: indeed a SFML::Image
 * is a heavy resource, and any operation on it is slow (often too slow for real-time applications). On the other side,
 * a SFML::Sprite is a lightweight object which can use the pixel data of a SFML::Image and draw it with its own
 * transformation / color / blending attributes.
 *
 * It is important to note that the SFML::Sprite instance doesn't copy the image that it uses, it only keeps a reference
 * to it. Thus, a SFML::Image must not be destructed while it is used by a SFML::Sprite (i.e. never write a function that
 * uses a local SFML::Image instance for creating a sprite).
 *
 * NOTE: This is the ruby bindings so the images will be managed by the ruby garbage collector and thus the image won't
 * be destructed until all sprites referencing it is destructed. But it's still a good practice to keep in mind.
 *
 * Usage example:
 *
 *   # Declare and load an image
 *   image = SFML::Image.new
 *   image.loadFromFile( "image.png" )
 *
 *   # Create a sprite
 *   sprite = SFML::Sprite.new
 *   sprite.image = image
 *   sprite.subRect = [10, 10, 50, 30]
 *   sprite.resize( 100, 60 )
 *
 *   # Display it
 *   window.draw( sprite ) # window is a SFML::RenderWindow
 *
 */
	globalSpriteClass = rb_define_class_under( sfml, "Sprite", rb_cObject );
	rb_include_module( globalSpriteClass, globalDrawableModule );

	// Class methods
	//rb_define_singleton_method( globalSpriteClass, "new", Sprite_New, -1 );
	rb_define_alloc_func( globalSpriteClass, Sprite_Alloc );

	// Instance methods
	rb_define_method( globalSpriteClass, "initialize", Sprite_Initialize, -1 );
	rb_define_method( globalSpriteClass, "initialize_copy", Sprite_InitializeCopy, 1 );
	rb_define_method( globalSpriteClass, "setImage", Sprite_SetImage, -1 );
	rb_define_method( globalSpriteClass, "setSubRect", Sprite_SetSubRect, 1 );
	rb_define_method( globalSpriteClass, "resize", Sprite_Resize, -1 );
	rb_define_method( globalSpriteClass, "flipX", Sprite_FlipX, 1 );
	rb_define_method( globalSpriteClass, "flipY", Sprite_FlipY, 1 );
	rb_define_method( globalSpriteClass, "getImage", Sprite_GetImage, 0 );
	rb_define_method( globalSpriteClass, "getSubRect", Sprite_GetSubRect, 0 );
	rb_define_method( globalSpriteClass, "getSize", Sprite_GetSize, 0 );
	rb_define_method( globalSpriteClass, "getPixel", Sprite_GetPixel, 2 );

	// Instance Aliases
	rb_define_alias( globalSpriteClass, "image=", "setImage" );
	rb_define_alias( globalSpriteClass, "set_image", "setImage" );
	rb_define_alias( globalSpriteClass, "image", "getImage" );
	rb_define_alias( globalSpriteClass, "get_image", "getImage" );

	rb_define_alias( globalSpriteClass, "subRect=", "setSubRect" );
	rb_define_alias( globalSpriteClass, "sub_rect=", "setSubRect" );
	rb_define_alias( globalSpriteClass, "subRect", "getSubRect" );
	rb_define_alias( globalSpriteClass, "sub_rect", "getSubRect" );

	rb_define_alias( globalSpriteClass, "flip_x", "flipX" );
	rb_define_alias( globalSpriteClass, "flip_y", "flipY" );
	rb_define_alias( globalSpriteClass, "flip_x=", "flipX" );
	rb_define_alias( globalSpriteClass, "flip_y=", "flipY" );
	rb_define_alias( globalSpriteClass, "flipX=", "flipX" );
	rb_define_alias( globalSpriteClass, "flipY=", "flipY" );

	rb_define_alias( globalSpriteClass, "get_size", "getSize" );
	rb_define_alias( globalSpriteClass, "size", "getSize" );

	rb_define_alias( globalSpriteClass, "get_pixel", "getPixel" );
}

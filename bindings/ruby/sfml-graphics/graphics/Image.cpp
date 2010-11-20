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
 
#include "Image.hpp"
#include "Color.hpp"
#include "main.hpp"
#include <SFML/Graphics/Image.hpp>

VALUE globalImageClass;

/* External classes */
extern VALUE globalColorClass;

/* Free a heap allocated object 
 * Not accessible trough ruby directly!
 */
static void Image_Free( sf::Image *anObject )
{
	delete anObject;
}

static VALUE Image_LoadFromFile( VALUE self, VALUE aFileName )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	if( object->LoadFromFile( rb_string_value_cstr( aFileName ) ) == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

static VALUE Image_LoadFromPixels( VALUE self, VALUE aWidth, VALUE aHeight, VALUE somePixels )
{
	const unsigned int rawWidth = FIX2UINT( aWidth );
	const unsigned int rawHeight = FIX2UINT( aHeight );
	VALIDATE_CLASS( somePixels, rb_cArray, "pixels" );
	const unsigned long dataSize = rawWidth * rawHeight * 4;
	sf::Uint8 * const tempData = new sf::Uint8[dataSize];
	VALUE pixels = rb_funcall( somePixels, rb_intern("flatten"), 0 );
	for(unsigned long index = 0; index < dataSize; index++)
	{
		sf::Uint8 val = NUM2CHR( rb_ary_entry( pixels, index ) );
		tempData[index] = val;
	}
	
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	bool result = object->LoadFromPixels( rawWidth, rawHeight, tempData );
	delete[] tempData;
	
	if( result == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

static VALUE Image_SaveToFile( VALUE self, VALUE aFileName )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	if( object->SaveToFile( rb_string_value_cstr( aFileName ) ) == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

static VALUE Image_Create( int argc, VALUE *args, VALUE self )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	
	unsigned int width = 0;
	unsigned int height = 0;
	VALUE rubyColor = Qnil;
	sf::Color color;
	
	switch( argc )
	{
		case 3:
			rubyColor = Color_ForceType( args[2] );
			color.r = FIX2INT( Color_GetR( rubyColor ) );
			color.g = FIX2INT( Color_GetG( rubyColor ) );
			color.b = FIX2INT( Color_GetB( rubyColor ) );
			color.a = FIX2INT( Color_GetA( rubyColor ) );
		case 2:
			width = FIX2UINT( args[0] );
			height = FIX2UINT( args[1] );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 2 or 3 arguments but was given %d", argc );
	}
	
	return ( object->Create( width, height, color ) == true ? Qtrue : Qfalse );
}

static VALUE Image_CreateMaskFromColor( int argc, VALUE *args, VALUE self )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	
	sf::Uint8 alpha = 0;
	VALUE rubyColor = Qnil;
	sf::Color color;
	
	switch( argc )
	{
		case 2:
			alpha = FIX2UINT( alpha );
		case 1:
			rubyColor = Color_ForceType( args[0] );
			color.r = FIX2INT( Color_GetR( rubyColor ) );
			color.g = FIX2INT( Color_GetG( rubyColor ) );
			color.b = FIX2INT( Color_GetB( rubyColor ) );
			color.a = FIX2INT( Color_GetA( rubyColor ) );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 1 or 2 arguments but was given %d", argc );
	}
	
	object->CreateMaskFromColor( color, alpha );
	return Qnil;
}

static VALUE Image_Copy( int argc, VALUE *args, VALUE self )
{
	Image *source;
	unsigned int destX = 0;
	unsigned int destY = 0;
	sf::IntRect sourceRect = sf::IntRect(0, 0, 0, 0);
	VALUE rubySourceRect = Qnil;
	bool applyAlpha = false;
	
	switch( argc )
	{
		case 5:
			if( args[4] == Qtrue )
			{
				applyAlpha = true;
			}
			else if( args[4] == Qfalse )
			{
				applyAlpha = false;
			}
			else
			{
				VALIDATE_CLASS( args[4], rb_cTrueClass, "applyAlpha" );
			}
		case 4:
			rubySourceRect = Rect_ForceType( args[3] );
			sourceRect.Left = FIX2INT( Rect_GetLeft( rubySourceRect ) );
			sourceRect.Top = FIX2INT( Rect_GetTop( rubySourceRect ) );
			sourceRect.Width = FIX2INT( Rect_GetWidth( rubySourceRect ) );
			sourceRect.Height = FIX2INT( Rect_GetHeight( rubySourceRect ) );
		case 3:
			VALIDATE_CLASS( args[0], globalImageClass, "source" );
			Data_Get_Struct( args[0], sf::Image, source );
			destX = FIX2UINT( args[1] );
			destX = FIX2UINT( args[2] );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 3..5 arguments but was given %d", argc );
	}
	
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	object->Copy( *source, dextX, dextY, sourceRect, applyAlpha );
	return Qnil;
}

static VALUE Image_CopyScreen( int argc, VALUE *args, VALUE self )
{
	sf::RenderWindow *source;
	sf::IntRect sourceRect = sf::IntRect(0, 0, 0, 0);
	VALUE rubySourceRect = Qnil;
	
	switch( argc )
	{
		case 2:
			rubySourceRect = Rect_ForceType( args[3] );
			sourceRect.Left = FIX2INT( Rect_GetLeft( rubySourceRect ) );
			sourceRect.Top = FIX2INT( Rect_GetTop( rubySourceRect ) );
			sourceRect.Width = FIX2INT( Rect_GetWidth( rubySourceRect ) );
			sourceRect.Height = FIX2INT( Rect_GetHeight( rubySourceRect ) );
		case 1:
			VALIDATE_CLASS( args[0], globalImageClass, "source" );
			Data_Get_Struct( args[0], sf::RenderWindow, source );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 1 or 2 arguments but was given %d", argc );
	}
	
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );	
	if( object->CopyScreen( *source, sourceRect ) == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

static VALUE Image_SetPixel( VALUE self, VALUE aX, VALUE aY, VALUE aColor )
{
	VALUE rbColor = Color_ForceType( aColor );
	sf::Color color;
	color.r = FIX2INT( Color_GetR( rubyColor ) );
	color.g = FIX2INT( Color_GetG( rubyColor ) );
	color.b = FIX2INT( Color_GetB( rubyColor ) );
	color.a = FIX2INT( Color_GetA( rubyColor ) );
	
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	object->SetPixel( FIX2INT( aX ), FIX2INT( aY ), color );
	return Qnil;
}

static VALUE Image_SetPixel( VALUE self, VALUE aX, VALUE aY )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	const sf::Color color = object->SetPixel( FIX2INT( aX ), FIX2INT( aY ) );
	return rb_funcall( globalColorClass, rb_intern( "new" ), 4,
				INT2FIX( color.r ), INT2FIX( color.g ),
				INT2FIX( color.b ), INT2FIX( color.a ) );
}

static VALUE Image_GetPixelPtr( VALUE self )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	
	VALUE pixels = rb_ary_new2( dataSize );
	const sf::Uint8 *const pixelPointer = object->GetPixelPtr();
	
	const unsigned int rawWidth = object->GetWidth();
	const unsigned int rawHeight = object->GetHeight();
	const unsigned long dataSize = rawWidth * rawHeight * 4;
	for(unsigned long index = 0; index < dataSize; index++)
	{
		rb_ary_store( pixels, index, CHR2FIX( pixelPointer[index] ) );
	}
	
	return pixels;
}

static VALUE Image_UpdatePixels( int argc, VALUE *args, VALUE self )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	VALUE somePixels = Qnil;
	VALUE aRectangle = Qnil;
	IntRect rectangle = IntRect(0, 0, object->GetWidth(), object->GetHeight() );
	
	switch( argc )
	{
		case 2:
			aRectangle = Rect_ForceType( args[1] );
			rectangle.Left = FIX2INT( Rect_GetLeft( aRectangle ) );
			rectangle.Top = FIX2INT( Rect_GetTop( aRectangle ) );
			rectangle.Width = FIX2INT( Rect_GetWidth( aRectangle ) );
			rectangle.Height = FIX2INT( Rect_GetHeight( aRectangle ) );
		case 1:
			VALIDATE_CLASS( args[0], rb_cArray, "pixels" );
			somePixels = args[0];
			break;
		default:
			rb_raise( rb_eArgError, "Expected 1 or 2 arguments but was given %d", argc );		
	}
	const unsigned int rawWidth = FIX2UINT( rectangle.Width );
	const unsigned int rawHeight = FIX2UINT( rectangle.Height );
	const unsigned long dataSize = rawWidth * rawHeight * 4;
	sf::Uint8 * const tempData = new sf::Uint8[dataSize];
	VALUE pixels = rb_funcall( somePixels, rb_intern("flatten"), 0 );
	for(unsigned long index = 0; index < dataSize; index++)
	{
		sf::Uint8 val = NUM2CHR( rb_ary_entry( pixels, index ) );
		tempData[index] = val;
	}
	bool result = object->UpdatePixels( tempData, rectangle );
	delete[] tempData;
	
	return Qnil;	
}

static VALUE Image_Bind( VALUE self )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	object->Bind();
	return Qnil;
}

static VALUE Image_SetSmooth( VALUE self, VALUE aSmoothFlag )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	
	if( aSmoothFlag == Qtrue )
	{
		object->SetSmooth( true ):
	}
	else if( aSmoothFlag == Qfalse )
	{
		object->SetSmooth( false );
	}
	else
	{
		VALIDATE_CLASS( aSmoothFlag, rb_cTrueClass, "smoothFlag" );
	}
	return Qnil;
}

static VALUE Image_IsSmooth( VALUE self )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	return ( object->IsSmooth() == true ? Qtrue : Qfalse );
}

static VALUE Image_GetWidth( VALUE self )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	return INT2FIX( object->GetWidth() );
}

static VALUE Image_GetHeight( VALUE self )
{
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	return INT2FIX( object->GetHeight() );
}

static VALUE Image_GetTexCoords( VALUE self, VALUE aRectangle )
{
	VALUE rubyRectangle = Rect_ForceType( aRectangle );
	sf::IntRect rectangle;
	rectangle.Left = FIX2INT( Rect_GetLeft( aRectangle ) );
	rectangle.Top = FIX2INT( Rect_GetTop( aRectangle ) );
	rectangle.Width = FIX2INT( Rect_GetWidth( aRectangle ) );
	rectangle.Height = FIX2INT( Rect_GetHeight( aRectangle ) );
	
	sf::Image *object = NULL;
	Data_Get_Struct( self, sf::Image, object );
	
	sf::FloatRect result = object->GetTexCords( rectangle );
	return rb_funcall( globalRectClass, rb_intern( "new" ), 4, 
					rb_float_new( result.Left ), rb_float_new( result.Top ), 
					rb_float_new( result.Width ), rb_float_new( result.Height ) );
}

/* call-seq:
 *   Image.new()	-> image
 *
 * The clock starts automatically after being constructed.
 */
static VALUE Image_New( VALUE aKlass )
{
	sf::Image *object = new sf::Image();
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, Image_Free, object );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

void Init_Image( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Class for loading, manipulating and saving images.
 *
 * SFML::Image is an abstraction to manipulate images as bidimensional arrays of pixels.
 *
 * The class provides functions to load, read, write and save pixels, as well as many other useful functions to...
 *
 * SFML::Image can handle a unique internal representation of pixels, which is RGBA 32 bits. This means that a pixel
 * must be composed of 8 bits red, green, blue and alpha channels -- just like a SFML::Color. All the functions that 
 * return an array of pixels follow this rule, and all parameters that you pass to sf::Image functions (such as 
 * loadFromPixels or updatePixels) must use this representation as well.
 * 
 * A SFML::Image can be copied, but it is a heavy resource and if possible you should always use [const] references 
 * to pass or return them to avoid useless copies.
 * 
 * Usage example:
 *
 *   # Load an image file
 *   background = SFML::Image.new;
 *   if background.loadFromFile( "background.jpg" ) == false
 *     # Error
 *   end
 *
 *   # Create a 20x20 image filled with black color
 *   image = SFML::Image.new;
 *   if image.create( 20, 20, SFML::Color::Black ) == false
 *     # Error
 *   end
 *
 *   # Copy image1 on image2 at position (10, 10)
 *   image.copy( background, 10, 10 )
 *
 *   # Make the top-left pixel transparent
 *   color = image.getPixel( 0, 0 )
 *   color.a = 0
 *   image.setPixel( 0, 0, color )
 *
 *   # Save the image to a file
 *   if image.saveToFile( "result.png" ) == false
 *     # Error
 *   end

 */
	globalImageClass = rb_define_class_under( sfml, "Image", rb_cObject );
	
	// Class methods
	rb_define_singleton_method( globalImageClass, "new", Image_New, 0 );
	
	// Instance methods
	
	// Aliases
}

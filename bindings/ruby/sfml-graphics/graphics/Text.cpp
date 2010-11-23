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
 
#include "Text.hpp"
#include "Vector2.hpp"
#include "Rect.hpp"
#include "Color.hpp"
#include "main.hpp"
#include <SFML/Graphics/Text.hpp>

VALUE globalTextClass;
/* External classes */
extern VALUE globalVector2Class;
extern VALUE globalRectClass;
extern VALUE globalDrawableModule;
extern VALUE globalColorClass;
extern VALUE globalFontClass;

static void Text_Free( sf::Text *anObject )
{
	delete anObject;
}

static VALUE Text_Initialize( int argc, VALUE *args, VALUE self )
{
	VALUE temp			= Qnil;
	sf::String string 		= "";
	const sf::Font *font		= &sf::Font::GetDefaultFont();
	unsigned int characterSize 	= 30;
	
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	switch( argc )
	{
		case 3:
			characterSize = FIX2UINT( args[2] );
		case 2:
			VALIDATE_CLASS( args[1], globalFontClass, "font" );
			Data_Get_Struct( args[1], sf::Font, font );
			rb_iv_set( self, "@__font_ref", args[1] );
		case 1:
			string = rb_string_value_cstr( &args[0] );
			*object = sf::Text( string, *font, characterSize );
		case 0:
			break;
		default:
			rb_raise( rb_eArgError, "Expected 0..3 arguments but was given %d", argc );
	}
	return self;
}

static VALUE Text_SetString( VALUE self, VALUE aString )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	object->SetString( rb_string_value_cstr( &aString ) );
	return Qnil;
}

static VALUE Text_SetFont( VALUE self, VALUE aFont )
{
	VALIDATE_CLASS( aFont, globalFontClass, "font" );
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	sf::Font *font = NULL;
	Data_Get_Struct( self, sf::Font, font );
	object->SetFont( *font );
	rb_iv_set( self, "@__font_ref", aFont );
	return Qnil;
}

static VALUE Text_SetCharacterSize( VALUE self, VALUE aSize )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	object->SetCharacterSize( FIX2UINT( aSize ) );
	return Qnil;
}

static VALUE Text_SetStyle( VALUE self, VALUE aStyle )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	object->SetStyle( FIX2UINT( aStyle ) );
	return Qnil;
}

static VALUE Text_GetString( VALUE self )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	return rb_str_new2( object->GetString().ToAnsiString().c_str() );
}

static VALUE Text_GetFont( VALUE self )
{
	return rb_iv_get( self, "@__font_ref" );
}

static VALUE Text_GetCharacterSize( VALUE self )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	return INT2FIX( object->GetCharacterSize() );
}

static VALUE Text_GetStyle( VALUE self )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	return INT2FIX( object->GetStyle() );
}

static VALUE Text_GetCharacterPos( VALUE self, VALUE anIndex )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	const sf::Vector2f pos = object->GetCharacterPos( FIX2UINT( anIndex ) );
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, rb_float_new( pos.x ), rb_float_new( pos.y ) );
}

static VALUE Text_GetRect( VALUE self )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	const sf::FloatRect rect = object->GetRect();
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 4, 
		rb_float_new( rect.Left ), rb_float_new( rect.Top ),
		rb_float_new( rect.Width ), rb_float_new( rect.Height ) );
}

static VALUE Text_New( int argc, VALUE *args, VALUE aKlass )
{
	sf::Text *object = new sf::Text();
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, Text_Free, object );
	rb_obj_call_init( rbData, argc, args );
	return rbData;
}

static void CreateStyleEnum()
{
	rb_define_const( globalTextClass, "Regular", INT2FIX( sf::Text::Regular ) );
	rb_define_const( globalTextClass, "Bold", INT2FIX( sf::Text::Bold ) );
	rb_define_const( globalTextClass, "Italic", INT2FIX( sf::Text::Italic ) );
	rb_define_const( globalTextClass, "Underlined", INT2FIX( sf::Text::Underlined ) );
}

void Init_Text( void )
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
	globalTextClass = rb_define_class_under( sfml, "Text", rb_cObject );
	rb_include_module( globalTextClass, globalDrawableModule );
	CreateStyleEnum();
	
	// Class methods
	rb_define_singleton_method( globalTextClass, "new", Text_New, -1 );
	
	// Instance methods
	rb_define_method( globalTextClass, "initialize", Text_Initialize, -1 );
	rb_define_method( globalTextClass, "setString", Text_SetString, 1 );
	rb_define_method( globalTextClass, "setFont", Text_SetFont, 1 );
	rb_define_method( globalTextClass, "setCharacterSize", Text_SetCharacterSize, 1 );
	rb_define_method( globalTextClass, "setStyle", Text_SetStyle, 1 );
	rb_define_method( globalTextClass, "getString", Text_GetString, 0 );
	rb_define_method( globalTextClass, "getFont", Text_GetFont, 0 );
	rb_define_method( globalTextClass, "getCharacterSize", Text_GetCharacterSize, 0 );
	rb_define_method( globalTextClass, "getStyle", Text_GetStyle, 0 );
	rb_define_method( globalTextClass, "getCharacterPos", Text_GetCharacterPos, 1 );
	rb_define_method( globalTextClass, "getRect", Text_GetRect, 0 );
	
	// Instance Aliases
	rb_define_alias( globalTextClass, "string=", "setString" );
	rb_define_alias( globalTextClass, "string", "getString" );
	
	rb_define_alias( globalTextClass, "font=", "setFont" );
	rb_define_alias( globalTextClass, "font", "getFont" );
	
	rb_define_alias( globalTextClass, "characterSize=", "setCharacterSize" );
	rb_define_alias( globalTextClass, "character_size=", "setCharacterSize" );
	rb_define_alias( globalTextClass, "characterSize", "getCharacterSize" );
	rb_define_alias( globalTextClass, "character_size", "getCharacterSize" );
	
	rb_define_alias( globalTextClass, "style=", "setStyle" );
	rb_define_alias( globalTextClass, "style", "getStyle" );
	
	rb_define_alias( globalTextClass, "get_character_pos", "getCharacterPos" );
	
	rb_define_alias( globalTextClass, "rect", "getRect" );
}

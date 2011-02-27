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

/* call-seq:
 *   Text.new()									-> text
 *   Text.new( string, font = SFML::Font::DefaultFont, characterSize = 30 )	-> text
 *
 * Create a text instance
 */
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
			object->SetCharacterSize( characterSize );
		case 2:
			VALIDATE_CLASS( args[1], globalFontClass, "font" );
			Data_Get_Struct( args[1], sf::Font, font );
			rb_iv_set( self, "@__font_ref", args[1] );
			object->SetFont( *font );
		case 1:
			string = rb_string_value_cstr( &args[0] );
			object->SetString( string );
		case 0:
			break;
		default:
			rb_raise( rb_eArgError, "Expected 0..3 arguments but was given %d", argc );
	}
	return self;
}

static VALUE Text_InitializeCopy( VALUE self, VALUE aSource )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	sf::Text *source = NULL;
	Data_Get_Struct( aSource, sf::Text, source );
	*object = *source;
}

/* call-seq:
 *   text.setString( string )
 *
 * Set the text's string. 
 */
static VALUE Text_SetString( VALUE self, VALUE aString )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	object->SetString( rb_string_value_cstr( &aString ) );
	return Qnil;
}

/* call-seq:
 *   text.setFont( font )
 *
 * Set the text's font.
 *
 * Texts have a valid font by default, which the built-in SFML::Font::DefaultFont.
 */
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

/* call-seq:
 *   text.setCharacterSize( size )
 *
 * Set the character size.
 *
 * The default size is 30.
 */
static VALUE Text_SetCharacterSize( VALUE self, VALUE aSize )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	object->SetCharacterSize( FIX2UINT( aSize ) );
	return Qnil;
}

/* call-seq:
 *   text.setStyle( style )
 *
 * Set the text's style.
 *
 * You can pass a combination of one or more styles, for example sf::Text::Bold | sf::Text::Italic. The default style 
 * is sf::Text::Regular.
 */
static VALUE Text_SetStyle( VALUE self, VALUE aStyle )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	object->SetStyle( FIX2UINT( aStyle ) );
	return Qnil;
}

/* call-seq:
 *   text.getString()	-> string
 *
 * Get the text's string. 
 */
static VALUE Text_GetString( VALUE self )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	return rb_str_new2( object->GetString().ToAnsiString().c_str() );
}

/* call-seq:
 *   text.getFont()	-> font
 *
 * Get the text's font. 
 */
static VALUE Text_GetFont( VALUE self )
{
	return rb_iv_get( self, "@__font_ref" );
}

/* call-seq:
 *   text.getCharacterSize()	-> fixnum
 *
 * Get the character size
 */
static VALUE Text_GetCharacterSize( VALUE self )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	return INT2FIX( object->GetCharacterSize() );
}

/* call-seq:
 *   text.getStyle()	-> fixnum
 *
 * Get the text's style. 
 */
static VALUE Text_GetStyle( VALUE self )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	return INT2FIX( object->GetStyle() );
}

/* call-seq:
 *   text.getCharacterPos( index )	-> vector2
 *
 * Return the position of the index-th character.
 *
 * This function computes the visual position of a character from its index in the string. The returned position is in
 * local coordinates (translation, rotation, scale and origin are not applied). You can easily get the corresponding
 * global position with the TransformToGlobal function. If index is out of range, the position of the end of the 
 * string is returned.
 */
static VALUE Text_GetCharacterPos( VALUE self, VALUE anIndex )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	const sf::Vector2f pos = object->GetCharacterPos( FIX2UINT( anIndex ) );
	return rb_funcall( globalVector2Class, rb_intern( "new" ), 2, rb_float_new( pos.x ), rb_float_new( pos.y ) );
}

/* call-seq:
 *   text.getRect()	-> rectangle
 *
 * Get the bounding rectangle of the text.
 *
 * The returned rectangle is in global coordinates.
 */
static VALUE Text_GetRect( VALUE self )
{
	sf::Text *object = NULL;
	Data_Get_Struct( self, sf::Text, object );
	const sf::FloatRect rect = object->GetRect();
	return rb_funcall( globalRectClass, rb_intern( "new" ), 4, 
		rb_float_new( rect.Left ), rb_float_new( rect.Top ),
		rb_float_new( rect.Width ), rb_float_new( rect.Height ) );
}

static VALUE Text_Alloc( VALUE aKlass )
{
	sf::Text *object = new sf::Text();
	return Data_Wrap_Struct( aKlass, 0, Text_Free, object );
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
/* Graphical text that can be drawn to a render target.
 *
 * SFML::Text is a drawable class that allows to easily display some text with custom style and color on a render target.
 *
 * It inherits all the functions from SFML::Drawable: position, rotation, scale, origin, global color and blend mode. 
 * It also adds text-specific properties such as the font to use, the character size, the font style (bold, italic,
 * underlined), and the text to display of course. It also provides convenience functions to calculate the graphical 
 * size of the text, or to get the visual position of a given character.
 *
 * SFML::Text works in combination with the sf::Font class, which loads and provides the glyphs (visual characters) of
 * a given font.
 *
 * The separation of SFML::Font and SFML::Text allows more flexibility and better performances: indeed a SFML::Font is a 
 * heavy resource, and any operation on it is slow (often too slow for real-time applications). On the other side, a 
 * SFML::Text is a lightweight object which can combine the glyphs data and metrics of a SFML::Font to display any 
 * text on a render target.
 * 
 * It is important to note that the SFML::Text instance doesn't copy the font that it uses, it only keeps a 
 * reference to it. Thus, a SFML::Font must not be destructed while it is used by a SFML::Text (i.e. never write a
 * function that uses a local SFML::Font instance for creating a text).
 *
 * NOTE: This is the ruby bindings so the fonts will be managed by the ruby garbage collector and thus the font won't
 * be destructed until all sprites referencing it is destructed. But it's still a good practice to keep in mind.
 *
 * Usage example:
 *
 *   # Declare and load a font
 *   font = SFML::Font.new
 *   font.loadFromFile( "arial.ttf" )
 *
 *   # Create a text
 *   text SFML::Text.new( "hello" )
 *   text.setFont( font )
 *   text.SetCharacterSize( 30 )
 *   text.setStyle( SFML::Text::Regular )
 *
 *   # Display it
 *   window.draw( text ) # window is a SFML::RenderWindow
 */
	globalTextClass = rb_define_class_under( sfml, "Text", rb_cObject );
	rb_include_module( globalTextClass, globalDrawableModule );
	CreateStyleEnum();
	
	// Class methods
	//rb_define_singleton_method( globalTextClass, "new", Text_New, -1 );
	rb_define_alloc_func( globalTextClass, Text_Alloc );
	
	// Instance methods
	rb_define_method( globalTextClass, "initialize", Text_Initialize, -1 );
	rb_define_method( globalTextClass, "initialize_copy", Text_InitializeCopy, 1 );
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

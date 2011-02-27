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
 
#include "Font.hpp"
#include "main.hpp"
#include <SFML/Graphics/Font.hpp>

VALUE globalFontClass;

/* External classes */
extern VALUE globalGlyphClass;
extern VALUE globalRectClass;
extern VALUE globalImageClass;

static void Font_Free( sf::Font *anObject )
{
	delete anObject;
}

/* call-seq:
 *   font.loadFromFile( filename )	-> true or false
 *
 * Load the font from a file.
 *
 * The supported font formats are: TrueType, Type 1, CFF, OpenType, SFNT, X11 PCF, Windows FNT, BDF, PFR and 
 * Type 42. Note that this function know nothing about the standard fonts installed on the user's system, thus 
 * you can't load them directly.
 */
static VALUE Font_LoadFromFile( VALUE self, VALUE aFileName )
{
	sf::Font *object = NULL;
	Data_Get_Struct( self, sf::Font, object );
	if( object->LoadFromFile( rb_string_value_cstr( &aFileName ) ) == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

/* call-seq:
 *   font.getGlyph( codePoint, characterSize, boldFlag )	-> glyph
 *
 * Retrieve a glyph of the font. 
 */
static VALUE Font_GetGlyph( VALUE self, VALUE aCodePoint, VALUE aCharacterSize, VALUE aBoldFlag )
{
	sf::Font *object = NULL;
	Data_Get_Struct( self, sf::Font, object );
	const sf::Glyph &glyph = object->GetGlyph( FIX2UINT( aCodePoint ), FIX2UINT( aCharacterSize ), aBoldFlag != Qfalse );
	VALUE rbGlyph = rb_funcall( globalGlyphClass, rb_intern( "new" ), 0 );
	VALUE bounds = rb_funcall( globalRectClass, rb_intern( "new" ), 4,
					INT2FIX( glyph.Bounds.Left ), INT2FIX( glyph.Bounds.Top ), 
					INT2FIX( glyph.Bounds.Width ), INT2FIX( glyph.Bounds.Height ) );
	VALUE subRect = rb_funcall( globalRectClass, rb_intern( "new" ), 4,
					INT2FIX( glyph.SubRect.Left ), INT2FIX( glyph.SubRect.Top ), 
					INT2FIX( glyph.SubRect.Width ), INT2FIX( glyph.SubRect.Height ) );
	rb_funcall( rbGlyph, rb_intern( "advance=" ), 1, INT2FIX( glyph.Advance ) );
	rb_funcall( rbGlyph, rb_intern( "bounds=" ), 1, bounds );
	rb_funcall( rbGlyph, rb_intern( "subRect=" ), 1, subRect );
	return rbGlyph;
}

/* call-seq:
 *   font.getKerning( first, size, characterSize )	-> fixnum
 *
 * Get the kerning offset of two glyphs.
 *
 * The kerning is an extra offset (negative) to apply between two glyphs when rendering them, to make the pair 
 * look more "natural". For example, the pair "AV" have a special kerning to make them closer than other characters. 
 * Most of the glyphs pairs have a kerning offset of zero, though.
 */
static VALUE Font_GetKerning( VALUE self, VALUE aFirst, VALUE aSecond, VALUE aCharacterSize )
{
	sf::Font *object = NULL;
	Data_Get_Struct( self, sf::Font, object );
	return INT2FIX( object->GetKerning( FIX2UINT( aFirst ), FIX2UINT( aSecond ), FIX2UINT( aCharacterSize ) ) );
}

/* call-seq:
 *   font.getLineSpacing( characterSize )	-> fixnum
 *
 * Get the line spacing.
 *
 * Line spacing is the vertical offset to apply between two consecutive lines of text.
 */
static VALUE Font_GetLineSpacing( VALUE self, VALUE aCharacterSize )
{
	sf::Font *object = NULL;
	Data_Get_Struct( self, sf::Font, object );
	return INT2FIX( object->GetLineSpacing( FIX2UINT( aCharacterSize ) ) );
}

/* call-seq:
 *   font.getImage( characterSize )	-> image
 *
 * Retrieve the image containing the loaded glyphs of a certain size.
 *
 * The contents of the returned image changes as more glyphs are requested, thus it is not very relevant. 
 * It is mainly used internally by SFML::Text.
 */
static VALUE Font_GetImage( VALUE self, VALUE aCharacterSize )
{
	sf::Font *object = NULL;
	Data_Get_Struct( self, sf::Font, object );
	const sf::Image& image = object->GetImage( FIX2UINT( aCharacterSize ) );
	VALUE rbImage = Data_Wrap_Struct( globalImageClass, 0, 0, const_cast<sf::Image *>( &image ) );
	rb_iv_set( rbImage, "@__owner_ref", self );
	return rbImage;
}

/* call-seq:
 *   Font.new()				-> font
 *   Font.new( filename )	-> font
 *
 * Will create a new font instance.
 * 
 * If a filename argument is specified then font#loadFromFile will be called on the created instance.
 */
static VALUE Font_Initialize( int argc, VALUE *args, VALUE self )
{
	if( argc > 0 )
	{
		rb_funcall2( self, rb_intern( "loadFromFile" ), argc, args );
	}
	return self;
}

static VALUE Font_InitializeCopy( VALUE self, VALUE aSource )
{
	sf::Font *object = NULL;
	Data_Get_Struct( self, sf::Font, object );
	sf::Font *source = NULL;
	Data_Get_Struct( aSource, sf::Font, source );
	*object = *source;
}

static VALUE Font_Alloc( VALUE aKlass )
{
	sf::Font *object = new sf::Font();
	return Data_Wrap_Struct( aKlass, 0, Font_Free, object );
}

/* call-seq:
 *   Font.getDefaultFont()	-> font
 *
 * Return the default built-in font.
 *
 * This font is provided for convenience, it is used by SFML::Text instances by default. It is provided so that users 
 * don't have to provide and load a font file in order to display text on screen. The font used is Arial.
 */
static VALUE Font_GetDefaultFont( VALUE aKlass )
{
	const sf::Font& font = sf::Font::GetDefaultFont();
	VALUE rbFont = Data_Wrap_Struct( globalFontClass, 0, 0, const_cast<sf::Font *>( &font ) );
	rb_obj_call_init( rbFont, 0, 0 );
	return rbFont;
}

void Init_Font( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Class for loading and manipulating character fonts.
 *
 * Fonts can be loaded from a file or from memory, from the most common types of fonts.
 *
 * See the loadFromFile method for the complete list of supported formats.
 * 
 * Once it is loaded, a SFML::Font instance provides three types of informations about the font:
 *
 *    - Global metrics, such as the line spacing
 *    - Per-glyph metrics, such as bounding box or kerning
 *    - Pixel representation of glyphs
 *
 * Fonts alone are not very useful: they hold the font data but cannot make anything useful of it. To do so you need 
 * to use the SFML::Text class, which is able to properly output text with several options such as character size, style,
 * color, position, rotation, etc. This separation allows more flexibility and better performances: indeed a sf::Font 
 * is a heavy resource, and any operation on it is slow (often too slow for real-time applications). On the other side,
 * a SFML::Text is a lightweight object which can combine the glyphs data and metrics of a SFML::Font to display any text
 * on a render target. Note that it is also possible to bind several SFML::Text instances to the same sf::Font.
 *
 * It is important to note that the sf::Text instance doesn't copy the font that it uses, it only keeps a reference to 
 * it. Thus, a SFML::Font must not be destructed while it is used by a SFML::Text (i.e. never write a function that uses
 * a local SFML::Font instance for creating a text).
 *
 * Usage example:
 *
 *   # Declare a new font
 *   font = SFML::Font.new
 * 
 *   # Load it from a file
 *   if font.LoadFromFile("arial.ttf") == false
 *     # error...
 *   end
 * 
 *   # Create a text which uses our font
 *   text1 = SFML::Text.new
 *   text1.setFont( font )
 *   text1.setCharacterSize( 30 )
 *   text1.setStyle( sf::Text::Regular )
 * 
 *   # Create another text using the same font, but with different parameters
 *   text2 = SFML::Text.new
 *   text2.setFont( font )
 *   text2.setCharacterSize( 50 )
 *   text2.setStyle( SFML::Text::Italic )
 *
 * Apart from loading font files, and passing them to instances of SFML::Text, you should normally not have to deal 
 * directly with this class. However, it may be useful to access the font metrics or rasterized glyphs for advanced 
 * usage.
 */
	globalFontClass = rb_define_class_under( sfml, "Font", rb_cObject );
	
	// Class methods
	//rb_define_singleton_method( globalFontClass, "new", Font_New, -1 );
	rb_define_alloc_func( globalFontClass, Font_Alloc );
	rb_define_singleton_method( globalFontClass, "getDefaultFont", Font_GetDefaultFont, 0 );
	
	// Instance methods
	rb_define_method( globalFontClass, "initialize", Font_Initialize, -1 );
	rb_define_method( globalFontClass, "initialize_copy", Font_InitializeCopy, 1 );
	rb_define_method( globalFontClass, "loadFromFile", Font_LoadFromFile, 1 );
	rb_define_method( globalFontClass, "getGlyph", Font_GetGlyph, 3 );
	rb_define_method( globalFontClass, "getKerning", Font_GetKerning, 3 );
	rb_define_method( globalFontClass, "getLineSpacing", Font_GetLineSpacing, 1 );
	rb_define_method( globalFontClass, "getImage", Font_GetImage, 1 );
	
	// Class Aliases
	rb_define_alias( CLASS_OF( globalFontClass ), "get_default_font", "getDefaultFont" );
	rb_define_alias( CLASS_OF( globalFontClass ), "defaultFont", "getDefaultFont" );
	rb_define_alias( CLASS_OF( globalFontClass ), "default_font", "getDefaultFont" );
	rb_define_alias( CLASS_OF( globalFontClass ), "DefaultFont", "getDefaultFont" );
	
	// Instance Aliases
	rb_define_alias( globalFontClass , "load_from_file", "loadFromFile" );
	rb_define_alias( globalFontClass , "loadFile", "loadFromFile" );
	rb_define_alias( globalFontClass , "load_file", "loadFromFile" );
	
	rb_define_alias( globalFontClass , "get_glyph", "getGlyph" );
	rb_define_alias( globalFontClass , "get_kerning", "getKerning" );
	rb_define_alias( globalFontClass , "get_line_spacing", "getLineSpacing" );
	rb_define_alias( globalFontClass , "get_image", "getImage" );
}

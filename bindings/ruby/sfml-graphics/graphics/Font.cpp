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

static void Font_Free( sf::Font *anObject )
{
	delete anObject;
}

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

static VALUE Font_GetKerning( VALUE self, VALUE aFirst, VALUE aSecond, VALUE aCharacterSize )
{
	sf::Font *object = NULL;
	Data_Get_Struct( self, sf::Font, object );
	return INT2FIX( object->GetKerning( FIX2UINT( aFirst ), FIX2UINT( aSecond ), FIX2UINT( aCharacterSize ) ) );
}

static VALUE Font_GetLineSpacing( VALUE self, VALUE aCharacterSize )
{
	sf::Font *object = NULL;
	Data_Get_Struct( self, sf::Font, object );
	return INT2FIX( object->GetLineSpacing( FIX2UINT( aCharacterSize ) ) );
}

static VALUE Font_New( int argc, VALUE *args, VALUE aKlass )
{
	sf::Font *object = new sf::Font();
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, Font_Free, object );
	rb_obj_call_init( rbData, argc, args );
	return rbData;
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
}

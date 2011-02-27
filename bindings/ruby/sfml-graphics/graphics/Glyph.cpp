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
 
#include "Glyph.hpp"
#include "Rect.hpp"
#include "main.hpp"
#include <SFML/Graphics/Glyph.hpp>

VALUE globalGlyphClass;

/* External classes */
extern VALUE globalRectClass;

/* call-seq:
 *   Glyph.new()	-> glyph
 * 
 * Create a new glyph instance.
 */
static VALUE Glyph_Initialize( VALUE self )
{	
	rb_iv_set( self, "@advance", INT2FIX( 0 ) );
	rb_iv_set( self, "@bounds", rb_funcall( globalRectClass, rb_intern( "new" ), 0 ) );
	rb_iv_set( self, "@subRect", rb_funcall( globalRectClass, rb_intern( "new" ), 0 ) );
	return self;
}

void Init_Glyph( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Structure describing a glyph.
 *
 * A glyph is the visual representation of a character.
 *
 * The SFML::Glyph structure provides the information needed to handle the glyph:
 *
 *   - its coordinates in the font's image
 *   - its bounding rect
 *   - the offset to apply to get the starting position of the next glyph
 */
	globalGlyphClass = rb_define_class_under( sfml, "Glyph", rb_cObject );
	
	// Instance methods
	rb_define_method( globalGlyphClass, "initialize", Glyph_Initialize, 0 );
	
	// Attribute accessors
	rb_define_attr( globalGlyphClass, "advance", 1, 1 );
	rb_define_attr( globalGlyphClass, "bounds", 1, 1 );
	rb_define_attr( globalGlyphClass, "subRect", 1, 1 );
	
	// Aliases
	rb_define_alias( globalGlyphClass, "sub_rect", "subRect" );
	rb_define_alias( globalGlyphClass, "sub_rect=", "subRect=" );
}

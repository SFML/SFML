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

#include "main.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "NonCopyable.hpp"
#include "Color.hpp"
#include "Rect.hpp"
#include "Drawable.hpp"
#include "Font.hpp"
#include "Glyph.hpp"
#include "Image.hpp"
#include "Renderer.hpp"
#include "RenderImage.hpp"
#include "RenderTarget.hpp"
#include "RenderWindow.hpp"
#include "Shape.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"
#include "Text.hpp"
#include "View.hpp"

#include <SFML/Graphics.hpp>

VALUE globalBlendNamespace;

/* External classes */
VALUE globalWindowClass;

static bool CheckDependencies( void )
{
	if( rb_cvar_defined( globalSFMLNamespace, rb_intern( "WindowLoaded" ) ) == Qtrue )
	{
		return true;
	}

	return false;
}

/* Available blending modes for drawable objects. */
static void CreateBlendEnum( void )
{
	globalBlendNamespace = rb_define_module_under( globalSFMLNamespace, "Blend" );
	rb_define_const( globalBlendNamespace, "Alpha", INT2FIX( sf::Blend::Alpha ) );
	rb_define_const( globalBlendNamespace, "Add", INT2FIX( sf::Blend::Add ) );
	rb_define_const( globalBlendNamespace, "Multiply", INT2FIX( sf::Blend::Multiply ) );
	rb_define_const( globalBlendNamespace, "None", INT2FIX( sf::Blend::None ) );
}

void Init_graphics( void )
{
	/* SFML namespace which contains the classes of this module. */
	globalSFMLNamespace = rb_define_module( "SFML" );
	if( CheckDependencies() == false )
	{
		rb_raise( rb_eRuntimeError, "This module depends on sfml-window" );
	}
	globalVector2Class = RetrieveSFMLClass( "Vector2" );
	globalVector3Class = RetrieveSFMLClass( "Vector3" );
	globalWindowClass  = RetrieveSFMLClass( "Window" );
	globalNonCopyableModule = RetrieveSFMLClass( "NonCopyable" );
	rb_define_const(globalSFMLNamespace, "GraphicsLoaded", Qtrue);

	CreateBlendEnum();

	Init_Color();
	Init_Rect();
	Init_Drawable();
	Init_Glyph();
	Init_Font();
	Init_Image();
	Init_Renderer();
	Init_RenderTarget();
	Init_RenderImage();
	Init_RenderWindow();
	Init_Shape();
	Init_Shader();
	Init_Sprite();
	Init_Text();
	Init_View();
}

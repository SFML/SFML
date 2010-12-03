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
 
#include "Renderer.hpp"
#include "Color.hpp"
#include "Rect.hpp"
#include "main.hpp"
#include <SFML/Graphics/Renderer.hpp>

VALUE globalRendererClass;

/* External classes */
extern VALUE globalColorClass;
extern VALUE globalImageClass;
extern VALUE globalShaderClass;
extern VALUE globalNonCopyableModule;

/* call-seq:
 *   renderer.saveGLStates()
 *
 * Save the current OpenGL render states and matrices. 
 */
static VALUE Renderer_SaveGLStates( VALUE self )
{
	sf::Renderer *object = NULL;
	Data_Get_Struct( self, sf::Renderer, object );
	object->SaveGLStates();
	return Qnil;
}

/* call-seq:
 *   renderer.restoreGLStates()
 *
 * Restore the previously saved OpenGL render states and matrices. 
 */
static VALUE Renderer_RestoreGLStates( VALUE self )
{
	sf::Renderer *object = NULL;
	Data_Get_Struct( self, sf::Renderer, object );
	object->RestoreGLStates();
	return Qnil;
}

/* call-seq:
 *   renderer.clear()
 *
 * Clear the color buffer. 
 */
static VALUE Renderer_Clear( VALUE self, VALUE aColor )
{
	VALUE temp = Color_ForceType( aColor );
	sf::Color color;
	color.r = FIX2UINT( Color_GetR( temp ) );
	color.g = FIX2UINT( Color_GetG( temp ) );
	color.b = FIX2UINT( Color_GetB( temp ) );
	color.a = FIX2UINT( Color_GetA( temp ) );
	sf::Renderer *object = NULL;
	Data_Get_Struct( self, sf::Renderer, object );
	object->Clear( color );
	return Qnil;
}

/* call-seq:
 *   renderer.pushStates()
 *
 * Save the current render states. 
 */
static VALUE Renderer_PushStates( VALUE self )
{
	sf::Renderer *object = NULL;
	Data_Get_Struct( self, sf::Renderer, object );
	object->PushStates();
	return Qnil;
}

/* call-seq:
 *   renderer.popStates()
 *
 * Restore the previously saved render states. 
 */
static VALUE Renderer_PopStates( VALUE self )
{
	sf::Renderer *object = NULL;
	Data_Get_Struct( self, sf::Renderer, object );
	object->PopStates();
	return Qnil;
}

/* call-seq:
 *   renderer.setColor( color )
 *
 * Set the current global color.
 *
 * This color will be modulated with each vertex's color. Note: any call to this function after a call to BeginBatch
 * will be ignored, and delayed until BeginBatch is called again.
 */
static VALUE Renderer_SetColor( VALUE self, VALUE aColor )
{
	VALUE temp = Color_ForceType( aColor );
	sf::Color color;
	color.r = FIX2UINT( Color_GetR( temp ) );
	color.g = FIX2UINT( Color_GetG( temp ) );
	color.b = FIX2UINT( Color_GetB( temp ) );
	color.a = FIX2UINT( Color_GetA( temp ) );
	sf::Renderer *object = NULL;
	Data_Get_Struct( self, sf::Renderer, object );
	object->SetColor( color );
	return Qnil;
}

/* call-seq:
 *   renderer.applyColor( color )
 *
 * Modulate the current global color with a new one.
 *
 * This color will be modulated with each vertex's color. Note: any call to this function after a call to BeginBatch 
 * will be ignored, and delayed until BeginBatch is called again.
 */
static VALUE Renderer_ApplyColor( VALUE self, VALUE aColor )
{
	VALUE temp = Color_ForceType( aColor );
	sf::Color color;
	color.r = FIX2UINT( Color_GetR( temp ) );
	color.g = FIX2UINT( Color_GetG( temp ) );
	color.b = FIX2UINT( Color_GetB( temp ) );
	color.a = FIX2UINT( Color_GetA( temp ) );
	sf::Renderer *object = NULL;
	Data_Get_Struct( self, sf::Renderer, object );
	object->ApplyColor( color );
	return Qnil;
}

/* call-seq:
 *   renderer.setViewport( rectangle )
 *
 * Set the current viewport.
 *
 * Note: any call to this function after a call to BeginBatch will be ignored, and delayed until BeginBatch is called again.
 */
static VALUE Renderer_SetViewport( VALUE self, VALUE aRect )
{
	VALUE temp = Rect_ForceType( aRect );
	sf::IntRect rectangle;
	rectangle.Left   = FIX2UINT( Rect_GetLeft( temp ) );
	rectangle.Top    = FIX2UINT( Rect_GetTop( temp ) );
	rectangle.Width  = FIX2UINT( Rect_GetWidth( temp ) );
	rectangle.Height = FIX2UINT( Rect_GetHeight( temp ) );
	sf::Renderer *object = NULL;
	Data_Get_Struct( self, sf::Renderer, object );
	object->SetViewport( rectangle );
	return Qnil;
}

/* call-seq:
 *   renderer.setBlendMode( mode )
 *
 * Set the current alpha-blending mode.
 *
 * Note: any call to this function after a call to BeginBatch will be ignored, and delayed until BeginBatch is called again.
 */
static VALUE Renderer_SetBlendMode( VALUE self, VALUE aMode )
{
	sf::Renderer *object = NULL;
	Data_Get_Struct( self, sf::Renderer, object );
	object->SetBlendMode( static_cast< sf::Blend::Mode >( FIX2INT( aMode ) ) );
	return Qnil;
}

/* call-seq:
 *   renderer.setTexture( texture )
 *
 * Set the current texture.
 *
 * Note: any call to this function after a call to BeginBatch will be ignored, and delayed until BeginBatch is called again.
 */
static VALUE Renderer_SetTexture( VALUE self, VALUE aTexture )
{
	VALIDATE_CLASS( aTexture, globalImageClass, "texture" );
	sf::Renderer *object = NULL;
	Data_Get_Struct( self, sf::Renderer, object );
	sf::Image * texture = NULL;
	Data_Get_Struct( aTexture, sf::Image, texture );
	object->SetTexture( texture );
	return Qnil;
}

/* call-seq:
 *   renderer.setShader( shader )
 *
 * Set the current shader.
 *
 * Note: any call to this function after a call to BeginBatch will be ignored, and delayed until BeginBatch is called again.
 */
static VALUE Renderer_SetShader( VALUE self, VALUE aShader )
{
	VALIDATE_CLASS( aShader, globalShaderClass, "shader" );
	sf::Renderer *object = NULL;
	Data_Get_Struct( self, sf::Renderer, object );
	sf::Shader * shader = NULL;
	Data_Get_Struct( aShader, sf::Shader, shader );
	object->SetShader( shader );
	return Qnil;
}

/* call-seq:
 *   renderer.begin( type )
 *
 * Begin rendering a new geometry batch.
 * 
 * You need to call SFML::Renderer#end to complete the batch and trigger the actual rendering of the geometry that you
 * passed between begin() and end().
 *
 * Usage:
 *
 *   renderer.begin( SFML::Renderer::TriangleList )
 *   renderer.addVertex(...)
 *   renderer.addVertex(...)
 *   renderer.addVertex(...)
 *   renderer.end()
 *
 */
static VALUE Renderer_Begin( VALUE self, VALUE aType )
{
	sf::Renderer *object = NULL;
	Data_Get_Struct( self, sf::Renderer, object );
	object->Begin( static_cast< sf::Renderer::PrimitiveType >( FIX2INT( aType ) ) );
	return Qnil;
}

/* call-seq:
 *   renderer.end()
 *
 * End the current geometry batch and render it. 
 */
static VALUE Renderer_End( VALUE self )
{
	sf::Renderer *object = NULL;
	Data_Get_Struct( self, sf::Renderer, object );
	object->End();
	return Qnil;
}

/* call-seq:
 *   renderer.addVertex( x, y )
 *   renderer.addVertex( x, y, u, v )
 *   renderer.addVertex( x, y color )
 *   renderer.addVertex( x, y, u, v, color )
 *
 * This function adds a new vertex to the current batch.
 */
static VALUE Renderer_AddVertex( int argc, VALUE *args, VALUE self )
{
	sf::Renderer *object = NULL;
	Data_Get_Struct( self, sf::Renderer, object );
	switch( argc )
	{
		case 2:
		{
			object->AddVertex( NUM2DBL( args[0] ), NUM2DBL( args[1] ) );
			break;
		}
		case 3:
		{
			VALUE temp = Color_ForceType( args[2] );
			sf::Color color;
			color.r = FIX2UINT( Color_GetR( temp ) );
			color.g = FIX2UINT( Color_GetG( temp ) );
			color.b = FIX2UINT( Color_GetB( temp ) );
			color.a = FIX2UINT( Color_GetA( temp ) );
			object->AddVertex( NUM2DBL( args[0] ), NUM2DBL( args[1] ), color );
			break;
		}
		case 4:
		{
			object->AddVertex( NUM2DBL( args[0] ), NUM2DBL( args[1] ), NUM2DBL( args[2] ), NUM2DBL( args[3] ) );
			break;
		}
		case 5:
		{
			VALUE temp = Color_ForceType( args[4] );
			sf::Color color;
			color.r = FIX2UINT( Color_GetR( temp ) );
			color.g = FIX2UINT( Color_GetG( temp ) );
			color.b = FIX2UINT( Color_GetB( temp ) );
			color.a = FIX2UINT( Color_GetA( temp ) );
			object->AddVertex( NUM2DBL( args[0] ), NUM2DBL( args[1] ), NUM2DBL( args[2] ), NUM2DBL( args[3] ), color );
			break;
		}
		default:
			rb_raise( rb_eArgError, "Expected 2..5 arguments but was given %d", argc );
	}
	return Qnil;
}

static void DefinePrimitiveTypesEnum( void )
{
	rb_define_const( globalRendererClass, "TriangleList", INT2FIX( sf::Renderer::TriangleList ) );
	rb_define_const( globalRendererClass, "TriangleStrip", INT2FIX( sf::Renderer::TriangleStrip ) );
	rb_define_const( globalRendererClass, "TriangleFan", INT2FIX( sf::Renderer::TriangleFan ) );
	rb_define_const( globalRendererClass, "QuadList", INT2FIX( sf::Renderer::QuadList ) );
}

void Init_Renderer( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Handles the low-level rendering (states and geometry).
 *
 * SFML::Renderer is the abstraction layer between SFML code and the low-level drawing API (OpenGL).
 *
 * It manages render states efficiently, and provides a lightweight abstraction for rendering geometry.
 *
 * The purpose of this class is to provide a single abstract entry point for everything related to low-level 
 * rendering. Hiding everything behind SFML::Renderer makes optimizing easy, as well as porting to other technologies 
 * in the future (like OpenGL ES or OpenGL 3.x).
 *
 * This class is mainly meant for internal usage, you should never care about it unless you write your own 
 * SFML::Drawable class that uses raw geometry in its Render function. 
 */
	globalRendererClass = rb_define_class_under( sfml, "Renderer", rb_cObject );
	rb_include_module( globalRendererClass, globalNonCopyableModule );
	DefinePrimitiveTypesEnum();
	
	// Instance methods
	rb_define_method( globalRendererClass, "saveGLStates", Renderer_SaveGLStates, 0 );
	rb_define_method( globalRendererClass, "restoreGLStates", Renderer_RestoreGLStates, 0 );
	rb_define_method( globalRendererClass, "clear", Renderer_Clear, 1 );
	rb_define_method( globalRendererClass, "pushStates", Renderer_PushStates, 0 );
	rb_define_method( globalRendererClass, "popStates", Renderer_PopStates, 0 );
	rb_define_method( globalRendererClass, "setColor", Renderer_SetColor, 1 );
	rb_define_method( globalRendererClass, "applyColor", Renderer_ApplyColor, 1 );
	rb_define_method( globalRendererClass, "setViewport", Renderer_SetViewport, 1 );
	rb_define_method( globalRendererClass, "setBlendMode", Renderer_SetBlendMode, 1 );
	rb_define_method( globalRendererClass, "setTexture", Renderer_SetTexture, 1 );
	rb_define_method( globalRendererClass, "setShader", Renderer_SetShader, 1 );
	rb_define_method( globalRendererClass, "begin", Renderer_Begin, 1 );
	rb_define_method( globalRendererClass, "end", Renderer_End, 0 );
	rb_define_method( globalRendererClass, "addVertex", Renderer_AddVertex, -1 );
	
	// Instance Aliases
	rb_define_alias( globalRendererClass, "save_gl_states", "saveGLStates" );
	rb_define_alias( globalRendererClass, "restore_gl_states", "restoreGLStates" );
	rb_define_alias( globalRendererClass, "push_states", "pushStates" );
	rb_define_alias( globalRendererClass, "pop_states", "popStates" );
	
	rb_define_alias( globalRendererClass, "color=", "setColor" );
	rb_define_alias( globalRendererClass, "apply_color", "applyColor" );
	rb_define_alias( globalRendererClass, "viewport=", "setViewport" );
	rb_define_alias( globalRendererClass, "blendMode=", "setBlendMode" );
	rb_define_alias( globalRendererClass, "blend_mode=", "setBlendMode" );
	rb_define_alias( globalRendererClass, "texture=", "setTexture" );
	rb_define_alias( globalRendererClass, "shader=", "setShader" );
	rb_define_alias( globalRendererClass, "add_vertex", "addVertex" );
}

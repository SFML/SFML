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
 
#include "Shader.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Image.hpp"
#include "main.hpp"
#include <SFML/Graphics/Shader.hpp>

VALUE globalShaderClass;
/* External classes */
extern VALUE globalVector2Class;
extern VALUE globalVector3Class;
extern VALUE globalImageClass;

static void Shader_Free( sf::Shader *anObject )
{
	delete anObject;
}

/* call-seq:
 *   shader.loadFromFile( filename )	-> true or false
 *
 * Load the shader from a file.
 *
 * The source must be a text file containing a valid fragment shader in GLSL language. GLSL is a C-like language 
 * dedicated to OpenGL shaders; you'll probably need to read a good documentation for it before writing your own shaders.
 */
static VALUE Shader_LoadFromFile( VALUE self, VALUE aFileName )
{
	sf::Shader *object = NULL;
	Data_Get_Struct( self, sf::Shader, object );
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
 *   shader.loadFromMemory( filename )	-> true or false
 *
 * Load the shader from a source code in memory.
 *
 * The source code must be a valid fragment shader in GLSL language. GLSL is a C-like language dedicated to OpenGL 
 * shaders; you'll probably need to read a good documentation for it before writing your own shaders.
 */
static VALUE Shader_LoadFromMemory( VALUE self, VALUE aShader )
{
	sf::Shader *object = NULL;
	Data_Get_Struct( self, sf::Shader, object );
	if( object->LoadFromMemory( rb_string_value_cstr( &aShader ) ) == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

/* call-seq:
 *   shader.setParameter( name, x )
 *   shader.setParameter( name, x, y )
 *   shader.setParameter( name, x, y, z )
 *   shader.setParameter( name, x, y, z, w )
 *   shader.setParameter( name, vector2 )
 *   shader.setParameter( name, vector3 )
 *
 * Change a vector parameter of the shader. 
 */
static VALUE Shader_SetParameter( int argc, VALUE *args, VALUE self )
{
	sf::Shader *object = NULL;
	Data_Get_Struct( self, sf::Shader, object );
	const char * name;
	
	switch( argc )
	{
		case 2:
		{
		
			name = rb_string_value_cstr( &args[0] );
			if( rb_obj_is_kind_of( args[1], rb_cFloat ) == Qtrue )
			{
				object->SetParameter( name, NUM2DBL( args[1] ) );
			}
			else if( rb_obj_is_kind_of( args[1], globalVector2Class ) == Qtrue || 
				 ( rb_obj_is_kind_of( args[1], rb_cArray ) == Qtrue && 
				   FIX2INT( rb_funcall( args[1], rb_intern( "size" ), 0 ) ) == 2 )
				)
			{
				VALUE arg1 = Vector2_ForceType( args[1] );
				sf::Vector2f vector;
				vector.x = NUM2DBL( Vector2_GetX( args[1] ) );
				vector.y = NUM2DBL( Vector2_GetY( args[1] ) );
				object->SetParameter( name, vector );
			}
			else if( rb_obj_is_kind_of( args[1], globalVector3Class ) == Qtrue || 
				 ( rb_obj_is_kind_of( args[1], rb_cArray ) == Qtrue && 
				   FIX2INT( rb_funcall( args[1], rb_intern( "size" ), 0 ) ) == 3 )
				)
			{
				VALUE arg1 = Vector3_ForceType( args[1] );
				sf::Vector3f vector;
				vector.x = NUM2DBL( Vector3_GetX( args[1] ) );
				vector.y = NUM2DBL( Vector3_GetY( args[1] ) );
				vector.z = NUM2DBL( Vector3_GetZ( args[1] ) );
				object->SetParameter( name, vector );
			}
			break;
		}
		case 3:
			name = rb_string_value_cstr( &args[0] );
			object->SetParameter( name, NUM2DBL( args[1] ), NUM2DBL( args[2] ) );
			break;
		case 4:
			name = rb_string_value_cstr( &args[0] );
			object->SetParameter( name, NUM2DBL( args[1] ), NUM2DBL( args[2] ), NUM2DBL( args[3] ) );
			break;
		case 5:
			name = rb_string_value_cstr( &args[0] );
			object->SetParameter( name, NUM2DBL( args[1] ), NUM2DBL( args[2] ), NUM2DBL( args[3] ), NUM2DBL( args[4] ) );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 2..5 arguments but was given %d", argc );
	}
	return Qnil;
}

/* call-seq:
 *   shader.setParameter( name, texture )
 *
 * Change a texture parameter of the shader.
 *
 * name is the name of the texture to change in the shader. To tell the shader to use the current texture of the object 
 * being drawn, pass Shader::CurrentTexture. Example:
 * 
 *   # These are the variables in the pixel shader
 *   uniform sampler2D current;
 *   uniform sampler2D other;
 * 
 *   image = SFML::Image.new
 *   ...
 *   shader.setParameter( "current", SFML::Shader::CurrentTexture )
 *   shader.setParameter( "other", image )
 *
* It is important to note that texture must remain alive as long as the shader uses it, no copy is made internally.
 */
static VALUE Shader_SetTexture( VALUE self, VALUE aName, VALUE aTexture )
{
	VALIDATE_CLASS( aName, rb_cString, "name" );
	VALIDATE_CLASS( aTexture, globalImageClass, "texture" );
	sf::Image *texture = NULL;
	Data_Get_Struct( self, sf::Image, texture );
	const char * name = rb_string_value_cstr( &aName );
	sf::Shader *object = NULL;
	Data_Get_Struct( self, sf::Shader, object );
	object->SetTexture( name, *texture );
	return Qnil;
}

/* call-seq:
 *   shader.bind()
 *
 * Bind the shader for rendering (activate it).
 *
 * This function is normally for internal use only, unless you want to use the shader with a custom OpenGL rendering 
 * instead of a SFML drawable.
 */
static VALUE Shader_Bind( VALUE self )
{
	sf::Shader *object = NULL;
	Data_Get_Struct( self, sf::Shader, object );
	object->Bind();
	return Qnil;
}

/* call-seq:
 *   shader.unbind()
 *
 * Unbind the shader (deactivate it).
 *
 * This function is normally for internal use only, unless you want to use the shader with a custom OpenGL rendering 
 * instead of a SFML drawable.
 */
static VALUE Shader_Unbind( VALUE self )
{
	sf::Shader *object = NULL;
	Data_Get_Struct( self, sf::Shader, object );
	object->Unbind();
	return Qnil;
}

/* call-seq:
 *   Shader.new()			-> shader
 *   Shader.new( filename )	-> shader
 *
 * Will create a new shader instance.
 * 
 * If a filename argument is specified then shader#loadFromFile will be called on the created instance.
 */
static VALUE Shader_Initialize( int argc, VALUE *args, VALUE self )
{
	if( argc > 0 )
	{
		rb_funcall2( self, rb_intern( "loadFromFile" ), argc, args );
	}
	return self;
}

static VALUE Shader_InitializeCopy( VALUE self, VALUE aSource )
{
	sf::Shader *object = NULL;
	Data_Get_Struct( self, sf::Shader, object );
	sf::Shader *source = NULL;
	Data_Get_Struct( aSource, sf::Shader, source );
	*object = *source;
}

/* call-seq:
 *   Shader.new()
 *
 * Create a new shader.
 */
static VALUE Shader_Alloc( VALUE aKlass )
{
	sf::Shader *object = new sf::Shader();
	return Data_Wrap_Struct( aKlass, 0, Shader_Free, object );
}

/* call-seq:
 *   Shader.isAvailable()
 *
 * Tell whether or not the system supports shaders.
 *
 * This function should always be called before using the shader features. If it returns false, then any attempt to 
 * use SFML::Shader will fail.
 */
static VALUE Shader_IsAvailable( VALUE aKlass )
{
	return ( sf::Shader::IsAvailable() == true ? Qtrue : Qfalse );
}

static VALUE CreateCurrentTextureWrapper( void )
{
	sf::Image * image = const_cast< sf::Image * >( &sf::Shader::CurrentTexture );
	VALUE rbData = Data_Wrap_Struct( globalImageClass, 0, 0, image );
	rb_obj_call_init( rbData, 0, 0 );
	return rbData;
}

void Init_Shader( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Pixel/fragment shader class.
 *
 * Pixel shaders (or fragment shaders) are programs written using a specific language, executed directly by the 
 * graphics card and allowing to apply per-pixel real-time operations to the rendered entities.
 *
 * Pixel shaders are written in GLSL, which is a C-like language dedicated to OpenGL shaders. You'll probably need 
 * to learn its basics before writing your own shaders for SFML.
 *
 * Like any program, a shader has its own variables that you can set from your Ruby application. SFML::Shader 
 * handles 3 different types of variables:
 *
 *   - floats
 *   - vectors (2, 3 or 4 components)
 *   - textures
 *
 * The value of the variables can be changed at any time with either SFML::Shader#setParameter or SFML::Shader#SetTexture:
 *
 *   shader.setParameter( "offset", 2.0 )
 *   shader.setParameter( "color", 0.5, 0.8, 0.3 )
 *   shader.setTexture( "image", image ) # image is a SFML::Image
 *   shader.setTexture( "current", SFML::Shader::CurrentTexture )
 *
 * Shader::CurrentTexture is a special value that represents the texture that the object being drawn is using.
 *
 * To apply a shader to a drawable, you must pass it as an additional parameter to the Draw function:
 *
 *   window.draw( sprite, shader )
 *
 * Shaders can be used on any drawable, but they are mainly made for SFML::Sprite. Using a shader on a SFML::String is 
 * more limited, because the texture of the string is not the actual text that you see on screen, it is a big image 
 * containing all the characters of the font in an arbitrary order. Thus, texture lookups on pixels other than the 
 * current one may not give you the expected result. Using a shader with SFML::Shape is even more limited, as shapes 
 * don't use any texture.
 *
 * Shaders can also be used to apply global post-effects to the current contents of the target 
 * (like the old sf::PostFx class in SFML 1). This can be done in two different ways:
 *
 *   - draw everything to a SFML::RenderImage, then draw it to the main target using the shader
 *   - draw everything directly to the main target, then use SFML::Image::CopyScreen to copy its contents to an image 
 *     and draw it to the main target using the shader
 *
 * The first technique is more optimized because it doesn't involve retrieving the target's pixels to system memory, 
 * but the second one doesn't impact the rendering process and can be easily inserted anywhere.
 *
 * Like SFML::Image that can be used as a raw OpenGL texture, SFML::Shader can also be used directly as a raw fragment
 * shader for custom OpenGL geometry.
 *
 *   window.setActive()
 *   shader.bind()
 *   # ... render OpenGL geometry ...
 *   shader.unbind()
 *
 */
	globalShaderClass = rb_define_class_under( sfml, "Shader", rb_cObject );
	
	// Class methods
	//rb_define_singleton_method( globalShaderClass, "new", Shader_New, -1 );
	rb_define_alloc_func( globalShaderClass, Shader_Alloc );
	rb_define_singleton_method( globalShaderClass, "isAvailable", Shader_IsAvailable, 0 );
	
	// Class Constants
	rb_define_const( globalShaderClass, "CurrentTexture", CreateCurrentTextureWrapper() );
	
	// Instance methods
	rb_define_method( globalShaderClass, "initialize", Shader_Initialize, -1 );
	rb_define_method( globalShaderClass, "initialize_copy", Shader_InitializeCopy, 1 );
	rb_define_method( globalShaderClass, "loadFromFile", Shader_LoadFromFile, 1 );
	rb_define_method( globalShaderClass, "loadFromMemory", Shader_LoadFromMemory, 1 );
	rb_define_method( globalShaderClass, "setParameter", Shader_SetParameter, -1 );
	rb_define_method( globalShaderClass, "setTexture", Shader_SetTexture, 2 );
	rb_define_method( globalShaderClass, "bind", Shader_Bind, 0 );
	rb_define_method( globalShaderClass, "unbind", Shader_Unbind, 0 );
	
	// Class Aliases
	rb_define_alias( CLASS_OF( globalShaderClass ), "is_available", "isAvailable" );
	rb_define_alias( CLASS_OF( globalShaderClass ), "available?", "isAvailable" );
	
	// Instance Aliases
	rb_define_alias( globalShaderClass, "load_from_file", "loadFromFile" );
	rb_define_alias( globalShaderClass, "loadFile", "loadFromFile" );
	rb_define_alias( globalShaderClass, "load_file", "loadFromFile" );
	
	rb_define_alias( globalShaderClass, "load_from_memory", "loadFromMemory" );
	rb_define_alias( globalShaderClass, "loadMemory", "loadFromMemory" );
	rb_define_alias( globalShaderClass, "load_memory", "loadFromMemory" );
	
	rb_define_alias( globalShaderClass, "set_parameter", "setParameter" );
	rb_define_alias( globalShaderClass, "[]=", "setParameter" );
	
	rb_define_alias( globalShaderClass, "set_texture", "setTexture" );
}

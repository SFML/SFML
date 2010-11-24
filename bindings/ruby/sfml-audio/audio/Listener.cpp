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
 
#include "Listener.hpp"
#include "Vector3.hpp"
#include "main.hpp"
#include <SFML/Audio/Listener.hpp>

VALUE globalListenerModule;

/* External class */
extern VALUE globalVector3Class;

/* call-seq:
 *   Listener.setGlobalVolume( volume )
 *
 * Change the global volume of all the sounds and musics.
 *
 * The volume is a number between 0 and 100; it is combined with the individual volume of each sound / music. The 
 * default value for the volume is 100 (maximum).
 */
static VALUE Listener_SetGlobalVolume( VALUE self, VALUE aVolume )
{
	sf::Listener::SetGlobalVolume( NUM2DBL( aVolume ) );
	return Qnil;
}

/* call-seq:
 *   Listener.getGlobalVolume()	-> float
 *
 * Get the current value of the global volume. 
 */
static VALUE Listener_GetGlobalVolume( VALUE self )
{
	return rb_float_new( sf::Listener::GetGlobalVolume() );
}

/* call-seq:
 *   Listener.setPosition( vector3 )
 *   Listener.setPosition( x, y, z )
 *
 * Set the position of the listener in the scene.
 *
 * The default listener's position is (0, 0, 0).
 */
static VALUE Listener_SetPosition( int argc, VALUE *args, VALUE self )
{
	VALUE temp;
	float x, y, z;
	switch( argc )
	{
		case 3:
			x = NUM2DBL( args[0] );
			y = NUM2DBL( args[1] );
			z = NUM2DBL( args[2] );
			break;
		case 1:
			temp = Vector3_ForceType( args[0] );
			x = NUM2DBL( Vector3_GetX( temp ) );
			y = NUM2DBL( Vector3_GetY( temp ) );
			z = NUM2DBL( Vector3_GetZ( temp ) );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 1 or 3 arguments but was given %d", argc );
	}
	sf::Listener::SetPosition( x, y, z );
	return Qnil;
}

/* call-seq:
 *   Listener.getPosition()	-> vector3
 *
 * Get the current position of the listener in the scene. 
 */
static VALUE Listener_GetPosition( VALUE self )
{
	const sf::Vector3f pos = sf::Listener::GetPosition();
	return rb_funcall( globalVector3Class, rb_intern( "new" ), 3, rb_float_new( pos.x ), rb_float_new( pos.y ), rb_float_new( pos.z ) );
}

/* call-seq:
 *   Listener.setDirection( vector3 )
 *   Listener.setDirection( x, y, z )
 *
 * Set the orientation of the listener in the scene.
 *
 * The orientation defines the 3D axes of the listener (left, up, front) in the scene. The orientation vector doesn't 
 * have to be normalized. The default listener's orientation is (0, 0, -1).
 */
static VALUE Listener_SetDirection( int argc, VALUE *args, VALUE self )
{
	VALUE temp;
	float x, y, z;
	switch( argc )
	{
		case 3:
			x = NUM2DBL( args[0] );
			y = NUM2DBL( args[1] );
			z = NUM2DBL( args[2] );
			break;
		case 1:
			temp = Vector3_ForceType( args[0] );
			x = NUM2DBL( Vector3_GetX( temp ) );
			y = NUM2DBL( Vector3_GetY( temp ) );
			z = NUM2DBL( Vector3_GetZ( temp ) );
			break;
		default:
			rb_raise( rb_eArgError, "Expected 1 or 3 arguments but was given %d", argc );
	}
	sf::Listener::SetDirection( x, y, z );
	return Qnil;
}

/* call-seq:
 *   Listener.getDirection()	-> vector3
 *
 * Get the current orientation of the listener in the scene. 
 */
static VALUE Listener_GetDirection( VALUE self )
{
	const sf::Vector3f pos = sf::Listener::GetDirection();
	return rb_funcall( globalVector3Class, rb_intern( "new" ), 3, rb_float_new( pos.x ), rb_float_new( pos.y ), rb_float_new( pos.z ) );
}

void Init_Listener( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* The audio listener is the point in the scene from where all the sounds are heard.
 *
 * The audio listener defines the global properties of the audio environment, it defines where and how sounds and 
 * musics are heard.
 * 
 * If sf::View is the eyes of the user, then sf::Listener is his ears (by the way, they are often linked together --
 * same position, orientation, etc.).
 * 
 * sf::Listener is a simple interface, which allows to setup the listener in the 3D audio environment (position and 
 * direction), and to adjust the global volume.
 * 
 * Because the listener is unique in the scene, sf::Listener only contains static functions and doesn't have to be 
 * instanciated.
 * 
 * Usage example:
 * 
 *   # Move the listener to the position (1, 0, -5)
 *   SFML::Listener.setPosition( 1, 0, -5 )
 * 
 *   # Make it face the right axis (1, 0, 0)
 *   SFML::Listener.setDirection( 1, 0, 0 )
 *
 *   # Reduce the global volume
 *   SFML::Listener.setGlobalVolume( 50 )
 *
 */
	globalListenerModule = rb_define_module_under( sfml, "Listener" );
	
	// Module methods
	rb_define_module_function( globalListenerModule, "setGlobalVolume", Listener_SetGlobalVolume, 1 );
	rb_define_module_function( globalListenerModule, "getGlobalVolume", Listener_GetGlobalVolume, 0 );
	rb_define_module_function( globalListenerModule, "setPosition", Listener_SetPosition, -1 );
	rb_define_module_function( globalListenerModule, "getPosition", Listener_GetPosition, 0 );
	rb_define_module_function( globalListenerModule, "setDirection", Listener_SetDirection, -1 );
	rb_define_module_function( globalListenerModule, "getDirection", Listener_GetDirection, 0 );
	
	// Aliases
	rb_define_alias( CLASS_OF( globalListenerModule ), "set_global_volume", "setGlobalVolume" );
	rb_define_alias( CLASS_OF( globalListenerModule ), "globalVolume=", "setGlobalVolume" );
	rb_define_alias( CLASS_OF( globalListenerModule ), "global_volume=", "setGlobalVolume" );
	rb_define_alias( CLASS_OF( globalListenerModule ), "globalVolume", "getGlobalVolume" );
	rb_define_alias( CLASS_OF( globalListenerModule ), "global_volume", "getGlobalVolume" );
	rb_define_alias( CLASS_OF( globalListenerModule ), "get_global_volume", "getGlobalVolume" );
	
	rb_define_alias( CLASS_OF( globalListenerModule ), "set_postion", "setGlobalVolume" );
	rb_define_alias( CLASS_OF( globalListenerModule ), "position=", "setGlobalVolume" );
	rb_define_alias( CLASS_OF( globalListenerModule ), "position", "getGlobalVolume" );
	rb_define_alias( CLASS_OF( globalListenerModule ), "get_position", "getGlobalVolume" );
	
	rb_define_alias( CLASS_OF( globalListenerModule ), "set_direction", "setDirection" );
	rb_define_alias( CLASS_OF( globalListenerModule ), "direction=", "setDirection" );
	rb_define_alias( CLASS_OF( globalListenerModule ), "direction", "getDirection" );
	rb_define_alias( CLASS_OF( globalListenerModule ), "get_direction", "getDirection" );
}

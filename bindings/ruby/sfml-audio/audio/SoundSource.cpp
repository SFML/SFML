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
 
#include "SoundSource.hpp"
#include "Vector3.hpp"
#include "main.hpp"
#include <SFML/Audio/SoundSource.hpp>

VALUE globalSoundSourceClass;

/* External classes */
extern VALUE globalVector3Class;

/* call-seq:
 *   sound_source.getAttenuation()	-> float
 *
 * Get the attenuation factor of the sound.
 */
static VALUE SoundSource_GetAttenuation( VALUE self )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	return rb_float_new( object->GetAttenuation() );
}

/* call-seq:
 *   sound_source.getMinDistance()	-> float
 *
 * Get the minimum distance of the sound.
 */
static VALUE SoundSource_GetMinDistance( VALUE self )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	return rb_float_new( object->GetMinDistance() );
}

/* call-seq:
 *   sound_source.getPitch()	-> float
 *
 * Get the pitch of the sound. 
 */
static VALUE SoundSource_GetPitch( VALUE self )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	return rb_float_new( object->GetPitch() );
}

/* call-seq:
 *   sound_source.getPosition()	-> vector3
 *
 * Get the 3D position of the sound in the audio scene. 
 */
static VALUE SoundSource_GetPosition( VALUE self )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	sf::Vector3f pos = object->GetPosition();
	return rb_funcall( globalVector3Class, rb_intern( "new" ), 3, rb_float_new( pos.x ), rb_float_new( pos.y ), rb_float_new( pos.z ) );
}

/* call-seq:
 *   sound_source.getVolume()	-> float
 *
 * Get the volume of the sound. 
 */
static VALUE SoundSource_GetVolume( VALUE self )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	return rb_float_new( object->GetVolume() );
}

/* call-seq:
 *   sound_source.isRelativeToListener()	-> true or false
 *
 * Tell whether the sound's position is relative to the listener or is absolute. 
 */
static VALUE SoundSource_IsRelativeToListener( VALUE self )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	return ( object->IsRelativeToListener() == true ? Qtrue : Qfalse );
}

/* call-seq:
 *   sound_source.setAttenuation( value )
 *
 * Set the attenuation factor of the sound.
 *
 * The attenuation is a multiplicative factor which makes the sound more or less loud according to its distance from 
 * the listener. An attenuation of 0 will produce a non-attenuated sound, i.e. its volume will always be the same 
 * whether it is heard from near or from far. On the other hand, an attenuation value such as 100 will make the sound 
 * fade out very quickly as it gets further from the listener. The default value of the attenuation is 1.
 */
static VALUE SoundSource_SetAttenuation( VALUE self, VALUE aValue )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	object->SetAttenuation( NUM2DBL( aValue ) );
	return Qnil;
}

/* call-seq:
 *   sound_source.setMinDistance( value )
 *
 * Set the minimum distance of the sound.
 *
 * The "minimum distance" of a sound is the maximum distance at which it is heard at its maximum volume. Further than 
 * the minimum distance, it will start to fade out according to its attenuation factor. A value of 0 ("inside the head 
 * of the listener") is an invalid value and is forbidden. The default value of the minimum distance is 1.
 */
static VALUE SoundSource_SetMinDistance( VALUE self, VALUE aValue )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	object->SetMinDistance( NUM2DBL( aValue ) );
	return Qnil;
}

/* call-seq:
 *   sound_source.setPitch( value )
 *
 * Set the pitch of the sound.
 *
 * The pitch represents the perceived fundamental frequency of a sound; thus you can make a sound more acute or grave 
 * by changing its pitch. A side effect of changing the pitch is to modify the playing speed of the sound as well. The 
 * default value for the pitch is 1.
 */
static VALUE SoundSource_SetPitch( VALUE self, VALUE aValue )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	object->SetPitch( NUM2DBL( aValue ) );
	return Qnil;
}

/* call-seq:
 *   sound_source.setPosition( x, y, z )
 *   sound_source.setPosition( vector3 )
 *
 * Set the 3D position of the sound in the audio scene.
 *
 * Only sounds with one channel (mono sounds) can be spatialized. The default position of a sound is (0, 0, 0).
 */
static VALUE SoundSource_SetPosition( int argc, VALUE *args, VALUE self )
{
	float x, y, z;
	switch( argc )
	{
		case 3:
		{
			x = NUM2DBL( args[0] );
			y = NUM2DBL( args[1] );
			z = NUM2DBL( args[2] );
			break;
		}
		case 1:
		{
			VALUE temp = Vector3_ForceType( args[0] );
			x = NUM2DBL( Vector3_GetX( temp ) );
			y = NUM2DBL( Vector3_GetY( temp ) );
			z = NUM2DBL( Vector3_GetZ( temp ) );
			break;
		}
		default:
			rb_raise( rb_eArgError, "Expected 1 or 3 arguments but was given %d", argc );
	}
	
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	object->SetPosition( x, y, z );
	
	return Qnil;
}

/* call-seq:
 *   sound_source.setRelativeToListener( value )
 *
 * Make the sound's position relative to the listener or absolute.
 *
 * Making a sound relative to the listener will ensure that it will always be played the same way regardless the 
 * position of the listener. This can be useful for non-spatialized sounds, sounds that are produced by the listener, 
 * or sounds attached to it. The default value is false (position is absolute).
 */
static VALUE SoundSource_SetRelativeToListener( VALUE self, VALUE aValue )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	if( aValue == Qtrue )
	{
		object->SetRelativeToListener( true );
	}
	else if( aValue == Qfalse )
	{
		object->SetRelativeToListener( false );
	}
	else
	{
		VALIDATE_CLASS( aValue, rb_cTrueClass, "relative" );
	}
	return Qnil;
}

/* call-seq:
 *   sound_source.setVolume( value )
 *
 * Set the volume of the sound.
 *
 * The volume is a value between 0 (mute) and 100 (full volume). The default value for the volume is 100.
 */
static VALUE SoundSource_SetVolume( VALUE self, VALUE aValue )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	object->SetVolume( NUM2DBL( aValue ) );
	return Qnil;
}

static VALUE SoundSource_Initialize( VALUE self )
{
	rb_raise( rb_eNotImpError, "Trying to construct instance of abstract class" );
}

static void DefineStatusEnum( void )
{
	rb_define_const( globalSoundSourceClass, "Stopped", INT2FIX( sf::SoundSource::Stopped ) );
	rb_define_const( globalSoundSourceClass, "Paused", INT2FIX( sf::SoundSource::Paused ) );
	rb_define_const( globalSoundSourceClass, "Playing", INT2FIX( sf::SoundSource::Playing ) );
}

void Init_SoundSource( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Base class defining a sound's properties.
 *
 * SFML::SoundSource is not meant to be used directly, it only serves as a common base for all audio objects that can 
 * live in the audio environment.
 *
 * It defines several properties for the sound: pitch, volume, position, attenuation, etc. All of them can be changed 
 * at any time with no impact on performances.
 */
	globalSoundSourceClass = rb_define_class_under( sfml, "SoundSource", rb_cObject );
	DefineStatusEnum();
	
	// Instance methods
	rb_define_method( globalSoundSourceClass, "getAttenuation", SoundSource_GetAttenuation, 0 );
	rb_define_method( globalSoundSourceClass, "getMinDistance", SoundSource_GetMinDistance, 0 );
	rb_define_method( globalSoundSourceClass, "getPitch", SoundSource_GetPitch, 0 );
	rb_define_method( globalSoundSourceClass, "getPosition", SoundSource_GetPosition, 0 );
	rb_define_method( globalSoundSourceClass, "getVolume", SoundSource_GetVolume, 0 );
	rb_define_method( globalSoundSourceClass, "isRelativeToListener", SoundSource_IsRelativeToListener, 0 );
	rb_define_method( globalSoundSourceClass, "setAttenuation", SoundSource_GetAttenuation, 1 );
	rb_define_method( globalSoundSourceClass, "setMinDistance", SoundSource_GetMinDistance, 1 );
	rb_define_method( globalSoundSourceClass, "setPitch", SoundSource_GetPitch, 1 );
	rb_define_method( globalSoundSourceClass, "setPosition", SoundSource_GetPosition, -1 );
	rb_define_method( globalSoundSourceClass, "setRelativeToListener", SoundSource_IsRelativeToListener, 1 );
	rb_define_method( globalSoundSourceClass, "setVolume", SoundSource_GetVolume, 1 );
	
	// Aliases
	rb_define_alias( globalSoundSourceClass, "attenuation", "getAttenuation" );
	rb_define_alias( globalSoundSourceClass, "attenuation=", "setAttenuation" );
	
	rb_define_alias( globalSoundSourceClass, "minDistance", "getMinDistance" );
	rb_define_alias( globalSoundSourceClass, "minDistance=", "setMinDistance" );
	rb_define_alias( globalSoundSourceClass, "min_distance", "getMinDistance" );
	rb_define_alias( globalSoundSourceClass, "min_distance=", "setMinDistance" );
	
	rb_define_alias( globalSoundSourceClass, "pitch", "getPitch" );
	rb_define_alias( globalSoundSourceClass, "pitch=", "setPitch" );
	
	rb_define_alias( globalSoundSourceClass, "position", "getPosition" );
	rb_define_alias( globalSoundSourceClass, "position=", "setPosition" );
	
	rb_define_alias( globalSoundSourceClass, "volume", "getVolume" );
	rb_define_alias( globalSoundSourceClass, "volume=", "setVolume" );
	
	rb_define_alias( globalSoundSourceClass, "relativeToListener?", "isRelativeToListener" );
	rb_define_alias( globalSoundSourceClass, "is_relative_to_listener", "isRelativeToListener" );
	rb_define_alias( globalSoundSourceClass, "relative_to_listener?", "isRelativeToListener" );
	rb_define_alias( globalSoundSourceClass, "relativeToListener=", "setRelativeToListener" );
	rb_define_alias( globalSoundSourceClass, "relative_to_listener=", "setRelativeToListener" );
}

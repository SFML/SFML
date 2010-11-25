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

static VALUE SoundSource_GetAttenuation( VALUE self )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	return rb_float_new( object->GetAttenuation() );
}

static VALUE SoundSource_GetMinDistance( VALUE self )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	return rb_float_new( object->GetMinDistance() );
}

static VALUE SoundSource_GetPitch( VALUE self )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	return rb_float_new( object->GetPitch() );
}

static VALUE SoundSource_GetPosition( VALUE self )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	sf::Vector3f pos = object->GetPosition();
	return rb_funcall( globalVector3Class, rb_intern( "new" ), 3, rb_float_new( pos.x ), rb_float_new( pos.y ), rb_float_new( pos.z ) );
}

static VALUE SoundSource_GetVolume( VALUE self )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	return rb_float_new( object->GetVolume() );
}

static VALUE SoundSource_IsRelativeToListener( VALUE self )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	return ( object->IsRelativeToListener() == true ? Qtrue : Qfalse );
}

static VALUE SoundSource_SetAttenuation( VALUE self, VALUE aValue )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	object->SetAttenuation( NUM2DBL( aValue ) );
	return Qnil;
}

static VALUE SoundSource_SetMinDistance( VALUE self, VALUE aValue )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	object->SetMinDistance( NUM2DBL( aValue ) );
	return Qnil;
}

static VALUE SoundSource_SetPitch( VALUE self, VALUE aValue )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	object->SetPitch( NUM2DBL( aValue ) );
	return Qnil;
}

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

static VALUE SoundSource_SetVolume( VALUE self, VALUE aValue )
{
	sf::SoundSource *object = NULL;
	Data_Get_Struct( self, sf::SoundSource, object );
	object->SetVolume( NUM2DBL( aValue ) );
	return Qnil;
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

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
 
#include "Sound.hpp"
#include "Vector3.hpp"
#include "main.hpp"
#include <SFML/Audio/Sound.hpp>

VALUE globalSoundClass;

/* External classes */
extern VALUE globalVector3Class;
extern VALUE globalSoundSourceClass;
extern VALUE globalSoundBufferClass;

static void Sound_Free( sf::Sound *anObject )
{
	delete anObject;
}

static VALUE Sound_Initialize( int argc, VALUE *args, VALUE self )
{
	sf::Sound *object = NULL;
	Data_Get_Struct( self, sf::Sound, object );
	switch( argc )
	{
		case 5:
		{
			VALUE temp = Vector3_ForceType( args[4] );
			sf::Vector3f position;
			position.x = NUM2DBL( Vector3_GetX( temp ) );
			position.y = NUM2DBL( Vector3_GetY( temp ) );
			position.z = NUM2DBL( Vector3_GetZ( temp ) );
			object->SetPosition( position );
		}
		case 4:
			object->SetVolume( NUM2DBL( args[3] ) );
		case 3:
			object->SetPitch( NUM2DBL( args[2] ) );
		case 2:
			if( args[1] == Qtrue )
			{
				object->SetLoop( true );
			}
			else if( args[1] == Qfalse )
			{
				object->SetLoop( false );
			}
			else
			{
				VALIDATE_CLASS( args[1], rb_cTrueClass, "loop" );
			}
		case 1:
		{
			VALIDATE_CLASS( args[0], globalSoundBufferClass, "buffer" );
			sf::SoundBuffer *buffer = NULL;
			Data_Get_Struct( args[0], sf::SoundBuffer, buffer );
			object->SetBuffer( *buffer );
			rb_iv_set( self, "@__buffer_ref", args[0] );
		}
		case 0:
			break;
	}
	
	return self;
}

static VALUE Sound_Play( VALUE self )
{
	sf::Sound *object = NULL;
	Data_Get_Struct( self, sf::Sound, object );
	object->Play();
	return Qnil;
}

static VALUE Sound_Pause( VALUE self )
{
	sf::Sound *object = NULL;
	Data_Get_Struct( self, sf::Sound, object );
	object->Pause();
	return Qnil;
}

static VALUE Sound_Stop( VALUE self )
{
	sf::Sound *object = NULL;
	Data_Get_Struct( self, sf::Sound, object );
	object->Stop();
	return Qnil;
}

static VALUE Sound_SetBuffer( VALUE self, VALUE aBuffer )
{
	VALIDATE_CLASS( aBuffer, globalSoundBufferClass, "buffer" );
	sf::Sound *object = NULL;
	Data_Get_Struct( self, sf::Sound, object );
	sf::SoundBuffer *buffer = NULL;
	Data_Get_Struct( aBuffer, sf::SoundBuffer, buffer );
	object->SetBuffer( *buffer );
	rb_iv_set( self, "@__buffer_ref", aBuffer );	
	return Qnil;
}

static VALUE Sound_SetLoop( VALUE self, VALUE aLoop )
{
	sf::Sound *object = NULL;
	Data_Get_Struct( self, sf::Sound, object );
	if( aLoop == Qtrue )
	{
		object->SetLoop( true );
	}
	else if( aLoop == Qfalse )
	{
		object->SetLoop( false );
	}
	else
	{
		VALIDATE_CLASS( aLoop, rb_cTrueClass, "loop" );
	}
	return Qnil;
}

static VALUE Sound_SetPlayingOffset( VALUE self, VALUE aOffset )
{
	sf::Sound *object = NULL;
	Data_Get_Struct( self, sf::Sound, object );
	object->SetPlayingOffset( NUM2DBL( aOffset ) );
	return Qnil;
}

static VALUE Sound_GetBuffer( VALUE self )
{
	return rb_iv_get( self, "@__buffer_ref" );
}

static VALUE Sound_GetLoop( VALUE self )
{
	sf::Sound *object = NULL;
	Data_Get_Struct( self, sf::Sound, object );
	if( object->GetLoop() == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

static VALUE Sound_GetPlayingOffset( VALUE self )
{
	sf::Sound *object = NULL;
	Data_Get_Struct( self, sf::Sound, object );
	return rb_float_new( object->GetPlayingOffset() );
}

static VALUE Sound_GetStatus( VALUE self )
{
	sf::Sound *object = NULL;
	Data_Get_Struct( self, sf::Sound, object );
	return INT2FIX( static_cast< int >( object->GetStatus() ) );
}

static VALUE Sound_ResetBuffer( VALUE self )
{
	sf::Sound *object = NULL;
	Data_Get_Struct( self, sf::Sound, object );
	object->ResetBuffer();
	return Qnil;
}

static VALUE Sound_New( int argc, VALUE *args, VALUE aKlass )
{
	sf::Sound *object = new sf::Sound();
	VALUE rbData = Data_Wrap_Struct( aKlass, 0, Sound_Free, object );
	rb_obj_call_init( rbData, argc, args );
	return rbData;
}

void Init_Sound( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Regular sound that can be played in the audio environment.
 *
 * SFML::Sound is the class to use to play sounds.
 *
 * It provides:
 *
 *   - Control (play, pause, stop)
 *   - Ability to modify output parameters in real-time (pitch, volume, ...)
 *   - 3D spatial features (position, attenuation, ...).
 *
 * SFML::Sound is perfect for playing short sounds that can fit in memory and require no latency, like foot steps or
 * gun shots. For longer sounds, like background musics or long speeches, rather see sf::Music 
 * (which is based on streaming).
 *
 * In order to work, a sound must be given a buffer of audio data to play. Audio data (samples) is stored in
 * sf::SoundBuffer, and attached to a sound with the SetBuffer() function. The buffer object attached to a sound 
 * must remain alive as long as the sound uses it. Note that multiple sounds can use the same sound buffer at the 
 * same time.
 *
 * Usage example:
 *
 *   buffer = SFML::SoundBuffer.new
 *   buffer.loadFromFile( "sound.wav" )
 *
 *   sound = SFML::Sound.new
 *   sound.setBuffer( buffer )
 *   sound.play()
 */
	globalSoundClass = rb_define_class_under( sfml, "Sound", globalSoundSourceClass );
	
	// Class methods
	rb_define_singleton_method( globalSoundClass, "new", Sound_New, -1 );
	
	// Instance methods
	rb_define_method( globalSoundClass, "initialize", Sound_Initialize, 0 );
	rb_define_method( globalSoundClass, "play", Sound_Play, 0 );
	rb_define_method( globalSoundClass, "pause", Sound_Pause, 0 );
	rb_define_method( globalSoundClass, "stop", Sound_Stop, 0 );
	rb_define_method( globalSoundClass, "setBuffer", Sound_SetBuffer, 1 );
	rb_define_method( globalSoundClass, "getBuffer", Sound_GetBuffer, 0 );
	rb_define_method( globalSoundClass, "setLoop", Sound_SetLoop, 1 );
	rb_define_method( globalSoundClass, "getLoop", Sound_GetLoop, 0 );
	rb_define_method( globalSoundClass, "setPlayingOffset", Sound_SetPlayingOffset, 1 );
	rb_define_method( globalSoundClass, "getPlayingOffset", Sound_GetPlayingOffset, 0 );
	rb_define_method( globalSoundClass, "getStatus", Sound_GetStatus, 0 );
	rb_define_method( globalSoundClass, "resetBuffer", Sound_ResetBuffer, 0 );
	
	// Aliases
	rb_define_alias( globalSoundClass, "buffer=", "setBuffer" );
	rb_define_alias( globalSoundClass, "buffer", "getBuffer" );
	rb_define_alias( globalSoundClass, "loop=", "setLoop" );
	rb_define_alias( globalSoundClass, "loop", "getLoop" );
	rb_define_alias( globalSoundClass, "playingOffset=", "setPlayingOffset" );
	rb_define_alias( globalSoundClass, "playing_offset=", "setPlayingOffset" );
	rb_define_alias( globalSoundClass, "playingOffset", "getPlayingOffset" );
	rb_define_alias( globalSoundClass, "playing_offset", "getPlayingOffset" );
	rb_define_alias( globalSoundClass, "status", "getStatus" );
	rb_define_alias( globalSoundClass, "reset_buffer", "resetBuffer" );
}

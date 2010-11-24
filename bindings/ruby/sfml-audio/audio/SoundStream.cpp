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
 
#include "SoundStream.hpp"
#include "main.hpp"
#include <SFML/Audio/SoundStream.hpp>

VALUE globalSoundStreamClass;

static VALUE SoundStream_GetAttenuation( VALUE self )
{
	sf::SoundStream *object = NULL;
	Data_Get_Struct( self, sf::SoundStream, object );
	return rb_float_new( object->GetAttenuation() );
}

static VALUE SoundStream_GetChannelsCount( VALUE self )
{
	sf::SoundStream *object = NULL;
	Data_Get_Struct( self, sf::SoundStream, object );
	return INT2FIX( object->GetChannelsCount() );
}

static VALUE SoundStream_GetLoop( VALUE self )
{
	sf::SoundStream *object = NULL;
	Data_Get_Struct( self, sf::SoundStream, object );
	return ( object->GetLoop() == true ? Qtrue : Qfalse );
}

static VALUE SoundStream_GetMinDistance( VALUE self )
{
	sf::SoundStream *object = NULL;
	Data_Get_Struct( self, sf::SoundStream, object );
	return rb_float_new( object->GetMinDistance() );
}

static VALUE SoundStream_GetPitch( VALUE self )
{
	sf::SoundStream *object = NULL;
	Data_Get_Struct( self, sf::SoundStream, object );
	return rb_float_new( object->GetPitch() );
}


static void DefineStatusEnum( void )
{
	rb_define_const( globalSoundStreamClass, "Stopped", INT2FIX( sf::SoundStream::Stopped ) );
	rb_define_const( globalSoundStreamClass, "Paused", INT2FIX( sf::SoundStream::Paused ) );
	rb_define_const( globalSoundStreamClass, "Playing", INT2FIX( sf::SoundStream::Playing ) );
}

void Init_SoundStream( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Abstract base class for streamed audio sources.
 *
 * Unlike audio buffers (see SFML::SoundBuffer), audio streams are never completely loaded in memory.
 *
 * Instead, the audio data is acquired continuously while the stream is playing. This behaviour allows to play a sound
 * with no loading delay, and keeps the memory consumption very low.
 *
 * Sound sources that need to be streamed are usually big files (compressed audio musics that would eat hundreds of MB
 * in memory) or files that would take a lot of time to be received (sounds played over the network).
 * 
 * SFML::SoundStream is a base class that doesn't care about the stream source, which is left to the derived class. 
 * SFML provides a built-in specialization for big files (see SFML::Music). No network stream source is provided, but you
 * can write your own by combining this class with the network module.
 *
 * A derived class has to override two virtual functions:
 *
 *   - onGetData fills a new chunk of audio data to be played
 *   - onSeek changes the current playing position in the source
 *
 * It is important to note that each SoundStream is played in its own separate thread, so that the streaming loop 
 * doesn't block the rest of the program. In particular, the onGetData and onSeek virtual functions may sometimes be 
 * called from this separate thread. It is important to keep this in mind, because you may have to take care of 
 * synchronization issues if you share data between threads.
 *
 * Usage example:
 *
 *   class CustomStream < SFML::SoundStream
 *     def open( location )
 *       # Open the source and get audio settings
 *       ...
 *       channelsCount = ...
 *       sampleRate = ...
 *
 *       # Initialize the stream -- important!
 *       initialize( channelsCount, sampleRate )
 *     end
 *
 *     def onGetData( data )
 *       # Fill the chunk with audio data from the stream source
 *       data.Samples = ...;
 *       data.NbSamples = ...;
 *
 *       # Return true to continue playing
 *       return true;
 *     end
 *
 *     def onSeek( timeOffset )
 *       # Change the current position in the stream source
 *       ...
 *     end
 *   end
 *
 * # Usage
 * CustomStream stream;
 * stream.open( "path/to/stream" )
 * stream.play
 */
	globalSoundStreamClass = rb_define_class_under( sfml, "SoundStream", rb_cObject );
	DefineStatusEnum();
	
	// Class methods
	
	// Instance methods
		
	// Aliases
}

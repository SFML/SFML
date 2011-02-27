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
 
#include "Music.hpp"
#include "main.hpp"
#include <SFML/Audio/Music.hpp>

VALUE globalMusicClass;

/* External classes */
extern VALUE globalSoundStreamClass;

static VALUE Music_Free( sf::Music *anObject )
{
	delete anObject;
}

/* call-seq:
 *   music.openFromFile()	-> true or false
 *
 * Open a music from an audio file.
 *
 * This function doesn't start playing the music (call Play() to do so). Here is a complete list of all the supported 
 * audio formats: ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam, w64, mat4, mat5 pvf, htk, sds, avr, sd2,
 * caf, wve, mpc2k, rf64.
 */
static VALUE Music_OpenFromFile( VALUE self, VALUE aFilename )
{
	sf::Music *object = NULL;
	Data_Get_Struct( self, sf::Music, object );
	if( object->OpenFromFile( rb_string_value_cstr( &aFilename ) ) == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

/* call-seq:
 *   Music.new()			-> music
 *   Music.new( filename )	-> music
 *
 * Will create a new music instance.
 * 
 * If a filename argument is specified then music#openFromFile will be called on the created instance.
 */
static VALUE Music_Initialize( int argc, VALUE *args, VALUE self )
{
	if( argc > 0 )
	{
		rb_funcall2( self, rb_intern( "openFromFile" ), argc, args );
	}
	return self;
}

/* call-seq:
 *   music.getDuration()	-> float
 *
 * Get the total duration of the music. 
 */
static VALUE Music_GetDuration( VALUE self )
{
	sf::Music *object = NULL;
	Data_Get_Struct( self, sf::Music, object );
	return rb_float_new( object->GetDuration() );
}

static VALUE Music_Alloc( VALUE aKlass )
{
	sf::Music *object = new sf::Music();
	return Data_Wrap_Struct( aKlass, 0, Music_Free, object );
}

void Init_Music( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Streamed music played from an audio file.
 *
 * Musics are sounds that are streamed rather than completely loaded in memory.
 *
 * This is especially useful for compressed musics that usually take hundreds of MB when they are uncompressed: by 
 * streaming it instead of loading it entirely, you avoid saturating the memory and have almost no loading delay.
 *
 * Apart from that, a sf::Music has almost the same features as the sf::SoundBuffer / sf::Sound pair: you can 
 * play/pause/stop it, request its parameters (channels, sample rate), change the way it is played (pitch, volume, 
 * 3D position, ...), etc.
 *
 * As a sound stream, a music is played in its own thread in order not to block the rest of the program. This means 
 * that you can leave the music alone after calling Play(), it will manage itself very well.
 * 
 * Usage example:
 * 
 *   # Declare a new music
 *   music = SFML::Music.new
 *
 *   # Open it from an audio file
 *   if music.openFromFile( "music.ogg" ) == false
 *     # error...
 *   end
 *
 *   # Change some parameters
 *   music.setPosition( 0, 1, 10 ) # change its 3D position
 *   music.setPitch( 2 )           # increase the pitch
 *   music.setVolume( 50 )         # reduce the volume
 *   music.setLoop( true )         # make it loop
 *
 *   # Play it
 *   music.play()
 */
	globalMusicClass = rb_define_class_under( sfml, "Music", globalSoundStreamClass );

	// Class methods
	//rb_define_singleton_method( globalMusicClass, "new", Music_New, -1 );
	rb_define_alloc_func( globalMusicClass, Music_Alloc );
	
	// Instance methods
	rb_define_method( globalMusicClass, "initialize", Music_Initialize, -1 );
	rb_define_method( globalMusicClass, "openFromFile", Music_OpenFromFile, 1 );
	rb_define_method( globalMusicClass, "getDuration", Music_GetDuration, 0 );
		
	// Instance Aliases
	rb_define_alias( globalMusicClass, "open_from_file", "openFromFile" );
	rb_define_alias( globalMusicClass, "openFile", "openFromFile" );
	rb_define_alias( globalMusicClass, "open_file", "openFromFile" );
	
	rb_define_alias( globalMusicClass, "get_duration", "getDuration" );
	rb_define_alias( globalMusicClass, "duration", "getDuration" );
}

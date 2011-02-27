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
 
#include "SoundBuffer.hpp"
#include "main.hpp"
#include <SFML/Audio/SoundBuffer.hpp>

VALUE globalSoundBufferClass;

/* Free a heap allocated object 
 * Not accessible trough ruby directly!
 */
static void SoundBuffer_Free( sf::SoundBuffer *anObject )
{
	delete anObject;
}

/* call-seq:
 *   sound_buffer.loadFromFile( filename )	-> true or false
 *
 * Load the sound buffer from a file.
 *
 * Here is a complete list of all the supported audio formats: ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, 
 * ircam, w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
 */
static VALUE SoundBuffer_LoadFromFile( VALUE self, VALUE aFileName )
{
	sf::SoundBuffer *object = NULL;
	Data_Get_Struct( self, sf::SoundBuffer, object );
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
 *   sound_buffer.loadFromSamples( samples, samplesCount, channelsCount, sampleRate )	-> true or false
 *
 * Load the sound buffer from an array of audio samples.
 *
 * The assumed format of the audio samples is 16 bits signed integer.
 */
static VALUE SoundBuffer_LoadFromSamples( VALUE self, VALUE someSamples, VALUE aSamplesCount, VALUE aChannelsCount, VALUE aSampleRate )
{
	const unsigned int rawSamplesCount = FIX2UINT( aSamplesCount );
	const unsigned int rawChannelsCount = FIX2UINT( aChannelsCount );
	const unsigned int rawSampleRate = FIX2UINT( aSampleRate );
	VALIDATE_CLASS( someSamples, rb_cArray, "samples" );
	sf::Int16 * const tempData = new sf::Int16[rawSamplesCount];
	VALUE samples = rb_funcall( someSamples, rb_intern("flatten"), 0 );
	for(unsigned long index = 0; index < rawSamplesCount; index++)
	{
		sf::Int16 val = NUM2INT( rb_ary_entry( samples, index ) );
		tempData[index] = val;
	}
	
	sf::SoundBuffer *object = NULL;
	Data_Get_Struct( self, sf::SoundBuffer, object );
	bool result = object->LoadFromSamples( tempData, rawSamplesCount, rawChannelsCount, rawSampleRate );
	delete[] tempData;
	
	if( result == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

/* call-seq:
 *   sound_buffer.saveToFile( filename )	-> true or false
 *
 * Save the sound buffer to an audio file. 
 *
 * Here is a complete list of all the supported audio formats: ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam, 
 * w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
 */
static VALUE SoundBuffer_SaveToFile( VALUE self, VALUE aFileName )
{
	sf::SoundBuffer *object = NULL;
	Data_Get_Struct( self, sf::SoundBuffer, object );
	if( object->SaveToFile( rb_string_value_cstr( &aFileName ) ) == true )
	{
		return Qtrue;
	}
	else
	{
		return Qfalse;
	}
}

/* call-seq:
 *   sound_buffer.getSamples()	-> array of samples
 *
 * Get the array of audio samples stored in the buffer.
 *
 * The format of the returned samples is 16 bits signed integer. The total number of samples in this array is given 
 * by the getSamplesCount() function.
 */
static VALUE SoundBuffer_GetSamples( VALUE self )
{
	sf::SoundBuffer *object = NULL;
	Data_Get_Struct( self, sf::SoundBuffer, object );
	
	const unsigned int samplesCount = object->GetSamplesCount();
	const sf::Int16 *const samplesPtr = object->GetSamples();
	VALUE samples = rb_ary_new2( samplesCount );
	for(unsigned long index = 0; index < samplesCount; index++)
	{
		rb_ary_store( samples, index, INT2FIX( samplesPtr[index] ) );
	}
	
	return samples;
}

/* call-seq:
 *   sound_buffer.getSamplesCount()	-> fixnum
 *
 * Get the number of samples stored in the buffer.
 *
 * The array of samples can be accessed with the getSamples() function.
 */
static VALUE SoundBuffer_GetSamplesCount( VALUE self )
{
	sf::SoundBuffer *object = NULL;
	Data_Get_Struct( self, sf::SoundBuffer, object );
	return INT2FIX( object->GetSamplesCount() );
}

/* call-seq:
 *   sound_buffer.getSampleRate()	-> fixnum
 *
 * Get the sample rate of the sound.
 *
 * The sample rate is the number of samples played per second. The higher, the better the quality (for example, 
 * 44100 samples/s is CD quality).
 */
static VALUE SoundBuffer_GetSampleRate( VALUE self )
{
	sf::SoundBuffer *object = NULL;
	Data_Get_Struct( self, sf::SoundBuffer, object );
	return INT2FIX( object->GetSampleRate() );
}

/* call-seq:
 *   sound_buffer.getChannelsCount()	-> float
 *
 * Get the total duration of the sound. 
 */
static VALUE SoundBuffer_GetChannelsCount( VALUE self )
{
	sf::SoundBuffer *object = NULL;
	Data_Get_Struct( self, sf::SoundBuffer, object );
	return INT2FIX( object->GetChannelsCount() );
}

/* call-seq:
 *   sound_buffer.getDuration()	-> fixnum
 *
 * Get the number of channels used by the sound.
 *
 * If the sound is mono then the number of channels will be 1, 2 for stereo, etc.
 */
static VALUE SoundBuffer_GetDuration( VALUE self )
{
	sf::SoundBuffer *object = NULL;
	Data_Get_Struct( self, sf::SoundBuffer, object );
	return rb_float_new( object->GetDuration() );
}

/* call-seq:
 *   SoundBuffer.new()														-> sound_buffer
 *   SoundBuffer.new( filename )											-> sound_buffer
 *   SoundBuffer.new( samples, samplesCount, channelsCount, sampleRate )	-> sound_buffer
 *
 * Will create a new sound buffer instance.
 * 
 * If a filename argument is specified then sound_buffer#loadFromFile will be called on the created instance. If 
 * samples, samplesCount, channelsCount and sampleRate are specified then image#loadFromPixels will be called on the 
 * created instance.
 */
static VALUE SoundBuffer_Initialize( int argc, VALUE *args, VALUE self )
{
	if( argc > 1 )
	{
		rb_funcall2( self, rb_intern( "loadFromSampels" ), argc, args );
	}
	else if( argc > 0 )
	{
		rb_funcall2( self, rb_intern( "loadFromFile" ), argc, args );
	}
	return self;
}

static VALUE SoundBuffer_InitializeCopy( VALUE self, VALUE aSource )
{
	sf::SoundBuffer *object = NULL;
	Data_Get_Struct( self, sf::SoundBuffer, object );
	sf::SoundBuffer *source = NULL;
	Data_Get_Struct( aSource, sf::SoundBuffer, source );
	*object = *source;
}

/* call-seq:
 *   SoundBuffer.new()	-> sound_buffer
 *
 * Creates an sound buffer instance for us.
 */
static VALUE SoundBuffer_Alloc( VALUE aKlass )
{
	sf::SoundBuffer *object = new sf::SoundBuffer();
	return Data_Wrap_Struct( aKlass, 0, SoundBuffer_Free, object );
}

void Init_SoundBuffer( void )
{
/* SFML namespace which contains the classes of this module. */
	VALUE sfml = rb_define_module( "SFML" );
/* Storage for audio samples defining a sound.
 *
 * A sound buffer holds the data of a sound, which is an array of audio samples.
 * 
 * A sample is a 16 bits signed integer that defines the amplitude of the sound at a given time. The sound is then 
 * restituted by playing these samples at a high rate (for example, 44100 samples per second is the standard rate used 
 * for playing CDs). In short, audio samples are like image pixels, and a SFML::SoundBuffer is similar to a SFML::Image.
 *
 * A sound buffer can be loaded from a file (see loadFromFile() for the complete list of supported formats), from 
 * memory or directly from an array of samples. It can also be saved back to a file.
 *
 * Sound buffers alone are not very useful: they hold the audio data but cannot be played. To do so, you need to use 
 * the SFML::Sound class, which provides functions to play/pause/stop the sound as well as changing the way it is 
 * outputted (volume, pitch, 3D position, ...). This separation allows more flexibility and better performances: 
 * indeed a SFML::SoundBuffer is a heavy resource, and any operation on it is slow (often too slow for real-time 
 * applications). On the other side, a SFML::Sound is a lightweight object, which can use the audio data of a sound 
 * buffer and change the way it is played without actually modifying that data. Note that it is also possible to bind 
 * several SFML::Sound instances to the same SFML::SoundBuffer.
 *
 * It is important to note that the SFML::Sound instance doesn't copy the buffer that it uses, it only keeps a reference 
 * to it. Thus, a SFML::SoundBuffer must not be destructed while it is used by a SFML::Sound (i.e. never write a function 
 * that uses a local SFML::SoundBuffer instance for loading a sound).
 *
 * Usage example:
 *
 *   # Declare a new sound buffer
 *   buffer = SFML::SoundBuffer.new
 * 
 *   # Load it from a file
 *   if buffer.loadFromFile( "sound.wav" ) == false
 *     # error...
 *   end
 *
 *   # Create a sound source and bind it to the buffer
 *   sound1 = SFML::Sound.new
 *   sound1.setBuffer( buffer )
 *
 *   # Play the sound
 *   sound1.play()
 *
 *   # Create another sound source bound to the same buffer
 *   sound2 = SFML::Sound.new
 *   sound2.setBuffer( buffer )
 *
 *   # Play it with a higher pitch -- the first sound remains unchanged
 *   sound2.setPitch( 2 )
 *   sound2.play()
 */
	globalSoundBufferClass = rb_define_class_under( sfml, "SoundBuffer", rb_cObject );
	
	// Class methods
	//rb_define_singleton_method( globalSoundBufferClass, "new", SoundBuffer_New, -1 );
	rb_define_alloc_func( globalSoundBufferClass, SoundBuffer_Alloc );
	
	// Instance methods
	rb_define_method( globalSoundBufferClass, "initialize", SoundBuffer_Initialize, -1 );
	rb_define_method( globalSoundBufferClass, "initialize_copy", SoundBuffer_InitializeCopy, 1 );
	rb_define_method( globalSoundBufferClass, "loadFromFile", SoundBuffer_LoadFromFile, 1 );
	rb_define_method( globalSoundBufferClass, "loadFromSamples", SoundBuffer_LoadFromSamples, 4 );
	rb_define_method( globalSoundBufferClass, "saveToFile", SoundBuffer_SaveToFile, 1 );
	rb_define_method( globalSoundBufferClass, "getSamples", SoundBuffer_GetSamples, 0 );
	rb_define_method( globalSoundBufferClass, "getSamplesCount", SoundBuffer_GetSamplesCount, 0 );
	rb_define_method( globalSoundBufferClass, "getSampleRate", SoundBuffer_GetSampleRate, 0 );
	rb_define_method( globalSoundBufferClass, "getChannelsCount", SoundBuffer_GetChannelsCount, 0 );
	rb_define_method( globalSoundBufferClass, "getDuration", SoundBuffer_GetDuration, 0 );
	
	// Instance Aliases
	rb_define_alias( globalSoundBufferClass, "load_from_file", "loadFromFile" );
	rb_define_alias( globalSoundBufferClass, "loadFile", "loadFromFile" );
	rb_define_alias( globalSoundBufferClass, "load_file", "loadFromFile" );
	rb_define_alias( globalSoundBufferClass, "load_from_samples", "loadFromSamples" );
	rb_define_alias( globalSoundBufferClass, "loadSamples", "loadFromSamples" );
	rb_define_alias( globalSoundBufferClass, "load_samples", "loadFromSamples" );
	rb_define_alias( globalSoundBufferClass, "save_to_file", "saveToFile" );
	rb_define_alias( globalSoundBufferClass, "save", "saveToFile" );
	
	rb_define_alias( globalSoundBufferClass, "get_samples", "getSamples" );
	rb_define_alias( globalSoundBufferClass, "samples", "getSamples" );
	
	rb_define_alias( globalSoundBufferClass, "get_samples_count", "getSamplesCount" );
	rb_define_alias( globalSoundBufferClass, "samples_count", "getSamplesCount" );
	rb_define_alias( globalSoundBufferClass, "samplesCount", "getSamplesCount" );
	
	rb_define_alias( globalSoundBufferClass, "get_sample_rate", "getSampleRate" );
	rb_define_alias( globalSoundBufferClass, "sample_rate", "getSampleRate" );
	rb_define_alias( globalSoundBufferClass, "sampleRate", "getSampleRate" );
	
	rb_define_alias( globalSoundBufferClass, "get_channels_count", "getChannelsCount" );
	rb_define_alias( globalSoundBufferClass, "channels_count", "getChannelsCount" );
	rb_define_alias( globalSoundBufferClass, "channelsCount", "getChannelsCount" );
	
	rb_define_alias( globalSoundBufferClass, "get_duration", "getDuration" );
	rb_define_alias( globalSoundBufferClass, "duration", "getDuration" );
}

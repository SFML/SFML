/*
*	DSFML - SFML Library wrapper for the D programming language.
*	Copyright (C) 2008 Julien Dagorn (sirjulio13@gmail.com)
*	Copyright (C) 2010 Andreas Hollandt
*
*	This software is provided 'as-is', without any express or
*	implied warranty. In no event will the authors be held
*	liable for any damages arising from the use of this software.
*
*	Permission is granted to anyone to use this software for any purpose,
*	including commercial applications, and to alter it and redistribute
*	it freely, subject to the following restrictions:
*
*	1.  The origin of this software must not be misrepresented;
*		you must not claim that you wrote the original software.
*		If you use this software in a product, an acknowledgment
*		in the product documentation would be appreciated but
*		is not required.
*
*	2.  Altered source versions must be plainly marked as such,
*		and must not be misrepresented as being the original software.
*
*	3.  This notice may not be removed or altered from any
*		source distribution.
*/

module dsfml.audio.music;

import dsfml.audio.soundstatus;

import dsfml.system.common;
import dsfml.system.exception;
import dsfml.system.stringutil;
import dsfml.system.vector3;

/**
* Music defines a big sound played using streaming,
* so usually what we call a music :)
*/
class Music : DSFMLObject
{
	/**
	*	Open a music file (doesn't play it -- call Play for that)
	*	
	*	Params:
	*		filename = Path of the file to open
	*		
	*/ 
	this(string filename)
	{
		if (filename is null || filename.length == 0)
			throw new LoadingException("LoadingException : Filename is invalid.");
		
		super(sfMusic_CreateFromFile(toStringz(filename)));
	}

	/**
	*	Open a music file from memory (doesn't play it -- call Play() for that)
	*
	*	Params:
	*		data = file data in memory
	*
	*/
	this(byte[] data)
	{
		if (data is null || data.length == 0)
			throw new Exception("LoadingException : Memory stream is invalid.");
		
		super(m_ptr = sfMusic_CreateFromMemory(data.ptr, data.length)); 
	}

	override void dispose()
	{
		sfMusic_Destroy(m_ptr);
	}

	/**
	*	Start playing the audio stream
	*/
	void play()
	{
		sfMusic_Play(m_ptr);
	}

	/**
	*	Stop playing the audio stream
	*/
	void stop()
	{
		sfMusic_Stop(m_ptr);
	}

	/**
	*	Pause the audio stream
	*/
	void pause()
	{
		sfMusic_Pause(m_ptr);
	}


	/**
	*	Return the number of channels (1 = mono, 2 = stereo)
	*
	*	Returns: 
	*		Number of channels
	*/
	uint getChannelsCount()
	{
		return sfMusic_GetChannelsCount(m_ptr);
	}

	/**
	*	Get the stream sample rate
	*
	*	Returns:
	*		Stream frequency (number of samples per second)
	*/
	uint getSampleRate() 
	{
		return sfMusic_GetSampleRate(m_ptr);
	}


	/**
	*	Get the music duration
	*
	*	Returns: 
	*		Music duration, in seconds
	*/
	float getDuration() 
	{
		return sfMusic_GetDuration(m_ptr);
	}

	/**
	*	Get the status of the stream (stopped, paused, playing)
	*
	*	Returns: 
	*		Current status of the sound
	*/
	SoundStatus getStatus() 
	{
		return sfMusic_GetStatus(m_ptr);
	}


	/**
	*	Tell whether or not the music is looping
	*
	*	Returns: 
	*		True if the music is looping, false otherwise
	*/
	bool getLoop() 
	{
		return cast(bool)sfMusic_GetLoop(m_ptr);
	}

	/**
	*	Get the pitch
	*
	*	Returns: 
	*		Pitch value
	*/
	float getPitch() 
	{
		return sfMusic_GetPitch(m_ptr);
	}

	/**
	*	Get the volume
	*
	*	Returns: 
	*		Volume value (in range [1, 100])
	*/
	float getVolume() 
	{
		return sfMusic_GetVolume(m_ptr);
	}

	/**
	*	Get the sound position
	*
	*	Returns:
	*		Current position of the music.
	*/
	Vector3f getPosition() 
	{
		Vector3f ret;
		sfMusic_GetPosition(m_ptr, &ret.x, &ret.y, &ret.z);
		return ret;
	}

	/**
	*	Get the minimum distance
	*
	*	Returns:
	*	  Minimum distance for the sound
	*/
	float getMinDistance()
	{
		return sfMusic_GetMinDistance(m_ptr);
	}

	/**
	*	Get the attenuation factor
	*
	*	Returns:
	*		Attenuation factor of the sound
	*
	*/
	float getAttenuation() 
	{
		return sfMusic_GetAttenuation(m_ptr);
	}


	/**
	*	Set the music loop state.
	*	This parameter is disabled by default
	*
	*	Params: 
	*		loop = True to play in loop, false to play once
	*/
	void setLoop(bool loop)
	{
		sfMusic_SetLoop(m_ptr, loop);
	}

	/**
	*	Set the sound pitch.
	*	The default pitch is 1
	*
	*	Params: 
	*		pitch = New pitch
	*
	*/
	void setPitch(float pitch)
	{
		sfMusic_SetPitch(m_ptr, pitch);
	}

	/**
	*	Set the sound volume.
	*	The default volume is 100
	*
	*	Params: 
	*		volume = Volume (in range [0, 100])
	*
	*/
	void setVolume(float volume)
	in
	{
		assert (volume >= 0.f && volume <= 100.f);
	}
	body
	{
		sfMusic_SetVolume(m_ptr, volume);
	}

	/**
	*	Set the sound position.
	*	The default position is (0, 0, 0)
	*
	*	Params: 
	*		x = X position of the sound in the world
	*		y = Y position of the sound in the world
	*		z = Z position of the sound in the world
	*
	*/
	void setPosition(float x, float y, float z)
	{
		sfMusic_SetPosition(m_ptr, x, y, z);
	}

	/**
	*	Set the sound position.
	*	The default position is (0, 0, 0)
	*
	*	Params: 
	*		position = new position
	*
	*/
	void setPosition(Vector3f position)
	{
		sfMusic_SetPosition(m_ptr, position.x, position.y, position.z);
	}

	/**
	*	Set the minimum distance - closer than thsi distance
	*	the listener will hear the sound at its maximum volume.
	*	The default distance is 1.0
	*	
	*	Params:	
	*		minDistance = new minimum distance for the sound 
	*/		
	void setMinDistance(float minDistance)
	{
		sfMusic_SetMinDistance(m_ptr, minDistance);
	}

	/**
	*	Set the attenuation factor - the higher the attenuation, the
	*	more the sound will be attenuated with distance from listener.
	*	The default attenuation factor 1.0
	*	
	*	Params:
	*		attenuation = new attenuation factor for the sound			
	*/		
	void setAttenuation(float attenuation)
	{
		sfMusic_SetAttenuation(m_ptr, attenuation);
	}

	/**
	 * Make the music's position relative to the listener's position, or absolute.
	 * The default value is false (absolute)
	 * 
	 *	Params:
	 *		relative = True to set the position relative, false to set it absolute
	 */
	void setRelativeToListener(bool relative)
	{
		sfMusic_SetRelativeToListener(m_ptr, relative);
	}

	/**
	 * Tell if the music's position is relative to the listener's
	 * position, or if it's absolute
	 * 
	 *	Returns:
	 *		true if the position is relative, sfFalse if it's absolute
	 */
	bool isRelativeToListener()
	{
		return sfMusic_IsRelativeToListener(m_ptr);
	}

}

private:
	
extern(C)
{
void* function(cchar*)						sfMusic_CreateFromFile;
void* function(byte*, size_t)				sfMusic_CreateFromMemory;
void function(void*)						sfMusic_Destroy;
void function(void*, int)					sfMusic_SetLoop;
bool function(void*)						sfMusic_GetLoop;
float function(void*)						sfMusic_GetDuration;
void function(void*)						sfMusic_Play;
void function(void*)						sfMusic_Pause;
void function(void*)						sfMusic_Stop;
uint function(void*)						sfMusic_GetChannelsCount;
uint function(void*)						sfMusic_GetSampleRate;
SoundStatus function(void*)					sfMusic_GetStatus;
void function(void*, float)					sfMusic_SetPitch;
void function(void*, float)					sfMusic_SetVolume;
void function(void*, float, float, float)	sfMusic_SetPosition;
float function(void*)						sfMusic_GetPitch;
float function(void*)						sfMusic_GetVolume;
void function(void*, float*, float*, float*)sfMusic_GetPosition;
float function(void*)						sfMusic_GetMinDistance;
float function(void*)						sfMusic_GetAttenuation;
void function(void*, float)					sfMusic_SetMinDistance;
void function(void*, float)					sfMusic_SetAttenuation;


void function(void*, bool)					sfMusic_SetRelativeToListener;
bool function(void*)						sfMusic_IsRelativeToListener;
}

static this()
{
debug
	DllLoader dll = DllLoader.load("csfml-audio-d");
else
	DllLoader dll = DllLoader.load("csfml-audio");

	mixin(loadFromSharedLib("sfMusic_CreateFromFile"));
	mixin(loadFromSharedLib("sfMusic_CreateFromMemory"));
	mixin(loadFromSharedLib("sfMusic_Destroy"));
	mixin(loadFromSharedLib("sfMusic_SetLoop"));
	mixin(loadFromSharedLib("sfMusic_GetLoop"));
	mixin(loadFromSharedLib("sfMusic_GetDuration"));
	mixin(loadFromSharedLib("sfMusic_Play"));
	mixin(loadFromSharedLib("sfMusic_Pause"));
	mixin(loadFromSharedLib("sfMusic_Stop"));
	mixin(loadFromSharedLib("sfMusic_GetChannelsCount"));
	mixin(loadFromSharedLib("sfMusic_GetSampleRate"));
	mixin(loadFromSharedLib("sfMusic_GetStatus"));
	mixin(loadFromSharedLib("sfMusic_SetPitch"));
	mixin(loadFromSharedLib("sfMusic_SetVolume"));
	mixin(loadFromSharedLib("sfMusic_SetPosition"));
	mixin(loadFromSharedLib("sfMusic_GetPitch"));
	mixin(loadFromSharedLib("sfMusic_GetVolume"));
	mixin(loadFromSharedLib("sfMusic_GetPosition"));
	mixin(loadFromSharedLib("sfMusic_GetMinDistance"));
	mixin(loadFromSharedLib("sfMusic_GetAttenuation"));
	mixin(loadFromSharedLib("sfMusic_SetMinDistance"));
	mixin(loadFromSharedLib("sfMusic_SetAttenuation"));

	mixin(loadFromSharedLib("sfMusic_SetRelativeToListener"));
	mixin(loadFromSharedLib("sfMusic_IsRelativeToListener"));
}
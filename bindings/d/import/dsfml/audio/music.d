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

import dsfml.audio.soundsource;

import dsfml.system.common;
import dsfml.system.exception;
import dsfml.system.stringutil;
import dsfml.system.vector;

/**
* Music defines a big sound played using streaming,
* so usually what we call a music :)
*/
class Music : SoundSource!("sfMusic")
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
		
		m_ptr = sfMusic_CreateFromFile(toStringz(filename)); // TODO: this is a hack, should properly call the super constructor
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
		
		m_ptr = sfMusic_CreateFromMemory(data.ptr, data.length); // TODO: ditto
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
}

private:
	
extern(C)
{
SFMLClass function(cchar*)						sfMusic_CreateFromFile;
SFMLClass function(byte*, size_t)				sfMusic_CreateFromMemory;
void function(SFMLClass, int)					sfMusic_SetLoop;
bool function(SFMLClass)						sfMusic_GetLoop;
float function(SFMLClass)						sfMusic_GetDuration;
void function(SFMLClass)						sfMusic_Play;
void function(SFMLClass)						sfMusic_Pause;
void function(SFMLClass)						sfMusic_Stop;
uint function(SFMLClass)						sfMusic_GetChannelsCount;
uint function(SFMLClass)						sfMusic_GetSampleRate;
}

static this()
{
debug
	DllLoader dll = DllLoader.load("csfml-audio-d-2");
else
	DllLoader dll = DllLoader.load("csfml-audio-2");

	mixin(loadFromSharedLib("sfMusic_CreateFromFile"));
	mixin(loadFromSharedLib("sfMusic_CreateFromMemory"));
	mixin(loadFromSharedLib("sfMusic_SetLoop"));
	mixin(loadFromSharedLib("sfMusic_GetLoop"));
	mixin(loadFromSharedLib("sfMusic_GetDuration"));
	mixin(loadFromSharedLib("sfMusic_Play"));
	mixin(loadFromSharedLib("sfMusic_Pause"));
	mixin(loadFromSharedLib("sfMusic_Stop"));
	mixin(loadFromSharedLib("sfMusic_GetChannelsCount"));
	mixin(loadFromSharedLib("sfMusic_GetSampleRate"));
}
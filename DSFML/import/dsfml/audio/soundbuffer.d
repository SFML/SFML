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

module dsfml.audio.soundbuffer;

import dsfml.system.common;
import dsfml.system.exception;
import dsfml.system.stringutil;


/**
 *	SoundBuffer is the low-level for loading and manipulating
 *	sound buffers
 */
class SoundBuffer : DSFMLObject
{
	/**
	 *	Load the sound buffer from a file
	 *
	 *	Params: 
	 *		filename = Path of the sound file to load
	 *			
	 *	Throws:
	 *		LoadingException on failure	
	 */
	this(string filename)
	{
		if (filename is null || filename.length == 0)
			throw new LoadingException("LoadingException : Filename is invalid.");
			
		super(sfSoundBuffer_CreateFromFile(toStringz(filename)));
	}

	/**
	 *	Load the sound buffer from a file in memory
	 *
	 *	Params:	 
	 *		data = Array of file data in memory
	 *		  
	 *	Throws:
	 *		LoadingException on failure	
	 */
	this(byte[] data)
	{
		if (data is null || data.length == 0)
			throw new Exception("LoadingException : Memory stream is invalid.");

		super(sfSoundBuffer_CreateFromMemory(data.ptr, data.length)); 
	}

	/**
	 *	Load the sound buffer from an array of samples - assumed format for
	 *	samples is 16 bits signed integer
	 *
	 *	Params:	 
	 *		samples = Array of samples in memory
	 *		channelsCount = Number of channels (1 = mono, 2 = stereo, ...)
	 *		sampleRate = Frequency (number of samples to play per second)
	 *	
	 *	Throws:
	 *		LoadingException on failure		
	 */
	this(const(short)[] samples, uint channelsCount, uint sampleRate)
	{
		if (samples is null || samples.length == 0)
			throw new Exception("LoadingException : Samples array is invalid.");

		super(sfSoundBuffer_CreateFromSamples(samples.ptr, samples.length, channelsCount, sampleRate));
	}

	override void dispose()
	{
		sfSoundBuffer_Destroy(m_ptr);
	}



	/**
	 *	Save the sound buffer to a file
	 *
	 *	Params: 
	 *		filename = Path of the sound file to write
	 *
	 *	Returns: 
	 *		True if saving has been successful
	 */
	bool saveToFile(string filename) 
	{
		if (filename !is null && filename.length > 0 )
		{
			return cast(bool)sfSoundBuffer_SaveToFile(m_ptr, toStringz(filename));
		}
		return false;
	}

@property
{
	/**
	 *	Return the sound samples
	 *
	 *	Returns: 
	 *		Array of sound samples, in 16 bits signed integer format
	 */
	short[] samples() 
	{
		short* temp = null;
		temp = sfSoundBuffer_GetSamples(m_ptr);		

		return temp is null ? null : temp[0..samplesCount()];
	}

	/**
	 *	Return the samples count
	 *
	 *	Returns: 
	 *		Number of samples
	 */
	size_t samplesCount() 
	{
		return sfSoundBuffer_GetSamplesCount(m_ptr);
	}

	/**
	 *	Get the sample rate
	 *
	 *	Returns: 
	 *		Sound frequency (number of samples per second)
	 */
	uint sampleRate() 
	{
		return sfSoundBuffer_GetSampleRate(m_ptr);
	}

	/**
	 *	Return the number of channels (1 = mono, 2 = stereo, ...)
	 *
	 *	Returns:
	 *		Number of channels
	 */
	uint channelsCount() 
	{
		return sfSoundBuffer_GetChannelsCount(m_ptr);
	}

	/**
	 *	Get the sound duration
	 *
	 *	Returns: 
	 *		Sound duration, in seconds
	 */
	float duration() 
	{
		return sfSoundBuffer_GetDuration(m_ptr);
	}
}

package:	
	this(SFMLClass ptr)
	{
		super(ptr, true);
	}

private:

	static extern(C)
	{
		SFMLClass	function(cchar*)							sfSoundBuffer_CreateFromFile;
		SFMLClass	function(const(byte)*, size_t)				sfSoundBuffer_CreateFromMemory;
		SFMLClass	function(const(short)*, size_t, uint, uint)	sfSoundBuffer_CreateFromSamples;
		void	function(SFMLClass) 							sfSoundBuffer_Destroy;
		int		function(SFMLClass, cchar*) 					sfSoundBuffer_SaveToFile;
		short*	function(SFMLClass) 							sfSoundBuffer_GetSamples;
		size_t	function(SFMLClass) 							sfSoundBuffer_GetSamplesCount;
		uint	function(SFMLClass) 							sfSoundBuffer_GetSampleRate;
		uint	function(SFMLClass) 							sfSoundBuffer_GetChannelsCount;
		float	function(SFMLClass) 							sfSoundBuffer_GetDuration;
	}

	mixin(loadFromSharedLib2("csfml-audio", "sfSoundBuffer",
		"CreateFromFile", "CreateFromMemory", "CreateFromSamples", "Destroy", "SaveToFile", "GetSamples", "GetSamplesCount",
		"GetSampleRate", "GetChannelsCount", "GetDuration"));
}
/*
*   DSFML - SFML Library binding in D language.
*   Copyright (C) 2008 Julien Dagorn (sirjulio13@gmail.com)
*
*   This software is provided 'as-is', without any express or
*   implied warranty. In no event will the authors be held
*   liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose,
*   including commercial applications, and to alter it and redistribute
*   it freely, subject to the following restrictions:
*
*   1.  The origin of this software must not be misrepresented;
*       you must not claim that you wrote the original software.
*       If you use this software in a product, an acknowledgment
*       in the product documentation would be appreciated but
*       is not required.
*
*   2.  Altered source versions must be plainly marked as such,
*       and must not be misrepresented as being the original software.
*
*   3.  This notice may not be removed or altered from any
*       source distribution.
*/

module dsfml.audio.soundbuffer;

import dsfml.system.common;
import dsfml.system.exception;
import dsfml.system.stringutil;


/**
*   SoundBuffer is the low-level for loading and manipulating
*   sound buffers
*/
class SoundBuffer : DSFMLObject
{
    /**
    *   Load the sound buffer from a file
    *
    *   Params: 
    *       filename = Path of the sound file to load
    *           
    *   Throws:
    *       LoadingException on failure    
    */
    this(char[] filename)
	{
	   if (filename is null || filename.length == 0)
	       throw new LoadingException("LoadingException : Filename is invalid.");
	       
	   super(sfSoundBuffer_CreateFromFile(toStringz(filename)));
	}

    /**
    *   Load the sound buffer from a file in memory
    *
    *   Params:     
    *       data = Array of file data in memory
    *          
    *   Throws:
    *       LoadingException on failure    
    */
    this(byte[] data)
	{
        if (data is null || data.length == 0)
            throw new Exception("LoadingException : Memory stream is invalid.");

		super(sfSoundBuffer_CreateFromMemory(data.ptr, data.length)); 
	}

    /**
    *   Load the sound buffer from an array of samples - assumed format for
    *   samples is 16 bits signed integer
    *
    *   Params:     
    *       samples = Array of samples in memory
    *       channelsCount = Number of channels (1 = mono, 2 = stereo, ...)
    *       sampleRate = Frequency (number of samples to play per second)
    *   
    *   Throws:
    *       LoadingException on failure        
    */
    this(short[] samples, uint channelsCount, uint sampleRate)
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
    *   Save the sound buffer to a file
    *
    *   Params: 
    *       filename = Path of the sound file to write
    *
    *   Returns: 
    *       True if saving has been successful
    */
    bool saveToFile(char[] filename) 
    {
		if (filename !is null && filename.length > 0 )
		{
			return cast(bool)sfSoundBuffer_SaveToFile(m_ptr, toStringz(filename));
		}
		return false;
	}

    /**
    *   Return the sound samples
    *
    *   Returns: 
    *       Array of sound samples, in 16 bits signed integer format
    */
    short[] getSamples() 
	{
        short* temp = null;
        temp = sfSoundBuffer_GetSamples(m_ptr);       

		return temp is null ? null : temp[0..getSamplesCount];
	}

    /**
    *   Return the samples count
    *
    *   Returns: 
    *       Number of samples
    */
    size_t getSamplesCount() 
	{
		return sfSoundBuffer_GetSamplesCount(m_ptr);
	}

    /**
    *   Get the sample rate
    *
    *   Returns: 
    *       Sound frequency (number of samples per second)
    */
    uint getSampleRate() 
	{
		return sfSoundBuffer_GetSampleRate(m_ptr);
	}

    /**
    *   Return the number of channels (1 = mono, 2 = stereo, ...)
    *
    *   Returns:
    *       Number of channels
    */
    uint getChannelsCount() 
	{
		return sfSoundBuffer_GetChannelsCount(m_ptr);
	}

    /**
    *   Get the sound duration
    *
    *   Returns: 
    *       Sound duration, in seconds
    */
    float getDuration() 
	{
		return sfSoundBuffer_GetDuration(m_ptr);
	}

package:    
    this(void* ptr)
    {
        super(ptr, true);
    }

private:
// External ====================================================================

    extern (C)
    {
    	typedef void* function(char*) pf_sfSoundBuffer_CreateFromFile;
    	typedef void* function(byte*, size_t) pf_sfSoundBuffer_CreateFromMemory;
        typedef void* function(short*, size_t, uint, uint) pf_sfSoundBuffer_CreateFromSamples;
    	typedef void function(void*) pf_sfSoundBuffer_Destroy;
    	typedef int function(void*, char*) pf_sfSoundBuffer_SaveToFile;
    	typedef short* function(void*) pf_sfSoundBuffer_GetSamples;
    	typedef size_t function(void*) pf_sfSoundBuffer_GetSamplesCount;
    	typedef uint function(void*) pf_sfSoundBuffer_GetSampleRate;
    	typedef uint function(void*) pf_sfSoundBuffer_GetChannelsCount;
    	typedef float function(void*) pf_sfSoundBuffer_GetDuration;
    	
    	static pf_sfSoundBuffer_CreateFromFile sfSoundBuffer_CreateFromFile;
    	static pf_sfSoundBuffer_CreateFromMemory sfSoundBuffer_CreateFromMemory;
    	static pf_sfSoundBuffer_CreateFromSamples sfSoundBuffer_CreateFromSamples;
    	static pf_sfSoundBuffer_Destroy sfSoundBuffer_Destroy;
    	static pf_sfSoundBuffer_SaveToFile sfSoundBuffer_SaveToFile;
    	static pf_sfSoundBuffer_GetSamples sfSoundBuffer_GetSamples;
    	static pf_sfSoundBuffer_GetSamplesCount sfSoundBuffer_GetSamplesCount;
    	static pf_sfSoundBuffer_GetSampleRate sfSoundBuffer_GetSampleRate;
    	static pf_sfSoundBuffer_GetChannelsCount sfSoundBuffer_GetChannelsCount;
    	static pf_sfSoundBuffer_GetDuration sfSoundBuffer_GetDuration;
    }

    static this()
    {
        DllLoader dll = DllLoader.load("csfml-audio");
        
        sfSoundBuffer_CreateFromFile = cast(pf_sfSoundBuffer_CreateFromFile)dll.getSymbol("sfSoundBuffer_CreateFromFile");
        sfSoundBuffer_CreateFromMemory = cast(pf_sfSoundBuffer_CreateFromMemory)dll.getSymbol("sfSoundBuffer_CreateFromMemory");
        sfSoundBuffer_CreateFromSamples = cast(pf_sfSoundBuffer_CreateFromSamples)dll.getSymbol("sfSoundBuffer_CreateFromSamples");
        sfSoundBuffer_Destroy = cast(pf_sfSoundBuffer_Destroy)dll.getSymbol("sfSoundBuffer_Destroy");
        sfSoundBuffer_SaveToFile = cast(pf_sfSoundBuffer_SaveToFile)dll.getSymbol("sfSoundBuffer_SaveToFile");
        sfSoundBuffer_GetSamples = cast(pf_sfSoundBuffer_GetSamples)dll.getSymbol("sfSoundBuffer_GetSamples");
        sfSoundBuffer_GetSamplesCount = cast(pf_sfSoundBuffer_GetSamplesCount)dll.getSymbol("sfSoundBuffer_GetSamplesCount");
        sfSoundBuffer_GetSampleRate = cast(pf_sfSoundBuffer_GetSampleRate)dll.getSymbol("sfSoundBuffer_GetSampleRate");
        sfSoundBuffer_GetChannelsCount = cast(pf_sfSoundBuffer_GetChannelsCount)dll.getSymbol("sfSoundBuffer_GetChannelsCount");
        sfSoundBuffer_GetDuration = cast(pf_sfSoundBuffer_GetDuration)dll.getSymbol("sfSoundBuffer_GetDuration");
    }
}


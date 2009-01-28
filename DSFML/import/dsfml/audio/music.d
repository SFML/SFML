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
    *   Open a music file (doesn't play it -- call Play for that)
    *   
    *   Params:
    *       filename = Path of the file to open
    *       
    */ 
    this(char[] filename)
    {
        if (filename is null || filename.length == 0)
            throw new LoadingException("LoadingException : Filename is invalid.");
        
        super(sfMusic_CreateFromFile(toStringz(filename)));
    }

    /**
    *   Open a music file from memory (doesn't play it -- call Play() for that)
    *
    *   Params:
    *       data = file data in memory
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
    *   Start playing the audio stream
    */
    void play()
    {
        sfMusic_Play(m_ptr);
    }

    /**
    *   Stop playing the audio stream
    */
    void stop()
    {
        sfMusic_Stop(m_ptr);
    }

    /**
    *   Pause the audio stream
    */
    void pause()
    {
        sfMusic_Pause(m_ptr);
    }


    /**
    *   Return the number of channels (1 = mono, 2 = stereo)
    *
    *   Returns: 
    *       Number of channels
    */
    uint getChannelsCount()
    {
        return sfMusic_GetChannelsCount(m_ptr);
    }

    /**
    *   Get the stream sample rate
    *
    *   Returns:
    *       Stream frequency (number of samples per second)
    */
    uint getSampleRate() 
    {
        return sfMusic_GetSampleRate(m_ptr);
    }


    /**
    *   Get the music duration
    *
    *   Returns: 
    *       Music duration, in seconds
    */
    float getDuration() 
    {
        return sfMusic_GetDuration(m_ptr);
    }

    /**
    *   Get the status of the stream (stopped, paused, playing)
    *
    *   Returns: 
    *       Current status of the sound
    */
    SoundStatus getStatus() 
    {
        return sfMusic_GetStatus(m_ptr);
    }


    /**
    *   Tell whether or not the music is looping
    *
    *   Returns: 
    *       True if the music is looping, false otherwise
    */
    bool getLoop() 
    {
        return cast(bool)sfMusic_GetLoop(m_ptr);
    }

    /**
    *   Get the pitch
    *
    *   Returns: 
    *       Pitch value
    */
    float getPitch() 
    {
        return sfMusic_GetPitch(m_ptr);
    }

    /**
    *   Get the volume
    *
    *   Returns: 
    *       Volume value (in range [1, 100])
    */
    float getVolume() 
    {
        return sfMusic_GetVolume(m_ptr);
    }

    /**
    *   Get the sound position
    *
    *   Returns:
    *       Current position of the music.        
    */
    Vector3f getPosition() 
    {
        Vector3f ret;
        sfMusic_GetPosition(m_ptr, &ret.x, &ret.y, &ret.z);
        return ret;
    }

    /**
    *   Get the minimum distance
    *
    *   Returns:
    *      Minimum distance for the sound
    */
    float getMinDistance()
    {
        return sfMusic_GetMinDistance(m_ptr);
    }

    /**
    *   Get the attenuation factor
    *
    *   Returns:
    *       Attenuation factor of the sound
    *
    */
    float getAttenuation() 
    {
        return sfMusic_GetAttenuation(m_ptr);
    }


    /**
    *   Set the music loop state.
    *   This parameter is disabled by default
    *
    *   Params: 
    *       loop = True to play in loop, false to play once
    */
    void setLoop(bool loop)
    {
        sfMusic_SetLoop(m_ptr, loop);
    }

    /**
    *   Set the sound pitch.
    *   The default pitch is 1
    *
    *   Params: 
    *       pitch = New pitch
    *
    */
    void setPitch(float pitch)
    {
        sfMusic_SetPitch(m_ptr, pitch);
    }

    /**
    *   Set the sound volume.
    *   The default volume is 100
    *
    *   Params: 
    *       volume = Volume (in range [0, 100])
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
    *   Set the sound position.
    *   The default position is (0, 0, 0)
    *
    *   Params: 
    *       x = X position of the sound in the world
    *       y = Y position of the sound in the world
    *       z = Z position of the sound in the world
    *
    */
    void setPosition(float x, float y, float z)
    {
        sfMusic_SetPosition(m_ptr, x, y, z);
    }

    /**
    *   Set the sound position.
    *   The default position is (0, 0, 0)
    *
    *   Params: 
    *       position = new position
    *
    */
    void setPosition(Vector3f position)
    {
        sfMusic_SetPosition(m_ptr, position.x, position.y, position.z);
    }


    /**
    *   Set the minimum distance - closer than thsi distance
    *   the listener will hear the sound at its maximum volume.
    *   The default distance is 1.0
    *   
    *   Params:    
    *       minDistance = new minimum distance for the sound 
    */        
    void setMinDistance(float minDistance)
    {
        sfMusic_SetMinDistance(m_ptr, minDistance);
    }

    /**
    *   Set the attenuation factor - the higher the attenuation, the
    *   more the sound will be attenuated with distance from listener.
    *   The default attenuation factor 1.0
    *   
    *   Params:
    *       attenuation = new attenuation factor for the sound            
    */        
    void setAttenuation(float attenuation)
    {
        sfMusic_SetAttenuation(m_ptr, attenuation);
    }

private:

// External ====================================================================

    extern (C)
    {
        typedef void* function(char*) pf_sfMusic_CreateFromFile;
    	typedef void* function(byte*, size_t) pf_sfMusic_CreateFromMemory;
    	typedef void function(void*) pf_sfMusic_Destroy;
    	typedef void function(void*, int) pf_sfMusic_SetLoop;
    	typedef bool function(void*) pf_sfMusic_GetLoop;
    	typedef float function(void*) pf_sfMusic_GetDuration;
    	typedef void function(void*) pf_sfMusic_Play;
    	typedef void function(void*) pf_sfMusic_Pause;
    	typedef void function(void*) pf_sfMusic_Stop;
    	typedef uint function(void*) pf_sfMusic_GetChannelsCount;
    	typedef uint function(void*) pf_sfMusic_GetSampleRate;
    	typedef SoundStatus function(void*) pf_sfMusic_GetStatus;
    	typedef void function(void*, float) pf_sfMusic_SetPitch;
    	typedef void function(void*, float) pf_sfMusic_SetVolume;
    	typedef void function(void*, float, float, float) pf_sfMusic_SetPosition;
    	typedef float function(void*) pf_sfMusic_GetPitch;
    	typedef float function(void*) pf_sfMusic_GetVolume;
    	typedef void function(void*, float*, float*, float*) pf_sfMusic_GetPosition;
    	typedef float function(void*) pf_sfMusic_GetMinDistance;
    	typedef float function(void*) pf_sfMusic_GetAttenuation;
    	typedef void function(void*, float) pf_sfMusic_SetMinDistance;
    	typedef void function(void*, float) pf_sfMusic_SetAttenuation;
    	
    	static pf_sfMusic_CreateFromFile sfMusic_CreateFromFile;
    	static pf_sfMusic_CreateFromMemory sfMusic_CreateFromMemory;
    	static pf_sfMusic_Destroy sfMusic_Destroy;
    	static pf_sfMusic_SetLoop sfMusic_SetLoop;
    	static pf_sfMusic_GetLoop sfMusic_GetLoop;
    	static pf_sfMusic_GetDuration sfMusic_GetDuration;
    	static pf_sfMusic_Play sfMusic_Play;
    	static pf_sfMusic_Pause sfMusic_Pause;
    	static pf_sfMusic_Stop sfMusic_Stop;
    	static pf_sfMusic_GetChannelsCount sfMusic_GetChannelsCount;
    	static pf_sfMusic_GetSampleRate sfMusic_GetSampleRate;
    	static pf_sfMusic_GetStatus sfMusic_GetStatus;
    	static pf_sfMusic_SetPitch sfMusic_SetPitch;
    	static pf_sfMusic_SetVolume sfMusic_SetVolume;
    	static pf_sfMusic_SetPosition sfMusic_SetPosition;
    	static pf_sfMusic_GetPitch sfMusic_GetPitch;
    	static pf_sfMusic_GetVolume sfMusic_GetVolume;
    	static pf_sfMusic_GetPosition sfMusic_GetPosition;
    	static pf_sfMusic_GetMinDistance sfMusic_GetMinDistance;
    	static pf_sfMusic_GetAttenuation sfMusic_GetAttenuation;
    	static pf_sfMusic_SetMinDistance sfMusic_SetMinDistance;
    	static pf_sfMusic_SetAttenuation sfMusic_SetAttenuation;
    }

    static this()
    {
        DllLoader dll = DllLoader.load("csfml-audio");
        
        sfMusic_CreateFromFile = cast(pf_sfMusic_CreateFromFile)dll.getSymbol("sfMusic_CreateFromFile");
        sfMusic_CreateFromMemory = cast(pf_sfMusic_CreateFromMemory)dll.getSymbol("sfMusic_CreateFromMemory");
        sfMusic_Destroy = cast(pf_sfMusic_Destroy)dll.getSymbol("sfMusic_Destroy");
        sfMusic_SetLoop = cast(pf_sfMusic_SetLoop)dll.getSymbol("sfMusic_SetLoop");
        sfMusic_GetLoop = cast(pf_sfMusic_GetLoop)dll.getSymbol("sfMusic_GetLoop");
        sfMusic_GetDuration = cast(pf_sfMusic_GetDuration)dll.getSymbol("sfMusic_GetDuration");
        sfMusic_Play = cast(pf_sfMusic_Play)dll.getSymbol("sfMusic_Play");
        sfMusic_Pause = cast(pf_sfMusic_Pause)dll.getSymbol("sfMusic_Pause");
        sfMusic_Stop = cast(pf_sfMusic_Stop)dll.getSymbol("sfMusic_Stop");
        sfMusic_GetChannelsCount = cast(pf_sfMusic_GetChannelsCount)dll.getSymbol("sfMusic_GetChannelsCount");
        sfMusic_GetSampleRate = cast(pf_sfMusic_GetSampleRate)dll.getSymbol("sfMusic_GetSampleRate");
        sfMusic_GetStatus = cast(pf_sfMusic_GetStatus)dll.getSymbol("sfMusic_GetStatus");
        sfMusic_SetPitch = cast(pf_sfMusic_SetPitch)dll.getSymbol("sfMusic_SetPitch");
        sfMusic_SetVolume = cast(pf_sfMusic_SetVolume)dll.getSymbol("sfMusic_SetVolume");
        sfMusic_SetPosition = cast(pf_sfMusic_SetPosition)dll.getSymbol("sfMusic_SetPosition");
        sfMusic_GetPitch = cast(pf_sfMusic_GetPitch)dll.getSymbol("sfMusic_GetPitch");
        sfMusic_GetVolume = cast(pf_sfMusic_GetVolume)dll.getSymbol("sfMusic_GetVolume");
        sfMusic_GetPosition = cast(pf_sfMusic_GetPosition)dll.getSymbol("sfMusic_GetPosition");
        sfMusic_GetMinDistance = cast(pf_sfMusic_GetMinDistance)dll.getSymbol("sfMusic_GetMinDistance");
        sfMusic_GetAttenuation = cast(pf_sfMusic_GetAttenuation)dll.getSymbol("sfMusic_GetAttenuation");
        sfMusic_SetMinDistance = cast(pf_sfMusic_SetMinDistance)dll.getSymbol("sfMusic_SetMinDistance");
        sfMusic_SetAttenuation = cast(pf_sfMusic_SetAttenuation)dll.getSymbol("sfMusic_SetAttenuation");
    }
}


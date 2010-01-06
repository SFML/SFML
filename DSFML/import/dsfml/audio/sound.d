/*
*   DSFML - SFML Library wrapper for the D programming language.
*   Copyright (C) 2008 Julien Dagorn (sirjulio13@gmail.com)
*   Copyright (C) 2010 Andreas Hollandt
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

module dsfml.audio.sound;

import dsfml.audio.soundbuffer;
import dsfml.audio.soundstatus;

import dsfml.system.common;
import dsfml.system.exception;
import dsfml.system.vector3;

/**
*   Sound defines the properties of the sound such as position,
*   volume, pitch, etc.
*/
class Sound : DSFMLObject
{
    /**
    *   Default constructor
    */
    this()
	{
		super(sfSound_Create());
	}

    /**
    *   Construct the sound from its parameters
    *
    *   Params:    
    *       buffer = Sound buffer to play
    *       loop = Loop flag (false by default)
    *       pitch = Value of the pitch (1 by default)
    *       volume = Volume (100 by default)
    *       x = X position (0 by default)
    *       y = Y position (0 by default)
    *       z = Z position (0 by default)
    *       
    *   Throws:
    *       NullParameterException if buffer is null            
    */
    this(SoundBuffer buffer, bool loop = false, float pitch = 1.f, float volume = 100.f, float x = 0.f, float y = 0.f, float z = 0.f)
	{
        if (buffer is null)
            throw new NullParameterException("NullParameterException : SoundBuffer is null.");
            
		super(sfSound_Create());
		sfSound_SetBuffer(m_ptr, buffer.getNativePointer());
		sfSound_SetLoop(m_ptr, loop);
		sfSound_SetPitch(m_ptr, pitch);
		sfSound_SetVolume(m_ptr, volume);
		sfSound_SetPosition(m_ptr, x, y, z);
	}


    override void dispose()
	{
		sfSound_Destroy(m_ptr);
	}

    /**
    *   Play the sound
    */
    void play()
	{
		sfSound_Play(m_ptr);
	}

    /**
    *   Pause the sound
    */
    void pause()
	{
		sfSound_Pause(m_ptr);
	}

    /**
    *   Stop the sound
    */
    void stop()
	{
		sfSound_Stop(m_ptr);
	}

    /**
    *   Set the source buffer
    *
    *   Params: 
    *       buffer = New sound buffer to bind to the sound
    */
    void setBuffer(SoundBuffer buffer)
	{
        if (buffer is null)
            throw new NullParameterException("NullParameterException : SoundBuffer is null.");
            
		m_buffer = buffer;
		sfSound_SetBuffer(m_ptr, buffer.getNativePointer);
	}

    /**
    *   Set the sound loop state.
    *   This parameter is disabled by default
    *
    *   Params: 
    *       loop = True to play in loop, false to play once
    */
    void setLoop(bool loop)
	{
		sfSound_SetLoop(m_ptr, loop);
	}

    /**
    *   Set the sound pitch.
    *   The default pitch is 1
    *
    *   Params: 
    *       pitch = New pitch
    */
    void setPitch(float pitch)
	{
		sfSound_SetPitch(m_ptr, pitch);
	}

    /**
    *   Set the sound volume.
    *   The default volume is 100
    *
    *   Params: 
    *       volume = Volume (in range [0, 100])
    */
    void setVolume(float volume)
    in
    {
        assert(volume >= 0 && volume <= 100);
    }
    body
	{
		sfSound_SetVolume(m_ptr, volume);
	}

    /**
    *   Set the sound position.
    *   The default position is (0, 0, 0)
    *
    *   Params: 
    *       x = X position of the sound in the world
    *       y = Y position of the sound in the world
    *       z = Z position of the sound in the world
    */
    void setPosition(float x, float y, float z)
	{
		sfSound_SetPosition(m_ptr, x, y, z);
	}

    /**
    *   Set the sound position.
    *   The default position is (0, 0, 0)
    *   
    *   Params :
    *       position = new position                
    */        
    void setPosition(Vector3f position)
    {
        sfSound_SetPosition(m_ptr, position.x, position.y, position.z);
    }

    /**
    *   Set the minimum distance - closer than this distance
    *   the listener will hear the sound at its maximum volume.
    *   The default distance is 1.0
    *   
    *   Params:    
    *       minDistance = new minimum distance for the sound 
    */        
    void setMinDistance(float minDistance)
    {
        sfSound_SetMinDistance(m_ptr, minDistance);
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
        sfSound_SetAttenuation(m_ptr, attenuation);
    }

    /**
    *   Set the current playing offset of a sound
    *   
    *   Params:
    *       offset = new playing position, expressed in seconds                    
    */
    void setPlayingOffset(float offset)
    {
        sfSound_SetPlayingOffset(m_ptr, offset);
    }
    

    /**
    *   Get the source buffer
    *
    *   Returns: 
    *       Sound buffer bound to the sound (can be NULL)
    */
    SoundBuffer getBuffer() 
	{
		return m_buffer;
	}

    /**
    *   Tell whether or not the sound is looping
    *
    *   Returns: 
    *       True if the sound is looping, false otherwise
    */
    bool getLoop() 
	{

		return cast(bool)(sfSound_GetLoop(m_ptr));
	}

    /**
    *   Get the pitch
    *
    *   Returns: 
    *       Pitch value
    */
    float getPitch() 
	{
		return sfSound_GetPitch(m_ptr);
	}

    /**
    *   Get the volume
    *
    *   Returns: 
    *       Volume value (in range [1, 100])
    */
    float getVolume() 
	{
		return sfSound_GetVolume(m_ptr);
	}

    /**
    *   Get the sound position
    *
    *   Params:    
    *       x = X position of the sound in the world
    *       y = Y position of the sound in the world
    *       z = Z position of the sound in the world
    */
    Vector3f getPosition() 
	{
        Vector3f ret;
		sfSound_GetPosition(m_ptr, &ret.x, &ret.y, &ret.z);
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
        return sfSound_GetMinDistance(m_ptr);
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
        return sfSound_GetAttenuation(m_ptr);
    }


    /**
    *   Get the status of the sound (stopped, paused, playing)
    *
    *   Returns: 
    *       Current status of the sound
    */
    SoundStatus getStatus() 
	{
		return sfSound_GetStatus(m_ptr);
	}

    /**
    *   Get the current playing position of the sound
    *
    *   Returns:  
    *       Current playing position, expressed in seconds
    */
    float getPlayingOffset() 
	{
		return sfSound_GetPlayingOffset(m_ptr);
	}

	/**
	 * Make the sound's position relative to the listener's position, or absolute.
	 * The default value is false (absolute)
	 * 
	 *	Params:
	 *		relative = True to set the position relative, false to set it absolute
	 */
	void setRelativeToListener(bool relative)
	{
		sfSound_SetRelativeToListener(m_ptr, relative);
	}

	/**
	 * Tell if the sound's position is relative to the listener's
	 * position, or if it's absolute
	 * 
	 *	Returns:
	 *		true if the position is relative, sfFalse if it's absolute
	 */
	bool isRelativeToListener()
	{
		return sfSound_IsRelativeToListener(m_ptr);
	}


private:
	SoundBuffer m_buffer;
	
// External ====================================================================

    extern (C)
    {
        typedef void* function() pf_sfSound_Create;
    	typedef void function(void*) pf_sfSound_Destroy;
    	typedef void function(void*) pf_sfSound_Play;
    	typedef void function(void*) pf_sfSound_Pause;
    	typedef void function(void*) pf_sfSound_Stop;
    	typedef void function(void*, void*) pf_sfSound_SetBuffer;
    	typedef void* function(void*) pf_sfSound_GetBuffer;
    	typedef void function(void*, int) pf_sfSound_SetLoop;
    	typedef int function(void*) pf_sfSound_GetLoop;
    	typedef SoundStatus function(void*) pf_sfSound_GetStatus;
    	typedef void function(void*, float) pf_sfSound_SetPitch;
    	typedef void function(void*, float) pf_sfSound_SetVolume;
    	typedef void function(void*, float, float, float) pf_sfSound_SetPosition;
    	typedef float function(void*) pf_sfSound_GetPitch;
    	typedef float function(void*) pf_sfSound_GetVolume;
    	typedef void function(void*, float*, float*, float*) pf_sfSound_GetPosition;
    	typedef float function(void*) pf_sfSound_GetPlayingOffset;
    	typedef float function(void*) pf_sfSound_GetMinDistance;
    	typedef float function(void*) pf_sfSound_GetAttenuation;
    	typedef void function(void*, float) pf_sfSound_SetMinDistance;
    	typedef void function(void*, float) pf_sfSound_SetAttenuation;
        typedef void function(void*, float) pf_sfSound_SetPlayingOffset;
        
        static void function(void*, bool)				sfSound_SetRelativeToListener;
        static bool function(void*)						sfSound_IsRelativeToListener;
    	
    	static pf_sfSound_Create sfSound_Create;
    	static pf_sfSound_Destroy sfSound_Destroy;
    	static pf_sfSound_Play sfSound_Play;
    	static pf_sfSound_Pause sfSound_Pause;
    	static pf_sfSound_Stop sfSound_Stop;
    	static pf_sfSound_SetBuffer sfSound_SetBuffer;
    	static pf_sfSound_GetBuffer sfSound_GetBuffer;
    	static pf_sfSound_SetLoop sfSound_SetLoop;
    	static pf_sfSound_GetLoop sfSound_GetLoop;
    	static pf_sfSound_GetStatus sfSound_GetStatus;
    	static pf_sfSound_SetPitch sfSound_SetPitch;
    	static pf_sfSound_SetVolume sfSound_SetVolume;
    	static pf_sfSound_SetPosition sfSound_SetPosition;
    	static pf_sfSound_GetPitch sfSound_GetPitch;
    	static pf_sfSound_GetVolume sfSound_GetVolume;
    	static pf_sfSound_GetPosition sfSound_GetPosition;
    	static pf_sfSound_GetPlayingOffset sfSound_GetPlayingOffset;
    	static pf_sfSound_GetMinDistance sfSound_GetMinDistance;
    	static pf_sfSound_GetAttenuation sfSound_GetAttenuation;
    	static pf_sfSound_SetMinDistance sfSound_SetMinDistance;
    	static pf_sfSound_SetAttenuation sfSound_SetAttenuation;
    	static pf_sfSound_SetPlayingOffset sfSound_SetPlayingOffset;
    }

    static this()
    {
	debug
		DllLoader dll = DllLoader.load("csfml-audio-d");
	else
		DllLoader dll = DllLoader.load("csfml-audio");
        
        sfSound_Create = cast(pf_sfSound_Create)dll.getSymbol("sfSound_Create");
        sfSound_Destroy = cast(pf_sfSound_Destroy)dll.getSymbol("sfSound_Destroy");
        sfSound_Play = cast(pf_sfSound_Play)dll.getSymbol("sfSound_Play");
        sfSound_Pause = cast(pf_sfSound_Pause)dll.getSymbol("sfSound_Pause");
        sfSound_Stop = cast(pf_sfSound_Stop)dll.getSymbol("sfSound_Stop");
        sfSound_SetBuffer = cast(pf_sfSound_SetBuffer)dll.getSymbol("sfSound_SetBuffer");
        sfSound_GetBuffer = cast(pf_sfSound_GetBuffer)dll.getSymbol("sfSound_GetBuffer");
        sfSound_SetLoop = cast(pf_sfSound_SetLoop)dll.getSymbol("sfSound_SetLoop");
        sfSound_GetLoop = cast(pf_sfSound_GetLoop)dll.getSymbol("sfSound_GetLoop");
        sfSound_GetStatus = cast(pf_sfSound_GetStatus)dll.getSymbol("sfSound_GetStatus");
        sfSound_SetPitch = cast(pf_sfSound_SetPitch)dll.getSymbol("sfSound_SetPitch");
        sfSound_SetVolume = cast(pf_sfSound_SetVolume)dll.getSymbol("sfSound_SetVolume");
        sfSound_SetPosition = cast(pf_sfSound_SetPosition)dll.getSymbol("sfSound_SetPosition");
        sfSound_GetPitch = cast(pf_sfSound_GetPitch)dll.getSymbol("sfSound_GetPitch");
        sfSound_GetVolume = cast(pf_sfSound_GetVolume)dll.getSymbol("sfSound_GetVolume");
        sfSound_GetPosition = cast(pf_sfSound_GetPosition)dll.getSymbol("sfSound_GetPosition");
        sfSound_GetPlayingOffset = cast(pf_sfSound_GetPlayingOffset)dll.getSymbol("sfSound_GetPlayingOffset");
        sfSound_GetMinDistance = cast(pf_sfSound_GetMinDistance)dll.getSymbol("sfSound_GetMinDistance");
        sfSound_GetAttenuation = cast(pf_sfSound_GetAttenuation)dll.getSymbol("sfSound_GetAttenuation");
        sfSound_SetMinDistance = cast(pf_sfSound_SetMinDistance)dll.getSymbol("sfSound_SetMinDistance");
        sfSound_SetAttenuation = cast(pf_sfSound_SetAttenuation)dll.getSymbol("sfSound_SetAttenuation");
        sfSound_SetPlayingOffset = cast(pf_sfSound_SetPlayingOffset)dll.getSymbol("sfSound_SetPlayingOffset");
        
    	mixin(loadFromSharedLib("sfSound_SetRelativeToListener"));
    	mixin(loadFromSharedLib("sfSound_IsRelativeToListener"));

    }
}

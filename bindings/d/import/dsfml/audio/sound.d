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

module dsfml.audio.sound;

import dsfml.audio.soundbuffer;
import dsfml.audio.soundsource;

import dsfml.system.common;
import dsfml.system.exception;
import dsfml.system.vector;

/**
  *	Sound defines the properties of the sound such as position,
  *	volume, pitch, etc.
  */
class Sound : SoundSource!("sfSound")
{
	/**
	 *	Default constructor
	 */
	this()
	{
		super();
	}

	/**
	 *	Construct the sound from its parameters
	 *
	 *	Params:	
	 *		soundbuffer = Sound buffer to play
	 *		loop = Loop flag (false by default)
	 *		pitch = Value of the pitch (1 by default)
	 *		volume = Volume (100 by default)
	 *		x = X position (0 by default)
	 *		y = Y position (0 by default)
	 *		z = Z position (0 by default)
	 *		
	 *	Throws:
	 *		NullParameterException if buffer is null
	 */
	this(SoundBuffer soundbuffer, bool loop = false, float pitch = 1.f, float volume = 100.f, float x = 0.f, float y = 0.f, float z = 0.f)
	{
		if (soundbuffer is null)
			throw new NullParameterException("NullParameterException : SoundBuffer is null.");
			
		super();
		buffer = soundbuffer;
		loop = loop;
		pitch = pitch;
		volume = volume;
		setPosition(x, y, z);
	}


	/**
	 *	Play the sound
	 */
	void play()
	{
		sfSound_Play(m_ptr);
	}

	/**
	 *	Pause the sound
	 */
	void pause()
	{
		sfSound_Pause(m_ptr);
	}

	/**
	 *	Stop the sound
	 */
	void stop()
	{
		sfSound_Stop(m_ptr);
	}

@property
{
	/**
	 *	Set the source buffer
	 *
	 *	Params: 
	 *		buffer = New sound buffer to bind to the sound
	 */
	void buffer(SoundBuffer buffer)
	{
		if (buffer is null)
			throw new NullParameterException("NullParameterException : SoundBuffer is null.");
			
		m_buffer = buffer;
		sfSound_SetBuffer(m_ptr, buffer.nativePointer);
	}

	/**
	 *	Set the sound loop state.
	 *	This parameter is disabled by default
	 *
	 *	Params: 
	 *		loop = True to play in loop, false to play once
	 */
	void loop(bool loop)
	{
		sfSound_SetLoop(m_ptr, loop);
	}

	/**
	 *	Set the current playing offset of a sound
	 *	
	 *	Params:
	 *		offset = new playing position, expressed in seconds					
	 */
	void playingOffset(float offset)
	{
		sfSound_SetPlayingOffset(m_ptr, offset);
	}
	

	/**
	 *	Get the source buffer
	 *
	 *	Returns: 
	 *		Sound buffer bound to the sound (can be NULL)
	 */
	SoundBuffer buffer() 
	{
		return m_buffer;
	}

	/**
	 *	Tell whether or not the sound is looping
	 *
	 *	Returns: 
	 *		True if the sound is looping, false otherwise
	 */
	bool loop() 
	{

		return cast(bool)(sfSound_GetLoop(m_ptr));
	}

	/**
	 *	Get the current playing position of the sound
	 *
	 *	Returns:  
	 *		Current playing position, expressed in seconds
	 */
	float playingOffset() 
	{
		return sfSound_GetPlayingOffset(m_ptr);
	}
}

private:
	SoundBuffer m_buffer;
	
// External ====================================================================

	extern (C)
	{
		typedef void function(SFMLClass) pf_sfSound_Play;
		typedef void function(SFMLClass) pf_sfSound_Pause;
		typedef void function(SFMLClass) pf_sfSound_Stop;
		typedef void function(SFMLClass, SFMLClass) pf_sfSound_SetBuffer;
		typedef SFMLClass function(SFMLClass) pf_sfSound_GetBuffer;
		typedef void function(SFMLClass, int) pf_sfSound_SetLoop;
		typedef int function(SFMLClass) pf_sfSound_GetLoop;
		typedef float function(SFMLClass) pf_sfSound_GetPlayingOffset;
		typedef void function(SFMLClass, float) pf_sfSound_SetPlayingOffset;
		
		static pf_sfSound_Play sfSound_Play;
		static pf_sfSound_Pause sfSound_Pause;
		static pf_sfSound_Stop sfSound_Stop;
		static pf_sfSound_SetBuffer sfSound_SetBuffer;
		static pf_sfSound_GetBuffer sfSound_GetBuffer;
		static pf_sfSound_SetLoop sfSound_SetLoop;
		static pf_sfSound_GetLoop sfSound_GetLoop;
		static pf_sfSound_GetPlayingOffset sfSound_GetPlayingOffset;
		static pf_sfSound_SetPlayingOffset sfSound_SetPlayingOffset;
	}

	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-audio-d-2");
	else
		DllLoader dll = DllLoader.load("csfml-audio-2");
		
		sfSound_Play = cast(pf_sfSound_Play)dll.getSymbol("sfSound_Play");
		sfSound_Pause = cast(pf_sfSound_Pause)dll.getSymbol("sfSound_Pause");
		sfSound_Stop = cast(pf_sfSound_Stop)dll.getSymbol("sfSound_Stop");
		sfSound_SetBuffer = cast(pf_sfSound_SetBuffer)dll.getSymbol("sfSound_SetBuffer");
		sfSound_GetBuffer = cast(pf_sfSound_GetBuffer)dll.getSymbol("sfSound_GetBuffer");
		sfSound_SetLoop = cast(pf_sfSound_SetLoop)dll.getSymbol("sfSound_SetLoop");
		sfSound_GetLoop = cast(pf_sfSound_GetLoop)dll.getSymbol("sfSound_GetLoop");
		sfSound_GetPlayingOffset = cast(pf_sfSound_GetPlayingOffset)dll.getSymbol("sfSound_GetPlayingOffset");
		sfSound_SetPlayingOffset = cast(pf_sfSound_SetPlayingOffset)dll.getSymbol("sfSound_SetPlayingOffset");
	}
}
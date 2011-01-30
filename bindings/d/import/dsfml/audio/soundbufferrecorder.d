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

module dsfml.audio.soundbufferrecorder;

import dsfml.system.common;

import dsfml.audio.soundbuffer;
import dsfml.audio.soundrecorder;

/**
*	Specialized sfSoundRecorder which saves the captured
*	audio data into a sound buffer
*/
class SoundBufferRecorder : SoundRecorder
{
	/**
	*	Constructor
	*/
	this()
	{
		super(sfSoundBufferRecorder_Create());
	}
	
	override void dispose()
	{
		sfSoundBufferRecorder_Destroy(m_ptr);
	}

	/**
	*	Get the sound buffer containing the captured audio data
	*
	*	Returns: 
	*		SoundBuffer containing the captured audio data 
	*
	*/
	SoundBuffer getBuffer()
	{
		return new SoundBuffer(sfSoundBufferRecorder_GetBuffer(m_ptr));
	}

protected:
	
	override bool onStart()
	{
		return true;
	}

	override bool onProcessSamples(short[] s)
	{
		return true;
	}
	
	override void onStop()
	{
		
	}
		
private:
	
// External ====================================================================

	extern (C)
	{
		typedef SFMLClass function() pf_sfSoundBufferRecorder_Create;
		typedef void function(SFMLClass) pf_sfSoundBufferRecorder_Destroy;
		typedef SFMLClass function(SFMLClass) pf_sfSoundBufferRecorder_GetBuffer;
	
		static pf_sfSoundBufferRecorder_Create sfSoundBufferRecorder_Create;
		static pf_sfSoundBufferRecorder_Destroy sfSoundBufferRecorder_Destroy;
		static pf_sfSoundBufferRecorder_GetBuffer sfSoundBufferRecorder_GetBuffer;
	}

	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-audio-d-2");
	else
		DllLoader dll = DllLoader.load("csfml-audio-2");
		
		sfSoundBufferRecorder_Create = cast(pf_sfSoundBufferRecorder_Create)dll.getSymbol("sfSoundBufferRecorder_Create");
		sfSoundBufferRecorder_Destroy = cast(pf_sfSoundBufferRecorder_Destroy)dll.getSymbol("sfSoundBufferRecorder_Destroy");
		sfSoundBufferRecorder_GetBuffer = cast(pf_sfSoundBufferRecorder_GetBuffer)dll.getSymbol("sfSoundBufferRecorder_GetBuffer");
	}
}


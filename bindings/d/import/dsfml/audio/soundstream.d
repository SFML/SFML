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

module dsfml.audio.soundstream;

import dsfml.system.alloc;
import dsfml.system.common;
import dsfml.system.vector;
import dsfml.system.linkedlist;
import dsfml.system.lock;

import core.thread;
import core.sync.mutex;

import dsfml.audio.sound;
import dsfml.audio.soundsource;

/**
*	SoundStream is a streamed sound, ie samples are acquired
*	while the sound is playing. Use it for big sounds that would
*	require hundreds of MB in memory, or for streaming sound from the network.
*	
*	SoundStream is a base class and cannot be instanciated directly.
*	
*	$(B onGetData override will be called by a different thread, take care of synchronization issues.) onStart is called by the thread which called .play().
*	
*	------------------------
*	class MySoundStream : SoundStream
*	{
*		this()
*		{
*			super(2, 44100); // you need to initialize the base class before any operation.	
*		}
*		protected bool onGetData(out short[] data)
*		{
*			//You need to fill data array with some samples
*			
*			return true; //or false if you want to stop playback
*		}
*		
*		protected bool onStart()
*		{
*			return true;	
*		}  
*	}
*	------------------------
*/
abstract class SoundStream : SoundSource!("sfSoundStream")
{
	override void dispose()
	{
		stop();
		sfSoundStream_Destroy(m_ptr);
		s_instances.remove(m_id);
	}
	
	/**
	*	Start playing the stream
	*/		
	void play()
	{
		m_flag = true;			
		sfSoundStream_Play(m_ptr);
		
		if (status != SoundStatus.Paused)
		{
			m_t = new Thread(&threadPoll);
			m_t.start();
		}
	}
	
	/**
	*	Pause the stream
	*/		
	void pause()
	{
		sfSoundStream_Pause(m_ptr);
	}

	/**
	*	Stop the stream
	*/		
	void stop()
	{
		m_flag = false;
		sfSoundStream_Stop(m_ptr);
		m_t.join();
		if (m_dummy !is null)
			delete m_dummy;
	}

@property
{
	/**
	*	Get number of channels of the stream
	*	
	*	Returns:
	*		number of channels			
	*/
	uint channelsCount()
	{
		return m_channelsCount;
	}

	/**
	*	Get the sample rate of the stream
	*	
	*	Returns:
	*		sample rate			
	*/		
	uint sampleRate()
	{
		return m_sampleRate;
	}

	/**
	*	Get the current playing offset of the stream
	*	
	*	Returns:
	*		current playing offset, in seconds.			
	*/		
	float playingOffset()
	{
		return sfSoundStream_GetPlayingOffset(m_ptr);
	}

	/**
	 *	Set the current playing position of a music
	 *
	 *	Params:
	 *	    timeOffset = New playing position, expressed in seconds
	 */
	void playingOffset(float timeOffset)
	{
		sfSoundStream_SetPlayingOffset(m_ptr, timeOffset);
	}
	
	/**
	*	Tell whether or not the stream is looping
	*
	*	Returns:
	*		True if the music is looping, false otherwise
	*/
	bool loop()
	{
		if (m_ptr !is null)
			return cast(bool)sfSoundStream_GetLoop(m_ptr);
		return false;
	}

	/**
	*	Set the stream loop state.
	*	
	*	Disabled by default.
	*	
	*	Params:
	*		loop = true to play in loop, false to play once					
	*/		
	void loop(bool loop)
	{
		if (m_ptr !is null)
			sfSoundStream_SetLoop(m_ptr, loop);
	}
} // of @property

protected:
	/**
	*	Protected constructor
	*		
	*	Params:
	*		channelsCount = number of channel
	*		sampleRate = sample rate of the stream
	*
	*/		
	this(uint channelsCount, uint sampleRate)
	{
		m_channelsCount = channelsCount;
		m_sampleRate = sampleRate;

		super(sfSoundStream_Create(&externalOnGetData, &externalOnSeek, channelsCount, sampleRate, &m_id));

		m_mutex = new Mutex();
		
		m_samples = new LinkedList!(Data);
		
		m_t = new Thread(&this.threadPoll);
		
		m_id = ++s_seed;
		s_instances[m_id] = this;
	}
	
	/**
	*	Called each time the stream is seeked
	*/		
	abstract void onSeek(float timeOffset);
	
	/**
	*	Called each time the stream needs new data.
	*	This method will be call by an other thread, take care of possible synchronisation issues.
	*	
	*	Params:
	*		data = array of samples to stream
	*	
	*	Returns:
	*		true to continue streaming, false to stop						
	*/		
	abstract bool onGetData(out short[] data);
private:

	// Called sync when user calling play()
	// TODO: check if it's correct that way
	extern(C) static void externalOnSeek(float t, void* user)
	{
		int id;
		if ((id = *cast(int*) user) in s_instances)
		{
			SoundStream temp = s_instances[id];
			return (temp.onSeek(t));
		}
	}
	
	// C Thread callback (no allocation can be done)
	extern (C) static int externalOnGetData(sfSoundStreamChunk* data, void* user)
	{
		int id, flag = false;
		// Get calling soundStream
		if ((id = *cast(int*) user) in s_instances)
		{
			SoundStream temp = s_instances[id];
			//if no samples are available but streaming is not stopped, we sleep the thread
			while (temp.m_samples.empty && temp.m_flag)
				Thread.sleep(10_000_0); // 10ms
			
			scope Lock l = new Lock(temp.m_mutex);
			if (!temp.m_samples.empty)
			{
				if (temp.m_dummy !is null)
					delete temp.m_dummy;
					
				temp.m_dummy = temp.m_samples.dequeue;
				
				if ((flag = temp.m_dummy.Flag) == true)
				{
					data.Samples = temp.m_dummy.Samples.ptr;
					data.NbSamples = temp.m_dummy.Samples.length;
				}
				else
				{
					data.Samples = null;
					data.NbSamples = 0;
				}
			}
		}
		return flag;
	}
	
	// Managed thread loop
	void threadPoll()
	{
		short[] data;
		bool ret = true;
		// while streaming is active ...
		while (ret && m_flag)
		{
			{
				scope Lock l = new Lock(m_mutex);
				// see how many samples are available (keep always 2 samples ready)
				if (m_samples.getCount < 2)
				{
					// if we need new samples, lock and call derived class
					ret = onGetData(data);
					m_samples.enqueue(new Data(data, ret));
				}
			}
			Thread.sleep(100_000_0); // 100ms
		}
	}
	
	private class Data
	{
		short[] Samples;
		bool Flag;
		
		mixin Alloc;
		
		this (short[] samples, bool flag)
		{
			this.Samples = samples;
			this.Flag = flag;
		}
	}
		
	Thread m_t;
	Mutex m_mutex;
	LinkedList!(Data) m_samples;
	Data m_dummy;
	
	bool m_flag;
	
	uint m_channelsCount;
	uint m_sampleRate;
	
	int m_id;
	static SoundStream[int] s_instances;
	static int s_seed = 0;
	
// External ====================================================================

	extern (C)
	{
		struct sfSoundStreamChunk{ short* Samples; uint NbSamples; }
		
		alias void function(float, void*) sfSoundStreamSeekCallback;
		alias int function (sfSoundStreamChunk*, void*) sfSoundStreamGetDataCallback;
		
		alias SFMLClass function(sfSoundStreamGetDataCallback, sfSoundStreamSeekCallback, uint, uint, void*) pf_sfSoundStream_Create;
		alias void function(SFMLClass) pf_sfSoundStream_Destroy;
		alias void function(SFMLClass) pf_sfSoundStream_Play;
		alias void function(SFMLClass) pf_sfSoundStream_Pause;
		alias void function(SFMLClass) pf_sfSoundStream_Stop;
		alias uint function(SFMLClass) pf_sfSoundStream_GetChannelsCount;
		alias uint function(SFMLClass) pf_sfSoundStream_GetSampleRate;
		alias float function(SFMLClass) pf_sfSoundStream_GetPlayingOffset;
		alias void function(SFMLClass, float) pf_sfSoundStream_SetPlayingOffset;
		alias int function(SFMLClass) pf_sfSoundStream_GetLoop;
		alias void function(SFMLClass, int) pf_sfSoundStream_SetLoop;


		static pf_sfSoundStream_Create sfSoundStream_Create;
		static pf_sfSoundStream_Destroy sfSoundStream_Destroy;
		static pf_sfSoundStream_Play sfSoundStream_Play;
		static pf_sfSoundStream_Pause sfSoundStream_Pause;
		static pf_sfSoundStream_Stop sfSoundStream_Stop;
		static pf_sfSoundStream_GetChannelsCount sfSoundStream_GetChannelsCount;
		static pf_sfSoundStream_GetSampleRate sfSoundStream_GetSampleRate;
		static pf_sfSoundStream_GetPlayingOffset sfSoundStream_GetPlayingOffset;
		static pf_sfSoundStream_SetPlayingOffset sfSoundStream_SetPlayingOffset;
		static pf_sfSoundStream_GetLoop sfSoundStream_GetLoop;
		static pf_sfSoundStream_SetLoop sfSoundStream_SetLoop;
	}

	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-audio-d-2");
	else
		DllLoader dll = DllLoader.load("csfml-audio-2");
		
		sfSoundStream_Create = cast(pf_sfSoundStream_Create)dll.getSymbol("sfSoundStream_Create");
		sfSoundStream_Destroy = cast(pf_sfSoundStream_Destroy)dll.getSymbol("sfSoundStream_Destroy");
		sfSoundStream_Play = cast(pf_sfSoundStream_Play)dll.getSymbol("sfSoundStream_Play");
		sfSoundStream_Pause = cast(pf_sfSoundStream_Pause)dll.getSymbol("sfSoundStream_Pause");
		sfSoundStream_Stop = cast(pf_sfSoundStream_Stop)dll.getSymbol("sfSoundStream_Stop");
		sfSoundStream_GetChannelsCount = cast(pf_sfSoundStream_GetChannelsCount)dll.getSymbol("sfSoundStream_GetChannelsCount");
		sfSoundStream_GetSampleRate = cast(pf_sfSoundStream_GetSampleRate)dll.getSymbol("sfSoundStream_GetSampleRate");
		sfSoundStream_GetPlayingOffset = cast(pf_sfSoundStream_GetPlayingOffset)dll.getSymbol("sfSoundStream_GetPlayingOffset");
		sfSoundStream_SetPlayingOffset = cast(pf_sfSoundStream_SetPlayingOffset)dll.getSymbol("sfSoundStream_SetPlayingOffset");
		sfSoundStream_GetLoop = cast(pf_sfSoundStream_GetLoop)dll.getSymbol("sfSoundStream_GetLoop");
		sfSoundStream_SetLoop = cast(pf_sfSoundStream_SetLoop)dll.getSymbol("sfSoundStream_SetLoop");
	}
}
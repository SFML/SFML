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

module dsfml.audio.soundrecorder;

import dsfml.audio.soundbuffer;

import dsfml.system.alloc;
import dsfml.system.common;

import dsfml.system.linkedlist;
import dsfml.system.lock;

import core.thread;
import core.sync.mutex;

/**
*	SoundRecorder is an interface for capturing sound data.
*	
*	$(B onProcessSamples and onStop will be called by a different thread, take care of synchronization issues.)
*	
*	Examples:
*	-------
*	class MySoundRecorder : SoundRecorder
*	{
*		this()
*		{
*			 
*		}
*		
*		protected bool onStart()
*		{
*			return true;
*		}
*	
*		protected void onStop()
*		{
*			 
*		}
*		
*		protected bool onProcessSamples(out short[])
*		{
*			// Process data here
*			
*			return true; //return true to continue capture, else return false
*		}
*	}
*	-------
*/
abstract class SoundRecorder : DSFMLObject
{
	override void dispose()
	{
		if (m_flag)
			stop();

		m_instances.remove(m_id);
		sfSoundRecorder_Destroy(m_ptr);
	}


	/**
	*	Start the capture.
	*	
	*	Only one capture can happen at the same time
	*
	*	Params: 
	*		sampleRate : Sound frequency (the more samples, the higher the quality)
	*					(44100 by default = CD quality)
	*/
	void start(uint sampleRate = 44100)
	{
		sfSoundRecorder_Start(m_ptr, sampleRate);
		m_t = new Thread(&threadPoll);
		m_t.start();
	}

	/**
	*	Stop the capture
	*/
	void stop()
	{
		sfSoundRecorder_Stop(m_ptr);
		m_flag = false;
		m_t.join();
		m_t = null;
	}

	/**
	*	Get the sample rate
	*
	*	Returns: 
	*		Frequency, in samples per second
	*/
	uint getSampleRate() 
	{
		return sfSoundRecorder_GetSampleRate(m_ptr);
	}

	/**
	*	Tell if the system supports sound capture.
	*	If not, this class won't be usable
	*
	*	Returns:
	*		True if audio capture is supported
	*
	*/
	static bool isAvailable()
	{
		return cast(bool) sfSoundRecorder_IsAvailable();
	}

protected:
	/**
	*	Protected constructor
	*/	
	this()
	{
		m_id = ++seed;
		m_instances[m_id] = this;
		super(sfSoundRecorder_Create(&internalOnStart, &internalCallback, &internalOnStop, &m_id));
		
		init(true);
	}

	this(SFMLClass ptr)
	{
		super(ptr);

		init(false);
	}

	/**
	*	Start recording audio data
	*
	*	Returns:
	*		False to abort recording audio data, true to start
	*/
	abstract bool onStart();
	
	/**
	*	Stop recording audio data
	*/
	abstract void onStop();
	
	/**
	*	callback function
	*	
	*	Parameters:
	*		samples = Array of samples
	*		
	*	Returns:
	*		true to continue recording, false to stop.			
	*/		
	abstract bool onProcessSamples(short[] samples);

	bool m_disposed;  
private:
	/*
	*	an init function to initialize id of the object.
	*/	
	void init(bool flag)
	{
		if (flag)
		{
			m_list = new LinkedList!(Samples)();
			
			m_flag = true;
			m_continue = true;
			
			m_mutex = new Mutex();
		}
	}
		
	void* m_userData;
	int m_id;
	
	static int seed = 0;
	static SoundRecorder[int] m_instances;
	
	/*
	*	Extern C callback function
	*			
	*	This function must be static for C interop. To retrieve the current
	*	instance, we retrieve id of the sender in the user data, and search associated instance
	*	in the associative array.
	*	
	*	We don't call delegate or derived class on that thread because GC is not aware of this thread
	*	instead we enqueue data informations in a queue and poll this queue with a managed thread.								  
	*/
	extern(C) static int internalCallback(short* s, size_t size, void* user)
	{
		int id;
		// retrieve instance
		if ((id = *cast(int*)(user)) in m_instances)
		{
			SoundRecorder temp = m_instances[id];
			scope Lock l = new Lock(temp.m_mutex);
			if (temp.m_continue)
				// this new is allowed because Samples is an custom alloc class.
				temp.m_list.enqueue(new Samples(s, size));
			return temp.m_continue;
		}
		return false; 
	}

	extern(C) static int internalOnStart(void* user)
	{
		int id;
		bool ret = false;
		if ((id = *cast(int*)(user)) in m_instances)
		{
			SoundRecorder temp = m_instances[id];
			ret = temp.onStart();
		}
	
		return ret;
	}

	extern(C) static void internalOnStop(void* user)
	{
		// Nothing to do
	}
	
	/*
	*	Managed thread loop
	*/	
	void threadPoll()
	{
		while (m_flag)
		{
			Thread.sleep(50_000_0); // 50ms
			// if samples are available
			if (!m_list.empty)
			{
				// Lock ressources
				scope Lock l = new Lock(m_mutex);
				
				Samples s = m_list.dequeue;
				m_continue = this.onProcessSamples(s.data[0..s.length].dup);
				
				delete s;
				
				if (!m_continue)
				{
					// delete all samples left
					foreach(Samples dummy; m_list)
						delete dummy;
					break;
				}
			}
		}
		
		onStop();
	}
	
	Mutex m_mutex;
	
	bool m_flag;
	bool m_continue = true;
	LinkedList!(Samples) m_list;
	Thread m_t;
	
// External ====================================================================

	static extern (C)
	{
		SFMLClass	function(int function(void*), int function(const(short)*, size_t, void*), void function(void*), void*)	sfSoundRecorder_Create;
		void	function(SFMLClass)						sfSoundRecorder_Destroy;
		void	function(SFMLClass, uint SampleRate)	sfSoundRecorder_Start;
		void	function(SFMLClass)						sfSoundRecorder_Stop;
		uint	function(SFMLClass)						sfSoundRecorder_GetSampleRate;
		int		function()							sfSoundRecorder_IsAvailable;
	}

	mixin(loadFromSharedLib2("csfml-audio", "sfSoundRecorder", "Create", "Destroy", "Start",
							"Stop", "GetSampleRate", "IsAvailable"));
}

// Use explicit alloc to allow instaciation by C thread
private class Samples
{
	mixin Alloc;
	
	this(short* data, size_t length)
	{
		this.data = data;
		this.length = length;
	}

	public short* data;
	public size_t length;
}
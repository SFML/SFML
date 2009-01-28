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

module dsfml.audio.soundstream;

import dsfml.system.alloc;
import dsfml.system.common;
import dsfml.system.vector3;
import dsfml.system.linkedlist;
import dsfml.system.lock;
import dsfml.system.mutex;
import dsfml.system.sleep;
import dsfml.system.thread;

import dsfml.audio.sound;
import dsfml.audio.soundstatus;

/**
*   SoundStream is a streamed sound, ie samples are acquired
*   while the sound is playing. Use it for big sounds that would
*   require hundreds of MB in memory, or for streaming sound from the network.
*   
*   SoundStream is a base class and cannot be instanciated directly.
*   
*   $(B onGetData override will be called by a different thread, take care of synchronization issues.) onStart is called by the thread which called .play().
*   
*   ------------------------
*   class MySoundStream : SoundStream
*   {
*       this()
*       {
*           super(2, 44100); // you need to initialize the base class before any operation.   
*       }
*       protected bool onGetData(out short[] data)
*       {
*           //You need to fill data array with some samples
*           
*           return true; //or false if you want to stop playback
*       }
*       
*       protected bool onStart()
*       {
*           return true;   
*       }  
*   }
*   ------------------------
*/
abstract class SoundStream : DSFMLObject
{
    override void dispose()
    {
        stop();
        sfSoundStream_Destroy(m_ptr);
        s_instances.remove(m_id);
    }
    
    /**
    *   Start playing the stream
    */        
    void play()
    {
        m_flag = true;           
        sfSoundStream_Play(m_ptr);
        
        if (getStatus() != SoundStatus.PAUSED)
        {
            m_t = new Thread(&threadPoll);
            m_t.launch();
        }
    }
    
    /**
    *   Pause the stream
    */        
    void pause()
    {
        sfSoundStream_Pause(m_ptr);
    }

    /**
    *   Stop the stream
    */        
    void stop()
    {
        m_flag = false;
        sfSoundStream_Stop(m_ptr);
        m_t.wait();
        if (m_dummy !is null)
            delete m_dummy;
    }
    
    /**
    *   Get number of channels of the stream
    *   
    *   Returns:
    *       number of channels            
    */        
    uint getChannelsCount()
    {
        return m_channelsCount;
    }

    /**
    *   Get the sample rate of the stream
    *   
    *   Returns:
    *       sample rate            
    */        
    uint getSampleRate()
    {
        return m_sampleRate;
    }

    /**
    *   Get the current status of the stream
    *   
    *   Returns:
    *       Current stream status            
    */        
    SoundStatus getStatus()
    {
        return sfSoundStream_GetStatus(m_ptr);
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
        sfSoundStream_SetPitch(m_ptr, pitch);
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
        assert(volume >= 0.f && volume <= 100.f);
    }
    body
    {
        sfSoundStream_SetVolume(m_ptr, volume);
    }

    /*
    *   Set the sound position (take 3 values).
    *   The default position is (0, 0, 0)
    *
    *   Params:
    *      x, y, z = Position of the sound in the world
    */

    void setPosition(float x, float y, float z)
    {
        sfSoundStream_SetPosition(m_ptr, x, y, z);
    }

    /**
    *   Set the sound position (take 3 values).
    *   The default position is (0, 0, 0)
    *
    *   Params:
    *     vec = Position of the sound in the world
    *
    */

    void setPosition(Vector3f vec)
    {
        sfSoundStream_SetPosition(m_ptr, vec.x, vec.y, vec.z);
    }

    /**
    *   Set the minimum distance - closer than this distance,
    *   the listener will hear the sound at its maximum volume.
    *   The default minimum distance is 1.0
    *
    *   Params:
    *       minDistance = New minimum distance for the sound
    */
    void setMinDistance(float minDistance)
    {
        sfSoundStream_SetMinDistance(m_ptr, minDistance);
    }

    /**
    *   Set the attenuation factor - the higher the attenuation, the
    *   more the sound will be attenuated with distance from listener.
    *   The default attenuation factor 1.0
    *
    *   Params: 
    *       attenuation = New attenuation factor for the sound
    *
    */
    void setAttenuation(float attenuation)
    {
        sfSoundStream_SetAttenuation(m_ptr, attenuation);
    }

    /**
    *   Get the pitch
    *
    *   Returns:
    *       Pitch value
    */

    float getPitch()
    {
        return sfSoundStream_GetPitch(m_ptr);
    }
    /**
    *   Get the volume
    *
    *   Returns:
    *       Volume value (in range [1, 100])
    *
    */

    float getVolume()
    {
        return sfSoundStream_GetVolume(m_ptr);
    }

    /**
    *   Get the sound position
    *   
    *   Returns:
    *       Sound position                
    */        
    Vector3f getPosition()
    {
        Vector3f vec;
        sfSoundStream_GetPosition(m_ptr, &vec.x, &vec.y, &vec.z);
        return vec;
    }

    /**
    *   Get the minimum distance
    *   
    *   Returns:
    *       Get the minimum distance of the sound            
    */        
    float getMinDistance()
    {
        return sfSoundStream_GetMinDistance(m_ptr);
    }

    /**
    *   Get the attenuation
    *   
    *   Returns:
    *       Get the attenuation of the sound            
    */        
    float getAttenuation()
    {
        return sfSoundStream_GetAttenuation(m_ptr);
    }

    /**
    *   Get the current playing offset of the stream
    *   
    *   Returns:
    *       current playing offset, in seconds.            
    */        
    float getPlayingOffset()
    {
        return sfSoundStream_GetPlayingOffset(m_ptr);
    }

    /**
    *   Tell whether or not the stream is looping
    *
    *   Returns:
    *       True if the music is looping, false otherwise
    */
    bool getLoop()
    {
        if (m_ptr !is null)
            return cast(bool)sfSoundStream_GetLoop(m_ptr);
        return false;
    }

    /**
    *   Set the stream loop state.
    *   
    *   Disabled by default.
    *   
    *   Params:
    *       loop = true to play in loop, false to play once                    
    */        
    void setLoop(bool loop)
    {
        if (m_ptr !is null)
            sfSoundStream_SetLoop(m_ptr, loop);
    }
protected:
    /**
    *   Protected constructor
    *       
    *   Params:
    *       channelsCount = number of channel
    *       sampleRate = sample rate of the stream
    *
    */        
    this(uint channelsCount, uint sampleRate)
    {
        m_channelsCount = channelsCount;
        m_sampleRate = sampleRate;
        super(sfSoundStream_Create(&externalOnStart, &externalOnGetData, channelsCount, sampleRate, &m_id));

        m_mutex = new Mutex();
        
        m_samples = new LinkedList!(Data);
        
        m_t = new Thread(&this.threadPoll);
        
        m_id = ++s_seed;
        s_instances[m_id] = this;
    }
    
    /**
    *   Called each time the stream restart
    *   
    *   Returns:
    *       false to abort the playback            
    */        
    abstract bool onStart();
    
    /**
    *   Called each time the stream needs new data.
    *   This method will be call by an other thread, take care of possible synchronisation issues.
    *   
    *   Params:
    *       data = array of samples to stream
    *   
    *   Returns:
    *       true to continue streaming, false to stop                        
    */        
    abstract bool onGetData(out short[] data);
private:

    // Called sync when user calling play()
    extern(C) static int externalOnStart(void* user)
    {
        int id;
        if ((id = *cast(int*) user) in s_instances)
        {
            SoundStream temp = s_instances[id];
            return (temp.m_flag = temp.onStart());
        }
        return true;
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
                sleep(0.01f);
            
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
    void threadPoll(void* dummy)
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
            sleep(0.1f);
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
        
        alias int function(void*) sfSoundStreamStartCallback;
    	alias int function (sfSoundStreamChunk*, void*) sfSoundStreamGetDataCallback;
    	
    	typedef void* function(sfSoundStreamStartCallback, sfSoundStreamGetDataCallback, uint, uint, void*) pf_sfSoundStream_Create;
    	typedef void function(void*) pf_sfSoundStream_Destroy;
    	typedef void function(void*) pf_sfSoundStream_Play;
    	typedef void function(void*) pf_sfSoundStream_Pause;
    	typedef void function(void*) pf_sfSoundStream_Stop;
    	typedef SoundStatus function(void*) pf_sfSoundStream_GetStatus;       
        typedef uint function(void*) pf_sfSoundStream_GetChannelsCount;
        typedef uint function(void*) pf_sfSoundStream_GetSampleRate;
        typedef void function(void*, float) pf_sfSoundStream_SetPitch;
        typedef void function(void*, float) pf_sfSoundStream_SetVolume;
        typedef void function(void*, float, float, float) pf_sfSoundStream_SetPosition;        
        typedef void function(void*, float) pf_sfSoundStream_SetMinDistance;
        typedef void function(void*, float) pf_sfSoundStream_SetAttenuation;
        typedef float function(void*) pf_sfSoundStream_GetPitch;
        typedef float function(void*) pf_sfSoundStream_GetVolume;
        typedef void function(void*, float*, float*, float*) pf_sfSoundStream_GetPosition;
        typedef float function(void*) pf_sfSoundStream_GetMinDistance;
        typedef float function(void*) pf_sfSoundStream_GetAttenuation;
        typedef float function(void*) pf_sfSoundStream_GetPlayingOffset;
        typedef int function(void*) pf_sfSoundStream_GetLoop;
        typedef void function(void*, int) pf_sfSoundStream_SetLoop;

        
        static pf_sfSoundStream_Create sfSoundStream_Create;
    	static pf_sfSoundStream_Destroy sfSoundStream_Destroy;
        static pf_sfSoundStream_Play sfSoundStream_Play;
        static pf_sfSoundStream_Pause sfSoundStream_Pause;
        static pf_sfSoundStream_Stop sfSoundStream_Stop;
        static pf_sfSoundStream_GetStatus sfSoundStream_GetStatus;   
        static pf_sfSoundStream_GetChannelsCount sfSoundStream_GetChannelsCount;
        static pf_sfSoundStream_GetSampleRate sfSoundStream_GetSampleRate;
        static pf_sfSoundStream_SetPitch sfSoundStream_SetPitch;
        static pf_sfSoundStream_SetVolume sfSoundStream_SetVolume;
        static pf_sfSoundStream_SetPosition sfSoundStream_SetPosition;    
        static pf_sfSoundStream_SetMinDistance sfSoundStream_SetMinDistance;
        static pf_sfSoundStream_SetAttenuation sfSoundStream_SetAttenuation;
        static pf_sfSoundStream_GetPitch sfSoundStream_GetPitch;
        static pf_sfSoundStream_GetVolume sfSoundStream_GetVolume;
        static pf_sfSoundStream_GetPosition sfSoundStream_GetPosition;
        static pf_sfSoundStream_GetMinDistance sfSoundStream_GetMinDistance;
        static pf_sfSoundStream_GetAttenuation sfSoundStream_GetAttenuation;
        static pf_sfSoundStream_GetPlayingOffset sfSoundStream_GetPlayingOffset;
        static pf_sfSoundStream_GetLoop sfSoundStream_GetLoop;
        static pf_sfSoundStream_SetLoop sfSoundStream_SetLoop;
    }

    static this()
    {
        DllLoader dll = DllLoader.load("csfml-audio");
        
        sfSoundStream_Create = cast(pf_sfSoundStream_Create)dll.getSymbol("sfSoundStream_Create");
        sfSoundStream_Destroy = cast(pf_sfSoundStream_Destroy)dll.getSymbol("sfSoundStream_Destroy");
        sfSoundStream_Play = cast(pf_sfSoundStream_Play)dll.getSymbol("sfSoundStream_Play");
        sfSoundStream_Pause = cast(pf_sfSoundStream_Pause)dll.getSymbol("sfSoundStream_Pause");
        sfSoundStream_Stop = cast(pf_sfSoundStream_Stop)dll.getSymbol("sfSoundStream_Stop");
        sfSoundStream_GetStatus = cast(pf_sfSoundStream_GetStatus)dll.getSymbol("sfSoundStream_GetStatus");
        sfSoundStream_GetChannelsCount = cast(pf_sfSoundStream_GetChannelsCount)dll.getSymbol("sfSoundStream_GetChannelsCount");
        sfSoundStream_GetSampleRate = cast(pf_sfSoundStream_GetSampleRate)dll.getSymbol("sfSoundStream_GetSampleRate");
        sfSoundStream_SetPitch = cast(pf_sfSoundStream_SetPitch)dll.getSymbol("sfSoundStream_SetPitch");
        sfSoundStream_SetVolume = cast(pf_sfSoundStream_SetVolume)dll.getSymbol("sfSoundStream_SetVolume");
        sfSoundStream_SetPosition = cast(pf_sfSoundStream_SetPosition)dll.getSymbol("sfSoundStream_SetPosition");
        sfSoundStream_SetMinDistance = cast(pf_sfSoundStream_SetMinDistance)dll.getSymbol("sfSoundStream_SetMinDistance");
        sfSoundStream_SetAttenuation = cast(pf_sfSoundStream_SetAttenuation)dll.getSymbol("sfSoundStream_SetAttenuation");
        sfSoundStream_GetPitch = cast(pf_sfSoundStream_GetPitch)dll.getSymbol("sfSoundStream_GetPitch");
        sfSoundStream_GetVolume = cast(pf_sfSoundStream_GetVolume)dll.getSymbol("sfSoundStream_GetVolume");
        sfSoundStream_GetPosition = cast(pf_sfSoundStream_GetPosition)dll.getSymbol("sfSoundStream_GetPosition");
        sfSoundStream_GetMinDistance = cast(pf_sfSoundStream_GetMinDistance)dll.getSymbol("sfSoundStream_GetMinDistance");
        sfSoundStream_GetAttenuation = cast(pf_sfSoundStream_GetAttenuation)dll.getSymbol("sfSoundStream_GetAttenuation");
        sfSoundStream_GetPlayingOffset = cast(pf_sfSoundStream_GetPlayingOffset)dll.getSymbol("sfSoundStream_GetPlayingOffset");
        sfSoundStream_GetLoop = cast(pf_sfSoundStream_GetLoop)dll.getSymbol("sfSoundStream_GetLoop");
        sfSoundStream_SetLoop = cast(pf_sfSoundStream_SetLoop)dll.getSymbol("sfSoundStream_SetLoop");
    }
}

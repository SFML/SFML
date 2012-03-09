////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundStream.hpp>
#include <SFML/Audio/AudioDevice.hpp>
#include <SFML/Audio/ALCheck.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Err.hpp>

#ifdef _MSC_VER
    #pragma warning(disable : 4355) // 'this' used in base member initializer list
#endif


namespace sf
{
////////////////////////////////////////////////////////////
SoundStream::SoundStream() :
m_thread          (&SoundStream::Stream, this),
m_isStreaming     (false),
m_channelCount    (0),
m_sampleRate      (0),
m_format          (0),
m_loop            (false),
m_samplesProcessed(0)
{

}


////////////////////////////////////////////////////////////
SoundStream::~SoundStream()
{
    // Stop the sound if it was playing
    Stop();
}


////////////////////////////////////////////////////////////
void SoundStream::Initialize(unsigned int channelCount, unsigned int sampleRate)
{
    m_channelCount = channelCount;
    m_sampleRate   = sampleRate;

    // Deduce the format from the number of channels
    m_format = priv::AudioDevice::GetFormatFromChannelCount(channelCount);

    // Check if the format is valid
    if (m_format == 0)
    {
        m_channelCount = 0;
        m_sampleRate   = 0;
        Err() << "Unsupported number of channels (" << m_channelCount << ")" << std::endl;
    }
}


////////////////////////////////////////////////////////////
void SoundStream::Play()
{
    // Check if the sound parameters have been set
    if (m_format == 0)
    {
        Err() << "Failed to play audio stream: sound parameters have not been initialized (call Initialize first)" << std::endl;
        return;
    }

    // If the sound is already playing (probably paused), just resume it
    if (m_isStreaming)
    {
        ALCheck(alSourcePlay(m_source));
        return;
    }

    // Move to the beginning
    OnSeek(Time::Zero);

    // Start updating the stream in a separate thread to avoid blocking the application
    m_samplesProcessed = 0;
    m_isStreaming = true;
    m_thread.Launch();
}


////////////////////////////////////////////////////////////
void SoundStream::Pause()
{
    ALCheck(alSourcePause(m_source));
}


////////////////////////////////////////////////////////////
void SoundStream::Stop()
{
    // Wait for the thread to terminate
    m_isStreaming = false;
    m_thread.Wait();
}


////////////////////////////////////////////////////////////
unsigned int SoundStream::GetChannelCount() const
{
    return m_channelCount;
}


////////////////////////////////////////////////////////////
unsigned int SoundStream::GetSampleRate() const
{
    return m_sampleRate;
}


////////////////////////////////////////////////////////////
SoundStream::Status SoundStream::GetStatus() const
{
    Status status = SoundSource::GetStatus();

    // To compensate for the lag between Play() and alSourcePlay()
    if ((status == Stopped) && m_isStreaming)
        status = Playing;

    return status;
}


////////////////////////////////////////////////////////////
void SoundStream::SetPlayingOffset(Time timeOffset)
{
    // Stop the stream
    Stop();

    // Let the derived class update the current position
    OnSeek(timeOffset);

    // Restart streaming
    m_samplesProcessed = static_cast<Uint64>(timeOffset.AsSeconds() * m_sampleRate * m_channelCount);
    m_isStreaming = true;
    m_thread.Launch();
}


////////////////////////////////////////////////////////////
Time SoundStream::GetPlayingOffset() const
{
    ALfloat seconds = 0.f;
    ALCheck(alGetSourcef(m_source, AL_SEC_OFFSET, &seconds));

    return Seconds(seconds + static_cast<float>(m_samplesProcessed) / m_sampleRate / m_channelCount);
}


////////////////////////////////////////////////////////////
void SoundStream::SetLoop(bool loop)
{
    m_loop = loop;
}


////////////////////////////////////////////////////////////
bool SoundStream::GetLoop() const
{
    return m_loop;
}


////////////////////////////////////////////////////////////
void SoundStream::Stream()
{
    // Create the buffers
    ALCheck(alGenBuffers(BufferCount, m_buffers));
    for (int i = 0; i < BufferCount; ++i)
        m_endBuffers[i] = false;

    // Fill the queue
    bool requestStop = FillQueue();

    // Play the sound
    ALCheck(alSourcePlay(m_source));

    while (m_isStreaming)
    {
        // The stream has been interrupted!
        if (SoundSource::GetStatus() == Stopped)
        {
            if (!requestStop)
            {
                // Just continue
                ALCheck(alSourcePlay(m_source));
            }
            else
            {
                // End streaming
                m_isStreaming = false;
            }
        }

        // Get the number of buffers that have been processed (ie. ready for reuse)
        ALint nbProcessed = 0;
        ALCheck(alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &nbProcessed));

        while (nbProcessed--)
        {
            // Pop the first unused buffer from the queue
            ALuint buffer;
            ALCheck(alSourceUnqueueBuffers(m_source, 1, &buffer));

            // Find its number
            unsigned int bufferNum = 0;
            for (int i = 0; i < BufferCount; ++i)
                if (m_buffers[i] == buffer)
                {
                    bufferNum = i;
                    break;
                }

            // Retrieve its size and add it to the samples count
            if (m_endBuffers[bufferNum])
            {
                // This was the last buffer: reset the sample count
                m_samplesProcessed = 0;
                m_endBuffers[bufferNum] = false;
            }
            else
            {
                ALint size, bits;
                ALCheck(alGetBufferi(buffer, AL_SIZE, &size));
                ALCheck(alGetBufferi(buffer, AL_BITS, &bits));
                m_samplesProcessed += size / (bits / 8);
            }

            // Fill it and push it back into the playing queue
            if (!requestStop)
            {
                if (FillAndPushBuffer(bufferNum))
                    requestStop = true;
            }
        }

        // Leave some time for the other threads if the stream is still playing
        if (SoundSource::GetStatus() != Stopped)
            Sleep(Milliseconds(10));
    }

    // Stop the playback
    ALCheck(alSourceStop(m_source));

    // Unqueue any buffer left in the queue
    ClearQueue();

    // Delete the buffers
    ALCheck(alSourcei(m_source, AL_BUFFER, 0));
    ALCheck(alDeleteBuffers(BufferCount, m_buffers));
}


////////////////////////////////////////////////////////////
bool SoundStream::FillAndPushBuffer(unsigned int bufferNum)
{
    bool requestStop = false;

    // Acquire audio data
    Chunk data = {NULL, 0};
    if (!OnGetData(data))
    {
        // Mark the buffer as the last one (so that we know when to reset the playing position)
        m_endBuffers[bufferNum] = true;

        // Check if the stream must loop or stop
        if (m_loop)
        {
            // Return to the beginning of the stream source
            OnSeek(Time::Zero);

            // If we previously had no data, try to fill the buffer once again
            if (!data.Samples || (data.SampleCount == 0))
            {
                return FillAndPushBuffer(bufferNum);
            }
        }
        else
        {
            // Not looping: request stop
            requestStop = true;
        }
    }

    // Fill the buffer if some data was returned
    if (data.Samples && data.SampleCount)
    {
        unsigned int buffer = m_buffers[bufferNum];

        // Fill the buffer
        ALsizei size = static_cast<ALsizei>(data.SampleCount) * sizeof(Int16);
        ALCheck(alBufferData(buffer, m_format, data.Samples, size, m_sampleRate));

        // Push it into the sound queue
        ALCheck(alSourceQueueBuffers(m_source, 1, &buffer));
    }

    return requestStop;
}


////////////////////////////////////////////////////////////
bool SoundStream::FillQueue()
{
    // Fill and enqueue all the available buffers
    bool requestStop = false;
    for (int i = 0; (i < BufferCount) && !requestStop; ++i)
    {
        if (FillAndPushBuffer(i))
            requestStop = true;
    }

    return requestStop;
}


////////////////////////////////////////////////////////////
void SoundStream::ClearQueue()
{
    // Get the number of buffers still in the queue
    ALint nbQueued;
    ALCheck(alGetSourcei(m_source, AL_BUFFERS_QUEUED, &nbQueued));

    // Unqueue them all
    ALuint buffer;
    for (ALint i = 0; i < nbQueued; ++i)
        ALCheck(alSourceUnqueueBuffers(m_source, 1, &buffer));
}

} // namespace sf

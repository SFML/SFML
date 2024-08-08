////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2016 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Audio/SoundFileReaderOpus.hpp>
#include <SFML/System/MemoryInputStream.hpp>
#include <SFML/System/Err.hpp>
#include <algorithm>
#include <cctype>
#include <cassert>
#include <climits>

namespace
{
    int read(void* data, unsigned char* ptr, int bytes)
    {
        sf::InputStream* stream = static_cast<sf::InputStream*>(data);
        return static_cast<int>(stream->read(ptr, bytes));
    }

    int seek(void* data, opus_int64 offset, int whence)
    {
        sf::InputStream* stream = static_cast<sf::InputStream*>(data);
        switch (whence)
        {
            case SEEK_SET:
                break;
            case SEEK_CUR:
                offset += stream->tell();
                break;
            case SEEK_END:
                offset = stream->getSize() - offset;
        }

        // Return value expected from libopusfile: 0 - Success and -1 - Failure
        return static_cast<int>(stream->seek(offset)) >= 0 ? 0 : -1;
    }

    opus_int64 tell(void* data)
    {
        sf::InputStream* stream = static_cast<sf::InputStream*>(data);
        return static_cast<opus_int64>(stream->tell());
    }

    const OpusFileCallbacks callbacks = {&read, &seek, &tell, NULL};
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
bool SoundFileReaderOpus::check(InputStream& stream)
{
    int error = 0;
    OggOpusFile* file = op_test_callbacks(&stream, &callbacks, NULL, 0, &error);
    if (error == 0)
    {
        op_free(file);
        return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
SoundFileReaderOpus::SoundFileReaderOpus() :
m_opus        (NULL),
m_channelCount(0)
{
}


////////////////////////////////////////////////////////////
SoundFileReaderOpus::~SoundFileReaderOpus()
{
    close();
}


////////////////////////////////////////////////////////////
bool SoundFileReaderOpus::open(InputStream& stream, Info& info)
{
    // Open the Opus stream
    int status = 0;
    m_opus = op_open_callbacks(&stream, &callbacks, NULL, 0, &status);
    if (status != 0)
    {
        err() << "Failed to open Opus file for reading" << std::endl;
        return false;
    }

    // Retrieve the music attributes
    const OpusHead* opusHead = op_head(m_opus, -1);
    info.channelCount = static_cast<unsigned int>(opusHead->channel_count);
    info.sampleCount = static_cast<std::size_t>(op_pcm_total(m_opus, -1) * opusHead->channel_count);

    // All Opus audio is encoded at 48kHz
    // https://opus-codec.org/docs/opusfile_api-0.5/structOpusHead.html#a73b80a913eca33d829f1667caee80d9e
    info.sampleRate = 48000;

    // We must keep the channel count for the seek function
    m_channelCount = info.channelCount;

    return true;
}


////////////////////////////////////////////////////////////
void SoundFileReaderOpus::seek(Uint64 sampleOffset)
{
    assert(m_opus != NULL);

    op_pcm_seek(m_opus, static_cast<ogg_int64_t>(sampleOffset / m_channelCount));
}


////////////////////////////////////////////////////////////
Uint64 SoundFileReaderOpus::read(Int16* samples, Uint64 maxCount)
{
    assert(m_opus != NULL);

    int samplesToRead;

    // Try to read the requested number of samples, stop only on error or end of file
    Uint64 count = 0;
    while (maxCount > 0)
    {
        // since maxCount is uint64 we have to ensure that samplesToRead is <= INT_MAX (int overflow)
        if (maxCount > INT_MAX)
        {
            samplesToRead = INT_MAX;
        }
        else
        {
            samplesToRead = static_cast<int>(maxCount);
        }

        // op_read returns number of SAMPLES read PER CHANNEL
        int samplesRead = op_read(m_opus, samples, samplesToRead, NULL) * static_cast<int>(m_channelCount);
        if (samplesRead > 0)
        {
            maxCount -= static_cast<sf::Uint64>(samplesRead);
            count    += static_cast<sf::Uint64>(samplesRead);
            samples  += samplesRead;
        }
        else
        {
            // error or end of file
            break;
        }
    }

    return count;
}


////////////////////////////////////////////////////////////
void SoundFileReaderOpus::close()
{
    if (m_opus != NULL)
    {
        op_free(m_opus);
        m_opus = NULL;
        m_channelCount = 0;
    }
}

} // namespace priv

} // namespace sf

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Audio/SoundFileReaderFlac.hpp>
#include <SFML/System/InputStream.hpp>
#include <SFML/System/Err.hpp>
#include <cassert>


namespace
{
    FLAC__StreamDecoderReadStatus streamRead(const FLAC__StreamDecoder*, FLAC__byte buffer[], std::size_t* bytes, void* clientData)
    {
        auto* data = static_cast<sf::priv::SoundFileReaderFlac::ClientData*>(clientData);

        sf::Int64 count = data->stream->read(buffer, static_cast<sf::Int64>(*bytes));
        if (count > 0)
        {
            *bytes = static_cast<std::size_t>(count);
            return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
        }
        else if (count == 0)
        {
            return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
        }
        else
        {
            return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
        }
    }

    FLAC__StreamDecoderSeekStatus streamSeek(const FLAC__StreamDecoder*, FLAC__uint64 absoluteByteOffset, void* clientData)
    {
        auto* data = static_cast<sf::priv::SoundFileReaderFlac::ClientData*>(clientData);

        sf::Int64 position = data->stream->seek(static_cast<sf::Int64>(absoluteByteOffset));
        if (position >= 0)
            return FLAC__STREAM_DECODER_SEEK_STATUS_OK;
        else
            return FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;
    }

    FLAC__StreamDecoderTellStatus streamTell(const FLAC__StreamDecoder*, FLAC__uint64* absoluteByteOffset, void* clientData)
    {
        auto* data = static_cast<sf::priv::SoundFileReaderFlac::ClientData*>(clientData);

        sf::Int64 position = data->stream->tell();
        if (position >= 0)
        {
            *absoluteByteOffset = static_cast<FLAC__uint64>(position);
            return FLAC__STREAM_DECODER_TELL_STATUS_OK;
        }
        else
        {
            return FLAC__STREAM_DECODER_TELL_STATUS_ERROR;
        }
    }

    FLAC__StreamDecoderLengthStatus streamLength(const FLAC__StreamDecoder*, FLAC__uint64* streamLength, void* clientData)
    {
        auto* data = static_cast<sf::priv::SoundFileReaderFlac::ClientData*>(clientData);

        sf::Int64 count = data->stream->getSize();
        if (count >= 0)
        {
            *streamLength = static_cast<FLAC__uint64>(count);
            return FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
        }
        else
        {
            return FLAC__STREAM_DECODER_LENGTH_STATUS_ERROR;
        }
    }

    FLAC__bool streamEof(const FLAC__StreamDecoder*, void* clientData)
    {
        auto* data = static_cast<sf::priv::SoundFileReaderFlac::ClientData*>(clientData);

        return data->stream->tell() == data->stream->getSize();
    }

    FLAC__StreamDecoderWriteStatus streamWrite(const FLAC__StreamDecoder*, const FLAC__Frame* frame, const FLAC__int32* const buffer[], void* clientData)
    {
        auto* data = static_cast<sf::priv::SoundFileReaderFlac::ClientData*>(clientData);

        // Reserve memory if we're going to use the leftovers buffer
        unsigned int frameSamples = frame->header.blocksize * frame->header.channels;
        if (data->remaining < frameSamples)
            data->leftovers.reserve(static_cast<std::size_t>(frameSamples - data->remaining));

        // Decode the samples
        for (unsigned i = 0; i < frame->header.blocksize; ++i)
        {
            for (unsigned int j = 0; j < frame->header.channels; ++j)
            {
                // Decode the current sample
                sf::Int16 sample = 0;
                switch (frame->header.bits_per_sample)
                {
                    case 8:
                        sample = static_cast<sf::Int16>(buffer[j][i] << 8);
                        break;
                    case 16:
                        sample = static_cast<sf::Int16>(buffer[j][i]);
                        break;
                    case 24:
                        sample = static_cast<sf::Int16>(buffer[j][i] >> 8);
                        break;
                    case 32:
                        sample = static_cast<sf::Int16>(buffer[j][i] >> 16);
                        break;
                    default:
                        assert(false);
                        break;
                }

                if (data->buffer && data->remaining > 0)
                {
                    // If there's room in the output buffer, copy the sample there
                    *data->buffer++ = sample;
                    data->remaining--;
                }
                else
                {
                    // We are either seeking (null buffer) or have decoded all the requested samples during a
                    // normal read (0 remaining), so we put the sample in a temporary buffer until next call
                    data->leftovers.push_back(sample);
                }
            }
        }

        return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
    }

    void streamMetadata(const FLAC__StreamDecoder*, const FLAC__StreamMetadata* meta, void* clientData)
    {
        auto* data = static_cast<sf::priv::SoundFileReaderFlac::ClientData*>(clientData);

        if (meta->type == FLAC__METADATA_TYPE_STREAMINFO)
        {
            data->info.sampleCount = meta->data.stream_info.total_samples * meta->data.stream_info.channels;
            data->info.sampleRate = meta->data.stream_info.sample_rate;
            data->info.channelCount = meta->data.stream_info.channels;
        }
    }

    void streamError(const FLAC__StreamDecoder*, FLAC__StreamDecoderErrorStatus, void* clientData)
    {
        auto* data = static_cast<sf::priv::SoundFileReaderFlac::ClientData*>(clientData);
        data->error = true;
    }
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
bool SoundFileReaderFlac::check(InputStream& stream)
{
    // Create a decoder
    FLAC__StreamDecoder* decoder = FLAC__stream_decoder_new();
    if (!decoder)
        return false;

    // Initialize the decoder with our callbacks
    ClientData data;
    data.stream = &stream;
    data.error = false;
    FLAC__stream_decoder_init_stream(decoder, &streamRead, &streamSeek, &streamTell, &streamLength, &streamEof, &streamWrite, nullptr, &streamError, &data);

    // Read the header
    bool valid = FLAC__stream_decoder_process_until_end_of_metadata(decoder) != 0;

    // Destroy the decoder
    FLAC__stream_decoder_finish(decoder);
    FLAC__stream_decoder_delete(decoder);

    return valid && !data.error;
}


////////////////////////////////////////////////////////////
SoundFileReaderFlac::SoundFileReaderFlac() :
m_decoder(nullptr),
m_clientData()
{
}


////////////////////////////////////////////////////////////
SoundFileReaderFlac::~SoundFileReaderFlac()
{
    close();
}


////////////////////////////////////////////////////////////
bool SoundFileReaderFlac::open(InputStream& stream, Info& info)
{
    // Create the decoder
    m_decoder = FLAC__stream_decoder_new();
    if (!m_decoder)
    {
        err() << "Failed to open FLAC file (failed to allocate the decoder)" << std::endl;
        return false;
    }

    // Initialize the decoder with our callbacks
    m_clientData.stream = &stream;
    FLAC__stream_decoder_init_stream(m_decoder, &streamRead, &streamSeek, &streamTell, &streamLength, &streamEof, &streamWrite, &streamMetadata, &streamError, &m_clientData);

    // Read the header
    if (!FLAC__stream_decoder_process_until_end_of_metadata(m_decoder))
    {
        close();
        err() << "Failed to open FLAC file (failed to read metadata)" << std::endl;
        return false;
    }

    // Retrieve the sound properties
    info = m_clientData.info; // was filled in the "metadata" callback

    return true;
}


////////////////////////////////////////////////////////////
void SoundFileReaderFlac::seek(Uint64 sampleOffset)
{
    assert(m_decoder);

    // Reset the callback data (the "write" callback will be called)
    m_clientData.buffer = nullptr;
    m_clientData.remaining = 0;
    m_clientData.leftovers.clear();

    // FLAC decoder expects absolute sample offset, so we take the channel count out
    if (sampleOffset < m_clientData.info.sampleCount)
    {
        // The "write" callback will populate the leftovers buffer with the first batch of samples from the
        // seek destination, and since we want that data in this typical case, we don't re-clear it afterward
        FLAC__stream_decoder_seek_absolute(m_decoder, sampleOffset / m_clientData.info.channelCount);
    }
    else
    {
        // FLAC decoder can't skip straight to EOF, so we short-seek by one sample and skip the rest
        FLAC__stream_decoder_seek_absolute(m_decoder, (m_clientData.info.sampleCount / m_clientData.info.channelCount) - 1);
        FLAC__stream_decoder_skip_single_frame(m_decoder);

        // This was re-populated during the seek, but we're skipping everything in this, so we need it emptied
        m_clientData.leftovers.clear();
    }
}


////////////////////////////////////////////////////////////
Uint64 SoundFileReaderFlac::read(Int16* samples, Uint64 maxCount)
{
    assert(m_decoder);

    // If there are leftovers from previous call, use it first
    std::size_t left = m_clientData.leftovers.size();
    if (left > 0)
    {
        if (left > maxCount)
        {
            // There are more leftovers than needed
            std::copy(m_clientData.leftovers.begin(), m_clientData.leftovers.begin() + static_cast<std::vector<Int16>::difference_type>(maxCount), samples);
            std::vector<Int16> leftovers(m_clientData.leftovers.begin() + static_cast<std::vector<Int16>::difference_type>(maxCount), m_clientData.leftovers.end());
            m_clientData.leftovers.swap(leftovers);
            return maxCount;
        }
        else
        {
            // We can use all the leftovers and decode new frames
            std::copy(m_clientData.leftovers.begin(), m_clientData.leftovers.end(), samples);
        }
    }

    // Reset the data that will be used in the callback
    m_clientData.buffer = samples + left;
    m_clientData.remaining = maxCount - left;
    m_clientData.leftovers.clear();

    // Decode frames one by one until we reach the requested sample count, the end of file or an error
    while (m_clientData.remaining > 0)
    {
        // Everything happens in the "write" callback
        // This will break on any fatal error (does not include EOF)
        if (!FLAC__stream_decoder_process_single(m_decoder))
            break;

        // Break on EOF
        if (FLAC__stream_decoder_get_state(m_decoder) == FLAC__STREAM_DECODER_END_OF_STREAM)
            break;
    }

    return maxCount - m_clientData.remaining;
}


////////////////////////////////////////////////////////////
void SoundFileReaderFlac::close()
{
    if (m_decoder)
    {
        FLAC__stream_decoder_finish(m_decoder);
        FLAC__stream_decoder_delete(m_decoder);
        m_decoder = nullptr;
    }
}

} // namespace priv

} // namespace sf

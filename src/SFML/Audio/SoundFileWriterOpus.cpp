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
#include <SFML/Audio/SoundFileWriterOpus.hpp>
#include <SFML/System/Err.hpp>
#include <algorithm>
#include <cassert>
#include <vector>

// Anonymous namespace
namespace
{
    // Make sure to write int into buffer little endian
    void writeUint32(std::vector<unsigned char>& buffer, const sf::Uint32 value)
    {
        buffer.push_back(static_cast<unsigned char>(value & 0x000000FF));
        buffer.push_back(static_cast<unsigned char>((value & 0x0000FF00) >> 8));
        buffer.push_back(static_cast<unsigned char>((value & 0x00FF0000) >> 16));
        buffer.push_back(static_cast<unsigned char>((value & 0xFF000000) >> 24));
    }
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
bool SoundFileWriterOpus::check(const std::string& filename)
{
    std::string extension = filename.substr(filename.find_last_of(".") + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    return extension == "opus";
}


////////////////////////////////////////////////////////////
SoundFileWriterOpus::SoundFileWriterOpus() :
m_channelCount (0),
m_file         (),
m_ogg          (),
m_opus         (NULL),
m_packageNumber(0)
{
}


////////////////////////////////////////////////////////////
SoundFileWriterOpus::~SoundFileWriterOpus()
{
    close();
}


////////////////////////////////////////////////////////////
bool SoundFileWriterOpus::open(const std::string& filename, unsigned int sampleRate, unsigned int channelCount)
{
    // Save the channel count
    m_channelCount = channelCount;
    m_sampleRate = sampleRate;

    // Initialize the ogg/opus stream
    if (ogg_stream_init(&m_ogg, std::rand()) == -1)
    {
        err() << "Stream init of ogg/opus file \"" << filename << "\" failed" << std::endl;
        close();
        return false;
    }

    int status = OPUS_OK;
    m_opus = opus_encoder_create(sampleRate, channelCount, OPUS_APPLICATION_AUDIO, &status);
    if (status != OPUS_OK)
    {
        err() << "Failed to write ogg/opus file \"" << filename << "\"" << std::endl;
        if (status == OPUS_BAD_ARG)
            err() << "Possibly wrong sample rate, allowed are 8000, 12000, 16000, 24000, or 48000 Hz." << std::endl;
        close();
        return false;
    }

    m_file.open(filename.c_str(), std::ios::binary);
    if (!m_file)
    {
        err() << "Failed to write ogg/opus file \"" << filename << "\" (cannot open file)" << std::endl;
        close();
        return false;
    }

    // Set bitrate (VBR is default)
    opus_encoder_ctl(m_opus, OPUS_SET_BITRATE(128000));

    // Create opus header MAGICBYTES VERSION
    std::string magicBytes("OpusHead1");
    std::vector<unsigned char> headerData(magicBytes.begin(), magicBytes.end());

    headerData.push_back(channelCount);
    writeUint32(headerData, static_cast<Uint32>(sampleRate));
    headerData.push_back(channelCount > 8 ? 255 : (channelCount > 2)); // Mapping family

    // Map opus header to ogg packet
    ogg_packet op;
    op.packet = &headerData.front(); // C++11: headerData.data();
    op.bytes = headerData.size();
    op.b_o_s = 1;
    op.e_o_s = 0;
    op.granulepos = 0;
    op.packetno = m_packageNumber++;

    // Write the header packet to the ogg stream
    ogg_stream_packetin(&m_ogg, &op);
    flushBlocks();

    // Create comment header, needs to be in a new page
    // commentData initialized with magic bytes
    magicBytes = "OpusTags";
    std::vector<unsigned char> commentData(magicBytes.begin(), magicBytes.end()); // C++11: commentData({'O', 'p', 'u', 's', 'T', 'a', 'g', 's'});

    // Vendor string
    const std::string opusVersion(opus_get_version_string());

    // unsigned 32bit integer: Length of vendor string (encoding library)
    writeUint32(commentData, opusVersion.size());
    commentData.insert(commentData.end(), opusVersion.begin(), opusVersion.end());

    // Length of user comments (E.g. you can add a ENCODER tag for SFML)
    writeUint32(commentData, 0);

    op.packet     = &commentData.front();
    op.bytes      = commentData.size();
    op.b_o_s      = 0;
    op.e_o_s      = 0;
    op.granulepos = 0;
    op.packetno   = m_packageNumber++;
    ogg_stream_packetin(&m_ogg, &op);

    // This ensures the actual audio data will start on a new page, as per spec
    flushBlocks();

    return true;
}


////////////////////////////////////////////////////////////
void SoundFileWriterOpus::write(const Int16* samples, Uint64 count)
{
    assert(m_opus);

    const opus_uint32 frame_size = 960;
    const opus_int32 buffer_size = frame_size * m_channelCount * sizeof(Int16);
    unsigned char* buffer = new unsigned char[buffer_size];

    Uint32 frame_number = 0;
    Uint8 endOfStream = 0;

    while (count > 0)
    {
        ogg_packet op;
        opus_int32 packet_size;

        // Check if wee need to pad the input
        if (count < frame_size * m_channelCount)
        {
            const Uint32 begin = (frame_number * frame_size * m_channelCount);
            std::vector<opus_int16> pad(samples + begin, samples + begin + count);
            pad.insert(pad.end(), (frame_size * m_channelCount) - pad.size(),0);
            packet_size = opus_encode(m_opus, &pad.front(), frame_size, buffer, buffer_size);
            endOfStream = 1;
            count = 0;
        }
        else
        {
            packet_size = opus_encode(m_opus, samples + (frame_number * frame_size * m_channelCount), frame_size, buffer, buffer_size);
            count -= frame_size * m_channelCount;
        }

        if (packet_size < 0)
        {
            err() << "An error occurred when encoding sound to opus." << std::endl;
            break;
        }
        op.packet     = buffer;
        op.bytes      = packet_size;
        op.granulepos = frame_number * frame_size * 48000ul / m_sampleRate;
        op.packetno   = m_packageNumber++;
        op.b_o_s      = 0;
        op.e_o_s      = endOfStream;
        ogg_stream_packetin(&m_ogg, &op);

        frame_number++;
    }

    // Flush any produced block
    flushBlocks();
    delete[] buffer;
}


////////////////////////////////////////////////////////////
void SoundFileWriterOpus::flushBlocks()
{
    ogg_page page;
    while ((ogg_stream_pageout(&m_ogg, &page) > 0) || (ogg_stream_flush(&m_ogg, &page) > 0))
    {
        m_file.write(reinterpret_cast<const char*>(page.header), page.header_len);
        m_file.write(reinterpret_cast<const char*>(page.body), page.body_len);
    }
}


////////////////////////////////////////////////////////////
void SoundFileWriterOpus::close()
{
    if (m_file.is_open())
    {
        flushBlocks();
        m_file.close();
    }

    // Clear all the ogg/opus structures
    ogg_stream_clear(&m_ogg);
    if (m_opus != NULL)
    {
        opus_encoder_destroy(m_opus);
        m_opus = NULL;
    }
}

} // namespace priv

} // namespace sf

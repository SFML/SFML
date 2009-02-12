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
#include <SFML/Audio/SoundFileOgg.hpp>
#include <SFML/Audio/stb_vorbis/stb_vorbis.h>
#include <iostream>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
SoundFileOgg::SoundFileOgg() :
myStream       (NULL),
myChannelsCount(0)
{

}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
SoundFileOgg::~SoundFileOgg()
{
    if (myStream)
        stb_vorbis_close(myStream);
}


////////////////////////////////////////////////////////////
/// Check if a given file is supported by this loader
////////////////////////////////////////////////////////////
bool SoundFileOgg::IsFileSupported(const std::string& Filename, bool Read)
{
    if (Read)
    {
        // Open the vorbis stream
        stb_vorbis* Stream = stb_vorbis_open_filename(const_cast<char*>(Filename.c_str()), NULL, NULL);

        if (Stream)
        {
            stb_vorbis_close(Stream);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        // No support for writing ogg files yet...
        return false;
    }
}


////////////////////////////////////////////////////////////
/// Check if a given file in memory is supported by this loader
////////////////////////////////////////////////////////////
bool SoundFileOgg::IsFileSupported(const char* Data, std::size_t SizeInBytes)
{
    // Open the vorbis stream
    unsigned char* Buffer = reinterpret_cast<unsigned char*>(const_cast<char*>(Data));
    int Length = static_cast<int>(SizeInBytes);
    stb_vorbis* Stream = stb_vorbis_open_memory(Buffer, Length, NULL, NULL);

    if (Stream)
    {
        stb_vorbis_close(Stream);
        return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
/// Open the sound file for reading
////////////////////////////////////////////////////////////
bool SoundFileOgg::OpenRead(const std::string& Filename, std::size_t& NbSamples, unsigned int& ChannelsCount, unsigned int& SampleRate)
{
    // Close the file if already opened
    if (myStream)
        stb_vorbis_close(myStream);

    // Open the vorbis stream
    myStream = stb_vorbis_open_filename(const_cast<char*>(Filename.c_str()), NULL, NULL);
    if (myStream == NULL)
    {
        std::cerr << "Failed to read sound file \"" << Filename << "\" (cannot open the file)" << std::endl;
        return false;
    }

    // Get the music parameters
    stb_vorbis_info Infos = stb_vorbis_get_info(myStream);
    ChannelsCount = myChannelsCount = Infos.channels;
    SampleRate    = Infos.sample_rate;
    NbSamples     = static_cast<std::size_t>(stb_vorbis_stream_length_in_samples(myStream) * ChannelsCount);

    return true;
}


////////////////////////////////////////////////////////////
/// /see sf::SoundFile::OpenRead
////////////////////////////////////////////////////////////
bool SoundFileOgg::OpenRead(const char* Data, std::size_t SizeInBytes, std::size_t& NbSamples, unsigned int& ChannelsCount, unsigned int& SampleRate)
{
    // Close the file if already opened
    if (myStream)
        stb_vorbis_close(myStream);

    // Open the vorbis stream
    unsigned char* Buffer = reinterpret_cast<unsigned char*>(const_cast<char*>(Data));
    int Length = static_cast<int>(SizeInBytes);
    myStream = stb_vorbis_open_memory(Buffer, Length, NULL, NULL);
    if (myStream == NULL)
    {
        std::cerr << "Failed to read sound file from memory (cannot open the file)" << std::endl;
        return false;
    }

    // Get the music parameters
    stb_vorbis_info Infos = stb_vorbis_get_info(myStream);
    ChannelsCount = myChannelsCount = Infos.channels;
    SampleRate    = Infos.sample_rate;
    NbSamples     = static_cast<std::size_t>(stb_vorbis_stream_length_in_samples(myStream) * ChannelsCount);

    return true;
}


////////////////////////////////////////////////////////////
/// Read samples from the loaded sound
////////////////////////////////////////////////////////////
std::size_t SoundFileOgg::Read(Int16* Data, std::size_t NbSamples)
{
    if (myStream && Data && NbSamples)
    {
        int Read = stb_vorbis_get_samples_short_interleaved(myStream, myChannelsCount, Data, static_cast<int>(NbSamples));
        return static_cast<std::size_t>(Read * myChannelsCount);
    }
    else
    {
        return 0;
    }
}

} // namespace priv

} // namespace sf

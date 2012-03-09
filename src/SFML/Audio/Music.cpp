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
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/ALCheck.hpp>
#include <SFML/Audio/SoundFile.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Err.hpp>
#include <fstream>


namespace sf
{
////////////////////////////////////////////////////////////
Music::Music() :
m_file    (new priv::SoundFile),
m_duration()
{

}


////////////////////////////////////////////////////////////
Music::~Music()
{
    // We must stop before destroying the file :)
    Stop();

    delete m_file;
}


////////////////////////////////////////////////////////////
bool Music::OpenFromFile(const std::string& filename)
{
    // First stop the music if it was already running
    Stop();

    // Open the underlying sound file
    if (!m_file->OpenRead(filename))
        return false;

    // Perform common initializations
    Initialize();

    return true;
}


////////////////////////////////////////////////////////////
bool Music::OpenFromMemory(const void* data, std::size_t sizeInBytes)
{
    // First stop the music if it was already running
    Stop();

    // Open the underlying sound file
    if (!m_file->OpenRead(data, sizeInBytes))
        return false;

    // Perform common initializations
    Initialize();

    return true;
}


////////////////////////////////////////////////////////////
bool Music::OpenFromStream(InputStream& stream)
{
    // First stop the music if it was already running
    Stop();

    // Open the underlying sound file
    if (!m_file->OpenRead(stream))
        return false;

    // Perform common initializations
    Initialize();

    return true;
}


////////////////////////////////////////////////////////////
Time Music::GetDuration() const
{
    return m_duration;
}


////////////////////////////////////////////////////////////
bool Music::OnGetData(SoundStream::Chunk& data)
{
    Lock lock(m_mutex);

    // Fill the chunk parameters
    data.Samples     = &m_samples[0];
    data.SampleCount = m_file->Read(&m_samples[0], m_samples.size());

    // Check if we have reached the end of the audio file
    return data.SampleCount == m_samples.size();
}


////////////////////////////////////////////////////////////
void Music::OnSeek(Time timeOffset)
{
    Lock lock(m_mutex);

    m_file->Seek(timeOffset);
}


////////////////////////////////////////////////////////////
void Music::Initialize()
{
    // Compute the music duration
    m_duration = Seconds(static_cast<float>(m_file->GetSampleCount()) / m_file->GetSampleRate() / m_file->GetChannelCount());

    // Resize the internal buffer so that it can contain 1 second of audio samples
    m_samples.resize(m_file->GetSampleRate() * m_file->GetChannelCount());

    // Initialize the stream
    SoundStream::Initialize(m_file->GetChannelCount(), m_file->GetSampleRate());
}

} // namespace sf

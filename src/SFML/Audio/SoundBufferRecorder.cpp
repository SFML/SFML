////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Audio/SoundBufferRecorder.hpp>

#include <SFML/System/Err.hpp>

#include <algorithm>
#include <iterator>
#include <ostream>


namespace sf
{
////////////////////////////////////////////////////////////
SoundBufferRecorder::~SoundBufferRecorder()
{
    // Make sure to stop the recording thread
    stop();
}


////////////////////////////////////////////////////////////
bool SoundBufferRecorder::onStart()
{
    m_samples.clear();
    m_buffer = SoundBuffer();

    return true;
}


////////////////////////////////////////////////////////////
bool SoundBufferRecorder::onProcessSamples(const std::int16_t* samples, std::size_t sampleCount)
{
    std::copy(samples, samples + sampleCount, std::back_inserter(m_samples));

    return true;
}


////////////////////////////////////////////////////////////
void SoundBufferRecorder::onStop()
{
    if (m_samples.empty())
        return;

    if (!m_buffer.loadFromSamples(m_samples.data(), m_samples.size(), getChannelCount(), getSampleRate()))
        err() << "Failed to stop capturing audio data" << std::endl;
}


////////////////////////////////////////////////////////////
const SoundBuffer& SoundBufferRecorder::getBuffer() const
{
    return m_buffer;
}

} // namespace sf

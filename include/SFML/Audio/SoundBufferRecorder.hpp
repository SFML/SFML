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

#ifndef SFML_SOUNDBUFFERRECORDER_HPP
#define SFML_SOUNDBUFFERRECORDER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/SoundRecorder.hpp>
#include <vector>


namespace sf
{
////////////////////////////////////////////////////////////
/// Specialized SoundRecorder which saves the captured
/// audio data into a sound buffer
////////////////////////////////////////////////////////////
class SFML_API SoundBufferRecorder : public SoundRecorder
{
public :

    ////////////////////////////////////////////////////////////
    /// Get the sound buffer containing the captured audio data
    ///
    /// \return Constant reference to the sound buffer
    ///
    ////////////////////////////////////////////////////////////
    const SoundBuffer& GetBuffer() const;

private :

    ////////////////////////////////////////////////////////////
    /// /see SoundBuffer::OnStart
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OnStart();

    ////////////////////////////////////////////////////////////
    /// /see SoundBuffer::OnProcessSamples
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OnProcessSamples(const Int16* Samples, std::size_t SamplesCount);

    ////////////////////////////////////////////////////////////
    /// /see SoundBuffer::OnStop
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnStop();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::vector<Int16> mySamples; ///< Temporary sample buffer to hold the recorded data
    SoundBuffer        myBuffer;  ///< Sound buffer that will contain the recorded data
};

} // namespace sf

#endif // SFML_SOUNDBUFFERRECORDER_HPP

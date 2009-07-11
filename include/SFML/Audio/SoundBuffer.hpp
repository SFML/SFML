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

#ifndef SFML_SOUNDBUFFER_HPP
#define SFML_SOUNDBUFFER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/System/Resource.hpp>
#include <string>
#include <vector>


namespace sf
{
////////////////////////////////////////////////////////////
/// SoundBuffer is the low-level for loading and manipulating
/// sound buffers
////////////////////////////////////////////////////////////
class SFML_API SoundBuffer : public Resource<SoundBuffer>
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    SoundBuffer();

    ////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param copy : Instance to copy
    ///
    ////////////////////////////////////////////////////////////
    SoundBuffer(const SoundBuffer& copy);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~SoundBuffer();

    ////////////////////////////////////////////////////////////
    /// Load the sound buffer from a file
    ///
    /// \param filename : Path of the sound file to load
    ///
    /// \return True if loading has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromFile(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// Load the sound buffer from a file in memory
    ///
    /// \param data :        Pointer to the file data in memory
    /// \param sizeInBytes : Size of the data to load, in bytes
    ///
    /// \return True if loading has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromMemory(const char* data, std::size_t sizeInBytes);

    ////////////////////////////////////////////////////////////
    /// Load the sound buffer from an array of samples - assumed format for
    /// samples is 16 bits signed integer
    ///
    /// \param samples :       Pointer to the samples in memory
    /// \param samplesCount :  Number of samples pointed by Samples
    /// \param channelsCount : Number of channels (1 = mono, 2 = stereo, ...)
    /// \param sampleRate :    Frequency (number of samples to play per second)
    ///
    /// \return True if loading has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromSamples(const Int16* samples, std::size_t samplesCount, unsigned int channelsCount, unsigned int sampleRate);

    ////////////////////////////////////////////////////////////
    /// Save the sound buffer to a file
    ///
    /// \param filename : Path of the sound file to write
    ///
    /// \return True if saving has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool SaveToFile(const std::string& filename) const;

    ////////////////////////////////////////////////////////////
    /// Return the sound samples
    ///
    /// \return Pointer to the array of sound samples, in 16 bits signed integer format
    ///
    ////////////////////////////////////////////////////////////
    const Int16* GetSamples() const;

    ////////////////////////////////////////////////////////////
    /// Return the samples count
    ///
    /// \return Number of samples
    ///
    ////////////////////////////////////////////////////////////
    std::size_t GetSamplesCount() const;

    ////////////////////////////////////////////////////////////
    /// Get the sample rate
    ///
    /// \return Sound frequency (number of samples per second)
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetSampleRate() const;

    ////////////////////////////////////////////////////////////
    /// Return the number of channels (1 = mono, 2 = stereo, ...)
    ///
    /// \return Number of channels
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetChannelsCount() const;

    ////////////////////////////////////////////////////////////
    /// Get the sound duration
    ///
    /// \return Sound duration, in seconds
    ///
    ////////////////////////////////////////////////////////////
    float GetDuration() const;

    ////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param other : Instance to assign
    ///
    /// \return Reference to the sound buffer
    ///
    ////////////////////////////////////////////////////////////
    SoundBuffer& operator =(const SoundBuffer& other);

private :

    friend class Sound;

    ////////////////////////////////////////////////////////////
    /// Update the internal buffer with the audio samples
    ///
    /// \param channelsCount : Number of channels
    /// \param sampleRate :    Sample rate
    ///
    /// \return True on success
    ///
    ////////////////////////////////////////////////////////////
    bool Update(unsigned int channelsCount, unsigned int sampleRate);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int       myBuffer;   ///< OpenAL buffer identifier
    std::vector<Int16> mySamples;  ///< Samples buffer
    float              myDuration; ///< Sound duration, in seconds
};

} // namespace sf


#endif // SFML_SOUNDBUFFER_HPP

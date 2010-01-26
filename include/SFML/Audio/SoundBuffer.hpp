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
#include <SFML/System/Resource.hpp>
#include <SFML/Audio/AudioResource.hpp>
#include <string>
#include <vector>
#include <set>


namespace sf
{
class Sound;

////////////////////////////////////////////////////////////
/// SoundBuffer is the low-level for loading and manipulating
/// sound buffers
////////////////////////////////////////////////////////////
class SFML_API SoundBuffer : public AudioResource, public Resource<SoundBuffer>
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
    /// \param Copy : Instance to copy
    ///
    ////////////////////////////////////////////////////////////
    SoundBuffer(const SoundBuffer& Copy);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~SoundBuffer();

    ////////////////////////////////////////////////////////////
    /// Load the sound buffer from a file
    ///
    /// \param Filename : Path of the sound file to load
    ///
    /// \return True if loading has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromFile(const std::string& Filename);

    ////////////////////////////////////////////////////////////
    /// Load the sound buffer from a file in memory
    ///
    /// \param Data :        Pointer to the file data in memory
    /// \param SizeInBytes : Size of the data to load, in bytes
    ///
    /// \return True if loading has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromMemory(const char* Data, std::size_t SizeInBytes);

    ////////////////////////////////////////////////////////////
    /// Load the sound buffer from an array of samples - assumed format for
    /// samples is 16 bits signed integer
    ///
    /// \param Samples :       Pointer to the samples in memory
    /// \param SamplesCount :  Number of samples pointed by Samples
    /// \param ChannelsCount : Number of channels (1 = mono, 2 = stereo, ...)
    /// \param SampleRate :    Frequency (number of samples to play per second)
    ///
    /// \return True if loading has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromSamples(const Int16* Samples, std::size_t SamplesCount, unsigned int ChannelsCount, unsigned int SampleRate);

    ////////////////////////////////////////////////////////////
    /// Save the sound buffer to a file
    ///
    /// \param Filename : Path of the sound file to write
    ///
    /// \return True if saving has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool SaveToFile(const std::string& Filename) const;

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
    /// \param Other : Instance to assign
    ///
    /// \return Reference to the sound buffer
    ///
    ////////////////////////////////////////////////////////////
    SoundBuffer& operator =(const SoundBuffer& Other);

private :

    friend class Sound;

    ////////////////////////////////////////////////////////////
    /// Update the internal buffer with the audio samples
    ///
    /// \param ChannelsCount : Number of channels
    /// \param SampleRate :    Sample rate
    ///
    /// \return True on success
    ///
    ////////////////////////////////////////////////////////////
    bool Update(unsigned int ChannelsCount, unsigned int SampleRate);

    ////////////////////////////////////////////////////////////
    /// Add a sound to the list of sounds that use this buffer
    ///
    /// \param Instance : Sound object to attach
    ///
    ////////////////////////////////////////////////////////////
    void AttachSound(Sound* Instance) const;

    ////////////////////////////////////////////////////////////
    /// Remove a sound from the list of sounds that use this buffer
    ///
    /// \param Instance : Sound object to detach
    ///
    ////////////////////////////////////////////////////////////
    void DetachSound(Sound* Instance) const;

    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    typedef std::set<Sound*> SoundList; ///< Set of unique sound instances

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int       myBuffer;   ///< OpenAL buffer identifier
    std::vector<Int16> mySamples;  ///< Samples buffer
    float              myDuration; ///< Sound duration, in seconds
    mutable SoundList  mySounds;   ///< List of sounds that are using this buffer
};

} // namespace sf


#endif // SFML_SOUNDBUFFER_HPP

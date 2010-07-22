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
#include <set>


namespace sf
{
class Sound;

////////////////////////////////////////////////////////////
/// \brief Storage for audio samples defining a sound
///
////////////////////////////////////////////////////////////
class SFML_API SoundBuffer : public Resource<SoundBuffer>
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    SoundBuffer();

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy Instance to copy
    ///
    ////////////////////////////////////////////////////////////
    SoundBuffer(const SoundBuffer& copy);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~SoundBuffer();

    ////////////////////////////////////////////////////////////
    /// \brief Load the sound buffer from a file
    ///
    /// Here is a complete list of all the supported audio formats:
    /// ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam,
    /// w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
    ///
    /// \param filename Path of the sound file to load
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see LoadFromMemory, LoadFromSamples, SaveToFile
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromFile(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Load the sound buffer from a file in memory
    ///
    /// Here is a complete list of all the supported audio formats:
    /// ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam,
    /// w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
    ///
    /// \param data        Pointer to the file data in memory
    /// \param sizeInBytes Size of the data to load, in bytes
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see LoadFromFile, LoadFromSamples, SaveToFile
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromMemory(const void* data, std::size_t sizeInBytes);

    ////////////////////////////////////////////////////////////
    /// \brief Load the sound buffer from an array of audio samples
    ///
    /// The assumed format of the audio samples is 16 bits signed integer
    /// (sf::Int16).
    ///
    /// \param samples       Pointer to the array of samples in memory
    /// \param samplesCount  Number of samples in the array
    /// \param channelsCount Number of channels (1 = mono, 2 = stereo, ...)
    /// \param sampleRate    Sample rate (number of samples to play per second)
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see LoadFromFile, LoadFromMemory, SaveToFile
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromSamples(const Int16* samples, std::size_t samplesCount, unsigned int channelsCount, unsigned int sampleRate);

    ////////////////////////////////////////////////////////////
    /// \brief Save the sound buffer to an audio file
    ///
    /// Here is a complete list of all the supported audio formats:
    /// ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam,
    /// w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
    ///
    /// \param filename Path of the sound file to write
    ///
    /// \return True if saving succeeded, false if it failed
    ///
    /// \see LoadFromFile, LoadFromMemory, LoadFromSamples
    ///
    ////////////////////////////////////////////////////////////
    bool SaveToFile(const std::string& filename) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the array of audio samples stored in the buffer
    ///
    /// The format of the returned samples is 16 bits signed integer
    /// (sf::Int16). The total number of samples in this array
    /// is given by the GetSamplesCount() function.
    ///
    /// \return Read-only pointer to the array of sound samples
    ///
    /// \see GetSamplesCount
    ///
    ////////////////////////////////////////////////////////////
    const Int16* GetSamples() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of samples stored in the buffer
    ///
    /// The array of samples can be accessed with the GetSamples()
    /// function.
    ///
    /// \return Number of samples
    ///
    /// \see GetSamples
    ///
    ////////////////////////////////////////////////////////////
    std::size_t GetSamplesCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the sample rate of the sound
    ///
    /// The sample rate is the number of samples played per second.
    /// The higher, the better the quality (for example, 44100
    /// samples/s is CD quality).
    ///
    /// \return Sample rate (number of samples per second)
    ///
    /// \see GetChannelsCount, GetDuration
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetSampleRate() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of channels used by the sound
    ///
    /// If the sound is mono then the number ofchannels will
    /// be 1, 2 for stereo, etc.
    ///
    /// \return Number of channels
    ///
    /// \see GetSampleRate, GetDuration
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetChannelsCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the total duration of the sound
    ///
    /// \return Sound duration, in seconds
    ///
    /// \see GetSampleRate, GetChannelsCount
    ///
    ////////////////////////////////////////////////////////////
    float GetDuration() const;

    ////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    SoundBuffer& operator =(const SoundBuffer& right);

private :

    friend class Sound;

    ////////////////////////////////////////////////////////////
    /// \brief Update the internal buffer with the cached audio samples
    ///
    /// \param channelsCount Number of channels
    /// \param sampleRate    Sample rate (number of samples per second)
    ///
    /// \return True on success, false if any error happened
    ///
    ////////////////////////////////////////////////////////////
    bool Update(unsigned int channelsCount, unsigned int sampleRate);

    ////////////////////////////////////////////////////////////
    /// \brief Add a sound to the list of sounds that use this buffer
    ///
    /// \param sound Sound instance to attach
    ///
    ////////////////////////////////////////////////////////////
    void AttachSound(Sound* sound) const;

    ////////////////////////////////////////////////////////////
    /// \brief Remove a sound from the list of sounds that use this buffer
    ///
    /// \param sound Sound instance to detach
    ///
    ////////////////////////////////////////////////////////////
    void DetachSound(Sound* sound) const;

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


////////////////////////////////////////////////////////////
/// \class sf::SoundBuffer
/// \ingroup audio
///
/// A sound buffer holds the data of a sound, which is
/// an array of audio samples. A sample is a 16 bits signed integer
/// that defines the amplitude of the sound at a given time.
/// The sound is then restituted by playing these samples at
/// a high rate (for example, 44100 samples per second is the
/// standard rate used for playing CDs). In short, audio samples
/// are like image pixels, and a sf::SoundBuffer is similar to
/// a sf::Image.
///
/// A sound buffer can be loaded from a file (see LoadFromFile()
/// for the complete list of supported formats), from memory
/// or directly from an array of samples. It can also be saved
/// back to a file.
///
/// Sound buffers alone are not very useful: they hold the audio data
/// but cannot be played. To do so, you need to use the sf::Sound class,
/// which provides functions to play/pause/stop the sound as well as
/// changing the way it is outputted (volume, pitch, 3D position, ...).
/// This separation allows more flexibility and better performances:
/// indeed a sf::SoundBuffer is a heavy resource, and any operation on it
/// is slow (often too slow for real-time applications). On the other
/// side, a sf::Sound is a lightweight object, which can use the audio data
/// of a sound buffer and change the way it is played without actually
/// modifying that data. Note that it is also possible to bind
/// several sf::Sound instances to the same sf::SoundBuffer.
///
/// It is important to note that the sf::Sound instance doesn't
/// copy the buffer that it uses, it only keeps a reference to it.
/// Thus, a sf::SoundBuffer must not be destructed while it is
/// used by a sf::Sound (i.e. never write a function that
/// uses a local sf::SoundBuffer instance for loading a sound).
///
/// Usage example:
/// \code
/// // Declare a new sound buffer
/// sf::SoundBuffer buffer;
/// 
/// // Load it from a file
/// if (!buffer.LoadFromFile("sound.wav"))
/// {
///     // error...
/// }
/// 
/// // Create a sound source and bind it to the buffer
/// sf::Sound sound1;
/// sound1.SetBuffer(buffer);
/// 
/// // Play the sound
/// sound1.Play();
/// 
/// // Create another sound source bound to the same buffer
/// sf::Sound sound2;
/// sound2.SetBuffer(buffer);
///
/// // Play it with a higher pitch -- the first sound remains unchanged
/// sound2.SetPitch(2);
/// sound2.Play();
/// \endcode
///
/// \see sf::Sound, sf::SoundBufferRecorder
///
////////////////////////////////////////////////////////////

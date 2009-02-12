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

#ifndef SFML_SOUNDFILE_HPP
#define SFML_SOUNDFILE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/NonCopyable.hpp>
#include <string>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// SoundFile is the abstract base class for loading
/// and saving different sound file formats
////////////////////////////////////////////////////////////
class SoundFile : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// Create a new sound from a file, for reading
    ///
    /// \param Filename :      Path of sound file
    /// \param NbSamples :     Number of samples in the file
    /// \param ChannelsCount : Number of channels in the loaded sound
    /// \param SampleRate :    Sample rate of the loaded sound
    ///
    /// \return Pointer to the new sound file (NULL if failed)
    ///
    ////////////////////////////////////////////////////////////
    static SoundFile* CreateRead(const std::string& Filename);

    ////////////////////////////////////////////////////////////
    /// Create a new sound from a file in memory, for reading
    ///
    /// \param Data :          Pointer to the file data in memory
    /// \param SizeInBytes :   Size of the data to load, in bytes
    /// \param NbSamples :     Number of samples in the file
    /// \param ChannelsCount : Number of channels in the loaded sound
    /// \param SampleRate :    Sample rate of the loaded sound
    ///
    /// \return Pointer to the new sound file (NULL if failed)
    ///
    ////////////////////////////////////////////////////////////
    static SoundFile* CreateRead(const char* Data, std::size_t SizeInBytes);

    ////////////////////////////////////////////////////////////
    /// Create a new sound from a file, for writing
    ///
    /// \param Filename :      Path of sound file
    /// \param ChannelsCount : Number of channels in the sound
    /// \param SampleRate :    Sample rate of the sound
    ///
    /// \return Pointer to the new sound file (NULL if failed)
    ///
    ////////////////////////////////////////////////////////////
    static SoundFile* CreateWrite(const std::string& Filename, unsigned int ChannelsCount, unsigned int SampleRate);

    ////////////////////////////////////////////////////////////
    /// Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~SoundFile();

    ////////////////////////////////////////////////////////////
    /// Get the total number of samples in the file
    ///
    /// \return Number of samples
    ///
    ////////////////////////////////////////////////////////////
    std::size_t GetSamplesCount() const;

    ////////////////////////////////////////////////////////////
    /// Get the number of channels used by the sound
    ///
    /// \return Number of channels (1 = mono, 2 = stereo)
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetChannelsCount() const;

    ////////////////////////////////////////////////////////////
    /// Get the sample rate of the sound
    ///
    /// \return Sample rate, in samples / sec
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetSampleRate() const;

    ////////////////////////////////////////////////////////////
    /// Restart the sound from the beginning
    ///
    /// \return True if restart was successful
    ///
    ////////////////////////////////////////////////////////////
    bool Restart();

    ////////////////////////////////////////////////////////////
    /// Read samples from the loaded sound
    ///
    /// \param Data :      Pointer to the samples array to fill
    /// \param NbSamples : Number of samples to read
    ///
    /// \return Number of samples read
    ///
    ////////////////////////////////////////////////////////////
    virtual std::size_t Read(Int16* Data, std::size_t NbSamples);

    ////////////////////////////////////////////////////////////
    /// Write samples to the file
    ///
    /// \param Data :      Pointer to the samples array to write
    /// \param NbSamples : Number of samples to write
    ///
    ////////////////////////////////////////////////////////////
    virtual void Write(const Int16* Data, std::size_t NbSamples);

protected :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    SoundFile();

private :

    ////////////////////////////////////////////////////////////
    /// Open the sound file for reading
    ///
    /// \param Filename :      Path of sound file to load
    /// \param NbSamples :     Number of samples in the file
    /// \param ChannelsCount : Number of channels in the loaded sound
    /// \param SampleRate :    Sample rate of the loaded sound
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OpenRead(const std::string& Filename, std::size_t& NbSamples, unsigned int& ChannelsCount, unsigned int& SampleRate);

    ////////////////////////////////////////////////////////////
    /// Open the sound file in memory for reading
    ///
    /// \param Data :          Pointer to the file data in memory
    /// \param SizeInBytes :   Size of the data to load, in bytes
    /// \param NbSamples :     Number of samples in the file
    /// \param ChannelsCount : Number of channels in the loaded sound
    /// \param SampleRate :    Sample rate of the loaded sound
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OpenRead(const char* Data, std::size_t SizeInBytes, std::size_t& NbSamples, unsigned int& ChannelsCount, unsigned int& SampleRate);

    ////////////////////////////////////////////////////////////
    /// Open the sound file for writing
    ///
    /// \param Filename :      Path of sound file to write
    /// \param ChannelsCount : Number of channels in the sound
    /// \param SampleRate :    Sample rate of the sound
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OpenWrite(const std::string& Filename, unsigned int ChannelsCount, unsigned int SampleRate);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::size_t  myNbSamples;     ///< Total number of samples in the file
    unsigned int myChannelsCount; ///< Number of channels used by the sound
    unsigned int mySampleRate;    ///< Number of samples per second
    std::string  myFilename;      ///< Path of the file (valid if loaded from a file)
    const char*  myData;          ///< Pointer to the file in memory (valid if loaded from memory)
    std::size_t  mySize;          ///< Size of the file in memory  (valid if loaded from memory)
};

} // namespace priv

} // namespace sf


#endif // SFML_SOUNDFILE_HPP

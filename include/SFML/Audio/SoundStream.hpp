////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/Export.hpp>

#include <SFML/Audio/SoundChannel.hpp>
#include <SFML/Audio/SoundSource.hpp>

#include <SFML/System/Time.hpp>

#include <memory>
#include <vector>

#include <cstddef>
#include <cstdint>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Abstract base class for streamed audio sources
///
////////////////////////////////////////////////////////////
class SFML_AUDIO_API SoundStream : public SoundSource
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Structure defining a chunk of audio data to stream
    ///
    ////////////////////////////////////////////////////////////
    struct Chunk
    {
        const std::int16_t* samples{};     //!< Pointer to the audio samples
        std::size_t         sampleCount{}; //!< Number of samples pointed by Samples
    };

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~SoundStream() override;

    ////////////////////////////////////////////////////////////
    /// \brief Start or resume playing the audio stream
    ///
    /// This function starts the stream if it was stopped, resumes
    /// it if it was paused, and restarts it from the beginning if
    /// it was already playing.
    /// This function uses its own thread so that it doesn't block
    /// the rest of the program while the stream is played.
    ///
    /// \see pause, stop
    ///
    ////////////////////////////////////////////////////////////
    void play() override;

    ////////////////////////////////////////////////////////////
    /// \brief Pause the audio stream
    ///
    /// This function pauses the stream if it was playing,
    /// otherwise (stream already paused or stopped) it has no effect.
    ///
    /// \see play, stop
    ///
    ////////////////////////////////////////////////////////////
    void pause() override;

    ////////////////////////////////////////////////////////////
    /// \brief Stop playing the audio stream
    ///
    /// This function stops the stream if it was playing or paused,
    /// and does nothing if it was already stopped.
    /// It also resets the playing position (unlike pause()).
    ///
    /// \see play, pause
    ///
    ////////////////////////////////////////////////////////////
    void stop() override;

    ////////////////////////////////////////////////////////////
    /// \brief Return the number of channels of the stream
    ///
    /// 1 channel means a mono sound, 2 means stereo, etc.
    ///
    /// \return Number of channels
    ///
    ////////////////////////////////////////////////////////////
    unsigned int getChannelCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the stream sample rate of the stream
    ///
    /// The sample rate is the number of audio samples played per
    /// second. The higher, the better the quality.
    ///
    /// \return Sample rate, in number of samples per second
    ///
    ////////////////////////////////////////////////////////////
    unsigned int getSampleRate() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the map of position in sample frame to sound channel
    ///
    /// This is used to map a sample in the sample stream to a
    /// position during spatialisation.
    ///
    /// \return Map of position in sample frame to sound channel
    ///
    ////////////////////////////////////////////////////////////
    std::vector<SoundChannel> getChannelMap() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current status of the stream (stopped, paused, playing)
    ///
    /// \return Current status
    ///
    ////////////////////////////////////////////////////////////
    Status getStatus() const override;

    ////////////////////////////////////////////////////////////
    /// \brief Change the current playing position of the stream
    ///
    /// The playing position can be changed when the stream is
    /// either paused or playing. Changing the playing position
    /// when the stream is stopped has no effect, since playing
    /// the stream would reset its position.
    ///
    /// \param timeOffset New playing position, from the beginning of the stream
    ///
    /// \see getPlayingOffset
    ///
    ////////////////////////////////////////////////////////////
    void setPlayingOffset(Time timeOffset);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current playing position of the stream
    ///
    /// \return Current playing position, from the beginning of the stream
    ///
    /// \see setPlayingOffset
    ///
    ////////////////////////////////////////////////////////////
    Time getPlayingOffset() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set whether or not the stream should loop after reaching the end
    ///
    /// If set, the stream will restart from beginning after
    /// reaching the end and so on, until it is stopped or
    /// setLoop(false) is called.
    /// The default looping state for streams is false.
    ///
    /// \param loop True to play in loop, false to play once
    ///
    /// \see getLoop
    ///
    ////////////////////////////////////////////////////////////
    void setLoop(bool loop);

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the stream is in loop mode
    ///
    /// \return True if the stream is looping, false otherwise
    ///
    /// \see setLoop
    ///
    ////////////////////////////////////////////////////////////
    bool getLoop() const;

protected:
    enum
    {
        NoLoop = -1 //!< "Invalid" endSeeks value, telling us to continue uninterrupted
    };

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor is only meant to be called by derived classes.
    ///
    ////////////////////////////////////////////////////////////
    SoundStream();

    ////////////////////////////////////////////////////////////
    /// \brief Define the audio stream parameters
    ///
    /// This function must be called by derived classes as soon
    /// as they know the audio settings of the stream to play.
    /// Any attempt to manipulate the stream (play(), ...) before
    /// calling this function will fail.
    /// It can be called multiple times if the settings of the
    /// audio stream change, but only when the stream is stopped.
    ///
    /// \param channelCount Number of channels of the stream
    /// \param sampleRate   Sample rate, in samples per second
    /// \param channelMap   Map of position in sample frame to sound channel
    ///
    ////////////////////////////////////////////////////////////
    void initialize(unsigned int channelCount, unsigned int sampleRate, const std::vector<SoundChannel>& channelMap);

    ////////////////////////////////////////////////////////////
    /// \brief Request a new chunk of audio samples from the stream source
    ///
    /// This function must be overridden by derived classes to provide
    /// the audio samples to play. It is called continuously by the
    /// streaming loop, in a separate thread.
    /// The source can choose to stop the streaming loop at any time, by
    /// returning false to the caller.
    /// If you return true (i.e. continue streaming) it is important that
    /// the returned array of samples is not empty; this would stop the stream
    /// due to an internal limitation.
    ///
    /// \param data Chunk of data to fill
    ///
    /// \return True to continue playback, false to stop
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] virtual bool onGetData(Chunk& data) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Change the current playing position in the stream source
    ///
    /// This function must be overridden by derived classes to
    /// allow random seeking into the stream source.
    ///
    /// \param timeOffset New playing position, relative to the beginning of the stream
    ///
    ////////////////////////////////////////////////////////////
    virtual void onSeek(Time timeOffset) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Change the current playing position in the stream source to the beginning of the loop
    ///
    /// This function can be overridden by derived classes to
    /// allow implementation of custom loop points. Otherwise,
    /// it just calls onSeek(Time::Zero) and returns 0.
    ///
    /// \return The seek position after looping (or -1 if there's no loop)
    ///
    ////////////////////////////////////////////////////////////
    virtual std::int64_t onLoop();

private:
    ////////////////////////////////////////////////////////////
    /// \brief Get the sound object
    ///
    /// \return The sound object
    ///
    ////////////////////////////////////////////////////////////
    void* getSound() const override;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    struct Impl;
    const std::unique_ptr<Impl> m_impl; //!< Implementation details
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::SoundStream
/// \ingroup audio
///
/// Unlike audio buffers (see sf::SoundBuffer), audio streams
/// are never completely loaded in memory. Instead, the audio
/// data is acquired continuously while the stream is playing.
/// This behavior allows to play a sound with no loading delay,
/// and keeps the memory consumption very low.
///
/// Sound sources that need to be streamed are usually big files
/// (compressed audio musics that would eat hundreds of MB in memory)
/// or files that would take a lot of time to be received
/// (sounds played over the network).
///
/// sf::SoundStream is a base class that doesn't care about the
/// stream source, which is left to the derived class. SFML provides
/// a built-in specialization for big files (see sf::Music).
/// No network stream source is provided, but you can write your own
/// by combining this class with the network module.
///
/// A derived class has to override two virtual functions:
/// \li onGetData fills a new chunk of audio data to be played
/// \li onSeek changes the current playing position in the source
///
/// It is important to note that each SoundStream is played in its
/// own separate thread, so that the streaming loop doesn't block the
/// rest of the program. In particular, the OnGetData and OnSeek
/// virtual functions may sometimes be called from this separate thread.
/// It is important to keep this in mind, because you may have to take
/// care of synchronization issues if you share data between threads.
///
/// Usage example:
/// \code
/// class CustomStream : public sf::SoundStream
/// {
/// public:
///
///     [[nodiscard]] bool open(const std::string& location)
///     {
///         // Open the source and get audio settings
///         ...
///         unsigned int channelCount = ...;
///         unsigned int sampleRate = ...;
///
///         // Initialize the stream -- important!
///         initialize(channelCount, sampleRate);
///         return true;
///     }
///
/// private:
///
///     bool onGetData(Chunk& data) override
///     {
///         // Fill the chunk with audio data from the stream source
///         // (note: must not be empty if you want to continue playing)
///         data.samples = ...;
///
///         // Return true to continue playing
///         data.sampleCount = ...;
///         return true;
///     }
///
///     void onSeek(sf::Time timeOffset) override
///     {
///         // Change the current position in the stream source
///         ...
///     }
/// };
///
/// // Usage
/// CustomStream stream;
/// stream.open("path/to/stream");
/// stream.play();
/// \endcode
///
/// \see sf::Music
///
////////////////////////////////////////////////////////////

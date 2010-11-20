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

#ifndef SFML_SOUNDSTREAM_HPP
#define SFML_SOUNDSTREAM_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundSource.hpp>
#include <SFML/System/Thread.hpp>
#include <cstdlib>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Abstract base class for streamed audio sources
///
////////////////////////////////////////////////////////////
class SFML_API SoundStream : public SoundSource
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Structure defining a chunk of audio data to stream
    ///
    ////////////////////////////////////////////////////////////
    struct Chunk
    {
        const Int16* Samples;   ///< Pointer to the audio samples
        std::size_t  NbSamples; ///< Number of samples pointed by Samples
    };

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~SoundStream();

    ////////////////////////////////////////////////////////////
    /// \brief Start or resume playing the audio stream
    ///
    /// This function starts the stream if it was stopped, resumes
    /// it if it was paused, and restarts it from beginning if it
    /// was it already playing.
    /// This function uses its own thread so that it doesn't block
    /// the rest of the program while the stream is played.
    ///
    /// \see Pause, Stop
    ///
    ////////////////////////////////////////////////////////////
    void Play();

    ////////////////////////////////////////////////////////////
    /// \brief Pause the audio stream
    ///
    /// This function pauses the stream if it was playing,
    /// otherwise (stream already paused or stopped) it has no effect.
    ///
    /// \see Play, Stop
    ///
    ////////////////////////////////////////////////////////////
    void Pause();

    ////////////////////////////////////////////////////////////
    /// \brief Stop playing the audio stream
    ///
    /// This function stops the stream if it was playing or paused,
    /// and does nothing if it was already stopped.
    /// It also resets the playing position (unlike Pause()).
    ///
    /// \see Play, Pause
    ///
    ////////////////////////////////////////////////////////////
    void Stop();

    ////////////////////////////////////////////////////////////
    /// \brief Return the number of channels of the stream
    ///
    /// 1 channel means a mono sound, 2 means stereo, etc.
    ///
    /// \return Number of channels
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetChannelsCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the stream sample rate of the stream
    ///
    /// The sample rate is the number of audio samples played per
    /// second. The higher, the better the quality.
    ///
    /// \return Sample rate, in number of samples per second
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetSampleRate() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the current status of the stream (stopped, paused, playing)
    ///
    /// \return Current status
    ///
    ////////////////////////////////////////////////////////////
    Status GetStatus() const;

    ////////////////////////////////////////////////////////////
    /// \brief Change the current playing position of the stream
    ///
    /// The playing position can be changed when the stream is
    /// either paused or playing.
    ///
    /// \param timeOffset New playing position, in seconds
    ///
    /// \see GetPlayingOffset
    ///
    ////////////////////////////////////////////////////////////
    void SetPlayingOffset(float timeOffset);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current playing position of the stream
    ///
    /// \return Current playing position, in seconds
    ///
    /// \see SetPlayingOffset
    ///
    ////////////////////////////////////////////////////////////
    float GetPlayingOffset() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set whether or not the stream should loop after reaching the end
    ///
    /// If set, the stream will restart from beginning after
    /// reaching the end and so on, until it is stopped or
    /// SetLoop(false) is called.
    /// The default looping state for streams is false.
    ///
    /// \param loop True to play in loop, false to play once
    ///
    /// \see GetLoop
    ///
    ////////////////////////////////////////////////////////////
    void SetLoop(bool loop);

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the stream is in loop mode
    ///
    /// \return True if the stream is looping, false otherwise
    ///
    /// \see SetLoop
    ///
    ////////////////////////////////////////////////////////////
    bool GetLoop() const;

protected :

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
    /// Any attempt to manipulate the stream (Play(), ...) before
    /// calling this function will fail.
    /// It can be called multiple times if the settings of the
    /// audio stream change, but only when the stream is stopped.
    ///
    /// \param channelsCount Number of channels of the stream
    /// \param sampleRate    Sample rate, in samples per second
    ///
    ////////////////////////////////////////////////////////////
    void Initialize(unsigned int channelsCount, unsigned int sampleRate);

private :

    ////////////////////////////////////////////////////////////
    /// \brief Function called as the entry point of the thread
    ///
    /// This function starts the streaming loop, and returns
    /// only when the sound is stopped.
    ///
    ////////////////////////////////////////////////////////////
    void Stream();

    ////////////////////////////////////////////////////////////
    /// \brief Request a new chunk of audio samples from the stream source
    ///
    /// This function must be overriden by derived classes to provide
    /// the audio samples to play. It is called continuously by the
    /// streaming loop, in a separate thread.
    /// The source can choose to stop the streaming loop at any time, by
    /// returning false to the caller.
    ///
    /// \param data Chunk of data to fill
    ///
    /// \return True to continue playback, false to stop
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OnGetData(Chunk& data) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Change the current playing position in the stream source
    ///
    /// This function must be overriden by derived classes to
    /// allow random seeking into the stream source.
    ///
    /// \param timeOffset New playing position, in seconds
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnSeek(float timeOffset) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Fill a new buffer with audio samples, and append
    ///        it to the playing queue
    ///
    /// This function is called as soon as a buffer has been fully
    /// consumed; it fills it again and inserts it back into the
    /// playing queue.
    ///
    /// \param buffer Number of the buffer to fill (in [0, BuffersCount])
    ///
    /// \return True if the stream source has requested to stop, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool FillAndPushBuffer(unsigned int bufferNum);

    ////////////////////////////////////////////////////////////
    /// \brief Fill the audio buffers and put them all into the playing queue
    ///
    /// This function is called when playing starts and the
    /// playing queue is empty.
    ///
    /// \return True if the derived class has requested to stop, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool FillQueue();

    ////////////////////////////////////////////////////////////
    /// \brief Clear all the audio buffers and empty the playing queue
    ///
    /// This function is called when the stream is stopped.
    ///
    ////////////////////////////////////////////////////////////
    void ClearQueue();

    enum
    {
        BuffersCount = 3 ///< Number of audio buffers used by the streaming loop
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Thread        myThread;                   ///< Thread running the background tasks
    bool          myIsStreaming;              ///< Streaming state (true = playing, false = stopped)
    unsigned int  myBuffers[BuffersCount];    ///< Sound buffers used to store temporary audio data
    unsigned int  myChannelsCount;            ///< Number of channels (1 = mono, 2 = stereo, ...)
    unsigned int  mySampleRate;               ///< Frequency (samples / second)
    unsigned long myFormat;                   ///< Format of the internal sound buffers
    bool          myLoop;                     ///< Loop flag (true to loop, false to play once)
    unsigned int  mySamplesProcessed;         ///< Number of buffers processed since beginning of the stream
    bool          myEndBuffers[BuffersCount]; ///< Each buffer is marked as "end buffer" or not, for proper duration calculation
};

} // namespace sf


#endif // SFML_SOUNDSTREAM_HPP


////////////////////////////////////////////////////////////
/// \class sf::SoundStream
/// \ingroup audio
///
/// Unlike audio buffers (see sf::SoundBuffer), audio streams
/// are never completely loaded in memory. Instead, the audio
/// data is acquired continuously while the stream is playing.
/// This behaviour allows to play a sound with no loading delay,
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
/// \li OnGetData fills a new chunk of audio data to be played
/// \li OnSeek changes the current playing position in the source
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
/// public :
///
///     bool Open(const std::string& location)
///     {
///         // Open the source and get audio settings
///         ...
///         unsigned int channelsCount = ...;
///         unsigned int sampleRate = ...;
///
///         // Initialize the stream -- important!
///         Initialize(channelsCount, sampleRate);
///     }
///
/// private :
///
///     virtual bool OnGetData(Chunk& data)
///     {
///         // Fill the chunk with audio data from the stream source
///         data.Samples = ...;
///         data.NbSamples = ...;
///
///         // Return true to continue playing
///         return true;
///     }
///
///     virtual void OnSeek(float timeOffset)
///     {
///         // Change the current position in the stream source
///         ...
///     }
/// }
///
/// // Usage
/// CustomStream stream;
/// stream.Open("path/to/stream");
/// stream.Play();
/// \endcode
///
/// \see sf::Music
///
////////////////////////////////////////////////////////////

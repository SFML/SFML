////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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

#include <SFML/Audio/SoundStream.hpp>

#include <filesystem>
#include <memory>
#include <optional>

#include <cstddef>
#include <cstdint>


namespace sf
{
class Time;
class InputStream;
class InputSoundFile;

////////////////////////////////////////////////////////////
/// \brief Streamed music played from an audio file
///
////////////////////////////////////////////////////////////
class SFML_AUDIO_API Music : public SoundStream
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Structure defining a time range using the template type
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    struct Span
    {
        T offset{}; //!< The beginning offset of the time range
        T length{}; //!< The length of the time range
    };

    // Associated `Span` type
    using TimeSpan = Span<Time>;

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Construct an empty music that does not contain any data.
    ///
    ////////////////////////////////////////////////////////////
    Music();

    ////////////////////////////////////////////////////////////
    /// \brief Construct a music from an audio file
    ///
    /// This function doesn't start playing the music (call `play()`
    /// to do so).
    /// See the documentation of `sf::InputSoundFile` for the list
    /// of supported formats.
    ///
    /// \warning Since the music is not loaded at once but rather
    /// streamed continuously, the file must remain accessible until
    /// the `sf::Music` object loads a new music or is destroyed.
    ///
    /// \param filename Path of the music file to open
    ///
    /// \throws sf::Exception if loading was unsuccessful
    ///
    /// \see `openFromMemory`, `openFromStream`
    ///
    ////////////////////////////////////////////////////////////
    explicit Music(const std::filesystem::path& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Construct a music from an audio file in memory
    ///
    /// This function doesn't start playing the music (call `play()`
    /// to do so).
    /// See the documentation of `sf::InputSoundFile` for the list
    /// of supported formats.
    ///
    /// \warning Since the music is not loaded at once but rather streamed
    /// continuously, the \a data buffer must remain accessible until
    /// the `sf::Music` object loads a new music or is destroyed. That is,
    /// you can't deallocate the buffer right after calling this function.
    ///
    /// \param data        Pointer to the file data in memory
    /// \param sizeInBytes Size of the data to load, in bytes
    ///
    /// \throws sf::Exception if loading was unsuccessful
    ///
    /// \see `openFromFile`, `openFromStream`
    ///
    ////////////////////////////////////////////////////////////
    Music(const void* data, std::size_t sizeInBytes);

    ////////////////////////////////////////////////////////////
    /// \brief Construct a music from an audio file in a custom stream
    ///
    /// This function doesn't start playing the music (call `play()`
    /// to do so).
    /// See the documentation of `sf::InputSoundFile` for the list
    /// of supported formats.
    ///
    /// \warning Since the music is not loaded at once but rather
    /// streamed continuously, the `stream` must remain accessible
    /// until the `sf::Music` object loads a new music or is destroyed.
    ///
    /// \param stream Source stream to read from
    ///
    /// \throws sf::Exception if loading was unsuccessful
    ///
    /// \see `openFromFile`, `openFromMemory`
    ///
    ////////////////////////////////////////////////////////////
    explicit Music(InputStream& stream);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~Music() override;

    ////////////////////////////////////////////////////////////
    /// \brief Move constructor
    ///
    ////////////////////////////////////////////////////////////
    Music(Music&&) noexcept;

    ////////////////////////////////////////////////////////////
    /// \brief Move assignment
    ///
    ////////////////////////////////////////////////////////////
    Music& operator=(Music&&) noexcept;

    ////////////////////////////////////////////////////////////
    /// \brief Open a music from an audio file
    ///
    /// This function doesn't start playing the music (call `play()`
    /// to do so).
    /// See the documentation of `sf::InputSoundFile` for the list
    /// of supported formats.
    ///
    /// \warning Since the music is not loaded at once but rather
    /// streamed continuously, the file must remain accessible until
    /// the `sf::Music` object loads a new music or is destroyed.
    ///
    /// \param filename Path of the music file to open
    ///
    /// \return `true` if loading succeeded, `false` if it failed
    ///
    /// \see `openFromMemory`, `openFromStream`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool openFromFile(const std::filesystem::path& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Open a music from an audio file in memory
    ///
    /// This function doesn't start playing the music (call `play()`
    /// to do so).
    /// See the documentation of `sf::InputSoundFile` for the list
    /// of supported formats.
    ///
    /// \warning Since the music is not loaded at once but rather streamed
    /// continuously, the `data` buffer must remain accessible until
    /// the `sf::Music` object loads a new music or is destroyed. That is,
    /// you can't deallocate the buffer right after calling this function.
    ///
    /// \param data        Pointer to the file data in memory
    /// \param sizeInBytes Size of the data to load, in bytes
    ///
    /// \return `true` if loading succeeded, `false` if it failed
    ///
    /// \see `openFromFile`, `openFromStream`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool openFromMemory(const void* data, std::size_t sizeInBytes);

    ////////////////////////////////////////////////////////////
    /// \brief Open a music from an audio file in a custom stream
    ///
    /// This function doesn't start playing the music (call `play()`
    /// to do so).
    /// See the documentation of `sf::InputSoundFile` for the list
    /// of supported formats.
    ///
    /// \warning Since the music is not loaded at once but rather
    /// streamed continuously, the `stream` must remain accessible
    /// until the `sf::Music` object loads a new music or is destroyed.
    ///
    /// \param stream Source stream to read from
    ///
    /// \return `true` if loading succeeded, `false` if it failed
    ///
    /// \see `openFromFile`, `openFromMemory`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool openFromStream(InputStream& stream);

    ////////////////////////////////////////////////////////////
    /// \brief Get the total duration of the music
    ///
    /// \return Music duration
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Time getDuration() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the positions of the of the sound's looping sequence
    ///
    /// \return Loop Time position class.
    ///
    /// \warning Since `setLoopPoints()` performs some adjustments on the
    /// provided values and rounds them to internal samples, a call to
    /// `getLoopPoints()` is not guaranteed to return the same times passed
    /// into a previous call to `setLoopPoints()`. However, it is guaranteed
    /// to return times that will map to the valid internal samples of
    /// this Music if they are later passed to `setLoopPoints()`.
    ///
    /// \see `setLoopPoints`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] TimeSpan getLoopPoints() const;

    ////////////////////////////////////////////////////////////
    /// \brief Sets the beginning and duration of the sound's looping sequence using `sf::Time`
    ///
    /// `setLoopPoints()` allows for specifying the beginning offset and the duration of the loop such that,
    /// when the music is enabled for looping, it will seamlessly seek to the beginning whenever it
    /// encounters the end of the duration. Valid ranges for `timePoints.offset` and `timePoints.length` are
    /// [0, Dur) and (0, Dur-offset] respectively, where Dur is the value returned by `getDuration()`.
    /// Note that the EOF "loop point" from the end to the beginning of the stream is still honored,
    /// in case the caller seeks to a point after the end of the loop range. This function can be
    /// safely called at any point after a stream is opened, and will be applied to a playing sound
    /// without affecting the current playing offset.
    ///
    /// \warning Setting the loop points while the stream's status is Paused
    /// will set its status to Stopped. The playing offset will be unaffected.
    ///
    /// \param timePoints The definition of the loop. Can be any time points within the sound's length
    ///
    /// \see `getLoopPoints`
    ///
    ////////////////////////////////////////////////////////////
    void setLoopPoints(TimeSpan timePoints);

protected:
    ////////////////////////////////////////////////////////////
    /// \brief Request a new chunk of audio samples from the stream source
    ///
    /// This function fills the chunk from the next samples
    /// to read from the audio file.
    ///
    /// \param data Chunk of data to fill
    ///
    /// \return `true` to continue playback, `false` to stop
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool onGetData(Chunk& data) override;

    ////////////////////////////////////////////////////////////
    /// \brief Change the current playing position in the stream source
    ///
    /// \param timeOffset New playing position, from the beginning of the music
    ///
    ////////////////////////////////////////////////////////////
    void onSeek(Time timeOffset) override;

    ////////////////////////////////////////////////////////////
    /// \brief Change the current playing position in the stream source to the loop offset
    ///
    /// This is called by the underlying `SoundStream` whenever it needs us to reset
    /// the seek position for a loop. We then determine whether we are looping on a
    /// loop point or the end-of-file, perform the seek, and return the new position.
    ///
    /// \return The seek position after looping (or `std::nullopt` if there's no loop)
    ///
    ////////////////////////////////////////////////////////////
    std::optional<std::uint64_t> onLoop() override;

private:
    ////////////////////////////////////////////////////////////
    /// \brief Helper to convert an `sf::Time` to a sample position
    ///
    /// \param position Time to convert to samples
    ///
    /// \return The number of samples elapsed at the given time
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::uint64_t timeToSamples(Time position) const;

    ////////////////////////////////////////////////////////////
    /// \brief Helper to convert a sample position to an `sf::Time`
    ///
    /// \param samples Sample count to convert to Time
    ///
    /// \return The Time position of the given sample
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Time samplesToTime(std::uint64_t samples) const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    struct Impl;
    std::unique_ptr<Impl> m_impl; //!< Implementation details
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::Music
/// \ingroup audio
///
/// Musics are sounds that are streamed rather than completely
/// loaded in memory. This is especially useful for compressed
/// musics that usually take hundreds of MB when they are
/// uncompressed: by streaming it instead of loading it entirely,
/// you avoid saturating the memory and have almost no loading delay.
/// This implies that the underlying resource (file, stream or
/// memory buffer) must remain valid for the lifetime of the
/// `sf::Music` object.
///
/// Apart from that, a `sf::Music` has almost the same features as
/// the `sf::SoundBuffer` / `sf::Sound` pair: you can play/pause/stop
/// it, request its parameters (channels, sample rate), change
/// the way it is played (pitch, volume, 3D position, ...), etc.
///
/// As a sound stream, a music is played in its own thread in order
/// not to block the rest of the program. This means that you can
/// leave the music alone after calling `play()`, it will manage itself
/// very well.
///
/// Usage example:
/// \code
/// // Open a music from an audio file
/// sf::Music music("music.ogg");
///
/// // Change some parameters
/// music.setPosition({0, 1, 10}); // change its 3D position
/// music.setPitch(2);             // increase the pitch
/// music.setVolume(50);           // reduce the volume
/// music.setLooping(true);        // make it loop
///
/// // Play it
/// music.play();
/// \endcode
///
/// \see `sf::Sound`, `sf::SoundStream`
///
////////////////////////////////////////////////////////////

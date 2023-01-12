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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/Export.hpp>

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/SoundRecorder.hpp>

#include <vector>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Specialized SoundRecorder which stores the captured
///        audio data into a sound buffer
///
////////////////////////////////////////////////////////////
class SFML_AUDIO_API SoundBufferRecorder : public SoundRecorder
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief destructor
    ///
    ////////////////////////////////////////////////////////////
    ~SoundBufferRecorder() override;

    ////////////////////////////////////////////////////////////
    /// \brief Get the sound buffer containing the captured audio data
    ///
    /// The sound buffer is valid only after the capture has ended.
    /// This function provides a read-only access to the internal
    /// sound buffer, but it can be copied if you need to
    /// make any modification to it.
    ///
    /// \return Read-only access to the sound buffer
    ///
    ////////////////////////////////////////////////////////////
    const SoundBuffer& getBuffer() const;

protected:
    ////////////////////////////////////////////////////////////
    /// \brief Start capturing audio data
    ///
    /// \return True to start the capture, or false to abort it
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool onStart() override;

    ////////////////////////////////////////////////////////////
    /// \brief Process a new chunk of recorded samples
    ///
    /// \param samples     Pointer to the new chunk of recorded samples
    /// \param sampleCount Number of samples pointed by \a samples
    ///
    /// \return True to continue the capture, or false to stop it
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool onProcessSamples(const std::int16_t* samples, std::size_t sampleCount) override;

    ////////////////////////////////////////////////////////////
    /// \brief Stop capturing audio data
    ///
    ////////////////////////////////////////////////////////////
    void onStop() override;

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::vector<std::int16_t> m_samples; //!< Temporary sample buffer to hold the recorded data
    SoundBuffer               m_buffer;  //!< Sound buffer that will contain the recorded data
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::SoundBufferRecorder
/// \ingroup audio
///
/// sf::SoundBufferRecorder allows to access a recorded sound
/// through a sf::SoundBuffer, so that it can be played, saved
/// to a file, etc.
///
/// It has the same simple interface as its base class (start(), stop())
/// and adds a function to retrieve the recorded sound buffer
/// (getBuffer()).
///
/// As usual, don't forget to call the isAvailable() function
/// before using this class (see sf::SoundRecorder for more details
/// about this).
///
/// Usage example:
/// \code
/// if (sf::SoundBufferRecorder::isAvailable())
/// {
///     // Record some audio data
///     sf::SoundBufferRecorder recorder;
///     if (!recorder.start())
///     {
///         // Handle error...
///     }
///     ...
///     recorder.stop();
///
///     // Get the buffer containing the captured audio data
///     const sf::SoundBuffer& buffer = recorder.getBuffer();
///
///     // Save it to a file (for example...)
///     if (!buffer.saveToFile("my_record.ogg"))
///     {
///         // Handle error...
///     }
/// }
/// \endcode
///
/// \see sf::SoundRecorder
///
////////////////////////////////////////////////////////////

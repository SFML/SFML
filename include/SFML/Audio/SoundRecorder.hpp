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

#include <memory>
#include <string>
#include <vector>

#include <cstddef>
#include <cstdint>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Abstract base class for capturing sound data
///
////////////////////////////////////////////////////////////
class SFML_AUDIO_API SoundRecorder
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~SoundRecorder();

    ////////////////////////////////////////////////////////////
    /// \brief Start the capture
    ///
    /// The `sampleRate` parameter defines the number of audio samples
    /// captured per second. The higher, the better the quality
    /// (for example, 44100 samples/sec is CD quality).
    /// This function uses its own thread so that it doesn't block
    /// the rest of the program while the capture runs.
    /// Please note that only one capture can happen at the same time.
    /// You can select which capture device will be used by passing
    /// the name to the `setDevice()` method. If none was selected
    /// before, the default capture device will be used. You can get a
    /// list of the names of all available capture devices by calling
    /// `getAvailableDevices()`.
    ///
    /// \param sampleRate Desired capture rate, in number of samples per second
    ///
    /// \return `true`, if start of capture was successful
    ///
    /// \see `stop`, `getAvailableDevices`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool start(unsigned int sampleRate = 44100);

    ////////////////////////////////////////////////////////////
    /// \brief Stop the capture
    ///
    /// \see `start`
    ///
    ////////////////////////////////////////////////////////////
    void stop();

    ////////////////////////////////////////////////////////////
    /// \brief Get the sample rate
    ///
    /// The sample rate defines the number of audio samples
    /// captured per second. The higher, the better the quality
    /// (for example, 44100 samples/sec is CD quality).
    ///
    /// \return Sample rate, in samples per second
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] unsigned int getSampleRate() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a list of the names of all available audio capture devices
    ///
    /// This function returns a vector of strings, containing
    /// the names of all available audio capture devices.
    ///
    /// \return A vector of strings containing the names
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static std::vector<std::string> getAvailableDevices();

    ////////////////////////////////////////////////////////////
    /// \brief Get the name of the default audio capture device
    ///
    /// This function returns the name of the default audio
    /// capture device. If none is available, an empty string
    /// is returned.
    ///
    /// \return The name of the default audio capture device
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static std::string getDefaultDevice();

    ////////////////////////////////////////////////////////////
    /// \brief Set the audio capture device
    ///
    /// This function sets the audio capture device to the device
    /// with the given `name`. It can be called on the fly (i.e:
    /// while recording). If you do so while recording and
    /// opening the device fails, it stops the recording.
    ///
    /// \param name The name of the audio capture device
    ///
    /// \return `true`, if it was able to set the requested device
    ///
    /// \see `getAvailableDevices`, `getDefaultDevice`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool setDevice(const std::string& name);

    ////////////////////////////////////////////////////////////
    /// \brief Get the name of the current audio capture device
    ///
    /// \return The name of the current audio capture device
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] const std::string& getDevice() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the channel count of the audio capture device
    ///
    /// This method allows you to specify the number of channels
    /// used for recording. Currently only 16-bit mono and
    /// 16-bit stereo are supported.
    ///
    /// \param channelCount Number of channels. Currently only
    ///                     mono (1) and stereo (2) are supported.
    ///
    /// \see `getChannelCount`
    ///
    ////////////////////////////////////////////////////////////
    void setChannelCount(unsigned int channelCount);

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of channels used by this recorder
    ///
    /// Currently only mono and stereo are supported, so the
    /// value is either 1 (for mono) or 2 (for stereo).
    ///
    /// \return Number of channels
    ///
    /// \see `setChannelCount`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] unsigned int getChannelCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the map of position in sample frame to sound channel
    ///
    /// This is used to map a sample in the sample stream to a
    /// position during spatialization.
    ///
    /// \return Map of position in sample frame to sound channel
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] const std::vector<SoundChannel>& getChannelMap() const;

    ////////////////////////////////////////////////////////////
    /// \brief Check if the system supports audio capture
    ///
    /// This function should always be called before using
    /// the audio capture features. If it returns `false`, then
    /// any attempt to use `sf::SoundRecorder` or one of its derived
    /// classes will fail.
    ///
    /// \return `true` if audio capture is supported, `false` otherwise
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static bool isAvailable();

protected:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor is only meant to be called by derived classes.
    ///
    ////////////////////////////////////////////////////////////
    SoundRecorder();

    ////////////////////////////////////////////////////////////
    /// \brief Start capturing audio data
    ///
    /// This virtual function may be overridden by a derived class
    /// if something has to be done every time a new capture
    /// starts. If not, this function can be ignored; the default
    /// implementation does nothing.
    ///
    /// \return `true` to start the capture, or `false` to abort it
    ///
    ////////////////////////////////////////////////////////////
    virtual bool onStart();

    ////////////////////////////////////////////////////////////
    /// \brief Process a new chunk of recorded samples
    ///
    /// This virtual function is called every time a new chunk of
    /// recorded data is available. The derived class can then do
    /// whatever it wants with it (storing it, playing it, sending
    /// it over the network, etc.).
    ///
    /// \param samples     Pointer to the new chunk of recorded samples
    /// \param sampleCount Number of samples pointed by `samples`
    ///
    /// \return `true` to continue the capture, or `false` to stop it
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] virtual bool onProcessSamples(const std::int16_t* samples, std::size_t sampleCount) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Stop capturing audio data
    ///
    /// This virtual function may be overridden by a derived class
    /// if something has to be done every time the capture
    /// ends. If not, this function can be ignored; the default
    /// implementation does nothing.
    ///
    ////////////////////////////////////////////////////////////
    virtual void onStop();

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    struct Impl;
    const std::unique_ptr<Impl> m_impl; //!< Implementation details
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::SoundRecorder
/// \ingroup audio
///
/// `sf::SoundBuffer` provides a simple interface to access
/// the audio recording capabilities of the computer
/// (the microphone). As an abstract base class, it only cares
/// about capturing sound samples, the task of making something
/// useful with them is left to the derived class. Note that
/// SFML provides a built-in specialization for saving the
/// captured data to a sound buffer (see `sf::SoundBufferRecorder`).
///
/// A derived class has only one virtual function to override:
/// \li `onProcessSamples` provides the new chunks of audio samples while the capture happens
///
/// Moreover, two additional virtual functions can be overridden
/// as well if necessary:
/// \li `onStart` is called before the capture happens, to perform custom initializations
/// \li `onStop` is called after the capture ends, to perform custom cleanup
///
/// The audio capture feature may not be supported or activated
/// on every platform, thus it is recommended to check its
/// availability with the isAvailable() function. If it returns
/// `false`, then any attempt to use an audio recorder will fail.
///
/// If you have multiple sound input devices connected to your
/// computer (for example: microphone, external sound card, webcam mic, ...)
/// you can get a list of all available devices through the
/// `getAvailableDevices()` function. You can then select a device
/// by calling `setDevice()` with the appropriate device. Otherwise
/// the default capturing device will be used.
///
/// By default the recording is in 16-bit mono. Using the
/// setChannelCount method you can change the number of channels
/// used by the audio capture device to record. Note that you
/// have to decide whether you want to record in mono or stereo
/// before starting the recording.
///
/// It is important to note that the audio capture happens in a
/// separate thread, so that it doesn't block the rest of the
/// program. In particular, the `onProcessSamples` virtual function
/// (but not `onStart` and not `onStop`) will be called
/// from this separate thread. It is important to keep this in
/// mind, because you may have to take care of synchronization
/// issues if you share data between threads.
/// Another thing to bear in mind is that you must call `stop()`
/// in the destructor of your derived class, so that the recording
/// thread finishes before your object is destroyed.
///
/// Usage example:
/// \code
/// class CustomRecorder : public sf::SoundRecorder
/// {
/// public:
///     ~CustomRecorder()
///     {
///         // Make sure to stop the recording thread
///         stop();
///     }
///
/// private:
///     bool onStart() override // optional
///     {
///         // Initialize whatever has to be done before the capture starts
///         ...
///
///         // Return true to start playing
///         return true;
///     }
///
///     [[nodiscard]] bool onProcessSamples(const std::int16_t* samples, std::size_t sampleCount) override
///     {
///         // Do something with the new chunk of samples (store them, send them, ...)
///         ...
///
///         // Return true to continue playing
///         return true;
///     }
///
///     void onStop() override // optional
///     {
///         // Clean up whatever has to be done after the capture ends
///         ...
///     }
/// };
///
/// // Usage
/// if (CustomRecorder::isAvailable())
/// {
///     CustomRecorder recorder;
///
///     if (!recorder.start())
///         return -1;
///
///     ...
///     recorder.stop();
/// }
/// \endcode
///
/// \see `sf::SoundBufferRecorder`
///
////////////////////////////////////////////////////////////
